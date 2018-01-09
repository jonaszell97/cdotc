//
// Created by Jonas Zell on 05.01.18.
//

#include "PrettyPrinter.h"

#include "../ASTIncludes.h"
#include "../../../Support/Format.h"
#include "../../../Support/WriterBase.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

class PrettyPrinterImpl: public WriterBase<PrettyPrinterImpl, 3> {
public:
   using Options = PrettyPrinter::Options;

   PrettyPrinterImpl(llvm::raw_ostream &out, Options opts)
      : WriterBase(out), opts(opts)
   {}

   void visit(Statement const* node)
   {
      switch (node->getTypeID()) {
#     define CDOT_STMT(Name)                                \
         case AstNode::Name##ID:                            \
            return visit##Name(static_cast<Name const*>(node));
#     include "../../AstNode.def"

         default:
            llvm_unreachable("not a stmt");
      }
   }

   void visit(Expression const* node)
   {
      switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                \
         case AstNode::Name##ID:                            \
            visit##Name(static_cast<Name const*>(node)); break;
#     include "../../AstNode.def"

         default:
            llvm_unreachable("not an expr");
      }

      if (auto expr = node->getMemberExpr().get())
         visit(expr);
   }

   void visitExpr(std::shared_ptr<Expression> const& expr)
   {
      visit(expr.get());
   }

   void visitStmt(std::shared_ptr<Statement> const& stmt)
   {
      visit(stmt.get());
   }

#  define CDOT_STMT(Name)                                \
   void visit##Name(Name const* stmt);

#  define CDOT_EXPR(Name)                                \
   void visit##Name(Name const* expr);

#  include "../../AstNode.def"

private:
   Options opts;

   void visitVarDecl(VarDecl const* node);

   void printTemplateArg(TemplateArg const& TA)
   {
      if (TA.isTypeName()) {
         visitTypeRef(TA.getType().get());
      }
      else {
         visitStaticExpr(TA.getStaticExpr().get());
      }
   }

   void printTemplateParam(TemplateParameter const &TP)
   {
      if (!TP.isTypeName()) {
         out << "value ";
      }

      out << TP.getGenericTypeName();

      if (auto V = TP.unresolvedCovariance) {
         out << ": ";
         visitTypeRef(V.get());
      }
   }

   void visitCasePatternArg(CasePattern::Argument const &A)
   {
      if (A.isExpr()) {
         visitExpr(A.getExpr());
      }
      else {
         out << (A.isConst() ? "let " : "var ");
         out << A.getIdentifier();
      }
   }

   void printAccessModifier(AccessModifier access)
   {
      switch (access) {
         case AccessModifier::DEFAULT:
            break;
         case AccessModifier::PUBLIC:
            out << "public ";
            break;
         case AccessModifier::PRIVATE:
            out << "private ";
            break;
         case AccessModifier::PROTECTED:
            out << "protected ";
            break;
      }
   }

   void WriteStringExprPair(std::pair<string, std::shared_ptr<Expression>>
                            const& pair) {
      if (!pair.first.empty())
         out << pair.first << ": ";

      visitExpr(pair.second);
   }
};

void PrettyPrinterImpl::visitCompoundStmt(CompoundStmt const* stmt)
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

//CDOT_STMT(BreakStmt)

void PrettyPrinterImpl::visitBreakStmt(BreakStmt const* stmt)
{
   out << "break";
}

void PrettyPrinterImpl::visitContinueStmt(ContinueStmt const* stmt)
{
   out << "continue";
}

void PrettyPrinterImpl::visitLabelStmt(LabelStmt const* stmt)
{
   out << stmt->getLabelName() << ":";
}

void PrettyPrinterImpl::visitGotoStmt(GotoStmt const* stmt)
{
   out << "goto " << stmt->getLabelName();
}

void PrettyPrinterImpl::visitForStmt(ForStmt const* stmt)
{
   out << "for ";

   visitStmt(stmt->getInitialization());
   out << ";";

   visitStmt(stmt->getTermination());
   out << ";";

   visitStmt(stmt->getIncrement());
   visitStmt(stmt->getBody());
}

