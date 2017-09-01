//
// Created by Jonas Zell on 13.08.17.
//

#include "Type.h"
#include "ObjectType.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Visitor/StaticAnalysis/Class.h"

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
         assert(concreteGenerics.find(className) != concreteGenerics.end() && "Incompatible generics should have "
            "been caught before");

         auto backup = *ty;
         *ty = concreteGenerics.at(className)->deepCopy();
         CopyProperties(backup, *ty);
      }
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

      auto coVar = SymbolTable::getClass(needed->getClassName());
      return coVar->isBaseClassOf(given->getClassName());
   }

   void Type::resolveUnqualified(Type *ty) {

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

      assert(cl->getGenerics().size() == unqal.size() && "Should have been caught before!");

      for (const auto& gen : cl->getGenerics()) {
         asObj->specifyGenericType(gen->getGenericClassName(), unqal.at(i));
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
      dst->lengthExpr = src->lengthExpr;
      dst->lengthExprType = src->lengthExprType;
      dst->lengthExprValue = src->lengthExprValue;
      dst->constantSize = src->constantSize;
      dst->length = src->length;
   }

   bool Type::operator==(Type *&other) {
      if (isNull_ && !other->isNullable_) {
         return false;
      }

      return true;
   }

   Type* Type::visitLengthExpr(TypeCheckVisitor *v) {
      if (lengthExprType != nullptr) {
         return lengthExprType;
      }

      lengthExprType = lengthExpr->accept(*v);
      return lengthExprType;
   }

   llvm::Value* Type::visitLengthExpr(CodeGenVisitor *v) {
      if (lengthExprValue != nullptr) {
         return lengthExprValue;
      }

      lengthExprValue = lengthExpr->accept(*v);
      return lengthExprValue;
   }

   short Type::getAlignment() {
      return 8;
   }

   Type* Type::deepCopy() {
      return this;
   }

} // namespace cdot