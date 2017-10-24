//
// Created by Jonas Zell on 30.09.17.
//

#include "Generic.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "GenericType.h"

namespace cdot {

   bool GenericTypesCompatible(
      GenericType* given,
      const GenericConstraint& needed)
   {
      string neededClassname(needed.covarName);
      if (neededClassname.empty() || neededClassname == "Any") {
         return true;
      }

      if (SymbolTable::hasClass(neededClassname) && SymbolTable::getClass(neededClassname)->isProtocol()) {
         if (given->getClassName() == neededClassname) {
            return true;
         }

         if (given->getClassName().empty()) {
            return false;
         }

         return SymbolTable::getClass(given->getClassName())->conformsTo(neededClassname);
      }

      if (!needed.contravarName.empty()) {
         auto contraVar = SymbolTable::getClass(given->getClassName());
         if (!contraVar->isBaseClassOf(needed.contravarName)) {
            return false;
         }
      }

      if (!SymbolTable::hasClass(neededClassname)) {
         return false;
      }

      auto coVar = SymbolTable::getClass(neededClassname);
      return coVar->isBaseClassOf(given->getClassName());
   }

   void resolveGenerics(
      Type& ty,
      std::vector<GenericConstraint>& generics)
   {
      if (ty->isPointerTy()) {
         auto pointee = ty->asPointerTy()->getPointeeType();
         resolveGenerics(pointee, generics);
         *ty = PointerType::get(pointee);
      }

      if (ty->isObject() && !ty->isGeneric()) {
         std::vector<GenericType*> resolvedGenerics;
         for (const auto & gen : ty->asObjTy()->getConcreteGenericTypes()) {
            if (gen->isGeneric()) {
               Type nested(gen);
               resolveGenerics(nested, generics);
               resolvedGenerics.push_back(GenericType::get(gen->getGenericClassName(), *nested));
            }
         }

         *ty = ObjectType::get(ty->getClassName(), resolvedGenerics);
      }

      if (!ty->isGeneric()) {
         return;
      }

      auto asGen = ty->asGenericTy();
      for (const auto& gen : generics) {
         if (gen.genericTypeName == asGen->getGenericClassName()) {
            string covar;
            if (!gen.covarName.empty()) {
               covar = gen.covarName;
            }
            else {
               covar = "Any";
            }

            *ty = GenericType::get(gen.genericTypeName, ObjectType::get(covar));
            break;
         }
      }
   }

   void resolveGenerics(
      Type& ty,
      Type& obj)
   {
      resolveGenerics(ty, obj->getConcreteGenericTypes());
   }

   void resolveGenerics(
      Type& ty,
      ObjectType*& obj)
   {
      resolveGenerics(ty, obj->getConcreteGenericTypes());
   }

   void resolveGenerics(
      Type& ty,
      std::vector<GenericType*>& generics)
   {
      if (ty->isGeneric()) {
         auto& genericClassName = ty->asGenericTy()->getGenericClassName();
         for (const auto &gen : generics) {
            if (gen->getGenericClassName() == genericClassName) {
               *ty = gen->getActualType();
               break;
            }
         }
      }

      // resolve nested types
      if (ty->isPointerTy()) {
         auto pointee = ty->asPointerTy()->getPointeeType();
         if (pointee->isGeneric()) {
            Type pte(pointee);
            resolveGenerics(pte, generics);

            *ty = PointerType::get(pte);
         }
      }

      if (ty->isObject()) {
         std::vector<GenericType*> resolvedGenerics;
         for (const auto & gen : ty->asObjTy()->getConcreteGenericTypes()) {
            if (gen->isGeneric()) {
               Type nested(gen);
               resolveGenerics(nested, generics);
               resolvedGenerics.push_back(GenericType::get(gen->getGenericClassName(), *nested));
            }
         }

         *ty = ObjectType::get(ty->getClassName(), resolvedGenerics);
      }

      //TODO function, tuple
   }
}