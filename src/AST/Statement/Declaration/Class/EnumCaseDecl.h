//
// Created by Jonas Zell on 29.08.17.
//

#ifndef CDOT_ENUMCASEDECL_H
#define CDOT_ENUMCASEDECL_H


#include "../../Statement.h"
#include "../../../../Variant/Variant.h"

class EnumCaseDecl: public Statement {
public:
   EnumCaseDecl(string&& name, std::shared_ptr<Expression>&& rawValue,
      std::vector<pair<string, std::shared_ptr<TypeRef>>>&& associatedTypes);
   EnumCaseDecl(string&& name, std::vector<pair<string,
      std::shared_ptr<TypeRef>>>&& associatedTypes);

   typedef std::shared_ptr<EnumCaseDecl> SharedPtr;
   typedef std::unique_ptr<EnumCaseDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;

   NodeType get_type() override {
      return NodeType::ENUM_CASE_DECL;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

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

   const std::vector<pair<string, std::shared_ptr<TypeRef>>> &getAssociatedTypes() const
   {
      return associatedTypes;
   }

   void setAssociatedTypes(const std::vector<pair<string, std::shared_ptr<TypeRef>>> &associatedTypes)
   {
      EnumCaseDecl::associatedTypes = associatedTypes;
   }
};


#endif //CDOT_ENUMCASEDECL_H
