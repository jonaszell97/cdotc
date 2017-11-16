//
// Created by Jonas Zell on 14.11.17.
//

#ifndef CDOT_PROPDECL_H
#define CDOT_PROPDECL_H

#include "RecordSubDecl.h"

namespace cdot {
namespace cl {

class Property;

}
}

class PropDecl: public RecordSubDecl {
public:
   PropDecl(
      string &&propName,
      std::shared_ptr<TypeRef> &&type,
      AccessModifier access,
      bool isStatic,
      bool hasDefinition,
      std::shared_ptr<CompoundStmt> &&getter,
      std::shared_ptr<CompoundStmt> &&setter,
      string &&newValName
   );

   std::vector<std::shared_ptr<AstNode>> get_children() override;

   NodeType get_type() override
   {
      return NodeType::PROP_DECL;
   }

   ASTNODE_ACCEPT_PASSES

   typedef std::shared_ptr<PropDecl> SharedPtr;

protected:
   std::shared_ptr<TypeRef> type;
   bool has_definition;

   std::shared_ptr<CompoundStmt> getterBody = nullptr;
   std::shared_ptr<CompoundStmt> setterBody = nullptr;

   string newValName;

   cl::Property *prop = nullptr;

public:
   const std::shared_ptr<TypeRef> &getType() const;
   void setType(const std::shared_ptr<TypeRef> &type);

   bool hasGetter() const;
   bool hasSetter() const;

   bool hasDefinition() const;
   void hasDefinition(bool has_definition);

   const std::shared_ptr<CompoundStmt> &getGetterBody() const;
   void setGetterBody(const std::shared_ptr<CompoundStmt> &getterBody);

   const std::shared_ptr<CompoundStmt> &getSetterBody() const;
   void setSetterBody(const std::shared_ptr<CompoundStmt> &setterBody);

   string &getNewValName();
   void setNewValName(const string &newValName);

   cl::Property *getProp() const;
   void setProp(cl::Property *prop);
};


#endif //CDOT_PROPDECL_H
