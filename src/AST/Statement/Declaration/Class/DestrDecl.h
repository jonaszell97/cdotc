//
// Created by Jonas Zell on 02.09.17.
//

#ifndef CDOT_DESTRDECL_H
#define CDOT_DESTRDECL_H

#include "../CallableDecl.h"

class CompoundStmt;

namespace cdot {
   namespace cl {
      struct Method;
   }
}

class DestrDecl: public CallableDecl {
public:
   DestrDecl();

   typedef std::shared_ptr<DestrDecl> SharedPtr;
   typedef std::unique_ptr<DestrDecl> UniquePtr;

   NodeType get_type() override
   {
      return NodeType::DESTR_DECL;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   // codegen
   cdot::cl::Method* declaredMethod;
   cl::Record* record;

public:

   cl::Method *getDeclaredMethod() const
   {
      return declaredMethod;
   }

   void setDeclaredMethod(cl::Method *declaredMethod)
   {
      DestrDecl::declaredMethod = declaredMethod;
   }

   cl::Record *getRecord() const;
   void setRecord(cl::Record *record);
};


#endif //CDOT_DESTRDECL_H
