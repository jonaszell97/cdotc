//
// Created by Jonas Zell on 12.08.17.
//

#include "Function.h"


Function::Function(string& name, Type* ret_type, std::vector<GenericType*>& generics, bool isLambda) :
    returnType(ret_type),
    name(name),
    generics(generics),
    isLambda_(isLambda)
{

}