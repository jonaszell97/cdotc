//
// Created by Jonas Zell on 12.08.17.
//

#include "Function.h"


Function::Function(string& name, Type* ret_type, std::vector<ObjectType*>& generics) :
    returnType(ret_type),
    name(name),
    generics(generics),
    isLambda_(false)
{

}

Function::Function(string& name, Type* ret_type) :
   returnType(ret_type),
   name(name),
   isLambda_(true)
{

}