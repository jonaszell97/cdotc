#include "cdotc/AST/ASTDumper.h"

#include "cdotc/AST/ASTVisitor.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Expression.h"
#include "cdotc/AST/Statement.h"

#include "cdotc/Support/Format.h"
#include "cdotc/Support/WriterBase.h"

using namespace cdot::support;
using namespace cdot::ast;

namespace cdot {
namespace {

class DumperImpl : public WriterBase<DumperImpl, 3>,
                   public RecursiveASTVisitor<DumperImpl> {
public:
   using Options = ASTDumper::Options;

   DumperImpl(llvm::raw_ostream& out, Options opts)
       : WriterBase(out), opts(opts)
   {
      CurrentTab = 0;
      (void)this->opts;
   }

   ~DumperImpl() { out << "\n"; }

   void visit(Statement* node)
   {
      out << "\n";
      ApplyTab();

      TabGuard guard(this);
      WriteStmtCommon(node);

      RecursiveASTVisitor::visit(node);
   }

   void visit(Expression* node)
   {
      out << "\n";
      ApplyTab();

      TabGuard guard(this);
      WriteExprCommon(node);

      RecursiveASTVisitor::visit(node);
   }

   void visitExpr(Expression* expr) { visit(expr); }

   void visitStmt(Statement* stmt) { visit(stmt); }

   void visit(SourceType) {}

#define CDOT_STMT(Name) bool visit##Name(Name* stmt);

#define CDOT_EXPR(Name) bool visit##Name(Name* expr);

#include "cdotc/AST/AstNode.def"

#define CDOT_DECL(Name) bool visit##Name(Name* decl);

#include "cdotc/AST/Decl.def"

   bool visitRecordDecl(RecordDecl* stmt);

private:
   Options opts;

   bool visitVarDecl(VarDecl* node);

   void printAccessModifier(AccessSpecifier access)
   {
      switch (access) {
      case AccessSpecifier::Default:
         break;
      case AccessSpecifier::Public:
         out << "public";
         break;
      case AccessSpecifier::Private:
         out << "private";
         break;
      case AccessSpecifier::Protected:
         out << "protected";
         break;
      case AccessSpecifier::Internal:
         out << "internal";
         break;
      case AccessSpecifier::FilePrivate:
         out << "fileprivate";
         break;
      }
   }

   void WriteStringExprPair(std::pair<string, Expression*> const& pair)
   {
      if (!pair.first.empty())
         out << pair.first << ": ";

      visitExpr(pair.second);
   }

   void WriteStmtCommon(Statement* Stmt)
   {
      out << "<";
      WritePointer(Stmt);
      out << "> " << Stmt->getNodeTypeAsString();
   }

   void WriteExprCommon(Expression* Expr) { WriteStmtCommon(Expr); }

   void WriteNamedDeclCommon(NamedDecl* ND)
   {
      out << "[" << ND->getName() << ", ";
      printAccessModifier(ND->getAccess());
      out << "]";
   }
};

bool DumperImpl::visitDeclStmt(DeclStmt* stmt) { return true; }

bool DumperImpl::visitSourceFileDecl(SourceFileDecl* decl) { return true; }

bool DumperImpl::visitCompoundStmt(CompoundStmt* stmt) { return true; }

bool DumperImpl::visitTemplateParamDecl(TemplateParamDecl* stmt)
{
   out << "[" << stmt->getName() << "]";
   return true;
}

bool DumperImpl::visitBreakStmt(BreakStmt* stmt) { return true; }

bool DumperImpl::visitContinueStmt(ContinueStmt* stmt) { return true; }

bool DumperImpl::visitForStmt(ForStmt* stmt) { return true; }

bool DumperImpl::visitForInStmt(ForInStmt* stmt) { return true; }

bool DumperImpl::visitIfStmt(IfStmt* stmt) { return true; }

bool DumperImpl::visitWhileStmt(WhileStmt* stmt) { return true; }

bool DumperImpl::visitMatchStmt(MatchStmt* stmt) { return true; }

bool DumperImpl::visitCaseStmt(CaseStmt* stmt) { return true; }

bool DumperImpl::visitVarDecl(VarDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitLocalVarDecl(LocalVarDecl* stmt)
{
   return visitVarDecl(stmt);
}

bool DumperImpl::visitGlobalVarDecl(GlobalVarDecl* stmt)
{
   return visitVarDecl(stmt);
}

bool DumperImpl::visitDestructuringDecl(DestructuringDecl* decl)
{
   return true;
}

bool DumperImpl::visitAliasDecl(AliasDecl* stmt) { return true; }

bool DumperImpl::visitRecordDecl(RecordDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitClassDecl(ClassDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitStructDecl(StructDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitEnumDecl(EnumDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitEnumCaseDecl(EnumCaseDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitUnionDecl(UnionDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitProtocolDecl(ProtocolDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitMethodDecl(MethodDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitFieldDecl(FieldDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitPropDecl(PropDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitSubscriptDecl(SubscriptDecl* decl)
{
   WriteNamedDeclCommon(decl);
   return true;
}

bool DumperImpl::visitInitDecl(InitDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitDeinitDecl(DeinitDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitAttributedStmt(AttributedStmt* stmt) { return true; }

bool DumperImpl::visitAttributedExpr(AttributedExpr* expr) { return true; }

bool DumperImpl::visitAssociatedTypeDecl(AssociatedTypeDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitTypedefDecl(TypedefDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitExtensionDecl(ExtensionDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitFunctionDecl(FunctionDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitFuncArgDecl(FuncArgDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitNamespaceDecl(NamespaceDecl* stmt)
{
   WriteNamedDeclCommon(stmt);
   return true;
}

bool DumperImpl::visitUnittestDecl(UnittestDecl* decl) { return true; }

bool DumperImpl::visitUsingDecl(UsingDecl* stmt) { return true; }

bool DumperImpl::visitModuleDecl(ModuleDecl* stmt) { return true; }

bool DumperImpl::visitImportDecl(ImportDecl* stmt) { return true; }

bool DumperImpl::visitDebugStmt(DebugStmt* stmt) { return true; }

bool DumperImpl::visitDebugDecl(DebugDecl* decl) { return true; }

bool DumperImpl::visitDoStmt(DoStmt* stmt) { return true; }

bool DumperImpl::visitTryExpr(TryExpr* expr) { return true; }

bool DumperImpl::visitAwaitExpr(AwaitExpr* expr) { return true; }

bool DumperImpl::visitThrowStmt(ThrowStmt* stmt) { return true; }

bool DumperImpl::visitReturnStmt(ReturnStmt* stmt) { return true; }

bool DumperImpl::visitDiscardAssignStmt(DiscardAssignStmt* stmt)
{
   return true;
}

bool DumperImpl::visitNullStmt(NullStmt* stmt) { return true; }

bool DumperImpl::visitStaticIfStmt(StaticIfStmt* stmt) { return true; }

bool DumperImpl::visitStaticForStmt(StaticForStmt* stmt) { return true; }

bool DumperImpl::visitStaticAssertDecl(StaticAssertDecl* stmt) { return true; }

bool DumperImpl::visitStaticPrintDecl(StaticPrintDecl* stmt) { return true; }

bool DumperImpl::visitFunctionTypeExpr(FunctionTypeExpr* expr) { return true; }

bool DumperImpl::visitTupleTypeExpr(TupleTypeExpr* expr) { return true; }

bool DumperImpl::visitArrayTypeExpr(ArrayTypeExpr* expr) { return true; }

bool DumperImpl::visitDeclTypeExpr(DeclTypeExpr* expr) { return true; }

bool DumperImpl::visitReferenceTypeExpr(ReferenceTypeExpr* expr)
{
   return true;
}

bool DumperImpl::visitPointerTypeExpr(PointerTypeExpr* expr) { return true; }

bool DumperImpl::visitOptionTypeExpr(OptionTypeExpr* expr) { return true; }

bool DumperImpl::visitExistentialTypeExpr(ExistentialTypeExpr* expr)
{
   return true;
}

bool DumperImpl::visitParenExpr(ParenExpr* expr) { return true; }

bool DumperImpl::visitIntegerLiteral(IntegerLiteral* expr)
{
   out << "[" << expr->getValue().toString(10, !expr->getType()->isUnsigned())
       << "]";

   return true;
}

bool DumperImpl::visitFPLiteral(FPLiteral* expr)
{
   llvm::SmallString<128> str;
   expr->getValue().toString(str);

   out << "[" << str << "]";
   return true;
}

bool DumperImpl::visitBoolLiteral(BoolLiteral* expr)
{
   out << (expr->getValue() ? "[true]" : "[false]");
   return true;
}

bool DumperImpl::visitCharLiteral(CharLiteral* expr)
{
   auto c = support::unescape_char(expr->getNarrow());
   out << "['";

   if (c != expr->getNarrow())
      out << "\\";

   out << c << "']";
   return true;
}

bool DumperImpl::visitNoneLiteral(NoneLiteral* expr) { return true; }

bool DumperImpl::visitStringLiteral(StringLiteral* expr)
{
   out << "[" << '"' << expr->getValue() << '"' << "]";
   return true;
}

bool DumperImpl::visitStringInterpolation(StringInterpolation* expr)
{
   return true;
}

bool DumperImpl::visitLambdaExpr(LambdaExpr* expr) { return true; }

bool DumperImpl::visitTupleLiteral(TupleLiteral* expr) { return true; }

bool DumperImpl::visitArrayLiteral(ArrayLiteral* expr) { return true; }

bool DumperImpl::visitDictionaryLiteral(DictionaryLiteral* expr)
{
   return true;
}

bool DumperImpl::visitIdentifierRefExpr(IdentifierRefExpr* expr)
{
   out << "[" << expr->getIdent() << "]";
   return true;
}

bool DumperImpl::visitDeclRefExpr(DeclRefExpr* expr)
{
   out << "[" << expr->getDecl()->getFullName() << "]";
   return true;
}

bool DumperImpl::visitMemberRefExpr(MemberRefExpr* expr)
{
   out << "[" << expr->getMemberDecl()->getFullName() << "]";
   return true;
}

bool DumperImpl::visitOverloadedDeclRefExpr(OverloadedDeclRefExpr* expr)
{
   return true;
}

bool DumperImpl::visitBuiltinIdentExpr(BuiltinIdentExpr* expr) { return true; }

bool DumperImpl::visitSelfExpr(SelfExpr* expr) { return true; }

bool DumperImpl::visitSuperExpr(SuperExpr* expr) { return true; }

bool DumperImpl::visitTupleMemberExpr(TupleMemberExpr* expr) { return true; }

bool DumperImpl::visitCallExpr(CallExpr* expr)
{
   out << "[" << expr->getDeclName() << "]";
   return true;
}

bool DumperImpl::visitAnonymousCallExpr(AnonymousCallExpr* expr)
{
   return true;
}

bool DumperImpl::visitEnumCaseExpr(EnumCaseExpr* expr)
{
   out << "[." << DeclarationName(expr->getIdentInfo()) << "]";
   return true;
}

bool DumperImpl::visitSubscriptExpr(SubscriptExpr* expr) { return true; }

bool DumperImpl::visitTemplateArgListExpr(TemplateArgListExpr* expr)
{
   return true;
}

bool DumperImpl::visitBuiltinExpr(BuiltinExpr* expr) { return true; }

bool DumperImpl::visitExpressionPattern(ExpressionPattern* expr)
{
   return true;
}

bool DumperImpl::visitCasePattern(CasePattern* expr) { return true; }

bool DumperImpl::visitIsPattern(IsPattern* expr) { return true; }

bool DumperImpl::visitUnaryOperator(UnaryOperator* expr)
{
   out << "[" << op::toString(expr->getKind()) << "]";
   return true;
}

bool DumperImpl::visitBinaryOperator(BinaryOperator* expr)
{
   out << "[" << op::toString(expr->getKind()) << "]";
   return true;
}

bool DumperImpl::visitAssignExpr(AssignExpr* expr) { return true; }

bool DumperImpl::visitTypePredicateExpr(TypePredicateExpr* expr)
{
   return true;
}

bool DumperImpl::visitCastExpr(CastExpr* expr)
{
   switch (expr->getStrength()) {
   case CastStrength::Normal:
      out << "[as]";
      break;
   case CastStrength::Force:
      out << "[as!]";
      break;
   case CastStrength::Fallible:
      out << "[as?]";
      break;
   default:
      llvm_unreachable("not an explicit cast");
   }

   return true;
}

bool DumperImpl::visitAddrOfExpr(AddrOfExpr* expr) { return true; }

bool DumperImpl::visitExprSequence(ExprSequence* expr) { return true; }

bool DumperImpl::visitImplicitCastExpr(ImplicitCastExpr* expr) { return true; }

bool DumperImpl::visitIfExpr(IfExpr* expr) { return true; }

bool DumperImpl::visitStaticExpr(StaticExpr* expr) { return true; }

bool DumperImpl::visitConstraintExpr(ConstraintExpr* expr)
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

bool DumperImpl::visitTraitsExpr(TraitsExpr* expr)
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

bool DumperImpl::visitMacroDecl(MacroDecl* decl) { return false; }

bool DumperImpl::visitMacroExpansionDecl(MacroExpansionDecl* decl)
{
   return false;
}

bool DumperImpl::visitMacroExpansionStmt(MacroExpansionStmt* decl)
{
   return false;
}

bool DumperImpl::visitMacroExpansionExpr(MacroExpansionExpr* decl)
{
   return false;
}

bool DumperImpl::visitMacroVariableExpr(MacroVariableExpr* decl)
{
   return true;
}

bool DumperImpl::visitMixinExpr(MixinExpr* expr) { return true; }

bool DumperImpl::visitVariadicExpansionExpr(VariadicExpansionExpr* expr)
{
   return true;
}

bool DumperImpl::visitMixinStmt(MixinStmt* expr) { return true; }

bool DumperImpl::visitMixinDecl(MixinDecl* stmt) { return true; }

} // anonymous namespace

ASTDumper::ASTDumper(llvm::raw_ostream& out, Options opts)
    : pImpl(new DumperImpl(out, opts))
{
}

ASTDumper::~ASTDumper() { delete reinterpret_cast<DumperImpl*>(pImpl); }

void ASTDumper::print(Expression* expr)
{
   reinterpret_cast<DumperImpl*>(pImpl)->visit(expr);
}

void ASTDumper::print(Statement* stmt)
{
   reinterpret_cast<DumperImpl*>(pImpl)->visit(stmt);
}

} // namespace cdot