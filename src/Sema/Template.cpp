//
// Created by Jonas Zell on 04.01.18.
//

#include "Template.h"

#include "AST/Expression.h"
#include "AST/Decl.h"
#include "SemaPass.h"
#include "Message/Diagnostics.h"
#include "Basic/Variant.h"

using std::string;

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::ast;

namespace cdot {
namespace sema {

ResolvedTemplateArg::ResolvedTemplateArg(ast::TemplateParamDecl *Param,
                                         QualType type,
                                         SourceLocation loc) noexcept
   : IsType(true), IsVariadic(false), IsNull(false),
     Dependent(type->isDependentType()),
     ManuallySpecifiedVariadicArgs(0),
     Param(Param), Type(type->getCanonicalType()), Loc(loc)
{}

ResolvedTemplateArg::ResolvedTemplateArg(ast::TemplateParamDecl *Param,
                                         Variant &&V,
                                         SourceLocation loc) noexcept
   : IsType(false), IsVariadic(false), IsNull(false), Dependent(false),
     ManuallySpecifiedVariadicArgs(0),
     Param(Param), V(new Variant(std::move(V))), Loc(loc)
{}

ResolvedTemplateArg::ResolvedTemplateArg(ast::TemplateParamDecl *Param,
                                         bool isType,
                                         std::vector<ResolvedTemplateArg>
                                                               &&variadicArgs,
                                         SourceLocation loc)
   : IsType(isType), IsVariadic(true), IsNull(false), Dependent(false),
     ManuallySpecifiedVariadicArgs(0),
     Param(Param), VariadicArgs(move(variadicArgs)), Loc(loc)
{
   for (auto &VA: this->VariadicArgs) {
      if (VA.Dependent) {
         Dependent = true;
         break;
      }
   }
}

ResolvedTemplateArg::ResolvedTemplateArg(ResolvedTemplateArg &&other) noexcept
   : IsType(other.IsType), IsVariadic(other.IsVariadic), IsNull(other.IsNull),
     Dependent(other.Dependent),
     ManuallySpecifiedVariadicArgs(other.ManuallySpecifiedVariadicArgs),
     Param(other.Param), Loc(other.Loc)
{
   if (IsVariadic) {
      new(&VariadicArgs)
         std::vector<ResolvedTemplateArg>(move(other.VariadicArgs));
   }
   else if (IsType) {
      Type = other.Type;
   }
   else {
      new (&V) std::unique_ptr<Variant>(new Variant(std::move(*other.V)));
   }
}

ResolvedTemplateArg::~ResolvedTemplateArg()
{
   destroyValue();
}

ResolvedTemplateArg& ResolvedTemplateArg::operator=(ResolvedTemplateArg &&other)
noexcept
{
   destroyValue();
   new(this) ResolvedTemplateArg(move(other));
   return *this;
}

void ResolvedTemplateArg::destroyValue()
{
   if (IsNull)
      return;

   if (IsVariadic) {
      for (auto &VA : VariadicArgs)
         VA.destroyValue();

      VariadicArgs.~vector();
   }
   else if (!IsType) {
      V.~unique_ptr();
   }
}

const Variant& ResolvedTemplateArg::getValue() const
{
   assert(!isType());
   return *V;
}

QualType ResolvedTemplateArg::getValueType() const
{
   assert(isValue());
   return Param->getValueType();
}

bool ResolvedTemplateArg::isStillDependent() const
{
   return Dependent;
}

ResolvedTemplateArg ResolvedTemplateArg::clone() const
{
   if (isNull()) {
      return ResolvedTemplateArg();
   }

   if (isVariadic()) {
      std::vector<ResolvedTemplateArg> args;
      for (auto &VA : getVariadicArgs())
         args.emplace_back(VA.clone());

      return ResolvedTemplateArg(Param, isType(), move(args), getLoc());
   }

   if (isType()) {
      return ResolvedTemplateArg(Param, getType(), getLoc());
   }

   return ResolvedTemplateArg(Param, Variant(getValue()), getLoc());
}

std::string ResolvedTemplateArg::toString() const
{
   if (isVariadic()) {
      string s = "(";
      size_t i = 0;

      for (auto &VA : VariadicArgs) {
         if (i != 0) s += ", ";
         s += VA.toString();

         ++i;
      }

      s += ")";
      return s;
   }

   if (isType()) {
      return Type->toString();
   }

   return V->toString();
}

void ResolvedTemplateArg::Profile(llvm::FoldingSetNodeID &ID) const
{
   ID.AddBoolean(isNull());
   ID.AddBoolean(isType());
   ID.AddBoolean(isVariadic());

   if (isNull()) {
      return;
   }

   if (isVariadic()) {
      for (auto &VA : getVariadicArgs())
         VA.Profile(ID);
   }
   else if (isType())
      ID.AddPointer(getType().getAsOpaquePtr());
   else
      getValue().Profile(ID);
}

class TemplateArgListImpl {
public:
   TemplateArgListImpl(SemaPass &SP,
                       NamedDecl *Template,
                       llvm::ArrayRef<Expression*> templateArgs,
                       SourceLocation loc)
      : SP(SP), OriginalArgs(templateArgs), ListLoc(loc),
        Template(Template),
        StillDependent(false), HadError(false),
        FullyInferred(templateArgs.empty()), PartiallyInferred(false)
   {
      doInitialFill();

      if (!templateArgs.empty()) {
         resolveWithParameters();
      }
      else {
         inferFromContext();
      }
   }

