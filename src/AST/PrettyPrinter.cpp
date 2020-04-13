#include "PrettyPrinter.h"

#include "AST/ASTVisitor.h"
#include "AST/Decl.h"
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "IL/Constants.h"
#include "Support/Format.h"
#include "Support/WriterBase.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

class PrettyPrinterImpl: public WriterBase<PrettyPrinterImpl, 3>,
                         public ASTVisitor<PrettyPrinterImpl> {
public:
   using Options = PrettyPrinter::Options;

   PrettyPrinterImpl(llvm::raw_ostream &out, Options opts)
      : WriterBase(out), opts(opts)
   {
      (void)this->opts;
   }

   void visitExpr(Expression* expr)
   {
      ASTVisitor::visit(expr);
   }

   void visitStmt(Statement* stmt)
   {
      ASTVisitor::visit(stmt);
   }

   void visit(Expression* E)
   {
      ASTVisitor::visit(E);
   }

   void visit(Statement *S)
   {
      ASTVisitor::visit(S);
   }

   void visitDecl(Decl* D)
   {
      llvm_unreachable("not yet");
   }

   void visit(SourceType Ty) {}

#  define CDOT_STMT(Name)                                \
   void visit##Name(Name* stmt);

#  define CDOT_EXPR(Name)                                \
   void visit##Name(Name* expr);

#  include "AST/AstNode.def"

#  define CDOT_DECL(Name)                                \
   void visit##Name(Name *decl);

#  include "AST/Decl.def"

   void visitRecordDecl(RecordDecl* stmt);

private:
   Options opts;

   void visitVarDecl(VarDecl* node);

   void printAccessModifier(AccessSpecifier access)
   {
      switch (access) {
      case AccessSpecifier::Default:
         break;
      case AccessSpecifier::Public:
         out << "public ";
         break;
      case AccessSpecifier::Private:
         out << "private ";
         break;
      case AccessSpecifier::Protected:
         out << "protected ";
         break;
      case AccessSpecifier::FilePrivate:
         out << "fileprivate ";
         break;
      case AccessSpecifier::Internal:
         out << "internal ";
         break;
      }
   }

   void WriteStringExprPair(std::pair<string, Expression* >
                           & pair) {
      if (!pair.first.empty())
         out << pair.first << ": ";

      visitExpr(pair.second);
   }
};

void PrettyPrinterImpl::visitCompoundStmt(CompoundStmt* stmt)
{
   out << "{\n";
   {
      TabGuard guard(this);
      for (auto &node : stmt->getStatements()) {
         ApplyTab();

         visitStmt(node);
         out << "\n";
      }
   }
   out << "}";
}

void PrettyPrinterImpl::visitSourceFileDecl(cdot::ast::SourceFileDecl *decl)
{

}

void PrettyPrinterImpl::visitAttributedStmt(cdot::ast::AttributedStmt *stmt)
{

}

void PrettyPrinterImpl::visitAttributedExpr(cdot::ast::AttributedExpr *E)
{

}

void PrettyPrinterImpl::visitTemplateParamDecl(TemplateParamDecl *stmt)
{
   out << stmt->getName();
}

void PrettyPrinterImpl::visitBreakStmt(BreakStmt* stmt)
{
   out << "break";
}

void PrettyPrinterImpl::visitContinueStmt(ContinueStmt* stmt)
{
   out << "continue";
}

void PrettyPrinterImpl::visitForStmt(ForStmt* stmt)
{
   out << "for ";

   visitStmt(stmt->getInitialization());
   out << ";";

   visitStmt(stmt->getTermination());
   out << ";";

   visitStmt(stmt->getIncrement());
   visitStmt(stmt->getBody());
}

void PrettyPrinterImpl::visitForInStmt(ForInStmt* stmt)
{
   out << "for ";
   visitDecl(stmt->getDecl());

   out << " in ";
   visitExpr(stmt->getRangeExpr());

   visitStmt(stmt->getBody());
}

void PrettyPrinterImpl::visitIfStmt(IfStmt* stmt)
{
   out << "if ";

   visitStmt(stmt->getIfBranch());

   if (auto Else = stmt->getElseBranch()) {
      out << " else ";
      visitStmt(Else);
   }
}

