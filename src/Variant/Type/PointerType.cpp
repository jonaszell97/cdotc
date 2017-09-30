//
// Created by Jonas Zell on 13.08.17.
//

#include "PointerType.h"
#include "../../AST/Passes/CodeGen/CGMemory.h"
#include "../../AST/Expression/Expression.h"
#include "IntegerType.h"
#include "ObjectType.h"
#include "FPType.h"
#include "VoidType.h"

namespace cdot {

   PointerType::PointerType(Type *pointeeType) :
      pointeeType(pointeeType)
   {
      id = TypeID::PointerTypeID;
   }

   bool PointerType::operator==(Type *&other) {
      switch (other->getTypeID()) {
         case TypeID::PointerTypeID: {
            auto asPtr = cast<PointerType>(other);
            return *pointeeType == asPtr->pointeeType && Type::operator==(other);
         }
         default:
            return false;
      }
   }

   bool PointerType::isLvalue() {
      return lvalue;
   }

   std::vector<Type*> PointerType::getContainedTypes(bool includeSelf) {
      if (includeSelf) {
         return { this, pointeeType };
      }

      return { pointeeType };
   }

   std::vector<Type**> PointerType::getTypeReferences() {
      return { &pointeeType };
   }

   Type* PointerType::deepCopy() {
      auto newTy = new PointerType(*this);
      newTy->pointeeType = pointeeType->deepCopy();

      return newTy;
   }

   bool PointerType::implicitlyCastableTo(Type *other) {
      if (isa<PointerType>(other)) {
         return pointeeType->implicitlyCastableTo(cast<PointerType>(other)->getPointeeType());
      }
      if (isa<VoidType>(other)) {
         return true;
      }

      return false;
   }

   bool PointerType::explicitlyCastableTo(Type *other) {
      return isa<PointerType>(other) || isa<IntegerType>(other);
   }

   llvm::Type* PointerType::_getLlvmType() {
      return pointeeType->getLlvmType()->getPointerTo();
   }

   string PointerType::_toString() {
      string res;

      if (lvalue) {
         res = "ref " + pointeeType->toString();
      }
      else {
         res = pointeeType->toString() + "*";
      }

      return res;
   }

} // namespace cdot