   TemplateArgListImpl(SemaPass &SP,
                       llvm::ArrayRef<Expression*> templateArgs,
                       SourceLocation loc)
      : SP(SP), OriginalArgs(templateArgs), ListLoc(loc),
        StillDependent(false), HadError(false),
        FullyInferred(templateArgs.empty()), PartiallyInferred(false)
   {

   }

   void doInitialFill()
   {
      auto Params = getParameters();
      ResolvedArgs.resize(Params.size());

      size_t idx = 0;
      for (auto &Param : Params) {
         if (Param->isVariadic()) {
            new(&ResolvedArgs[idx])
               ResolvedTemplateArg(Param, Param->isTypeName(), {});
         }
         else {
            ResolvedArgs[idx].Param = Param;
         }

         ++idx;
      }
   }

   void resolveWithParameters()
   {
      size_t i = 0;
      auto parameters = getParameters();
      bool variadic = false;

      for (auto &P : parameters) {
         if (i >= OriginalArgs.size())
            break;

         if (P->isVariadic()) {
            std::vector<ResolvedTemplateArg> variadicArgs;

            while (i < OriginalArgs.size()) {
               auto &TA = OriginalArgs[i];
               auto &Out = variadicArgs.emplace_back();

               if (!makeSingleArgument(P, Out, TA))
                  return;

               ++i;
            }

            variadic = true;

            auto numVariadics = (unsigned)variadicArgs.size();
            SourceLocation loc = numVariadics ? variadicArgs.front().getLoc()
                                              : P->getSourceLoc();

            auto Idx = getIndexFor(P);
            auto &VA = ResolvedArgs[Idx];

            assert(VA.isVariadic() && "incorrect initial template argument");
            assert(VA.getVariadicArgs().empty() && "duplicate variadic arg");

            VA.VariadicArgs = move(variadicArgs);
            VA.Loc = loc;
            VA.ManuallySpecifiedVariadicArgs =numVariadics;

            break;
         }

         auto &TA = OriginalArgs[i];
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

      for (auto &P : parameters) {
         if (auto V = P->getDefaultValue()) {
            if (getIndexFor(P) == string::npos) {
               if (P->isTypeName()) {
                  emplace(P, P, V->getExprType(), P->getSourceLoc());
               }
               else {
                  emplace(P, P,
                          Variant(cast<StaticExpr>(V)->getEvaluatedExpr()),
                          P->getSourceLoc());
               }
            }
         }
      }
   }

   bool makeSingleArgument(TemplateParamDecl *P,
                           ResolvedTemplateArg &Out,
                           Expression *TA) {
      auto res = SP.visitExpr(TA);
      StillDependent |= TA->isTypeDependent();

      if (TA->isDependent()) {
         StillDependent = true;
      }

      if (!res) {
         HadError = true;
         return false;
      }

      auto ty = res.get()->getExprType();
      if (isa<TypeExpr>(res.get())) {
         Out = ResolvedTemplateArg(P, ty, TA->getSourceLoc());
      }
      else if (ty->isMetaType()) {
         Out = ResolvedTemplateArg(
            P, cast<MetaType>(ty)->getUnderlyingType(),
            TA->getSourceLoc());
      }
      else {
         auto StatExp = StaticExpr::Create(SP.getContext(), TA);
         auto SemaResult = SP.visitExpr(StatExp);

         if (StatExp->isDependent()) {
            StillDependent = true;
            return false;
         }

         if (!SemaResult) {
            HadError = true;
            return false;
         }

         StatExp = cast<StaticExpr>(SemaResult.get());
         Out = ResolvedTemplateArg(P, Variant(StatExp->getEvaluatedExpr()),
                                   StatExp->getSourceLoc());
      }

      return true;
   }

   void copyFromList(const TemplateArgList &list)
   {
      auto it = list.begin();
      auto end = list.end();

      auto param_it = getParameters().begin();
      for (; it != end; ++it) {
         if (!it->isNull()) {
            auto &Param = *param_it++;
            emplace(Param, it->clone());
         }
      }
   }

   ResolvedTemplateArg* getNamedArg(llvm::StringRef name)
   {
      auto idx = getIndexFor(name);
      if (idx == string::npos)
         return nullptr;

      return &ResolvedArgs[idx];
   }

   ResolvedTemplateArg* getArgForParam(TemplateParamDecl *P)
   {
      auto idx = getIndexFor(P);
      if (idx == string::npos)
         return nullptr;

      return &ResolvedArgs[idx];
   }

   void inferFromReturnType(QualType contextualType, QualType returnType);
   void inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                         llvm::ArrayRef<FuncArgDecl*> neededArgs);

