////
//// Created by Jonas Zell on 07.07.17.
////
//
//#ifndef CDOT_CONSTEXPRVISITOR_H
//#define CDOT_CONSTEXPRVISITOR_H
//
//#include <stack>
//#include <vector>
//#include <string>
//#include <set>
//#include <unordered_map>
//
//#include "../AbstractPass.h"
//#include "../../../Variant/Variant.h"
//
//namespace cdot {
//   struct Variant;
//}
//
//class ConstExprPass: public AbstractPass {
//public:
//   explicit ConstExprPass();
//   void run(std::vector<cdot::CompilationUnit> &CUs) override;
//
//   void visit(NamespaceDecl *node) override;
//   void visit(CompoundStmt *node) override;
//
//   void visit(DeclStmt *node) override;
//   void DeclareGlobalVar(DeclStmt *node);
//
//   void visit(FunctionDecl *node) override;
//   void visit(DeclareStmt *node) override;
//
//   void visit(ClassDecl *node) override;
//   void visit(MethodDecl *node) override;
//   void visit(FieldDecl *node) override;
//   void DeclareField(FieldDecl *node);
//
//   void visit(ConstrDecl *node) override;
//   void visit(DestrDecl *node) override;
//   void visit(EnumDecl *node) override;
//   void visit(RecordTemplateDecl *node) override;
//
//   void visit(IdentifierRefExpr *node) override;
//   void visit(SubscriptExpr *node) override;
//   void visit(CallExpr *node) override;
//   void visit(MemberRefExpr *node) override;
//
//   void visit(ForStmt *node) override;
//   void visit(ForInStmt *node) override;
//   void visit(WhileStmt *node) override;
//   void visit(IfStmt *node) override;
//   void visit(MatchStmt *node) override;
//   void visit(CaseStmt *node) override;
//   void visit(LabelStmt *node) override;
//   void visit(GotoStmt *node) override;
//
//   void visit(ReturnStmt *node) override;
//   void visit(BreakStmt *node) override;
//   void visit(ContinueStmt *node) override;
//
//   void visit(CollectionLiteral *node) override;
//   void visit(IntegerLiteral *node) override;
//   void visit(FPLiteral *node) override;
//   void visit(BoolLiteral *node) override;
//   void visit(CharLiteral *node) override;
//
//   void visit(NoneLiteral *node) override;
//   void visit(StringLiteral *node) override;
//   void visit(StringInterpolation *node) override;
//   void visit(TupleLiteral *node) override;
//
//   void visit(BinaryOperator *node) override;
//   void visit(TertiaryOperator *node) override;
//   void visit(UnaryOperator *node) override;
//
//
//   void visit(FuncArgDecl *node) override;
//   void visit(Expression *node) override;
//   void visit(LambdaExpr *node) override;
//   void visit(ImplicitCastExpr *node) override;
//   void visit(TypedefDecl *node) override;
//   void visit(TypeRef *node) override;
//   void visit(LvalueToRvalue *node) override;
//
//   void visit(DebugStmt *node) override;
//
//   void visit(Statement *node) override;
//
//   cdot::Variant pop(AstNode *node);
//   cdot::Variant pop(std::shared_ptr<AstNode> node);
//
//protected:
//   static size_t ConditionStack;
//
//   std::unordered_map<std::string, cdot::Variant> Variables;
//   std::set<std::string> VisitedClasses;
//   std::stack<cdot::Variant> Results;
//
//   void returnResult(cdot::Variant v);
//
//   void doInitialPass(
//      const std::vector<std::shared_ptr<Statement>> &root);
//
//   bool hasVariable(const std::string& varName);
//   cdot::Variant& getVariable(const std::string& varName);
//   void setVariable(
//      const std::string& varName, const cdot::Variant& val);
//};
//
//
//#endif //CDOT_CONSTEXPRVISITOR_H
