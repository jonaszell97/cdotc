//
// Created by Jonas Zell on 13.08.17.
//

#include "BuiltinType.h"
#include "ObjectType.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/StaticAnalysis/Record/Class.h"

#include "IntegerType.h"
#include "PointerType.h"
#include "FunctionType.h"
#include "TupleType.h"
#include "GenericType.h"
#include "FPType.h"


namespace cdot {

   PointerType* BuiltinType::getPointerTo()
   {
      return PointerType::get(this);
   }

   PointerType* BuiltinType::asPointerTy()
   {
      return cast<PointerType>(this);
   }

   IntegerType* BuiltinType::asIntegerTy()
   {
      return cast<IntegerType>(this);
   }

   FPType* BuiltinType::asFloatingTy()
   {
      return cast<FPType>(this);
   }

   TupleType* BuiltinType::asTupleTy()
   {
      return cast<TupleType>(this);
   }

   FunctionType* BuiltinType::asFunctionTy()
   {
      return cast<FunctionType>(this);
   }

   ObjectType* BuiltinType::asObjTy()
   {
      return cast<ObjectType>(this);
   }

   GenericType* BuiltinType::asGenericTy()
   {
      return cast<GenericType>(this);
   }

   bool BuiltinType::isBoxedPrimitive()
   {
      return isObject() && util::matches(
         "(Float|Double|U?Int(1|8|16|32|64)?)",
         className
      );
   }

   short BuiltinType::getAlignment()
   {
      return 8;
   }

} // namespace cdot