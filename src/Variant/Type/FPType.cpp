//
// Created by Jonas Zell on 13.08.17.
//

#include "FPType.h"
#include "ObjectType.h"
#include "IntegerType.h"
#include "VoidType.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../../Token.h"

namespace cdot {

   unordered_map<unsigned, FPType*> FPType::Instances;

   FPType* FPType::get(unsigned precision)
   {
      if (Instances.find(precision) == Instances.end()) {
         Instances.emplace(precision, new FPType(precision));
      }

      return Instances[precision];
   }

   FPType* FPType::getFloatTy()
   {
      return get(32);
   }

   FPType* FPType::getDoubleTy()
   {
      return get(64);
   }

   FPType::FPType(unsigned precision) :
      precision(precision)
   {
      assert((precision == 32 || precision == 64 || precision == 16) && "Invalid FP precision!");
      id = TypeID::FPTypeID;
      switch (precision) {
         case 16:
            className = "half";
            break;
         case 32:
            className = "float";
            break;
         case 64:
            className = "double";
            break;
         default:
            llvm_unreachable("Invalid FP precision");
      }
   }

   BuiltinType* FPType::box() const
   {
      return ObjectType::get(string(1, ::toupper(className.front())) + className.substr(1));
   }

   bool FPType::implicitlyCastableTo(BuiltinType *other) const
   {
      switch (other->getTypeID()) {
         case TypeID::AutoTypeID:
            return true;
         case TypeID::FunctionTypeID:
            return false;
         case TypeID::PointerTypeID:
            return false;
         case TypeID::ObjectTypeID:
         case TypeID::GenericTypeID: {
            auto asObj = cast<ObjectType>(other);

            if (asObj->getClassName() == "Any" || asObj->getClassName() == "AnyVal") {
               return true;
            }

            string boxedCl = precision == 64 ? "Double" : "Float";

            return asObj->getClassName() == boxedCl;
         }
         case TypeID::IntegerTypeID:
            return false;
         case TypeID::FPTypeID: {
            auto asFloat = cast<FPType>(other);
            return asFloat->precision >= precision;
         }
         default:
            return false;
      }
   }

   BuiltinType* FPType::ArithmeticReturnType(string &op, BuiltinType *rhs) const
   {
      if (op == "+" || op == "-" || op == "*" || "/") {
         if (isa<FPType>(rhs)) {
            auto rhsPrecision = cast<FPType>(rhs)->getPrecision();
            return get(precision >= rhsPrecision ? precision : rhsPrecision);
         }

         if (isa<IntegerType>(rhs)) {
            return get(precision);
         }
      }

      return VoidType::get();
   }

   bool FPType::explicitlyCastableTo(BuiltinType *other) const
   {
      return isa<PrimitiveType>(other);
   }

   short FPType::getAlignment() const
   {
      return (short)(precision / 8);
   }

   llvm::Value* FPType::getDefaultVal(CodeGen &CGM) const
   {
      return llvm::ConstantFP::get(getLlvmType(), 0.0);
   }

   llvm::Constant* FPType::getConstantVal(Variant &val) const
   {
      return llvm::ConstantFP::get(getLlvmType(), val.floatVal);
   }

   llvm::Type* FPType::getLlvmType() const
   {
      switch (precision) {
         case 32:
            return llvm::Type::getFloatTy(CodeGen::Context);
         case 64:
         default:
            return llvm::Type::getDoubleTy(CodeGen::Context);
      }
   }

   string FPType::toString() const
   {
      return className;
   }

} // namespace cdot