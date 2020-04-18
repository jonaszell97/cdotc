
#include "cdotc/Sema/Template.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Expression.h"
#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Message/Diagnostics.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"

using std::string;

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::ast;

namespace cdot {
namespace sema {

TemplateArgument::TemplateArgument(TemplateParamDecl* Param, QualType type,
                                   SourceLocation loc) noexcept
    : IsType(true), IsVariadic(false), IsNull(false),
      Dependent(type->isDependentType() || type->containsAssociatedType()
                || type->containsTypeVariable()
                || type->containsTemplateParamType()
                || type->containsProtocolWithAssociatedTypes()),
      Frozen(false), Runtime(false),
      ManuallySpecifiedVariadicArgs(0), Param(Param),
      Type(type->getCanonicalType()), Loc(loc)
{
}

TemplateArgument::TemplateArgument(TemplateParamDecl* Param, StaticExpr* Expr,
                                   SourceLocation loc) noexcept
    : IsType(false), IsVariadic(false), IsNull(false),
      Dependent(Expr && Expr->isDependent()), Frozen(false), Runtime(false),
      ManuallySpecifiedVariadicArgs(0), Param(Param), Expr(Expr), Loc(loc)
{
}

TemplateArgument::TemplateArgument(TemplateParamDecl* Param, bool isType,
                                   std::vector<TemplateArgument>&& variadicArgs,
                                   SourceLocation loc)
    : IsType(isType), IsVariadic(true), IsNull(false), Dependent(false),
      Frozen(false), Runtime(false), ManuallySpecifiedVariadicArgs(0),
      Param(Param), VariadicArgs(move(variadicArgs)), Loc(loc)
{
   for (auto& VA : this->VariadicArgs) {
      if (VA.Dependent) {
         Dependent = true;
         break;
      }
   }
}

TemplateArgument::TemplateArgument(TemplateArgument&& other) noexcept
    : IsType(other.IsType), IsVariadic(other.IsVariadic), IsNull(other.IsNull),
      Dependent(other.Dependent), Frozen(false), Runtime(other.Runtime),
      ManuallySpecifiedVariadicArgs(other.ManuallySpecifiedVariadicArgs),
      Param(other.Param), Loc(other.Loc)
{
   if (IsVariadic) {
      new (&VariadicArgs)
          std::vector<TemplateArgument>(move(other.VariadicArgs));
   }
   else if (IsType) {
      Type = other.Type;
   }
   else {
      Expr = other.Expr;
   }
}

TemplateArgument::~TemplateArgument() { destroyValue(); }

TemplateArgument& TemplateArgument::operator=(TemplateArgument&& other) noexcept
{
   destroyValue();
   new (this) TemplateArgument(move(other));
   return *this;
}

void TemplateArgument::destroyValue()
{
   if (IsNull)
      return;

   if (IsVariadic) {
      for (auto& VA : VariadicArgs)
         VA.destroyValue();

      VariadicArgs.~vector();
   }
}

QualType TemplateArgument::getValueType() const
{
   assert(isValue());
   return Param->getValueType();
}

il::Constant* TemplateArgument::getValue() const
{
   assert(isValue() && "not a value template argument");
   return Expr ? Expr->getEvaluatedExpr() : nullptr;
}

void TemplateArgument::freeze()
{
   assert(isVariadic());
   Frozen = true;
   ManuallySpecifiedVariadicArgs = (unsigned)VariadicArgs.size();
}

bool TemplateArgument::isStillDependent() const { return Dependent; }

TemplateArgument TemplateArgument::clone(bool Canonicalize, bool Freeze,
                                         ast::TemplateParamDecl* P) const
{
   if (!P)
      P = getParam();

   TemplateArgument Result;
   if (isNull()) {
      Result = TemplateArgument();
   }
   else if (isVariadic()) {
      std::vector<TemplateArgument> args;
      for (auto& VA : getVariadicArgs())
         args.emplace_back(VA.clone(Canonicalize));

      Result = TemplateArgument(P, isType(), move(args), getLoc());
   }
   else if (isType()) {
      Result = TemplateArgument(
          P, Canonicalize ? getType() : getNonCanonicalType(), getLoc());
   }
   else {
      Result = TemplateArgument(P, getValueExpr(), getLoc());
   }

   if (Freeze && Result.isVariadic())
      Result.freeze();

   Result.Runtime = Runtime;
   return Result;
}

bool TemplateArgument::operator==(const TemplateArgument& RHS) const
{
   if (isVariadic()) {
      if (!RHS.isVariadic()) {
         return false;
      }

      unsigned N1 = VariadicArgs.size();
      unsigned N2 = RHS.VariadicArgs.size();

      if (N1 != N2) {
         return false;
      }

      for (unsigned i = 0; i < N1; ++i) {
         if (VariadicArgs[i] != RHS.VariadicArgs[i]) {
            return false;
         }
      }

      return true;
   }

   if (isType()) {
      if (!RHS.isType()) {
         return false;
      }

      return getType() == RHS.getType();
   }

   return getValueExpr() == RHS.getValueExpr();
}

std::string TemplateArgument::toString() const
{
   if (isNull()) {
      return "<null>";
   }

   if (isVariadic()) {
      string s = "(";
      size_t i = 0;

      for (auto& VA : VariadicArgs) {
         if (i != 0)
            s += ", ";
         s += VA.toString();

         ++i;
      }

      s += ")";
      return s;
   }

   if (isType()) {
      return getNonCanonicalType()->toString();
   }

   std::string str;
   llvm::raw_string_ostream OS(str);

   OS << *getValue();
   return OS.str();
}

void TemplateArgument::Profile(llvm::FoldingSetNodeID& ID,
                               bool Canonicalize) const
{
   uint8_t Flags = IsType | (IsVariadic << 1) | (IsNull << 2);

   ID.AddInteger(Flags);
   ID.AddPointer(getParam());

   if (isNull()) {
      return;
   }

   if (isVariadic()) {
      ID.AddInteger(getVariadicArgs().size());
      for (auto& VA : getVariadicArgs()) {
         VA.Profile(ID, Canonicalize);
      }
   }
   else if (isType()) {
      if (Canonicalize) {
         ID.AddPointer(getType().getAsOpaquePtr());
      }
      else {
         ID.AddPointer(getNonCanonicalType().getAsOpaquePtr());
      }
   }
   else {
      ID.AddPointer(getValue());
   }
}

class TemplateArgListImpl {
public:
   TemplateArgListImpl(SemaPass& SP, NamedDecl* Template,
                       llvm::ArrayRef<Expression*> templateArgs,
                       SourceLocation loc)
       : SP(SP), ListLoc(loc), Template(Template), StillDependent(false),
         HasRuntimeParam(false), HadError(false),
         FullyInferred(templateArgs.empty()), PartiallyInferred(false)
   {
      doInitialFill();

      if (!templateArgs.empty()) {
         resolveWithParameters(templateArgs);
      }
      else {
         inferFromContext();
      }
   }

