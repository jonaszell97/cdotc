
#include "AbstractPass.h"

void AbstractPass::visit(NamespaceDecl *node) {}
void AbstractPass::visit(UsingStmt *node) {}
void AbstractPass::visit(CompoundStmt *node) {}

void AbstractPass::visit(DeclStmt *node) {}
void AbstractPass::visit(FunctionDecl *node) {}
void AbstractPass::visit(DeclareStmt *node) {}

void AbstractPass::visit(ClassDecl *node) {}
void AbstractPass::visit(MethodDecl *node) {}
void AbstractPass::visit(FieldDecl *node) {}
void AbstractPass::visit(ConstrDecl *node) {}
void AbstractPass::visit(DestrDecl *node) {}
void AbstractPass::visit(EnumDecl *node) {}
void AbstractPass::visit(EnumCaseDecl *node) {}

void AbstractPass::visit(IdentifierRefExpr *node) {}
void AbstractPass::visit(SubscriptExpr *node) {}
void AbstractPass::visit(CallExpr *node) {}
void AbstractPass::visit(MemberRefExpr *node) {}

void AbstractPass::visit(ForStmt *node) {}
void AbstractPass::visit(ForInStmt *node) {}
void AbstractPass::visit(WhileStmt *node) {}
void AbstractPass::visit(IfStmt *node) {}
void AbstractPass::visit(MatchStmt *node) {}
void AbstractPass::visit(CaseStmt *node) {}
void AbstractPass::visit(LabelStmt *node) {}
void AbstractPass::visit(GotoStmt *node) {}

void AbstractPass::visit(ReturnStmt *node) {}
void AbstractPass::visit(BreakStmt *node) {}
void AbstractPass::visit(ContinueStmt *node) {}

void AbstractPass::visit(CollectionLiteral *node) {}
void AbstractPass::visit(NumericLiteral *node) {}
void AbstractPass::visit(NoneLiteral *node) {}
void AbstractPass::visit(StringLiteral *node) {}
void AbstractPass::visit(StringInterpolation *node) {}
void AbstractPass::visit(TupleLiteral *node) {}

void AbstractPass::visit(UnaryOperator *node) {}
void AbstractPass::visit(BinaryOperator *node) {}
void AbstractPass::visit(TertiaryOperator *node) {}

void AbstractPass::visit(FuncArgDecl *node) {}
void AbstractPass::visit(Expression *node) {}
void AbstractPass::visit(LambdaExpr *node) {}
void AbstractPass::visit(ImplicitCastExpr *node) {}
void AbstractPass::visit(TypedefDecl *node) {}
void AbstractPass::visit(TypeRef *node) {}
void AbstractPass::visit(LvalueToRvalue *node) {}

void AbstractPass::visit(EndOfFileStmt *node) {}
void AbstractPass::visit(DebugStmt *node) {}

void AbstractPass::visit(Statement *node) {}