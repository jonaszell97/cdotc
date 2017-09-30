//
// Created by Jonas Zell on 13.08.17.
//

#include "Type.h"
#include "ObjectType.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/StaticAnalysis/Class.h"

namespace cdot {

   llvm::IRBuilder<>* Type::Builder = nullptr;

   std::vector<Type*> Type::getContainedTypes(bool includeSelf) {
      if (includeSelf) {
         return {this};
      }

      return {};
   }

   std::vector<Type**> Type::getTypeReferences() {
      return {};
   }

   PointerType* Type::getPointerTo() {
      return new PointerType(this);
   }

   void Type::resolveGeneric(Type** ty, unordered_map<string, Type *> concreteGenerics) {
      for (auto& cont : (*ty)->getTypeReferences()) {
         resolveGeneric(cont, concreteGenerics);
      }

      if ((*ty)->isGeneric()) {
         auto& className = (*ty)->getGenericClassName();
         if (concreteGenerics.find(className) == concreteGenerics.end()) {
            return;
         }

         auto backup = *ty;
         *ty = concreteGenerics.at(className)->deepCopy();
         CopyProperties(backup, *ty);
      }
   }

   bool Type::isBoxedPrimitive() {
      return isObject() && util::matches(
         "(Float|Double|U?Int(1|8|16|32|64)?)",
         className
      );
   }

   bool Type::GenericTypesCompatible(Type *given, Type *needed) {
      auto& neededClassname = needed->getClassName();

      if (SymbolTable::hasClass(neededClassname) && SymbolTable::getClass(neededClassname)->isProtocol()) {
         if (given->getClassName() == needed->getClassName()) {
            return true;
         }

         if (needed->getClassName() == "Any") {
            return true;
         }

         if (given->getClassName().empty()) {
            return false;
         }

         return SymbolTable::getClass(given->getClassName())->conformsTo(needed->getClassName());
      }

      if (needed->getContravariance() != nullptr) {
         auto contraVar = SymbolTable::getClass(given->getClassName());
         if (!contraVar->isBaseClassOf(needed->getContravariance()->getClassName())) {
            return false;
         }
      }

      if (needed->getClassName() == "Any") {
         return true;
      }

      if (!SymbolTable::hasClass(needed->getClassName())) {
         return false;
      }

      auto coVar = SymbolTable::getClass(needed->getClassName());
      return coVar->isBaseClassOf(given->getClassName());
   }

   void Type::resolveUnqualified(Type *ty)
   {
      for (const auto& cont : ty->getContainedTypes()) {
         resolveUnqualified(cont);
      }

      if (!isa<ObjectType>(ty)) {
         return;
      }

      auto asObj = cast<ObjectType>(ty);
      auto& unqal = asObj->getUnqualifiedGenerics();

      if (unqal.empty()) {
         return;
      }

      // should throw later
      if (!SymbolTable::hasClass(asObj->getClassName())) {
         return;
      }

      auto cl = SymbolTable::getClass(asObj->getClassName());
      size_t i = 0;

      if (cl->getGenerics().size() != unqal.size()) {
         return;
      }

      for (const auto& gen : cl->getGenerics()) {
         asObj->specifyGenericType(gen->getGenericClassName(), unqal.at(i));
         ++i;
      }

      unqal.clear();
   }

   unordered_map<string, Type*> Type::resolveUnqualified(std::vector<Type*>& given, std::vector<ObjectType*>& needed) {
      assert(given.size() == needed.size() && "should be checked before");

      unordered_map<string, Type*> resolved;

      size_t i = 0;
      for (const auto& gen : needed) {
         resolved.emplace(gen->getGenericClassName(), given[i]);
         ++i;
      }

      return resolved;
   }

   void Type::CopyProperties(Type *src, Type *dst) {
      dst->isNull_ = src->isNull_;
      dst->isNullable_ = src->isNullable_;
      dst->isInferred_ = src->isInferred_;
      dst->isConst_ = src->isConst_;
      dst->lvalue = src->lvalue;
      dst->hasDefaultArg = src->hasDefaultArg;
      dst->vararg = src->vararg;
      dst->cstyleVararg = src->cstyleVararg;
      dst->cstyleArray = src->cstyleArray;
      dst->carrayElement = src->carrayElement;
   }

   namespace {
      ObjectType* typeIsGeneric(
         std::vector<ObjectType*>& generics,
         Type*& obj)
      {
         if (!obj->isObject() || generics.empty()) {
            return nullptr;
         }

         auto index = std::find_if(generics.begin(), generics.end(), [obj](ObjectType* gen) {
            return gen->getGenericClassName() == obj->getClassName();
         });

         if (index != generics.end()) {
            return *index;
         }

         return nullptr;
      }
   }

   bool Type::resolve(
      Type **ty,
      string& className,
      std::vector<ObjectType*>* generics,
      std::vector<string>& namespaces)
   {
      for (const auto& cont : (*ty)->getTypeReferences()) {
         if (!resolve(cont, className, generics, namespaces)) {
            return false;
         }
      }

      SymbolTable::resolveTypedef(*ty, namespaces);

      if (isa<ObjectType>(*ty) && (*ty)->getClassName() == "Self" && !className.empty()) {
         *ty = ObjectType::get(className);
         (*ty)->isGeneric(true);
         (*ty)->setGenericClassName("Self");
         (*ty)->setContravariance(ObjectType::get(className));
         (*ty)->hasSelfRequirement(true);
      }

      if ((*ty)->isObject()) {
         auto asObj = cast<ObjectType>(*ty);
         auto& unqual = asObj->getUnqualifiedGenerics();
         if (!unqual.empty()) {
            auto cl = SymbolTable::getClass(asObj->getClassName(), namespaces);
            if (cl->getGenerics().size() != unqual.size()) {
               return false;
            }

            size_t i = 0;
            for (const auto& gen : cl->getGenerics()) {
               resolve(&unqual[i], className, generics, namespaces);
               asObj->specifyGenericType(gen->getGenericClassName(), unqual[i]);
               ++i;
            }

            unqual.clear();
         }
      }

      if (generics != nullptr) {
         if (auto gen = typeIsGeneric(*generics, *ty)) {
            auto backup = *ty;
            *ty = gen->deepCopy();
            Type::CopyProperties(backup, *ty);
         }
      }

      if ((*ty)->isObject()) {
         auto& declaredClassName = (*ty)->getClassName();
         if (SymbolTable::hasClass(declaredClassName, namespaces)) {
            auto cl = SymbolTable::getClass(declaredClassName, namespaces);
            auto asObj = cast<ObjectType>(*ty);
            asObj->setClassName(cl->getName());

            if (cl->isStruct()) {
               asObj->isStruct(true);
            }
            else if (cl->isEnum()) {
               asObj->isEnum(true);
            }
            else if (cl->isProtocol()) {
               asObj->isProtocol(true);
            }
         }
         else {
            return false;
         }
      }

      return true;
   }

   bool Type::operator==(Type *&other) {
      if (isNull_ && !other->isNullable_) {
         return false;
      }

      return true;
   }

   short Type::getAlignment() {
      return 8;
   }

   Type* Type::deepCopy() {
      return this;
   }

} // namespace cdot