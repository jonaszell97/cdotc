//
// Created by Jonas Zell on 29.08.17.
//

#ifndef CDOT_ENUMCASEDECL_H
#define CDOT_ENUMCASEDECL_H


#include "../../Statement.h"
#include "../../../../Variant/Variant.h"

class EnumCaseDecl: public Statement {
public:
   EnumCaseDecl(string&& name, long rawValue, std::vector<pair<string, std::shared_ptr<TypeRef>>>&& associatedTypes);
   EnumCaseDecl(string&& name, std::vector<pair<string, std::shared_ptr<TypeRef>>>&& associatedTypes);

   typedef std::shared_ptr<EnumCaseDecl> SharedPtr;
   typedef std::unique_ptr<EnumCaseDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::ENUM_CASE_DECL;
   }

   friend class ConstExprVisitor;
   friend class CodeGenVisitor;
   friend class TypeCheckVisitor;
   friend class cdot::cl::Class;

protected:
   string caseName;

   long rawValue;
   bool hasRawValue;

   std::vector<pair<string, std::shared_ptr<TypeRef>>> associatedTypes;
};


#endif //CDOT_ENUMCASEDECL_H