   bool checkSingleCompatibility(ResolvedTemplateArg& TA,
                                 TemplateParamDecl* const& TP,
                                 size_t idx,
                                 bool ignoreVariadic = false);

   void checkCompatibility();

   bool isStillDependant() const
   {
      return StillDependent;
   }

   llvm::ArrayRef<TemplateParamDecl*> getParameters() const
   {
      if (!Template)
         return {};

      return Template->getTemplateParams();
   }

   bool insert(llvm::StringRef name, QualType ty)
   {
      auto idx = getIndexFor(name);
      if (idx == string::npos)
         return false;

      auto Param = getParameters()[idx];
      return emplace(Param, Param, ty).second;
   }

   bool insert(llvm::StringRef name, Variant &&V)
   {
      auto idx = getIndexFor(name);
      if (idx == string::npos)
         return false;

      auto Param = getParameters()[idx];
      return emplace(Param, Param, std::move(V)).second;
   }

   bool insert(llvm::StringRef name, bool isType,
               std::vector<ResolvedTemplateArg> &&variadicArgs) {
      auto idx = getIndexFor(name);
      if (idx == string::npos)
         return false;

      auto Param = getParameters()[idx];
      return emplace(Param, Param, isType, std::move(variadicArgs)).second;
   }

   bool insert(ResolvedTemplateArg &&arg)
   {
      auto Param = arg.getParam();
      StillDependent |= arg.isStillDependent();

      return emplace(Param, move(arg)).second;
   }

   size_t getIndexFor(llvm::StringRef name)
   {
      size_t idx = 0;
      for (auto &param : getParameters()) {
         if (param->getName() == name)
            return idx;

         ++idx;
      }

      return string::npos;
   }

   size_t getIndexFor(TemplateParamDecl *Param)
   {
      size_t idx = 0;
      for (auto &param : getParameters()) {
         if (param == Param)
            return idx;

         ++idx;
      }

      return string::npos;
   }

   friend class TemplateArgList;

private:
   SemaPass &SP;
   llvm::ArrayRef<Expression*> OriginalArgs;
   llvm::SmallVector<ResolvedTemplateArg, 0> ResolvedArgs;

   SourceLocation ListLoc;
   NamedDecl *Template = nullptr;

   bool StillDependent    : 1;
   bool HadError          : 1;
   bool FullyInferred     : 1;
   bool PartiallyInferred : 1;

   TemplateArgListResult Res;

   bool inferTemplateArg(QualType given, QualType needed);

