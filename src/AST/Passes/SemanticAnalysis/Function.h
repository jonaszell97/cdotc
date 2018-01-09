//
// Created by Jonas Zell on 12.08.17.
//

#ifndef CDOT_FUNCTION_H
#define CDOT_FUNCTION_H

#include "../../../Variant/Variant.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/Generic.h"
#include "../../../Variant/Type/QualType.h"
#include "../../../Basic/Precedence.h"
#include "../../../lex/Token.h"
#include "../../Attribute/Attribute.h"

#include "SemaPass.h"
#include "Template.h"

using std::string;
using namespace cdot;

namespace cdot {

struct Namespace;
enum class AccessModifier : unsigned int;

namespace ast {

class FunctionDecl;
class MethodDecl;
class CompoundStmt;

} // namespace ast

class Callable: public AttributeOwner {
public:
   enum TypeID : unsigned char {
      FunctionID,
      MethodID
   };

   const string &getName() const;
   void setName(const string &name);

   llvm::StringRef getNameWithoutNamespace() const;
   llvm::StringRef getNameWithoutFix() const;

   Namespace *getDeclarationNamespace() const
   {
      return declarationNamespace;
   }

   const string &getLinkageName() const;
   void setLinkageName(const string &linkageName);

   AccessModifier getAccessModifier() const;
   void setAccessModifier(AccessModifier accessModifier);

   QualType &getReturnType();
   void setReturnType(const QualType &returnType);

   QualType const& getReturnType() const { return returnType; }

   std::vector<Argument> &getArguments();
   std::vector<Argument> const& getArguments() const { return arguments; }

   bool isNative() const
   {
      return native;
   }

   void setNative(bool native)
   {
      Callable::native = native;
   }

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

   void addUse()
   {
      ++uses;
   }

   void setDecl(ast::CallableDecl *decl)
   {
      declaration = decl;
   }

   ast::CallableDecl *getDeclaration() const
   {
      return declaration;
   }

   bool isTemplate() const
   {
      return !templateParams.empty();
   }

   std::vector<TemplateParameter>& getTemplateParams()
   {
      return templateParams;
   }

   std::vector<TemplateParameter> const& getTemplateParams() const
   {
      return templateParams;
   }

   const sema::TemplateArgList &getTemplateArgs() const
   {
      return templateArgs;
   }

   void setTemplateArgs(sema::TemplateArgList &&templateArgs)
   {
      Callable::templateArgs = std::move(templateArgs);
   }

   const SourceLocation &getSourceLoc() const { return loc; }
   void setSourceLoc(const SourceLocation &loc) { Callable::loc = loc; }

   bool isExternC() const { return externC; }
   void setExternC(bool externC) { Callable::externC = externC; }

   const SourceLocation &getInstantiatedFrom() const
   {
      return instantiatedFrom;
   }

   void setInstantiatedFrom(const SourceLocation &instantiatedFrom)
   {
      Callable::instantiatedFrom = instantiatedFrom;
   }

   TypeID getTypeID() const
   {
      return typeID;
   }

   OperatorInfo &getOperator()
   {
      return op;
   }

   void setOperator(const OperatorInfo &op)
   {
      Callable::op = op;
   }

   bool isOperator() const
   {
      return op.getPrecedenceGroup().isValid();
   }

   Callable const* getSpecializedTemplate() const
   {
      return specializedTemplate;
   }

   void setSpecializedTemplate(Callable const* specializedTemplate)
   {
      Callable::specializedTemplate = specializedTemplate;
   }

   static bool classof(Callable const *C) { return true; }

protected:
   Callable(
      TypeID id,
      string &&name,
      AccessModifier am,
      const QualType &returnType,
      std::vector<Argument> &&arguments,
      std::vector<TemplateParameter> &&templateParams,
      OperatorInfo op,
      Namespace *declarationNamespace
   );

   TypeID typeID;

   string name;
   string linkageName;

   AccessModifier  accessModifier;

   QualType returnType;
   std::vector<Argument> arguments;

   bool is_nothrow : 1;
   bool externC : 1;
   bool native : 1;

   std::vector<Type*> thrownTypes;

   OperatorInfo op;

   unsigned uses = 0;

   SourceLocation loc;

   ast::CallableDecl* declaration = nullptr;
   Namespace *declarationNamespace = nullptr;

