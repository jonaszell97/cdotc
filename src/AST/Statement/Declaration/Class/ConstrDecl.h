//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CONSTRDECL_H
#define CDOT_CONSTRDECL_H

#include "../CallableDecl.h"

class FuncArgDecl;
class CompoundStmt;

namespace cdot {
   namespace cl {
      struct Method;
   }
}

class ConstrDecl : public CallableDecl {
public:
   ConstrDecl();
   ConstrDecl(
      std::vector<std::shared_ptr<FuncArgDecl>> &&args,
      AccessModifier am
   );

   typedef std::shared_ptr<ConstrDecl> SharedPtr;
   typedef std::unique_ptr<ConstrDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;

   NodeType get_type() override {
      return NodeType::CONSTR_DECL;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   bool memberwise = false;

   // codegen
   cdot::cl::Method* method;
   cl::Record *record;

public:
   bool isMemberwise() const
   {
      return memberwise;
   }

   void setMemberwise(bool memberwise)
   {
      ConstrDecl::memberwise = memberwise;
   }

   cl::Method *getMethod() const
   {
      return method;
   }

   void setMethod(cl::Method *method)
   {
      ConstrDecl::method = method;
   }

   cl::Record *getRecord() const;
   void setRecord(cl::Record *record);
};


#endif //CDOT_CONSTRDECL_H
