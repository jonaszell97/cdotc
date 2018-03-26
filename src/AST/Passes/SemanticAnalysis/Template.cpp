//
// Created by Jonas Zell on 04.01.18.
//

#include "Template.h"

#include "AST/Expression.h"
#include "AST/Decl.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "Message/Diagnostics.h"
#include "Variant/Variant.h"

using std::string;

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::ast;

namespace cdot {
namespace sema {

ResolvedTemplateArg::ResolvedTemplateArg(Variant &&V,
                                         QualType valueType,
                                         SourceLocation loc) noexcept
   : IsType(false), IsVariadic(false), IsNull(false),
     ManuallySpecifiedVariadicArgs(0),
     Type(valueType), V(new Variant(std::move(V))), Loc(loc)
{}

ResolvedTemplateArg::ResolvedTemplateArg(ResolvedTemplateArg &&other) noexcept
   : IsType(other.IsType), IsVariadic(other.IsVariadic), IsNull(other.IsNull),
     ManuallySpecifiedVariadicArgs(other.ManuallySpecifiedVariadicArgs),
     Type(other.Type), Loc(other.Loc)
{
   if (IsVariadic) {
      new(&VariadicArgs)
         std::vector<ResolvedTemplateArg>(move(other.VariadicArgs));
   }
   else if(!IsType) {
      new (&V) std::unique_ptr<Variant>(new Variant(std::move(*other.V)));
   }
}

ResolvedTemplateArg::~ResolvedTemplateArg()
{
   destroyValue();
}

ResolvedTemplateArg& ResolvedTemplateArg::operator=(ResolvedTemplateArg &&other)
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

bool ResolvedTemplateArg::isStillDependent() const
{
   if (!isType())
      return false;

   if (isVariadic()) {
      for (auto &VA : VariadicArgs)
         if (VA.isStillDependent())
            return true;

      return false;
   }

   return Type->isDependentType();
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

      return ResolvedTemplateArg(isType(), move(args), getLoc());
   }

   if (isType()) {
      return ResolvedTemplateArg(getType(), getLoc());
   }

   return ResolvedTemplateArg(Variant(getValue()), getType(), getLoc());
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
   else if (isType()) {
      return Type->toString();
   }
   else {
      return V->toString();
   }
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
      ID.AddPointer(getType());
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
      resolveWithParameters();
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
            new(&ResolvedArgs[idx]) ResolvedTemplateArg(Param->isTypeName(),
                                                        {});
         }

