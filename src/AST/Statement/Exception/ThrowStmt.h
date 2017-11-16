//
// Created by Jonas Zell on 05.10.17.
//

#ifndef CDOT_THROWSTMT_H
#define CDOT_THROWSTMT_H

#include "../Statement.h"

class ThrowStmt: public Statement {
public:
   ThrowStmt(std::shared_ptr<Expression>&& thrownVal);

   std::vector<AstNode::SharedPtr> get_children() override;

   typedef std::shared_ptr<ThrowStmt> SharedPtr;

   NodeType get_type() override {
      return NodeType::THROW_STMT;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   std::shared_ptr<Expression> thrownVal;
   BuiltinType *thrownType;

   cl::Method *descFn = nullptr;

public:
   const std::shared_ptr<Expression> &getThrownVal() const;

   void setThrownVal(const std::shared_ptr<Expression> &thrownVal);

   BuiltinType *getThrownType() const;

   void setThrownType(BuiltinType *thrownType);
};


#endif //CDOT_THROWSTMT_H
