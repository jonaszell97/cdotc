//
// Created by Jonas Zell on 04.01.18.
//

#include "Template.h"

#include "Message/Diagnostics.h"

#include "Variant/Variant.h"
#include "Variant/Type/Type.h"

#include "AST/Passes/SemanticAnalysis/SemaPass.h"

#include "AST/Expression/TypeRef.h"
#include "AST/Expression/StaticExpr.h"

#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Declaration/LocalVarDecl.h"
#include "AST/Statement/Declaration/TypedefDecl.h"

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::ast;

namespace cdot {
namespace sema {

ResolvedTemplateArg::ResolvedTemplateArg(Variant &&V,
                                         QualType valueType,
                                         SourceLocation loc) noexcept
   : is_type(false), is_variadic(false), is_inferred(false), ignore(false),
     type(valueType), V(new Variant(std::move(V))), loc(loc)
{}

ResolvedTemplateArg::ResolvedTemplateArg(ResolvedTemplateArg &&other) noexcept
   : is_type(other.is_type), is_variadic(other.is_variadic), loc(other.loc)
{
   if (is_variadic)
      new (&variadicArgs)
         std::vector<ResolvedTemplateArg>(move(other.variadicArgs));
   else if (is_type) {
      type = other.type;
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
{
   destroyValue();
   is_type = other.is_type;
   is_variadic = other.is_variadic;
   type = other.type;
   loc = other.loc;

   if (is_variadic)
      new (&variadicArgs)
         std::vector<ResolvedTemplateArg>(move(other.variadicArgs));
   else if (!is_type) {
      new (&V) std::unique_ptr<Variant>(new Variant(std::move(*other.V)));
   }

   return *this;
}

void ResolvedTemplateArg::destroyValue()
{
   if (is_variadic) {
      for (auto &VA : variadicArgs)
         VA.destroyValue();
   }
   else if (!is_type) {
      V.~unique_ptr();
   }
}

const Variant& ResolvedTemplateArg::getValue() const
{
   assert(!isType());
   return *V;
}

bool ResolvedTemplateArg::isStillDependant() const
{
   if (!isType())
      return false;

   if (isVariadic()) {
      for (auto &VA : variadicArgs)
         if (VA.isStillDependant())
            return true;

      return false;
   }

   return type->isDependentType();
}

std::string ResolvedTemplateArg::toString() const
{
   if (isVariadic()) {
      string s = "(";
      size_t i = 0;

      for (auto &VA : variadicArgs) {
         if (i != 0) s += ", ";
         s += VA.toString();

         ++i;
      }

      s += ")";
      return s;
   }
   else if (isType()) {
      return type->toString();
   }
   else {
      return V->toString();
   }
}

void ResolvedTemplateArg::Profile(llvm::FoldingSetNodeID &ID) const
{
   ID.AddBoolean(isType());
   ID.AddBoolean(isVariadic());

   if (isVariadic()) {
      for (auto &VA : getVariadicArgs())
         VA.Profile(ID);
   }
   else if (isType())
      ID.AddPointer(getType());
   else
      getValue().Profile(ID);
}

class TemplateArgListImpl: public DiagnosticIssuer {
public:
   TemplateArgListImpl(SemaPass &SP,
                       NamedDecl *Template,
                       llvm::ArrayRef<TemplateArgExpr*> templateArgs)
      : SP(SP), originalArgs(templateArgs),
        Template(Template)
   {
      resolveWithParameters();
   }

   TemplateArgListImpl(SemaPass &SP,
                       llvm::ArrayRef<TemplateArgExpr*> templateArgs)
      : SP(SP), originalArgs(templateArgs)
   {

   }

   void resolveWithParameters()
   {
      size_t addedFromOuterRecords = 0;

      if (Template) {
         if (auto M =dyn_cast_or_null<MethodDecl>(Template))
            addedFromOuterRecords =inferFromRecordContext(M->getRecord());

         if (auto R =dyn_cast_or_null<RecordDecl>(Template))
            addedFromOuterRecords = inferFromRecordContext(R);
      }

      auto parameters = getParameters();
      llvm::ArrayRef<TemplateParamDecl*> restParams(
         parameters.data() + addedFromOuterRecords,
         parameters.size() - addedFromOuterRecords);

      size_t i = 0;
      for (auto &P : restParams) {
         if (originalArgs.size() <= i)
            break;

         if (P->isVariadic()) {
            std::vector<ResolvedTemplateArg> variadicArgs;
            SourceLocation loc = P->getSourceLoc();

            while (i < originalArgs.size()) {
               auto &TA = originalArgs[i];
               if (TA->isTypeName()) {
                  variadicArgs.emplace_back(
                     *SP.visitTypeRef(TA->getType()), TA->getSourceLoc());
               }
               else {
                  auto val = SP.evalStaticExpr(TA->getExpr());
                  if (TA->getExpr()->isTypeDependent()) {
                     stillDependant = true;
                  }

                  variadicArgs.emplace_back(std::move(val.getValue()),
                                            TA->getExpr()->getExprType(),
                                            TA->getSourceLoc());
               }

               loc = TA->getSourceLoc();
               ++i;
            }

            emplace(P, P->isTypeName(), std::move(variadicArgs), loc);

            break;
         }

         auto &TA = originalArgs[i];
         if (TA->isTypeName()) {
            emplace(P, *SP.visitTypeRef(TA->getType()), TA->getSourceLoc());
         }
         else {
            auto val = SP.evalStaticExpr(TA->getExpr());
            if (TA->getExpr()->isTypeDependent()) {
               stillDependant = true;
            }

            emplace(P, std::move(val.getValue()), TA->getExpr()->getExprType(),
                    TA->getSourceLoc());
         }

         ++i;
      }

      if (i < originalArgs.size()) {
         excessTemplateArgLoc = originalArgs[i]->getSourceLoc();
         has_excess_arg = true;
         return;
      }

      if (parameters.size() == resolvedArgs.size())
         return;

      for (auto &P : parameters) {
         if (auto V = P->getDefaultValue()) {
            auto it = resolvedArgs.find(P->getName());
            if (it == resolvedArgs.end()) {
               if (P->isTypeName()) {
                  emplace(P, *V->getType()->getTypeRef(),
                          P->getSourceLoc());
               }
               else {
                  emplace(P, Variant(V->getExpr()->getEvaluatedExpr()),
                          V->getExpr()->getExprType(),
                          P->getSourceLoc());
               }
            }
         }
      }
   }

   TemplateParamDecl const* getMissingArgument() const
   {
      for (auto &P : getParameters()) {
         auto it = resolvedArgs.find(P->getName());
         if (it == resolvedArgs.end() && !P->isVariadic())
            return P;
      }

      return nullptr;
   }

   ResolvedTemplateArg const* getNamedArg(llvm::StringRef name)
   {
      auto it = resolvedArgs.find(name);
      if (it == resolvedArgs.end() || it->second.ignore)
         return nullptr;

      return &it->second;
   }

   void inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                         llvm::ArrayRef<FuncArgDecl*> neededArgs);

   bool checkSingleCompatibility(ResolvedTemplateArg const& TA,
                                 TemplateParamDecl* const& TP);
   bool checkCompatibility();

   bool isStillDependant() const
   {
      return stillDependant;
   }

   llvm::ArrayRef<TemplateParamDecl*> getParameters() const
   {
      auto params = getParametersImpl();
      return {
         params.data() + parameterOffsetBegin,
         params.size() - parameterOffsetBegin - parameterOffsetEnd
      };
   }

   bool insert(llvm::StringRef name, Type *ty)
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

   TemplateArgList moveInitializerArgs()
   {
      auto M = cast<MethodDecl>(Template);
      assert(isa<InitDecl>(M));

      auto R = M->getRecord();
      llvm::ArrayRef<TemplateParamDecl*> initParams{
         M->getTemplateParams().data() + R->getTemplateParams().size(),
         M->getTemplateParams().size() - R->getTemplateParams().size()
      };

      TemplateArgList newList(SP);

      for (auto &P : initParams) {
         auto it = resolvedArgs.find(P->getName());
         assert (it != resolvedArgs.end());

         newList.pImpl->resolvedArgs.try_emplace(P->getName(),
                                                 std::move(it->second));

         resolvedArgs.erase(it);
      }

      newList.resolveWith(Template);

      return newList;
   }

   void markAsIgnored(llvm::StringRef name)
   {
      auto it = resolvedArgs.find(name);
      if (it != resolvedArgs.end())
         it->second.ignore = true;
   }

   void markAllAsNotIgnored()
   {
      for (auto &TA : resolvedArgs)
         TA.second.ignore = false;
   }

   void erase(llvm::StringRef name)
   {
      auto it = resolvedArgs.find(name);
      if (it != resolvedArgs.end())
         resolvedArgs.erase(it);
   }

   friend class TemplateArgList;

private:
   SemaPass &SP;
   llvm::ArrayRef<TemplateArgExpr*> originalArgs;
   llvm::StringMap<ResolvedTemplateArg> resolvedArgs;

   union {
      NamedDecl *Template = nullptr;
   };

   bool stillDependant = false;
   SourceLocation excessTemplateArgLoc;
   bool has_excess_arg = false;

   size_t parameterOffsetBegin = 0;
   size_t parameterOffsetEnd   = 0;

   bool inferTemplateArg(Type* given, Type *needed);

   template<class ...Args>
   auto emplace(llvm::StringRef name, Args&&... args)
      -> decltype(resolvedArgs.try_emplace("", nullptr))
   {
      auto it = resolvedArgs.try_emplace(name, std::forward<Args&&>(args)...);
      stillDependant |= it.first->getValue().isStillDependant();

      return it;
   }

   template<class ...Args>
   auto emplace(TemplateParamDecl* const& P, Args&&... args)
      -> decltype(resolvedArgs.try_emplace("", nullptr))
   {
      auto it = resolvedArgs.try_emplace(P->getName(),
                                         std::forward<Args&&>(args)...);

      stillDependant |= it.first->getValue().isStillDependant();
      return it;
   }

   size_t inferFromRecordContext(RecordDecl *R)
   {
      if (!R->getOuterRecord())
         return 0;

      size_t addedFromOuterRecords = 0;
      for (const auto &TP : R->getOuterRecord()->getTemplateParams()) {
         if (TP->isVariadic()) {
            emplace(TP->getName(), TP->isTypeName(),
                    std::vector<ResolvedTemplateArg>(),
                    TP->getSourceLoc());
         }
         else if (TP->isTypeName()) {
            emplace(TP->getName(),
                    SP.getContext().getTemplateArgType(
                       TP->getCovariance()->getType(), TP->getName()),
                    TP->getSourceLoc());
         }
         else {
            emplace(TP->getName(), Variant(0llu), SP.getContext().getIntTy(),
                    TP->getSourceLoc());
         }

         ++addedFromOuterRecords;
      }

      return addedFromOuterRecords;
   }

   llvm::ArrayRef<TemplateParamDecl*> getParametersImpl() const
   {
      if (!Template)
         return {};

      return Template->getTemplateParams();
   }
};

void TemplateArgListImpl::inferFromArgList(
                                      llvm::ArrayRef<QualType> givenArgs,
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

         auto success = inferTemplateArg(*arg,
                                         *neededArg->getArgType()->getType());
         if (!success)
            return;

         ++i;
      }
   }
}

bool TemplateArgListImpl::inferTemplateArg(Type *given, Type *needed)
{
   if (needed->isPointerType()) {
      if (!given->isPointerType()) {
         return false;
      }

      auto givenPointee = *given->asPointerType()->getPointeeType();
      auto neededPointee = *needed->asPointerType()->getPointeeType();

      return inferTemplateArg(givenPointee, neededPointee);
   }

   if (needed->isFunctionType()) {
      if (!given->isFunctionType()) {
         return false;
      }

      auto givenFunc = given->asFunctionType();
      auto neededFunc = needed->asFunctionType();

      if (givenFunc->getArgTypes().size() != neededFunc->getArgTypes().size()) {
         return false;
      }

      auto givenRet = *givenFunc->getReturnType();
      auto neededRet = *neededFunc->getReturnType();

      if (!inferTemplateArg(givenRet, neededRet))
         return false;

      size_t i = 0;
      auto neededArgs = neededFunc->getArgTypes();
      for (auto &given : givenFunc->getArgTypes()) {
         auto neededTy = neededArgs.size() > i ? *neededArgs[i]
                                               : *neededArgs.back();

         if (!inferTemplateArg(*given, neededTy))
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
      for (auto &given : givenTuple->getContainedTypes()) {
         auto &neededTy = neededTys.size() > i ? neededTys[i]
                                               : neededTys.back();

         if (!inferTemplateArg(*given, *neededTy))
            return false;

         ++i;
      }

      return true;
   }

   if (needed->isObjectType() && !needed->isGenericType()) {
      if (!given->isObjectType()) {
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

      if (!inferTemplateArg(*givenArr->getElementType(),
                            *neededArr->getElementType()))
         return false;

//      if (auto Inf = dyn_cast<InferredArrayType>(neededArr)) {
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

   auto neededGen = needed->asGenericType();
   auto neededTypeName = neededGen->getGenericTypeName();

   auto parameters = getParameters();
   auto paramIt = std::find_if(parameters.begin(), parameters.end(),
                            [neededTypeName](TemplateParamDecl const* TP) {
                               return TP->getName() == neededTypeName;
                            });

   if (paramIt == parameters.end()) {
      return true;
   }

   auto &Param = *paramIt;

   auto it = resolvedArgs.find(neededTypeName);
   if (it == resolvedArgs.end()) {
      if (Param->isVariadic()) {
         std::vector<ResolvedTemplateArg> variadicArgs;
         variadicArgs.emplace_back(given);

         auto inferredTA = emplace(neededTypeName, true, move(variadicArgs));
         inferredTA.first->second.is_inferred = true;
      }
      else {
         auto inferredTA = emplace(neededTypeName, given);
         inferredTA.first->second.is_inferred = true;
      }
   }
   else {
      auto &TA = it->second;
      if (TA.isVariadic() && TA.is_inferred) {
         TA.emplace_back(given);
      }
   }

   return true;
}

bool TemplateArgListImpl::checkSingleCompatibility(
                                                ResolvedTemplateArg const &TA,
                                                TemplateParamDecl* const &P) {
   if (P->isVariadic()) {
      if (!TA.isVariadic()) {
         err(err_generic_error)
            << "incompatible template argument " + P->getName()
               + ": expected variadic argument"
            << TA.getLoc() << end;

         note(note_generic_note)
            << "parameter declared here"
            << P->getSourceLoc() << end;

         return false;
      }

      for (auto &VA : TA.getVariadicArgs())
         if (!checkSingleCompatibility(VA, P))
            return false;
   }
   else if (P->isTypeName()) {
      if (!TA.isType()) {
         err(err_generic_error)
            << "incompatible template argument " + P->getName()
               + ": expected typename, but found value"
            << TA.getLoc() << end;

         note(note_generic_note)
            << "parameter declared here"
            << P->getSourceLoc() << end;

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
         err(err_generic_error)
            << "incompatible template argument " + P->getName()
               + ": expected value, but found typename"
            << TA.getLoc() << end;

         note(note_generic_note)
            << "parameter declared here"
            << P->getSourceLoc() << end;

         return false;
      }

      if (!SP.implicitlyCastableTo(TA.getValueType(),
                                   P->getValueType()->getType())) {
         err(err_generic_error)
            << "incompatible template argument " + P->getName()
               + ": expected " + P->getValueType()->toString()
            << TA.getLoc() << end;

         note(note_generic_note)
            << "parameter declared here"
            << P->getSourceLoc() << end;

         return false;
      }
   }

   return true;
}

bool TemplateArgListImpl::checkCompatibility()
{
   auto M = getMissingArgument();
   if (M) {
      err(err_generic_error)
         << "could not infer template argument " + M->getName()
         << M->getSourceLoc() << end;

      return false;
   }

   auto parameters = getParameters();

   if (has_excess_arg) {
      assert(parameters.size() != originalArgs.size());
      err(err_generic_error)
         << "too many template arguments: expected "
            + std::to_string(parameters.size()) + " but found "
            + std::to_string(originalArgs.size())
         << excessTemplateArgLoc << end;

      return false;
   }

   for (auto &P : parameters) {
      auto it = resolvedArgs.find(P->getName());
      if (it == resolvedArgs.end()) {
         assert(P->isVariadic());
         emplace(P->getName(), true,
                 std::vector<ResolvedTemplateArg>(), P->getSourceLoc());

         continue;
      }

      auto &TA = it->second;
      if (!checkSingleCompatibility(TA, P))
         return false;
   }

   return true;
}

TemplateArgList::TemplateArgList(SemaPass &S,
                                 NamedDecl *Template,
                                 llvm::ArrayRef<TemplateArgExpr*> templateArguments)
   : pImpl(new TemplateArgListImpl(S, Template, templateArguments))
{

}

TemplateArgList::TemplateArgList(SemaPass &S,
                                 llvm::ArrayRef<TemplateArgExpr*> templateArguments)
   : pImpl(new TemplateArgListImpl(S, templateArguments))
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

void TemplateArgList::inferFromArgList(llvm::ArrayRef<QualType> givenArgs,
                                       llvm::ArrayRef<FuncArgDecl*> neededArgs){
   assert(pImpl && "incomplete argument list!");
   pImpl->inferFromArgList(givenArgs, neededArgs);
}

void TemplateArgList::resolveWith(NamedDecl *T)
{
   pImpl->Template = T;
   pImpl->resolveWithParameters();
}

TemplateArgList TemplateArgList::moveInitializerArgs()
{
   if (!pImpl)
      return {};

   return pImpl->moveInitializerArgs();
}

void TemplateArgList::markAsIgnored(llvm::StringRef name)
{
   pImpl->markAsIgnored(name);
}

void TemplateArgList::markAllAsNotIgnored()
{
   pImpl->markAllAsNotIgnored();
}

bool TemplateArgList::checkCompatibility() const
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->checkCompatibility();
}

llvm::SmallVector<diag::DiagnosticBuilder, 4>& TemplateArgList::getDiagnostics()
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->getDiagnostics();
}

bool TemplateArgList::isStillDependent() const
{
   return pImpl && pImpl->isStillDependant();
}

ResolvedTemplateArg const* TemplateArgList::getNamedArg(llvm::StringRef name)
const
{
   return pImpl ? pImpl->getNamedArg(name) : nullptr;
}

TemplateParamDecl const* TemplateArgList::getMissingArgument() const
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->getMissingArgument();
}

llvm::ArrayRef<TemplateArgExpr*> TemplateArgList::getOriginalArgs() const
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->originalArgs;
}

void TemplateArgList::setParameterOffsetBegin(size_t parameterOffsetBegin)
{
   pImpl->parameterOffsetBegin = parameterOffsetBegin;
}

void TemplateArgList::setParameterOffsetEnd(size_t parameterOffsetEnd)
{
   pImpl->parameterOffsetEnd = parameterOffsetEnd;
}

bool TemplateArgList::empty() const
{
   return !pImpl || pImpl->resolvedArgs.empty();
}

size_t TemplateArgList::size() const
{
   return pImpl ? pImpl->resolvedArgs.size() : 0;
}

bool TemplateArgList::insert(llvm::StringRef name, Type *ty)
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
   string s(1, beginC);
   size_t i = 0;
   auto end_it = end();

   for (auto it = begin(); it != end_it; ++it, ++i) {
      if (i != 0) { s += ", "; }
      if (showNames) {
         s += it.getParam()->getName();
         s += " = ";
      }

      s += it->toString();
   }

   s += endC;
   return s;
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

} // namespace sema
} // namespace cdot