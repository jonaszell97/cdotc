//
// Created by Jonas Zell on 12.08.17.
//

#ifndef CDOT_FUNCTION_H
#define CDOT_FUNCTION_H

#include "../../../Variant/Variant.h"
#include "../../../Variant/Type/Type.h"

using std::string;
using namespace cdot;

namespace cdot {
   struct Argument {
      Argument(string label, Type* ty, std::shared_ptr<Expression> defVal = nullptr) :
         label(label), type(ty), defaultVal(defVal)
      {
         if (ty != nullptr) {
            isVararg = ty->isVararg();
            cstyleVararg = ty->isCStyleVararg();
         }
      }

      string label;
      Type* type;
      std::shared_ptr<Expression> defaultVal;

      bool isVararg = false;
      bool cstyleVararg = false;
   };
}

class Function {
public:
   Function(string&, Type*, std::vector<ObjectType*>&);
   Function(string&, Type*);

   void addArgument(Type* type, string& name) {
      arguments.push_back(Argument{ name, type });
   }
   
   void addArgument(Type* type, std::shared_ptr<Expression> def_val, string& name) {
      arguments.push_back(Argument{ name, type, def_val });
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

   std::vector<Argument>& getArguments() {
      return arguments;
   }

   void hasHiddenParam(bool b) {
      hasHiddenParam_ = b;
   }

   bool& hasHiddenParam() {
      return hasHiddenParam_;
   }

   std::vector<ObjectType*>& getGenerics() {
      return generics;
   }

   bool isLambda() {
      return isLambda_;
   }

   void addUse() {
      ++uses;
   }

   size_t& getNumUses() {
      return uses;
   }

   typedef std::unique_ptr<Function> UniquePtr;

protected:
   string name;
   string mangledName;

   Type* returnType;
   std::vector<Argument> arguments;
   std::vector<ObjectType*> generics;

   size_t uses = 0;

   bool isLambda_;
   bool hasHiddenParam_ = false;
};


#endif //CDOT_FUNCTION_H