void PrettyPrinterImpl::visitForInStmt(ForInStmt const* stmt)
{
   out << "for ";
   visitLocalVarDecl(stmt->getDecl().get());

   out << " in ";
   visitExpr(stmt->getRangeExpr());

   visitStmt(stmt->getBody());
}

void PrettyPrinterImpl::visitIfStmt(IfStmt const* stmt)
{
   out << "if ";
   visitExpr(stmt->getCondition());

   visitStmt(stmt->getIfBranch());

   if (auto Else = stmt->getElseBranch()) {
      out << " else ";
      visitStmt(Else);
   }
}

void PrettyPrinterImpl::visitWhileStmt(WhileStmt const* stmt)
{
   out << "while ";
   visitExpr(stmt->getCondition());
   visitStmt(stmt->getBody());
}

void PrettyPrinterImpl::visitMatchStmt(MatchStmt const* stmt)
{
   out << "match ";
   visitExpr(stmt->getSwitchValue());

   out << "{\n";

   {
      TabGuard guard(this);
      for (auto &C : stmt->getCases()) {
         ApplyTab();
         visitCaseStmt(C.get());
         out << "\n";
      }
   }

   out << "}";
}

void PrettyPrinterImpl::visitCaseStmt(CaseStmt const* stmt)
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

void PrettyPrinterImpl::visitVarDecl(VarDecl const* stmt)
{
   out << (stmt->isConst() ? "let " : "var ");
   if (stmt->getIdentifiers().size() > 1) {
      WriteList(stmt->getIdentifiers(), &WriterBase::WriteString);
   }
   else {
      out << stmt->getIdentifiers().front();
   }

   out << ": ";
   visitTypeRef(stmt->getType().get());

   if (auto V = stmt->getValue()) {
      out << " = ";
      visitExpr(V);
   }
}

void PrettyPrinterImpl::visitLocalVarDecl(LocalVarDecl const* stmt)
{
   visitVarDecl(stmt);
}

void PrettyPrinterImpl::visitGlobalVarDecl(GlobalVarDecl const* stmt)
{
   printAccessModifier(stmt->getAccess());
   visitVarDecl(stmt);
}

void PrettyPrinterImpl::visitAliasDecl(AliasDecl const* stmt)
{
   out << "alias ";
   WriteList(stmt->getTemplateParams(),
             &PrettyPrinterImpl::printTemplateParam, "<", ", ", ">");

   WriteList(stmt->getConstraints(), &PrettyPrinterImpl::visitExpr,
             "", " ", "");

   out << " = ";
   visitExpr(stmt->getAliasExpr());
}

void PrettyPrinterImpl::visitRecordDecl(RecordDecl const* stmt)
{

}

void PrettyPrinterImpl::visitClassDecl(ClassDecl const* stmt)
{

}

void PrettyPrinterImpl::visitEnumDecl(EnumDecl const* stmt)
{

}

void PrettyPrinterImpl::visitEnumCaseDecl(EnumCaseDecl const* stmt)
{

}

void PrettyPrinterImpl::visitUnionDecl(UnionDecl const* stmt)
{

}

void PrettyPrinterImpl::visitProtocolDecl(ProtocolDecl const* stmt)
{

}

void PrettyPrinterImpl::visitMethodDecl(MethodDecl const* stmt)
{

}

void PrettyPrinterImpl::visitFieldDecl(FieldDecl const* stmt)
{

}

void PrettyPrinterImpl::visitPropDecl(PropDecl const* stmt)
{

}

void PrettyPrinterImpl::visitConstrDecl(ConstrDecl const* stmt)
{

}

void PrettyPrinterImpl::visitDestrDecl(DestrDecl const* stmt)
{

}

void PrettyPrinterImpl::visitAssociatedTypeDecl(AssociatedTypeDecl const* stmt)
{

}

void PrettyPrinterImpl::visitTypedefDecl(TypedefDecl const* stmt)
{

}

void PrettyPrinterImpl::visitExtensionDecl(ExtensionDecl const* stmt)
{

}

void PrettyPrinterImpl::visitCallableDecl(CallableDecl const* stmt)
{

}

