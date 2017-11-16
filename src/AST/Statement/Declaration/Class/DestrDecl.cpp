//
// Created by Jonas Zell on 02.09.17.
//

#include "DestrDecl.h"
#include "../../Block/CompoundStmt.h"
#include "../../../../Util.h"

DestrDecl::DestrDecl()
   : CallableDecl(AccessModifier::PUBLIC, {}, {}, {})
{

}

cl::Record *DestrDecl::getRecord() const
{
   return record;
}

void DestrDecl::setRecord(cl::Record *record)
{
   DestrDecl::record = record;
}