         ++idx;
      }
   }

   void resolveWithParameters()
   {
      size_t addedFromOuterRecords = 0;

      if (Template) {
         if (auto M = dyn_cast_or_null<MethodDecl>(Template))
            addedFromOuterRecords = inferFromRecordContext(M->getRecord());

         if (auto R = dyn_cast_or_null<RecordDecl>(Template))
            addedFromOuterRecords = inferFromRecordContext(R);
      }

      auto parameters = getParameters();
      llvm::ArrayRef<TemplateParamDecl*> restParams
         = parameters.drop_front(addedFromOuterRecords);

      size_t i = 0;
      bool variadic = false;

      for (auto &P : restParams) {
         if (i >= OriginalArgs.size())
            break;

         if (P->isVariadic()) {
            std::vector<ResolvedTemplateArg> variadicArgs;

            while (i < OriginalArgs.size()) {
               auto &TA = OriginalArgs[i];
               auto &Out = variadicArgs.emplace_back();

               if (!makeSingleArgument(Out, TA))
                  return;

               ++i;
            }

            variadic = true;

            auto numVariadics = (unsigned)variadicArgs.size();
            SourceLocation loc = numVariadics ? variadicArgs.front().getLoc()
                                              : P->getSourceLoc();

            auto Pair = emplace(P, P->isTypeName(), std::move(variadicArgs),
                                loc);

            assert(Pair.first != string::npos && "failed insertion");
            ResolvedArgs[Pair.first].ManuallySpecifiedVariadicArgs =
               numVariadics;

            break;
         }

         auto &TA = OriginalArgs[i];
         if (!makeSingleArgument(ResolvedArgs[i], TA))
            return;

         ++i;
      }

      if (OriginalArgs.size() > restParams.size() && !variadic) {
         Res.setHasTooManyTemplateArgs(OriginalArgs.size(), parameters.size());
         return;
      }

      if (parameters.size() == ResolvedArgs.size())
         return;

      for (auto &P : parameters) {
         if (auto V = P->getDefaultValue()) {
            if (getIndexFor(P) == string::npos) {
               if (P->isTypeName()) {
                  emplace(P, V->getExprType(), P->getSourceLoc());
               }
               else {
                  emplace(P, Variant(cast<StaticExpr>(V)->getEvaluatedExpr()),
                          V->getExprType(),
                          P->getSourceLoc());
               }
            }
         }
      }
   }

   bool makeSingleArgument(ResolvedTemplateArg &Out,
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
         Out = ResolvedTemplateArg(ty, TA->getSourceLoc());
      }
      else if (ty->isMetaType()) {
         Out = ResolvedTemplateArg(ty->asMetaType()->getUnderlyingType(),
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
         Out = ResolvedTemplateArg(Variant(StatExp->getEvaluatedExpr()),
                                   StatExp->getExprType(),
                                   StatExp->getSourceLoc());
      }

      return true;
   }

   void copyFromList(const TemplateArgList &list)
   {
      auto it = list.begin();
      auto end = list.end();

      for (; it != end; ++it) {
         if (!it->isNull()) {
            auto Param = it.getParam();
            emplace(Param, it->clone());
         }
      }
   }

   TemplateParamDecl* getMissingParameter() const
   {
      size_t i = 0;
      for (auto &Arg : ResolvedArgs) {
         if (Arg.isNull())
            return getParameters()[i];

         ++i;
      }

      return nullptr;
   }

   ResolvedTemplateArg* getNamedArg(llvm::StringRef name)
   {
      auto idx = getIndexFor(name);
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
      return emplace(name, ty).second;
   }

   bool insert(llvm::StringRef name, Variant &&V, QualType valueTy)
   {
      return emplace(name, std::move(V), valueTy).second;
   }

   bool insert(llvm::StringRef name, bool isType,
               std::vector<ResolvedTemplateArg> &&variadicArgs) {
      return emplace(name, isType, std::move(variadicArgs)).second;
   }

   bool insert(llvm::StringRef name, ResolvedTemplateArg &&arg)
   {
      return emplace(name, move(arg)).second;
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
   std::vector<ResolvedTemplateArg> ResolvedArgs;

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
      return emplace(getIndexFor(name), std::forward<Args&&>(args)...);
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

      new(&ResolvedArgs[idx])
         ResolvedTemplateArg(std::forward<Args&&>(args)...);

      StillDependent |= ResolvedArgs[idx].isStillDependent();
      return { idx, true };
   }

   size_t inferFromRecordContext(RecordDecl *R)
   {
      RecordDecl *Outer = nullptr;
      auto Ctx = R->getDeclContext();
      while (Ctx) {
         if (auto RD = dyn_cast<RecordDecl>(Ctx)) {
            Outer = RD;
            break;
         }
         if (isa<TranslationUnit>(Ctx))
            break;

         Ctx = Ctx->getParentCtx();
      }

      if (!Outer)
         return 0;

      size_t addedFromOuterRecords = 0;
      for (const auto &TP : Outer->getTemplateParams()) {
         if (TP->isVariadic()) {
            emplace(TP, TP->isTypeName(),
                    std::vector<ResolvedTemplateArg>(),
                    TP->getSourceLoc());
         }
         else if (TP->isTypeName()) {
            emplace(TP, SP.getContext().getTemplateArgType(TP->getCovariance(),
                                                           TP->getName()),
                    TP->getSourceLoc());
         }
         else {
            emplace(TP, Variant(0llu), SP.getContext().getIntTy(),
                    TP->getSourceLoc());
         }

         ++addedFromOuterRecords;
      }

      return addedFromOuterRecords;
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

      for (const auto &arg : givenArgs) {
         if (i >= neededArgs.size() && !variadic) {
            break;
         }

         auto &neededArg = neededArgs.size() > i ? neededArgs[i]
                                                 : neededArgs.back();

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

      return inferTemplateArg(given->getPointeeType(),
                              needed->getPointeeType());
   }

   if (needed->isReferenceType()) {
      if (!given->isReferenceType()) {
         return false;
      }

      return inferTemplateArg(given->getReferencedType(),
                              needed->getReferencedType());
   }

   if (needed->isFunctionType()) {
      if (!given->isFunctionType()) {
         return false;
      }

      auto givenFunc = given->asFunctionType();
      auto neededFunc = needed->asFunctionType();

      if (givenFunc->getParamTypes().size() != neededFunc->getParamTypes().size()) {
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

      auto givenTuple = given->asTupleType();
      auto neededTuple = needed->asTupleType();

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
         auto &givenConcrete = given->getTemplateArgs();
         auto &neededConcrete = needed->getTemplateArgs();

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

      ArrayType *givenArr = given->asArrayType();
      ArrayType *neededArr = needed->asArrayType();

      if (!inferTemplateArg(givenArr->getElementType(),
                            neededArr->getElementType()))
         return false;

//      if (auto Inf = dyn_cast<DependentSizeArrayType>(neededArr)) {
//         size_t i = 0;
//         for (const auto &P : getParameters()) {
//            if (Inf->getParam().equals(P->getName())) {
//               auto it = resolvedArgs.find(P->getName());
//               if (it == resolvedArgs.end()) {
//                  emplace(P, Variant(givenArr->getNumElements()));
//               }
//            }
//
//            ++i;
//         }
//      }

      return true;
   }

   if (!needed->isGenericType()) {
      return true;
   }

   GenericType *neededGen = needed->asGenericType();
   auto neededTypeName = neededGen->getGenericTypeName();

   auto parameters = getParameters();
   auto idx = getIndexFor(neededTypeName);

   if (idx >= parameters.size())
      return true;

   TemplateParamDecl const* Param = parameters[idx];
   assert(Param->isTypeName()
          && "allowed Value parameter to be used as argument type!");

   auto &Arg = ResolvedArgs[idx];
   if (Arg.isNull()) {
      if (Param->isVariadic()) {
         std::vector<ResolvedTemplateArg> variadicArgs;
         variadicArgs.emplace_back(given);

         emplace(idx, true, move(variadicArgs), ListLoc);
      }
      else {
         emplace(idx, given, ListLoc);
      }
   }
   else {
      if (Arg.isVariadic()) {
         if (!Arg.ManuallySpecifiedVariadicArgs) {
            Arg.emplace_back(given);
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
   auto parameters = getParameters();
   for (auto &P : parameters) {
      auto &Arg = ResolvedArgs[idx];
      if (Arg.isNull())
         return Res.setCouldNotInfer(P);

      if (!checkSingleCompatibility(Arg, P, idx))
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

void TemplateArgList::Profile(llvm::FoldingSetNodeID &ID,
                              TemplateArgList const& list) {
   ID.AddInteger(list.size());
   for (auto &arg : list)
      arg.Profile(ID);
}

void TemplateArgList::inferFromReturnType(QualType contextualType,
                                          QualType returnType) {
   assert(pImpl && "incomplete argument list!");
   pImpl->inferFromReturnType(contextualType, returnType);
}

void TemplateArgList::inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                                       llvm::ArrayRef<FuncArgDecl*> neededArgs){
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

bool TemplateArgList::insert(llvm::StringRef name, QualType ty)
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(name, ty);
}

bool TemplateArgList::insert(llvm::StringRef name,Variant &&V, QualType valueTy)
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(name, std::move(V), valueTy);
}

bool TemplateArgList::insert(llvm::StringRef name, bool isType,
                             std::vector<ResolvedTemplateArg> &&variadicArgs) {
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(name, isType, move(variadicArgs));
}

bool TemplateArgList::insert(llvm::StringRef name, ResolvedTemplateArg &&arg)
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(name, move(arg));
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
         OS << it.getParam()->getName();
         OS << " = ";
      }

      OS << it->toString();
   }

   if (endC)
      OS << endC;
}

TemplateArgList::arg_iterator::arg_iterator(TemplateArgList const &list)
   : arg(nullptr), list(&list)
{
   if (list.empty() || list.pImpl->getParameters().empty()) {
      this->list = nullptr;
      return;
   }

   it = list.pImpl->getParameters().begin();
   arg = list.getNamedArg((*it)->getName());
}

void TemplateArgList::arg_iterator::operator++()
{
   ++it;
   if (it == list->pImpl->getParameters().end()) {
      list = nullptr;
      arg = nullptr;

      return;
   }

   arg = list->getNamedArg((*it)->getName());
}

void MultiLevelTemplateArgList::print(llvm::raw_ostream &OS) const
{
   OS << "[";

   size_t i = 0;
   for (auto &list : *this) {
      auto end_it = list->end();
      for (auto it = list->begin(); it != end_it; ++it, ++i) {
         if (i != 0) OS << ", ";
         OS << it.getParam()->getName()
            << " = " << it->toString();
      }
   }

   OS << "]";
}

} // namespace sema
} // namespace cdot