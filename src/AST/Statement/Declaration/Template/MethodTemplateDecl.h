//
// Created by Jonas Zell on 04.11.17.
//

#ifndef CDOT_METHODTEMPLATEDECL_H
#define CDOT_METHODTEMPLATEDECL_H

#include "CallableTemplateDecl.h"

namespace cdot {
namespace cl {
struct Method;
struct RecordTemplateInstantiation;
}
}

class MethodTemplateDecl: public CallableTemplateDecl {
public:
   MethodTemplateDecl(
      string &&name,
      std::unique_ptr<TokenStore> &&Store,
      std::vector<TemplateConstraint> &&constraints,
      std::vector<std::shared_ptr<FuncArgDecl>> &&args,
      std::shared_ptr<TypeRef> &&returnType,
      bool is_static
   );

   typedef std::shared_ptr<CallableTemplateDecl> SharedPtr;

   std::vector<AstNode::SharedPtr> get_children() override
   {
      return {};
   }

   NodeType get_type() override
   {
      return NodeType::METHOD_TEMPLATE_DECL;
   }

   ASTNODE_ACCEPT_PASSES

protected:
   bool is_static;
   bool is_operator;
   bool is_mutating;

   cl::RecordTemplateInstantiation *outerRecord = nullptr;

   cl::Method *method;

public:
   bool isStatic() const;
   void isStatic(bool is_static);

   bool isOperator() const;
   void isOperator(bool is_operator);

   bool isMutating() const;
   void isMutating(bool is_mutating);

   cl::RecordTemplateInstantiation *getOuterRecord() const;
   void setOuterRecord(cl::RecordTemplateInstantiation *outerRecord);

   cl::Method *getMethod() const;
   void setMethod(cl::Method *method);
};


#endif //CDOT_METHODTEMPLATEDECL_H
