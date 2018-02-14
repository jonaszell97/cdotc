////
//// Created by Jonas Zell on 12.01.18.
////
//
//#include "Serialization.h"
//#include "Module.h"
//
//#include "../AST/Passes/Serialization/Serialize.h"
//#include "../AST/Passes/Declaration/DeclPass.h"
//#include "../AST/Passes/SemanticAnalysis/SemaPass.h"
//
//#include "../AST/Statement/Block/CompoundStmt.h"
//#include "../AST/Statement/Declaration/CallableDecl.h"
//#include "../AST/Statement/Declaration/Class/RecordDecl.h"
//#include "../AST/Statement/Declaration/Class/MethodDecl.h"
//#include "../AST/Statement/Declaration/Class/PropDecl.h"
//#include "../AST/Statement/Declaration/Class/FieldDecl.h"
//#include "../AST/Statement/Declaration/Class/EnumCaseDecl.h"
//#include "../AST/Statement/Declaration/TypedefDecl.h"
//#include "../AST/Statement/Declaration/NamespaceDecl.h"
//#include "../AST/Statement/Declaration/LocalVarDecl.h"
//
//#include "../AST/Expression/StaticExpr.h"
//#include "../AST/Expression/TypeRef.h"
//
//#include "../AST/ASTContext.h"
//#include "../AST/SymbolTable.h"
//
//#include "../AST/Passes/ILGen/ILGenPass.h"
//
//#include "../Support/Format.h"
//#include "../Support/SerializerBase.h"
//#include "../Files/FileManager.h"
//
//#include "../Variant/Type/QualType.h"
//#include "../Variant/Type/ArrayType.h"
//#include "../Variant/Type/TupleType.h"
//#include "../Variant/Type/FunctionType.h"
//#include "../Variant/Type/GenericType.h"
//#include "../Variant/Type/FPType.h"
//#include "../Variant/Type/IntegerType.h"
//#include "../Variant/Type/PointerType.h"
//#include "../Variant/Type/AutoType.h"
//#include "../Variant/Type/VoidType.h"
//#include "../Variant/Type/MetaType.h"
//
//using namespace cdot::support;
//using namespace cdot::serial;
//
//namespace cdot {
//namespace module {
//namespace {
//
//LLVM_ATTRIBUTE_UNUSED
//void printHexBuffer(const char* data, size_t size = 10)
//{
//   for (size_t i = 0; i < size; ++i) {
//      llvm::outs() << formatAsHexInteger(*data++) << " ";
//   }
//
//   llvm::outs() << "\n";
//}
//
//ptrdiff_t deserializeASTImpl(SemaPass &SP,
//                             Module &M,
//                             llvm::MemoryBuffer &Buf,
//                             size_t sourceId,
//                             SourceLocation importedFrom) {
//   serial::ASTDeserializer deserializer(SP, Buf, &M, importedFrom);
//   M.setDeclRoot(deserializer.ReadSpecific<CompoundStmt>());
//
//   return 0;
//}
//
//static const unsigned char StopByte = SymbolTableEntry::AliasID + 1;
//
//class ModuleInterfaceSerializer:
//   public SerializerBase<ModuleInterfaceSerializer> {
//public:
//   explicit ModuleInterfaceSerializer(Module *M, SymbolTable const &SymTab)
//      : M(M), SymTab(SymTab)
//   {
//      doSerialize();
//   }
//
//private:
//   Module *M;
//   SymbolTable const &SymTab;
//   serial::ASTSerializer serializer;
//
//   void doSerialize()
//   {
//      llvm::SmallVector<NamedDecl*, 8> ForwardDecls;
//      auto pos = Writer.reserve(8);
//      auto beginSize = Writer.size();
//
//      for (auto &Entry : SymTab.getEntries()) {
//         if (Entry.second.isRecord()) {
//            auto R = Entry.second.getRecord();
//            if (!R->isExternal() && !R->getOuterRecord())
//               ForwardDecls.push_back(R);
//         }
//         else if (Entry.second.isTypedef()) {
//            auto td = Entry.second.getTypedef();
//            if (!td->isExternal())
//               ForwardDecls.push_back(td);
//         }
//      }
//
//      WriteSize(ForwardDecls);
//      for (auto &D : ForwardDecls)
//         WriteForwardDecl(D);
//
//      *reinterpret_cast<size_t*>(Writer.data() + pos)
//         = Writer.size() - beginSize;
//
//      for (auto &EntryPair : SymTab.getEntries()) {
//         auto &Entry = EntryPair.second;
//
//         switch (Entry.getKind()) {
//            case SymbolTableEntry::RecordID: {
//               auto R = Entry.getRecord();
//               if (R->isExternal() || R->getOuterRecord())
//                  continue;
//
//               WriteByte(Entry.getKind());
//               WriteRecord(*R);
//
//               break;
//            }
//            case SymbolTableEntry::AliasID: {
////               auto aliases = Entry.getAlias()->getOverloads();
////
////               size_t i = 0;
////               for (auto &alias : aliases) {
////                  if (alias->isExternal())
////                     continue;
////
////                  WriteByte(Entry.getKind());
////                  WriteAlias(alias, i++);
////               }
////
//               break;
//            }
//            case SymbolTableEntry::FunctionID: {
////               size_t i = 0;
////               for (auto &fn : Entry.getFunction()->getOverloads()) {
////                  if (fn->isExternal())
////                     continue;
////
////                  WriteByte(Entry.getKind());
////                  WriteFunction(*cast<FunctionDecl>(fn), i++);
////               }
//
//               break;
//            }
//            case SymbolTableEntry::TypedefID: {
//               auto td = Entry.getTypedef();
//               if (td->isExternal())
//                  continue;
//
//               WriteByte(Entry.getKind());
//               WriteTypedef(*td, false);
//
//               break;
//            }
//            default:
//               break;
//         }
//      }
//
//      Writer.WriteByte(StopByte);
//   }
//
//   void WriteForwardDecl(NamedDecl *decl)
//   {
//      WriteByte(decl->getTypeID());
//
//      if (auto R = dyn_cast<RecordDecl>(decl)) {
//         WriteByte(R->getAccess());
//
//         WriteString(R->getName());
//         WriteList(R->getTemplateParams(),
//                   &ModuleInterfaceSerializer::WriteTemplateParameter);
//
//         if (auto C = dyn_cast<ClassDecl>(R))
//            WriteBool(C->isAbstract());
//
//         WriteSize(R->getInnerRecords());
//         for (auto I : R->getInnerRecords())
//            WriteForwardDecl(I);
//      }
//      else if (auto td = dyn_cast<TypedefDecl>(decl)) {
//         WriteByte(td->getAccess());
//         WriteString(td->getName());
//         WriteList(td->getTemplateParams(),
//                   &ModuleInterfaceSerializer::WriteTemplateParameter);
//      }
//      else {
//         llvm_unreachable("bad forward decl kind!");
//      }
//   }
//
//   void serializeStmt(Statement *Stmt)
//   {
//      llvm::SmallString<256> str;
//      llvm::raw_svector_ostream outs(str);
//
//      serializer.serialize(Stmt, outs);
//      WriteStringWithoutSize(str.str());
//   }
//
//   void WriteQualType(QualType Ty)
//   {
//      WriteBools(Ty.isLvalue(), Ty.isConst());
//      WriteType(*Ty);
//   }
//
//   void WriteType(Type *Ty)
//   {
//      Writer.WriteByte((char) Ty->getTypeID());
//      switch (Ty->getTypeID()) {
//         case TypeID::IntegerTypeID: {
//            auto bw = Ty->getBitwidth();
//            bw |= Ty->isUnsigned() << 10;
//
//            Writer.WriteULEB128(bw);
//            break;
//         }
//         case TypeID::FPTypeID:WriteBool(Ty->isDoubleTy());
//            break;
//         case TypeID::PointerTypeID:WriteQualType(Ty->getPointeeType());
//            break;
//         case TypeID::ArrayTypeID: {
//            auto ArrTy = Ty->asArrayType();
//            Writer.WriteULEB128(ArrTy->getNumElements());
//            WriteQualType(ArrTy->getElementType());
//
//            break;
//         }
//         case TypeID::InferredArrayTypeID: {
//            auto arr = dyn_cast<InferredArrayType>(Ty);
//            WriteString(arr->getParam());
//            WriteQualType(arr->getElementType());
//
//            break;
//         }
//         case TypeID::TupleTypeID: {
//            auto Tup = Ty->asTupleType();
//            WriteList(Tup->getContainedTypes(),
//                      &ModuleInterfaceSerializer::WriteStringTypePair);
//
//            break;
//         }
//         case TypeID::FunctionTypeID: {
//            auto Fun = Ty->asFunctionType();
//            WriteQualType(Fun->getReturnType());
//            WriteList(Fun->getArgTypes(),
//                      &ModuleInterfaceSerializer::WriteArgument);
//            WriteBool(Fun->isRawFunctionTy());
//
//            break;
//
//         }
//         case TypeID::ObjectTypeID: {
//            WriteString(Ty->getClassName());
//            break;
//         }
//         case TypeID::InconcreteObjectTypeID: {
//            WriteString(Ty->getClassName());
//            WriteTemplateArgList(cast<InconcreteObjectType>(Ty)
//                                    ->getTemplateArgs());
//
//            break;
//         }
//         case TypeID::GenericTypeID: {
//            auto Gen = Ty->asGenericType();
//            WriteString(Gen->getGenericTypeName());
//            WriteQualType(Gen->getActualType());
//
//            break;
//         }
//         case TypeID::TypedefTypeID: {
//            auto td = Ty->asRealTypedefType();
//            WriteString(td->getTypedef()->getName());
//            WriteQualType(td->getAliasedType());
//
//            break;
//         }
//         case TypeID::MetaTypeID:
//            WriteType(cast<MetaType>(Ty)->getUnderlyingType());
//            break;
//         case TypeID::NamespaceTypeID:
//            WriteString(cast<NamespaceType>(Ty)->getNamespace()->getName());
//            break;
//         case TypeID::AutoTypeID:
//         case TypeID::VoidTypeID:
//            break;
//         default:
//            llvm_unreachable("bad type!");
//      }
//   }
//
//   void WriteStringTypePair(std::pair<std::string, QualType> const &P)
//   {
//      WriteString(P.first);
//      WriteQualType(P.second);
//   }
//
//   void WriteSourceLoc(SourceLocation loc)
//   {
//      Writer.WriteULEB128(loc.getOffset());
//      Writer.WriteULEB128(loc.getSourceId());
//   }
//
//   void WriteArgument(Argument const &A)
//   {
//      WriteBools(A.isCstyleVararg(), A.isVariadic());
//      if (A.isCstyleVararg())
//         return;
//
//      WriteString(A.getLabel());
//      WriteQualType(A.getType());
//   }
//
//   void WriteTemplateParameter(TemplateParameter const &Param)
//   {
//      WriteString(Param.getGenericTypeName());
//      WriteBools(Param.isTypeName(), Param.isVariadic);
//
//      if (auto cov = Param.covariance) {
//         WriteBool(true);
//         WriteType(cov);
//      }
//      else {
//         WriteBool(false);
//      }
//
//      if (auto con = Param.contravariance) {
//         WriteBool(true);
//         WriteType(con);
//      }
//      else {
//         WriteBool(false);
//      }
//   }
//
//   void WriteTemplateArgList(sema::TemplateArgList const &list)
//   {
//      WriteSize(list);
//      for (auto it = list.begin(); it != list.end(); ++it) {
//         WriteString(it.getParam().getGenericTypeName());
//         WriteResolvedTemplateArg(*it);
//      }
//   }
//
//   void WriteResolvedTemplateArg(sema::ResolvedTemplateArg const& arg)
//   {
//      WriteBools(arg.isType(), arg.isVariadic());
//
//      if (arg.isVariadic()) {
//         WriteList(arg.getVariadicArgs(),
//                   &ModuleInterfaceSerializer::WriteResolvedTemplateArg);
//      }
//      else if (arg.isValue()) {
//         WriteVariant(arg.getValue());
//      }
//      else if (arg.isType()) {
//         WriteType(arg.getType());
//      }
//   }
//
//   void WriteNamespace(NamespaceDecl const *NS)
//   {
//      WriteString(NS->getName());
//      WriteBools(NS->isAnonymousNamespace(),
//                 NS->getOuterNamespace() != nullptr);
//
//      if (auto parent = NS->getOuterNamespace())
//         WriteString(parent->getName());
//   }
//
//   void WriteVariable(Variable const *V)
//   {
//      WriteString(V->getName());
//      WriteByte(V->getAccess());
//      WriteQualType(V->getType());
//
//      WriteBools(V->isIsTypeDependant(), V->isIsGlobal(),
//                 V->getDeclarationNamespace() != nullptr);
//
//      if (auto ns = V->getDeclarationNamespace())
//         WriteString(ns->getName());
//   }
//
//   void WriteArgDecl(FuncArgDecl *Arg)
//   {
//      WriteString(Arg->getArgName());
//      WriteQualType(Arg->getArgType()->getType());
//      WriteBools(Arg->isConst(), Arg->isVararg(), Arg->isCstyleVararg(),
//                 Arg->isVariadicArgPackExpansion());
//   }
//
//   void WriteFunctionPtr(FunctionDecl *F, size_t idx)
//   {
//      WriteFunction(*F, idx);
//   }
//
//   void WriteFunction(FunctionDecl& F, size_t overloadIdx)
//   {
//      Writer.WriteULEB128(overloadIdx);
//      WriteString(F.getName());
//      WriteString(F.getLinkageName());
//
//      WriteQualType(F.getReturnType()->getType());
//      WriteByte(F.getAccess());
//
//      WriteList(F.getArgs(),
//                &ModuleInterfaceSerializer::WriteArgDecl);
//
//      WriteList(F.getTemplateParams(),
//                &ModuleInterfaceSerializer::WriteTemplateParameter);
//
//      WriteSize(F.getConstraints());
//      for (auto &C : F.getConstraints())
//         serializeStmt(C);
//
//      WriteOperatorInfo(F.getOperator());
//      WriteBools(F.isExternC(), F.isNoThrow(), F.isNative(),
//                 F.isConversionOp(), F.getClosestNamespace() != nullptr);
//
//      WriteSourceLoc(F.getSourceLoc());
//
//      if (auto ns = F.getClosestNamespace())
//         WriteString(ns->getName());
//
//      if (F.isTemplate() && F.getBody()) {
//         WriteBool(true);
//         serializeStmt(F.getBody());
//      }
//      else {
//         WriteBool(false);
//      }
//   }
//
//   void WriteTypedef(TypedefDecl const &TD, bool writeFull)
//   {
//      if (writeFull) {
//         WriteByte(TD.getAccess());
//         WriteString(TD.getName());
//         WriteQualType(TD.getOriginTy()->getType());
//         WriteList(TD.getTemplateParams(),
//                   &ModuleInterfaceSerializer::WriteTemplateParameter);
//      }
//      else {
//         WriteString(TD.getName());
//         WriteQualType(TD.getOriginTy()->getType());
//      }
//   }
//
//   void WriteAlias(AliasDecl* alias, size_t idx)
//   {
//      WriteString(alias->getName());
//      Writer.WriteULEB128(idx);
//
//      WriteSize(alias->getConstraints());
//      for (auto &C : alias->getConstraints())
//         serializeStmt(C);
//
//      if (!alias->isTypeDependant()) {
//         WriteBool(true);
//         WriteVariant(alias->getVal());
//      }
//      else {
//         WriteBool(false);
//         serializeStmt(alias->getAliasExpr());
//      }
//   }
//
//   void WriteRecord(RecordDecl &R)
//   {
//      WriteString(R.getName());
//
//      WriteSize(R.getNamedDecls());
//      for (auto &decl : R.getNamedDecls()) {
//         Writer.WriteULEB128(decl.getValue()->getTypeID());
//
//         if (auto Init = dyn_cast<InitDecl>(decl.getValue())) {
//            WriteConstr(*Init);
//         }
//         else if (auto Deinit = dyn_cast<DeinitDecl>(decl.getValue())) {
//            WriteDestr(*Deinit);
//         }
//         else if (auto M = dyn_cast<MethodDecl>(decl.getValue())) {
//            WriteMethodOverloads(M);
//         }
//         else if (auto F = dyn_cast<FieldDecl>(decl.getValue())) {
//            WriteField(*F);
//         }
//         else if (auto P = dyn_cast<PropDecl>(decl.getValue())) {
//            WriteProp(*P);
//         }
//         else if (auto AT = dyn_cast<AssociatedTypeDecl>(decl.getValue())) {
//            WriteAssociatedType(*AT, R);
//         }
//         else if (auto C = dyn_cast<EnumCaseDecl>(decl.getValue())) {
//            WriteCase(*C);
//         }
//         else if (auto Inner = dyn_cast<RecordDecl>(decl.getValue())) {
//            WriteRecord(*Inner);
//         }
//         else {
//            llvm_unreachable("bad decl kind");
//         }
//      }
//
//      WriteSize(R.getConstraints());
//      for (auto &C : R.getConstraints())
//         serializeStmt(C);
//
//      if (auto C = dyn_cast<ClassDecl>(&R)) {
//         if (auto P = C->getParentClass()) {
//            WriteBool(true);
//            WriteString(P->getName());
//         }
//         else {
//            WriteBool(false);
//         }
//      }
//      else if (auto E = dyn_cast<EnumDecl>(&R)) {
//         WriteType(*E->getRawType()->getType());
//      }
//   }
//
//   void WriteField(FieldDecl const& F)
//   {
//      WriteString(F.getName());
//      WriteString(F.getLinkageName());
//      WriteQualType(F.getType()->getType());
//      WriteByte(F.getAccess());
//
//      WriteBools(F.isConst(), F.isStatic(), F.hasGetter(), F.hasSetter());
//
//      if (F.hasGetter())
//         WriteString(F.getGetterMethod()->getLinkageName());
//
//      if (F.hasSetter())
//         WriteString(F.getSetterMethod()->getLinkageName());
//   }
//
//   void WriteConstr(InitDecl const& C)
//   {
//      WriteString(C.getName());
//      WriteString(C.getLinkageName());
//
//      WriteByte(C.getAccess());
//
//      WriteList(C.getArgs(), &ModuleInterfaceSerializer::WriteArgDecl);
//
//      llvm::ArrayRef<TemplateParameter> params{
//         C.getTemplateParams().data(),
//         C.getTemplateParams().size()
//      };
//
//      WriteList(params, &ModuleInterfaceSerializer::WriteTemplateParameter);
//
//      WriteSize(C.getConstraints());
//      for (auto &C : C.getConstraints())
//         serializeStmt(C);
//
//      WriteBool(C.isMemberwiseInitializer());
//
//      if ((C.isTemplate() || C.getRecord()->isTemplate()) && C.getBody()){
//         WriteBool(true);
//         serializeStmt(C.getBody());
//      }
//      else {
//         WriteBool(false);
//      }
//   }
//
//   void WriteDestr(DeinitDecl const &D)
//   {
//      WriteString(D.getLinkageName());
//
//      if ((D.isTemplate() || D.getRecord()->isTemplate()) && D.getBody()){
//         WriteBool(true);
//         serializeStmt(D.getBody());
//      }
//      else {
//         WriteBool(false);
//      }
//   }
//
//   void WriteMethodOverloads(MethodDecl const* M)
//   {
//      WriteSize(M->getOverloads());
//      for (auto &ovl : M->getOverloads())
//         WriteMethod(*cast<MethodDecl>(ovl));
//   }
//
//   void WriteMethod(MethodDecl const& M)
//   {
//      WriteString(M.getName());
//      WriteString(M.getLinkageName());
//
//      WriteQualType(M.getReturnType()->getType());
//      WriteByte(M.getAccess());
//
//      WriteList(M.getArgs(), &ModuleInterfaceSerializer::WriteArgDecl);
//      WriteList(M.getTemplateParams(),
//                &ModuleInterfaceSerializer::WriteTemplateParameter);
//
//      WriteSize(M.getConstraints());
//      for (auto &C : M.getConstraints())
//         serializeStmt(C);
//
//      WriteOperatorInfo(M.getOperator());
//
//      WriteBools(M.isStatic(), isa<InitDecl>(&M), M.isMemberwiseInitializer(),
//                 M.isProperty(), M.isProtocolDefaultImpl(),
//                 M.isProtocolMethod(), M.isNoThrow(), M.isConversionOp());
//
//      if ((M.isTemplate() || M.getRecord()->isTemplate()) && M.getBody()){
//         WriteBool(true);
//
//         auto begin = Writer.size();
//         serializeStmt(M.getBody());
//         llvm::outs() << M.getLinkageName() << ": " << (Writer.size() - begin)
//                                                    << " bytes\n";
//      }
//      else {
//         WriteBool(false);
//      }
//   }
//
//   void WriteProp(PropDecl const& P)
//   {
//      WriteByte(P.getAccess());
//      WriteString(P.getName());
//      WriteQualType(P.getType()->getType());
//
//      WriteBools(P.isStatic(), P.hasGetter(), P.hasSetter());
//
//      if (P.hasGetter())
//         WriteString(P.getGetterMethod()->getLinkageName());
//
//      if (P.hasSetter())
//         WriteStringWithoutSize(P.getSetterMethod()->getLinkageName());
//   }
//
//   void WriteAssociatedType(AssociatedTypeDecl const &A, RecordDecl const& R)
//   {
//      WriteString(A.getProtocolSpecifier());
//      WriteString(A.getName());
//
//      if (auto ty = A.getActualType()) {
//         WriteBool(true);
//         WriteQualType(A.getActualType()->getType());
//      }
//      else {
//         WriteBool(false);
//      }
//
//      WriteList(A.getConstraints(), &ModuleInterfaceSerializer::serializeStmt);
//   }
//
//   void WriteCase(EnumCaseDecl const& C)
//   {
//      WriteString(C.getName());
//      Writer.WriteSLEB128(C.getRawValue());
//      WriteList(C.getArgs(),
//                &ModuleInterfaceSerializer::WriteArgDecl);
//   }
//
//   void WritePrecedenceGroup(PrecedenceGroup const& PG)
//   {
//      Writer.WriteSLEB128(PG.getPrecedence());
//      WriteByte(PG.getAssociativity());
//   }
//
//   void WriteOperatorInfo(OperatorInfo const& Op)
//   {
//      WritePrecedenceGroup(Op.getPrecedenceGroup());
//      WriteByte(Op.getFix());
//      WriteBool(Op.isImplicit());
//   }
//
//   void WriteVariant(Variant const& V)
//   {
//      WriteType(V.typeOf());
//      WriteByte(V.getKind());
//
//      switch (V.getKind()) {
//         case VariantType::INT:
//            Writer.WriteULEB128(V.getInt());
//            break;
//         case VariantType::FLOAT: {
//            union {
//               double d;
//               size_t s;
//            } u;
//
//            u.d = V.getDouble();
//            Writer.WriteULEB128(u.s);
//            break;
//         }
//         case VariantType::STRING: {
//            WriteString(V.getString());
//            break;
//         }
//         case VariantType::ARRAY: {
//            WriteList(V.getVec(), &ModuleInterfaceSerializer::WriteVariant);
//            break;
//         }
//         case VariantType::STRUCT: {
//            WriteSize(V.getFields());
//            for (auto &F : V.getFields()) {
//               WriteString(F.first);
//               WriteVariant(F.second);
//            }
//
//            break;
//         }
//         case VariantType::MetaType: {
//            WriteQualType(V.getType());
//            break;
//         }
//         case VariantType::VOID:
//            break;
//         default:
//            llvm_unreachable("can't serialize Variant!");
//      }
//   }
//};
//
//static CompoundStmt root; //FIXME
//
//class ModuleInterfaceDeserializer:
//   public DeserializerBase<ModuleInterfaceDeserializer> {
//public:
//   ModuleInterfaceDeserializer(SemaPass &SP,
//                               llvm::MemoryBuffer &Buf,
//                               Module *SrcMod,
//                               SourceLocation importedFrom)
//      : DeserializerBase(&Buf), SP(SP),
//        DstMod(SP.getCompilationUnit().getCompiledModule()),
//        SrcMod(SrcMod),
//        SymTab(*SP.getCompilationUnit().getSymTab()),
//        deserializer(SP, Reader, SrcMod, importedFrom),
//        importedFrom(importedFrom)
//   {}
//
//   void deserialize(bool skipForwardDecls = false)
//   {
//      llvm::SmallString<128> filePrivateName;
//      if (importedFrom) {
//         filePrivateName += SP.getDeclPass()
//                              ->getPrivateFileNamespace(importedFrom)
//                              ->getName();
//         filePrivateName += ".";
//      }
//
//      auto initialSize = filePrivateName.size();
//
//      auto ptr = reinterpret_cast<size_t const*>(Reader.skip(8));
//      auto forwardDeclSize = *ptr;
//
//      if (skipForwardDecls) {
//         Reader.skip(forwardDeclSize);
//      }
//      else {
//         auto numForwardDecls = ReadSize();
//         for (size_t i = 0; i < numForwardDecls; ++i) {
//            ReadForwardDecl(filePrivateName);
//            filePrivateName.resize(initialSize);
//         }
//      }
//
//      while (true) {
//         auto byte = Reader.ReadByte();
//         if (byte == StopByte)
//            break;
//
//         switch (byte) {
//            case SymbolTableEntry::RecordID:
//               ReadRecord();
//               break;
//            case SymbolTableEntry::TypedefID:
//               ReadTypedef();
//               break;
//            case SymbolTableEntry::VariableID:
//               ReadVariable();
//               break;
//            case SymbolTableEntry::NamespaceID:
//               ReadNamespace();
//               break;
//            case SymbolTableEntry::AliasID: {
//               ReadAlias(filePrivateName, skipForwardDecls);
//               break;
//            }
//            case SymbolTableEntry::FunctionID: {
//               ReadFunction(filePrivateName, skipForwardDecls);
//               break;
//            }
//            default:
//               llvm_unreachable("bad decl kind!");
//         }
//
//         filePrivateName.resize(initialSize);
//      }
//
//      DstMod->setDeserialized(true);
//   }
//
//private:
//   SemaPass &SP;
//   Module *DstMod;
//   Module *SrcMod;
//   SymbolTable &SymTab;
//   serial::ASTDeserializer deserializer;
//   SourceLocation importedFrom;
//
//   NamedDecl *ReadForwardDecl(llvm::SmallString<128> &filePrivateNamespace)
//   {
//      auto initialSize = filePrivateNamespace.size();
//      auto kind = ReadEnum<AstNode::NodeType>();
//
//      if (RecordDecl::classofKind(kind)) {
//         auto access = ReadEnum<AccessModifier>();
//
//         auto name = ReadString();
//         filePrivateNamespace += name;
//
//         auto params = ReadList<TemplateParameter>
//            (&ModuleInterfaceDeserializer::ReadTemplateParameter);
//
//         CommonRecordContents contents;
//         contents.am = access;
//         contents.recordName = move(name);
//         contents.templateParams = move(params);
//
//         RecordDecl *R;
//         if (kind == AstNode::StructDeclID) {
//            R = new(SP.getContext()) StructDecl(move(contents));
//         }
//         else if (kind == AstNode::ClassDeclID) {
//            contents.isAbstract = ReadBool();
//            R = new(SP.getContext()) ClassDecl(move(contents));
//         }
//         else if (kind == AstNode::EnumDeclID) {
//            R = new(SP.getContext()) EnumDecl(move(contents));
//         }
//         else if (kind == AstNode::UnionDeclID) {
//            R = new(SP.getContext()) UnionDecl(move(contents));
//         }
//         else {
//            assert(kind == AstNode::ProtocolDeclID);
//            R = new(SP.getContext()) ProtocolDecl(move(contents));
//         }
//
//         R->setExternal(true);
//
//         SymTab.declareRecord(filePrivateNamespace.str(), R);
//         DstMod->addDecl(R);
//
//         SP.getILGen().ForwardDeclareRecord(R);
//
//         auto numInnerRecords = ReadSize();
//         for (size_t i = 0; i < numInnerRecords; ++i) {
//            filePrivateNamespace.resize(initialSize);
//            auto Inner = ReadForwardDecl(filePrivateNamespace);
//            R->addInnerRecord(cast<RecordDecl>(Inner));
//         }
//
//         return R;
//      }
//      else if (kind == AstNode::TypedefDeclID) {
//         auto access = ReadEnum<AccessModifier>();
//         auto name = ReadString();
//         auto params = ReadList<TemplateParameter>(
//            &ModuleInterfaceDeserializer::ReadTemplateParameter);
//
//         auto typeref = new (SP.getContext()) TypeRef;
//         auto TD = new (SP.getContext()) TypedefDecl(access, move(name),
//                                                     typeref, move(params));
//
//         TD->setExternal(true);
//
//         filePrivateNamespace += TD->getName();
//
//         SymTab.declareTypedef(filePrivateNamespace.str(), TD);
//         DstMod->addDecl(TD);
//
//         return TD;
//      }
//      else {
//         llvm_unreachable("bad forward decl kind");
//      }
//   }
//
//   template<class T>
//   T* deserializeStmt()
//   {
//      return deserializer.ReadSpecific<T>();
//   }
//
//   QualType ReadQualType()
//   {
//      char c = Reader.ReadByte();
//      return QualType(ReadType(), (c & 1) != 0, (c & 2) != 0);
//   }
//
//   Type *ReadType()
//   {
//      auto typeID = (TypeID)Reader.ReadByte();
//      switch (typeID) {
//         case TypeID::IntegerTypeID: {
//            auto bw = Reader.ReadULEB128();
//            bool isUnsigned = (bw & (1 << 10)) != 0;
//            bw &= ~(1 << 10);
//
//            return IntegerType::get(bw, isUnsigned);
//         }
//         case TypeID::FPTypeID:
//            return ReadBool() ? FPType::getDoubleTy() : FPType::getFloatTy();
//         case TypeID::PointerTypeID:
//            return PointerType::get(ReadQualType());
//         case TypeID::ArrayTypeID: {
//            auto numElements = Reader.ReadULEB128();
//            auto elementTy = ReadQualType();
//
//            return ArrayType::get(*elementTy, numElements);
//         }
//         case TypeID::InferredArrayTypeID: {
//            auto param = ReadString();
//            auto elementTy = ReadQualType();
//
//            return InferredArrayType::get(*elementTy, param);
//         }
//         case TypeID::TupleTypeID: {
//            std::vector<std::pair<string, QualType>> types;
//            auto size = Reader.ReadULEB128();
//
//            for (size_t i = 0; i < size; ++i) {
//               auto fst = ReadString();
//               auto snd = ReadQualType();
//
//               types.emplace_back(std::move(fst), snd);
//            }
//
//            return TupleType::get(move(types));
//         }
//         case TypeID::FunctionTypeID: {
//            auto ret = ReadQualType();
//            auto args = ReadList<Argument>(
//               &ModuleInterfaceDeserializer::ReadArgument);
//
//            return FunctionType::get(ret, move(args), ReadBool());
//         }
//         case TypeID::ObjectTypeID: {
//            return SP.getObjectTy(ReadString());
//         }
//         case TypeID::InconcreteObjectTypeID: {
//            auto R = SP.getRecord(ReadString());
//            auto args = ReadTemplateArgList();
//
//            return InconcreteObjectType::get(R, move(args));
//         }
//         case TypeID::GenericTypeID: {
//            auto name = ReadString();
//            auto cov = ReadQualType();
//
//            return GenericType::get(name, *cov);
//         }
//         case TypeID::VoidTypeID:
//            return VoidType::get();
//         case TypeID::AutoTypeID:
//            return AutoType::get();
//         case TypeID::TypedefTypeID: {
//            auto td = SP.getTypedef(ReadString());
//            auto ty = ReadQualType();
//
//            return TypedefType::get(td, ty);
//         }
//         case TypeID::MetaTypeID:
//            return MetaType::get(ReadType());
//         case TypeID::NamespaceTypeID:
//            return NamespaceType::get(SP.getNamespace(ReadString()));
//         default:
//            llvm_unreachable("bad type!");
//      }
//   }
//
//   sema::TemplateArgList ReadTemplateArgList()
//   {
//      auto numParams = ReadSize();
//      sema::TemplateArgList list(SP);
//
//      for (size_t i = 0; i < numParams; ++i) {
//         auto name = ReadString();
//         auto arg = ReadResolvedTemplateArg();
//
//         list.insert(name, move(arg));
//      }
//
//      return list;
//   }
//
//   sema::ResolvedTemplateArg ReadResolvedTemplateArg()
//   {
//      bool isTypeName, isVariadic;
//      ReadBools(isTypeName, isVariadic);
//
//      if (isVariadic) {
//         auto variadicArgs = ReadList<sema::ResolvedTemplateArg>(
//            &ModuleInterfaceDeserializer::ReadResolvedTemplateArg);
//
//         return sema::ResolvedTemplateArg(isTypeName, move(variadicArgs));
//      }
//      else if (!isTypeName) {
//         return sema::ResolvedTemplateArg(ReadVariant());
//      }
//      else {
//         return sema::ResolvedTemplateArg(ReadType());
//      }
//   }
//
//   SourceLocation ReadSourceLoc()
//   {
//      auto offset = unsigned(Reader.ReadULEB128());
//      auto id = unsigned(Reader.ReadULEB128());
//
//      return SourceLocation(offset, id);
//   }
//
//   Argument ReadArgument()
//   {
//      bool cstyleVararg, variadic;
//      ReadBools(cstyleVararg, variadic);
//
//      if (cstyleVararg) {
//         return Argument("", {}, cstyleVararg, variadic);
//      }
//
//      auto label = ReadString();
//      auto type = ReadQualType();
//
//      return Argument(std::move(label), type, cstyleVararg, variadic);
//   }
//
//   TemplateParameter ReadTemplateParameter()
//   {
//      auto name = ReadString();
//      bool isTypeName, isVariadic;
//      ReadBools(isTypeName, isVariadic);
//
//      Type *cov = nullptr;
//      if (ReadBool()) {
//         cov = ReadType();
//      }
//
//      Type *con = nullptr;
//      if (ReadBool()) {
//         con = ReadType();
//      }
//
//      return TemplateParameter(isTypeName ? TemplateParameter::TypeName
//                                          : TemplateParameter::Value,
//                               move(name), cov, con, nullptr, isVariadic);
//   }
//
//   NamespaceDecl *ReadNamespace()
//   {
//      auto name = ReadString();
//
//      bool isAnon, hasParent;
//      ReadBools(isAnon, hasParent);
//
//      NamespaceDecl *parent = nullptr;
//      if (hasParent)
//         parent = SP.getOrDeclareNamespace(ReadString());
//
//      auto NS = new (SP.getContext()) NamespaceDecl(move(name), nullptr,
//                                                    isAnon);
//
//      NS->setOuterNamespace(parent);
//
//      return NS;
//   }
//
//   void ReadVariable()
//   {
//      auto name = ReadString();
//      auto access = ReadEnum<AccessModifier>();
//      auto ty = ReadQualType();
//
//      bool isTypeDependent, isGlobal, hasNS;
//      ReadBools(isTypeDependent, isGlobal, hasNS);
//
//      NamespaceDecl *declNamespace = nullptr;
//      if (hasNS)
//         declNamespace = SP.getOrDeclareNamespace(ReadString());
//
//      auto Entry = SymTab.declareVariable(name, ty, access, false,
//                                          declNamespace->getNamespaceId(),
//                                          nullptr);
//
////      SP.getILGen().DeclareGlobalVariable(*Entry->getVariable());
//   }
//
//   FuncArgDecl* ReadArgDecl()
//   {
//      auto name = ReadString();
//      auto ty = ReadQualType();
//      bool isConst, isVararg, isCstyleVararg, isVariadic;
//      ReadBools(isConst, isVararg, isCstyleVararg, isVariadic);
//
//      auto argTy = new (SP.getContext()) TypeRef(ty);
//      return new (SP.getContext()) FuncArgDecl(move(name), argTy, nullptr,
//                                               isVariadic, isConst,
//                                               isCstyleVararg);
//   }
//
//   void ReadFunction(llvm::SmallString<128> &filePrivateNamespace,
//                     bool skipForwardDecls) {
//      auto idx = Reader.ReadULEB128();
//      auto name = ReadString();
//      filePrivateNamespace += name;
//
//      auto linkageName = ReadString();
//
//      auto ret = ReadQualType();
//      auto access = ReadEnum<AccessModifier>();
//
//      auto args = ReadList<FuncArgDecl*>
//         (&ModuleInterfaceDeserializer::ReadArgDecl);
//
//      auto params = ReadList<TemplateParameter>
//         (&ModuleInterfaceDeserializer::ReadTemplateParameter);
//
//      std::vector<StaticExpr* > constraints;
//      auto numConstraints = Reader.ReadULEB128();
//
//      for (size_t i = 0; i < numConstraints; ++i)
//         constraints.push_back(deserializeStmt<StaticExpr>());
//
//      auto op = ReadOperatorInfo();
//
//      bool isExternC, isNoThrow, isNative, isConversionOp, hasNS;
//      ReadBools(isExternC, isNoThrow, isNative, isConversionOp, hasNS);
//
//      auto loc = ReadSourceLoc();
//      if (auto alias =
//                  SP.getCompilationUnit().getSourceAlias(loc.getSourceId())) {
//         loc.sourceId = unsigned(alias);
//      }
//      else {
//         loc = DstMod->getSourceLoc();
//      }
//
//      NamespaceDecl *declNamespace = nullptr;
//      if (hasNS)
//         declNamespace = SP.getOrDeclareNamespace(ReadString());
//
//      CompoundStmt *body = nullptr;
//      if (ReadBool()) {
//         body = deserializeStmt<CompoundStmt>();
//      }
//
//      auto retTy = new (SP.getContext()) TypeRef(ret);
//
//      FunctionDecl *func;
//      if (skipForwardDecls) {
//         func = cast<FunctionDecl>(cast<FunctionDecl>(DstMod->getDecl(name)
//                                                       ->getDecl())
//                                      ->getOverloads()[idx]);
//         func->setArgs(move(args));
//         func->setReturnType(retTy);
//         func->setConstraints(move(constraints));
//         func->setBody(body);
//      }
//      else {
//         func = new (SP.getContext())
//            FunctionDecl(access, move(name), move(args), retTy, move(params),
//                         move(constraints), body, op);
//      }
//
//      func->setExternC(isExternC);
//      func->isNoThrow(isNoThrow);
//      func->setNative(isNative);
//      func->setIsConversionOp(isConversionOp);
//      func->setLinkageName(move(linkageName));
//      func->setExternal(true);
//      func->setSourceLoc(loc);
//
//      func->createFunctionType(SP);
//
//      if (!skipForwardDecls) {
//         SP.getILGen().DeclareFunction(func);
//
//         if (idx) {
//            cast<FunctionDecl>(DstMod->getDecl(func->getName())->getDecl())
//               ->addOverload(func);
//         }
//         else {
//            DstMod->addDecl(func);
//            SymTab.declareFunction(filePrivateNamespace.str(), func);
//         }
//      }
//   }
//
//   void ReadTypedef()
//   {
//      auto name = ReadString();
//      auto td = cast<TypedefDecl>(DstMod->getDecl(name)->getDecl());
//      td->getOriginTy()->setType(ReadQualType());
//   }
//
//   void ReadTypedef(RecordDecl *R)
//   {
//      auto access = ReadEnum<AccessModifier>();
//      auto name = ReadString();
//      auto qualTy = ReadQualType();
//      auto params = ReadList<TemplateParameter>(
//         &ModuleInterfaceDeserializer::ReadTemplateParameter);
//
//      auto typeref = new (SP.getContext()) TypeRef(qualTy);
//      auto TD = new (SP.getContext()) TypedefDecl(access, move(name),
//                                                  typeref, move(params));
//
//      TD->setExternal(true);
//      R->addDecl(TD);
//   }
//
//   void ReadAlias(llvm::SmallString<128> &filePrivateNamespace,
//                  bool skipForwardDecls) {
//      auto name = ReadString();
//      auto idx = ReadSize();
//
//      auto alias = cast<AliasDecl>(DstMod->getDecl(name)->getDecl())
//         ->getOverloads()[idx];
//
//      auto numConstraints = ReadSize();
//      std::vector<StaticExpr* > constraints;
//      for (size_t i = 0; i < numConstraints; ++i)
//         constraints.push_back(deserializeStmt<StaticExpr>());
//
//      alias->setConstraints(move(constraints));
//
//      if (ReadBool()) {
//         auto V = ReadVariant();
//         alias->setVal(std::move(V));
//      }
//      else {
//         auto expr = deserializeStmt<StaticExpr>();
//         alias->setAliasExpr(move(expr));
//      }
//   }
//
//   void ReadRecord(RecordDecl *Outer = nullptr)
//   {
//      auto name = ReadString();
//      auto R = cast<RecordDecl>(DstMod->getDecl(name)->getDecl());
//
//      auto numDecls = ReadSize();
//      for (size_t i = 0; i < numDecls; ++i) {
//         auto kind = ReadEnum<AstNode::NodeType>();
//         switch (kind) {
//            case AstNode::MethodDeclID:
//               ReadMethods(R);
//               break;
//            case AstNode::InitDeclID:
//               ReadInitializer(R);
//               break;
//            case AstNode::DeinitDeclID:
//               ReadDeinitializer(R);
//               break;
//            case AstNode::FieldDeclID:
//               ReadField(R);
//               break;
//            case AstNode::PropDeclID:
//               ReadProperty(R);
//               break;
//            case AstNode::AssociatedTypeDeclID:
//               ReadAssociatedType(R);
//               break;
//            case AstNode::EnumCaseDeclID:
//               ReadCase(cast<EnumDecl>(R));
//               break;
//            case AstNode::TypedefDeclID:
//               ReadTypedef(R);
//               break;
//            case AstNode::EnumDeclID:
//            case AstNode::StructDeclID:
//            case AstNode::ClassDeclID:
//            case AstNode::UnionDeclID:
//            case AstNode::ProtocolDeclID:
//               ReadRecord(R);
//               break;
//            default:
//               llvm_unreachable("bad decl kind");
//         }
//      }
//
//      auto numConstraints = ReadSize();
//      std::vector<StaticExpr* > constraints;
//      for (size_t i = 0; i < numConstraints; ++i) {
//         constraints.push_back(deserializeStmt<StaticExpr>());
//      }
//
//      R->setConstraints(move(constraints));
//
//      if (auto C = dyn_cast<ClassDecl>(R)) {
//         if (ReadBool())
//            C->setParentClass(
//               cast<ClassDecl>(DstMod->getDecl(ReadString())->getDecl()));
//      }
//      else if (auto E = dyn_cast<EnumDecl>(R)) {
//         E->setRawType(new (SP.getContext()) TypeRef(ReadType()));
//      }
//
//      R->setExternal(true);
//
//      SP.getILGen().DeclareRecord(R);
//   }
//
//   void ReadField(RecordDecl *R)
//   {
//      auto name = ReadString();
//      auto linkageName = ReadString();
//      auto type = ReadQualType();
//      auto access = ReadEnum<AccessModifier>();
//
//      bool isConst, isStatic, hasGetter, hasSetter;
//      ReadBools(isConst, isStatic, hasGetter, hasSetter);
//
//      MethodDecl *getter = nullptr;
//      if (hasGetter) {
//         getter = R->getMethod(ReadString());
//      }
//
//      MethodDecl *setter = nullptr;
//      if (hasSetter) {
//         setter = R->getMethod(ReadString());
//      }
//
//      auto fieldTy = new (SP.getContext()) TypeRef(type);
//      auto F = new (SP.getContext()) FieldDecl(move(name), fieldTy, access,
//                                               isStatic, isConst, nullptr);
//
//      F->setLinkageName(move(linkageName));
//      F->setGetterMethod(getter);
//      F->setSetterMethod(setter);
//
//      R->addDecl(F);
//   }
//
//   void ReadInitializer(RecordDecl *R)
//   {
//      auto name = ReadString();
//      auto linkageName = ReadString();
//
//      auto access = ReadEnum<AccessModifier>();
//
//      auto args = ReadList<FuncArgDecl*>
//         (&ModuleInterfaceDeserializer::ReadArgDecl);
//
//      auto params = ReadList<TemplateParameter>
//         (&ModuleInterfaceDeserializer::ReadTemplateParameter);
//
//      std::vector<StaticExpr* > constraints;
//      auto numConstraints = Reader.ReadULEB128();
//
//      for (size_t i = 0; i < numConstraints; ++i)
//         constraints.push_back(deserializeStmt<StaticExpr>());
//
//      bool memberwise = ReadBool();
//
//      CompoundStmt *body = nullptr;
//      if (ReadBool()) {
//         body = deserializeStmt<CompoundStmt>();
//      }
//
//      auto I = new (SP.getContext()) InitDecl(move(args), access,
//                                                move(params), body, move(name));
//
//      I->setMemberwiseInitializer(memberwise);
//      I->setLinkageName(move(linkageName));
//      I->setExternal(true);
//
//      if (memberwise)
//         cast<StructDecl>(R)->setMemberwiseInitializer(I);
//
//      R->addDecl(I);
//      I->createFunctionType(SP);
//   }
//
//   void ReadDeinitializer(RecordDecl *R)
//   {
//      auto linkageName = ReadString();
//
//      CompoundStmt *body = nullptr;
//      if (ReadBool()) {
//         body = deserializeStmt<CompoundStmt>();
//      }
//
//      auto D = new (SP.getContext()) DeinitDecl(body);
//      D->setExternal(true);
//      D->setLinkageName(move(linkageName));
//
//      R->addDecl(D);
//      D->createFunctionType(SP);
//   }
//
//   void ReadMethods(RecordDecl *R)
//   {
//      auto numOverloads = ReadSize();
//      assert(numOverloads && "no method overload?");
//
//      for (size_t i = 0; i < numOverloads; ++i)
//         ReadMethod(R);
//   }
//
//   void ReadMethod(RecordDecl *R)
//   {
//      auto name = ReadString();
//      auto linkageName = ReadString();
//
//      auto ret = ReadQualType();
//      auto access = ReadEnum<AccessModifier>();
//
//      auto args = ReadList<FuncArgDecl*>
//         (&ModuleInterfaceDeserializer::ReadArgDecl);
//
//      auto params = ReadList<TemplateParameter>
//         (&ModuleInterfaceDeserializer::ReadTemplateParameter);
//
//      std::vector<StaticExpr* > constraints;
//      auto numConstraints = Reader.ReadULEB128();
//
//      for (size_t i = 0; i < numConstraints; ++i)
//         constraints.push_back(deserializeStmt<StaticExpr>());
//
//      auto op = ReadOperatorInfo();
//
//      bool isStatic, isInit, isMemberwiseInit, isProp, isProtoDefaultImpl,
//         isProtoMethod, isNoThrow, isConvOp;
//
//      ReadBools(isStatic, isInit, isMemberwiseInit, isProp, isProtoDefaultImpl,
//                isProtoMethod, isNoThrow, isConvOp);
//
//      CompoundStmt *body = nullptr;
//      if (ReadBool()) {
//         auto begin = Reader.getOffset();
//         body = deserializeStmt<CompoundStmt>();
//         llvm::outs()
//            << "read " << linkageName << ": " << (Reader.getOffset() - begin)
//            << " bytes\n";
//      }
//
//      auto retTy = new (SP.getContext()) TypeRef(ret);
//      auto M = new (SP.getContext()) MethodDecl(move(name), retTy, move(args),
//                                                move(params),
//                                                move(constraints), body, op,
//                                                isConvOp, access, isStatic);
//
//      M->setMemberwiseInitializer(isMemberwiseInit);
//      M->setProperty(isProp);
//      M->setProtocolDefaultImpl(isProtoDefaultImpl);
//      M->isNoThrow(isNoThrow);
//      M->setIsConversionOp(isConvOp);
//      M->setLinkageName(move(linkageName));
//      M->setExternal(true);
//
//      if (M->isMemberwiseInitializer())
//         cast<StructDecl>(R)->setMemberwiseInitializer(M);
//
//      R->addDecl(M);
//      M->createFunctionType(SP);
//   }
//
//   void ReadProperty(RecordDecl *R)
//   {
//      auto access = ReadEnum<AccessModifier>();
//      auto name = ReadString();
//      auto ty = ReadQualType();
//
//      bool isStatic, hasGetter, hasSetter;
//      ReadBools(isStatic, hasGetter, hasSetter);
//
//      MethodDecl *getter = nullptr;
//      if (hasGetter) {
//         getter = R->getMethod(ReadString());
//      }
//
//      MethodDecl *setter = nullptr;
//      if (hasSetter) {
//         setter = R->getMethod(ReadString());
//      }
//
//      auto propTy = new (SP.getContext()) TypeRef(ty);
//      auto P = new (SP.getContext()) PropDecl(move(name), propTy, access,
//                                              isStatic, true,
//                                              getter != nullptr,
//                                              setter != nullptr,
//                                              nullptr, nullptr, "");
//
//      P->setGetterMethod(getter);
//      P->setSetterMethod(setter);
//
//      R->addDecl(P);
//   }
//
//   void ReadAssociatedType(RecordDecl *R)
//   {
//      auto proto = ReadString();
//      auto name = ReadString();
//      TypeRef *ty = nullptr;
//
//      if (ReadBool()) {
//         ty = new (SP.getContext()) TypeRef(ReadQualType());
//      }
//
//      std::vector<StaticExpr* > constraints;
//      auto numConstraints = Reader.ReadULEB128();
//
//      for (size_t i = 0; i < numConstraints; ++i)
//         constraints.push_back(deserializeStmt<StaticExpr>());
//
//      auto AT = new (SP.getContext()) AssociatedTypeDecl(move(proto),
//                                                         move(name),
//                                                         move(constraints), ty);
//
//      R->addDecl(AT);
//   }
//
//   void ReadCase(EnumDecl *E)
//   {
//      auto name = ReadString();
//      auto rawVal = long(Reader.ReadSLEB128());
//      auto args = ReadList<FuncArgDecl*>
//         (&ModuleInterfaceDeserializer::ReadArgDecl);
//
//      auto C = new (SP.getContext()) EnumCaseDecl(move(name), move(args));
//      C->setRawValue(rawVal);
//
//      E->addDecl(C);
//   }
//
//   PrecedenceGroup ReadPrecedenceGroup()
//   {
//      long prec = long(Reader.ReadSLEB128());
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
//   Variant ReadVariant()
//   {
//      auto ty = ReadType();
//      auto kind = ReadEnum<VariantType>();
//
//      switch (kind) {
//         case VariantType::INT:
//            return Variant(Reader.ReadULEB128());
//         case VariantType::FLOAT: {
//            union {
//               double d;
//               size_t s;
//            } u;
//
//            u.s = Reader.ReadULEB128();
//            return Variant(u.d);
//         }
//         case VariantType::STRING:
//            return Variant(ReadString());
//         case VariantType::ARRAY:
//            return Variant(ReadList<Variant>
//                              (&ModuleInterfaceDeserializer::ReadVariant),
//                           ty);
//         case VariantType::STRUCT: {
//            auto numFields = ReadSize();
//            std::vector<Variant::Field> fields;
//
//            for (size_t i = 0; i < numFields; ++i) {
//               auto name = ReadString();
//               auto val = ReadVariant();
//
//               fields.emplace_back(std::move(name), std::move(val));
//            }
//
//            return Variant(ty, move(fields));
//         }
//         case VariantType::MetaType:
//            return Variant(ReadType());
//         case VariantType::VOID:
//            return Variant();
//         default:
//            llvm_unreachable("can't deserialize Variant!");
//      }
//   }
//};
//
//} // anonymous namespace
//
//void deserializeAST(SemaPass &SP,
//                    Module &M,
//                    llvm::StringRef file,
//                    SourceLocation importedFrom) {
//   auto Buf = fs::FileManager::openFile(file, true);
//   (void)deserializeASTImpl(SP, M, *Buf.second, Buf.first, importedFrom);
//}
//
//void createModuleInterface(Module *M,
//                           SymbolTable const& SymTab,
//                           llvm::raw_ostream &out) {
//   ModuleInterfaceSerializer(M, SymTab).finalize(out);
//}
//
//
//void readModuleInterface(SemaPass &SP, llvm::MemoryBuffer &Buf,
//                         Module *SrcMod, bool skipForwardDecls,
//                         SourceLocation importedFrom) {
//   return ModuleInterfaceDeserializer(SP, Buf, SrcMod, importedFrom)
//      .deserialize(skipForwardDecls);
//}
//
//} // namespace module
//} // namespace cdot