   template<class ...Args>
   std::pair<size_t, bool> emplace(llvm::StringRef name, Args&&... args)
   {
      auto idx = getIndexFor(name);
      return emplace(idx, std::forward<Args&&>(args)...);
   }

   template<class ...Args>
   std::pair<size_t, bool> emplace(TemplateParamDecl *Decl, Args&&... args)
   {
      return emplace(getIndexFor(Decl), std::forward<Args&&>(args)...);
   }

   template<class ...Args>
   std::pair<size_t, bool> emplace(size_t idx, Args&&... args)
   {
      if (idx == string::npos || !ResolvedArgs[idx].isNull())
         return { idx, false };

      auto &Arg = ResolvedArgs[idx];
      new(&Arg) ResolvedTemplateArg(std::forward<Args&&>(args)...);

      StillDependent |= ResolvedArgs[idx].isStillDependent();
      return { idx, true };
   }

   void fillImplicitTemplateArgs()
   {
      for (auto P : getParameters()) {
         if (P->isVariadic()) {
            emplace(P, P, P->isTypeName(),
                    std::vector<ResolvedTemplateArg>(),
                    P->getSourceLoc());
         }
         else if (P->isTypeName()) {
            emplace(P, P, P->getCovariance(), P->getSourceLoc());
         }
         else {
            emplace(P, P, Variant(), P->getSourceLoc());
         }
      }
   }

   void inferFromContext()
   {
      if (!isa<RecordDecl>(Template))
         return;

      // you can refer to a template without any template arguments from
      // within itself
      for (auto *Ctx = &SP.getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
         auto ND = dyn_cast<NamedDecl>(Ctx);
         if (!ND)
            continue;

         if (dyn_cast<NamedDecl>(ND) == Template) {
            fillImplicitTemplateArgs();
            break;
         }

         if (ND->isInstantiation() && ND->getSpecializedTemplate() == Template){
            copyFromList(ND->getTemplateArgs());
         }
      }

      // chekc if we're in a context that allows incomplete template names,
      // e.g. an extension type
      if (SP.allowIncompleteTemplateTypes()) {
         fillImplicitTemplateArgs();
      }
   }

