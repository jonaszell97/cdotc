//
// Created by Jonas Zell on 17.10.17.
//

#include <llvm/Support/raw_ostream.h>
#include "ModuleReader.h"

#include "../ASTIncludes.h"
#include "ModuleSerial.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/AutoType.h"
#include "../../../Variant/Type/VoidType.h"

namespace cdot {
namespace serial {

ModuleReader::ModuleReader(std::unique_ptr<llvm::MemoryBuffer> &&buff)
   : Reader(std::move(buff)),
     root(std::make_shared<CompoundStmt>())
{

}

std::shared_ptr<CompoundStmt> ModuleReader::Read()
{
   readModuleHeader();

   auto numStmts = readSize();
   for (size_t i = 0; i < numStmts; ++i) {
      root->addStatement(ReadNextStmt());
   }

   return root;
}

void ModuleReader::readModuleHeader()
{
   moduleName = readString();
   llvm::outs() << moduleName << "\n";
}

string ModuleReader::readString()
{
   size_t strLen = readSize();
   string s;
   s.reserve(strLen);

   for (size_t i = 0; i < strLen; ++i) {
      s += Reader.ReadByte();
   }

   return s;
}

size_t ModuleReader::readSize()
{
   return Reader.ReadULEB128();
}

bool ModuleReader::readPackedBool()
{
   return Reader.ReadBit();
}

void ModuleReader::readBoolPack(bool **b, unsigned n)
{
   for (int i = 0; i < n; ++i, ++b) {
      **b = Reader.ReadBit();
   }

   Reader.SkipCurrentByte();
}

Type ModuleReader::readQualType()
{
   bool lvalue, const_, self;
   readBoolPack((bool*[]) {
      &lvalue, &const_, &self
   }, 3);

   Type ty(readType());

   ty.isLvalue(lvalue);
   ty.isConst(const_);
   ty.isSelf(self);

   return ty;
}

BuiltinType *ModuleReader::readType()
{
   auto typeID = (TypeID)readSize();

   switch (typeID) {
      case TypeID::IntegerTypeID: {
         auto bits = (unsigned)readSize();
         auto isUnsigned = (bool)Reader.ReadByte();

         return IntegerType::get(bits, isUnsigned);
      }
      case TypeID::FPTypeID: {
         auto isFloat = (bool)Reader.ReadByte();

         return isFloat ? FPType::getFloatTy() : FPType::getDoubleTy();
      }
      case TypeID::PointerTypeID: {
         auto pointee = readQualType();
         return PointerType::get(pointee);
      }
      case TypeID::ObjectTypeID: {
         auto className = readString();
         auto numGenerics = readSize();

         std::vector<GenericType *> genericTypes;
         for (size_t i = 0; i < numGenerics; ++i) {
            auto gen = readType();
            assert(gen->isGeneric());
            genericTypes.push_back(gen->asGenericTy());
         }

         return ObjectType::get(className, genericTypes);
      }
      case TypeID::GenericTypeID: {
         auto genTypeName = readString();
         auto actualTy = readType();

         return GenericType::get(genTypeName, actualTy);
      }
      case TypeID::TupleTypeID: {
         std::vector<pair<string, BuiltinType*>> containedTypes;
         auto numTypes = readSize();

         for (size_t i = 0; i < numTypes; ++i) {
            auto name = readString();
            auto ty = readType();

            containedTypes.emplace_back(name, ty);
         }

         return TupleType::get(containedTypes);
      }
      case TypeID::FunctionTypeID: {
         auto isRawFuncTy = (bool)Reader.ReadByte();
         auto retTy = readQualType();

         std::vector<Argument> args;
         auto numArgs = readSize();
         for (size_t i = 0; i < numArgs; ++i) {
            args.push_back(readArg());
         }

         return FunctionType::get(retTy, args, isRawFuncTy);
      }
      case TypeID::AutoTypeID:
         return AutoType::get();
      case TypeID::VoidTypeID:
         return VoidType::get();
      default:
         llvm_unreachable("unsupported type");
   }
}

Argument ModuleReader::readArg()
{
   auto label = readString();
   auto ty = readQualType();

   auto hasDefault = (bool)Reader.ReadByte();

   Expression::SharedPtr defaultVal = nullptr;
   if (hasDefault) {
      defaultVal = ReadExpr();
   }

   Argument arg(label, ty, defaultVal);
   readBoolPack((bool*[]) {
      &arg.isVararg, &arg.cstyleVararg, &arg.ref
   }, 3);

   return arg;
}

Attribute ModuleReader::readAttr()
{
   Attribute attr;
   attr.kind = (Attr)readSize();
   attr.name = readString();

   auto numArgs = readSize();
   attr.args.reserve(numArgs);

   for (size_t i = 0; i < numArgs; ++i) {
      attr.args.push_back(readString());
   }

   return attr;
}

Variant ModuleReader::readVariant()
{
   Variant v;
   v.type = (VariantType)readSize();
   v.bitwidth = (int)readSize();

   readBoolPack((bool*[]) {
      &v.isUnsigned, &v.isBoxed, &v.rawStr
   }, 3);

   if (v.type != VariantType::STRING) {
      v.intVal = readSize();
   }
   else {
      v.strVal = readString();
   }

   return v;
}

GenericConstraint ModuleReader::readGeneric()
{
   GenericConstraint gen;
   gen.genericTypeName = readString();
   gen.covarName = readString();
   gen.contravarName = readString();

   return gen;
}

AstNodeHeader ModuleReader::readAstNodeHeader()
{
   AstNodeHeader header;
   auto numAttrs = readSize();

   for (size_t i = 0; i < numAttrs; ++i) {
      header.attrs.push_back(readAttr());
   }

   readBoolPack((bool*[]) {
      &header.returnedVal, &header.sret
   }, 2);

   header.binding = readString();
   return header;
}

void ModuleReader::copyAstNodeProps(
   AstNodeHeader &header,
   AstNode *node)
{
   node->setAttributes(std::move(header.attrs));
   node->isReturnedValue(header.returnedVal);
   node->isStructRetVal(header.sret);
}

StatementHeader ModuleReader::readStmtHeader()
{
   StatementHeader stmtHeader;
   stmtHeader.astNodeHeader = readAstNodeHeader();
   stmtHeader.externKind = (ExternKind)Reader.ReadByte();
   stmtHeader.isDeclaration = (bool)Reader.ReadByte();

   return stmtHeader;
}

void ModuleReader::copyStmtProps(
   StatementHeader &header,
   Statement *stmt)
{
   copyAstNodeProps(header.astNodeHeader, stmt);
   stmt->setExternKind(header.externKind);
   stmt->isDeclaration(header.isDeclaration);
}

ExpressionHeader ModuleReader::readExprHeader()
{
   ExpressionHeader exprHeader;
   exprHeader.stmtHeader = readStmtHeader();
   exprHeader.uses = (unsigned)readSize();

   exprHeader.needsCast = (bool)Reader.ReadByte();
   if (exprHeader.needsCast) {
      exprHeader.castFrom = readQualType();
      exprHeader.castTo = readQualType();
   }

   readBoolPack((bool*[]) {
      &exprHeader.lvalueCast, &exprHeader.byvalPass, &exprHeader.isFuncArg,
      &exprHeader.isLhsOfAssignment, &exprHeader.isPartOfRetVal, &exprHeader.isSetterCall,
      &exprHeader.isAssigned, &exprHeader.isEnumCase, &exprHeader.isTemporary
   }, 9);

   if (exprHeader.isSetterCall) {
      exprHeader.setterName = readString();
   }

   if (exprHeader.isEnumCase) {
      exprHeader.caseVal = readSize();
   }

   return exprHeader;
}

void ModuleReader::copyExprProps(
   ExpressionHeader &header,
   Expression *expr)
{
   copyStmtProps(header.stmtHeader, expr);
   expr->setUses(header.uses);
   expr->needsCast(header.needsCast);
   expr->setCastFrom(header.castFrom);
   expr->setCastTo(header.castTo);
   expr->isLvalueCast(header.lvalueCast);
   expr->needsByValPass(header.byvalPass);
   expr->setIsFunctionArgument(header.isFuncArg);
   expr->setIsLhsOfAssignment(header.isLhsOfAssignment);
   expr->setIsPartOfReturnValue(header.isPartOfRetVal);
   expr->isSetterCall(header.isSetterCall);
   expr->isAssigned(header.isAssigned);
   expr->isEnumCase(header.isEnumCase);
   expr->isTemporary(header.isTemporary);
   expr->setSetterName(header.setterName);
   expr->setCaseVal(header.caseVal);
}

std::shared_ptr<NamespaceDecl> ModuleReader::ReadNamespaceDecl()
{
   auto header = readStmtHeader();

   bool isAnon = Reader.ReadByte();
   string nsName;

   if (!isAnon) {
      nsName = readString();
   }

   auto id = Reader.ReadByte();
   assert(id == CMPND_STMT);

   auto contents = ReadCompoundStmt();
   auto ns = std::make_shared<NamespaceDecl>(
      nsName,
      contents,
      isAnon
   );

   copyStmtProps(header, ns.get());
   return ns;
}

std::shared_ptr<CompoundStmt> ModuleReader::ReadCompoundStmt()
{
   auto header = readStmtHeader();

   auto preserveScope = (bool)Reader.ReadByte();
   auto isUnsafe = (bool)Reader.ReadByte();

   auto compound = std::make_shared<CompoundStmt>(preserveScope);
   compound->isUnsafe(isUnsafe);

   auto numStmts = readSize();
   for (size_t i = 0; i < numStmts; ++i) {
      compound->addStatement(ReadNextStmt());
   }

   copyStmtProps(header, compound.get());
   return compound;
}

std::shared_ptr<UsingStmt> ModuleReader::ReadUsingStmt()
{
   auto header = readStmtHeader();
   auto kind = (UsingKind)Reader.ReadWord();

   auto importNamespace = readString();
   auto isWildcard = (bool)Reader.ReadByte();

   std::vector<string> fullNames;
   if (!isWildcard) {
      auto numItems = readSize();
      for (size_t i = 0; i < numItems; ++i) {
         fullNames.push_back(readString());
      }
   }

   auto stmt = std::make_shared<UsingStmt>(
      std::move(fullNames)
   );

   stmt->setIsWildcardImport(isWildcard);
   stmt->setKind(kind);
   stmt->setImportNamespace(importNamespace);

   copyStmtProps(header, stmt.get());
   return stmt;
}

std::shared_ptr<DeclStmt> ModuleReader::ReadDeclStmt()
{
   auto header = readStmtHeader();

   // access modifier
   auto access = (AccessModifier)readSize();

   // identifier
   auto ident = readString();

   // boolean flags
   bool isConst, isGlobal, structAlloca, protocolDecl, incRefCount;
   readBoolPack((bool*[]) {
      &isConst, &isGlobal, &structAlloca, &protocolDecl, &incRefCount
   }, 5);

   // type
   auto id = Reader.ReadByte();
   assert(id == TYPEREF_EXPR);
   auto ty = ReadTypeRef();

   // initializer
   auto isInitialized = (bool)Reader.ReadByte();

   Expression::SharedPtr val = nullptr;
   if (isInitialized) {
      val = ReadExpr();
   }

   auto decl = std::make_shared<DeclStmt>(
      ident,
      ty,
      isConst,
      isGlobal,
      val
   );

   decl->setAccess(access);
   decl->isStructAlloca(structAlloca);
   decl->isProtocolDecl(protocolDecl);
   decl->incRefCount(incRefCount);

   copyStmtProps(header, decl.get());
   return decl;
}

std::shared_ptr<FunctionDecl> ModuleReader::ReadFunctionDecl()
{
   auto header = readStmtHeader();

   auto funcName = readString();
   auto id = Reader.ReadByte();

   assert(id == TYPEREF_EXPR);
   auto retType = ReadTypeRef();

   std::vector<FuncArgDecl::SharedPtr> args;
   auto numArgs = readSize();
   for (size_t i = 0; i < numArgs; ++i) {
      auto id = Reader.ReadByte();
      assert(id == FUNC_ARG_DECL);
      args.push_back(ReadFuncArgDecl());
   }

   std::vector<GenericConstraint> generics;
   auto numGenerics = readSize();
   for (size_t i = 0; i < numGenerics; ++i) {
      generics.push_back(readGeneric());
   }

   bool hasStructRet, hasBody;
   readBoolPack((bool*[]) {
      &hasStructRet, &hasBody
   }, 2);

   CompoundStmt::SharedPtr body = nullptr;
   if (hasBody) {
      auto id = Reader.ReadByte();
      assert(id == CMPND_STMT);
      body = ReadCompoundStmt();
   }


   auto decl = std::make_shared<FunctionDecl>(
      std::move(funcName),
      std::move(retType),
      std::move(args),
      std::move(body),
      std::move(generics),
      hasStructRet
   );

   copyStmtProps(header, decl.get());
   return decl;
}

std::shared_ptr<DeclareStmt> ModuleReader::ReadDeclareStmt()
{
   auto header = readStmtHeader();

   std::vector<Statement::SharedPtr> decls;
   auto numDecls = readSize();

   for (size_t i = 0; i < numDecls; ++i) {
      decls.push_back(ReadNextStmt());
   }

   auto stmt = std::make_shared<DeclareStmt>(header.externKind);
   copyStmtProps(header, stmt.get());

   return stmt;
}


std::shared_ptr<UnionDecl> ModuleReader::ReadUnionDecl()
{
   return nullptr;
}

std::shared_ptr<ClassDecl> ModuleReader::ReadClassDecl()
{
   return nullptr;
}

std::shared_ptr<MethodDecl> ModuleReader::ReadMethodDecl()
{
   return nullptr;
}

std::shared_ptr<FieldDecl> ModuleReader::ReadFieldDecl()
{
   return nullptr;
}

std::shared_ptr<ConstrDecl> ModuleReader::ReadConstrDecl()
{
   return nullptr;
}

std::shared_ptr<DestrDecl> ModuleReader::ReadDestrDecl()
{
   return nullptr;
}

std::shared_ptr<EnumDecl> ModuleReader::ReadEnumDecl()
{
   return nullptr;
}

std::shared_ptr<EnumCaseDecl> ModuleReader::ReadEnumCaseDecl()
{
   return nullptr;
}

std::shared_ptr<IdentifierRefExpr> ModuleReader::ReadIdentifierRefExpr()
{
   return nullptr;
}

std::shared_ptr<SubscriptExpr> ModuleReader::ReadSubscriptExpr()
{
   return nullptr;
}

std::shared_ptr<CallExpr> ModuleReader::ReadCallExpr()
{
   return nullptr;
}

std::shared_ptr<MemberRefExpr> ModuleReader::ReadMemberRefExpr()
{
   return nullptr;
}

std::shared_ptr<ForStmt> ModuleReader::ReadForStmt()
{
   return nullptr;
}

std::shared_ptr<ForInStmt> ModuleReader::ReadForInStmt()
{
   return nullptr;
}

std::shared_ptr<WhileStmt> ModuleReader::ReadWhileStmt()
{
   return nullptr;
}

std::shared_ptr<IfStmt> ModuleReader::ReadIfStmt()
{
   return nullptr;
}

std::shared_ptr<MatchStmt> ModuleReader::ReadMatchStmt()
{
   return nullptr;
}

std::shared_ptr<CaseStmt> ModuleReader::ReadCaseStmt()
{
   return nullptr;
}

std::shared_ptr<LabelStmt> ModuleReader::ReadLabelStmt()
{
   return nullptr;
}

std::shared_ptr<GotoStmt> ModuleReader::ReadGotoStmt()
{
   return nullptr;
}

std::shared_ptr<ReturnStmt> ModuleReader::ReadReturnStmt()
{
   return nullptr;
}

std::shared_ptr<BreakStmt> ModuleReader::ReadBreakStmt()
{
   return nullptr;
}

std::shared_ptr<ContinueStmt> ModuleReader::ReadContinueStmt()
{
   return nullptr;
}

std::shared_ptr<CollectionLiteral> ModuleReader::ReadCollectionLiteral()
{
   return nullptr;
}

std::shared_ptr<NumericLiteral> ModuleReader::ReadNumericLiteral()
{
   auto header = readExprHeader();
   auto val = readVariant();
   auto ty = readType();
   auto className = readString();

   bool isChar, isBool, isPrimitive;
   readBoolPack((bool*[]) {
      &isChar, &isBool, &isPrimitive
   }, 3);

   auto lit = std::make_shared<NumericLiteral>(val);
   lit->setClassName(className);
   lit->setType(ty);
   lit->isChar(isChar);
   lit->isBool(isBool);
   lit->isPrimitive(isPrimitive);

   copyExprProps(header, lit.get());
   return lit;
}

std::shared_ptr<NoneLiteral> ModuleReader::ReadNoneLiteral()
{
   return nullptr;
}

std::shared_ptr<StringLiteral> ModuleReader::ReadStringLiteral()
{
   return nullptr;
}

std::shared_ptr<StringInterpolation> ModuleReader::ReadStringInterpolation()
{
   return nullptr;
}

std::shared_ptr<TupleLiteral> ModuleReader::ReadTupleLiteral()
{
   return nullptr;
}

std::shared_ptr<UnaryOperator> ModuleReader::ReadUnaryOperator()
{
   return nullptr;
}

std::shared_ptr<BinaryOperator> ModuleReader::ReadBinaryOperator()
{
   return nullptr;
}

std::shared_ptr<TertiaryOperator> ModuleReader::ReadTertiaryOperator()
{
   return nullptr;
}


std::shared_ptr<FuncArgDecl> ModuleReader::ReadFuncArgDecl()
{
   return nullptr;
}

std::shared_ptr<Expression> ModuleReader::ReadExpression()
{
   return nullptr;
}

std::shared_ptr<LambdaExpr> ModuleReader::ReadLambdaExpr()
{
   return nullptr;
}

std::shared_ptr<ImplicitCastExpr> ModuleReader::ReadImplicitCastExpr()
{
   return nullptr;
}

std::shared_ptr<TypedefDecl> ModuleReader::ReadTypedefDecl()
{
   return nullptr;
}

std::shared_ptr<TypeRef> ModuleReader::ReadTypeRef()
{
   auto header = readExprHeader();
   auto ty = readQualType();

   auto typeref = std::make_shared<TypeRef>(ty);
   copyExprProps(header, typeref.get());

   return typeref;
}

std::shared_ptr<LvalueToRvalue> ModuleReader::ReadLvalueToRvalue()
{
   return nullptr;
}

std::shared_ptr<EndOfFileStmt> ModuleReader::ReadEndOfFileStmt()
{
   return std::make_shared<EndOfFileStmt>();
}

std::shared_ptr<DebugStmt> ModuleReader::ReadDebugStmt()
{
   return nullptr;
}


std::shared_ptr<Statement> ModuleReader::ReadStatement()
{
   return nullptr;
}

std::shared_ptr<TryStmt> ModuleReader::ReadTryStmt()
{
   return nullptr;
}

std::shared_ptr<ThrowStmt> ModuleReader::ReadThrowStmt()
{
   return nullptr;
}

std::shared_ptr<Expression> ModuleReader::ReadExpr()
{
   auto expr = ReadNextStmt();
   assert(expr->isExpression());

   return std::static_pointer_cast<Expression>(expr);
}

std::shared_ptr<Statement> ModuleReader::ReadNextStmt()
{
   auto id = Reader.ReadByte();
   switch (id) {
      default:
         llvm_unreachable("unknown node type");
      case NS_DECL:
         return ReadNamespaceDecl();
      case USING_STMT:
         return ReadUsingStmt();
      case CMPND_STMT:
         return ReadCompoundStmt();
      case DECL_STMT:
         return ReadDeclStmt();
      case FUNC_DECL:
         return ReadFunctionDecl();
      case DECLARE_STMT:
         return ReadDeclareStmt();
      case CLASS_DECL:
         return ReadClassDecl();
      case METHOD_DECL:
         return ReadMethodDecl();
      case FIELD_DECL:
         return ReadFieldDecl();
      case DESTR_DECL:
         return ReadDestrDecl();
      case ENUM_DECL:
         return ReadEnumDecl();
      case ENUM_CASE_DECL:
         return ReadEnumCaseDecl();
      case UNION_DECL:
         return ReadUnionDecl();
      case IDENT_EXPR:
         return ReadIdentifierRefExpr();
      case SUBSCRIPT_EXPR:
         return ReadSubscriptExpr();
      case CALL_EXPR:
         return ReadCallExpr();
      case MEMBER_REF_EXPR:
         return ReadMemberRefExpr();
      case FOR_STMT:
         return ReadForStmt();
      case FOR_IN_STMT:
         return ReadForInStmt();
      case WHILE_STMT:
         return ReadWhileStmt();
      case IF_STMT:
         return ReadIfStmt();
      case MATCH_STMT:
         return ReadMatchStmt();
      case CASE_STMT:
         return ReadCaseStmt();
      case LABEL_STMT:
         return ReadLabelStmt();
      case GOTO_STMT:
         return ReadGotoStmt();
      case RETURN_STMT:
         return ReadReturnStmt();
      case BREAK_STMT:
         return ReadBreakStmt();
      case CONTINUE_STMT:
         return ReadContinueStmt();
      case COLLECTION_LIT:
         return ReadCollectionLiteral();
      case NUMERIC_LIT:
         return ReadNumericLiteral();
      case NON_LIT:
         return ReadNoneLiteral();
      case STRING_LIT:
         return ReadStringLiteral();
      case STRING_INTERPOLATION:
         return ReadStringInterpolation();
      case TUPLE_LIT:
         return ReadTupleLiteral();
      case UNARY_OP:
         return ReadUnionDecl();
      case BINARY_OP:
         return ReadBinaryOperator();
      case TERTIARY_OP:
         return ReadTertiaryOperator();
      case FUNC_ARG_DECL:
         return ReadFuncArgDecl();
      case LAMDBA_EXPR:
         return ReadLambdaExpr();
      case IMPLICIT_CAST_EXPR:
         return ReadImplicitCastExpr();
      case TYPEDEF_DECL:
         return ReadTypedefDecl();
      case TYPEREF_EXPR:
         return ReadTypeRef();
      case LVAL_TO_RVAL:
         return ReadLvalueToRvalue();
      case EOF_STMT:
         return ReadEndOfFileStmt();
      case DEBUG_STMT:
         return ReadDebugStmt();
      case TRY_STMT:
         return ReadTryStmt();
      case THROW_STMT:
         return ReadThrowStmt();
   }
}

}
}