   TemplateArgListImpl(SemaPass& SP, llvm::ArrayRef<Expression*> templateArgs,
                       SourceLocation loc)
       : SP(SP), ListLoc(loc), StillDependent(false), HasRuntimeParam(false),
         HadError(false), FullyInferred(templateArgs.empty()),
         PartiallyInferred(false)
   {
   }

   void doInitialFill()
   {
      auto Params = getParameters();
      ResolvedArgs.resize(Params.size());

      size_t idx = 0;
      for (auto& Param : Params) {
         if (Param->isVariadic()) {
            new (&ResolvedArgs[idx])
                TemplateArgument(Param, Param->isTypeName(), {});
         }
         else {
            ResolvedArgs[idx].Param = Param;
         }

         ++idx;
      }
   }

   void resolveWithParameters(llvm::ArrayRef<Expression*> OriginalArgs)
   {
      size_t i = 0;
      auto parameters = getParameters();
      bool variadic = false;

      for (auto* E : OriginalArgs) {
         if (E->isDependent()) {
            StillDependent = true;
            break;
         }
      }

      for (auto& P : parameters) {
         if (i >= OriginalArgs.size())
            break;

         if (P->isVariadic()) {
            std::vector<TemplateArgument> variadicArgs;

            while (i < OriginalArgs.size()) {
               auto& TA = OriginalArgs[i];
               auto& Out = variadicArgs.emplace_back();

               if (!makeSingleArgument(P, Out, TA)) {
                  variadicArgs.pop_back();
                  return;
               }

               ++i;
            }

            variadic = true;

            auto numVariadics = (unsigned)variadicArgs.size();
            SourceLocation loc = numVariadics ? variadicArgs.front().getLoc()
                                              : P->getSourceLoc();

            auto Idx = getIndexFor(P);
            auto& VA = ResolvedArgs[Idx];

            assert(VA.isVariadic() && "incorrect initial template argument");
            assert(VA.getVariadicArgs().empty() && "duplicate variadic arg");

            VA.VariadicArgs = move(variadicArgs);
            VA.Loc = loc;
            VA.freeze();

            break;
         }

         auto& TA = OriginalArgs[i];
         if (!makeSingleArgument(P, ResolvedArgs[i], TA))
            return;

         ++i;
      }

      if (OriginalArgs.size() > parameters.size() && !variadic) {
         return Res.setHasTooManyTemplateArgs(OriginalArgs.size(),
                                              parameters.size());
      }

      if (parameters.size() == ResolvedArgs.size())
         return;

      for (auto& P : parameters) {
         if (auto V = P->getDefaultValue()) {
            if (getIndexFor(P) == string::npos) {
               if (P->isTypeName()) {
                  emplace(P, P, V->getExprType(), P->getSourceLoc());
               }
               else {
                  emplace(P, P, cast<StaticExpr>(V), P->getSourceLoc());
               }
            }
         }
      }
   }

   void checkCovariance(TemplateParamDecl* P, QualType Ty)
   {
      if (SP.QC.PrepareDeclInterface(P))
         return;

      bool IsCovariant;
      if (SP.QC.IsCovariant(IsCovariant, Ty, P->getCovariance()) || IsCovariant)
         return;

      Res.setCovarianceError(Ty, P);
   }

   TemplateParamDecl* getRuntimeParameter(Expression* E)
   {
      if (E->isDependent())
         return nullptr;

      auto* Ident = dyn_cast<IdentifierRefExpr>(E);
      if (Ident && Ident->getKind() == IdentifierKind::TemplateParam)
         return Ident->getTemplateParam();

      return nullptr;
   }

   void checkDependentType(QualType T)
   {
      if (T->isDependentType() || T->containsAssociatedType()
      || T->containsTemplateParamType()) {
         StillDependent = true;
      }
   }

   bool setParamValue(TemplateParamDecl* P, TemplateArgument&& NewVal)
   {
      auto Idx = getIndexFor(P);
      assert(Idx != -1 && "bad template parameter");

      auto& Val = ResolvedArgs[Idx];
      if (!Val.isNull()
          && (!Val.isVariadic() && !Val.isFrozen()
              && Val.getVariadicArgs().empty())) {
         if (Val != NewVal) {
            Res.setHasConflict(Val.getType(), P);
            return true;
         }

         return false;
      }

      if (P->isVariadic()) {
         if (!NewVal.isVariadic()) {
            Res.setHasIncompatibleKind(0, P);
            return true;
         }
      }

      if (P->isTypeName()) {
         if (!NewVal.isType()) {
            Res.setHasIncompatibleKind(0, P);
            return true;
         }

         if (NewVal.isVariadic()) {
            for (auto& VA : NewVal.getVariadicArgs()) {
               checkCovariance(P, VA.getType());
               checkDependentType(VA.getType());
            }
         }
         else {
            checkCovariance(P, NewVal.getType());
            checkDependentType(NewVal.getType());
         }
      }

      Val = move(NewVal);
      return false;
   }