void PrettyPrinterImpl::visitFunctionDecl(FunctionDecl const* stmt)
{

}

void PrettyPrinterImpl::visitFuncArgDecl(FuncArgDecl const* stmt)
{

}

void PrettyPrinterImpl::visitNamespaceDecl(NamespaceDecl const* stmt)
{

}

void PrettyPrinterImpl::visitUsingStmt(UsingStmt const* stmt)
{

}

void PrettyPrinterImpl::visitModuleStmt(const ModuleStmt *stmt)
{

}

void PrettyPrinterImpl::visitImportStmt(const ImportStmt *stmt)
{

}

void PrettyPrinterImpl::visitDeclareStmt(DeclareStmt const* stmt)
{

}

void PrettyPrinterImpl::visitDebugStmt(DebugStmt const* stmt)
{

}

void PrettyPrinterImpl::visitTryStmt(TryStmt const* stmt)
{
   out << "try ";
   visitStmt(stmt->getBody());
}

void PrettyPrinterImpl::visitThrowStmt(ThrowStmt const* stmt)
{
   out << "throw";
   if (auto val = stmt->getThrownVal()) {
      out << " ";
      visitExpr(val);
   }
}

void PrettyPrinterImpl::visitReturnStmt(ReturnStmt const* stmt)
{
   out << "return";
   if (auto val = stmt->getReturnValue()) {
      out << " ";
      visitExpr(val);
   }
}

void PrettyPrinterImpl::visitNullStmt(NullStmt const* stmt)
{
   out << ";";
}

void PrettyPrinterImpl::visitStaticIfStmt(StaticIfStmt const* stmt)
{

}

void PrettyPrinterImpl::visitStaticForStmt(StaticForStmt const* stmt)
{

}

void PrettyPrinterImpl::visitStaticAssertStmt(StaticAssertStmt const* stmt)
{

}

void PrettyPrinterImpl::visitStaticPrintStmt(StaticPrintStmt const* stmt)
{

}

void PrettyPrinterImpl::visitTypeRef(TypeRef const* expr)
{

}

void PrettyPrinterImpl::visitIntegerLiteral(IntegerLiteral const* expr)
{
   out << expr->getValue().toString();
}

void PrettyPrinterImpl::visitFPLiteral(FPLiteral const* expr)
{
   out << expr->getValue().toString();
}

void PrettyPrinterImpl::visitBoolLiteral(BoolLiteral const* expr)
{
   out << (expr->getValue() ? "true" : "false");
}

void PrettyPrinterImpl::visitCharLiteral(CharLiteral const* expr)
{
   auto c = support::unescape_char(expr->getNarrow());
   out << "'";

   if (c != expr->getNarrow())
      out << "\\";

   out << c << "'";
}

void PrettyPrinterImpl::visitNoneLiteral(NoneLiteral const* expr)
{
   out << "none";
}

void PrettyPrinterImpl::visitStringLiteral(StringLiteral const* expr)
{
   out << '"' << expr->getValue() << '"';
}

void PrettyPrinterImpl::visitStringInterpolation(StringInterpolation const* expr)
{
   out << '"';

   for (auto &str : expr->getStrings()) {
      if (auto S = dyn_cast<StringLiteral>(expr)) {
         out << S->getValue();
      }
      else {
         out << "${";
         visit(str.get());
         out << "}";
      }
   }

   out << '"';
}

void PrettyPrinterImpl::visitLambdaExpr(LambdaExpr const* expr)
{
   WriteList(expr->getArgs(), &PrettyPrinterImpl::visitStmt);
   out << " => ";

   visit(expr->getBody().get());
}

void PrettyPrinterImpl::visitTupleLiteral(TupleLiteral const* expr)
{
   WriteList(expr->getElements(), &PrettyPrinterImpl::WriteStringExprPair);
}

void PrettyPrinterImpl::visitArrayLiteral(ArrayLiteral const* expr)
{
   WriteList(expr->getValues(), &PrettyPrinterImpl::visitExpr, "[", ", ", "]");
}

