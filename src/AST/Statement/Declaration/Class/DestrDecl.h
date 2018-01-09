//
// Created by Jonas Zell on 02.09.17.
//

#ifndef CDOT_DESTRDECL_H
#define CDOT_DESTRDECL_H

#include "../CallableDecl.h"

namespace cdot {

enum class AccessModifier : unsigned;

namespace cl {

struct Method;

}

namespace ast {

class CompoundStmt;

class DestrDecl: public CallableDecl {
public:
   DestrDecl()
      : CallableDecl(DestrDeclID, (AccessModifier)0, {}, {}, {},
                     {}, {}, {}, {})
   {

   }

   explicit DestrDecl(std::shared_ptr<CompoundStmt> &&body)
      : CallableDecl(DestrDeclID, (AccessModifier)0, {}, {}, {}, {}, {},
                     move(body), {})
   {

   }

   typedef std::shared_ptr<DestrDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == DestrDeclID;
   }

protected:
   // codegen
   cdot::cl::Record* record = nullptr;

public:

   cdot::cl::Method *getMethod() const;

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