   bool makeSingleArgument(TemplateParamDecl* P, TemplateArgument& Out,
                           Expression* TA)
   {
      auto res = SP.visitExpr(TA);
      if (!res) {
         HadError = true;
         return false;
      }

      //      if (Template->isUnboundedTemplate()
      //            && TA->getExprType()->containsRuntimeGenericParam()) {
      //         SP.diagnose(TA, err_generic_error,
      //                     "cannot use a runtime generic parameter as a "
      //                     "compile-time template parameter",
      //                     TA->getSourceRange());
      //
      //         SP.diagnose(note_template_parameter_here, P->getSourceLoc());
      //         HadError = true;
      //      }

      auto ty = res.get()->getExprType();
      if (isa<TypeExpr>(res.get()) || ty->isErrorType()) {
         if (!P->isTypeName()) {
            Res.setHasIncompatibleKind(0, P);
            Out = TemplateArgument(P, nullptr, TA->getSourceLoc());
            return false;
         }

         checkCovariance(P, ty);
         checkDependentType(ty);

         StillDependent |= ty->isDependentType();

         Out = TemplateArgument(P, ty, TA->getSourceLoc());
      }
      else if (ty->isMetaType()) {
         if (!P->isTypeName()) {
            Res.setHasIncompatibleKind(0, P);
            Out = TemplateArgument(P, nullptr, TA->getSourceLoc());
            return false;
         }

         QualType RealTy = cast<MetaType>(ty)->getUnderlyingType();

         checkCovariance(P, RealTy);
         checkDependentType(RealTy);

         StillDependent |= RealTy->isDependentType();

         Out = TemplateArgument(P, RealTy, TA->getSourceLoc());
      }
      else {
         if (P->isTypeName()) {
            Res.setHasIncompatibleKind(1, P);
            Out = TemplateArgument(P, SP.getContext().getErrorTy(),
                                   TA->getSourceLoc());
            return false;
         }

         auto StatExp = StaticExpr::Create(SP.getContext(), TA);
         auto SemaResult = SP.visitExpr(StatExp);

         if (!SemaResult) {
            HadError = true;
            Out = TemplateArgument(P, nullptr, TA->getSourceLoc());
            return false;
         }

         ty = StatExp->getExprType();

         if (ty->isDependentType()) {
            StillDependent = true;
         }
         else if (StatExp->getExprType() != P->getValueType()) {
            Res.setHasIncompatibleType(StatExp->getExprType(), P);
            Out = TemplateArgument(P, nullptr, TA->getSourceLoc());
            return false;
         }

         StatExp = cast<StaticExpr>(SemaResult.get());
         Out = TemplateArgument(P, StatExp, StatExp->getSourceLoc());
      }

      return true;
   }

   void copyFromList(const TemplateArgList& list)
   {
      auto it = list.begin();
      auto end = list.end();

      auto param_it = getParameters().begin();
      for (; it != end; ++it) {
         if (!it->isNull()) {
            auto& Param = *param_it++;
            emplace(Param, it->clone());
         }
      }
   }

   void copyFromList(const FinalTemplateArgumentList& list)
   {
      auto it = list.begin();
      auto end = list.end();

      auto param_it = getParameters().begin();
      for (; it != end; ++it) {
         if (!it->isNull()) {
            auto& Param = *param_it++;
            emplace(Param, it->clone());
         }
      }
   }

   TemplateArgument* getNamedArg(DeclarationName Name)
   {
      auto idx = getIndexFor(Name);
      if (idx == string::npos)
         return nullptr;

      return &ResolvedArgs[idx];
   }

   TemplateArgument* getArgForParam(TemplateParamDecl* P)
   {
      auto idx = getIndexFor(P);
      if (idx == string::npos)
         return nullptr;

      return &ResolvedArgs[idx];
   }

   bool inferFromReturnType(QualType contextualType, QualType returnType,
                            bool IsLastVariadicParam);
   void inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                         llvm::ArrayRef<FuncArgDecl*> neededArgs);

   bool checkSingleCompatibility(TemplateArgument& TA,
                                 TemplateParamDecl* const& TP, size_t idx,
                                 bool ignoreVariadic = false);

   void checkCompatibility();

   bool isStillDependent() const { return StillDependent; }

   llvm::ArrayRef<TemplateParamDecl*> getParameters() const
   {
      if (!Template)
         return {};

      return Template->getTemplateParams();
   }

   bool insert(TemplateArgument&& arg)
   {
      auto Param = arg.getParam();
      StillDependent |= arg.isStillDependent();

      return emplace(Param, move(arg)).second;
   }

   size_t getIndexFor(DeclarationName Name)
   {
      size_t idx = 0;
      for (auto& param : getParameters()) {
         if (param->getDeclName() == Name)
            return idx;

         ++idx;
      }

      return string::npos;
   }

   size_t getIndexFor(TemplateParamDecl* Param)
   {
      size_t idx = 0;
      for (auto& param : getParameters()) {
         if (param == Param)
            return idx;

         ++idx;
      }

      return string::npos;
   }

   friend class TemplateArgList;

private:
   SemaPass& SP;
   llvm::SmallVector<TemplateArgument, 0> ResolvedArgs;

   SourceLocation ListLoc;
   NamedDecl* Template = nullptr;

   bool StillDependent : 1;
   bool HasRuntimeParam : 1;
   bool HadError : 1;
   bool FullyInferred : 1;
   bool PartiallyInferred : 1;

   TemplateArgListResult Res;

   bool inferTemplateArg(QualType givenNonCanon, QualType needed);
   bool inferTemplateArg(const FinalTemplateArgumentList& GivenList,
                         ArrayRef<TemplateParamDecl*> NeededParams);

   template<class... Args>
   std::pair<size_t, bool> emplace(TemplateParamDecl* Decl, Args&&... args)
   {
      return emplace(getIndexFor(Decl), std::forward<Args&&>(args)...);
   }

