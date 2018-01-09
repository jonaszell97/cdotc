//
// Created by Jonas Zell on 29.08.17.
//

#ifndef CDOT_ENUMCASEDECL_H
#define CDOT_ENUMCASEDECL_H

#include "../../Statement.h"
#include "../../../../Variant/Variant.h"

namespace cdot {
namespace ast {

class Expression;
class TypeRef;

class EnumCaseDecl: public Statement {
public:
   typedef std::vector<pair<string, std::shared_ptr<TypeRef>>>
      AssociatedTypeVec;

   EnumCaseDecl(string&& name, std::shared_ptr<Expression>&& rawValue,
                AssociatedTypeVec&& associatedTypes);
   EnumCaseDecl(string&& name, AssociatedTypeVec&& associatedTypes);

   typedef std::shared_ptr<EnumCaseDecl> SharedPtr;
   typedef std::unique_ptr<EnumCaseDecl> UniquePtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == EnumCaseDeclID;
   }

protected:
   string caseName;
   std::shared_ptr<Expression> rawVal = nullptr;

   long rawValue;
   bool has_raw_value;

   std::vector<pair<string, std::shared_ptr<TypeRef>>> associatedTypes;

public:
   const string &getCaseName() const
   {
      return caseName;
   }

   void setCaseName(const string &caseName)
   {
      EnumCaseDecl::caseName = caseName;
   }

   const std::shared_ptr<Expression> &getRawVal() const
   {
      return rawVal;
   }

   void setRawVal(const std::shared_ptr<Expression> &rawVal)
   {
      EnumCaseDecl::rawVal = rawVal;
   }

   long getRawValue() const
   {
      return rawValue;
   }

   void setRawValue(long rawValue)
   {
      EnumCaseDecl::rawValue = rawValue;
   }

   bool hasRawValue() const
   {
      return has_raw_value;
   }

   void hasRawValue(bool has_raw_value)
   {
      EnumCaseDecl::has_raw_value = has_raw_value;
   }

   const AssociatedTypeVec& getAssociatedTypes() const
   {
      return associatedTypes;
   }

   void setAssociatedTypes(AssociatedTypeVec &associatedTypes)
   {
      EnumCaseDecl::associatedTypes = associatedTypes;
   }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_ENUMCASEDECL_H
