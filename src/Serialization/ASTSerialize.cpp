////
//// Created by Jonas Zell on 09.01.18.
////

#include <llvm/ADT/SmallString.h>
#include "ASTSerialize.h"

#include "Support/ExtendedSerializerBase.h"
#include "Support/Casting.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "Sema/SemaPass.h"
#include "Sema/Template.h"

#include "AST/Type.h"

using namespace cdot::ast;
using namespace cdot::support;

namespace cdot {
namespace serial {

//class ASTSerializerImpl: public ExtendedSerializerBase<ASTSerializerImpl> {
//public:
//   using Options = ASTSerializer::Options;
//
//   ASTSerializerImpl(Options opts)
//      : opts(opts)
//   {}
//
//   void serialize(ast::Statement* const &stmt)
//   {
//      visit(stmt);
//   }
//
//   void serialize(ast::Statement const* stmt)
//   {
//      visit(stmt);
//   }
//
//   void WriteRecordDecl(ast::RecordDecl const* stmt,
//                        bool skipDefinitions,
//                        bool omitHeader = false,
//                        bool fullName = false);
//
//   void WriteCallableDecl(ast::CallableDecl const* stmt,
//                          bool skipBody,
//                          bool omitHeader = false);
//
//   void WriteMethodDecl(ast::MethodDecl const* stmt,
//                        bool skipBody,
//                        bool omitHeader = false);
//
//   void WriteInitDecl(ast::InitDecl const* stmt,
//                      bool skipBody,
//                      bool omitHeader = false);
//
//   void WriteDeinitDecl(ast::DeinitDecl const* stmt,
//                        bool skipBody,
//                        bool omitHeader = false);
//
//   void WritePropDecl(ast::PropDecl const* stmt,
//                      bool skipBody,
//                      bool omitHeader = false);
//
//   void WriteAliasDecl(ast::AliasDecl const* stmt,
//                       bool skipBody,
//                       bool omitHeader = false);
//
//   void WriteFieldDecl(ast::FieldDecl const* stmt,
//                       bool skipDefaultVal,
//                       bool omitHeader = false);
//
//   void WriteEnumCaseDecl(ast::EnumCaseDecl const* stmt,
//                          bool skipDefaultVal,
//                          bool omitHeader = false);
//
//private:
//   unsigned opts;
//
//   bool optionEnabled(Options opt)
//   {
//      return (opts & opt) != 0;
//   }
//
//   void visit(Statement const* node)
//   {
//      if (auto E = dyn_cast<Expression>(node))
//         WriteExprHeader(E);
//      else
//         WriteStmtHeader(node);
//
//      switch (node->getTypeID()) {
//#     define CDOT_ASTNODE(Name)                                 \
//         case AstNode::Name##ID:                                \
//            return visit##Name(static_cast<Name const*>(node));
//#     include "../../AstNode.def"
//
//         default:
//            llvm_unreachable("not a stmt");
//      }
//   }
//
//   void visitStmt(Statement* const& stmt)
//   {
//      visit(stmt);
//   }
//
//   void visitExpr(Expression* const& expr)
//   {
//      visit(expr);
//   }
//
//#  define CDOT_ASTNODE(Name)            \
//   void visit##Name(Name const* stmt);
//
//#  include "../../AstNode.def"
//
//   void visitRecordDecl(RecordDecl const* stmt);
//
//   void WriteExprHeader(Expression const *expr)
//   {
//      WriteStmtHeader(expr);
//      Writer.WriteByte(expr->isVariadicArgPackExpansion());
//   }
//
//   void WriteStmtHeader(Statement const *stmt)
//   {
//      Writer.WriteULEB128(stmt->getTypeID());
//      Writer.WriteULEB128(stmt->getSourceLoc().getOffset());
//      Writer.WriteULEB128(stmt->getSourceLoc().getSourceId());
//
//      if (auto ND = dyn_cast<NamedDecl>(stmt))
//         Writer.WriteByte((unsigned char)ND->getExternKind());
//
//      WriteList(stmt->getAttributes(), &ASTSerializerImpl::WriteAttribute);
//   }
//
//   void WriteAttribute(Attribute const &attr)
//   {
//
//   }
//
//   void WriteCatchBlock(CatchBlock const& CB)
//   {
//      visitStmt(CB.varDecl);
//      visitStmt(CB.body);
//   }
//
//   void WriteCaseArgument(CasePattern::Argument const& A)
//   {
//      WriteBool(A.isExpr());
//
//      if (A.isExpr()) {
//         visitExpr(A.getExpr());
//      }
//      else {
//         WriteBool(A.isConst());
//         WriteString(A.getIdentifier());
//         WriteSourceLoc(A.getSourceLoc());
//      }
//   }
//
//   void WriteSeqElement(ExprSequence::SequenceElement const& E)
//   {
//      Writer.WriteByte((char)E.getKind());
//      switch (E.getKind()) {
//         case ExprSequence::SequenceElement::EF_Expression:
//            visitExpr(E.getExpr());
//            break;
//         case ExprSequence::SequenceElement::EF_PossibleOperator:
//            WriteString(E.getOp());
//            WriteSourceLoc(E.getLoc());
//            break;
//         case ExprSequence::SequenceElement::EF_Operator:
//            Writer.WriteULEB128((size_t)E.getOperatorKind());
//            WriteSourceLoc(E.getLoc());
//            break;
//      }
//   }
//
//   void WriteTraitsArg(TraitsExpr::TraitsArgument const& A)
//   {
//      Writer.WriteByte(A.getKind());
//      switch (A.getKind()) {
//         case TraitsExpr::TraitsArgument::Type:
//            visitExpr(A.getType());
//            break;
//         case TraitsExpr::TraitsArgument::Stmt:
//            visitStmt(A.getStmt());
//            break;
//         case TraitsExpr::TraitsArgument::Expr:
//            visitExpr(A.getExpr());
//            break;
//         case TraitsExpr::TraitsArgument::String:
//            WriteString(A.getStr());
//            break;
//      }
//   }
//
//   template<class T>
//   void MaybeWriteStmt(T* const& val)
//   {
//      if (val) {
//         WriteBool(true);
//         visitStmt(val);
//      }
//      else {
//         WriteBool(false);
//      }
//   }
//
//   template<class T>
//   void MaybeWriteExpr(T* const& val)
//   {
//      if (val) {
//         WriteBool(true);
//         visitExpr(val);
//      }
//      else {
//         WriteBool(false);
//      }
//   }
//};
//
//class ASTDeserializerImpl {
//public:
//   ASTDeserializerImpl(SemaPass &SP,
//                       BitstreamReader &Reader,
//                       module::Module *M,
//                       SourceLocation loc)
//      : SP(SP), Context(SP.getCompilationUnit().getContext()), loc(loc),
//        Reader(Reader)
//   {}
//
//   Statement* read()
//   {
//      return readStmt();
//   }
//
//   ptrdiff_t getOffset() const
//   {
//      return Reader.getOffset();
//   }
//
//private:
//   SemaPass &SP;
//   ASTContext const& Context;
//   SourceLocation loc;
//   BitstreamReader &Reader;
//
//#  define CDOT_ASTNODE(Name)            \
//   Name* read##Name();
//
//#  include "../../AstNode.def"
//
//   struct StmtHeader {
//      SourceLocation sourceLoc;
//      ExternKind externKind;
//
//      std::vector<Attribute> attrs;
//   };
//
//   struct ExprHeader {
//      StmtHeader stmtHeader;
//      bool isVariadicArgPackExpansion;
//      Expression* SubExpr;
//   };
//
//   template <class T>
//   T* readSpecific()
//   {
//      auto kind = (AstNode::NodeType)Reader.ReadULEB128();
//
//      Statement* stmt;
//      switch (kind) {
//#     define CDOT_STMT(Name)                                                  \
//         case AstNode::Name##ID:                                              \
//            stmt = readStmtWithKnownKind(kind);                               \
//            break;
//#     define CDOT_EXPR(Name)                                                  \
//         case AstNode::Name##ID:                                              \
//            stmt = readExprWithKnownKind(kind);                               \
//            break;
//#     include "../../AstNode.def"
//
//         default:
//            llvm_unreachable("bad node kind");
//      }
//
//      return cast<T>(stmt);
//   }
//
//   Statement* readStmt()
//   {
//      auto kind = (AstNode::NodeType)Reader.ReadULEB128();
//      return readStmtWithKnownKind(kind);
//   }
//
//   Statement* readStmtWithKnownKind(AstNode::NodeType kind)
//   {
//      switch (kind) {
//#     define CDOT_EXPR(Name)                                    \
//         case AstNode::Name##ID:                                \
//            return readExprWithKnownKind(kind);
//#     include "../../AstNode.def"
//
//         default:
//            break;
//      }
//
//      auto offset = Reader.ReadULEB128();
//      auto sourceID = Reader.ReadULEB128();
//
//      ExternKind externKind = ExternKind::None;
//      if (NamedDecl::classofKind(kind)) {
//         externKind = (ExternKind)Reader.ReadByte();
//      }
//
//      auto attrs = ReadList<Attribute>(&ASTDeserializerImpl::ReadAttribute);
//
//      Statement* stmt;
//      switch (kind) {
//#     define CDOT_STMT(Name)                                    \
//         case AstNode::Name##ID:                                \
//            stmt = read##Name();                                \
//            break;
//#     include "../../AstNode.def"
//
//         default:
//            llvm_unreachable("not a stmt");
//      }
//
//      SourceLocation sourceLoc;
//      if (auto alias = SP.getCompilationUnit().getSourceAlias(sourceID)) {
//         sourceLoc = SourceLocation(offset, alias);
//      }
//      else {
//         sourceLoc = loc ? loc : SourceLocation(offset, sourceID);
//      }
//
//      stmt->setSourceLoc(sourceLoc);
//      stmt->setAttributes(std::move(attrs));
//
//      if (auto ND = dyn_cast<NamedDecl>(stmt))
//         ND->setExternKind(externKind);
//
//      return stmt;
//   }
//
//   Expression* readExpr()
//   {
//      auto kind = (AstNode::NodeType)Reader.ReadULEB128();
//      return readExprWithKnownKind(kind);
//   }
//
//   Expression* readExprWithKnownKind(AstNode::NodeType kind)
//   {
//      auto offset = Reader.ReadULEB128();
//      auto sourceID = Reader.ReadULEB128();
//      auto attrs = ReadList<Attribute>(&ASTDeserializerImpl::ReadAttribute);
//      bool isVariadicArgPackExpansion = Reader.ReadByte() != 0;
//
//      Expression* expr;
//      switch (kind) {
//#     define CDOT_EXPR(Name)                                    \
//         case AstNode::Name##ID:                                \
//            expr = read##Name();                                \
//            break;
//#     include "../../AstNode.def"
//
//         default:
//            llvm_unreachable("not an expression");
//      }
//
//      SourceLocation sourceLoc;
//      if (auto alias = SP.getCompilationUnit().getSourceAlias(sourceID)) {
//         sourceLoc = SourceLocation(offset, alias);
//      }
//      else {
//         sourceLoc = loc ? loc : SourceLocation(offset, sourceID);
//      }
//
//      expr->setSourceLoc(sourceLoc);
//      expr->setAttributes(std::move(attrs));
//      expr->setIsVariadicArgPackExpansion(isVariadicArgPackExpansion);
//
//      return expr;
//   }
//
//   template<class T, class ReaderT>
//   std::vector<T> ReadList(ReaderT read)
//   {
//      std::vector<T> vec;
//      auto size = Reader.ReadULEB128();
//
//      for (size_t i = 0; i < size; ++i)
//         vec.push_back((this->*read)());
//
//      return vec;
//   }
//
//   template<class T>
//   std::vector<T* > ReadASTVec()
//   {
//      std::vector<T* > vec;
//      auto size = Reader.ReadULEB128();
//
//      for (size_t i = 0; i < size; ++i)
//         vec.push_back(readSpecific<T>());
//
//      return vec;
//   }
//
//   string ReadString()
//   {
//      auto size = Reader.ReadULEB128();
//      string s;
//      s.reserve(size);
//
//      for (size_t i = 0; i < size; ++i)
//         s += Reader.ReadByte();
//
//      return s;
//   }
//
//   SourceLocation ReadSourceLoc()
//   {
//      return SourceLocation(Reader.ReadULEB128(), Reader.ReadULEB128());
//   }
//
//   PrecedenceGroup ReadPrecedenceGroup()
//   {
//      int prec = (int)Reader.ReadULEB128();
//      Associativity assoc = (Associativity)Reader.ReadByte();
//
//      return PrecedenceGroup(prec, assoc);
//   }
//
//   OperatorInfo ReadOperatorInfo()
//   {
//      auto PG = ReadPrecedenceGroup();
//      auto fix = (FixKind)Reader.ReadByte();
//      auto implicit = ReadBool();
//
//      return OperatorInfo(PG, fix, implicit);
//   }
//
//   CatchBlock ReadCatchBlock()
//   {
//      auto decl = readSpecific<LocalVarDecl>();
//      auto body = readStmt();
//
//      return CatchBlock(decl, body);
//   }
//
//   CasePattern::Argument ReadCaseArgument()
//   {
//      auto isExpr = ReadBool();
//
//      if (isExpr) {
//         return CasePattern::Argument(readExpr());
//      }
//
//      auto isConst = ReadBool();
//      auto s = ReadString();
//      auto loc = ReadSourceLoc();
//
//      return CasePattern::Argument(move(s), isConst, loc);
//   }
//
//   ExprSequence::SequenceElement ReadSeqElement()
//   {
//      auto kind = (ExprSequence::SequenceElement::Kind)Reader.ReadByte();
//      switch (kind) {
//         case ExprSequence::SequenceElement::EF_Expression:
//            return ExprSequence::SequenceElement(readExpr());
//         case ExprSequence::SequenceElement::EF_PossibleOperator: {
//            auto op = ReadString();
//            auto loc = ReadSourceLoc();
//
//            return ExprSequence::SequenceElement(move(op), loc);
//         }
//         case ExprSequence::SequenceElement::EF_Operator: {
//            auto opKind = (op::OperatorKind)Reader.ReadULEB128();
//            auto loc = ReadSourceLoc();
//
//            return ExprSequence::SequenceElement(opKind, loc);
//         }
//      }
//   }
//
//   TraitsExpr::TraitsArgument ReadTraitsArg()
//   {
//      auto kind = (TraitsExpr::TraitsArgument::Kind)Reader.ReadByte();
//      switch (kind) {
//         case TraitsExpr::TraitsArgument::Type:
//            return TraitsExpr::TraitsArgument(readSpecific<TypeRef>());
//         case TraitsExpr::TraitsArgument::Stmt:
//            return TraitsExpr::TraitsArgument(readStmt());
//         case TraitsExpr::TraitsArgument::Expr:
//            return TraitsExpr::TraitsArgument(readExpr());
//         case TraitsExpr::TraitsArgument::String:
//            return TraitsExpr::TraitsArgument(ReadString());
//      }
//   }
//
//   QualType ReadQualType()
//   {
//      char c = Reader.ReadByte();
//      return QualType(ReadType(), (c & 1) != 0);
//   }
//
//   Type *ReadType()
//   {
//      auto typeID = (TypeID)Reader.ReadByte();
//      switch (typeID) {
//         case TypeID::BuiltinTypeID:
//            return Context.getBuiltinType((BuiltinType::Kind)Reader.ReadByte());
//         case TypeID::PointerTypeID:
//            return Context.getPointerType(ReadQualType());
//         case TypeID::ArrayTypeID: {
//            auto numElements = Reader.ReadULEB128();
//            auto elementTy = ReadQualType();
//
//            return Context.getArrayType(elementTy, numElements);
//         }
//         case TypeID::TupleTypeID: {
//            return Context.getTupleType(
//               ReadList<QualType>(&ASTDeserializerImpl::ReadQualType));
//         }
//         case TypeID::FunctionTypeID: {
//            auto ret = ReadQualType();
//            auto args = ReadList<QualType>(&ASTDeserializerImpl::ReadQualType);
//            unsigned flags = unsigned(Reader.ReadULEB128());
//
//            return Context.getFunctionType(ret, move(args), flags);
//         }
//         case TypeID::RecordTypeID: {
//            return SP.getObjectTy(ReadString());
//         }
//         case TypeID::GenericTypeID: {
//            auto name = ReadString();
//            auto cov = ReadQualType();
//
//            return Context.getTemplateArgType(cov, name);
//         }
//         default:
//            llvm_unreachable("bad type!");
//      }
//   }
//
//   RecordDecl *readRecordDecl(AstNode::NodeType kind);
//
//   bool ReadBool()
//   {
//      return Reader.ReadByte() != 0;
//   }
//
//   Attribute ReadAttribute()
//   {
//      return {};
//   }
//
//   template<class T>
//   T* MaybeRead()
//   {
//      auto b = ReadBool();
//      if (b)
//         return readSpecific<T>();
//
//      return nullptr;
//   }
//
//   llvm::APInt readAPInt(bool isUnsigned)
//   {
//      static constexpr unsigned bits = sizeof(size_t) * 8;
//      if (ReadBool()) {
//         if (isUnsigned) {
//            return llvm::APInt(bits, Reader.ReadULEB128(), false);
//         }
//         else {
//            return llvm::APInt(bits, Reader.ReadULEB128(), true);
//         }
//      }
//      else {
//         return llvm::APSInt(ReadString());
//      }
//   }
//
//   llvm::APFloat readAPFloat(bool isSingle)
//   {
//      if (ReadBool()) {
//         if (isSingle) {
//            union {
//               size_t s;
//               float f;
//            } u;
//
//            u.s = Reader.ReadULEB128();
//            return llvm::APFloat(u.f);
//         }
//         else {
//            union {
//               size_t s;
//               double d;
//            } u;
//
//            u.s = Reader.ReadULEB128();
//            return llvm::APFloat(u.d);
//         }
//      }
//      else {
//         llvm::APFloat APF(0.0);
//         APF.convertFromString(ReadString(),
//                               llvm::APFloat::rmNearestTiesToEven);
//
//         return APF;
//      }
//   }
//};
//
//void ASTSerializerImpl::visitTranslationUnit(const TranslationUnit *stmt)
//{
//   llvm_unreachable("should not be serialized");
//}
//
//TranslationUnit* ASTDeserializerImpl::readTranslationUnit()
//{
//   llvm_unreachable("should not be serialized");
//}
//
//void ASTSerializerImpl::visitCompoundStmt(CompoundStmt const* stmt)
//{
//   WriteList(stmt->getStatements(), &ASTSerializerImpl::visitStmt);
//   WriteBool(stmt->preservesScope());
//}
//
//CompoundStmt* ASTDeserializerImpl::readCompoundStmt()
//{
//   return new (Context) CompoundStmt(
//      ReadList<Statement* >(&ASTDeserializerImpl::readStmt),
//      ReadBool()
//   );
//}
//
//void ASTSerializerImpl::visitTemplateParamDecl(const TemplateParamDecl *stmt)
//{
//   WriteString(stmt->getName());
//   WriteBool(stmt->isTypeName());
//   WriteBool(stmt->isVariadic());
//   MaybeWriteExpr(stmt->getCovariance());
//   MaybeWriteExpr(stmt->getContravariance());
//   MaybeWriteExpr(stmt->getDefaultValue());
//}
//
//TemplateParamDecl* ASTDeserializerImpl::readTemplateParamDecl()
//{
//   auto name = ReadString();
//   bool isTypeName = ReadBool();
//   bool variadic = ReadBool();
//   auto cov = MaybeRead<TypeRef>();
//   auto con = MaybeRead<TypeRef>();
//   auto def = MaybeRead<Expression>();
//
//   if (isTypeName) {
//      return new (Context) TemplateParamDecl(move(name), cov, con, variadic,
//                                             def);
//   }
//   else {
//      return new (Context) TemplateParamDecl(move(name), cov, variadic,
//                                             def);
//   }
//}
//
//void ASTSerializerImpl::visitBreakStmt(BreakStmt const* stmt)
//{
//
//}
//
//BreakStmt* ASTDeserializerImpl::readBreakStmt()
//{
//   return new (Context) BreakStmt();
//}
//
//void ASTSerializerImpl::visitContinueStmt(ContinueStmt const* stmt)
//{
//
//}
//
//ContinueStmt* ASTDeserializerImpl::readContinueStmt()
//{
//   return new (Context) ContinueStmt();
//}
//
//void ASTSerializerImpl::visitLabelStmt(LabelStmt const* stmt)
//{
//   WriteString(stmt->getLabelName());
//}
//
//LabelStmt* ASTDeserializerImpl::readLabelStmt()
//{
//   return new (Context) LabelStmt(ReadString());
//}
//
//void ASTSerializerImpl::visitGotoStmt(GotoStmt const* stmt)
//{
//   WriteString(stmt->getLabelName());
//}
//
//GotoStmt* ASTDeserializerImpl::readGotoStmt()
//{
//   return new (Context) GotoStmt(ReadString());
//}
//
//void ASTSerializerImpl::visitForStmt(ForStmt const* stmt)
//{
//   visitStmt(stmt->getInitialization());
//   visitExpr(stmt->getTermination());
//   visitStmt(stmt->getIncrement());
//   visitStmt(stmt->getBody());
//}
//
//ForStmt* ASTDeserializerImpl::readForStmt()
//{
//   return new (Context) ForStmt(readStmt(),
//                                readExpr(),
//                                readStmt(),
//                                readStmt());
//}
//
//void ASTSerializerImpl::visitForInStmt(ForInStmt const* stmt)
//{
//   visitStmt(stmt->getDecl());
//   visitExpr(stmt->getRangeExpr());
//   visitStmt(stmt->getBody());
//}
//
//ForInStmt* ASTDeserializerImpl::readForInStmt()
//{
//   return new (Context) ForInStmt(readSpecific<LocalVarDecl>(),
//                                  readExpr(),
//                                  readStmt());
//}
//
//void ASTSerializerImpl::visitIfStmt(IfStmt const* stmt)
//{
//   visitExpr(stmt->getCondition());
//   visitStmt(stmt->getIfBranch());
//
//   if (auto Else = stmt->getElseBranch()) {
//      WriteBool(true);
//      visitStmt(Else);
//   }
//}
//
//IfStmt* ASTDeserializerImpl::readIfStmt()
//{
//   auto cond = readExpr();
//   auto body = readStmt();
//
//   Statement* elseBody = nullptr;
//   if (ReadBool()) {
//      elseBody = readStmt();
//   }
//
//   return new (Context) IfStmt(move(cond), move(body), move(elseBody));
//}
//
//void ASTSerializerImpl::visitWhileStmt(WhileStmt const* stmt)
//{
//   visitExpr(stmt->getCondition());
//   visitStmt(stmt->getBody());
//   WriteBool(stmt->isAtLeastOnce());
//}
//
//WhileStmt* ASTDeserializerImpl::readWhileStmt()
//{
//   return new (Context) WhileStmt(readExpr(), readStmt(), ReadBool());
//}
//
//void ASTSerializerImpl::visitMatchStmt(MatchStmt const* stmt)
//{
//   visitExpr(stmt->getSwitchValue());
//   WriteList(stmt->getCases(), &ASTSerializerImpl::visitStmt);
//}
//
//MatchStmt* ASTDeserializerImpl::readMatchStmt()
//{
//   return new (Context) MatchStmt(
//      readExpr(),
//      ReadList<CaseStmt* >(&ASTDeserializerImpl::readCaseStmt));
//}
//
//void ASTSerializerImpl::visitCaseStmt(CaseStmt const* stmt)
//{
//   WriteBool(stmt->isDefault());
//   if (!stmt->isDefault())
//      visitExpr(stmt->getPattern());
//
//   visitStmt(stmt->getBody());
//}
//
//CaseStmt* ASTDeserializerImpl::readCaseStmt()
//{
//   auto isDefault = ReadBool();
//   if (isDefault) {
//      return new (Context) CaseStmt(nullptr, readStmt());
//   }
//   else {
//      return new (Context) CaseStmt(readSpecific<PatternExpr>(),
//                                    readStmt());
//   }
//}
//
//void ASTSerializerImpl::visitLocalVarDecl(LocalVarDecl const* stmt)
//{
//   WriteByte(stmt->getAccess());
//   WriteBool(stmt->isConst());
//   WriteString(stmt->getName());
//   visitExpr(stmt->getType());
//
//   MaybeWriteExpr(stmt->getValue());
//}
//
//LocalVarDecl* ASTDeserializerImpl::readLocalVarDecl()
//{
//   return new (Context) LocalVarDecl(
//      (AccessSpecifier)Reader.ReadByte(),
//      ReadBool(),
//      ReadString(),
//      readSpecific<TypeRef>(),
//      MaybeRead<Expression>()
//   );
//}
//
//void ASTSerializerImpl::visitGlobalVarDecl(GlobalVarDecl const* stmt)
//{
//   WriteByte(stmt->getAccess());
//   WriteBool(stmt->isConst());
//   WriteString(stmt->getName());
//   visitExpr(stmt->getType());
//
//   MaybeWriteExpr(stmt->getValue());
//}
//
//GlobalVarDecl* ASTDeserializerImpl::readGlobalVarDecl()
//{
//   return new (Context) GlobalVarDecl(
//      (AccessSpecifier)Reader.ReadByte(),
//      ReadBool(),
//      ReadString(),
//      readSpecific<TypeRef>(),
//      MaybeRead<Expression>()
//   );
//}
//
//void ASTSerializerImpl::visitLocalDestructuringDecl(
//   const LocalDestructuringDecl *stmt)
//{
//   llvm_unreachable("fuck this");
//}
//
//void ASTSerializerImpl::visitGlobalDestructuringDecl(
//   const GlobalDestructuringDecl *stmt)
//{
//   llvm_unreachable("fuck this");
//}
//
//GlobalDestructuringDecl * ASTDeserializerImpl::readGlobalDestructuringDecl()
//{
//   llvm_unreachable("fuck this");
//}
//
//LocalDestructuringDecl * ASTDeserializerImpl::readLocalDestructuringDecl()
//{
//   llvm_unreachable("fuck this");
//}
//
//void ASTSerializerImpl::visitAliasDecl(AliasDecl const* stmt)
//{
//   WriteAliasDecl(stmt, optionEnabled(Options::Opt_SkipDefinitions), true);
//}
//
//void ASTSerializerImpl::WriteAliasDecl(ast::AliasDecl const *stmt,
//                                       bool skipBody,
//                                       bool omitHeader) {
//   if (!omitHeader)
//      WriteStmtHeader(stmt);
//
//   WriteString(stmt->getName());
//   WriteList(stmt->getConstraints(), &ASTSerializerImpl::visitExpr);
//
//   if (!skipBody) {
//      visitExpr(stmt->getAliasExpr());
//   }
//}
//
//AliasDecl* ASTDeserializerImpl::readAliasDecl()
//{
//   return new (Context) AliasDecl(
//      ReadString(),
//      ReadASTVec<StaticExpr>(),
//      readSpecific<StaticExpr>()
//   );
//}
//
//void ASTSerializerImpl::visitRecordDecl(RecordDecl const* stmt)
//{
//   WriteRecordDecl(stmt, optionEnabled(Options::Opt_SkipDefinitions), true);
//}
//
//void ASTSerializerImpl::WriteRecordDecl(ast::RecordDecl const *stmt,
//                                        bool skipDefinitions,
//                                        bool omitHeader,
//                                        bool fullName) {
//   if (!omitHeader)
//      WriteStmtHeader(stmt);
//
//   Writer.WriteByte((unsigned char) stmt->getAccess());
//   WriteString(stmt->getName());
//
//   WriteList(stmt->getConformanceTypes(), &ASTSerializerImpl::visitExpr);
//
//   WriteList(stmt->getConstraints(), &ASTSerializerImpl::visitExpr);
//   WriteList(stmt->getStaticStatements(), &ASTSerializerImpl::visitStmt);
//
//   if (auto C = dyn_cast<ClassDecl>(stmt)) {
//      char c = 0;
//      c |= C->isAbstract();
//      c |= char(C->getParentType() != nullptr) << 3;
//
//      Writer.WriteByte(c);
//
//      if (auto P = C->getParentType())
//         visitExpr(P);
//   }
//   else if (auto E = dyn_cast<EnumDecl>(stmt)) {
//      MaybeWriteExpr(E->getRawType());
//   }
//
//   llvm::SmallVector<NamedDecl*, 16> decls;
//   decls.append(stmt->decl_begin(), stmt->decl_end());
//
//   WriteList(decls, &ASTSerializerImpl::visitStmt);
//}
//
//RecordDecl *ASTDeserializerImpl::readRecordDecl(AstNode::NodeType kind)
//{
//   auto access = (AccessSpecifier)Reader.ReadByte();
//   auto recordName = ReadString();
//   auto conformances = ReadASTVec<TypeRef>();
//   auto constraints = ReadASTVec<StaticExpr>();
//   auto staticStmts = ReadASTVec<StaticStmt>();
//
//   RecordDecl *decl;
//   switch (kind) {
//      case AstNode::StructDeclID:
//         decl = new (Context) StructDecl(access,
//                                         move(recordName),
//                                         move(conformances),
//                                         move(constraints));
//         break;
//      case AstNode::ClassDeclID: {
//         auto parentClass = readSpecific<TypeRef>();
//         bool isAbstract = ReadBool();
//
//         decl = new (Context) ClassDecl(access,
//                                        move(recordName),
//                                        move(conformances),
//                                        move(constraints),
//                                        parentClass, isAbstract);
//         break;
//      }
//      case AstNode::EnumDeclID: {
//         auto enumRawType = readSpecific<TypeRef>();
//         decl = new (Context) EnumDecl(access,
//                                       move(recordName),
//                                       move(conformances),
//                                       move(constraints),
//                                       enumRawType);
//         break;
//      }
//      case AstNode::UnionDeclID:
//         decl = new (Context) UnionDecl(access,
//                                    move(recordName),
//                                    move(conformances),
//                                    move(constraints));
//         break;
//      case AstNode::ProtocolDeclID:
//         decl = new (Context) ProtocolDecl(access,
//                                       move(recordName),
//                                       move(conformances),
//                                       move(constraints));
//         break;
//      case AstNode::ExtensionDeclID:
//         decl = new (Context) ExtensionDecl(access,
//                                        move(recordName),
//                                        move(conformances),
//                                        move(constraints));
//         break;
//      default:
//         llvm_unreachable("not a record decl!");
//   }
//
//   auto numDecls = Reader.ReadULEB128();
//   for (size_t i = 0; i < numDecls; ++i)
//      SP.addDeclToContext(*decl, readSpecific<NamedDecl>());
//
//   return decl;
//}
//
//void ASTSerializerImpl::visitStructDecl(const StructDecl *stmt)
//{
//   visitRecordDecl(stmt);
//}
//
//void ASTSerializerImpl::visitClassDecl(ClassDecl const* stmt)
//{
//   visitRecordDecl(stmt);
//}
//
//ClassDecl* ASTDeserializerImpl::readClassDecl()
//{
//   return cast<ClassDecl>(readRecordDecl(AstNode::ClassDeclID));
//}
//
//StructDecl * ASTDeserializerImpl::readStructDecl()
//{
//   return cast<StructDecl>(readRecordDecl(AstNode::StructDeclID));
//}
//
//void ASTSerializerImpl::visitEnumDecl(EnumDecl const* stmt)
//{
//   visitRecordDecl(stmt);
//}
//
//EnumDecl* ASTDeserializerImpl::readEnumDecl()
//{
//   return cast<EnumDecl>(readRecordDecl(AstNode::EnumDeclID));
//}
//
//void ASTSerializerImpl::visitEnumCaseDecl(EnumCaseDecl const* stmt)
//{
//   WriteEnumCaseDecl(stmt, optionEnabled(Options::Opt_SkipDefinitions), true);
//}
//
//void ASTSerializerImpl::WriteEnumCaseDecl(ast::EnumCaseDecl const *stmt,
//                                          bool skipCaseVal,
//                                          bool omitHeader) {
//   if (!omitHeader)
//      WriteStmtHeader(stmt);
//
//   WriteString(stmt->getName());
//
//   if (skipCaseVal)
//      WriteBool(false);
//   else
//      MaybeWriteExpr(stmt->getRawValExpr());
//
//   WriteList(stmt->getArgs(), &ASTSerializerImpl::visitStmt);
//}
//
//EnumCaseDecl* ASTDeserializerImpl::readEnumCaseDecl()
//{
//   auto name = ReadString();
//
//   StaticExpr* rawVal = nullptr;
//   if (ReadBool()) {
//      rawVal = readSpecific<StaticExpr>();
//   }
//
//   auto args = ReadASTVec<FuncArgDecl>();
//   return new (Context) EnumCaseDecl(move(name), move(rawVal), move(args));
//}
//
//void ASTSerializerImpl::visitUnionDecl(UnionDecl const* stmt)
//{
//   visitRecordDecl(stmt);
//}
//
//UnionDecl* ASTDeserializerImpl::readUnionDecl()
//{
//   return cast<UnionDecl>(readRecordDecl(AstNode::UnionDeclID));
//}
//
//void ASTSerializerImpl::visitProtocolDecl(ProtocolDecl const* stmt)
//{
//   visitRecordDecl(stmt);
//}
//
//ProtocolDecl* ASTDeserializerImpl::readProtocolDecl()
//{
//   return cast<ProtocolDecl>(readRecordDecl(AstNode::ProtocolDeclID));
//}
//
//void ASTSerializerImpl::visitMethodDecl(MethodDecl const* stmt)
//{
//   WriteMethodDecl(stmt, optionEnabled(Options::Opt_SkipDefinitions), true);
//}
//
//void ASTSerializerImpl::WriteMethodDecl(ast::MethodDecl const *stmt,
//                                        bool skipBody,
//                                        bool omitHeader) {
//   if (!omitHeader)
//      WriteStmtHeader(stmt);
//
//   WriteString(stmt->getName());
//   WriteBool(stmt->isAlias());
//
//   if (stmt->isAlias()) {
//      WriteString(stmt->getAlias());
//      WriteList(stmt->getArgs(), &ASTSerializerImpl::visitStmt);
//   }
//   else {
//      visitExpr(stmt->getReturnType());
//      WriteList(stmt->getArgs(), &ASTSerializerImpl::visitStmt);
//
//      WriteList(stmt->getConstraints(), &ASTSerializerImpl::visitExpr);
//
//      if (skipBody)
//         WriteBool(false);
//      else
//         MaybeWriteStmt(stmt->getBody());
//
//      WriteOperatorInfo(stmt->getOperator());
//      Writer.WriteByte((char) stmt->getAccess());
//
//      char c = 0;
//      c |= stmt->isCastOp();
//      c |= ((char)stmt->isStatic() << 1);
//
//      Writer.WriteByte(c);
//   }
//}
//
//MethodDecl* ASTDeserializerImpl::readMethodDecl()
//{
//   auto name = ReadString();
//   if (ReadBool()) {
//      auto alias = ReadString();
//      auto args = ReadASTVec<FuncArgDecl>();
//
//      return new (Context) MethodDecl(move(name), move(alias), move(args));
//   }
//   else {
//      auto ret = readSpecific<TypeRef>();
//      auto args = ReadASTVec<FuncArgDecl>();
//
//      auto constraints = ReadASTVec<StaticExpr>();
//
//      CompoundStmt* body = nullptr;
//      if (ReadBool()) {
//         body = readSpecific<CompoundStmt>();
//      }
//
//      auto Op = ReadOperatorInfo();
//      AccessSpecifier am = (AccessSpecifier)Reader.ReadByte();
//
//      char c = Reader.ReadByte();
//      auto isCastOp = (c & 1)        != 0;
//      auto isStatic = (c & (1 << 1)) != 0;
//
//      return new (Context) MethodDecl(move(name), move(ret), move(args),
//                                      move(constraints),
//                                      move(body), std::move(Op),
//                                      isCastOp, am, isStatic);
//   }
//}
//
//void ASTSerializerImpl::visitFieldDecl(FieldDecl const* stmt)
//{
//   WriteFieldDecl(stmt, optionEnabled(Options::Opt_SkipDefinitions), true);
//}
//
//void ASTSerializerImpl::WriteFieldDecl(ast::FieldDecl const *stmt,
//                                       bool skipDefaultVal,
//                                       bool omitHeader) {
//   if (!omitHeader)
//      WriteStmtHeader(stmt);
//
//   WriteString(stmt->getName());
//   visitExpr(stmt->getType());
//   Writer.WriteByte((char)stmt->getAccess());
//
//   char c = 0;
//   c |= stmt->isStatic();
//   c |= ((char)stmt->isConst() << 1);
//
//   Writer.WriteByte(c);
//
//   if (skipDefaultVal)
//      WriteBool(false);
//   else
//      MaybeWriteExpr(stmt->getDefaultVal());
//}
//
//FieldDecl* ASTDeserializerImpl::readFieldDecl()
//{
//   auto name = ReadString();
//   auto ty = readSpecific<TypeRef>();
//   auto am = (AccessSpecifier)Reader.ReadByte();
//
//   char c = Reader.ReadByte();
//   bool isStatic = (c & 1)        != 0;
//   bool isConst  = (c & (1 << 1)) != 0;
//
//   Expression* def = nullptr;
//   if (ReadBool()) {
//      def = readExpr();
//   }
//
//   return new (Context) FieldDecl(move(name), move(ty), am, isStatic,
//                                  isConst, move(def));
//}
//
//void ASTSerializerImpl::visitPropDecl(PropDecl const* stmt)
//{
//   WritePropDecl(stmt, optionEnabled(Options::Opt_SkipDefinitions), true);
//}
//
//void ASTSerializerImpl::WritePropDecl(ast::PropDecl const *stmt,
//                                      bool skipBody,
//                                      bool omitHeader) {
//   if (!omitHeader)
//      WriteStmtHeader(stmt);
//
//   WriteString(stmt->getName());
//   visitExpr(stmt->getType());
//   Writer.WriteByte((char)stmt->getAccess());
//
//   char c = 0;
//   c |= stmt->isStatic();
//   c |= ((char)stmt->hasDefinition() << 1);
//   c |= ((char)stmt->hasGetter()     << 2);
//   c |= ((char)stmt->hasSetter()     << 3);
//
//   Writer.WriteByte(c);
//
//   if (skipBody) {
//      WriteBool(false);
//      WriteBool(false);
//   }
//   else {
//      MaybeWriteStmt(stmt->getGetterBody());
//      MaybeWriteStmt(stmt->getSetterBody());
//   }
//
//   WriteString(stmt->getNewValName());
//}
//
//PropDecl* ASTDeserializerImpl::readPropDecl()
//{
//   auto name = ReadString();
//   auto ty = readSpecific<TypeRef>();
//   auto am = (AccessSpecifier)Reader.ReadByte();
//
//   char c = Reader.ReadByte();
//   bool isStatic  = (c & 1)        != 0;
//   bool hasDef    = (c & (1 << 1)) != 0;
//   bool hasGetter = (c & (1 << 2)) != 0;
//   bool hasSetter = (c & (1 << 3)) != 0;
//
//   CompoundStmt* getterBody = nullptr;
//   CompoundStmt* setterBody = nullptr;
//
//   if (ReadBool()) {
//      getterBody = readSpecific<CompoundStmt>();
//   }
//
//   if (ReadBool()) {
//      setterBody = readSpecific<CompoundStmt>();
//   }
//
//   auto newValName = ReadString();
//
//   return new (Context) PropDecl(move(name), move(ty), am, isStatic,
//                                 hasDef, hasGetter, hasSetter,
//                                 getterBody, setterBody,
//                                 move(newValName));
//}
//
//void ASTSerializerImpl::visitInitDecl(InitDecl const* stmt)
//{
//   WriteInitDecl(stmt, optionEnabled(Options::Opt_SkipDefinitions), true);
//}
//
//void ASTSerializerImpl::WriteInitDecl(ast::InitDecl const *stmt,
//                                      bool skipBody, bool omitHeader) {
//   if (!omitHeader)
//      WriteStmtHeader(stmt);
//
//   if (stmt->isMemberwise()) {
//      WriteBool(true);
//   }
//   else {
//      WriteBool(false);
//
//      WriteList(stmt->getArgs(), &ASTSerializerImpl::visitStmt);
//      Writer.WriteByte((char) stmt->getAccess());
//
//      if (skipBody)
//         WriteBool(false);
//      else
//         MaybeWriteStmt(stmt->getBody());
//   }
//}
//
//InitDecl* ASTDeserializerImpl::readInitDecl()
//{
//   auto memberwise = ReadBool();
//   if (memberwise) {
//      return new (Context) InitDecl();
//   }
//   else {
//      auto args = ReadASTVec<FuncArgDecl>();
//      auto am = (AccessSpecifier)Reader.ReadByte();
//
//      auto body = MaybeRead<CompoundStmt>();
//      return new (Context) InitDecl(move(args), am, move(body));
//   }
//}
//
//void ASTSerializerImpl::visitDeinitDecl(DeinitDecl const* stmt)
//{
//   WriteDeinitDecl(stmt, optionEnabled(Options::Opt_SkipDefinitions), true);
//}
//
//void ASTSerializerImpl::WriteDeinitDecl(ast::DeinitDecl const *stmt,
//                                        bool skipBody, bool omitHeader) {
//   if (!omitHeader)
//      WriteStmtHeader(stmt);
//
//   if (skipBody)
//      WriteBool(false);
//   else
//      MaybeWriteStmt(stmt->getBody());
//}
//
//DeinitDecl* ASTDeserializerImpl::readDeinitDecl()
//{
//   auto body = MaybeRead<CompoundStmt>();
//   return new (Context) DeinitDecl(move(body));
//}
//
//void ASTSerializerImpl::visitAssociatedTypeDecl(AssociatedTypeDecl const* stmt)
//{
//   WriteString(stmt->getProtocolSpecifier());
//   WriteString(stmt->getName());
//   WriteList(stmt->getConstraints(), &ASTSerializerImpl::visitStmt);
//   MaybeWriteExpr(stmt->getActualType());
//}
//
//AssociatedTypeDecl*
//ASTDeserializerImpl::readAssociatedTypeDecl()
//{
//   auto protoSpecifier = ReadString();
//   auto name = ReadString();
//   auto constraints = ReadASTVec<StaticExpr>();
//   auto ty = MaybeRead<TypeRef>();
//
//   return new (Context) AssociatedTypeDecl(move(protoSpecifier),
//                                           move(name), move(constraints),
//                                           move(ty));
//}
//
//void ASTSerializerImpl::visitTypedefDecl(TypedefDecl const* stmt)
//{
//   Writer.WriteByte((char)stmt->getAccess());
//   WriteString(stmt->getName());
//   visitExpr(stmt->getOriginTy());
//}
//
//TypedefDecl* ASTDeserializerImpl::readTypedefDecl()
//{
//   auto am = (AccessSpecifier)Reader.ReadByte();
//   auto name = ReadString();
//   auto ty = readSpecific<TypeRef>();
//
//   return new (Context) TypedefDecl(am, move(name), move(ty));
//}
//
//void ASTSerializerImpl::visitExtensionDecl(ExtensionDecl const* stmt)
//{
//   visitRecordDecl(stmt);
//}
//
//ExtensionDecl* ASTDeserializerImpl::readExtensionDecl()
//{
//   return cast<ExtensionDecl>(readRecordDecl(AstNode::ExtensionDeclID));
//}
//
//namespace {
//
//LLVM_ATTRIBUTE_UNUSED
//void printBinary(uintptr_t val)
//{
//   uintptr_t shift = 1;
//   for (unsigned i = 0; i < sizeof(uintptr_t) * CHAR_BIT; ++i, shift <<= 1) {
//      llvm::outs() << ((val & shift) != 0 ? "1" : "0");
//   }
//
//   llvm::outs() << "\n";
//}
//
//} // anonymous namespace
//
//void ASTSerializerImpl::visitFunctionDecl(FunctionDecl const* stmt)
//{
//   Writer.WriteByte((char) stmt->getAccess());
//   WriteString(stmt->getName());
//   visitExpr(stmt->getReturnType());
//   WriteList(stmt->getArgs(), &ASTSerializerImpl::visitStmt);
//
//   WriteList(stmt->getConstraints(), &ASTSerializerImpl::visitExpr);
//
//   if (!optionEnabled(Options::Opt_SkipDefinitions)
//       || !stmt->getTemplateParams().empty())
//      MaybeWriteStmt(stmt->getBody());
//   else
//      WriteBool(false);
//
//
//   WriteOperatorInfo(stmt->getOperator());
//}
//
//void ASTSerializerImpl::WriteCallableDecl(ast::CallableDecl const *stmt,
//                                          bool skipBody,
//                                          bool omitHeader) {
//   if (!omitHeader)
//      WriteStmtHeader(stmt);
//
//   Writer.WriteByte((char) stmt->getAccess());
//   WriteString(stmt->getName());
//   visitExpr(stmt->getReturnType());
//   WriteList(stmt->getArgs(), &ASTSerializerImpl::visitStmt);
//
//   WriteList(stmt->getConstraints(), &ASTSerializerImpl::visitExpr);
//
//   if (skipBody)
//      WriteBool(false);
//   else
//      MaybeWriteStmt(stmt->getBody());
//
//   WriteOperatorInfo(stmt->getOperator());
//}
//
//FunctionDecl* ASTDeserializerImpl::readFunctionDecl()
//{
//   auto am = (AccessSpecifier)Reader.ReadByte();
//   auto name = ReadString();
//   auto ret = readSpecific<TypeRef>();
//   auto args = ReadASTVec<FuncArgDecl>();
//   auto constraints = ReadASTVec<StaticExpr>();
//   auto body = MaybeRead<CompoundStmt>();
//   auto op = ReadOperatorInfo();
//
//   return new (Context) FunctionDecl(am, move(name), move(args),
//                                     move(ret),
//                                     move(constraints), move(body), op);
//}
//
//void ASTSerializerImpl::visitFuncArgDecl(FuncArgDecl const* stmt)
//{
//   WriteString(stmt->getArgName());
//   visitExpr(stmt->getArgType());
//   MaybeWriteExpr(stmt->getDefaultVal());
//
//   WriteBools(stmt->isVariadicArgPackExpansion(), stmt->isConst(),
//              stmt->isVararg(), stmt->isCstyleVararg());
//}
//
//FuncArgDecl* ASTDeserializerImpl::readFuncArgDecl()
//{
//   auto name = ReadString();
//   auto ty = readSpecific<TypeRef>();
//   auto def = MaybeRead<Expression>();
//
//   char c = Reader.ReadByte();
//   bool isVariadicExp = (c & 1) != 0;
//   bool isConst = (c & (1 << 1)) != 0;
//   bool vararg  = (c & (1 << 2)) != 0; (void)vararg;
//   bool cvararg = (c & (1 << 3)) != 0;
//
//   return new (Context) FuncArgDecl(move(name), move(ty), move(def),
//                                    isVariadicExp, isConst, cvararg);
//}
//
//void ASTSerializerImpl::visitNamespaceDecl(NamespaceDecl const* stmt)
//{
//   WriteBool(stmt->isAnonymousNamespace());
//   visitStmt(stmt->getBody());
//
//   if (!stmt->isAnonymousNamespace())
//      WriteString(stmt->getName());
//}
//
//NamespaceDecl* ASTDeserializerImpl::readNamespaceDecl()
//{
//   bool isAnon = ReadBool();
//   auto body = readSpecific<CompoundStmt>();
//
//   string name;
//   if (!isAnon)
//      name = ReadString();
//
//   return new (Context) NamespaceDecl(move(name), move(body), isAnon);
//}
//
//void ASTSerializerImpl::visitUsingDecl(UsingDecl const* stmt)
//{
//   WriteList(stmt->getDeclContextSpecifier(), &ASTSerializerImpl::WriteString);
//   WriteList(stmt->getImportedItems(), &ASTSerializerImpl::WriteString);
//   WriteBool(stmt->isWildCardImport());
//}
//
//UsingDecl* ASTDeserializerImpl::readUsingDecl()
//{
//   auto declCtx = ReadList<string>(&ASTDeserializerImpl::ReadString);
//   auto items = ReadList<string>(&ASTDeserializerImpl::ReadString);
//   auto wildcard = ReadBool();
//
//   return new (Context) UsingDecl(move(declCtx), move(items), wildcard);
//}
//
//void ASTSerializerImpl::visitModuleDecl(const ModuleDecl *stmt)
//{
//   WriteList(stmt->getQualifiedModuleName(), &ASTSerializerImpl::WriteString);
//}
//
//ModuleDecl* ASTDeserializerImpl::readModuleDecl()
//{
//   return new (Context) ModuleDecl(
//      ReadList<string>(&ASTDeserializerImpl::ReadString)
//   );
//}
//
//void ASTSerializerImpl::visitImportDecl(const ImportDecl *stmt)
//{
//   WriteList(stmt->getQualifiedModuleName(), &ASTSerializerImpl::WriteString);
//}
//
//ImportDecl* ASTDeserializerImpl::readImportDecl()
//{
//   return new (Context) ImportDecl(
//      ReadList<string>(&ASTDeserializerImpl::ReadString)
//   );
//}
//
//void ASTSerializerImpl::visitDebugStmt(DebugStmt const* stmt)
//{
//   WriteBool(stmt->isUnreachable());
//}
//
//DebugStmt* ASTDeserializerImpl::readDebugStmt()
//{
//   return new (Context) DebugStmt(ReadBool());
//}
//
//void ASTSerializerImpl::visitTryStmt(TryStmt const* stmt)
//{
//   visitStmt(stmt->getBody());
//   WriteList(stmt->getCatchBlocks(), &ASTSerializerImpl::WriteCatchBlock);
//   MaybeWriteStmt(stmt->getFinallyBlock());
//}
//
//TryStmt* ASTDeserializerImpl::readTryStmt()
//{
//   auto body = readStmt();
//   auto catchBlocks = ReadList<CatchBlock>
//      (&ASTDeserializerImpl::ReadCatchBlock);
//   auto finally = readStmt();
//
//   return new (Context) TryStmt(move(body), move(catchBlocks),
//                                    move(finally));
//}
//
//void ASTSerializerImpl::visitThrowStmt(ThrowStmt const* stmt)
//{
//   visitExpr(stmt->getThrownVal());
//}
//
//ThrowStmt* ASTDeserializerImpl::readThrowStmt()
//{
//   return new (Context) ThrowStmt(readExpr());
//}
//
//void ASTSerializerImpl::visitReturnStmt(ReturnStmt const* stmt)
//{
//   MaybeWriteExpr(stmt->getReturnValue());
//}
//
//ReturnStmt* ASTDeserializerImpl::readReturnStmt()
//{
//   auto retVal = MaybeRead<Expression>();
//   return new (Context) ReturnStmt(move(retVal));
//}
//
//void ASTSerializerImpl::visitNullStmt(NullStmt const* stmt)
//{
//
//}
//
//NullStmt* ASTDeserializerImpl::readNullStmt()
//{
//   return new (Context) NullStmt();
//}
//
//void ASTSerializerImpl::visitStaticIfStmt(StaticIfStmt const* stmt)
//{
//   visitExpr(stmt->getCondition());
//   visitStmt(stmt->getIfBranch());
//   MaybeWriteStmt(stmt->getElseBranch());
//}
//
//StaticIfStmt* ASTDeserializerImpl::readStaticIfStmt()
//{
//   auto cond = readSpecific<StaticExpr>();
//   auto ifBr = readStmt();
//   auto elseBr = MaybeRead<Statement>();
//
//   return new (Context) StaticIfStmt(move(cond), move(ifBr), move(elseBr));
//}
//
//void ASTSerializerImpl::visitStaticForStmt(StaticForStmt const* stmt)
//{
//   WriteString(stmt->getElementName());
//   visitExpr(stmt->getRange());
//   visitStmt(stmt->getBody());
//}
//
//StaticForStmt* ASTDeserializerImpl::readStaticForStmt()
//{
//   auto el = ReadString();
//   auto range = readSpecific<StaticExpr>();
//   auto body = readStmt();
//
//   return new (Context) StaticForStmt(move(el), move(range), move(body));
//}
//
//void ASTSerializerImpl::visitStaticAssertStmt(StaticAssertStmt const* stmt)
//{
//   visitExpr(stmt->getExpr());
//   WriteString(stmt->getMessage());
//}
//
//StaticAssertStmt* ASTDeserializerImpl::readStaticAssertStmt()
//{
//   auto cond = readSpecific<StaticExpr>();
//   auto msg = ReadString();
//
//   return new (Context) StaticAssertStmt(move(cond), move(msg));
//}
//
//void ASTSerializerImpl::visitStaticPrintStmt(StaticPrintStmt const* stmt)
//{
//   visitExpr(stmt->getExpr());
//}
//
//StaticPrintStmt* ASTDeserializerImpl::readStaticPrintStmt()
//{
//   return new (Context) StaticPrintStmt(readSpecific<StaticExpr>());
//}
//
//void ASTSerializerImpl::visitTypeRef(TypeRef const* expr)
//{
//   bool substituteClassName = false;
//   if (expr->isResolved()) {
//      switch (expr->getType()->getTypeID()) {
//         case TypeID::BuiltinTypeID:
//            WriteBool(true);
//            return WriteQualType(expr->getType());
//         case TypeID::RecordTypeID:
//            substituteClassName = true;
//            break;
//         default:
//            break;
//      }
//   }
//
//   WriteBool(false);
//   Writer.WriteByte((char)expr->getKind());
//
//   WriteBools(expr->isReference(), expr->isGlobalLookup(), expr->isMetaTy(),
//              expr->allowUnexpandedTemplateArgs());
//
//   switch (expr->getKind()) {
//      case TypeRef::Auto:
//         break;
//      case TypeRef::ArrayType:
//         visitExpr(expr->getArraySize());
//         visitExpr(expr->getElementType());
//         break;
//      case TypeRef::FunctionType:
//         visitExpr(expr->getReturnType());
//         LLVM_FALLTHROUGH;
//      case TypeRef::TupleType:
//         Writer.WriteULEB128(expr->getContainedTypes().size());
//         for (auto &cont : expr->getContainedTypes()) {
//            WriteString(cont.line);
//            visitExpr(cont.col);
//         }
//
//         break;
//      case TypeRef::RecordType:
//         if (substituteClassName) {
//            Writer.WriteULEB128(1);
//            WriteString(expr->getType()->getClassName());
//            Writer.WriteULEB128(0);
//         }
//         else {
//            Writer.WriteULEB128(expr->getNamespaceQual().size());
//            for (auto &cont : expr->getNamespaceQual()) {
//               WriteString(cont.line);
//               WriteList(cont.col, &ASTSerializerImpl::visitExpr);
//            }
//         }
//
//         break;
//      case TypeRef::DeclTypeExpr:
//         visitExpr(expr->getDeclTypeExpr());
//         break;
//      case TypeRef::Pointer:
//         visitExpr(expr->getSubject());
//         break;
//      case TypeRef::Option:
//         visitExpr(expr->getSubject());
//         break;
//   }
//}
//
//TypeRef* ASTDeserializerImpl::readTypeRef()
//{
//   if (ReadBool()) {
//      return new (Context) TypeRef(ReadQualType());
//   }
//
//   auto kind = (TypeRef::TypeKind)Reader.ReadByte();
//
//   char c = Reader.ReadByte();
//   bool is_reference = (c & (1 << 0)) != 0;
//   bool globalLookup = (c & (1 << 1)) != 0;
//   bool is_meta_ty   = (c & (1 << 2)) != 0;
//   bool allow_unexp  = (c & (1 << 3)) != 0;
//
//   TypeRef* typeref;
//   switch (kind) {
//      case TypeRef::Auto:
//         typeref = new (Context) TypeRef();
//         break;
//      case TypeRef::ArrayType: {
//         auto arrSize = readSpecific<StaticExpr>();
//         auto elTy = readSpecific<TypeRef>();
//
//         typeref = new (Context) TypeRef(move(elTy), move(arrSize));
//         break;
//      }
//      case TypeRef::FunctionType: {
//         auto ret = readSpecific<TypeRef>();
//         auto size = Reader.ReadULEB128();
//         std::vector<std::pair<string, TypeRef* >> cont;
//
//         for (size_t i = 0; i < size; ++i) {
//            auto name = ReadString();
//            auto ty = readSpecific<TypeRef>();
//
//            cont.emplace_back(std::move(name), move(ty));
//         }
//
//         typeref = new (Context) TypeRef(move(ret), move(cont));
//         break;
//      }
//      case TypeRef::TupleType: {
//         auto size = Reader.ReadULEB128();
//         std::vector<std::pair<string, TypeRef* >> cont;
//
//         for (size_t i = 0; i < size; ++i) {
//            auto name = ReadString();
//            auto ty = readSpecific<TypeRef>();
//
//            cont.emplace_back(std::move(name), move(ty));
//         }
//
//         typeref = new (Context) TypeRef(move(cont));
//         break;
//      }
//      case TypeRef::RecordType: {
//         TypeRef::NamespaceVec vec;
//         auto size = Reader.ReadULEB128();
//
//         for (size_t i = 0; i < size; ++i) {
//            auto name = ReadString();
//            auto TAs = ReadList<Expression*>
//               (&ASTDeserializerImpl::readExpr);
//
//            vec.emplace_back(move(name), move(TAs));
//         }
//
//         typeref = new (Context) TypeRef(move(vec));
//         break;
//      }
//      case TypeRef::DeclTypeExpr:
//         typeref = new (Context) TypeRef(readSpecific<StaticExpr>());
//         break;
//      case TypeRef::Pointer:
//      case TypeRef::Option:
//         typeref = new (Context) TypeRef(readSpecific<TypeRef>(), kind);
//         break;
//   }
//
//   typeref->isReference(is_reference);
//   typeref->setGlobalLookup(globalLookup);
//   typeref->isMetaTy(is_meta_ty);
//   typeref->setAllowUnexpandedTemplateArgs(allow_unexp);
//
//   return typeref;
//}
//
//void ASTSerializerImpl::visitParenExpr(const ParenExpr *stmt)
//{
//   WriteSourceLoc(stmt->getLParenLoc());
//   WriteSourceLoc(stmt->getRParenLoc());
//   visitExpr(stmt->getParenthesizedExpr());
//}
//
//ParenExpr * ASTDeserializerImpl::readParenExpr()
//{
//   auto lparenloc = ReadSourceLoc();
//   auto rparenloc = ReadSourceLoc();
//   auto expr = readExpr();
//
//   return new(Context) ParenExpr(lparenloc, rparenloc, expr);
//}
//
//void ASTSerializerImpl::visitIntegerLiteral(IntegerLiteral const* expr)
//{
//   WriteType(expr->getType());
//   WriteAPInt(expr->getValue(), expr->getType()->isUnsigned());
//}
//
//IntegerLiteral* ASTDeserializerImpl::readIntegerLiteral()
//{
//   auto type = ReadType();
//
//   static constexpr unsigned bits = sizeof(size_t) * 8;
//   llvm::APInt APInt(bits, 0);
//   if (ReadBool()) {
//      if (type->isUnsigned()) {
//         APInt = llvm::APInt(bits, Reader.ReadULEB128(), false);
//      }
//      else {
//         APInt = llvm::APInt(bits, Reader.ReadULEB128(), true);
//      }
//   }
//   else {
//      APInt = llvm::APSInt(ReadString());
//   }
//
//   return new (Context) IntegerLiteral(type, std::move(APInt));
//}
//
//void ASTSerializerImpl::visitFPLiteral(FPLiteral const* expr)
//{
//   WriteType(expr->getType());
//   WriteAPFloat(expr->getValue(), expr->getType()->isFloatTy());
//}
//
//FPLiteral* ASTDeserializerImpl::readFPLiteral()
//{
//   auto type = ReadType();
//
//   llvm::APFloat APFloat(0.0);
//   if (ReadBool()) {
//      if (type->isFloatTy()) {
//         union {
//            size_t s;
//            float f;
//         } u;
//
//         u.s = Reader.ReadULEB128();
//         APFloat = llvm::APFloat(u.f);
//      }
//      else {
//         union {
//            size_t s;
//            double d;
//         } u;
//
//         u.s = Reader.ReadULEB128();
//         APFloat = llvm::APFloat(u.d);
//      }
//   }
//   else {
//      APFloat.convertFromString(ReadString(),
//                                llvm::APFloat::rmNearestTiesToEven);
//   }
//
//   return new (Context) FPLiteral(type, std::move(APFloat));
//}
//
//void ASTSerializerImpl::visitBoolLiteral(BoolLiteral const* expr)
//{
//   WriteType(expr->getType());
//   WriteBool(expr->getValue());
//}
//
//BoolLiteral* ASTDeserializerImpl::readBoolLiteral()
//{
//   auto ty = ReadType();
//   return new (Context) BoolLiteral(ty, ReadBool());
//}
//
//void ASTSerializerImpl::visitCharLiteral(CharLiteral const* expr)
//{
//   WriteType(expr->getType());
//   Writer.WriteByte(expr->getNarrow());
//}
//
//CharLiteral* ASTDeserializerImpl::readCharLiteral()
//{
//   auto ty = ReadType();
//   return new (Context) CharLiteral(ty, Reader.ReadByte());
//}
//
//void ASTSerializerImpl::visitNoneLiteral(NoneLiteral const* expr)
//{
//
//}
//
//NoneLiteral* ASTDeserializerImpl::readNoneLiteral()
//{
//   return new (Context) NoneLiteral();
//}
//
//void ASTSerializerImpl::visitStringLiteral(StringLiteral const* expr)
//{
//   WriteString(expr->getValue());
//}
//
//StringLiteral* ASTDeserializerImpl::readStringLiteral()
//{
//   return new (Context) StringLiteral(ReadString());
//}
//
//void
//ASTSerializerImpl::visitStringInterpolation(StringInterpolation const* expr)
//{
//   WriteList(expr->getStrings(), &ASTSerializerImpl::visitExpr);
//}
//
//StringInterpolation*
//ASTDeserializerImpl::readStringInterpolation()
//{
//   return new (Context) StringInterpolation(ReadASTVec<Expression>());
//}
//
//void ASTSerializerImpl::visitLambdaExpr(LambdaExpr const* expr)
//{
//   MaybeWriteExpr(expr->getReturnType());
//   WriteList(expr->getArgs(), &ASTSerializerImpl::visitStmt);
//   visitStmt(expr->getBody());
//}
//
//LambdaExpr* ASTDeserializerImpl::readLambdaExpr()
//{
//   auto ret = MaybeRead<TypeRef>();
//   auto args = ReadASTVec<FuncArgDecl>();
//   auto body = MaybeRead<Statement>();
//
//   return new (Context) LambdaExpr(move(ret), move(args), move(body));
//}
//
//void ASTSerializerImpl::visitTupleLiteral(TupleLiteral const* expr)
//{
//   WriteList(expr->getElements(), &ASTSerializerImpl::visitExpr);
//}
//
//TupleLiteral* ASTDeserializerImpl::readTupleLiteral()
//{
//   return new (Context) TupleLiteral(ReadASTVec<Expression>());
//}
//
//void ASTSerializerImpl::visitArrayLiteral(ArrayLiteral const* expr)
//{
//   WriteList(expr->getValues(), &ASTSerializerImpl::visitExpr);
//}
//
//ArrayLiteral* ASTDeserializerImpl::readArrayLiteral()
//{
//   return new (Context) ArrayLiteral(ReadASTVec<Expression>());
//}
//
//void ASTSerializerImpl::visitDictionaryLiteral(DictionaryLiteral const* expr)
//{
//   WriteList(expr->getKeys(), &ASTSerializerImpl::visitExpr);
//   WriteList(expr->getValues(), &ASTSerializerImpl::visitExpr);
//}
//
//DictionaryLiteral* ASTDeserializerImpl::readDictionaryLiteral()
//{
//   auto keys = ReadASTVec<Expression>();
//   auto vals = ReadASTVec<Expression>();
//
//   return new (Context) DictionaryLiteral(move(keys), move(vals));
//}
//
//void ASTSerializerImpl::visitIdentifierRefExpr(IdentifierRefExpr const* expr)
//{
//   if (auto P = expr->getParentExpr()) {
//      WriteBool(true);
//      visitExpr(P);
//   }
//   else {
//      WriteBool(false);
//   }
//
//   WriteString(expr->getIdent());
//   WriteList(expr->getTemplateArgs(), &ASTSerializerImpl::visitExpr);
//}
//
//void ASTSerializerImpl::visitSelfExpr(const SelfExpr *stmt)
//{
//
//}
//
//SelfExpr* ASTDeserializerImpl::readSelfExpr()
//{
//   return new(Context) SelfExpr;
//}
//
//void ASTSerializerImpl::visitSuperExpr(const SuperExpr *stmt)
//{
//
//}
//
//SuperExpr* ASTDeserializerImpl::readSuperExpr()
//{
//   return new(Context) SuperExpr;
//}
//
//void ASTSerializerImpl::visitBuiltinIdentExpr(const BuiltinIdentExpr *stmt)
//{
//   WriteByte(stmt->getIdentifierKind());
//}
//
//BuiltinIdentExpr* ASTDeserializerImpl::readBuiltinIdentExpr()
//{
//   return new(Context) BuiltinIdentExpr((BuiltinIdentifier)Reader.ReadByte());
//}
//
//IdentifierRefExpr* ASTDeserializerImpl::readIdentifierRefExpr()
//{
//   Expression *Parent = nullptr;
//   if (ReadBool())
//      Parent = readExpr();
//
//   auto ident = ReadString();
//   auto templateArgs = ReadList<Expression*>(&ASTDeserializerImpl::readExpr);
//
//   auto E = new (Context) IdentifierRefExpr(move(ident), move(templateArgs));
//   E->setParentExpr(Parent);
//
//   return E;
//}
//
//void ASTSerializerImpl::visitMemberRefExpr(MemberRefExpr const* expr)
//{
//   visitExpr(expr->getParentExpr());
//   WriteBool(expr->isPointerAccess());
//   WriteString(expr->getIdent());
//   WriteList(expr->getTemplateArgs(), &ASTSerializerImpl::visitExpr);
//}
//
//MemberRefExpr* ASTDeserializerImpl::readMemberRefExpr()
//{
//   auto parent = readExpr();
//   auto isPointerAccess = ReadBool();
//   auto MemExpr = new (Context) MemberRefExpr(parent, ReadString(),
//                                              isPointerAccess);
//
//   auto templateArgs = ReadList<Expression*>(&ASTDeserializerImpl::readExpr);
//   MemExpr->setTemplateArgs(move(templateArgs));
//
//   return MemExpr;
//}
//
//void ASTSerializerImpl::visitTupleMemberExpr(const TupleMemberExpr *stmt)
//{
//   visitExpr(stmt->getParentExpr());
//
//   unsigned idx = stmt->getIndex();
//   idx &= (stmt->isPointerAccess() << 31);
//
//   Writer.WriteULEB128(idx);
//}
//
//TupleMemberExpr* ASTDeserializerImpl::readTupleMemberExpr()
//{
//   auto parent = readExpr();
//   auto idx = unsigned(Reader.ReadULEB128());
//   bool isPointerAccess = (idx & (1 << 31)) != 0;
//   idx &= ~(1 << 31);
//
//   return new (Context) TupleMemberExpr(parent, idx, isPointerAccess);
//}
//
//void ASTSerializerImpl::visitCallExpr(CallExpr const* expr)
//{
//   if (auto P = expr->getParentExpr()) {
//      WriteBool(true);
//      visitExpr(P);
//   }
//   else {
//      WriteBool(false);
//   }
//
//   WriteString(expr->getIdent());
//   WriteList(expr->getArgs(), &ASTSerializerImpl::visitExpr);
//
//   Writer.WriteEnum(expr->getBuiltinFnKind());
//   Writer.WriteEnum(expr->getKind());
//
//   WriteList(expr->getTemplateArgs(), &ASTSerializerImpl::visitExpr);
//}
//
//CallExpr* ASTDeserializerImpl::readCallExpr()
//{
//   Expression *Parent = nullptr;
//   if (ReadBool())
//      Parent = readExpr();
//
//   auto ident = ReadString();
//   auto args = ReadASTVec<Expression>();
//
//   auto builtin = (BuiltinFn)Reader.ReadULEB128();
//   auto kind = (CallKind)Reader.ReadULEB128();
//
//   auto templateArgs = ReadList<Expression*>(&ASTDeserializerImpl::readExpr);
//
//   auto call = new (Context) CallExpr(Parent, move(args), move(ident));
//
//   call->setKind(kind);
//   call->setBuiltinFnKind(builtin);
//   call->setTemplateArgs(move(templateArgs));
//
//   return call;
//}
//
//void ASTSerializerImpl::visitEnumCaseExpr(EnumCaseExpr const* expr)
//{
//   WriteString(expr->getIdent());
//   WriteList(expr->getArgs(), &ASTSerializerImpl::visitExpr);
//}
//
//EnumCaseExpr* ASTDeserializerImpl::readEnumCaseExpr()
//{
//   auto ident = ReadString();
//   auto args = ReadASTVec<Expression>();
//
//   return new (Context) EnumCaseExpr(move(ident), move(args));
//}
//
//void ASTSerializerImpl::visitSubscriptExpr(SubscriptExpr const* expr)
//{
//   visitExpr(expr->getParentExpr());
//   WriteList(expr->getIndices(), &ASTSerializerImpl::visitExpr);
//}
//
//SubscriptExpr* ASTDeserializerImpl::readSubscriptExpr()
//{
//   auto parent = readExpr();
//   return new (Context) SubscriptExpr(parent, ReadASTVec<Expression>());
//}
//
//void ASTSerializerImpl::visitBuiltinExpr(
//   BuiltinExpr const* expr)
//{
//   llvm_unreachable("bad boy");
//}
//
//BuiltinExpr*
//ASTDeserializerImpl::readBuiltinExpr()
//{
//   llvm_unreachable("bad boy");
//}
//
//void ASTSerializerImpl::visitExpressionPattern(ExpressionPattern const* expr)
//{
//   visitExpr(expr->getExpr());
//}
//
//ExpressionPattern* ASTDeserializerImpl::readExpressionPattern()
//{
//   return new (Context) ExpressionPattern(readExpr());
//}
//
//void ASTSerializerImpl::visitCasePattern(CasePattern const* expr)
//{
//   WriteString(expr->getCaseName());
//   WriteList(expr->getArgs(), &ASTSerializerImpl::WriteCaseArgument);
//}
//
//CasePattern* ASTDeserializerImpl::readCasePattern()
//{
//   auto caseName = ReadString();
//   auto args = ReadList<CasePattern::Argument>
//      (&ASTDeserializerImpl::ReadCaseArgument);
//
//   return new (Context) CasePattern(move(caseName), move(args));
//}
//
//void ASTSerializerImpl::visitIsPattern(IsPattern const* expr)
//{
//   visitExpr(expr->getIsType());
//}
//
//IsPattern* ASTDeserializerImpl::readIsPattern()
//{
//   return new (Context) IsPattern(readSpecific<TypeRef>());
//}
//
//void ASTSerializerImpl::visitUnaryOperator(UnaryOperator const* expr)
//{
//   Writer.WriteByte(expr->getKind());
//   WriteType(expr->getFunctionType());
//   visitExpr(expr->getTarget());
//   WriteBool(expr->isPrefix());
//}
//
//UnaryOperator* ASTDeserializerImpl::readUnaryOperator()
//{
//   auto kind = (op::OperatorKind)Reader.ReadByte();
//   auto FuncTy = ReadType()->asFunctionType();
//   auto expr = readExpr();
//   auto prefix = ReadBool();
//
//   return new (Context) UnaryOperator(kind, FuncTy, move(expr), prefix);
//}
//
//void ASTSerializerImpl::visitBinaryOperator(BinaryOperator const* expr)
//{
//   Writer.WriteByte(expr->getKind());
//   WriteType(expr->getFunctionType());
//   visitExpr(expr->getLhs());
//   visitExpr(expr->getRhs());
//}
//
//BinaryOperator* ASTDeserializerImpl::readBinaryOperator()
//{
//   auto kind = (op::OperatorKind)Reader.ReadByte();
//   auto FuncTy = ReadType()->asFunctionType();
//   auto lhs = readExpr();
//   auto rhs = readExpr();
//
//   return new (Context) BinaryOperator(kind, FuncTy, move(lhs), move(rhs));
//}
//
//void ASTSerializerImpl::visitTypePredicateExpr(const TypePredicateExpr *stmt)
//{
//   visitExpr(stmt->getLHS());
//   visitExpr(stmt->getRHS());
//   WriteByte(stmt->getKind());
//}
//
//TypePredicateExpr* ASTDeserializerImpl::readTypePredicateExpr()
//{
//   auto lhs = readExpr();
//   auto rhs = readExpr();
//   auto kind = (op::OperatorKind)Reader.ReadByte();
//
//   return new(Context) TypePredicateExpr(lhs, rhs, kind);
//}
//
//void ASTSerializerImpl::visitCastExpr(const CastExpr *expr)
//{
//   Writer.WriteByte(expr->getStrength());
//   visitExpr(expr->getTarget());
//   visitExpr(expr->getTargetType());
//}
//
//CastExpr* ASTDeserializerImpl::readCastExpr()
//{
//   auto kind = (CastStrength)Reader.ReadByte();
//   auto Expr = readExpr();
//   auto Ty = readSpecific<TypeRef>();
//
//   return new (Context) CastExpr(kind, Expr, Ty);
//}
//
//void ASTSerializerImpl::visitExprSequence(ExprSequence const* expr)
//{
//   WriteList(expr->getFragments(), &ASTSerializerImpl::WriteSeqElement);
//}
//
//ExprSequence* ASTDeserializerImpl::readExprSequence()
//{
//   auto frags = ReadList<ExprSequence::SequenceElement>
//      (&ASTDeserializerImpl::ReadSeqElement);
//
//   return new (Context) ExprSequence(move(frags));
//}
//
//void ASTSerializerImpl::visitIfExpr(const IfExpr *expr)
//{
//   visitExpr(expr->getCond());
//   visitExpr(expr->getTrueVal());
//   visitExpr(expr->getFalseVal());
//}
//
//IfExpr* ASTDeserializerImpl::readIfExpr()
//{
//   auto Cond = readExpr();
//   auto TrueVal = readExpr();
//   auto FalseVal = readExpr();
//
//   return new (Context) IfExpr(Cond, TrueVal, FalseVal);
//}
//
//
//void ASTSerializerImpl::visitImplicitCastExpr(ImplicitCastExpr const* expr)
//{
//   llvm_unreachable("should not be possible");
//}
//
//ImplicitCastExpr* ASTDeserializerImpl::readImplicitCastExpr()
//{
//   llvm_unreachable("should not be possible");
//}
//
//void ASTSerializerImpl::visitStaticExpr(StaticExpr const* expr)
//{
//   visitExpr(expr->getExpr());
//}
//
//StaticExpr* ASTDeserializerImpl::readStaticExpr()
//{
//   return new (Context) StaticExpr(readExpr());
//}
//
//void ASTSerializerImpl::visitConstraintExpr(ConstraintExpr const* expr)
//{
//   Writer.WriteByte(expr->getKind());
//   if (expr->getKind() == ConstraintExpr::Type)
//      visitExpr(expr->getTypeConstraint());
//}
//
//ConstraintExpr* ASTDeserializerImpl::readConstraintExpr()
//{
//   auto kind = (ConstraintExpr::Kind)Reader.ReadByte();
//   if (kind == ConstraintExpr::Type)
//      return new (Context) ConstraintExpr(readSpecific<TypeRef>());
//
//   return new (Context) ConstraintExpr(kind);
//}
//
//void ASTSerializerImpl::visitTraitsExpr(TraitsExpr const* expr)
//{
//   Writer.WriteByte((char)expr->getKind());
//   WriteList(expr->getArgs(), &ASTSerializerImpl::WriteTraitsArg);
//}
//
//TraitsExpr* ASTDeserializerImpl::readTraitsExpr()
//{
//   auto kind = (TraitsExpr::Kind)Reader.ReadByte();
//   auto args = ReadList<TraitsExpr::TraitsArgument>
//      (&ASTDeserializerImpl::ReadTraitsArg);
//
//   return new (Context) TraitsExpr(kind, move(args));
//}
//
//void ASTSerializerImpl::visitMixinExpr(const MixinExpr *stmt)
//{
//   visitExpr(stmt->getMixinExpr());
//}
//
//MixinExpr* ASTDeserializerImpl::readMixinExpr()
//{
//   return new (Context) MixinExpr(readExpr());
//}
//
//void ASTSerializerImpl::visitMixinDecl(const MixinDecl *stmt)
//{
//   visitExpr(stmt->getMixinExpr());
//}
//
//MixinDecl* ASTDeserializerImpl::readMixinDecl()
//{
//   return new (Context) MixinDecl(readExpr());
//}

ASTSerializer::ASTSerializer(Options opts)
//   : pImpl(new ASTSerializerImpl(opts))
{
   (void)pImpl;
}

ASTSerializer::~ASTSerializer()
{
//   delete pImpl;
}

void ASTSerializer::serialize(ast::Statement* const &stmt,
                              llvm::raw_ostream &out) {
   llvm_unreachable("NO");
//   pImpl->serialize(stmt);
//   pImpl->finalize(out);
//   pImpl->clear();
}

void ASTSerializer::serialize(ast::Statement const *stmt,
                              llvm::raw_ostream &out) {
   llvm_unreachable("NO");
//   pImpl->serialize(stmt);
//   pImpl->finalize(out);
//   pImpl->clear();
}

void ASTSerializer::serialize(ast::RecordDecl const *stmt,
                              bool skipBody,
                              bool fullName,
                              llvm::raw_ostream &out) {
   llvm_unreachable("NO");
//   pImpl->WriteRecordDecl(stmt, skipBody, false, fullName);
//   pImpl->finalize(out);
//   pImpl->clear();
}

void ASTSerializer::serialize(ast::CallableDecl const *stmt, bool skipBody,
                              llvm::raw_ostream &out) {
   llvm_unreachable("NO");
//   pImpl->WriteCallableDecl(stmt, skipBody);
//   pImpl->finalize(out);
//   pImpl->clear();
}

void ASTSerializer::serialize(ast::PropDecl const *stmt, bool skipBody,
                              llvm::raw_ostream &out) {
   llvm_unreachable("NO");
//   pImpl->WritePropDecl(stmt, skipBody);
//   pImpl->finalize(out);
//   pImpl->clear();
}

void ASTSerializer::serialize(ast::AliasDecl const *stmt, bool skipBody,
                              llvm::raw_ostream &out) {
   llvm_unreachable("NO");
//   pImpl->WriteAliasDecl(stmt, skipBody);
//   pImpl->finalize(out);
//   pImpl->clear();
}

void ASTSerializer::serialize(ast::FieldDecl const *stmt, bool skipBody,
                              llvm::raw_ostream &out) {
   llvm_unreachable("NO");
//   pImpl->WriteFieldDecl(stmt, skipBody);
//   pImpl->finalize(out);
//   pImpl->clear();
}

void ASTSerializer::serialize(ast::EnumCaseDecl const *stmt, bool skipBody,
                              llvm::raw_ostream &out) {
   llvm_unreachable("NO");
//   pImpl->WriteEnumCaseDecl(stmt, skipBody);
//   pImpl->finalize(out);
//   pImpl->clear();
}

ASTDeserializer::ASTDeserializer(SemaPass &SP,
                                 llvm::MemoryBuffer &in,
                                 module::Module *M,
                                 SourceLocation loc)
   : Reader(new BitstreamReader(&in))
//     pImpl(new ASTDeserializerImpl(SP, *Reader, M, loc))
{

}

ASTDeserializer::ASTDeserializer(SemaPass &SP, BitstreamReader &Reader,
                                 module::Module *M,
                                 SourceLocation loc)
//   : pImpl(new ASTDeserializerImpl(SP, Reader, M, loc))
{
   (void)pImpl;
}

ast::Statement* ASTDeserializer::Read()
{
   llvm_unreachable("NO");
//   return pImpl->read();
}

ptrdiff_t ASTDeserializer::getOffset() const
{
   llvm_unreachable("NO");
//   return pImpl->getOffset();
}

ASTDeserializer::~ASTDeserializer()
{
//   delete pImpl;
//   delete Reader;
}

} // namespace serial
} // namespace cdot