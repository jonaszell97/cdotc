//
// Created by Jonas Zell on 12.08.17.
//

#ifndef CDOT_FUNCTION_H
#define CDOT_FUNCTION_H

#include <pngconf.h>
#include "../../../Variant/Variant.h"
#include "../../../Variant/Type/BuiltinType.h"
#include "../../../Variant/Type/Generic.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Token.h"
#include "../../Attribute/Attribute.h"

#include "SemaPass.h"

class FunctionDecl;
class MethodDecl;
class CompoundStmt;

enum class AccessModifier : unsigned int;

using std::string;
using namespace cdot;

namespace cdot {

class Callable: public AttributeOwner {
public:
   const string &getName() const;
   void setName(const string &name);

   const string &getMangledName() const;
   void setMangledName(const string &mandledName);

   AccessModifier getAccessModifier() const;
   void setAccessModifier(AccessModifier accessModifier);

   Type &getReturnType();
   void setReturnType(const Type &returnType);

   virtual std::vector<Argument> &getArguments();
   void setArguments(const std::vector<Argument> &arguments);

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

   cl::CallableTemplate *getTemplate()
   {
      return Template;
   }

   void setTemplate(cl::CallableTemplate *Template)
   {
      Callable::Template = Template;
   }

   llvm::Function *getLlvmFunc() const;

   void setLlvmFunc(llvm::Function *llvmFunc);

   void addUse()
   {
      ++uses;
   }

   void setDecl(CallableDecl *decl)
   {
      declaration = decl;
   }

   CallableDecl *getDeclaration()
   {
      return declaration;
   }

   bool isTemplate() const
   {
      return is_template;
   }

   void isTemplate(bool is_template)
   {
      Callable::is_template = is_template;
   }

protected:
   Callable(
      string &&name,
      AccessModifier am,
      const Type &returnType,
      std::vector<Argument> &&arguments
   );

   explicit Callable(cl::CallableTemplate *Template);

   string name;
   string mangledName;

   AccessModifier  accessModifier;

   Type returnType;
   std::vector<Argument> arguments;

   bool is_nothrow = false;
   std::vector<BuiltinType*> thrownTypes;

   bool has_struct_return = false;
   unsigned uses = 0;

   bool is_template = false;

   llvm::Function *llvmFunc;
   CallableDecl* declaration;
   cl::CallableTemplate *Template = nullptr;
};

namespace cl {

class Class;

struct Method: public Callable {
   Method(
      string name,
      const Type& ret_type,
      AccessModifier access_modifier,
      std::vector<Argument>&& args,
      bool isStatic,
      MethodDecl *decl,
      SourceLocation loc,
      unsigned id
   );

   explicit Method(
      MethodTemplate *Template
   );

   Method(
      string name,
      const Type& ret_type,
      std::vector<Argument>&& args,
      MethodDecl *decl,
      SourceLocation loc,
      unsigned id
   );

   ~Method();

   const string &getSelfBinding() const
   {
      return selfBinding;
   }

   void setSelfBinding(const string &selfBinding)
   {
      Method::selfBinding = selfBinding;
   }

   void isStatic(bool stat)
   {
      is_static = stat;
   }

   bool isStatic() const
   {
      return is_static;
   }

   bool isProtocolMethod() const
   {
      return is_protocol_method;
   }

   void isProtocolMethod(bool proto)
   {
      is_protocol_method = proto;
   }

   bool isVirtual() const
   {
      return is_virtual;
   }

   void isVirtual(bool virt)
   {
      is_virtual = virt;
   }

   bool isInitializer() const
   {
      return is_initializer;
   }

   void isInitializer(bool init)
   {
      is_initializer = init;
   }

   unsigned getMethodID() const
   {
      return methodID;
   }

   cl::MethodTemplate *getMethodTemplate()
   {
      return static_cast<MethodTemplate*>(Template);
   }

   AstNode *getTemplateOrMethodDecl();

   typedef std::unique_ptr<Method> UniquePtr;
   typedef std::shared_ptr<Method> SharedPtr;

   long constraintIndex = -1;
   unsigned methodID;

   bool is_static = false;
   bool is_protocol_method = false;
   bool is_virtual = false;
   bool mutableSelf = false;

   bool is_initializer = false;
   bool isProtocolDefaultImpl = false;
   bool hasDefinition = false;

   string protocolName;
   string selfBinding;

   SourceLocation loc;

   Record* owningClass = nullptr;
};
}
}

class Function: public Callable {
public:
   Function(string&, const Type&);
   explicit Function(cl::CallableTemplate *Template);

   ~Function();

   void addArgument(Argument &&arg)
   {
      arguments.push_back(std::move(arg));
   }

   AstNode *getTemplateOrFunctionDecl() const;

   typedef std::unique_ptr<Function> UniquePtr;
};


#endif //CDOT_FUNCTION_H