void PrettyPrinterImpl::visitWhileStmt(WhileStmt* stmt)
{
   out << "while ";
   visitStmt(stmt->getBody());
}

void PrettyPrinterImpl::visitMatchStmt(MatchStmt* stmt)
{
   out << "match ";
   visitExpr(stmt->getSwitchValue());

   out << "{\n";

   {
      TabGuard guard(this);
      for (auto &C : stmt->getCases()) {
         ApplyTab();
         visitCaseStmt(C);
         out << "\n";
      }
   }

   out << "}";
}

void PrettyPrinterImpl::visitCaseStmt(CaseStmt* stmt)
{
   if (stmt->isDefault()) {
      out << "default:";
   }
   else {
      out << "case ";
      visitExpr(stmt->getPattern());
      out << ":";
   }

   out << "\n";
   visitStmt(stmt->getBody());
}

void PrettyPrinterImpl::visitVarDecl(VarDecl* stmt)
{
   out << (stmt->isConst() ? "let " : "var ");
   out << stmt->getName();

   out << ": ";
   visit(stmt->getType().getTypeExpr());

   if (auto V = stmt->getValue()) {
      out << " = ";
      visitExpr(V);
   }
}

void PrettyPrinterImpl::visitLocalVarDecl(LocalVarDecl* stmt)
{
   visitVarDecl(stmt);
}

void PrettyPrinterImpl::visitGlobalVarDecl(GlobalVarDecl* stmt)
{
   printAccessModifier(stmt->getAccess());
   visitVarDecl(stmt);
}

void PrettyPrinterImpl::visitDestructuringDecl(DestructuringDecl *decl)
{

}

void PrettyPrinterImpl::visitAliasDecl(AliasDecl* stmt)
{
   out << "alias ";
   WriteList(stmt->getTemplateParams(),
             &PrettyPrinterImpl::visitTemplateParamDecl, "[", ", ", "]", true);

   out << " = ";
   visitExpr(stmt->getAliasExpr());
}

void PrettyPrinterImpl::visitRecordDecl(RecordDecl* stmt)
{

}

void PrettyPrinterImpl::visitClassDecl(ClassDecl* stmt)
{

}

void PrettyPrinterImpl::visitStructDecl(StructDecl* stmt)
{

}

void PrettyPrinterImpl::visitEnumDecl(EnumDecl* stmt)
{

}

void PrettyPrinterImpl::visitEnumCaseDecl(EnumCaseDecl* stmt)
{

}

void PrettyPrinterImpl::visitUnionDecl(UnionDecl* stmt)
{

}

void PrettyPrinterImpl::visitProtocolDecl(ProtocolDecl* stmt)
{

}

void PrettyPrinterImpl::visitMethodDecl(MethodDecl* stmt)
{

}

void PrettyPrinterImpl::visitFieldDecl(FieldDecl* stmt)
{

}

void PrettyPrinterImpl::visitPropDecl(PropDecl* stmt)
{

}

void PrettyPrinterImpl::visitSubscriptDecl(SubscriptDecl *decl)
{

}

void PrettyPrinterImpl::visitInitDecl(InitDecl* stmt)
{

}

void PrettyPrinterImpl::visitDeinitDecl(DeinitDecl* stmt)
{

}

void PrettyPrinterImpl::visitAssociatedTypeDecl(AssociatedTypeDecl* stmt)
{

}

void PrettyPrinterImpl::visitTypedefDecl(TypedefDecl* stmt)
{

}

void PrettyPrinterImpl::visitExtensionDecl(ExtensionDecl* stmt)
{

}

void PrettyPrinterImpl::visitFunctionDecl(FunctionDecl* stmt)
{

}

void PrettyPrinterImpl::visitFuncArgDecl(FuncArgDecl* stmt)
{

}

void PrettyPrinterImpl::visitNamespaceDecl(NamespaceDecl* stmt)
{

}

void PrettyPrinterImpl::visitUnittestDecl(UnittestDecl *decl)
{

}

