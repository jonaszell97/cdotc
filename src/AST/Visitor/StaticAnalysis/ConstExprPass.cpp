
#include "ConstExprPass.h"
#include "../Visitor.cpp"
#include "../../../Variant/Variant.h"
#include "../../Statement/Declaration/FuncArgDecl.h"
#include "../../../Variant/Type/PrimitiveType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FPType.h"

size_t ConstExprPass::ConditionStack = 0;

ConstExprPass::ConstExprPass() {

}

long ConstExprPass::hasVariable(string &varName) {
   return Variables.find(varName) != Variables.end();
}

Variant& ConstExprPass::getVariable(string &varName) {
   return Variables[varName];
}

void ConstExprPass::setVariable(string &varName, Variant &val) {
   if (Variables.find(varName) != Variables.end()) {
      Variables[varName] = val;
   }
   else {
      Variables.emplace(varName, val);
   }
}

Variant ConstExprPass::visit(NamespaceDecl *node)
{
   node->contents->accept(*this);
   return {};
}

Variant ConstExprPass::visit(FunctionDecl *node)
{
   node->body->accept(*this);
   return {};
}

Variant ConstExprPass::visit(CompoundStmt *node)
{
   for (const auto& stmt : node->get_children()) {
      stmt->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(IdentifierRefExpr *node)
{
   if (node->memberExpr != nullptr) {
      return node->memberExpr->accept(*this);
   }

   if (hasVariable(node->binding)) {
      node->staticVal = getVariable(node->binding);
   }

   return node->staticVal;
}

Variant ConstExprPass::visit(DeclStmt *node)
{
   if (node->value != nullptr) {
      node->value->staticVal = node->value->accept(*this);
      setVariable(node->binding, node->value->staticVal);
   }

   return {};
}

Variant ConstExprPass::visit(ForStmt *node)
{
   if (node->termination == nullptr) {
      return {};
   }

   auto cond = node->termination->accept(*this);
   if (cond.isVoid()) {
      ++ConditionStack;
      node->body->accept(*this);
      --ConditionStack;
   }
   else if (cond.intVal != 0) {
      node->body->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(ForInStmt *node)
{
   ++ConditionStack;
   node->body->accept(*this);
   --ConditionStack;

   return {};
}

Variant ConstExprPass::visit(WhileStmt *node)
{
   auto cond = node->condition->accept(*this);

   if (cond.isVoid()) {
      ++ConditionStack;
      node->body->accept(*this);
      --ConditionStack;
   }
   else if (cond.intVal != 0) {
      node->body->accept(*this);
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
   node->staticVal = node->value;
   if (!node->isPrimitive) {
      node->staticVal.isBoxed = true;
   }

   return node->staticVal;
}

Variant ConstExprPass::visit(NoneLiteral *node)
{
   return {};
}

Variant ConstExprPass::visit(StringLiteral *node)
{
   node->staticVal = Variant(node->value);
   node->staticVal.rawStr = node->raw;

   return node->staticVal;
}

Variant ConstExprPass::visit(StringInterpolation *node)
{
   return {};
   string str;
   for (const auto& s : node->strings) {
      Expression::SharedPtr next = s;
      while (true) {
         switch (next->get_type()) {
            case NodeType::IMPLICIT_CAST_EXPR: {
               auto asCast = std::static_pointer_cast<ImplicitCastExpr>(next);
               if (asCast->to->isProtocol()) {
                  next = asCast->target;
               }

               continue;
            }
            case NodeType::LVALUE_TO_RVALUE: {
               auto asLtoR = std::static_pointer_cast<LvalueToRvalue>(next);
               next = asLtoR->target;
               continue;
            }
            default:
               goto end;
         }
      }

      end:
      auto nextStr = next->accept(*this);
      if (nextStr.isVoid()) {
         return {};
      }

      return nextStr.strVal;
   }

   return {};
}

Variant ConstExprPass::visit(SubscriptExpr *node)
{
   return {};
}

Variant ConstExprPass::visit(CallExpr *node)
{
   size_t i = 0;
   for (const auto& arg : node->args) {
      auto argDefined = node->declaredArgTypes != nullptr && node->declaredArgTypes->size() > i;
      if (arg.second->needsByValPass || (argDefined && node->declaredArgTypes->at(i).type->isLvalue())) {
         continue;
      }

      arg.second->staticVal = arg.second->accept(*this);
      if (argDefined && !node->declaredArgTypes->at(i).type->isStruct()) {
         arg.second->staticVal.isBoxed = false;
      }

      ++i;
   }

   // check if any values are used as in-out parameters,
   // and mark them as non-static if they are
   i = 0;
   for (const auto& ty : node->resolvedArgs) {
      auto& arg = node->args[i].second;
      if (ty.type->isLvalue() && arg->get_type() == NodeType::IDENTIFIER_EXPR) {
         auto ident = std::static_pointer_cast<IdentifierRefExpr>(arg);

         Variant v;
         setVariable(ident->binding, v);
      }
   }

   if (node->memberExpr != nullptr) {
      return node->memberExpr->accept(*this);
   }

   return {};
}

Variant ConstExprPass::visit(MemberRefExpr *node)
{
   return {};
}


namespace {
   Variant handleCast(Variant& target, Type*& to) {
      switch (target.type) {
         case VariantType::INT:
            switch (to->getTypeID()) {
               case TypeID::IntegerTypeID: {
                  auto asInt = cast<IntegerType>(to);
                  target.bitwidth = asInt->getBitwidth();
                  target.isUnsigned = asInt->isUnsigned();

                  return target;
               }
               case TypeID::FPTypeID: return Variant((double)target.intVal);
               case TypeID::ObjectTypeID: return Variant(std::to_string(target.intVal));
               default:
                  llvm_unreachable("Should have returned before");
            }
         case VariantType::FLOAT:
            switch (to->getTypeID()) {
               case TypeID::IntegerTypeID: return Variant((long)target.floatVal);
               case TypeID::FPTypeID: {
                  auto asFloat = cast<FPType>(to);
                  target.bitwidth = asFloat->getPrecision();

                  return target;
               }
               case TypeID::ObjectTypeID: return Variant(std::to_string(target.floatVal));
               default:
                  llvm_unreachable("Should have returned before");
            }
//         case VariantType::STRING:
//            switch (to->getTypeID()) {
//               case TypeID::IntegerTypeID: return Variant(std::stol(target.strVal));
//               case TypeID::FPTypeID: return Variant(std::stod(target.strVal));
//               case TypeID::ObjectTypeID: return target;
//               default:
//                  llvm_unreachable("Should have returned before");
//            }
         default:
            return {};
      }
   }
}

Variant ConstExprPass::visit(BinaryOperator *node)
{
   auto lhs = node->lhs->accept(*this);
   auto rhs = node->rhs->accept(*this);

   string assignOp;
   if (node->opType == BinaryOperatorType::ASSIGNMENT) {
      assignOp = node->op;
      node->op = node->op.substr(0, node->op.length() - 1);
   }
   else if (node->opType == BinaryOperatorType::CAST) {
      auto ty = std::static_pointer_cast<TypeRef>(node->rhs)->getType();
      if (isa<PrimitiveType>(ty) || (ty->isObject() && ty->getClassName() == "String")) {
         node->staticVal = handleCast(lhs, ty);
      }
   }

   node->staticVal = lhs.applyBinaryOp(rhs, node->op);

   if (node->opType == BinaryOperatorType::ASSIGNMENT) {
      node->op = assignOp;

      if (node->lhs->get_type() == NodeType::IDENTIFIER_EXPR) {
         auto ident = std::static_pointer_cast<IdentifierRefExpr>(node->lhs);

         Variant newVal = ConditionStack > 0 || ident->memberExpr != nullptr ? Variant()
                                                                             : rhs;
         setVariable(ident->binding, newVal);
      }

      return {};
   }

   node->staticVal.isBoxed = lhs.isBoxed || rhs.isBoxed;
   return node->staticVal;
}

Variant ConstExprPass::visit(TertiaryOperator *node)
{
   auto cond = node->condition->accept(*this);
   if (cond.isVoid()) {
      ++ConditionStack;
      node->lhs->accept(*this);
      node->rhs->accept(*this);
      --ConditionStack;
   }
   else if (cond.intVal == 0) {
      node->rhs->accept(*this);
   }
   else {
      node->lhs->accept(*this);
   }

   return {};
}

#define UNARY_OPERATOR_INT(OP) \
   if (node->op == #OP) {\
      node->staticVal = Variant(OP target.intVal);\
   }

#define UNARY_OPERATOR_FLOAT(OP) \
   if (node->op == #OP) {\
      node->staticVal = Variant(OP target.floatVal);\
   }

Variant ConstExprPass::visit(UnaryOperator *node)
{
   auto target = node->target->accept(*this);
   if (target.isVoid()) {
      return {};
   }

   node->staticVal = target.applyUnaryOp(node->op);
   node->staticVal.isBoxed = target.isBoxed;

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
   auto cond = node->condition->accept(*this);

   if (cond.isVoid()) {
      ++ConditionStack;

      node->ifBranch->accept(*this);
      if (node->elseBranch) {
         node->elseBranch->accept(*this);
      }

      --ConditionStack;
   }
   else if (cond.intVal == 1) {
      node->ifBranch->accept(*this);
   }
   else if (cond.intVal == 0 && node->elseBranch != nullptr) {
      node->elseBranch->accept(*this);
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
   for (const auto& inner : node->innerDeclarations) {
      inner->accept(*this);
   }
   for (const auto& field : node->fields) {
      field->accept(*this);
   }

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
   if (node->defaultVal != nullptr) {
      node->defaultVal->staticVal = node->defaultVal->accept(*this);

      if (node->isStatic) {
         setVariable(node->binding, node->defaultVal->staticVal);
      }
   }

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
   auto target = node->target->accept(*this);
   if (target.isVoid()) {
      return {};
   }

   auto& targetTy = node->to;
   if (!isa<PrimitiveType>(targetTy) && !(targetTy->isObject() && targetTy->getClassName() == "String")) {
      return {};
   }

   node->staticVal = handleCast(target, node->to);
   node->staticVal.isBoxed = target.isBoxed;

   return node->staticVal;
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
   return node->target->accept(*this);
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
