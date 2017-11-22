//
// Created by Jonas Zell on 29.08.17.
//

#ifndef CDOT_ENUMDECL_H
#define CDOT_ENUMDECL_H

#include "RecordDecl.h"

namespace cdot {

struct TemplateConstraint;
enum class AccessModifier : unsigned int;

namespace cl {

struct Method;
class Enum;

}

namespace ast {

class MethodDecl;
class EnumCaseDecl;

class EnumDecl: public RecordDecl {
public:
   EnumDecl(
      AccessModifier am,
      string&& enumName,
      std::shared_ptr<TypeRef>& rawType,
      std::vector<std::shared_ptr<Statement>>&& methods,
      std::vector<std::shared_ptr<PropDecl>> &&properties,
      std::vector<std::shared_ptr<TypeRef>>&& conformsTo,
      std::vector<std::shared_ptr<EnumCaseDecl>>&& cases,
      std::vector<Statement::SharedPtr>&& innerDeclarations
   );

   typedef std::shared_ptr<EnumDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == EnumDeclID;
   }

protected:
   std::vector<std::shared_ptr<EnumCaseDecl>> cases;
   std::shared_ptr<TypeRef> rawType;

   // codegen
   string selfBinding;

public:
   const std::vector<std::shared_ptr<EnumCaseDecl>> &getCases() const
   {
      return cases;
   }

   void setCases(const std::vector<std::shared_ptr<EnumCaseDecl>> &cases)
   {
      EnumDecl::cases = cases;
   }

   const std::shared_ptr<TypeRef> &getRawType() const
   {
      return rawType;
   }

   void setRawType(const std::shared_ptr<TypeRef> &rawType)
   {
      EnumDecl::rawType = rawType;
   }

   const string &getSelfBinding() const
   {
      return selfBinding;
   }

   void setSelfBinding(const string &selfBinding)
   {
      EnumDecl::selfBinding = selfBinding;
   }

   cdot::cl::Enum *getDeclaredEnum() const;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_ENUMDECL_H
