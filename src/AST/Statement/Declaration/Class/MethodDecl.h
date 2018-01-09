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
   MethodDecl(
      string &&methodName,
      std::shared_ptr<TypeRef> &&returnType,
      std::vector<std::shared_ptr<FuncArgDecl>> &&args,
      std::vector<TemplateParameter> &&templateParams,
      std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
      std::shared_ptr<CompoundStmt> &&body,
      AccessModifier access,
      bool isStatic = false
   );

   MethodDecl(
      string &&methodName,
      std::shared_ptr<TypeRef> &&returnType,
      std::vector<std::shared_ptr<FuncArgDecl>> &&args,
      std::vector<TemplateParameter> &&templateParams,
      std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
      std::shared_ptr<CompoundStmt> &&body,
      OperatorInfo op,
      bool isCastOp,
      AccessModifier access,
      bool isStatic
   );

   MethodDecl(
      string &&methodName,
      std::shared_ptr<TypeRef> &&returnType,
      std::vector<std::shared_ptr<FuncArgDecl>> &&args,
      std::vector<TemplateParameter> &&templateParams,
      std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
      AccessModifier access,
      bool isStatic = false
   );

   MethodDecl(
      string &&methodName,
      string &&aliasedMethod,
      std::vector<std::shared_ptr<FuncArgDecl>> &&args
   );

   void isMutating(bool mut)
   {
      isMutating_ = mut;
   }

   void isCastOp(bool castop)
   {
      isCastOp_ = castop;
   }

   typedef std::shared_ptr<MethodDecl> SharedPtr;
   typedef std::unique_ptr<MethodDecl> UniquePtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == MethodDeclID;
   }

protected:
   bool isStatic;
   bool isAbstract;
   bool isAlias = false;

   string alias;
   bool isMutating_ = false;
   bool isCastOp_ = false;

   bool hasDefinition_ = false;
   bool is_protocol_method = false;

   FixKind fix;
   Associativity associativity;

   // codegen
   cdot::cl::Record *record;
   bool isUsed = false;

public:
   bool isProtocolMethod() const
   {
      return is_protocol_method;
   }

   void isProtocolMethod(bool proto)
   {
      is_protocol_method = proto;
   }

   bool isIsStatic() const {
      return isStatic;
   }

   void setIsStatic(bool isStatic) {
      MethodDecl::isStatic = isStatic;
   }

   bool isIsAbstract() const {
      return isAbstract;
   }

   void setIsAbstract(bool isAbstract) {
      MethodDecl::isAbstract = isAbstract;
   }

   bool isIsAlias() const {
      return isAlias;
   }

   void setIsAlias(bool isAlias) {
      MethodDecl::isAlias = isAlias;
   }

   const string &getAlias() const {
      return alias;
   }

   void setAlias(const string &alias) {
      MethodDecl::alias = alias;
   }

   bool isIsMutating_() const {
      return isMutating_;
   }

   void setIsMutating_(bool isMutating_) {
      MethodDecl::isMutating_ = isMutating_;
   }

   bool isIsCastOp_() const {
      return isCastOp_;
   }

   void setIsCastOp_(bool isCastOp_) {
      MethodDecl::isCastOp_ = isCastOp_;
   }

   bool isHasDefinition_() const {
      return hasDefinition_;
   }

   void setHasDefinition_(bool hasDefinition_) {
      MethodDecl::hasDefinition_ = hasDefinition_;
   }

   cdot::cl::Record *getRecord() const;
   void setRecord(cdot::cl::Record *record);

   cdot::cl::Method *getMethod() const;

   bool isIsUsed() const {
      return isUsed;
   }

   void setIsUsed(bool isUsed) {
      MethodDecl::isUsed = isUsed;
   }

   FixKind getFix() const
   {
      return fix;
   }

   void setFix(FixKind fix)
   {
      MethodDecl::fix = fix;
   }

   Associativity getAssociativity() const
   {
      return associativity;
   }

   void setAssociativity(Associativity associativity)
   {
      MethodDecl::associativity = associativity;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_MEMBERDECL_H
