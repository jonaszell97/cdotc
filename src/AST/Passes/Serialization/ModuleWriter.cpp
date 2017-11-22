////
//// Created by Jonas Zell on 17.10.17.
////
//
//#include "ModuleWriter.h"
//
//#include <llvm/Support/raw_ostream.h>
//#include <llvm/Support/FileSystem.h>
//
//#include "ModuleSerial.h"
//#include "../ASTIncludes.h"
//#include "../../../Variant/Type/FPType.h"
//#include "../../../Variant/Type/PointerType.h"
//#include "../../../Variant/Type/GenericType.h"
//#include "../../../Variant/Type/FunctionType.h"
//#include "../../../Variant/Type/TupleType.h"
//
//namespace cdot {
//namespace serial {
//
//ModuleWriter::ModuleWriter(
//   const string &moduleName,
//   std::shared_ptr<CompoundStmt> &moduleRoot) : moduleName(moduleName),
//                                                moduleRoot(moduleRoot)
//{
//
//}
//
//void ModuleWriter::write(
//   const string &outFilePath)
//{
//   writeModuleHeader();
//
//   writeSize(moduleRoot->getStatements().size());
//   for (const auto &stmt : moduleRoot->getStatements()) {
//      stmt->accept(this);
//   }
//
//   std::error_code ec;
//   llvm::raw_fd_ostream outStream(outFilePath + moduleName + ".cdotm", ec, llvm::sys::fs::F_RW);
//
//   Writer.Finalize(outStream);
//   outStream.flush();
//   outStream.close();
//}
//
//void ModuleWriter::writeModuleHeader()
//{
//   writeString(moduleName);
//}
//
//void ModuleWriter::writeString(const string &str)
//{
//   Writer.WriteULEB128(str.length());
//   for (const auto &c : str) {
//      Writer.WriteByte(c);
//   }
//}
//
//void ModuleWriter::writeSize(const size_t &size)
//{
//   Writer.WriteULEB128(size);
//}
//
//void ModuleWriter::beginPack()
//{
//   Packs.push(0);
//   WrittenBitStack.push(0);
//}
//
//void ModuleWriter::endPack()
//{
//   Writer.WriteByte(Packs.top());
//   Packs.pop();
//   WrittenBitStack.pop();
//}
//
//void ModuleWriter::writeBoolPack(bool *b, unsigned n)
//{
//   unsigned char byte = 0;
//   for (int i = 0; i < n; ++i, ++b) {
//      byte |= *b << i;
//      if (i != 0 && i % 8 == 0) {
//         Writer.WriteByte(byte);
//         byte = 0;
//      }
//   }
//
//   Writer.WriteByte(byte);
//}
//
//void ModuleWriter::writeBool(bool b)
//{
//   if (!Packs.empty()) {
//      Packs.top() |= b << WrittenBitStack.top()++;
//   }
//   else {
//      Writer.WriteByte((unsigned char)b);
//   }
//}
//
//void ModuleWriter::writeQualType(const QualType &ty)
//{
//   // qualifiers
//   writeBoolPack((bool[]) {
//      ty.isLvalue(), ty.isConst(), ty.isSelf()
//   }, 3);
//
//   writeType(ty.getBuiltinTy());
//}
//
//void ModuleWriter::writeType(Type *ty)
//{
//   writeSize((unsigned)ty->getTypeID());
//
//   switch (ty->getTypeID()) {
//      case TypeID::IntegerTypeID: {
//         writeSize(ty->getBitwidth());
//         writeBool(ty->isUnsigned());
//         break;
//      }
//      case TypeID::FPTypeID: {
//         writeBool(ty->isFloatTy());
//         break;
//      }
//      case TypeID::PointerTypeID: {
//         writeQualType(ty->asPointerTy()->getPointeeType());
//         break;
//      }
//      case TypeID::ObjectTypeID: {
//         writeString(ty->getClassName());
//
//         auto &generics = ty->getTemplateArgs();
//         writeSize(generics.size());
//
//         for (auto &gen : generics) {
//            write(gen.toString());
//         }
//
//         break;
//      }
//      case TypeID::GenericTypeID: {
//         auto gen = ty->asGenericTy();
//
//         writeString(gen->getClassName());
//         writeType(gen->getActualType());
//
//         break;
//      }
//      case TypeID::TupleTypeID: {
//         auto &contained = ty->asTupleTy()->getContainedTypes();
//         writeSize(contained.size());
//
//         for (auto &cont : contained) {
//            writeString(cont.first);
//            writeType(cont.second);
//         }
//
//         break;
//      }
//      case TypeID::FunctionTypeID: {
//         auto func = ty->asFunctionTy();
//         writeBool(func->isRawFunctionTy());
//
//         writeQualType(func->getReturnType());
//
//         auto &args = func->getArgTypes();
//         writeSize(args.size());
//
//         for (const auto &arg : args) {
//            writeArg(arg);
//         }
//
//         break;
//      }
//      default:
//         break;
//   }
//}
//
//void ModuleWriter::writeArg(const Argument &arg)
//{
//   writeString(arg.label);
//   writeQualType(arg.type);
//
//   auto &def = arg.defaultVal;
//   auto hasDef = def != nullptr;
//
//   writeBool(hasDef);
//   if (hasDef) {
//      def->accept(this);
//   }
//
//   writeBoolPack((bool[]) {
//      arg.isVararg, arg.cstyleVararg, arg.ref
//   }, 3);
//}
//
//void ModuleWriter::writeAttribute(const Attribute &attr)
//{
//   writeSize((unsigned)attr.kind);
//   writeString(attr.name);
//
//   writeSize(attr.args.size());
//   for (const auto &arg : attr.args) {
//      writeString(arg.toString());
//   }
//}
//
//void ModuleWriter::writeVariant(const Variant &var)
//{
//   writeSize((unsigned)var.type);
//   writeSize(var.bitwidth);
//
//   writeBoolPack((bool[]) {
//      var.is_unsigned, var.boxed
//   }, 3);
//
//   if (var.type != VariantType::STRING) {
//      writeSize(var.intVal);
//   }
//   else {
//      writeString(var.strVal);
//   }
//}
//
//void ModuleWriter::writeGeneric(const TemplateConstraint &gen)
//{
//   writeString(gen.genericTypeName);
//   if (gen.kind == TemplateConstraint::TypeName && gen.covariance) {
//      writeString("-" + gen.covariance->toString());
//   }
//   if (gen.kind == TemplateConstraint::Value && gen.valueType) {
//      writeString(gen.valueType->toString());
//   }
//   if (gen.contravariance) {
//      writeString("+" + gen.contravariance->toString());
//   }
//}
//
//void ModuleWriter::visit(AstNode *node)
//{
//   auto &attrs = node->getAttributes();
//   writeSize(attrs.size());
//
//   for (const auto& attr : attrs) {
//      writeAttribute(attr);
//   }
//
//   writeBoolPack((bool[]) {
//      node->isReturnedValue(), node->isStructRetValue()
//   }, 2);
//
//   writeString(node->getBinding());
//}
//
//void ModuleWriter::visit(Statement *node)
//{
//   visit((AstNode*)node);
//   writeSize((unsigned)node->getExternKind());
//   writeBool(node->isDeclaration());
//}
//
//void ModuleWriter::visit(Expression *node)
//{
//   visit((Statement*)node);
//   writeSize(node->getUses());
//
//   writeBool(node->needsCast());
//   if (node->needsCast()) {
//      writeQualType(node->getCastFrom());
//      writeQualType(node->getCastTo());
//   }
//
//   writeBoolPack((bool[]) {
//      node->isLvalueCast(), node->needsByValPass(), node->getIsFunctionArgument(),
//      node->getIsLhsOfAssigment(), node->getIsPartOfReturnValue(), node->isSetterCall(),
//      node->isAssigned(), node->isEnumCase(), node->isTemporary()
//   }, 9);
//
////   if (node->isSetterCall()) {
////      writeString(node->getAccessorName());
////   }
//
//   if (node->isEnumCase()) {
//      writeSize(node->getCaseVal());
//   }
//}
//
//void ModuleWriter::visit(NamespaceDecl *node)
//{
//   Writer.WriteByte(NS_DECL);
//   visit((Statement*)node);
//
//   Writer.WriteByte((unsigned char)node->isAnonymousNamespace());
//   if (!node->isAnonymousNamespace()) {
//      writeString(node->getNsName());
//   }
//
//   node->getContents()->accept(this);
//}
//
//void ModuleWriter::visit(UsingStmt *node)
//{
//   Writer.WriteByte(USING_STMT);
//   visit((Statement*)node);
//
//   Writer.WriteWord((unsigned)node->getKind());
//
//   writeString(node->getImportNamespace());
//   Writer.WriteByte(node->isIsWildcardImport());
//
//   if (!node->isIsWildcardImport()) {
//      writeSize(node->getFullNames().size());
//      for (const auto &item : node->getFullNames()) {
//         writeString(item);
//      }
//   }
//}
//
//void ModuleWriter::visit(CompoundStmt *node)
//{
//   Writer.WriteByte(CMPND_STMT);
//   visit((Statement*)node);
//
//   Writer.WriteByte(node->preservesScope());
//   Writer.WriteByte(node->isUnsafe());
//
//   writeSize(node->getStatements().size());
//   for (const auto& stmt : node->getStatements()) {
//      stmt->accept(this);
//   }
//}
//
//void ModuleWriter::visit(DeclStmt *node)
//{
//   Writer.WriteByte(DECL_STMT);
//   visit((Statement*)node);
//
//   // access modifier
//   writeSize((unsigned)node->getAccess());
//
//   // identifier
//   writeString(node->getIdentifier());
//
//   // flags
//   writeBoolPack((bool[]) {
//      node->isConst(), node->isGlobal(), node->isStructAlloca(),
//      node->isProtocolDecl(), node->incRefCount()
//   }, 5);
//
//   // type
//   node->getType()->accept(this);
//
//   // initializer
//   bool isInitialized = node->getValue() != nullptr;
//   writeBool(isInitialized);
//   if (isInitialized) {
//      node->getValue()->accept(this);
//   }
//
//}
//
//void ModuleWriter::visit(FunctionDecl *node)
//{
//   Writer.WriteByte(FUNC_DECL);
//   visit((Statement*)node);
//
//   writeString(node->getName());
//   node->getReturnType()->accept(this);
//
//   auto &args = node->getArgs();
//   writeSize(args.size());
//
//   for (const auto &arg : args) {
//      arg->accept(this);
//   }
//
//   auto hasBody = node->getBody() != nullptr;
//   writeBoolPack((bool[]) {
//      node->hasStructRet(), hasBody
//   }, 2);
//
//   if (hasBody) {
//      node->getBody()->accept(this);
//   }
//}
//
//void ModuleWriter::visit(DeclareStmt *node)
//{
//   Writer.WriteByte(DECLARE_STMT);
//   visit((Statement*)node);
//
//   auto &decls = node->getDeclarations();
//   writeSize(decls.size());
//
//   for (const auto &decl : decls) {
//      decl->accept(this);
//   }
//}
//
//void ModuleWriter::visit(ClassDecl *node) {}
//void ModuleWriter::visit(MethodDecl *node) {}
//void ModuleWriter::visit(FieldDecl *node) {}
//void ModuleWriter::visit(ConstrDecl *node) {}
//void ModuleWriter::visit(DestrDecl *node) {}
//void ModuleWriter::visit(EnumDecl *node) {}
//void ModuleWriter::visit(EnumCaseDecl *node) {}
//void ModuleWriter::visit(UnionDecl *node) {}
//
//void ModuleWriter::visit(IdentifierRefExpr *node) {}
//void ModuleWriter::visit(SubscriptExpr *node) {}
//void ModuleWriter::visit(CallExpr *node) {}
//void ModuleWriter::visit(MemberRefExpr *node) {}
//
//void ModuleWriter::visit(ForStmt *node) {}
//void ModuleWriter::visit(ForInStmt *node) {}
//void ModuleWriter::visit(WhileStmt *node) {}
//void ModuleWriter::visit(IfStmt *node) {}
//void ModuleWriter::visit(MatchStmt *node) {}
//void ModuleWriter::visit(CaseStmt *node) {}
//void ModuleWriter::visit(LabelStmt *node) {}
//void ModuleWriter::visit(GotoStmt *node) {}
//
//void ModuleWriter::visit(ReturnStmt *node) {}
//void ModuleWriter::visit(BreakStmt *node) {}
//void ModuleWriter::visit(ContinueStmt *node) {}
//
//void ModuleWriter::visit(CollectionLiteral *node) {}
//
//void ModuleWriter::visit(IntegerLiteral *node)
//{
//   Writer.WriteByte(NUMERIC_LIT);
//   visit((Expression*)node);
//
//   writeVariant(node->getValue());
//   writeType(node->getType());
//}
//
//void ModuleWriter::visit(NoneLiteral *node)
//{
//   Writer.WriteByte(NON_LIT);
//   visit((Expression*)node);
//}
//
//void ModuleWriter::visit(StringLiteral *node)
//{
//   Writer.WriteByte(STRING_LIT);
//   visit((Expression*)node);
//
//   writeBool(node->isRaw());
//   writeString(node->getValue());
//}
//
//void ModuleWriter::visit(StringInterpolation *node)
//{
//
//}
//void ModuleWriter::visit(TupleLiteral *node) {}
//
//void ModuleWriter::visit(UnaryOperator *node) {}
//void ModuleWriter::visit(BinaryOperator *node) {}
//void ModuleWriter::visit(TertiaryOperator *node) {}
//
//void ModuleWriter::visit(FuncArgDecl *node) {}
//void ModuleWriter::visit(LambdaExpr *node) {}
//void ModuleWriter::visit(ImplicitCastExpr *node) {}
//void ModuleWriter::visit(TypedefDecl *node) {}
//
//void ModuleWriter::visit(TypeRef *node)
//{
//   Writer.WriteByte(TYPEREF_EXPR);
//   visit((Expression*)node);
//
//   writeQualType(node->getType());
//}
//
//void ModuleWriter::visit(LvalueToRvalue *node) {}
//
//void ModuleWriter::visit(EndOfFileStmt *node)
//{
//   Writer.WriteByte(EOF_STMT);
//}
//
//void ModuleWriter::visit(DebugStmt *node) {}
//
//void ModuleWriter::visit(TryStmt *node) {}
//void ModuleWriter::visit(ThrowStmt *node) {}
//
//}
//}