void PrettyPrinterImpl::visitUsingDecl(UsingDecl* stmt)
{

}

void PrettyPrinterImpl::visitModuleDecl(ModuleDecl *stmt)
{

}

void PrettyPrinterImpl::visitImportDecl(ImportDecl *stmt)
{

}

void PrettyPrinterImpl::visitDebugStmt(DebugStmt* stmt)
{

}

void PrettyPrinterImpl::visitDebugDecl(DebugDecl *decl)
{

}

void PrettyPrinterImpl::visitDoStmt(DoStmt* stmt)
{
   out << "do ";
   visitStmt(stmt->getBody());
}

void PrettyPrinterImpl::visitTryExpr(TryExpr *expr)
{
   out << "try ";
   visitExpr(expr->getExpr());
}

void PrettyPrinterImpl::visitAwaitExpr(AwaitExpr *expr)
{
   out << "await ";
   visitExpr(expr->getExpr());
}

void PrettyPrinterImpl::visitThrowStmt(ThrowStmt* stmt)
{
   out << "throw";
   if (auto val = stmt->getThrownVal()) {
      out << " ";
      visitExpr(val);
   }
}

void PrettyPrinterImpl::visitReturnStmt(ReturnStmt* stmt)
{
   out << "return";
   if (auto val = stmt->getReturnValue()) {
      out << " ";
      visitExpr(val);
   }
}

void PrettyPrinterImpl::visitDiscardAssignStmt(DiscardAssignStmt *stmt)
{
   out << "_ = ";
   visitExpr(stmt->getRHS());
}

void PrettyPrinterImpl::visitNullStmt(NullStmt* stmt)
{
   out << ";";
}

void PrettyPrinterImpl::visitStaticIfStmt(StaticIfStmt* stmt)
{

}

void PrettyPrinterImpl::visitStaticForStmt(StaticForStmt* stmt)
{

}

void PrettyPrinterImpl::visitStaticAssertDecl(StaticAssertDecl* stmt)
{

}

void PrettyPrinterImpl::visitStaticPrintDecl(StaticPrintDecl* stmt)
{

}

void PrettyPrinterImpl::visitFunctionTypeExpr(FunctionTypeExpr *expr)
{
   out << "(";

   size_t i = 0;
   for (auto &Ty : expr->getArgTypes()) {
      if (i++ != 0) out << ", ";
      visit(Ty);
   }

   out << ") -> ";
   visit(expr->getReturnType());
}

void PrettyPrinterImpl::visitTupleTypeExpr(TupleTypeExpr *expr)
{
   out << "(";

   size_t i = 0;
   for (auto &Ty : expr->getContainedTypes()) {
      if (i++ != 0) out << ", ";
      visit(Ty);
   }

   out << ")";
}

void PrettyPrinterImpl::visitArrayTypeExpr(ArrayTypeExpr *expr)
{
   out << "[";
   visit(expr->getElementTy());
//   out << "; " << expr->getSizeExpr()->getEvaluatedExpr().getAPSInt() << "]";
}

void PrettyPrinterImpl::visitDeclTypeExpr(DeclTypeExpr *expr)
{
   out << "decltype(";
   visitExpr(expr->getTyExpr());
   out << ")";
}

void PrettyPrinterImpl::visitReferenceTypeExpr(ReferenceTypeExpr *expr)
{
   out << "ref ";
   visit(expr->getSubType());
}

void PrettyPrinterImpl::visitPointerTypeExpr(PointerTypeExpr *expr)
{
   visit(expr->getSubType());
   out << "*";
}

void PrettyPrinterImpl::visitOptionTypeExpr(OptionTypeExpr *expr)
{
   visit(expr->getSubType());
   out << "?";
}

void PrettyPrinterImpl::visitExistentialTypeExpr(ExistentialTypeExpr *expr)
{
   unsigned i = 0;
   for (auto &E : expr->getExistentials()) {
      if (i++ != 0) out << " & ";
      visit(E);
   }
}

void PrettyPrinterImpl::visitParenExpr(ast::ParenExpr *expr)
{
   out << "(";
   visitExpr(expr->getParenthesizedExpr());
   out << ")";
}

