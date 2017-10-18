//
// Created by Jonas Zell on 28.08.17.
//

#include "TupleType.h"
#include "../../Util.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/StaticAnalysis/Record/Class.h"
#include "../../AST/Passes/CodeGen/CGMemory.h"
#include "../../AST/Expression/TypeRef.h"

namespace cdot {

   unordered_map<string, TupleType*> TupleType::Instances;
   unordered_map<string, llvm::StructType*> TupleType::TupleTypes;

   TupleType* TupleType::get(std::vector<pair<string, BuiltinType*>>& containedTypes)
   {
      auto key = typesToString(containedTypes);
      if (Instances.find(key) == Instances.end()) {
         Instances[key] = new TupleType(containedTypes, key);
      }

      return Instances[key];
   }

   TupleType::TupleType(std::vector<pair<string, BuiltinType*>> &containedTypes, string& className) :
      arity(containedTypes.size()),
      align(1),
      size(0)
   {
      id = TypeID::TupleTypeID;
      this->className = className;

      for (const auto& ty : containedTypes) {
         this->containedTypes.push_back(ty);
         string str = ty.second->toString();
         className += std::to_string(str.length()) + str;

         auto al = ty.second->getAlignment();
         if (al > align) {
            align = al;
         }

         size += ty.second->getSize();
      }
   }

   string TupleType::typesToString(
      const std::vector<pair<string, BuiltinType *>> &containedTypes)
   {
      string str = "(";
      size_t i = 0;

      for (const auto& ty : containedTypes) {
         str += ty.second->toString();
         if (i < containedTypes.size() - 1) {
            str += ", ";
         }

         ++i;
      }

      return str + ")";
   }

   bool TupleType::implicitlyCastableTo(BuiltinType *other)
   {
      if (other->isTupleTy()) {
         auto asTuple = cast<TupleType>(other);
         if (asTuple->arity != arity) {
            return false;
         }

         for (size_t i = 0; i < containedTypes.size(); ++i) {
            if (!containedTypes[i].second->implicitlyCastableTo(asTuple->containedTypes[i].second)) {
               return false;
            }
         }

         return true;
      }

      return false;
   }

   string TupleType::toString()
   {
      return className;
   }

   llvm::Type* TupleType::getLlvmType()
   {
      if (TupleTypes.find(className) != TupleTypes.end()) {
         return TupleTypes[className];
      }

      std::vector<llvm::Type*> cont;
      cont.reserve(containedTypes.size());

      for (const auto& ty : containedTypes) {
         cont.push_back(ty.second->getLlvmType());
      }

      llvm::StructType* tupleTy = llvm::StructType::get(CodeGen::Context, cont, true);
      TupleTypes.emplace(className, tupleTy);

      return tupleTy;
   }
}