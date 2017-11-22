//
//#include "ConstExprPass.h"
//
//#include "../ASTIncludes.h"
//#include "Builtin.h"
//
//#include "../../../Variant/Variant.h"
//#include "../../../Variant/Type/PrimitiveType.h"
//#include "../../../Variant/Type/IntegerType.h"
//#include "../../../Variant/Type/FPType.h"
//
//#include "../SemanticAnalysis/Record/Class.h"
//#include "../../../Message/Diagnostics.h"
//
//
//using namespace cdot;
//using namespace cdot::diag;
//
//using std::stack;
//using std::unordered_map;
//using std::string;
//using std::set;
//
//size_t ConstExprPass::ConditionStack = 0;
//
//ConstExprPass::ConstExprPass()
//{
//
//}
//
//void ConstExprPass::run(std::vector<CompilationUnit> &CUs)
//{
//   for (const auto &CU : CUs) {
//      doInitialPass(CU.root->getStatements());
//   }
//
//   for (const auto &CU : CUs) {
//      visit(CU.root.get());
//   }
//}
//
//cdot::Variant ConstExprPass::pop(std::shared_ptr<AstNode> node)
//{
//   node->accept(this);
//   if (Results.empty()) {
//      return {};
//   }
//
//   auto res = std::move(Results.top());
//   Results.pop();
//
//   return res;
//}
//
//cdot::Variant ConstExprPass::pop(AstNode *node)
//{
//   node->accept(this);
//   if (Results.empty()) {
//      return {};
//   }
//
//   auto res = std::move(Results.top());
//   Results.pop();
//
//   return res;
//}
//
//void ConstExprPass::returnResult(cdot::Variant v)
//{
//   Results.push(std::move(v));
//}
//
//void ConstExprPass::doInitialPass(
//   const std::vector<std::shared_ptr<Statement>> &statements)
//{
//   for (const auto &stmt : statements) {
//      switch (stmt->get_type()) {
////         case NodeType::CLASS_DECL: {
////            auto clDec = std::static_pointer_cast<ClassDecl>(stmt);
////            for (const auto & field : clDec->getFields()) {
////               DeclareField(field.get());
////            }
////
////            doInitialPass(clDec->getInnerDeclarations());
////            break;
////         }
////         case NodeType::RECORD_TEMPLATE_DECL: {
////            auto recTempl = std::static_pointer_cast<RecordTemplateDecl>(stmt);
////            doInitialPass(recTempl->getInstantiations());
////
////            break;
////         }
////         case NodeType::NAMESPACE_DECL: {
////            auto nsDecl = std::static_pointer_cast<NamespaceDecl>(stmt);
////            doInitialPass(nsDecl->getContents()->getStatements());
////
////            break;
////         }
//         case NodeType::DECLARATION: {
//            auto decl = std::static_pointer_cast<DeclStmt>(stmt);
//            DeclareGlobalVar(decl.get());
//
//            break;
//         }
//         default:
//            break;
//      }
//   }
//}
//
//bool ConstExprPass::hasVariable(const string &varName)
//{
//   return Variables.find(varName) != Variables.end();
//}
//
//Variant& ConstExprPass::getVariable(const string &varName)
//{
//   return Variables[varName];
//}
//
//void ConstExprPass::setVariable(const string &varName, const Variant &val)
//{
//   if (Variables.find(varName) != Variables.end()) {
//      Variables[varName] = val;
//   }
//   else {
//      Variables.emplace(varName, val);
//   }
//}
//
//void ConstExprPass::visit(NamespaceDecl *node)
//{
//   node->getContents()->accept(this);
//}
//
//void ConstExprPass::visit(FunctionDecl *node)
//{
//   node->getBody()->accept(this);
//}
//
//void ConstExprPass::visit(CompoundStmt *node)
//{
//   for (const auto& stmt : node->get_children()) {
//      stmt->accept(this);
//   }
//}
//
//void ConstExprPass::visit(IdentifierRefExpr *node)
//{
//   if (node->getMemberExpr() != nullptr) {
//      return node->getMemberExpr()->accept(this);
//   }
//
//   if (hasVariable(node->getBinding())) {
//      auto var = getVariable(node->getBinding());
//      if (var.type != VariantType::STRING) {
//         node->setStaticVal(var);
//      }
//   }
//
//   returnResult(node->getStaticVal());
//}
//
//void ConstExprPass::DeclareGlobalVar(DeclStmt *node)
//{
//   if (node->value != nullptr && node->isConst() && node->isGlobal()) {
//      node->value->staticVal = pop(node->value);
//      setVariable(node->binding, node->value->staticVal);
//   }
//}
//
//void ConstExprPass::visit(DeclStmt *node)
//{
//   if (!node->isGlobal() && node->getValue()) {
//      node->value->staticVal = pop(node->value);
//      setVariable(node->binding, node->value->staticVal);
//   }
//}
//
//void ConstExprPass::visit(ForStmt *node)
//{
//   if (node->termination == nullptr) {
//      return;
//   }
//
//   auto cond = pop(node->termination);
//   if (cond.isVoid()) {
//      ++ConditionStack;
//      node->body->accept(this);
//      --ConditionStack;
//   }
//   else if (cond.intVal != 0) {
//      node->body->accept(this);
//   }
//}
//
//void ConstExprPass::visit(ForInStmt *node)
//{
//   ++ConditionStack;
//   node->body->accept(this);
//   --ConditionStack;
//}
//
//void ConstExprPass::visit(WhileStmt *node)
//{
//   auto cond = pop(node->condition);
//
//   if (cond.isVoid()) {
//      ++ConditionStack;
//      node->body->accept(this);
//      --ConditionStack;
//   }
//   else if (cond.intVal != 0) {
//      node->body->accept(this);
//   }
//}
//
//void ConstExprPass::visit(CollectionLiteral *node)
//{
//   for (const auto& stmt : node->get_children()) {
//      stmt->accept(this);
//   }
//}
//
//void ConstExprPass::visit(IntegerLiteral *node)
//{
//   node->setStaticVal(node->getValue());
//   node->getStaticVal().boxed = node->getType()->isObjectTy();
//
//   returnResult(node->getStaticVal());
//}
//
//void ConstExprPass::visit(FPLiteral *node)
//{
//   node->setStaticVal(node->getValue());
//   node->getStaticVal().boxed = node->getType()->isObjectTy();
//
//   returnResult(node->getStaticVal());
//}
//
//void ConstExprPass::visit(BoolLiteral *node)
//{
//   node->setStaticVal({node->getValue()});
//   node->getStaticVal().boxed = node->getType()->isObjectTy();
//
//   returnResult(node->getStaticVal());
//}
//
//void ConstExprPass::visit(CharLiteral *node)
//{
//   node->setStaticVal({node->getNarrow()});
//   node->getStaticVal().boxed = node->getType()->isObjectTy();
//
//   returnResult(node->getStaticVal());
//}
//
//void ConstExprPass::visit(NoneLiteral *node)
//{
//
//}
//
//void ConstExprPass::visit(StringLiteral *node)
//{
//   node->staticVal = Variant(string(node->value));
//   node->staticVal.boxed = node->isRaw();
//
//   returnResult(node->staticVal);
//}
//
//void ConstExprPass::visit(StringInterpolation *node)
//{
//   string str;
//   for (const auto& s : node->strings) {
//      Expression::SharedPtr next = s;
//      while (true) {
//         switch (next->get_type()) {
//            case NodeType::IMPLICIT_CAST_EXPR: {
//               auto asCast = std::static_pointer_cast<ImplicitCastExpr>(next);
//               if (asCast->to->isProtocol()) {
//                  next = asCast->target;
//               }
//
//               continue;
//            }
//            case NodeType::LVALUE_TO_RVALUE: {
//               auto asLtoR = std::static_pointer_cast<LvalueToRvalue>(next);
//               next = asLtoR->target;
//               continue;
//            }
//            default:
//               goto end;
//         }
//      }
//
//      end:
//      auto nextStr = pop(next);
//      if (nextStr.isVoid()) {
//         return;
//      }
//
//      return returnResult({std::move(nextStr.strVal)});
//   }
//}
//
//void ConstExprPass::visit(SubscriptExpr *node)
//{
//
//}
//
//void ConstExprPass::visit(CallExpr *node)
//{
//   size_t i = 0;
//   for (const auto& arg : node->args) {
//      auto argDefined = node->declaredArgTypes.size() > i;
//      if (arg.second->byval_pass
//          || (argDefined && node->declaredArgTypes.at(i).type.isLvalue())) {
//         continue;
//      }
//
//      arg.second->staticVal = pop(arg.second);
//
//
//      if (argDefined && !node->declaredArgTypes.at(i).type->isStruct()) {
//         arg.second->staticVal.boxed = false;
//      }
//
//      ++i;
//   }
//
//   // check if any values are used as in-out parameters,
//   // and mark them as non-static if they are
//   i = 0;
//   for (const auto& ty : node->resolvedArgs) {
//      auto& arg = node->args[i].second;
//      if (ty.type.isLvalue() && arg->get_type() == NodeType::IDENTIFIER_EXPR) {
//         auto ident = std::static_pointer_cast<IdentifierRefExpr>(arg);
//
//         Variant v;
//         setVariable(ident->binding, v);
//      }
//   }
//
//   if (node->memberExpr != nullptr) {
//      return node->memberExpr->accept(this);
//   }
//
//   if (node->isBuiltin()) {
//      switch (node->builtinFnKind) {
//         case BuiltinFn::SIZEOF: {
//            auto ty = std::static_pointer_cast<TypeRef>(
//               node->args.front().second)->getType();
//            node->staticVal = Variant((unsigned long long) ty->getSize());
//            break;
//         }
//         case BuiltinFn::ALIGNOF: {
//            auto ty = std::static_pointer_cast<TypeRef>(
//               node->args.front().second)->getType();
//            node->staticVal = Variant(ty->getAlignment());
//            break;
//         }
//         default:
//            break;
//      }
//   }
//
//   returnResult(node->staticVal);
//}
//
//void ConstExprPass::visit(MemberRefExpr *node)
//{
//   if (VisitedClasses.find(node->getClassName()) == VisitedClasses.end()) {
//      if (SymbolTable::hasRecord(node->getClassName())) {
//         SymbolTable::getRecord(node->getClassName())->getDecl()
//                                                     ->accept(this);
//      }
//   }
//
//   if (node->isNsMember()) {
//      if (hasVariable(node->getBinding())) {
//         node->setStaticVal(getVariable(node->getBinding()));
//      }
//   }
//
//   returnResult(node->getStaticVal());
//}
//
//void ConstExprPass::visit(BinaryOperator *node)
//{
//   if (node->op == "..") {
//      return;
//   }
//
//   auto lhs = pop(node->lhs);
//   auto rhs = pop(node->rhs);
//
//   string assignOp;
//   if (node->opType == BinaryOperatorType::ASSIGNMENT) {
//      assignOp = node->op;
//      node->op = node->op.substr(0, node->op.length() - 1);
//   }
//   else if (node->opType == BinaryOperatorType::CAST) {
//      auto ty = std::static_pointer_cast<TypeRef>(node->rhs)->getType();
//      if (ty->isNumeric()
//          || (ty->isObjectTy() && ty->getClassName() == "String")) {
//         node->staticVal = lhs.castTo(*ty);
//      }
//   }
//
//   node->staticVal = lhs.applyBinaryOp(rhs, node->op);
//
//   if (node->op == "**" && node->operandType->isIntegerTy()) {
//      node->staticVal = Variant((unsigned long long)node->staticVal.floatVal);
//   }
//
//   if (node->opType == BinaryOperatorType::ASSIGNMENT) {
//      node->op = assignOp;
//
//      if (node->lhs->get_type() == NodeType::IDENTIFIER_EXPR) {
//         auto ident = std::static_pointer_cast<IdentifierRefExpr>(node->lhs);
//
//         Variant newVal = ConditionStack > 0 || ident->memberExpr != nullptr
//                          ? Variant()
//                          : node->staticVal;
//         setVariable(ident->binding, newVal);
//      }
//
//      return;
//   }
//
//   node->staticVal.boxed = lhs.boxed || rhs.boxed;
//   returnResult(node->staticVal);
//}
//
//void ConstExprPass::visit(TertiaryOperator *node)
//{
//   auto cond = pop(node->condition);
//   if (cond.isVoid()) {
//      ++ConditionStack;
//      node->lhs->accept(this);
//      node->rhs->accept(this);
//      --ConditionStack;
//   }
//   else if (cond.intVal == 0) {
//      node->rhs->accept(this);
//   }
//   else {
//      node->lhs->accept(this);
//   }
//}
//
//#define UNARY_OPERATOR_INT(OP) \
//   if (node->op == #OP) {\
//      node->staticVal = Variant(OP target.intVal);\
//   }
//
//#define UNARY_OPERATOR_FLOAT(OP) \
//   if (node->op == #OP) {\
//      node->staticVal = Variant(OP target.floatVal);\
//   }
//
//void ConstExprPass::visit(UnaryOperator *node)
//{
//   auto target = pop(node->target);
//   if (target.isVoid()) {
//      return;
//   }
//
//   node->staticVal = target.applyUnaryOp(node->op);
//   node->staticVal.boxed = target.boxed;
//
//   returnResult(node->getStaticVal());
//}
//
//void ConstExprPass::visit(BreakStmt *node)
//{
//
//}
//
//void ConstExprPass::visit(ContinueStmt *node)
//{
//
//}
//
//void ConstExprPass::visit(IfStmt *node)
//{
//   auto cond = pop(node->condition);
//   if (cond.isVoid()) {
//      ++ConditionStack;
//
//      node->ifBranch->accept(this);
//      if (node->elseBranch) {
//         node->elseBranch->accept(this);
//      }
//
//      --ConditionStack;
//   }
//   else if (cond.intVal == 1) {
//      node->ifBranch->accept(this);
//   }
//   else if (cond.intVal == 0 && node->elseBranch != nullptr) {
//      node->elseBranch->accept(this);
//   }
//}
//
//void ConstExprPass::visit(MatchStmt *node)
//{
//   for (const auto& stmt : node->get_children()) {
//      stmt->accept(this);
//   }
//}
//
//void ConstExprPass::visit(CaseStmt *node)
//{
//
//}
//
//void ConstExprPass::visit(LabelStmt *node)
//{
//
//}
//
//void ConstExprPass::visit(GotoStmt *node)
//{
//
//}
//
//void ConstExprPass::visit(FuncArgDecl *node)
//{
//   if (node->defaultVal) {
//      node->defaultVal->accept(this);
//   }
//}
//
//void ConstExprPass::visit(ReturnStmt *node)
//{
//   if (node->returnValue != nullptr) {
//      node->returnValue->accept(this);
//   }
//}
//
//void ConstExprPass::visit(Expression *node)
//{
//   returnResult({});
//}
//
//void ConstExprPass::visit(ClassDecl *node)
//{
//   auto &name = node->getDeclaredClass()->getName();
//   if (VisitedClasses.find(name) != VisitedClasses.end()) {
//      return;
//   }
//
//   VisitedClasses.insert(name);
//   for (const auto& inner : node->getInnerDeclarations()) {
//      inner->accept(this);
//   }
//   for (const auto& field : node->getFields()) {
//      field->accept(this);
//   }
//   for (const auto& method : node->getMethods()) {
//      method->accept(this);
//   }
//   for (const auto& constr : node->constructors) {
//      constr->accept(this);
//   }
//   if (node->destructor) {
//      node->destructor->accept(this);
//   }
//}
//
//void ConstExprPass::visit(MethodDecl *node)
//{
//   if (node->getBody() != nullptr) {
//      node->getBody()->accept(this);
//   }
//}
//
//void ConstExprPass::DeclareField(FieldDecl *node)
//{
//
//}
//
//void ConstExprPass::visit(FieldDecl *node)
//{
//   auto &defaultVal = node->getDefaultVal();
//   if (defaultVal != nullptr && node->isStatic() && node->isConst()) {
//      defaultVal->setStaticVal(pop(defaultVal));
//
//      if (node->isStatic()) {
//         setVariable(node->getBinding(), defaultVal->getStaticVal());
//      }
//   }
//}
//
//void ConstExprPass::visit(ConstrDecl *node)
//{
//   if (node->getBody() != nullptr) {
//      node->getBody()->accept(this);
//   }
//}
//
//void ConstExprPass::visit(DestrDecl *node)
//{
//   if (node->getBody() != nullptr) {
//      node->getBody()->accept(this);
//   }
//}
//
//void ConstExprPass::visit(LambdaExpr *node)
//{
//   if (node->getBody() != nullptr) {
//      node->getBody()->accept(this);
//   }
//}
//
//void ConstExprPass::visit(ImplicitCastExpr *node)
//{
//   auto target = pop(node->target);
//   if (target.isVoid()) {
//      returnResult({});
//   }
//
//   auto& targetTy = node->to;
//   if (!targetTy->isNumeric()
//       && !(targetTy->isObjectTy() && targetTy->getClassName() == "String")) {
//      return returnResult({});
//   }
//
//   node->staticVal = target.castTo(*node->to);
//   node->staticVal.boxed = target.boxed;
//
//   returnResult(node->staticVal);
//}
//
//void ConstExprPass::visit(TypedefDecl *node)
//{
//
//}
//
//void ConstExprPass::visit(TypeRef *node)
//{
//   returnResult({});
//}
//
//void ConstExprPass::visit(DeclareStmt *node)
//{
//
//}
//
//void ConstExprPass::visit(LvalueToRvalue *node)
//{
//   returnResult(pop(node->target));
//}
//
//void ConstExprPass::visit(DebugStmt *node)
//{
//   if (!node->isUnreachable) {
//      int i = 3;
//   }
//}
//
//void ConstExprPass::visit(TupleLiteral *node)
//{
//   returnResult({});
//}
//
//void ConstExprPass::visit(EnumDecl *node)
//{
//
//}
//
//void ConstExprPass::visit(RecordTemplateDecl *node)
//{
//   for (const auto &inst : node->getInstantiations()) {
//      inst->accept(this);
//   }
//}
//
//void ConstExprPass::visit(Statement *node)
//{
//
//}