void PrettyPrinterImpl::visitIntegerLiteral(IntegerLiteral* expr)
{
   out << expr->getValue().toString(10, !expr->getType()->isUnsigned());
}

void PrettyPrinterImpl::visitFPLiteral(FPLiteral* expr)
{
   llvm::SmallString<128> str;
   expr->getValue().toString(str);

   out << str;
}

void PrettyPrinterImpl::visitBoolLiteral(BoolLiteral* expr)
{
   out << (expr->getValue() ? "true" : "false");
}

void PrettyPrinterImpl::visitCharLiteral(CharLiteral* expr)
{
   auto c = support::unescape_char(expr->getNarrow());
   out << "'";

   if (c != expr->getNarrow())
      out << "\\";

   out << c << "'";
}

void PrettyPrinterImpl::visitNoneLiteral(NoneLiteral* expr)
{
   out << "none";
}

void PrettyPrinterImpl::visitStringLiteral(StringLiteral* expr)
{
   out << '"' << expr->getValue() << '"';
}

void PrettyPrinterImpl::visitStringInterpolation(StringInterpolation* expr)
{
   out << '"';

   for (auto &str : expr->getSegments()) {
      if (auto S = dyn_cast<StringLiteral>(expr)) {
         out << S->getValue();
      }
      else {
         out << "${";
         visit(str);
         out << "}";
      }
   }

   out << '"';
}

void PrettyPrinterImpl::visitLambdaExpr(LambdaExpr* expr)
{
   WriteList(expr->getArgs(), &PrettyPrinterImpl::visitDecl);
   out << " => ";

   visit(expr->getBody());
}

void PrettyPrinterImpl::visitTupleLiteral(TupleLiteral* expr)
{
   WriteList(expr->getElements(), &PrettyPrinterImpl::visitExpr);
}

void PrettyPrinterImpl::visitArrayLiteral(ArrayLiteral* expr)
{
   WriteList(expr->getValues(), &PrettyPrinterImpl::visitExpr, "[", ", ", "]");
}

void PrettyPrinterImpl::visitDictionaryLiteral(DictionaryLiteral* expr)
{
   out << "[";
   if (expr->size() == 0) {
      out << ":]";
      return;
   }

   size_t i = 0;
   for (auto &K : expr->getKeys()) {
      if (i != 0) out << ", ";

      auto &V = expr->getValues()[i];

      visitExpr(K);
      out << ": ";
      visitExpr(V);

      ++i;
   }

   out << "]";
}

void PrettyPrinterImpl::visitIdentifierRefExpr(IdentifierRefExpr* expr)
{
   if (expr->getIdentInfo())
      out << DeclarationName(expr->getIdentInfo());
}

void PrettyPrinterImpl::visitDeclRefExpr(DeclRefExpr *expr)
{
   out << expr->getDecl()->getDeclName();
}

void PrettyPrinterImpl::visitMemberRefExpr(MemberRefExpr *expr)
{
   out << "." << expr->getMemberDecl()->getDeclName();
}

void PrettyPrinterImpl::visitOverloadedDeclRefExpr(OverloadedDeclRefExpr *expr)
{

}

void PrettyPrinterImpl::visitBuiltinIdentExpr(BuiltinIdentExpr *expr)
{

}

void PrettyPrinterImpl::visitSelfExpr(SelfExpr *expr)
{
   out << "self";
}

void PrettyPrinterImpl::visitSuperExpr(SuperExpr *expr)
{
   out << "super";
}

void PrettyPrinterImpl::visitTupleMemberExpr(ast::TupleMemberExpr *expr)
{
   out << (expr->isPointerAccess() ? "->" : ".") << expr->getIndex();
}

void PrettyPrinterImpl::visitCallExpr(CallExpr* expr)
{
   if (expr->getParentExpr()) {
      out << (expr->isPointerAccess() ? "->" : ".");
   }

   if (auto F = expr->getFunc()) {
      out << F->getDeclName();
   }
   else {
      out << expr->getDeclName();
   }

   WriteList(expr->getArgs(), &PrettyPrinterImpl::visitExpr);
}