   template<class... Args>
   std::pair<size_t, bool> emplace(size_t idx, Args&&... args)
   {
      if (idx == string::npos || !ResolvedArgs[idx].isNull())
         return {idx, false};

      auto& Arg = ResolvedArgs[idx];
      new (&Arg) TemplateArgument(std::forward<Args&&>(args)...);

      StillDependent |= ResolvedArgs[idx].isStillDependent();
      return {idx, true};
   }

   void fillImplicitTemplateArgs()
   {
      for (auto P : getParameters()) {
         if (P->isVariadic()) {
            emplace(P, P, P->isTypeName(), std::vector<TemplateArgument>(),
                    P->getSourceLoc());
         }
         else if (P->isTypeName()) {
            emplace(P, P, SP.getContext().getTemplateArgType(P),
                    P->getSourceLoc());
         }
         else {
            emplace(P, P, nullptr, P->getSourceLoc());
         }
      }
   }

   void inferFromContext()
   {
      if (!isa<RecordDecl>(Template))
         return;

      // you can refer to a template without any template arguments from
      // within itself
      for (auto* Ctx = &SP.getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
         auto ND = dyn_cast<NamedDecl>(Ctx);
         if (!ND)
            continue;

         if (auto* Ext = dyn_cast<ExtensionDecl>(ND)) {
            if (auto* Rec = Ext->getExtendedRecord())
               ND = Rec;
         }

         if (dyn_cast<NamedDecl>(ND) == Template) {
            fillImplicitTemplateArgs();
            break;
         }

         if (ND->isInstantiation()
             && ND->getSpecializedTemplate() == Template) {
            copyFromList(ND->getTemplateArgs());
         }
      }

      // chekc if we're in a context that allows incomplete template names,
      // e.g. an extension type
      if (SP.allowIncompleteTemplateTypes()) {
         fillImplicitTemplateArgs();
      }
   }

