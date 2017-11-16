//
// Created by Jonas Zell on 13.08.17.
//

#include "BuiltinType.h"
#include "ObjectType.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Class.h"

#include "IntegerType.h"
#include "PointerType.h"
#include "FunctionType.h"
#include "TupleType.h"
#include "GenericType.h"
#include "FPType.h"


namespace cdot {

const PointerType* BuiltinType::asPointerTy() const
{
   return cast<PointerType>(this);
}

const IntegerType* BuiltinType::asIntegerTy() const
{
   return cast<IntegerType>(this);
}

const FPType* BuiltinType::asFloatingTy() const
{
   return cast<FPType>(this);
}

const TupleType* BuiltinType::asTupleTy() const
{
   return cast<TupleType>(this);
}

const FunctionType* BuiltinType::asFunctionTy() const
{
   return cast<FunctionType>(this);
}

const ObjectType* BuiltinType::asObjTy() const
{
   return cast<ObjectType>(this);
}

const GenericType* BuiltinType::asGenericTy() const
{
   return cast<GenericType>(this);
}

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

bool BuiltinType::needsCastTo(const BuiltinType *ty) const
{
   return this != ty;
}

bool BuiltinType::isBoxedPrimitive() const
{
   return isObject() && util::matches(
      "(Float|Double|U?Int(1|8|16|32|64)?)",
      className
   );
}

short BuiltinType::getAlignment() const
{
   return 8;
}

} // namespace cdot