void PrettyPrinterImpl::visitAnonymousCallExpr(AnonymousCallExpr *expr)
{
   WriteList(expr->getArgs(), &PrettyPrinterImpl::visitExpr);
}

void PrettyPrinterImpl::visitEnumCaseExpr(EnumCaseExpr* expr)
{
   out << "." << expr->getIdent();
   if (!expr->getArgs().empty())
      WriteList(expr->getArgs(), &PrettyPrinterImpl::visitExpr);
}

void PrettyPrinterImpl::visitSubscriptExpr(SubscriptExpr* expr)
{
   WriteList(expr->getIndices(), &PrettyPrinterImpl::visitExpr, "[", ", ",
             "]");
}

void PrettyPrinterImpl::visitTemplateArgListExpr(TemplateArgListExpr *expr)
{
   WriteList(expr->getExprs(), &PrettyPrinterImpl::visitExpr, "<", ", ",
             ">");
}

void PrettyPrinterImpl::visitBuiltinExpr(
   BuiltinExpr* expr)
{

}

void PrettyPrinterImpl::visitExpressionPattern(ExpressionPattern* expr)
{
   visitExpr(expr->getExpr());
}

void PrettyPrinterImpl::visitCasePattern(CasePattern* expr)
{
   out << "." << expr->getCaseName();
}

void PrettyPrinterImpl::visitIsPattern(IsPattern* expr)
{
   out << "is ";
   visit(expr->getIsType().getTypeExpr());
}

void PrettyPrinterImpl::visitUnaryOperator(UnaryOperator* expr)
{
   out << op::toString(expr->getKind()) << " ";
   visitExpr(expr->getTarget());
}

void PrettyPrinterImpl::visitBinaryOperator(BinaryOperator* expr)
{
   visitExpr(expr->getLhs());
   out << " " << op::toString(expr->getKind()) << " ";
   visitExpr(expr->getRhs());
}

void PrettyPrinterImpl::visitAssignExpr(AssignExpr *expr)
{
   visitExpr(expr->getLhs());
   out << " = ";
   visitExpr(expr->getRhs());
}

void PrettyPrinterImpl::visitTypePredicateExpr(TypePredicateExpr *expr)
{
   visitExpr(expr->getLHS());
   out << " is ";
   visitExpr(expr->getRHS());
}

void PrettyPrinterImpl::visitExprSequence(ExprSequence* expr)
{
   size_t i = 0;
   for (auto &frag : expr->getFragments()) {
      if (i++ != 0) out << " ";
      if (frag.isExpression()) {
         visitExpr(frag.getExpr());
      }
      else if (frag.isOperator()) {
         out << op::toString(frag.getOperatorKind());
      }
      else {
         out << frag.getOp()->getIdentifier();
      }
   }
}

void PrettyPrinterImpl::visitCastExpr(CastExpr *expr)
{
   visit(expr->getTarget());

   switch (expr->getStrength()) {
      case CastStrength::Normal: out << "as"; break;
      case CastStrength::Force: out << "as!"; break;
      case CastStrength::Fallible: out << "as?"; break;
      default:
         llvm_unreachable("not an explicit cast!");
   }

   visit(expr->getTargetType());
}

void PrettyPrinterImpl::visitAddrOfExpr(AddrOfExpr *expr)
{
   out << "&";
   visitExpr(expr->getTarget());
}

void PrettyPrinterImpl::visitImplicitCastExpr(ImplicitCastExpr* expr)
{
   visitExpr(expr->getTarget());
}

void PrettyPrinterImpl::visitIfExpr(IfExpr *expr)
{
   out << "if ";
   visit(expr->getCond().ExprData.Expr);
   out << " then ";
   visit(expr->getTrueVal());
   out << " else ";
   visit(expr->getFalseVal());
}

void PrettyPrinterImpl::visitDeclStmt(DeclStmt *stmt)
{

}

void PrettyPrinterImpl::visitStaticExpr(StaticExpr* expr)
{
   if (expr->getExpr())
      visitExpr(expr->getExpr());
   else
      out << *expr->getEvaluatedExpr();
}