   bool resolveDefault(TemplateArgument& Arg)
   {
      auto Param = Arg.getParam();
      if (SP.QC.PrepareDeclInterface(Param)) {
         return true;
      }

      auto Def = Param->getDefaultValue();
      if (!Def) {
         return false;
      }

      assert(!Param->isVariadic() && "variadics cannot have a default value!");

      if (Param->isTypeName()) {
         Arg = TemplateArgument(Param, Def->getExprType(), Def->getSourceLoc());
      }
      else {
         auto SE = cast<StaticExpr>(Def);
         Arg = TemplateArgument(Arg.getParam(), SE, Def->getSourceLoc());
      }

      return true;
   }
};

bool TemplateArgListImpl::inferFromReturnType(QualType contextualType,
                                              QualType returnType,
                                              bool IsLastVariadicParam)
{
   if (contextualType->isAutoType())
      return true;

   bool Result = inferTemplateArg(contextualType, returnType);

   if (!IsLastVariadicParam)
      return Result;

   // update variadic arguments so we don't infer them again
   for (auto& Arg : ResolvedArgs) {
      if (!Arg.isVariadic())
         continue;

      Arg.freeze();
   }

   return Result;
}

void TemplateArgListImpl::inferFromArgList(
    llvm::ArrayRef<QualType> givenArgs, llvm::ArrayRef<FuncArgDecl*> neededArgs)
{
   if (!neededArgs.empty()) {
      bool variadic = neededArgs.back()->isVariadicArgPackExpansion();
      size_t i = 0;

      FuncArgDecl* Previous = nullptr;
      for (const auto& arg : givenArgs) {
         if (i >= neededArgs.size() && !variadic) {
            break;
         }

         auto& neededArg
             = neededArgs.size() > i ? neededArgs[i] : neededArgs.back();

         if (Previous && Previous != neededArg) {
            // update variadic arguments so we don't infer them again
            for (auto& Arg : ResolvedArgs) {
               if (!Arg.isVariadic())
                  continue;

               Arg.freeze();
            }
         }

         Previous = neededArg;

         auto success = inferTemplateArg(arg, neededArg->getType());
         if (!success)
            return;

         ++i;
      }
   }
}

bool TemplateArgListImpl::inferTemplateArg(QualType givenNonCanon,
                                           QualType neededNonCanon)
{
   if (givenNonCanon->isDependentType()) {
      StillDependent = true;
   }

   CanType given = givenNonCanon;
   CanType needed = neededNonCanon;

   if (isa<ReferenceType>(given) && !needed->isReferenceType()) {
      given = cast<ReferenceType>(given)->getReferencedType();
   }

   if (TemplateParamType* neededGen
       = dyn_cast<TemplateParamType>(neededNonCanon)) {
      auto parameters = getParameters();
      auto idx = getIndexFor(neededGen->getParam());

      if (idx >= parameters.size())
         return true;

      auto& Arg = ResolvedArgs[idx];
      auto Param = Arg.getParam();
      assert(Param->isTypeName()
             && "allowed Value parameter to be used as argument type!");

      if (auto* G = dyn_cast<TemplateParamType>(given)) {
         if (!cast<NamedDecl>(G->getParam()->getDeclContext())
                  ->isUnboundedTemplate()) {
            HasRuntimeParam = true;
         }
      }

      checkCovariance(Param, given);

      if (Arg.isNull()) {
         if (Param->isVariadic()) {
            std::vector<TemplateArgument> variadicArgs;
            variadicArgs.emplace_back(Param, given);

            emplace(Param, Param, true, move(variadicArgs), ListLoc);
         }
         else {
            emplace(Param, Param, given, ListLoc);
         }
      }
      else {
         // invalid argument kind, ignore for now
         if (!Arg.isType()) {
            return false;
         }

         if (Arg.isVariadic()) {
            if (!Arg.Frozen) {
               Arg.emplace_back(Param, given, ListLoc);
            }
            else {
               auto variadicIdx = Arg.VariadicArgs.size()
                                  - Arg.ManuallySpecifiedVariadicArgs;
               auto& ManuallySpecifiedTA = Arg.VariadicArgs[variadicIdx];

               if (ManuallySpecifiedTA.getType() != given) {
                  while (Arg.VariadicArgs.size()
                         > Arg.ManuallySpecifiedVariadicArgs + 1) {
                     Arg.VariadicArgs.pop_back();
                  }

                  Res.setHasConflict(given, Param);
                  return false;
               }

               --Arg.ManuallySpecifiedVariadicArgs;
               if (Arg.ManuallySpecifiedVariadicArgs == 0)
                  Arg.Frozen = false;
            }
         }
         else {
            // ensure that both inferred types are the same
            QualType ty = Arg.getType();
            if (ty->getCanonicalType() != given->getCanonicalType()) {
               Res.setHasConflict(given, Param);
               return false;
            }
         }
      }

      PartiallyInferred = true;
      return true;
   }

   if (needed->isPointerType()) {
      if (!given->isPointerType()) {
         return false;
      }

      return inferTemplateArg(cast<PointerType>(given)->getPointeeType(),
                              cast<PointerType>(needed)->getPointeeType());
   }

   if (needed->isReferenceType()) {
      if (!given->isReferenceType()) {
         return false;
      }

      return inferTemplateArg(cast<ReferenceType>(given)->getReferencedType(),
                              cast<ReferenceType>(needed)->getReferencedType());
   }

   if (needed->isMetaType()) {
      if (!given->isMetaType()) {
         return false;
      }

      return inferTemplateArg(cast<MetaType>(given)->getUnderlyingType(),
                              cast<MetaType>(needed)->getUnderlyingType());
   }

   if (needed->isFunctionType()) {
      if (!given->isFunctionType()) {
         return false;
      }

      auto givenFunc = cast<FunctionType>(given);
      auto neededFunc = cast<FunctionType>(needed);

      QualType givenRet = givenFunc->getReturnType();
      QualType neededRet = neededFunc->getReturnType();

      if (!inferTemplateArg(givenRet, neededRet))
         return false;

      auto neededArgs = neededFunc->getParamTypes();
      auto givenArgs = givenFunc->getParamTypes();

      unsigned i = 0;
      for (auto& NeededTy : neededArgs) {
         bool IsVariadic = false;
         if (auto* TA = NeededTy->asTemplateParamType()) {
            IsVariadic = TA->isVariadic();
         }

         if (IsVariadic) {
            while (i < givenArgs.size()) {
               if (!inferTemplateArg(givenArgs[i++], NeededTy))
                  return false;
            }

            auto* Arg
                = getArgForParam(NeededTy->asTemplateParamType()->getParam());
            Arg->freeze();

            return true;
         }

         if (!inferTemplateArg(givenArgs[i++], NeededTy))
            return false;
      }

      return true;
   }

   if (needed->isTupleType()) {
      if (!given->isTupleType()) {
         return false;
      }

      auto givenTuple = cast<TupleType>(given);
      auto neededTuple = cast<TupleType>(needed);

      auto givenTys = givenTuple->getContainedTypes();
      auto neededTys = neededTuple->getContainedTypes();

      unsigned i = 0;
      for (auto& NeededTy : neededTys) {
         bool IsVariadic = false;
         if (auto* TA = NeededTy->asTemplateParamType()) {
            IsVariadic = TA->isVariadic();
         }

         if (IsVariadic) {
            while (i < givenTys.size()) {
               if (!inferTemplateArg(givenTys[i++], NeededTy))
                  return false;
            }

            auto* Arg
                = getArgForParam(NeededTy->asTemplateParamType()->getParam());
            Arg->freeze();

            return true;
         }

         if (!inferTemplateArg(givenTys[i++], NeededTy))
            return false;
      }

      return true;
   }

   if (needed->isRecordType()) {
      if (!given->isRecordType()) {
         return false;
      }

      if (given->hasTemplateArgs() && needed->hasTemplateArgs()) {
         auto& givenConcrete = given->getTemplateArgs();
         auto& neededConcrete = needed->getTemplateArgs();

         unsigned i = 0;
         for (auto& NeededArg : neededConcrete) {
            if (NeededArg.isValue()) {
               if (NeededArg.isVariadic())
                  break;

               ++i;
               continue;
            }

            QualType NeededTy = NeededArg.getType();

            bool IsVariadic = NeededArg.isVariadic();
            if (IsVariadic) {
               while (i < givenConcrete.size()) {
                  auto& ConcreteArg = givenConcrete[i++];
                  if (!ConcreteArg.isType() || ConcreteArg.isVariadic())
                     return false;

                  if (!inferTemplateArg(ConcreteArg.getType(), NeededTy))
                     return false;
               }

               auto* Arg = getArgForParam(
                   NeededTy->asTemplateParamType()->getParam());
               Arg->freeze();

               return true;
            }

            auto& ConcreteArg = givenConcrete[i++];
            if (!ConcreteArg.isType() || ConcreteArg.isVariadic())
               return false;

            if (!inferTemplateArg(ConcreteArg.getType(), NeededTy))
               return false;
         }

         return true;
      }

      // Otherwise, try to infer from unconstrained template parameters.
      if (given->hasTemplateArgs() && needed->getRecord()->isTemplate()) {
         const auto& givenConcrete = given->getTemplateArgs();
         ArrayRef<TemplateParamDecl*> neededParams
             = needed->getRecord()->getTemplateParams();

         return inferTemplateArg(givenConcrete, neededParams);
      }

      return true;
   }

   if (needed->isArrayType()) {
      if (!given->isArrayType())
         return false;

      ArrayType* givenArr = cast<ArrayType>(given);
      ArrayType* neededArr = cast<ArrayType>(needed);

      if (!inferTemplateArg(givenArr->getElementType(),
                            neededArr->getElementType()))
         return false;

      if (auto Inf = dyn_cast<DependentSizeArrayType>(neededArr)) {
         auto Ident
             = dyn_cast<IdentifierRefExpr>(Inf->getSizeExpr()->getExpr());

         if (!Ident || Ident->getKind() != IdentifierKind::TemplateParam)
            return true;

         auto Param = Ident->getTemplateParam();

         // have to lookup via name because the address might change if an
         // outer record is instantiated
         auto Idx = getIndexFor(Param->getDeclName());
         if (Idx == string::npos)
            return true;

         if (ResolvedArgs[Idx].isNull()) {
            auto& Builder = SP.getILGen().Builder;
            emplace(Idx, Param,
                    Builder.GetConstantInt(Param->getValueType(),
                                           givenArr->getNumElements()));
         }
      }

      return true;
   }

   if (auto* DN = needed->asDependentNameType()) {
      auto* Name = DN->getNameSpec();
      if (Name->getKind() != NestedNameSpecifier::TemplateArgList
          || Name->depth() != 2) {
         return true;
      }

      ArrayRef<TemplateParamDecl*> NeededParams;

      auto* Prev = Name->getPrevious();
      switch (Prev->getKind()) {
      case NestedNameSpecifier::Alias: {
         if (!givenNonCanon->isTypedefType()) {
            return true;
         }

         auto* NeededAlias = Prev->getAlias();
         auto* GivenAlias = givenNonCanon->asTypedefType()->getTypedef();

         if (!GivenAlias->isInstantiation()
             || GivenAlias->getSpecializedTemplate() != NeededAlias) {
            return true;
         }

         NeededParams = NeededAlias->getTemplateParams();
         break;
      }
      case NestedNameSpecifier::Type: {
         if (!givenNonCanon->isRecordType()) {
            return true;
         }

         QualType NeededT = Prev->getType();
         if (!NeededT->isRecordType()) {
            return true;
         }

         auto* NeededRec = NeededT->getRecord();
         auto* GivenRec = givenNonCanon->getRecord();

         if (!GivenRec->isInstantiation()
             || GivenRec->getSpecializedTemplate() != NeededRec) {
            return true;
         }

         NeededParams = NeededRec->getTemplateParams();
         break;
      }
      default:
         return true;
      }

      return inferTemplateArg(*Name->getTemplateArgs(), NeededParams);
   }

   return true;
}

bool TemplateArgListImpl::inferTemplateArg(
    const FinalTemplateArgumentList& GivenList,
    ArrayRef<TemplateParamDecl*> NeededParams)
{
   auto& Context = SP.getContext();

   unsigned i = 0;
   for (auto* NeededArg : NeededParams) {
      if (!NeededArg->isTypeName()) {
         if (NeededArg->isVariadic())
            break;

         ++i;
         continue;
      }

      QualType NeededTy = Context.getTemplateArgType(NeededArg);

      bool IsVariadic = NeededArg->isVariadic();
      if (IsVariadic) {
         while (i < GivenList.size()) {
            auto& ConcreteArg = GivenList[i++];
            if (!ConcreteArg.isType() || ConcreteArg.isVariadic())
               return false;

            if (!inferTemplateArg(ConcreteArg.getType(), NeededTy))
               return false;
         }

         auto* Arg = getArgForParam(NeededArg);
         Arg->freeze();

         return true;
      }

      auto& ConcreteArg = GivenList[i++];
      if (!ConcreteArg.isType() || ConcreteArg.isVariadic())
         return false;

      if (!inferTemplateArg(ConcreteArg.getType(), NeededTy))
         return false;
   }

   return true;
}

bool MultiLevelTemplateArgList::setParamValue(TemplateParamDecl* Param,
                                              TemplateArgument&& Arg)
{
   for (auto& list : *this) {
      if (list->getArgForParam(Param)) {
         return list->setParamValue(Param, move(Arg));
      }
   }

   return false;
}

bool MultiLevelTemplateArgList::inferFromType(QualType contextualType,
                                              QualType returnType,
                                              bool IsLastVariadicParam)
{
   for (auto& list : *this) {
      if (!list->inferFromType(contextualType, returnType, IsLastVariadicParam))
         return false;
   }

   return true;
}

bool TemplateArgListImpl::checkSingleCompatibility(TemplateArgument& TA,
                                                   TemplateParamDecl* const& P,
                                                   size_t idx,
                                                   bool ignoreVariadic)
{
   enum DiagSelectIndex : unsigned {
      Type = 0,
      Value = 1,
      VariadicType = 2,
      VariadicValue = 3,
   };

   if (P->isVariadic() && !ignoreVariadic) {
      if (!TA.isVariadic()) {
         unsigned diagSelect = P->isTypeName() ? VariadicType : VariadicValue;
         diagSelect |= (TA.isType() ? Type : Value) << 2u;

         Res.setHasIncompatibleKind(diagSelect, P);
         return false;
      }

      size_t i = 0;
      for (auto& VA : TA.VariadicArgs) {
         if (!checkSingleCompatibility(VA, P, idx, true)) {
            while (TA.VariadicArgs.size()
                   > TA.ManuallySpecifiedVariadicArgs + 1) {
               TA.VariadicArgs.pop_back();
            }

            return false;
         }

         ++i;
      }
   }
   else if (P->isTypeName()) {
      if (!TA.isType()) {
         unsigned diagSelect = Type;
         diagSelect |= (TA.isVariadic() ? VariadicValue : Value) << 2u;

         Res.setHasIncompatibleKind(diagSelect, P);
         return false;
      }

      //         if (P.covariance) {
      //            if (!TA.getType()->implicitlyCastableTo(P.covariance)) {
      //               err(err_generic_error)
      //                  << "incompatible template argument " + P->getName()
      //                     + ": expected " + P.covariance->toString()
      //                  << P->getSourceLoc() << end;
      //
      //               compatible = false;
      //               continue;
      //            }
      //         }
   }
   else {
      if (!TA.isValue()) {
         unsigned diagSelect = Value;
         diagSelect |= (TA.isVariadic() ? VariadicType : Type) << 2u;

         Res.setHasIncompatibleKind(diagSelect, P);
         return false;
      }

      if (TA.getValueType() != P->getValueType()) {
         Res.setHasIncompatibleType(TA.getValueType(), P);
         return false;
      }
   }

   return true;
}

void TemplateArgListImpl::checkCompatibility()
{
   assert(ResolvedArgs.size() == getParameters().size()
          && "broken template argument list");

   if (!Res)
      return;

   size_t idx = 0;
   for (auto& Arg : ResolvedArgs) {
      if (Arg.isNull()) {
         if (resolveDefault(Arg)) {
            ++idx;
            continue;
         }

         return Res.setCouldNotInfer(Arg.getParam());
      }

      if (!checkSingleCompatibility(Arg, Arg.getParam(), idx))
         return;

      ++idx;
   }
}

TemplateArgList::TemplateArgList(SemaPass& S, NamedDecl* Template,
                                 RawArgList templateArguments,
                                 SourceLocation loc)
    : pImpl(new TemplateArgListImpl(S, Template, templateArguments, loc))
{
}

TemplateArgList::TemplateArgList(SemaPass& S, QualType RecordTy,
                                 RawArgList templateArguments,
                                 SourceLocation loc)
    : pImpl(new TemplateArgListImpl(S, RecordTy->getRecord(), templateArguments,
                                    loc))
{
   if (auto Dep = RecordTy->asDependentRecordType()) {
      pImpl->copyFromList(Dep->getTemplateArgs());
   }
}

TemplateArgList::TemplateArgList(SemaPass& S, RawArgList templateArguments,
                                 SourceLocation loc)
    : pImpl(new TemplateArgListImpl(S, templateArguments, loc))
{
}

TemplateArgList::~TemplateArgList() { delete pImpl; }

TemplateArgList TemplateArgList::copy() const
{
   TemplateArgList Copy(pImpl->SP, pImpl->Template);
   Copy.pImpl->copyFromList(*this);

   return Copy;
}

void TemplateArgList::Profile(llvm::FoldingSetNodeID& ID,
                              TemplateArgList const& list)
{
   ID.AddInteger(list.size());
   for (auto& arg : list)
      arg.Profile(ID);
}

bool TemplateArgList::setParamValue(TemplateParamDecl* Param,
                                    TemplateArgument&& Arg) const
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->setParamValue(Param, move(Arg));
}

