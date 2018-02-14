//
// Created by Jonas Zell on 11.02.18.
//

#include "ASTDumper.h"

#include "AST/Passes/ASTIncludes.h"
#include "AST/Traverse.h"

#include "Support/Format.h"
#include "Support/WriterBase.h"

using namespace cdot::support;
using namespace cdot::ast;

namespace cdot {
namespace {

class DumperImpl: public WriterBase<DumperImpl, 3> {
public:
   using Options = ASTDumper::Options;

   DumperImpl(llvm::raw_ostream &out, Options opts)
      : WriterBase(out), opts(opts)
   {
      CurrentTab = 0;
   }

   ~DumperImpl()
   {
      out << "\n";
   }

   void visit(Statement const* node)
   {
      out << "\n";
      ApplyTab();

      TabGuard guard(this);
      WriteStmtCommon(node);

      bool writeChildren = true;
      switch (node->getTypeID()) {
#     define CDOT_STMT(Name)                                \
         case AstNode::Name##ID:                            \
            writeChildren = visit##Name(static_cast<Name const*>(node)); break;
#     include "../../AstNode.def"

         default:
            llvm_unreachable("not a stmt");
      }

      if (writeChildren)
         visitDirectChildren(node, [&](Statement *Child) {
            visit(Child);
         });
   }

   void visit(Expression const* node)
   {
      out << "\n";
      ApplyTab();

      TabGuard guard(this);
      WriteExprCommon(node);

      bool writeChildren = true;
      switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                \
         case AstNode::Name##ID:                            \
            writeChildren = visit##Name(static_cast<Name const*>(node)); break;
#     include "../../AstNode.def"

         default:
            llvm_unreachable("not an expr");
      }

      if (auto expr = node->getSubExpr())
         visit(expr);

      if (writeChildren)
         visitDirectChildren(node, [&](Statement *Child) {
            visit(Child);
         });
   }

   void visitExpr(Expression* expr)
   {
      visit(expr);
   }

   void visitStmt(Statement* stmt)
   {
      visit(stmt);
   }

#  define CDOT_STMT(Name)                                \
   bool visit##Name(Name const* stmt);

#  define CDOT_EXPR(Name)                                \
   bool visit##Name(Name const* expr);

#  include "../../AstNode.def"

   bool visitRecordDecl(RecordDecl const* stmt);

private:
   Options opts;

   bool visitVarDecl(VarDecl const* node);

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
            out << "public";
            break;
         case AccessModifier::PRIVATE:
            out << "private";
            break;
         case AccessModifier::PROTECTED:
            out << "protected";
            break;
      }
   }

   void WriteStringExprPair(std::pair<string, Expression* >
                            const& pair) {
      if (!pair.first.empty())
         out << pair.first << ": ";

      visitExpr(pair.second);
   }

   void WriteStmtCommon(Statement const* Stmt)
   {
      out << "<";
      WritePointer(Stmt);
      out << "> " << Stmt->getNodeTypeAsString();
   }

   void WriteExprCommon(Expression const* Expr)
   {
      WriteStmtCommon(Expr);
   }

   void WriteNamedDeclCommon(NamedDecl const* ND)
   {
      out << "[" << ND->getName() << ", ";
      printAccessModifier(ND->getAccess());
      out << "]";
   }
};

