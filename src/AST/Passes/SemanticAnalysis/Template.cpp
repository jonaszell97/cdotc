//
// Created by Jonas Zell on 04.01.18.
//

#include "Template.h"

#include "SemaPass.h"
#include "Function.h"
#include "Record/Record.h"

#include "../../SymbolTable.h"

#include "../../../Message/Diagnostics.h"
#include "../../../Variant/Variant.h"

#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/ArrayType.h"
#include "../../../Variant/Type/PointerType.h"

#include "../../Expression/TypeRef.h"
#include "../../Expression/StaticExpr.h"

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::ast;
using namespace cdot::cl;

namespace cdot {
namespace sema {

ResolvedTemplateArg::ResolvedTemplateArg(Variant &&V,
                                         SourceLocation loc) noexcept
   : is_type(false), is_variadic(false), is_inferred(false), ignore(false),
     V(new Variant(std::move(V))), loc(loc)
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
   loc = other.loc;

   if (is_variadic)
      new (&variadicArgs)
         std::vector<ResolvedTemplateArg>(move(other.variadicArgs));
   else if (is_type) {
      type = other.type;
   }
   else {
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

   return type->isDependantType();
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

class TemplateArgListImpl: public DiagnosticIssuer {
public:
   TemplateArgListImpl(SemaPass &SP,
                       Callable *C,
                       llvm::ArrayRef<TemplateArg> templateArgs)
      : ctx(Context::Function), SP(SP), originalArgs(templateArgs), C(C)
   {
      resolveWithParameters();
   }

   TemplateArgListImpl(SemaPass &SP,
                       Record *R,
                       llvm::ArrayRef<TemplateArg> templateArgs)
      : ctx(Context::Record), SP(SP), originalArgs(templateArgs), R(R)
   {
      resolveWithParameters();
   }

   TemplateArgListImpl(SemaPass &SP,
                       Typedef *td,
                       llvm::ArrayRef<TemplateArg> templateArgs)
      : ctx(Context::Typedef), SP(SP), originalArgs(templateArgs), td(td)
   {
      resolveWithParameters();
   }

   TemplateArgListImpl(SemaPass &SP,
                       Alias *alias,
                       llvm::ArrayRef<TemplateArg> templateArgs)
      : ctx(Context::Alias), SP(SP), originalArgs(templateArgs), alias(alias)
   {
      resolveWithParameters();
   }

   TemplateArgListImpl(SemaPass &SP,
                       llvm::ArrayRef<TemplateArg> templateArgs)
      : ctx(Context::None), SP(SP), originalArgs(templateArgs), R(nullptr)
   {

   }

   void resolveWithParameters()
   {
      size_t addedFromOuterRecords = 0;

      if (ctx == Context::Function) {
         if (auto M = dyn_cast<Method>(C))
            addedFromOuterRecords =inferFromRecordContext(M->getOwningRecord());
      }

      if (ctx == Context::Record)
         addedFromOuterRecords = inferFromRecordContext(R);

      auto parameters = getParameters();
      llvm::ArrayRef<TemplateParameter> restParams(
                                    parameters.data() + addedFromOuterRecords,
                                    parameters.size() - addedFromOuterRecords);

      size_t i = 0;
      for (auto &P : restParams) {
         if (originalArgs.size() <= i)
            break;

         if (P.isVariadic) {
            std::vector<ResolvedTemplateArg> variadicArgs;
            SourceLocation loc = P.getSourceLoc();

            while (i < originalArgs.size()) {
               auto &TA = originalArgs[i];
               if (TA.isTypeName()) {
                  variadicArgs.emplace_back(
                     *SP.visitTypeRef(TA.getType().get()), TA.getSourceLoc());
               }
               else {
                  auto val = SP.evalStaticExpr(TA.getStaticExpr().get());
                  if (TA.getStaticExpr()->isTypeDependant()) {
                     stillDependant = true;
                  }

                  variadicArgs.emplace_back(std::move(val), TA.getSourceLoc());
               }

               loc = TA.getSourceLoc();
               ++i;
            }

            emplace(P, P.isTypeName(), std::move(variadicArgs), loc);

            break;
         }

         auto &TA = originalArgs[i];
         if (TA.isTypeName()) {
            emplace(P, *SP.visitTypeRef(TA.getType().get()), TA.getSourceLoc());
         }
         else {
            auto val = SP.evalStaticExpr(TA.getStaticExpr().get());
            if (TA.getStaticExpr()->isTypeDependant()) {
               stillDependant = true;
            }

            emplace(P, std::move(val), TA.getSourceLoc());
         }

         ++i;
      }

      if (i < originalArgs.size()) {
         excessTemplateArgLoc = originalArgs[i].getSourceLoc();
         has_excess_arg = true;
         return;
      }

      if (parameters.size() == resolvedArgs.size())
         return;

      for (auto &P : parameters) {
         if (P.defaultValue) {
            auto it = resolvedArgs.find(P.getGenericTypeName());
            if (it == resolvedArgs.end()) {
               if (P.isTypeName()) {
                  emplace(P, *P.defaultValue->getType()->getTypeRef(),
                          P.getSourceLoc());
               }
               else {
                  emplace(P, Variant(P.defaultValue->getStaticExpr()
                                      ->getEvaluatedExpr()),
                          P.getSourceLoc());
               }
            }
         }
      }
   }

   TemplateParameter const* getMissingArgument() const
   {
      for (auto &P : getParameters()) {
         auto it = resolvedArgs.find(P.getGenericTypeName());
         if (it == resolvedArgs.end() && !P.isVariadic)
            return &P;
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

   void inferFromArgList(std::vector<QualType> const& givenArgs,
                         std::vector<Argument> const& neededArgs);

   bool checkSingleCompatibility(ResolvedTemplateArg const& TA,
                                 TemplateParameter const& TP);
   bool checkCompatibility();

   bool isStillDependant() const
   {
      return stillDependant;
   }

   llvm::ArrayRef<TemplateParameter> getParameters() const
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

   bool insert(llvm::StringRef name, Variant &&V)
   {
      return emplace(name, std::move(V)).second;
   }

   bool insert(llvm::StringRef name, bool isType,
               std::vector<ResolvedTemplateArg> &&variadicArgs) {
      return emplace(name, isType, std::move(variadicArgs)).second;
   }

   TemplateArgList moveInitializerArgs()
   {
      assert(ctx == Context::Function);

      auto M = cast<cl::Method>(C);
      assert(M->isInitializer());

      auto R = M->getOwningRecord();
      llvm::ArrayRef<TemplateParameter> initParams{
         M->getTemplateParams().data() + R->getTemplateParams().size(),
         M->getTemplateParams().size() - R->getTemplateParams().size()
      };

      TemplateArgList newList(SP);

      for (auto &P : initParams) {
         auto it = resolvedArgs.find(P.getGenericTypeName());
         assert (it != resolvedArgs.end());

         newList.pImpl->resolvedArgs.try_emplace(P.getGenericTypeName(),
                                                 std::move(it->second));

         resolvedArgs.erase(it);
      }

      newList.resolveWith(R);

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
   enum class Context : unsigned char {
      None, Record, Function, Typedef, Alias,
   };

   Context ctx;
   SemaPass &SP;
   llvm::ArrayRef<TemplateArg> originalArgs;
   llvm::StringMap<ResolvedTemplateArg> resolvedArgs;

   union {
      cl::Record *R;
      Callable *C;
      Typedef *td;
      Alias *alias;
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
      auto it = resolvedArgs.try_emplace(name, std::forward<Args>(args)...);
      stillDependant |= it.first->getValue().isStillDependant();

      return it;
   }

   template<class ...Args>
   auto emplace(TemplateParameter const& P, Args&&... args)
      -> decltype(resolvedArgs.try_emplace("", nullptr))
   {
      auto it = resolvedArgs.try_emplace(P.getGenericTypeName(),
                                         std::forward<Args>(args)...);

      stillDependant |= it.first->getValue().isStillDependant();
      return it;
   }

   size_t inferFromRecordContext(cl::Record *R)
   {
      if (!R->getOuterRecord())
         return 0;

      size_t addedFromOuterRecords = 0;
      for (const auto &TP : R->getOuterRecord()->getTemplateParams()) {
         if (TP.isVariadic) {
            emplace(TP.getGenericTypeName(), TP.isTypeName(),
                    std::vector<ResolvedTemplateArg>(),
                    TP.getSourceLoc());
         }
         else if (TP.isTypeName()) {
            emplace(TP.getGenericTypeName(),
                    GenericType::get(TP.getGenericTypeName(),
                                     TP.covariance),
                    TP.getSourceLoc());
         }
         else {
            emplace(TP.getGenericTypeName(), Variant(0llu),
                    TP.getSourceLoc());
         }

         ++addedFromOuterRecords;
      }

      return addedFromOuterRecords;
   }

   llvm::ArrayRef<TemplateParameter> getParametersImpl() const
   {
      switch (ctx) {
         case Context::None:     return {};
         case Context::Record:   return R->getTemplateParams();
         case Context::Function: return C->getTemplateParams();
         case Context::Typedef:  return td->templateParams;
         case Context::Alias:    return alias->getTemplateParams();
      }
   }
};

void TemplateArgListImpl::inferFromArgList(
                                    std::vector<QualType> const &givenArgs,
                                    std::vector<Argument> const &neededArgs) {
   if (!neededArgs.empty()) {
      bool variadic = neededArgs.back().isVariadic();
      size_t i = 0;

      for (const auto &arg : givenArgs) {
         if (i >= neededArgs.size() && !variadic) {
            break;
         }

         auto &neededArg = neededArgs.size() > i ? neededArgs[i]
                                                 : neededArgs.back();

         auto success = inferTemplateArg(*arg, *neededArg.type);
         if (!success)
            return;

         ++i;
      }
   }
}

bool TemplateArgListImpl::inferTemplateArg(Type *given, Type *needed)
{
   if (needed->isPointerTy()) {
      if (!given->isPointerTy()) {
         return false;
      }

      auto givenPointee = *given->asPointerTy()->getPointeeType();
      auto neededPointee = *needed->asPointerTy()->getPointeeType();

      return inferTemplateArg(givenPointee, neededPointee);
   }

   if (needed->isFunctionTy()) {
      if (!given->isFunctionTy()) {
         return false;
      }

      auto givenFunc = given->asFunctionTy();
      auto neededFunc = needed->asFunctionTy();

      if (givenFunc->getArgTypes().size() != neededFunc->getArgTypes().size()) {
         return false;
      }

      auto givenRet = *givenFunc->getReturnType();
      auto neededRet = *neededFunc->getReturnType();

      if (!inferTemplateArg(givenRet, neededRet))
         return false;

      size_t i = 0;
      auto &neededArgs = neededFunc->getArgTypes();
      for (auto &given : givenFunc->getArgTypes()) {
         auto neededTy = neededArgs.size() > i ? *neededArgs[i].type
                                               : *neededArgs.back().type;

         if (!inferTemplateArg(*given.type, neededTy))
            return false;

         ++i;
      }

      return true;
   }

   if (needed->isTupleTy()) {
      if (!given->isTupleTy()) {
         return false;
      }

      auto givenTuple = given->asTupleTy();
      auto neededTuple = needed->asTupleTy();

      size_t i = 0;

      auto &neededTys = neededTuple->getContainedTypes();
      for (auto &given : givenTuple->getContainedTypes()) {
         auto &neededTy = neededTys.size() > i ? neededTys[i].second
                                               : neededTys.back().second;

         if (!inferTemplateArg(*given.second, *neededTy))
            return false;

         ++i;
      }

      return true;
   }

   if (needed->isObjectTy() && !needed->isGenericTy()) {
      if (!given->isObjectTy()) {
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

   if (needed->isArrayTy()) {
      if (!given->isArrayTy())
         return false;

      auto givenArr = cast<ArrayType>(given);
      auto neededArr = cast<ArrayType>(needed);

      if (!inferTemplateArg(givenArr->getElementType(),
                            neededArr->getElementType()))
         return false;

      if (auto Inf = dyn_cast<InferredArrayType>(neededArr)) {
         size_t i = 0;
         for (const auto &P : getParameters()) {
            if (Inf->getParam().equals(P.getGenericTypeName())) {
               auto it = resolvedArgs.find(P.getGenericTypeName());
               if (it == resolvedArgs.end()) {
                  emplace(P, Variant(givenArr->getNumElements()));
               }
            }

            ++i;
         }
      }

      return true;
   }

   if (!needed->isGenericTy()) {
      return true;
   }

   if (given->isTypeGroup())
      given = given->getGroupDefault();

   auto neededGen = needed->asGenericTy();
   auto &neededTypeName = neededGen->getGenericTypeName();

   auto parameters = getParameters();
   auto paramIt = std::find_if(parameters.begin(), parameters.end(),
                            [neededTypeName](const TemplateParameter &constr) {
                               return constr.genericTypeName == neededTypeName;
                            });

   if (paramIt == parameters.end()) {
      return true;
   }

   auto &Param = *paramIt;

   auto it = resolvedArgs.find(neededTypeName);
   if (it == resolvedArgs.end()) {
      if (Param.isVariadic) {
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
                                                TemplateParameter const &P) {
   if (TA.isVariadic()) {
      for (auto &VA : TA.getVariadicArgs())
         if (!checkSingleCompatibility(VA, P))
            return false;
   }
   else if (P.isTypeName()) {
      if (!TA.isType()) {
         err(err_generic_error)
            << "incompatible template argument " + P.getGenericTypeName()
               + ": expected typename, but found value"
            << TA.getLoc() << end;

         note(note_generic_note)
            << "parameter declared here"
            << P.getSourceLoc() << end;

         return false;
      }

//         if (P.covariance) {
//            if (!TA.getType()->implicitlyCastableTo(P.covariance)) {
//               err(err_generic_error)
//                  << "incompatible template argument " + P.getGenericTypeName()
//                     + ": expected " + P.covariance->toString()
//                  << P.getSourceLoc() << end;
//
//               compatible = false;
//               continue;
//            }
//         }
   }
   else {
      if (!TA.getValue().typeOf()->implicitlyCastableTo(P.valueType)) {
         err(err_generic_error)
            << "incompatible template argument " + P.getGenericTypeName()
               + ": expected " + P.valueType->toString()
            << TA.getLoc() << end;

         note(note_generic_note)
            << "parameter declared here"
            << P.getSourceLoc() << end;

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
         << "could not infer template argument " + M->getGenericTypeName()
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
      auto it = resolvedArgs.find(P.getGenericTypeName());
      if (it == resolvedArgs.end()) {
         assert(P.isVariadic);
         emplace(P.getGenericTypeName(), true,
                 std::vector<ResolvedTemplateArg>(), P.getSourceLoc());

         continue;
      }

      auto &TA = it->second;
      if (!checkSingleCompatibility(TA, P))
         return false;
   }

   return true;
}

TemplateArgList::TemplateArgList(SemaPass &S,
                                 cl::Record *R,
                                 llvm::ArrayRef<TemplateArg> templateArguments)
   : pImpl(new TemplateArgListImpl(S, R, templateArguments))
{

}

TemplateArgList::TemplateArgList(ast::SemaPass &S, Callable *C,
                                 llvm::ArrayRef<TemplateArg> templateArguments)
   : pImpl(new TemplateArgListImpl(S, C, templateArguments))
{

}

TemplateArgList::TemplateArgList(ast::SemaPass &S, Typedef *td,
                                 llvm::ArrayRef<TemplateArg> templateArguments)
   : pImpl(new TemplateArgListImpl(S, td, templateArguments))
{

}

TemplateArgList::TemplateArgList(ast::SemaPass &S, Alias *alias,
                                 llvm::ArrayRef<TemplateArg> templateArguments)
   : pImpl(new TemplateArgListImpl(S, alias, templateArguments))
{

}

TemplateArgList::TemplateArgList(SemaPass &S,
                                 llvm::ArrayRef<TemplateArg> templateArguments)
   : pImpl(new TemplateArgListImpl(S, templateArguments))
{

}

TemplateArgList::~TemplateArgList()
{
   delete pImpl;
}

void TemplateArgList::inferFromArgList(std::vector<QualType> const &givenArgs,
                                       std::vector<Argument> const &neededArgs){
   assert(pImpl && "incomplete argument list!");
   pImpl->inferFromArgList(givenArgs, neededArgs);
}

void TemplateArgList::resolveWith(cl::Record *R)
{
   pImpl->ctx = TemplateArgListImpl::Context::Record;
   pImpl->R = R;
   pImpl->resolveWithParameters();
}

void TemplateArgList::resolveWith(Callable *C)
{
   pImpl->ctx = TemplateArgListImpl::Context::Function;
   pImpl->C = C;
   pImpl->resolveWithParameters();
}

void TemplateArgList::resolveWith(Typedef *td)
{
   pImpl->ctx = TemplateArgListImpl::Context::Typedef;
   pImpl->td = td;
   pImpl->resolveWithParameters();
}

void TemplateArgList::resolveWith(Alias *alias)
{
   pImpl->ctx = TemplateArgListImpl::Context::Record;
   pImpl->alias = alias;
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

bool TemplateArgList::isStillDependant() const
{
   return pImpl && pImpl->isStillDependant();
}

ResolvedTemplateArg const* TemplateArgList::getNamedArg(llvm::StringRef name)
const
{
   return pImpl ? pImpl->getNamedArg(name) : nullptr;
}

TemplateParameter const* TemplateArgList::getMissingArgument() const
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->getMissingArgument();
}

llvm::ArrayRef<TemplateArg> TemplateArgList::getOriginalArgs() const
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

bool TemplateArgList::insert(llvm::StringRef name, Variant &&V)
{
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(name, std::move(V));
}

bool TemplateArgList::insert(llvm::StringRef name, bool isType,
                             std::vector<ResolvedTemplateArg> &&variadicArgs) {
   assert(pImpl && "incomplete argument list!");
   return pImpl->insert(name, isType, move(variadicArgs));
}

std::string TemplateArgList::toString(char beginC, char endC,
                                      bool showNames) const {
   string s(1, beginC);
   size_t i = 0;
   auto end_it = end();

   for (auto it = begin(); it != end_it; ++it, ++i) {
      if (i != 0) { s += ", "; }
      if (showNames) {
         s += it.getParam().getGenericTypeName();
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
   arg = list.getNamedArg(it->getGenericTypeName());
}

void TemplateArgList::arg_iterator::operator++()
{
   ++it;
   if (it == list->pImpl->getParameters().end()) {
      list = nullptr;
      arg = nullptr;

      return;
   }

   arg = list->getNamedArg(it->getGenericTypeName());
}

} // namespace sema
} // namespace cdot