   SourceLocation instantiatedFrom;
   std::vector<TemplateParameter> templateParams;
   sema::TemplateArgList templateArgs;
   Callable const* specializedTemplate = nullptr;
};

namespace cl {

class Class;

struct Method: public Callable {
   Method(
      string name,
      const QualType& ret_type,
      AccessModifier access_modifier,
      std::vector<Argument>&& args,
      std::vector<TemplateParameter> &&templateParams,
      OperatorInfo op,
      bool isStatic,
      ast::CallableDecl *decl,
      SourceLocation loc,
      Namespace *declarationNamespace,
      size_t id
   );

   static bool classof(Callable const *C)
   {
      return C->getTypeID() == MethodID;
   }

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

   bool isTemplatedInitializer() const;

   size_t getMethodID() const
   {
      return methodID;
   }

   bool isProperty() const
   {
      return property;
   }

   void setIsProperty(bool prop)
   {
      property = prop;
   }

   bool isConversionOp() const
   {
      return conversionOp;
   }

   void setIsConversionOp(bool conversionOp)
   {
      Method::conversionOp = conversionOp;
   }

   bool isMemberwiseInitializer() const
   {
      return memberwiseInitializer;
   }

   void setMemberwiseInitializer(bool memberwiseInitializer)
   {
      Method::memberwiseInitializer = memberwiseInitializer;
   }

   bool hasMutableSelf() const;

   void setMutableSelf(bool mutableSelf)
   {
      Method::mutableSelf = mutableSelf;
   }

   bool isProtocolDefaultImpl() const
   {
      return protocolDefaultImpl;
   }

   void setProtocolDefaultImpl(bool protocolDefaultImpl)
   {
      Method::protocolDefaultImpl = protocolDefaultImpl;
   }

   const string &getProtocolName() const
   {
      return protocolName;
   }

   void setProtocolName(const string &protocolName)
   {
      Method::protocolName = protocolName;
   }

   Record *getOwningRecord() const
   {
      return owningRecord;
   }

   void setOwningRecord(Record *owningClass)
   {
      Method::owningRecord = owningClass;
   }

   size_t getProtocolTableOffset() const
   {
      return protocolTableOffset;
   }

   void setProtocolTableOffset(size_t protocolTableOffset)
   {
      Method::protocolTableOffset = protocolTableOffset;
   }

   const std::vector<std::shared_ptr<ast::CallableDecl>> &getInstantiations()
   const
   {
      return Instantiations;
   }

   void addInstantiation(std::shared_ptr<ast::CallableDecl> &&Inst)
   {
      Instantiations.push_back(move(Inst));
   }

   Method *hasInstantiation(const std::string &withName) const;

   typedef std::unique_ptr<Method> UniquePtr;
   typedef std::shared_ptr<Method> SharedPtr;

   long constraintIndex = -1;
   size_t methodID;
   size_t protocolTableOffset = 0;

   bool is_static : 1;
   bool is_protocol_method : 1;
   bool is_virtual : 1;
   bool mutableSelf : 1;
   bool property : 1;
   bool is_initializer : 1;
   bool protocolDefaultImpl : 1;
   bool hasDefinition : 1;
   bool conversionOp : 1;
   bool memberwiseInitializer : 1;

   string protocolName;
   string selfBinding;

   Record* owningRecord = nullptr;

   std::vector<std::shared_ptr<ast::CallableDecl>> Instantiations;
};

} // namespace cl

namespace ast {

class Function: public Callable {
public:
   Function(string &funcName,
            const QualType &returnType,
            std::vector<Argument> &&args,
            std::vector<TemplateParameter> &&templateParams,
            OperatorInfo op,
            Namespace *declarationNamespace);

   Function(llvm::StringRef funcName,
            std::vector<TemplateParameter> &&templateParams,
            OperatorInfo op,
            Namespace *declarationNamespace);

   void addArgument(Argument &&arg)
   {
      arguments.push_back(std::move(arg));
   }

   void addTemplateParam(TemplateParameter &&P)
   {
      templateParams.push_back(std::move(P));
   }

   static bool classof(Callable const *C)
   {
      return C->getTypeID() == FunctionID;
   }

   const std::vector<std::shared_ptr<FunctionDecl>> &getInstantiations() const
   {
      return Instantiations;
   }

   void addInstantiation(std::shared_ptr<FunctionDecl> &&Inst)
   {
      Instantiations.push_back(move(Inst));
   }

   Function *hasInstantiation(const std::string &withName) const;

   typedef std::unique_ptr<Function> UniquePtr;

private:
   std::vector<std::shared_ptr<ast::FunctionDecl>> Instantiations;
};

} // namespace ast

} // namespace cdot


#endif //CDOT_FUNCTION_H