bool DumperImpl::visitCompoundStmt(CompoundStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitTranslationUnit(const TranslationUnit *stmt)
{
   return true;
}

bool DumperImpl::visitTemplateParamDecl(const TemplateParamDecl *stmt)
{
   out << "[" << stmt->getName() << "]";
   return true;
}

bool DumperImpl::visitTemplateArgExpr(const ast::TemplateArgExpr *expr)
{
   return true;
}

bool DumperImpl::visitBreakStmt(BreakStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitContinueStmt(ContinueStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitLabelStmt(LabelStmt const* stmt)
{
   out << "[" << stmt->getLabelName() << "]";
   return true;
}

bool DumperImpl::visitGotoStmt(GotoStmt const* stmt)
{
   out << "[" << stmt->getLabelName() << "]";
   return true;
}

bool DumperImpl::visitForStmt(ForStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitForInStmt(ForInStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitIfStmt(IfStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitWhileStmt(WhileStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitMatchStmt(MatchStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitCaseStmt(CaseStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitVarDecl(VarDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitLocalVarDecl(LocalVarDecl const* stmt)
{
   return visitVarDecl(stmt);
}

bool DumperImpl::visitGlobalVarDecl(GlobalVarDecl const* stmt)
{
   return visitVarDecl(stmt);
}

bool DumperImpl::visitLocalDestructuringDecl(
   const LocalDestructuringDecl *stmt)
{
   return true;
}

bool DumperImpl::visitGlobalDestructuringDecl(
   const GlobalDestructuringDecl *stmt)
{
   return true;
}

bool DumperImpl::visitAliasDecl(AliasDecl const* stmt)
{
   return true;
}

bool DumperImpl::visitRecordDecl(RecordDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitClassDecl(ClassDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitStructDecl(StructDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitEnumDecl(EnumDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitEnumCaseDecl(EnumCaseDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitUnionDecl(UnionDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitProtocolDecl(ProtocolDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitMethodDecl(MethodDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitFieldDecl(FieldDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitPropDecl(PropDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitInitDecl(InitDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitDeinitDecl(DeinitDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitAssociatedTypeDecl(AssociatedTypeDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitTypedefDecl(TypedefDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitExtensionDecl(ExtensionDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitFunctionDecl(FunctionDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitFuncArgDecl(FuncArgDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitNamespaceDecl(NamespaceDecl const* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitUsingStmt(UsingStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitModuleStmt(const ModuleStmt *stmt)
{
   return true;
}

bool DumperImpl::visitImportStmt(const ImportStmt *stmt)
{
   return true;
}

bool DumperImpl::visitDebugStmt(DebugStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitTryStmt(TryStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitThrowStmt(ThrowStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitReturnStmt(ReturnStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitNullStmt(NullStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitStaticIfStmt(StaticIfStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitStaticForStmt(StaticForStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitStaticAssertStmt(StaticAssertStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitStaticPrintStmt(StaticPrintStmt const* stmt)
{
   return true;
}

bool DumperImpl::visitTypeRef(TypeRef const* expr)
{
   return true;
}

bool DumperImpl::visitIntegerLiteral(IntegerLiteral const* expr)
{
   out << "["
       << expr->getValue().toString(10, !expr->getType()->isUnsigned())
       << "]";

   return true;
}

bool DumperImpl::visitFPLiteral(FPLiteral const* expr)
{
   llvm::SmallString<128> str;
   expr->getValue().toString(str);

   out << "[" << str << "]";
   return true;
}

bool DumperImpl::visitBoolLiteral(BoolLiteral const* expr)
{
   out << (expr->getValue() ? "[true]" : "[false]");
   return true;
}

bool DumperImpl::visitCharLiteral(CharLiteral const* expr)
{
   auto c = support::unescape_char(expr->getNarrow());
   out << "['";

   if (c != expr->getNarrow())
      out << "\\";

   out << c << "']";
   return true;
}

bool DumperImpl::visitNoneLiteral(NoneLiteral const* expr)
{
   return true;
}

bool DumperImpl::visitStringLiteral(StringLiteral const* expr)
{
   out << "[" << '"' << expr->getValue() << '"' << "]";
   return true;
}

bool DumperImpl::visitStringInterpolation(StringInterpolation const* expr)
{
   out << "[" << '"';

   for (auto &str : expr->getStrings()) {
      if (auto S = dyn_cast<StringLiteral>(expr)) {
         out << S->getValue();
      }
      else {
         out << "${";
         visit(str);
         out << "}";
      }
   }

   out << '"' << "]";
   return true;
}

bool DumperImpl::visitLambdaExpr(LambdaExpr const* expr)
{
   return true;
}

bool DumperImpl::visitTupleLiteral(TupleLiteral const* expr)
{
   return true;
}

bool DumperImpl::visitArrayLiteral(ArrayLiteral const* expr)
{
   return true;
}

bool DumperImpl::visitDictionaryLiteral(DictionaryLiteral const* expr)
{
   return true;
}

bool DumperImpl::visitIdentifierRefExpr(IdentifierRefExpr const* expr)
{
   out << "[" << expr->getIdent() << "]";
   return true;
}

bool DumperImpl::visitMemberRefExpr(MemberRefExpr const* expr)
{
   out << "[" << expr->getIdent() << "]";
   return true;
}

bool DumperImpl::visitCallExpr(CallExpr const* expr)
{
   out << "[" << expr->getIdent() << "]";
   return true;
}

bool DumperImpl::visitEnumCaseExpr(EnumCaseExpr const* expr)
{
   out << "[." << expr->getIdent() << "]";
   return true;
}

bool DumperImpl::visitSubscriptExpr(SubscriptExpr const* expr)
{
   return true;
}

bool DumperImpl::visitBuiltinExpr(BuiltinExpr const* expr)
{
   return true;
}

bool DumperImpl::visitExpressionPattern(ExpressionPattern const* expr)
{
   return true;
}

bool DumperImpl::visitCasePattern(CasePattern const* expr)
{
   return true;
}

bool DumperImpl::visitIsPattern(IsPattern const* expr)
{
   return true;
}

bool DumperImpl::visitUnaryOperator(UnaryOperator const* expr)
{
   out << "[" << expr->getOp() << "]";
   return true;
}

bool DumperImpl::visitBinaryOperator(BinaryOperator const* expr)
{
   out << "[" << expr->getOp() << "]";
   return true;
}

bool DumperImpl::visitTertiaryOperator(TertiaryOperator const* expr)
{
   return true;
}

bool DumperImpl::visitExprSequence(ExprSequence const* expr)
{
   return true;
}

bool DumperImpl::visitImplicitCastExpr(ImplicitCastExpr const* expr)
{
   return true;
}

bool DumperImpl::visitLvalueToRvalue(LvalueToRvalue const* expr)
{
   return true;
}

bool DumperImpl::visitStaticExpr(StaticExpr const* expr)
{
   return true;
}

bool DumperImpl::visitConstraintExpr(ConstraintExpr const* expr)
{
   out << "[";
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
         out << "type";
         break;
   }

   out << "]";
   return true;
}

bool DumperImpl::visitTraitsExpr(TraitsExpr const* expr)
{
   out << "[";

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

   out << "]";

   return true;
}

} // anonymous namespace

ASTDumper::ASTDumper(llvm::raw_ostream &out, Options opts)
   : pImpl(new DumperImpl(out, opts))
{

}

ASTDumper::~ASTDumper()
{
   delete reinterpret_cast<DumperImpl*>(pImpl);
}

void ASTDumper::print(Expression const* expr)
{
   reinterpret_cast<DumperImpl*>(pImpl)->visit(expr);
}

void ASTDumper::print(Statement const* stmt)
{
   reinterpret_cast<DumperImpl*>(pImpl)->visit(stmt);
}

} // namespace cdot