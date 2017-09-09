
#include "ConstExprPass.h"
#include "../Visitor.cpp"
#include "../../../Variant/Variant.h"
#include "../../Statement/Declaration/FuncArgDecl.h"

size_t ConstExprPass::latestScope = 0;

ConstExprPass::ConstExprPass() {

}

void ConstExprPass::pushScope() {
   Scopes.push_back(std::to_string(latestScope++));
}

void ConstExprPass::popScope() {
   Scopes.pop_back();
}

long ConstExprPass::hasVariable(string &varName) {
   long current = Scopes.size() - 1;
   while (current >= 0) {
      if (Variables.find(varName + Scopes[current]) != Variables.end()) {
         return current;
      }

      --current;
   }

   return -1;
}

Variant& ConstExprPass::getVariable(string &varName) {
   auto index = hasVariable(varName);
   assert(index != -1 && "Call hasVariable first!");

   return Variables[varName + Scopes[index]];
}

void ConstExprPass::setVariable(string &varName, Variant &val) {
   auto index = hasVariable(varName);
   if (index != -1) {
      Variables[varName + Scopes[index]] = val;
   }
   else {
      Variables.emplace(varName + Scopes.back(), val);
   }
}

Variant ConstExprPass::visit(NamespaceDecl *node)
{
   node->contents->accept(*this);
   return {};
}

