//
// Created by Jonas Zell on 01.09.17.
//

#include "ForInStmt.h"
#include "../Declaration/DeclStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

ForInStmt::ForInStmt(std::shared_ptr<DeclStmt>&& decl,
                     std::shared_ptr<Expression>&& range,
                     std::shared_ptr<Statement>&& body)
   : Statement(ForInStmtID), decl(move(decl)), rangeExpr(move(range)),
     body(move(body))
{

}

const std::shared_ptr<DeclStmt> &ForInStmt::getDecl() const
{
   return decl;
}

void ForInStmt::setDecl(const std::shared_ptr<DeclStmt> &decl)
{
   ForInStmt::decl = decl;
}

const std::shared_ptr<Expression> &ForInStmt::getRangeExpr() const
{
   return rangeExpr;
}

void ForInStmt::setRangeExpr(const std::shared_ptr<Expression> &rangeExpr)
{
   ForInStmt::rangeExpr = rangeExpr;
}

const std::shared_ptr<Statement> &ForInStmt::getBody() const
{
   return body;
}

void ForInStmt::setBody(const std::shared_ptr<Statement> &body)
{
   ForInStmt::body = body;
}

const string &ForInStmt::getIteratorGetter() const
{
   return iteratorGetter;
}

void ForInStmt::setIteratorGetter(const string &iteratorGetter)
{
   ForInStmt::iteratorGetter = iteratorGetter;
}

const string &ForInStmt::getIteratorClass() const
{
   return iteratorClass;
}

void ForInStmt::setIteratorClass(const string &iteratorClass)
{
   ForInStmt::iteratorClass = iteratorClass;
}

const string &ForInStmt::getNextFunc() const
{
   return nextFunc;
}

void ForInStmt::setNextFunc(const string &nextFunc)
{
   ForInStmt::nextFunc = nextFunc;
}

const QualType &ForInStmt::getIteratedType() const
{
   return iteratedType;
}

void ForInStmt::setIteratedType(const QualType &iteratedType)
{
   ForInStmt::iteratedType = iteratedType;
}

bool ForInStmt::isRangeIsRefcounted() const
{
   return rangeIsRefcounted;
}

void ForInStmt::setRangeIsRefcounted(bool rangeIsRefcounted)
{
   ForInStmt::rangeIsRefcounted = rangeIsRefcounted;
}

const string &ForInStmt::getRangeClassName() const
{
   return rangeClassName;
}

void ForInStmt::setRangeClassName(const string &rangeClassName)
{
   ForInStmt::rangeClassName = rangeClassName;
}

Type *ForInStmt::getProtocolTy() const
{
   return protocolTy;
}

void ForInStmt::setProtocolTy(Type *protocolTy)
{
   ForInStmt::protocolTy = protocolTy;
}

} // namespace ast
} // namespace cdot