void PrettyPrinterImpl::visitDictionaryLiteral(DictionaryLiteral const* expr)
{
   out << "[";
   if (expr->getKeys().empty()) {
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

void PrettyPrinterImpl::visitIdentifierRefExpr(IdentifierRefExpr const* expr)
{
   out << expr->getIdent();
   WriteList(expr->getTemplateArgs(), &PrettyPrinterImpl::printTemplateArg,
             "<", ", ", ">");
}

void PrettyPrinterImpl::visitMemberRefExpr(MemberRefExpr const* expr)
{
   out << "." << expr->getIdent();
   WriteList(expr->getTemplateArgs(), &PrettyPrinterImpl::printTemplateArg,
             "<", ", ", ">");
}

void PrettyPrinterImpl::visitCallExpr(CallExpr const* expr)
{
   WriteList(expr->getArgs(), &PrettyPrinterImpl::visitExpr);
}

void PrettyPrinterImpl::visitEnumCaseExpr(EnumCaseExpr const* expr)
{
   out << "." << expr->getIdent();
   if (!expr->getArgs().empty())
      WriteList(expr->getArgs(), &PrettyPrinterImpl::visitExpr);
}

void PrettyPrinterImpl::visitSubscriptExpr(SubscriptExpr const* expr)
{
   out << "[";
   visitExpr(expr->getIndex());
   out << "]";
}

void PrettyPrinterImpl::visitNonTypeTemplateArgExpr(
   NonTypeTemplateArgExpr const* expr)
{

}

void PrettyPrinterImpl::visitExpressionPattern(ExpressionPattern const* expr)
{
   visitExpr(expr->getExpr());
}

void PrettyPrinterImpl::visitCasePattern(CasePattern const* expr)
{
   out << "." << expr->getCaseName();
   if (!expr->getArgs().empty())
      WriteList(expr->getArgs(), &PrettyPrinterImpl::visitCasePatternArg);
}

void PrettyPrinterImpl::visitIsPattern(IsPattern const* expr)
{
   out << "is ";
   visitTypeRef(expr->getIsType().get());
}

void PrettyPrinterImpl::visitUnaryOperator(UnaryOperator const* expr)
{
   out << expr->getOp() << " ";
   visitExpr(expr->getTarget());
}

void PrettyPrinterImpl::visitBinaryOperator(BinaryOperator const* expr)
{
   visitExpr(expr->getLhs());
   out << " " << expr->getOp() << " ";
   visitExpr(expr->getRhs());
}

void PrettyPrinterImpl::visitTertiaryOperator(TertiaryOperator const* expr)
{
   out << "(";
   visitExpr(expr->getCondition());
   out << " ? ";
   visitExpr(expr->getLhs());
   out << " : ";
   visitExpr(expr->getRhs());
   out << ")";
}

void PrettyPrinterImpl::visitExprSequence(ExprSequence const* expr)
{
   visitExpr(expr->getResolvedExpression());
}

void PrettyPrinterImpl::visitImplicitCastExpr(ImplicitCastExpr const* expr)
{
   visitExpr(expr->getTarget());
}

void PrettyPrinterImpl::visitLvalueToRvalue(LvalueToRvalue const* expr)
{
   visitExpr(expr->getTarget());
}

void PrettyPrinterImpl::visitStaticExpr(StaticExpr const* expr)
{
   visitExpr(expr->getExpr());
}

void PrettyPrinterImpl::visitConstraintExpr(ConstraintExpr const* expr)
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
      case ConstraintExpr::Type:
         visitTypeRef(expr->getTypeConstraint().get());
         break;
   }
}

void PrettyPrinterImpl::visitTraitsExpr(TraitsExpr const* expr)
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

PrettyPrinter::PrettyPrinter(llvm::raw_ostream &out, Options opts)
   : pImpl(new PrettyPrinterImpl(out, opts))
{

}

PrettyPrinter::~PrettyPrinter()
{
   delete pImpl;
}

void PrettyPrinter::print(Expression const* expr)
{
   pImpl->visit(expr);
}

void PrettyPrinter::print(Statement const* stmt)
{
   pImpl->visit(stmt);
}

} // namespace ast
} // namespace cdot