bool TemplateArgList::inferFromType(QualType contextualType,
                                    QualType returnType,
                                    bool IsLastVariadicParam) const
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->inferFromReturnType(contextualType, returnType,
                                     IsLastVariadicParam);
}

void TemplateArgList::inferFromArgList(
    llvm::ArrayRef<QualType> givenArgs,
    llvm::ArrayRef<FuncArgDecl*> neededArgs) const
{
   assert(pImpl && "incomplete argument list!");
   pImpl->inferFromArgList(givenArgs, neededArgs);
}

bool TemplateArgList::isFullyInferred() const
{
   if (!pImpl)
      return false;
   return pImpl->FullyInferred && pImpl->PartiallyInferred;
}

bool TemplateArgList::isPartiallyInferred() const
{
   if (!pImpl)
      return false;
   return !pImpl->FullyInferred && pImpl->PartiallyInferred;
}

TemplateArgListResult TemplateArgList::checkCompatibility() const
{
   assert(pImpl && "incomplete argument list!");
   pImpl->checkCompatibility();

   return pImpl->Res;
}

bool TemplateArgList::isStillDependent() const
{
   return pImpl && pImpl->isStillDependent();
}

bool TemplateArgList::hasRuntimeParameter() const
{
   return pImpl && pImpl->HasRuntimeParam;
}

