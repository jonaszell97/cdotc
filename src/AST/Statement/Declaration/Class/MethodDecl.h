//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_MEMBERDECL_H
#define CDOT_MEMBERDECL_H

#include "../CallableDecl.h"
#include "../../../../Basic/Precedence.h"

namespace cdot {

enum class FixKind : unsigned char;
enum class Associativity : unsigned char;

namespace ast {

class TypeRef;
class CompoundStmt;
class FuncArgDecl;

class MethodDecl : public CallableDecl {
public:
   MethodDecl(string &&methodName,
              TypeRef* returnType,
              std::vector<FuncArgDecl* > &&args,
              std::vector<StaticExpr* > &&Constraints,
              CompoundStmt* body,
              AccessModifier access,
              bool isStatic = false)
      : CallableDecl(MethodDeclID, access, std::move(methodName), returnType,
                     std::move(args), move(Constraints),
                     body, {})
   {
      setFlag(Abstract, !body);
      setDeclFlag(DF_Static, isStatic);
   }

   MethodDecl(string &&methodName,
              TypeRef* returnType,
              std::vector<FuncArgDecl* > &&args,
              std::vector<StaticExpr* > &&Constraints,
              CompoundStmt* body,
              OperatorInfo op,
              bool isCastOp,
              AccessModifier access,
              bool isStatic)
      : CallableDecl(MethodDeclID, access, std::move(methodName), returnType,
                     std::move(args), move(Constraints),
                     body, op)
   {
      setFlag(Abstract, !body);
      setFlag(ConvOp, isCastOp);
      setDeclFlag(DF_Static, isStatic);
   }

   MethodDecl(string &&methodName,
              string &&aliasedMethod,
              std::vector<FuncArgDecl* > &&args)
      : CallableDecl(MethodDeclID, (AccessModifier)0,
                     std::move(methodName),
                     {}, std::move(args), {}, {}, {}),
        alias(move(aliasedMethod))
   {
      setFlag(Alias, true);
   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(AstNode::NodeType kind)
   {
      switch (kind) {
         case MethodDeclID:
         case InitDeclID:
         case DeinitDeclID:
            return true;
         default:
            return false;
      }
   }

   static DeclContext *castToDeclContext(MethodDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<MethodDecl*>(D));
   }

   static MethodDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<MethodDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;

protected:
   MethodDecl(NodeType typeID,
              string &&methodName,
              TypeRef* returnType,
              std::vector<FuncArgDecl* > &&args,
              std::vector<StaticExpr* > &&Constraints,
              CompoundStmt* body,
              AccessModifier access,
              bool isStatic = false)
      : CallableDecl(typeID, access, std::move(methodName), returnType,
                     std::move(args), move(Constraints),
                     body, {})
   {
      setFlag(Abstract, !body);
      setDeclFlag(DF_Static, isStatic);
   }

   string alias;

   size_t methodID;
   size_t protocolTableOffset = 0;

   std::vector<ast::CallableDecl* > Instantiations;

public:
   const string &getAlias() const
   {
      return alias;
   }

   size_t getMethodID() const
   {
      return methodID;
   }

   size_t getProtocolTableOffset() const
   {
      return protocolTableOffset;
   }

   void setProtocolTableOffset(size_t protocolTableOffset)
   {
      MethodDecl::protocolTableOffset = protocolTableOffset;
   }

   bool isAbstract() const
   {
      return getFlag(Abstract);
   }

   bool isAlias() const
   {
      return getFlag(Alias);
   }

   bool hasMutableSelf() const
   {
      return getFlag(MutableSelf);
   }

   bool isProtocolMethod() const
   {
      return getFlag(ProtoMethod);
   }

   bool isVirtual() const
   {
      return getFlag(Virtual);
   }

   bool isOverride() const
   {
      return getFlag(Override);
   }

   bool isProperty() const
   {
      return getFlag(Property);
   }

   bool isTemplatedInitializer() const;

   bool isProtocolDefaultImpl() const
   {
      return getFlag(ProtoMethod);
   }

   bool isHasDefinition() const
   {
      return getFlag(Defined);
   }

   bool isMemberwiseInitializer() const
   {
      return getFlag(MemberwiseInit);
   }

   bool isDefaultInitializer() const
   {
      return getFlag(DefaultInit);
   }

   void setDefaultInitializer(bool defaultInitializer)
   {
      setFlag(DefaultInit, defaultInitializer);
   }

   const std::vector<CallableDecl *> &getInstantiations() const
   {
      return Instantiations;
   }

   bool isCastOp() const
   {
      return getFlag(ConvOp);
   }

   void setMutating(bool mutating)
   {
      setFlag(MutableSelf, mutating);
   }

   void setIsProtocolMethod(bool is_protocol_method)
   {
      setFlag(ProtoMethod, is_protocol_method);
   }

   void setIsVirtual(bool is_virtual)
   {
      setFlag(Virtual, is_virtual);
   }

   void setProperty(bool property)
   {
      setFlag(Property, property);
   }

   void setProtocolDefaultImpl(bool protocolDefaultImpl)
   {
      setFlag(ProtoDefaultImpl, protocolDefaultImpl);
   }

   void setMemberwiseInitializer(bool memberwiseInitializer)
   {
      setFlag(MemberwiseInit, memberwiseInitializer);
   }
};

class InitDecl: public MethodDecl {
public:
   InitDecl()
      : MethodDecl(InitDeclID, "init", nullptr, {}, {}, nullptr,
                   (AccessModifier)0),
        memberwise(true)
   {
      setMemberwiseInitializer(true);
   }

   InitDecl(std::vector<FuncArgDecl* > &&args,
              AccessModifier am,
              CompoundStmt* body,
              string &&name = "init")
      : MethodDecl(InitDeclID, move(name), {}, std::move(args),
                   {}, body, am),
        memberwise(false)
   {}

   std::vector<TemplateParamDecl*> &getTemplateParamsRef()
   {
      return templateParams;
   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == InitDeclID;
   }

   static DeclContext *castToDeclContext(InitDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<InitDecl*>(D));
   }

   static InitDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<InitDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;

protected:
   bool memberwise : 1;

public:
   bool isMemberwise() const
   {
      return memberwise;
   }
};

class DeinitDecl: public MethodDecl {
public:
   explicit DeinitDecl(CompoundStmt* body = nullptr)
      : MethodDecl(DeinitDeclID, "deinit", {}, {}, {}, body,
                   (AccessModifier)0)
   {

   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == DeinitDeclID;
   }

   static DeclContext *castToDeclContext(DeinitDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<DeinitDecl*>(D));
   }

   static DeinitDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<DeinitDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_MEMBERDECL_H
