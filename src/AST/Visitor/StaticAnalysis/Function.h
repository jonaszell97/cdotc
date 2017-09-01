//
// Created by Jonas Zell on 12.08.17.
//

#ifndef CDOT_FUNCTION_H
#define CDOT_FUNCTION_H

#include "../../../Variant/Variant.h"
#include "../../../Variant/Type/Type.h"

using std::string;
using namespace cdot;

class Function {
public:
   Function(string&, Type*, std::vector<ObjectType*>&);
   Function(string&, Type*);

   void addArgument(Type* type, string& name) {
      arg_types.push_back(type);
      argNames.push_back(name);
   }
   
   void addArgument(Type* type, std::shared_ptr<Expression> def_val, string& name) {
      arg_types.push_back(type);
      arg_defaults.push_back(def_val);
      argNames.push_back(name);
   }

   string& getName() {
      return name;
   }

   string& getMangledName() {
      return mangledName;
   }

   void setMangledName(string mangled) {
      mangledName = mangled;
   }

   Type*& getReturnType() {
      return returnType;
   }

   std::vector<string>& getArgNames() {
      return argNames;
   }

   std::vector<Type*>& getArgTypes() {
      return arg_types;
   }

   std::vector<std::shared_ptr<Expression>>& getArgDefaults() {
      return arg_defaults;
   }

   void hasHiddenParam(bool b) {
      hasHiddenParam_ = b;
   }

   bool hasHiddenParam() {
      return hasHiddenParam_;
   }

   std::vector<ObjectType*>& getGenerics() {
      return generics;
   }

   bool isLambda() {
      return isLambda_;
   }

   typedef std::unique_ptr<Function> UniquePtr;

protected:
   string name;
   string mangledName;

   Type* returnType;
   std::vector<Type*> arg_types;
   std::vector<std::shared_ptr<Expression>> arg_defaults;
   std::vector<string> argNames;
   std::vector<ObjectType*> generics;

   bool isLambda_;
   bool hasHiddenParam_ = false;
};


#endif //CDOT_FUNCTION_H
