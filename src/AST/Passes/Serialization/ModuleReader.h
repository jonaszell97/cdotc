//
// Created by Jonas Zell on 17.10.17.
//

#ifndef CDOT_MODULEREADER_H
#define CDOT_MODULEREADER_H

#include <memory>
#include <stack>

#include "BitstreamReader.h"
#include "../../AstDeclarations.h"
#include "../../../Variant/Type/Type.h"
#include "../StaticAnalysis/Function.h"

namespace llvm {
class MemoryBuffer;
}

enum class ExternKind : unsigned char;

using std::string;
using cdot::BinaryOperator;

namespace cdot {

class BuiltinType;
struct Attribute;

namespace serial {

struct AstNodeHeader {
   std::vector<Attribute> attrs;
   bool returnedVal;
   bool sret;
   string binding;
};

struct StatementHeader {
   AstNodeHeader astNodeHeader;
   ExternKind externKind;
   bool isDeclaration;
};

struct ExpressionHeader {
   StatementHeader stmtHeader;
   unsigned uses;

   bool needsCast;
   Type castFrom;
   Type castTo;

   bool lvalueCast;
   bool byvalPass;
   bool isFuncArg;
   bool isLhsOfAssignment;
   bool isPartOfRetVal;
   bool isSetterCall;
   bool isAssigned;
   bool isEnumCase;
   bool isTemporary;

   string setterName;
   long caseVal;
};

class ModuleReader {
public:
   explicit ModuleReader(std::unique_ptr<llvm::MemoryBuffer> &&buff);
   std::shared_ptr<CompoundStmt> Read();

protected:
   BitstreamReader Reader;

   string moduleName;
   std::stack<std::shared_ptr<AstNode>> NodeStack;
   std::shared_ptr<CompoundStmt> root;

   string readString();
   size_t readSize();

   BuiltinType *readType();
   Type readQualType();
   Argument readArg();
   Attribute readAttr();
   GenericConstraint readGeneric();

   Variant readVariant();

   bool readPackedBool();
   void readBoolPack(bool **b, unsigned n);

   void readModuleHeader();

   AstNodeHeader readAstNodeHeader();
   StatementHeader readStmtHeader();
   ExpressionHeader readExprHeader();

   void copyAstNodeProps(AstNodeHeader &header, AstNode *node);
   void copyStmtProps(StatementHeader &header, Statement *stmt);
   void copyExprProps(ExpressionHeader &header, Expression *expr);

   std::shared_ptr<Expression> ReadExpr();
   std::shared_ptr<Statement> ReadNextStmt();

   std::shared_ptr<NamespaceDecl> ReadNamespaceDecl();
   std::shared_ptr<UsingStmt> ReadUsingStmt();
   std::shared_ptr<CompoundStmt> ReadCompoundStmt();

   std::shared_ptr<DeclStmt> ReadDeclStmt();
   std::shared_ptr<FunctionDecl> ReadFunctionDecl();
   std::shared_ptr<DeclareStmt> ReadDeclareStmt();

   std::shared_ptr<UnionDecl> ReadUnionDecl();
   std::shared_ptr<ClassDecl> ReadClassDecl();
   std::shared_ptr<MethodDecl> ReadMethodDecl();
   std::shared_ptr<FieldDecl> ReadFieldDecl();
   std::shared_ptr<ConstrDecl> ReadConstrDecl();
   std::shared_ptr<DestrDecl> ReadDestrDecl();

   std::shared_ptr<EnumDecl> ReadEnumDecl();
   std::shared_ptr<EnumCaseDecl> ReadEnumCaseDecl();

   std::shared_ptr<IdentifierRefExpr> ReadIdentifierRefExpr();
   std::shared_ptr<SubscriptExpr> ReadSubscriptExpr();
   std::shared_ptr<CallExpr> ReadCallExpr();
   std::shared_ptr<MemberRefExpr> ReadMemberRefExpr();

   std::shared_ptr<ForStmt> ReadForStmt();
   std::shared_ptr<ForInStmt> ReadForInStmt();
   std::shared_ptr<WhileStmt> ReadWhileStmt();
   std::shared_ptr<IfStmt> ReadIfStmt();
   std::shared_ptr<MatchStmt> ReadMatchStmt();
   std::shared_ptr<CaseStmt> ReadCaseStmt();
   std::shared_ptr<LabelStmt> ReadLabelStmt();
   std::shared_ptr<GotoStmt> ReadGotoStmt();

   std::shared_ptr<ReturnStmt> ReadReturnStmt();
   std::shared_ptr<BreakStmt> ReadBreakStmt();
   std::shared_ptr<ContinueStmt> ReadContinueStmt();

   std::shared_ptr<CollectionLiteral> ReadCollectionLiteral();
   std::shared_ptr<NumericLiteral> ReadNumericLiteral();
   std::shared_ptr<NoneLiteral> ReadNoneLiteral();
   std::shared_ptr<StringLiteral> ReadStringLiteral();
   std::shared_ptr<StringInterpolation> ReadStringInterpolation();
   std::shared_ptr<TupleLiteral> ReadTupleLiteral();

   std::shared_ptr<UnaryOperator> ReadUnaryOperator();
   std::shared_ptr<BinaryOperator> ReadBinaryOperator();
   std::shared_ptr<TertiaryOperator> ReadTertiaryOperator();

   std::shared_ptr<FuncArgDecl> ReadFuncArgDecl();
   std::shared_ptr<Expression> ReadExpression();
   std::shared_ptr<LambdaExpr> ReadLambdaExpr();
   std::shared_ptr<ImplicitCastExpr> ReadImplicitCastExpr();
   std::shared_ptr<TypedefDecl> ReadTypedefDecl();
   std::shared_ptr<TypeRef> ReadTypeRef();
   std::shared_ptr<LvalueToRvalue> ReadLvalueToRvalue();

   std::shared_ptr<EndOfFileStmt> ReadEndOfFileStmt();
   std::shared_ptr<DebugStmt> ReadDebugStmt();

   std::shared_ptr<Statement> ReadStatement();

   std::shared_ptr<TryStmt> ReadTryStmt();
   std::shared_ptr<ThrowStmt> ReadThrowStmt();
};

}
}

#endif //CDOT_MODULEREADER_H