TemplateArgument* TemplateArgList::getNamedArg(DeclarationName Name) const
{
   return pImpl ? pImpl->getNamedArg(Name) : nullptr;
}

TemplateArgument* TemplateArgList::getArgForParam(TemplateParamDecl* P) const
{
   return pImpl ? pImpl->getArgForParam(P) : nullptr;
}

TemplateParamDecl* TemplateArgList::getParameter(TemplateArgument* forArg) const
{
   size_t idx = 0;
   for (auto& arg : pImpl->ResolvedArgs) {
      if (&arg == forArg)
         return pImpl->getParameters()[idx];

      ++idx;
   }

   return nullptr;
}

ast::NamedDecl* TemplateArgList::getTemplate() const
{
   if (!pImpl)
      return nullptr;

   return pImpl->Template;
}

bool TemplateArgList::empty() const
{
   return !pImpl || pImpl->ResolvedArgs.empty();
}

size_t TemplateArgList::size() const
{
   return pImpl ? pImpl->ResolvedArgs.size() : 0;
}

const TemplateArgument& TemplateArgList::front() const
{
   return pImpl->ResolvedArgs.front();
}

const TemplateArgument& TemplateArgList::back() const
{
   return pImpl->ResolvedArgs.back();
}

const TemplateArgument& TemplateArgList::operator[](size_t idx) const
{
   return pImpl->ResolvedArgs[idx];
}

bool TemplateArgList::insert(TemplateArgument&& arg)
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(move(arg));
}

std::string TemplateArgList::toString(char beginC, char endC,
                                      bool showNames) const
{
   string s;
   {
      llvm::raw_string_ostream OS(s);
      print(OS, beginC, endC, showNames);
   }

   return s;
}

void TemplateArgList::print(llvm::raw_ostream& OS, char beginC, char endC,
                            bool showNames) const
{
   if (beginC)
      OS << beginC;

   size_t i = 0;
   auto end_it = end();

   for (auto it = begin(); it != end_it; ++it, ++i) {
      if (i != 0)
         OS << ", ";
      if (showNames) {
         OS << it->getParam()->getName();
         OS << " = ";
      }

      OS << it->toString();
   }

   if (endC)
      OS << endC;
}

TemplateArgList::arg_iterator TemplateArgList::begin()
{
   return pImpl->ResolvedArgs.begin();
}

