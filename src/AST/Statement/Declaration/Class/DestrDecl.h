//
// Created by Jonas Zell on 02.09.17.
//

#ifndef CDOT_DESTRDECL_H
#define CDOT_DESTRDECL_H

#include "../CallableDecl.h"

namespace cdot {
namespace cl {

struct Method;

}

namespace ast {

class CompoundStmt;

class DestrDecl: public CallableDecl {
public:
   DestrDecl() : CallableDecl(DestrDeclID, AccessModifier::PUBLIC, {}, {}, {})
   {

   }

   typedef std::shared_ptr<DestrDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == DestrDeclID;
   }

protected:
   // codegen
   cdot::cl::Method* declaredMethod = nullptr;
   cdot::cl::Record* record = nullptr;

public:

   cdot::cl::Method *getDeclaredMethod() const
   {
      return declaredMethod;
   }

   void setDeclaredMethod(cdot::cl::Method *declaredMethod)
   {
      DestrDecl::declaredMethod = declaredMethod;
   }

   cdot::cl::Record* getRecord() const
   {
      return record;
   }

   void setRecord(cdot::cl::Record* record)
   {
      this->record = record;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_DESTRDECL_H
