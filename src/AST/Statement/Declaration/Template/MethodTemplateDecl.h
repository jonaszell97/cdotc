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

namespace cdot {
namespace ast {

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

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == MethodTemplateDeclID;
   }

protected:
   bool is_static;
   bool is_operator;
   bool is_mutating;

   cdot::cl::RecordTemplateInstantiation *outerRecord = nullptr;

   cdot::cl::Method *method;

public:
   bool isStatic() const;
   void isStatic(bool is_static);

   bool isOperator() const;
   void isOperator(bool is_operator);

   bool isMutating() const;
   void isMutating(bool is_mutating);

   cdot::cl::RecordTemplateInstantiation *getOuterRecord() const;
   void setOuterRecord(cdot::cl::RecordTemplateInstantiation *outerRecord);

   cdot::cl::Method *getMethod() const;
   void setMethod(cdot::cl::Method *method);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_METHODTEMPLATEDECL_H