TemplateArgList::arg_iterator TemplateArgList::end()
{
   return pImpl->ResolvedArgs.end();
}

TemplateArgList::const_arg_iterator TemplateArgList::begin() const
{
   return pImpl->ResolvedArgs.begin();
}

TemplateArgList::const_arg_iterator TemplateArgList::end() const
{
   return pImpl->ResolvedArgs.end();
}

llvm::MutableArrayRef<TemplateArgument> TemplateArgList::getMutableArgs() const
{
   return pImpl ? pImpl->ResolvedArgs
                : llvm::MutableArrayRef<TemplateArgument>();
}

TemplateArgListResult MultiLevelTemplateArgList::checkCompatibility() const
{
   for (auto& list : *this) {
      auto comp = list->checkCompatibility();
      if (!comp)
         return comp;
   }

   return TemplateArgListResult();
}

void MultiLevelTemplateArgList::Profile(llvm::FoldingSetNodeID& ID) const
{
   ID.AddInteger(size());
   for (auto* list : *this) {
      list->Profile(ID);
   }
}

void MultiLevelTemplateArgList::print(llvm::raw_ostream& OS) const
{
   OS << "[";

   for (auto& list : *this) {
      list->print(OS, '\0', '\0', true);
   }

   OS << "]";
}

std::string MultiLevelTemplateArgList::toString() const
{
   std::string str;
   {
      llvm::raw_string_ostream OS(str);
      print(OS);
   }

   return str;
}

FinalTemplateArgumentList::FinalTemplateArgumentList(
    MutableArrayRef<TemplateArgument> Args, bool Dependent, bool RuntimeParam,
    bool Canonicalize)
    : NumArgs((unsigned)Args.size()), Dependent(Dependent),
      RuntimeParam(RuntimeParam)
{
   auto it = getTrailingObjects<TemplateArgument>();
   for (auto& Arg : Args) {
      assert(!Arg.isNull() && "finalizing null template argument!");
      this->RuntimeParam |= Arg.isRuntime();
      this->Dependent |= Arg.isStillDependent();

      new (it++) TemplateArgument(Arg.clone(Canonicalize, true));
   }
}

FinalTemplateArgumentList*
FinalTemplateArgumentList::Create(ASTContext& C, const TemplateArgList& list,
                                  bool Canonicalize)
{
   return Create(C, list.getMutableArgs(), !list.isStillDependent());
}

FinalTemplateArgumentList* FinalTemplateArgumentList::Create(
    ASTContext& C, MutableArrayRef<TemplateArgument> Args, bool Canonicalize)
{
   llvm::FoldingSetNodeID ID;
   Profile(ID, Args, Canonicalize);

   void* InsertPos;
   if (auto* List = C.TemplateArgs.FindNodeOrInsertPos(ID, InsertPos)) {
      return List;
   }

   void* Mem = C.Allocate(totalSizeToAlloc<TemplateArgument>(Args.size()),
                          alignof(FinalTemplateArgumentList));

   auto* List
       = new (Mem) FinalTemplateArgumentList(Args, false, false, Canonicalize);

   C.TemplateArgs.InsertNode(List, InsertPos);
   return List;
}

void FinalTemplateArgumentList::Profile(llvm::FoldingSetNodeID& ID,
                                        bool Canonicalize) const
{
   Profile(ID, getArguments(), Canonicalize);
}

void FinalTemplateArgumentList::Profile(llvm::FoldingSetNodeID& ID,
                                        ArrayRef<TemplateArgument> Args,
                                        bool Canonicalize)
{
   ID.AddInteger(Args.size());
   for (auto& Arg : Args) {
      Arg.Profile(ID, Canonicalize);
   }
}

void FinalTemplateArgumentList::print(llvm::raw_ostream& OS, char beginC,
                                      char endC, bool showNames) const
{
   if (beginC)
      OS << beginC;

   size_t i = 0;
   auto end_it = end();

   for (auto it = begin(); it != end_it; ++it, ++i) {
      if (i != 0)
         OS << ", ";
      if (showNames) {
         OS << it->getParam()->getName();
         OS << " = ";
      }

      OS << it->toString();
   }

   if (endC)
      OS << endC;
}

std::string FinalTemplateArgumentList::toString(char begin, char end,
                                                bool showNames) const
{
   std::string s;
   llvm::raw_string_ostream OS(s);
   print(OS, begin, end, showNames);

   return OS.str();
}

const TemplateArgument*
FinalTemplateArgumentList::getNamedArg(DeclarationName Name) const
{
   for (auto& Arg : *this) {
      if (!Arg.isNull() && Arg.getParam()->getDeclName() == Name)
         return &Arg;
   }

   return nullptr;
}

const TemplateArgument*
FinalTemplateArgumentList::getArgForParam(TemplateParamDecl* P) const
{
   for (auto& Arg : *this) {
      if (Arg.getParam() == P)
         return &Arg;
   }

   return nullptr;
}

ast::TemplateParamDecl*
FinalTemplateArgumentList::getParameter(TemplateArgument* forArg) const
{
   for (auto& Arg : *this) {
      if (&Arg == forArg)
         return Arg.getParam();
   }

   return nullptr;
}

void MultiLevelFinalTemplateArgList::Profile(llvm::FoldingSetNodeID& ID) const
{
   for (auto& L : *this)
      L->Profile(ID);
}

void MultiLevelFinalTemplateArgList::print(llvm::raw_ostream& OS) const
{
   OS << "[";

   for (auto& list : *this) {
      list->print(OS, '\0', '\0', true);
   }

   OS << "]";
}

std::string MultiLevelFinalTemplateArgList::toString() const
{
   std::string str;
   {
      llvm::raw_string_ostream OS(str);
      print(OS);
   }

   return str;
}

void MultiLevelFinalTemplateArgList::reverse()
{
   std::reverse(ArgLists.begin(), ArgLists.end());
}

} // namespace sema
} // namespace cdot