   bool resolveDefault(ResolvedTemplateArg &Arg)
   {
      auto Def = Arg.getParam()->getDefaultValue();
      if (!Def)
         return false;

      assert(!Arg.isVariadic() && "variadics cannot have a default value!");

      if (Arg.isType()) {
         Arg = ResolvedTemplateArg(Arg.getParam(),
                                   Def->getExprType(),
                                   Def->getSourceLoc());
      }
      else {
         auto SE = cast<StaticExpr>(Def);
         Arg = ResolvedTemplateArg(Arg.getParam(),
                                   Variant(SE->getEvaluatedExpr()),
                                   Def->getSourceLoc());
      }

      return true;
   }
};

void TemplateArgListImpl::inferFromReturnType(QualType contextualType,
                                              QualType returnType) {
   if (contextualType->isAutoType())
      return;

   inferTemplateArg(contextualType, returnType);
}

void
TemplateArgListImpl::inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                                      llvm::ArrayRef<FuncArgDecl*> neededArgs) {
   if (!neededArgs.empty()) {
      bool variadic = neededArgs.back()->isVariadicArgPackExpansion();
      size_t i = 0;

      FuncArgDecl *Previous = nullptr;
      for (const auto &arg : givenArgs) {
         if (i >= neededArgs.size() && !variadic) {
            break;
         }

         auto &neededArg = neededArgs.size() > i ? neededArgs[i]
                                                 : neededArgs.back();

         if (Previous && Previous != neededArg) {
            // update variadic arguments so we don't infer them again
            for (auto &Arg : ResolvedArgs) {
               if (!Arg.isVariadic())
                  continue;

               Arg.ManuallySpecifiedVariadicArgs
                  = (unsigned)Arg.VariadicArgs.size();
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

bool TemplateArgListImpl::inferTemplateArg(QualType given, QualType needed)
{
   if (given->isReferenceType() && !needed->isReferenceType()) {
      given = given->getReferencedType();
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

   if (needed->isFunctionType()) {
      if (!given->isFunctionType()) {
         return false;
      }

      auto givenFunc = cast<FunctionType>(given);
      auto neededFunc = cast<FunctionType>(needed);

      if (givenFunc->getParamTypes().size()
          != neededFunc->getParamTypes().size()) {
         return false;
      }

      auto givenRet = *givenFunc->getReturnType();
      auto neededRet = *neededFunc->getReturnType();

      if (!inferTemplateArg(givenRet, neededRet))
         return false;

      size_t i = 0;
      auto neededArgs = neededFunc->getParamTypes();
      for (auto &givenArg : givenFunc->getParamTypes()) {
         auto neededTy = neededArgs.size() > i ? neededArgs[i]
                                               : neededArgs.back();

         if (!inferTemplateArg(givenArg, neededTy))
            return false;

         ++i;
      }

      return true;
   }

   if (needed->isTupleType()) {
      if (!given->isTupleType()) {
         return false;
      }

      auto givenTuple = cast<TupleType>(given);
      auto neededTuple = cast<TupleType>(needed);

      size_t i = 0;

      auto neededTys = neededTuple->getContainedTypes();
      for (auto &givenEl : givenTuple->getContainedTypes()) {
         auto &neededTy = neededTys.size() > i ? neededTys[i]
                                               : neededTys.back();

         if (!inferTemplateArg(givenEl, neededTy))
            return false;

         ++i;
      }

      return true;
   }

   if (needed->isRecordType()) {
      if (!given->isRecordType()) {
         return false;
      }

      if (given->hasTemplateArgs() && needed->hasTemplateArgs()) {
         auto &givenConcrete = cast<RecordType>(given)->getTemplateArgs();
         auto &neededConcrete = cast<RecordType>(needed)->getTemplateArgs();

         if (givenConcrete.size() != neededConcrete.size()) {
            return false;
         }

         auto given_it = givenConcrete.begin();
         auto needed_it = neededConcrete.begin();
         auto given_end = givenConcrete.end();

         while (given_it != given_end) {
            auto &TA = *given_it;
            auto &TA2 = *needed_it;

            if (!TA.isType() || !TA2.isType())
               continue;

            if (!inferTemplateArg(TA.getType(), TA2.getType()))
               return false;

            ++given_it;
            ++needed_it;
         }

         return true;
      }

      return true;
   }

   if (needed->isArrayType()) {
      if (!given->isArrayType())
         return false;

      ArrayType *givenArr = cast<ArrayType>(given);
      ArrayType *neededArr = cast<ArrayType>(needed);

      if (!inferTemplateArg(givenArr->getElementType(),
                            neededArr->getElementType()))
         return false;

      if (auto Inf = dyn_cast<DependentSizeArrayType>(neededArr)) {
         auto Ident = dyn_cast<IdentifierRefExpr>(Inf->getSizeExpr()
                                                     ->getExpr());
         if (!Ident)
            return true;

         size_t i = 0;
         for (const auto &P : getParameters()) {
            if (Ident->getIdentInfo() == P->getDeclName().getIdentifierInfo()) {
               auto idx = getIndexFor(P);
               if (ResolvedArgs[idx].isNull()) {
                  emplace(P, P, Variant((uint64_t)givenArr->getNumElements()));
               }
            }

            ++i;
         }
      }

      return true;
   }

   if (!isa<GenericType>(needed)) {
      return true;
   }

   GenericType *neededGen = cast<GenericType>(needed);
   auto neededTypeName = neededGen->getGenericTypeName();

   auto parameters = getParameters();
   auto idx = getIndexFor(neededTypeName);

   if (idx >= parameters.size())
      return true;

   auto &Arg = ResolvedArgs[idx];
   auto Param = Arg.getParam();
   assert(Param->isTypeName()
          && "allowed Value parameter to be used as argument type!");


   if (Arg.isNull()) {
      if (Param->isVariadic()) {
         std::vector<ResolvedTemplateArg> variadicArgs;
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
         if (!Arg.ManuallySpecifiedVariadicArgs) {
            Arg.emplace_back(Param, given);
         }
         else {
            auto variadicIdx =
               Arg.VariadicArgs.size() - Arg.ManuallySpecifiedVariadicArgs;
            auto &ManuallySpecifiedTA =
               Arg.VariadicArgs[variadicIdx];

            if (ManuallySpecifiedTA.getType() != given) {
               while (Arg.VariadicArgs.size()
                      > Arg.ManuallySpecifiedVariadicArgs + 1) {
                  Arg.VariadicArgs.pop_back();
               }

               Res.setHasConflict(given, idx);
               return false;
            }

            --Arg.ManuallySpecifiedVariadicArgs;
         }
      }
      else {
         // ensure that both inferred types are the same
         QualType ty = Arg.getType();
         if (ty != given) {
            Res.setHasConflict(given, idx);
            return false;
         }
      }
   }

   PartiallyInferred = true;
   return true;
}

void MultiLevelTemplateArgList::inferFromReturnType(QualType contextualType,
                                                    QualType returnType) {
   for (auto &list : *this)
      list->inferFromReturnType(contextualType, returnType);
}

void MultiLevelTemplateArgList::inferFromArgList(
                                    llvm::ArrayRef<QualType> givenArgs,
                                    llvm::ArrayRef<FuncArgDecl *> neededArgs) {
   for (auto &list : *this)
      list->inferFromArgList(givenArgs, neededArgs);
}

bool
TemplateArgListImpl::checkSingleCompatibility(ResolvedTemplateArg &TA,
                                              TemplateParamDecl* const &P,
                                              size_t idx,
                                              bool ignoreVariadic) {
   enum DiagSelectIndex : unsigned {
      Type          = 0,
      Value         = 1,
      VariadicType  = 2,
      VariadicValue = 3,
   };

   if (P->isVariadic() && !ignoreVariadic) {
      if (!TA.isVariadic()) {
         unsigned diagSelect = P->isTypeName() ? VariadicType : VariadicValue;
         diagSelect |= (TA.isType() ? Type : Value) << 2u;

         Res.setHasIncompatibleKind(diagSelect, idx);
         return false;
      }

      size_t i = 0;
      for (auto &VA : TA.VariadicArgs) {
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

         Res.setHasIncompatibleKind(diagSelect, idx);
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

         Res.setHasIncompatibleKind(diagSelect, idx);
         return false;
      }

      if (!SP.implicitlyCastableTo(TA.getValueType(), P->getValueType())) {
         Res.setHasIncompatibleType(TA.getValueType(), idx);
         return false;
      }
   }

   return true;
}

void TemplateArgListImpl::checkCompatibility()
{
   if (StillDependent)
      return;

   assert(ResolvedArgs.size() == getParameters().size()
          && "broken template argument list");

   if (!Res)
      return;

   size_t idx = 0;
   for (auto &Arg : ResolvedArgs) {
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

TemplateArgList::TemplateArgList(SemaPass &S,
                                 NamedDecl *Template,
                                 llvm::ArrayRef<Expression*> templateArguments,
                                 SourceLocation loc)
   : pImpl(new TemplateArgListImpl(S, Template, templateArguments, loc))
{

}

TemplateArgList::TemplateArgList(SemaPass &S,
                                 QualType RecordTy,
                                 RawArgList templateArguments,
                                 SourceLocation loc)
   : pImpl(new TemplateArgListImpl(S, RecordTy->getRecord(), templateArguments,
                                   loc))
{
   if (auto Dep = RecordTy->asDependentRecordType()) {
      pImpl->copyFromList(Dep->getTemplateArgs());
   }
}

TemplateArgList::TemplateArgList(SemaPass &S,
                                 llvm::ArrayRef<Expression*> templateArguments,
                                 SourceLocation loc)
   : pImpl(new TemplateArgListImpl(S, templateArguments, loc))
{

}

TemplateArgList::~TemplateArgList()
{
   delete pImpl;
}

TemplateArgList TemplateArgList::copy() const
{
   TemplateArgList Copy(pImpl->SP, pImpl->Template);
   Copy.pImpl->copyFromList(*this);

   return Copy;
}

void TemplateArgList::Profile(llvm::FoldingSetNodeID &ID,
                              TemplateArgList const& list) {
   ID.AddInteger(list.size());
   for (auto &arg : list)
      arg.Profile(ID);
}

void TemplateArgList::inferFromReturnType(QualType contextualType,
                                          QualType returnType) const {
   assert(pImpl && "incomplete argument list!");
   pImpl->inferFromReturnType(contextualType, returnType);
}

void
TemplateArgList::inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                                  llvm::ArrayRef<FuncArgDecl*> neededArgs)const{
   assert(pImpl && "incomplete argument list!");
   pImpl->inferFromArgList(givenArgs, neededArgs);
}

bool TemplateArgList::isFullyInferred() const
{
   if (!pImpl) return false;
   return pImpl->FullyInferred && pImpl->PartiallyInferred;
}

bool TemplateArgList::isPartiallyInferred() const
{
   if (!pImpl) return false;
   return !pImpl->FullyInferred && pImpl->PartiallyInferred;
}

void TemplateArgList::resolveWith(NamedDecl *T)
{
   pImpl->Template = T;

   assert(pImpl->ResolvedArgs.empty() && "should only be called once!");

   pImpl->doInitialFill();
   pImpl->resolveWithParameters();
}

TemplateArgListResult TemplateArgList::checkCompatibility() const
{
   assert(pImpl && "incomplete argument list!");
   pImpl->checkCompatibility();

   return pImpl->Res;
}

bool TemplateArgList::isStillDependent() const
{
   return pImpl && pImpl->isStillDependant();
}

ResolvedTemplateArg* TemplateArgList::getNamedArg(llvm::StringRef name)
const
{
   return pImpl ? pImpl->getNamedArg(name) : nullptr;
}

ResolvedTemplateArg* TemplateArgList::getArgForParam(TemplateParamDecl *P) const
{
   return pImpl ? pImpl->getArgForParam(P) : nullptr;
}

TemplateParamDecl*
TemplateArgList::getParameter(ResolvedTemplateArg *forArg) const
{
   size_t idx = 0;
   for (auto &arg : pImpl->ResolvedArgs) {
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

llvm::ArrayRef<Expression*> TemplateArgList::getOriginalArgs() const
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->OriginalArgs;
}

bool TemplateArgList::empty() const
{
   return !pImpl || pImpl->ResolvedArgs.empty();
}

size_t TemplateArgList::size() const
{
   return pImpl ? pImpl->ResolvedArgs.size() : 0;
}

const ResolvedTemplateArg& TemplateArgList::front() const
{
   return pImpl->ResolvedArgs.front();
}

const ResolvedTemplateArg& TemplateArgList::back() const
{
   return pImpl->ResolvedArgs.back();
}

const ResolvedTemplateArg& TemplateArgList::operator[](size_t idx) const
{
   return pImpl->ResolvedArgs[idx];
}

bool TemplateArgList::insert(llvm::StringRef name, QualType ty)
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(name, ty);
}

bool TemplateArgList::insert(llvm::StringRef name,Variant &&V)
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(name, std::move(V));
}

bool TemplateArgList::insert(llvm::StringRef name, bool isType,
                             std::vector<ResolvedTemplateArg> &&variadicArgs) {
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(name, isType, move(variadicArgs));
}

bool TemplateArgList::insert(ResolvedTemplateArg &&arg)
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(move(arg));
}

std::string TemplateArgList::toString(char beginC, char endC,
                                      bool showNames) const {
   string s;
   {
      llvm::raw_string_ostream OS(s);
      print(OS, beginC, endC, showNames);
   }

   return s;
}

void TemplateArgList::print(llvm::raw_ostream &OS,
                            char beginC, char endC, bool showNames) const {
   if (beginC)
      OS << beginC;

   size_t i = 0;
   auto end_it = end();

   for (auto it = begin(); it != end_it; ++it, ++i) {
      if (i != 0) OS << ", ";
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

TemplateArgListResult MultiLevelTemplateArgList::checkCompatibility() const
{
   for (auto &list : *this) {
      auto comp = list->checkCompatibility();
      if (!comp)
         return comp;
   }

   return TemplateArgListResult();
}

void MultiLevelTemplateArgList::print(llvm::raw_ostream &OS) const
{
   OS << "[";

   for (auto &list : *this) {
      list->print(OS, '\0', '\0', true);
   }

   OS << "]";
}

} // namespace sema
} // namespace cdot