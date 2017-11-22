//
// Created by Jonas Zell on 12.08.17.
//

#ifndef CDOT_FUNCTION_H
#define CDOT_FUNCTION_H

#include <pngconf.h>
#include "../../../Variant/Variant.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/Generic.h"
#include "../../../Variant/Type/QualType.h"
#include "../../../lex/Token.h"
#include "../../Attribute/Attribute.h"

#include "SemaPass.h"

using std::string;
using namespace cdot;

namespace cdot {

enum class AccessModifier : unsigned int;

namespace ast {

class FunctionDecl;
class MethodDecl;
class CompoundStmt;

} // namespace ast

class Callable: public AttributeOwner {
public:
   const string &getName() const;
   void setName(const string &name);

   const string &getMangledName() const;
   virtual string getLinkageName() const;
   void setMangledName(const string &mandledName);

   AccessModifier getAccessModifier() const;
   void setAccessModifier(AccessModifier accessModifier);

   QualType &getReturnType();
   void setReturnType(const QualType &returnType);

   virtual std::vector<Argument> &getArguments();
   void setArguments(const std::vector<Argument> &arguments);

   bool hasStructReturn() const;
   void hasStructReturn(bool has_struct_return);

   unsigned int getUses() const;
   void setUses(unsigned int uses);

   const std::vector<Type*> &getThrownTypes() const;

   void copyThrows(Callable *callable);
   void addThrownTypes(const std::vector<Type*> &tys);
   void addThrownType(Type *ty);

   bool throws(Type *ty);
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

   void setDecl(ast::CallableDecl *decl)
   {
      declaration = decl;
   }

   ast::CallableDecl *getDeclaration()
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
      const QualType &returnType,
      std::vector<Argument> &&arguments
   );

   explicit Callable(cl::CallableTemplate *Template);

   string name;
   string mangledName;

   AccessModifier  accessModifier;

   QualType returnType;
   std::vector<Argument> arguments;

   bool is_nothrow = false;
   std::vector<Type*> thrownTypes;

   bool has_struct_return = false;
   unsigned uses = 0;

   bool is_template = false;

   llvm::Function *llvmFunc;
   ast::CallableDecl* declaration = nullptr;
   cl::CallableTemplate *Template = nullptr;
};

namespace cl {

class Class;

struct Method: public Callable {
   Method(
      string name,
      const QualType& ret_type,
      AccessModifier access_modifier,
      std::vector<Argument>&& args,
      bool isStatic,
      ast::MethodDecl *decl,
      SourceLocation loc,
      unsigned id
   );

   explicit Method(
      MethodTemplate *Template
   );

   Method(
      string name,
      const QualType& ret_type,
      std::vector<Argument>&& args,
      ast::MethodDecl *decl,
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

   bool isProperty() const
   {
      return property;
   }

   void setIsProperty(bool prop)
   {
      property = prop;
   }

   string getLinkageName() const override;

   ast::AstNode *getTemplateOrMethodDecl();

   typedef std::unique_ptr<Method> UniquePtr;
   typedef std::shared_ptr<Method> SharedPtr;

   long constraintIndex = -1;
   unsigned methodID;

   bool is_static = false;
   bool is_protocol_method = false;
   bool is_virtual = false;
   bool mutableSelf = false;
   bool property = false;

   bool is_initializer = false;
   bool isProtocolDefaultImpl = false;
   bool hasDefinition = false;

   string protocolName;
   string selfBinding;

   SourceLocation loc;

   Record* owningClass = nullptr;
};

} // namespace cl

namespace ast {

class Function: public Callable {
public:
   Function(string&, const QualType&);
   explicit Function(cl::CallableTemplate *Template);

   ~Function();

   void addArgument(Argument &&arg)
   {
      arguments.push_back(std::move(arg));
   }

   ast::AstNode *getTemplateOrFunctionDecl() const;

   typedef std::unique_ptr<Function> UniquePtr;
};

} // namespace ast

} // namespace cdot


#endif //CDOT_FUNCTION_H