Variant ConstExprPass::visit(FunctionDecl *node)
{
   for (const auto& stmt : node->get_children()) {
      stmt->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(CompoundStmt *node)
{
   pushScope();
   for (const auto& stmt : node->get_children()) {
      stmt->accept(*this);
   }
   popScope();

   return {};
}

Variant ConstExprPass::visit(IdentifierRefExpr *node)
{
   if (hasVariable(node->ident) != -1) {
      return getVariable(node->ident);
   }

   return {};
}

Variant ConstExprPass::visit(DeclStmt *node)
{
   if (node->value != nullptr) {
      auto val = node->value->accept(*this);
      if (!val.isVoid()) {
         setVariable(node->identifier, val);
      }
   }

   return {};
}

Variant ConstExprPass::visit(ForStmt *node)
{
   for (const auto& stmt : node->get_children()) {
      stmt->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(ForInStmt *node)
{
   for (const auto& stmt : node->get_children()) {
      stmt->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(WhileStmt *node)
{
   for (const auto& stmt : node->get_children()) {
      stmt->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(CollectionLiteral *node)
{
   for (const auto& stmt : node->get_children()) {
      stmt->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(NumericLiteral *node)
{
   return node->value;
}

Variant ConstExprPass::visit(NoneLiteral *node)
{
   return {};
}

Variant ConstExprPass::visit(StringLiteral *node)
{
   return Variant(node->value);
}

Variant ConstExprPass::visit(SubscriptExpr *node)
{
   return {};
}

Variant ConstExprPass::visit(CallExpr *node)
{
   return {};
}

Variant ConstExprPass::visit(MemberRefExpr *node)
{
   return {};
}

#define BINARY_OPERATOR_INT(OP) \
   if(#OP == node->op) {\
      node->staticVal = Variant(lhs.intVal OP rhs.intVal);\
   }

#define BINARY_OPERATOR_FLOAT(OP) \
   if(#OP == node->op) {\
      node->staticVal = Variant(lhs.floatVal OP rhs.floatVal);\
   }

Variant ConstExprPass::visit(BinaryOperator *node)
{
   auto lhs = node->lhs->accept(*this);
   auto rhs = node->rhs->accept(*this);
   if (lhs.isVoid() || rhs.isVoid()) {
      return {};
   }

   string assignOp;
   if (node->opType == BinaryOperatorType::ASSIGNMENT) {
      assignOp = node->op;
      node->op = node->op.substr(0, node->op.length() - 1);
   }

   if (lhs.type == VariantType::INT && rhs.type == VariantType::INT) {
      BINARY_OPERATOR_INT(+);
      BINARY_OPERATOR_INT(-);
      BINARY_OPERATOR_INT(*);
      BINARY_OPERATOR_INT(/);
      BINARY_OPERATOR_INT(&);
      BINARY_OPERATOR_INT(|);
      BINARY_OPERATOR_INT(&&);
      BINARY_OPERATOR_INT(||);
      BINARY_OPERATOR_INT(%);
      BINARY_OPERATOR_INT(<<);
      BINARY_OPERATOR_INT(>>);
   }
   else if (lhs.type == VariantType::STRING) {
      switch (rhs.type) {
         case VariantType::STRING:
            node->staticVal = Variant(lhs.strVal + rhs.strVal);
         case VariantType::INT:
            node->staticVal = Variant(lhs.strVal + std::to_string(rhs.intVal));
         case VariantType::FLOAT:
            node->staticVal = Variant(lhs.strVal + std::to_string(rhs.floatVal));
         default:
            llvm_unreachable("Should have been caught before!");
      }

      return node->staticVal;
   }
   else if (rhs.type == VariantType::STRING) {
      switch (lhs.type) {
         case VariantType::INT:
            node->staticVal = Variant(std::to_string(lhs.intVal) + rhs.strVal);
         case VariantType::FLOAT:
            node->staticVal = Variant(std::to_string(lhs.floatVal) + rhs.strVal);
         default:
            llvm_unreachable("Should have been caught before!");
      }

      return node->staticVal;
   }
   else if (lhs.type == VariantType::FLOAT || rhs.type == VariantType::FLOAT) {
      BINARY_OPERATOR_FLOAT(+);
      BINARY_OPERATOR_FLOAT(-);
      BINARY_OPERATOR_FLOAT(*);
      BINARY_OPERATOR_FLOAT(/);
   }

   if (node->opType == BinaryOperatorType::ASSIGNMENT) {
      node->op = assignOp;
      if (node->lhs->get_type() == NodeType::IDENTIFIER_EXPR) {
         auto ident = std::static_pointer_cast<IdentifierRefExpr>(node->lhs);
         if (hasVariable(ident->ident) != -1) {
            setVariable(ident->ident, node->staticVal);
         }
         else {
            Variant v;
            setVariable(ident->ident, v);
         }
      }

      return {};
   }

   return node->staticVal;
}

Variant ConstExprPass::visit(TertiaryOperator *node)
{
   return {};
}

#define UNARY_OPERATOR_INT(OP) \
   if (node->op == #OP) {\
      node->staticVal = Variant(OP target.intVal);\
      return node->staticVal;\
   }

#define UNARY_OPERATOR_FLOAT(OP) \
   if (node->op == #OP) {\
      node->staticVal = Variant(OP target.floatVal);\
      return node->staticVal;\
   }

Variant ConstExprPass::visit(UnaryOperator *node)
{
   auto target = node->target->accept(*this);
   if (target.isVoid()) {
      return {};
   }

   if (target.type == VariantType::INT) {
      UNARY_OPERATOR_INT(+);
      UNARY_OPERATOR_INT(-);
      UNARY_OPERATOR_INT(!);
      UNARY_OPERATOR_INT(~);
   }

   if (target.type == VariantType::FLOAT) {
      UNARY_OPERATOR_FLOAT(+);
      UNARY_OPERATOR_FLOAT(-);
   }

   return {};
}

Variant ConstExprPass::visit(BreakStmt *node)
{
   return {};
}

Variant ConstExprPass::visit(ContinueStmt *node)
{
   return {};
}

Variant ConstExprPass::visit(IfStmt *node)
{
   for (const auto& stmt : node->get_children()) {
      stmt->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(MatchStmt *node)
{
   for (const auto& stmt : node->get_children()) {
      stmt->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(CaseStmt *node)
{
   return {};
}

Variant ConstExprPass::visit(LabelStmt *node)
{
   return {};
}

Variant ConstExprPass::visit(GotoStmt *node)
{
   return {};
}

Variant ConstExprPass::visit(FuncArgDecl *node)
{
   if (node->defaultVal) {
      node->defaultVal->accept(*this);
   }
   return {};
}

Variant ConstExprPass::visit(ReturnStmt *node)
{
   if (node->returnValue != nullptr) {
      node->returnValue->accept(*this);
   }
   return {};
}

Variant ConstExprPass::visit(Expression *node)
{
   return {};
}

Variant ConstExprPass::visit(ClassDecl *node)
{
   return {};
}

Variant ConstExprPass::visit(MethodDecl *node)
{
   if (node->body != nullptr) {
      node->body->accept(*this);
   }
   return {};
}

Variant ConstExprPass::visit(FieldDecl *node)
{
   return {};
}

Variant ConstExprPass::visit(ConstrDecl *node)
{
   if (node->body != nullptr) {
      node->body->accept(*this);
   }
   return {};
}

Variant ConstExprPass::visit(DestrDecl *node)
{
   if (node->body != nullptr) {
      node->body->accept(*this);
   }
   return {};
}

Variant ConstExprPass::visit(LambdaExpr *node)
{
   if (node->body != nullptr) {
      node->body->accept(*this);
   }
   return {};
}

Variant ConstExprPass::visit(ImplicitCastExpr *node)
{
   return {};
}

Variant ConstExprPass::visit(ExtendStmt *node)
{
   return {};
}

Variant ConstExprPass::visit(TypedefDecl *node)
{
   return {};
}

Variant ConstExprPass::visit(TypeRef *node)
{
   return {};
}

Variant ConstExprPass::visit(DeclareStmt *node)
{
   return {};
}

Variant ConstExprPass::visit(LvalueToRvalue *node)
{
   return {};
}

Variant ConstExprPass::visit(DebugStmt *node)
{
   if (!node->isUnreachable) {
      int i = 3;
   }

   return {};
}

Variant ConstExprPass::visit(TupleLiteral *node)
{
   return {};
}

Variant ConstExprPass::visit(EnumDecl *node)
{
   return {};
}

Variant ConstExprPass::visit(Statement *node)
{
   return {};
}
