//
// Created by Jonas Zell on 28.08.17.
//

#include "TupleType.h"
#include "../../Util.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/StaticAnalysis/Class.h"
#include "../../AST/Passes/CodeGen/CGMemory.h"
#include "../../AST/Expression/TypeRef.h"

namespace cdot {

   unordered_map<string, llvm::StructType*> TupleType::TupleTypes = {};

   TupleType::TupleType(std::vector<pair<string, std::shared_ptr<TypeRef>>>& rawTypes) :
      rawTypes(rawTypes),
      arity(rawTypes.size()),
      align(1),
      size(0)
   {
      id = TypeID::TupleTypeID;
      className = "tuple.";
   }

   TupleType::TupleType(std::vector<Type *> &containedTypes) :
      arity(containedTypes.size()),
      align(1),
      size(0)
   {
      id = TypeID::TupleTypeID;
      className = "tuple.";
      for (const auto& ty : containedTypes) {
         this->containedTypes.emplace_back("", ty);
         string str = ty->toString();
         className += std::to_string(str.length()) + str;

         auto al = ty->getAlignment();
         if (al > align) {
            align = al;
         }

         size += ty->getSize();
      }
   }

   TupleType::TupleType(std::vector<pair<string, Type*>> &containedTypes) :
      arity(containedTypes.size()),
      align(1),
      size(0)
   {
      id = TypeID::TupleTypeID;
      className = "tuple.";
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

   TupleType::~TupleType() {
      for (const auto& cont : containedTypes) {
         delete cont.second;
      }
   }

   void TupleType::visitContained(TypeCheckPass &v) {
      for (const auto& raw : rawTypes) {
         raw.second->accept(v);
         containedTypes.emplace_back(raw.first, raw.second->getType()->deepCopy());
      }

      for (const auto& ty : containedTypes) {
         string str = ty.second->toString();
         className += std::to_string(str.length()) + str;

         auto al = ty.second->getAlignment();
         if (al > align) {
            align = al;
         }

         size += ty.second->getSize();
      }
   }

   Type* TupleType::deepCopy() {
      auto newTy = new TupleType(*this);

      for (size_t i = 0; i < containedTypes.size(); ++i) {
         newTy->containedTypes[i].second = newTy->containedTypes[i].second->deepCopy();
      }

      return newTy;
   }

   bool TupleType::operator==(Type *&other) {
      if (other->isTupleTy()) {
         auto asTuple = cast<TupleType>(other);
         if (asTuple->arity != arity) {
            return false;
         }

         for (size_t i = 0; i < containedTypes.size(); ++i) {
            if (!containedTypes[i].second->operator==(asTuple->containedTypes[i].second)) {
               return false;
            }
         }

         return true;
      }

      return false;
   }

   bool TupleType::implicitlyCastableTo(Type *other) {
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

   std::vector<Type*> TupleType::getContainedTypes(bool includeSelf) {
      std::vector<Type*> cont;
      if (includeSelf) {
         cont.push_back(this);
      }

      for (auto& ty : containedTypes) {
         cont.push_back(ty.second);
      }

      return cont;
   }

   std::vector<Type**> TupleType::getTypeReferences() {
      std::vector<Type**> cont;
      cont.reserve(containedTypes.size());

      for (auto& ty : containedTypes) {
         cont.push_back(&ty.second);
      }

      return cont;
   }

   string TupleType::_toString() {
      string str = "(";
      size_t i = 0;

      for (const auto& ty : containedTypes) {
         str += ty.second->toString();
         if (i++ < containedTypes.size() - 1) {
            str += ", ";
         }
      }

      return str + ")";
   }

   llvm::Type* TupleType::_getLlvmType() {
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