void PrettyPrinterImpl::visitConstraintExpr(ConstraintExpr* expr)
{
   switch (expr->getKind()) {
      case ConstraintExpr::Struct:
         out << "struct";
         break;
      case ConstraintExpr::Class:
         out << "class";
         break;
      case ConstraintExpr::Enum:
         out << "enum";
         break;
      case ConstraintExpr::Union:
         out << "union";
         break;
      case ConstraintExpr::Function:
         out << "def";
         break;
      case ConstraintExpr::DefaultConstructible:
         out << "default";
         break;
      case ConstraintExpr::Pointer:
         out << "*";
         break;
      case ConstraintExpr::Reference:
         out << "ref";
         break;
      case ConstraintExpr::Type: {
         out << expr->getTypeConstraint().getResolvedType();
         break;
      }
   }
}

void PrettyPrinterImpl::visitTraitsExpr(TraitsExpr* expr)
{
   using Kind = TraitsExpr::Kind;
   switch (expr->getKind()) {
      case Kind::Invalid:
         out << "<invalid>";
         break;
      case Kind::Compiles:
         out << "compiles";
         break;
      case Kind::CompileErrors:
         out << "compile_errors";
         break;
      case Kind::HasMember:
         out << "has_member";
         break;
      case Kind::HasProperty:
         out << "has_property";
         break;
      case Kind::HasStaticMember:
         out << "has_static_member";
         break;
      case Kind::HasStaticProperty:
         out << "has_static_property";
         break;
      case Kind::HasMethod:
         out << "has_method";
         break;
      case Kind::HasStaticMethod:
         out << "has_static_method";
         break;
      case Kind::ValidIdentifier:
         out << "valid_identifier";
         break;
      case Kind::ValidFunction:
         out << "valid_function";
         break;
      case Kind::IsInteger:
         out << "is_integer";
         break;
      case Kind::IsFloat:
         out << "is_fp";
         break;
      case Kind::IntegerBitwidth:
         out << "integer_bitwidth";
         break;
      case Kind::IsUnsigned:
         out << "is_unsigned";
         break;
      case Kind::FPPrecision:
         out << "fp_precision";
         break;
      case Kind::Arity:
         out << "arity";
         break;
   }
}

void PrettyPrinterImpl::visitMacroDecl(MacroDecl *decl)
{
   
}

void PrettyPrinterImpl::visitMacroExpansionDecl(MacroExpansionDecl *decl)
{
   
}

void PrettyPrinterImpl::visitMacroExpansionStmt(MacroExpansionStmt *decl)
{

}

void PrettyPrinterImpl::visitMacroExpansionExpr(MacroExpansionExpr *decl)
{

}

void PrettyPrinterImpl::visitMacroVariableExpr(MacroVariableExpr *decl)
{

}

void PrettyPrinterImpl::visitMixinExpr(ast::MixinExpr *expr)
{
   out << "mixin(";
   visitExpr(expr->getMixinExpr());
   out << ")";
}

void PrettyPrinterImpl::visitVariadicExpansionExpr(VariadicExpansionExpr *expr)
{
   visitExpr(expr);
   out << "...";
}

void PrettyPrinterImpl::visitMixinStmt(ast::MixinStmt *expr)
{
   out << "mixin(";
   visitExpr(expr->getMixinExpr());
   out << ")";
}

void PrettyPrinterImpl::visitMixinDecl(ast::MixinDecl *expr)
{
   out << "mixin(";
   visitExpr(expr->getMixinExpr());
   out << ")";
}

PrettyPrinter::PrettyPrinter(llvm::raw_ostream &out, Options opts)
   : pImpl(new PrettyPrinterImpl(out, opts))
{

}

PrettyPrinter::~PrettyPrinter()
{
   delete pImpl;
}

void PrettyPrinter::print(const Expression* expr)
{
   pImpl->visit(const_cast<Expression*>(expr));
}

void PrettyPrinter::print(const Statement* stmt)
{
   pImpl->visit(const_cast<Statement*>(stmt));
}

} // namespace ast
} // namespace cdot