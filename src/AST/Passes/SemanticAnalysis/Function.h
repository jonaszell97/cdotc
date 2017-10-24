//
// Created by Jonas Zell on 12.08.17.
//

#ifndef CDOT_FUNCTION_H
#define CDOT_FUNCTION_H

#include "../../../Variant/Variant.h"
#include "../../../Variant/Type/BuiltinType.h"
#include "../../../Variant/Type/Generic.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Token.h"

class FunctionDecl;
class MethodDecl;

enum class AccessModifier : unsigned int;

using std::string;
using namespace cdot;

namespace cdot {
struct Argument {
   Argument(string label, Type ty, std::shared_ptr<Expression> defVal = nullptr, bool vararg = false, bool
      cstyleVararg = false)
      : label(label), type(ty), defaultVal(defVal), isVararg(vararg), cstyleVararg(cstyleVararg)
   {

   }

   string label;
   Type type;
   std::shared_ptr<Expression> defaultVal;

   bool isVararg = false;
   bool cstyleVararg = false;
   bool ref = false;
};

class Callable {
public:
   const string &getName() const;
   void setName(const string &name);

   const string &getMangledName() const;
   void setMangledName(const string &mandledName);

   AccessModifier getAccessModifier() const;
   void setAccessModifier(AccessModifier accessModifier);

   Type &getReturnType();
   void setReturnType(const Type &returnType);

   std::vector<Argument> &getArguments();
   void setArguments(const std::vector<Argument> &arguments);

   std::vector<GenericConstraint> &getGenerics();
   void setGenerics(const std::vector<GenericConstraint> &generics);

   bool hasStructReturn() const;
   void hasStructReturn(bool has_struct_return);

   unsigned int getUses() const;
   void setUses(unsigned int uses);

   const std::vector<BuiltinType*> &getThrownTypes() const;

   void copyThrows(Callable *callable);
   void addThrownTypes(const std::vector<BuiltinType*> &tys);
   void addThrownType(BuiltinType *ty);

   bool throws(BuiltinType *ty);
   bool throws();

   bool isNoThrow();
   void isNoThrow(bool nothrow);

   llvm::Function *getLlvmFunc() const;

   void setLlvmFunc(llvm::Function *llvmFunc);

   void addUse()
   {
      ++uses;
   }

protected:
   Callable(
      string &&name,
      AccessModifier am,
      Type &&returnType,
      std::vector<Argument> &&arguments,
      std::vector<GenericConstraint> &&generics
   );

   string name;
   string mangledName;

   AccessModifier  accessModifier;

   Type returnType;
   std::vector<Argument> arguments;
   std::vector<GenericConstraint> generics;

   bool is_nothrow = false;
   std::vector<BuiltinType*> thrownTypes;

   bool has_struct_return = false;
   unsigned uses = 0;

   llvm::Function *llvmFunc;
};

namespace cl {

class Class;

struct Method: public Callable {
   Method(
      string name,
      Type& ret_type,
      AccessModifier access_modifier,
      std::vector<Argument>&& args,
       std::vector<GenericConstraint>& generics,
      bool isStatic,
      MethodDecl *decl,
      SourceLocation loc
   );

   Method(
      string name,
      Type& ret_type,
      std::vector<Argument>&& args,
      std::vector<GenericConstraint>& generics,
      MethodDecl *decl,
      SourceLocation loc
   );

   typedef std::unique_ptr<Method> UniquePtr;
   typedef std::shared_ptr<Method> SharedPtr;

   long constraintIndex = -1;

   bool isStatic = false;
   bool isProtocolMethod = false;
   bool isVirtual = false;
   bool mutableSelf = false;

   bool isProtocolDefaultImpl = false;
   bool hasDefinition = false;
   string protocolName;

   MethodDecl* declaration;
   SourceLocation loc;

   Class* owningClass = nullptr;
};
}
}

class Function: public Callable {
public:
   Function(string&, Type&, std::vector<GenericConstraint>&);
   Function(string&, Type&);

   void addArgument(Type type, string& name) {
      arguments.emplace_back(name, type);
   }
   
   void addArgument(
      Type type,
      std::shared_ptr<Expression> def_val,
      string& name)
   {
      arguments.emplace_back(name, type, def_val);
   }

   void addArgument(Argument& arg)
   {
      arguments.push_back(arg);
   }

   void setDecl(FunctionDecl *decl)
   {
      this->decl = decl;
   }

   FunctionDecl *getDecl()
   {
      return decl;
   }

   typedef std::unique_ptr<Function> UniquePtr;

protected:
   FunctionDecl *decl;
};


#endif //CDOT_FUNCTION_H
