#include "cdotc/IRGen/IRGen.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Type.h"
#include "cdotc/Basic/FileManager.h"
#include "cdotc/Basic/FileUtils.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/IL/Context.h"
#include "cdotc/IL/Module.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/Module.h>

using std::string;
using namespace cdot::fs;
using namespace cdot::ast;
using namespace cdot::support;

namespace cdot {
namespace il {

llvm::MDNode* IRGen::emitModuleDI()
{
   for (const auto& Sub : DIFuncMap) {
      Sub.second->replaceUnit(CU);
   }

   DI->finalize();

   M->addModuleFlag(llvm::Module::Warning, "Dwarf Version",
                    llvm::dwarf::DWARF_VERSION);
   M->addModuleFlag(llvm::Module::Warning, "Debug Info Version",
                    (uint32_t)llvm::DEBUG_METADATA_VERSION);

   return CU;
}

llvm::DIFile* IRGen::getFileDI(size_t fileID, llvm::StringRef fileName,
                               llvm::StringRef path)
{
   auto ID = fileID;
   auto it = DIFileMap.find(ID);
   if (it != DIFileMap.end())
      return it->second;

   auto File = DI->createFile(fileName, path);

   DIFileMap.try_emplace(ID, File);
   return File;
}

llvm::DIFile* IRGen::getFileDI(SourceLocation loc)
{
   auto ID = CI.getFileMgr().getSourceId(loc);
   if (!ID)
      return nullptr;

   auto it = DIFileMap.find(ID);
   if (it != DIFileMap.end())
      return it->second;

   auto& FileMgr = CI.getFileMgr();
   auto fileNameAndPath = FileMgr.getFileName(ID).str();

   llvm::SmallString<64> Path;
   Path += fs::getPath(fileNameAndPath);

   fs::makeAbsolute(Path);

   auto fileName = fs::getFileNameAndExtension(fileNameAndPath);

   auto File = DI->createFile(fileName, Path.str());

   DIFileMap.try_emplace(ID, File);
   return File;
}

llvm::DIType* IRGen::getTypeDI(QualType ty)
{
   ty = ty->getCanonicalType();

   auto it = DITypeMap.find(ty);
   if (it != DITypeMap.end())
      return it->second;

   auto& TI = CI.getContext().getTargetInfo();

   llvm::DIType* MD;
   switch (ty->getTypeID()) {
   case Type::BuiltinTypeID: {
      switch (ty->asBuiltinType()->getKind()) {
      case BuiltinType::i1:
         MD = DI->createBasicType("i1", 8, llvm::dwarf::DW_ATE_boolean);

         break;
      case BuiltinType::i8:
         MD = DI->createBasicType("i8", 8, llvm::dwarf::DW_ATE_signed_char);

         break;
      case BuiltinType::u8:
         MD = DI->createBasicType("u8", 8, llvm::dwarf::DW_ATE_unsigned_char);

         break;
      case BuiltinType::i16:
         MD = DI->createBasicType("i16", 16, llvm::dwarf::DW_ATE_signed);

         break;
      case BuiltinType::u16:
         MD = DI->createBasicType("u16", 16, llvm::dwarf::DW_ATE_unsigned);

         break;
      case BuiltinType::i32:
         MD = DI->createBasicType("i32", 32, llvm::dwarf::DW_ATE_signed);

         break;
      case BuiltinType::u32:
         MD = DI->createBasicType("u32", 32, llvm::dwarf::DW_ATE_unsigned);

         break;
      case BuiltinType::i64:
         MD = DI->createBasicType("i64", 64, llvm::dwarf::DW_ATE_signed);

         break;
      case BuiltinType::u64:
         MD = DI->createBasicType("u64", 64, llvm::dwarf::DW_ATE_unsigned);

         break;
      case BuiltinType::i128:
         MD = DI->createBasicType("i128", 128, llvm::dwarf::DW_ATE_signed);

         break;
      case BuiltinType::u128:
         MD = DI->createBasicType("u128", 128, llvm::dwarf::DW_ATE_unsigned);

         break;
      case BuiltinType::f16:
         MD = DI->createBasicType("f16", 16, llvm::dwarf::DW_ATE_float);
         break;
      case BuiltinType::f32:
         MD = DI->createBasicType("f32", 32, llvm::dwarf::DW_ATE_float);
         break;
      case BuiltinType::f64:
         MD = DI->createBasicType("f64", 64, llvm::dwarf::DW_ATE_float);
         break;
      case BuiltinType::f80:
         MD = DI->createBasicType("f80", 80, llvm::dwarf::DW_ATE_float);
         break;
      case BuiltinType::f128:
         MD = DI->createBasicType("f128", 128, llvm::dwarf::DW_ATE_float);
         break;
      case BuiltinType::Void:
         MD = nullptr;
         break;
      default:
         llvm_unreachable("bad builtin type");
      }

      break;
   }
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID: {
      MD = DI->createPointerType(getTypeDI(ty->getPointeeType()),
                                 TI.getPointerSizeInBytes() * 8,
                                 TI.getPointerAlignInBytes());

      break;
   }
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID: {
      MD = DI->createReferenceType(llvm::dwarf::DW_TAG_reference_type,
                                   getTypeDI(ty->getReferencedType()),
                                   TI.getPointerSizeInBytes() * 8u,
                                   TI.getPointerAlignInBytes());

      break;
   }
   case Type::BoxTypeID: {
      auto& ASTCtx = CI.getContext();
      auto Int8PtrTy = getTypeDI(ASTCtx.getInt8PtrTy());
      auto UWordTy = getTypeDI(ASTCtx.getUIntTy());

      llvm::Metadata* ContainedTys[] = {UWordTy, UWordTy, Int8PtrTy, Int8PtrTy};

      MD = DI->createStructType(
          ScopeStack.top(), ty->toString(), File, 0, TI.getSizeOfType(ty) * 8u,
          TI.getAlignOfType(ty) * 8u, llvm::DINode::DIFlags::FlagZero, nullptr,
          DI->getOrCreateArray(ContainedTys));

      break;
   }
   case Type::FunctionTypeID: {
      std::vector<llvm::Metadata*> argTypes;
      for (auto& argTy : ty->asFunctionType()->getParamTypes()) {
         argTypes.push_back(getTypeDI(*argTy));
      }

      MD = DI->createPointerType(
          DI->createSubroutineType(DI->getOrCreateTypeArray(argTypes)),
          TI.getPointerSizeInBytes() * 8u, TI.getPointerAlignInBytes());

      break;
   }
   case Type::LambdaTypeID: {
      auto& ASTCtx = CI.getContext();
      auto Int8PtrTy = getTypeDI(ASTCtx.getInt8PtrTy());

      llvm::Metadata* ContainedTys[5] = {Int8PtrTy, Int8PtrTy};

      MD = DI->createStructType(
          ScopeStack.top(), ty->toString(), File, 0, TI.getSizeOfType(ty) * 8u,
          TI.getAlignOfType(ty) * 8u, llvm::DINode::DIFlags::FlagZero, nullptr,
          DI->getOrCreateArray(ContainedTys));

      break;
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      MD = getRecordDI(ty);
      break;
   }
   case Type::TupleTypeID: {
      auto tuple = ty->asTupleType();

      auto flags = llvm::DINode::DIFlags::FlagZero;
      llvm::SmallVector<llvm::Metadata*, 4> containedTypes;

      llvm::DICompositeType* forwardDecl = DI->createReplaceableCompositeType(
          llvm::dwarf::DW_TAG_structure_type, "", ScopeStack.top(), File, 0);

      unsigned i = 0;
      unsigned offset = 0;

      std::string tupleName;
      llvm::raw_string_ostream OS(tupleName);

      OS << "(";

      for (auto& cont : tuple->getContainedTypes()) {
         if (i != 0)
            OS << ", ";
         OS << cont;

         auto size = TI.getSizeOfType(cont) * 8;
         auto elementDI = DI->createMemberType(
             forwardDecl, std::to_string(i++), File, 1, size,
             TI.getAlignOfType(cont) * 8, offset, flags, getTypeDI(cont));

         offset += size;
         containedTypes.push_back(elementDI);
      }

      OS << ")";

      MD = DI->createStructType(ScopeStack.top(), OS.str(), File, 1,
                                TI.getSizeOfType(ty) * 8u,
                                TI.getAlignOfType(ty) * 8u, flags, nullptr,
                                DI->getOrCreateArray(containedTypes));

      DI->replaceTemporary(llvm::TempDIType(forwardDecl), MD);

      break;
   }
   case Type::ArrayTypeID: {
      ArrayType* ArrTy = ty->asArrayType();
      MD = DI->createArrayType(
          ArrTy->getNumElements(), TI.getAlignOfType(ArrTy),
          getTypeDI(*ArrTy->getElementType()), DI->getOrCreateArray({}));

      break;
   }
   case Type::MetaTypeID: {
      auto& ASTCtx = CI.getContext();
      auto Int8PtrTy = getTypeDI(ASTCtx.getInt8PtrTy());

      llvm::Metadata* ContainedTys[]{Int8PtrTy, Int8PtrTy, Int8PtrTy, Int8PtrTy,
                                     Int8PtrTy};

      MD = DI->createStructType(
          ScopeStack.top(), ty->toString(), File, 0, TI.getSizeOfType(ty) * 8u,
          TI.getAlignOfType(ty) * 8u, llvm::DINode::DIFlags::FlagZero, nullptr,
          DI->getOrCreateArray(ContainedTys));

      break;
   }
   case Type::ExistentialTypeID: {
      auto& ASTCtx = CI.getContext();
      auto Int8PtrTy = getTypeDI(ASTCtx.getInt8PtrTy());

      llvm::Metadata* ContainedTys[]{Int8PtrTy, Int8PtrTy, Int8PtrTy};

      MD = DI->createStructType(
          ScopeStack.top(), ty->toString(), File, 0, TI.getSizeOfType(ty) * 8u,
          TI.getAlignOfType(ty) * 8u, llvm::DINode::DIFlags::FlagZero, nullptr,
          DI->getOrCreateArray(ContainedTys));

      break;
   }
   default:
      llvm_unreachable("should not be possible here");
   }

   DITypeMap[ty] = MD;
   return MD;
}

llvm::dwarf::Tag IRGen::getTagForRecord(ast::RecordDecl* R)
{
   llvm::dwarf::Tag Tag;
   switch (R->getKind()) {
   case Decl::StructDeclID:
   case Decl::ProtocolDeclID:
      Tag = llvm::dwarf::DW_TAG_structure_type;
      break;
   case Decl::ClassDeclID:
      Tag = llvm::dwarf::DW_TAG_class_type;
      break;
   case Decl::EnumDeclID:
      if (R->isRawEnum())
         Tag = llvm::dwarf::DW_TAG_enumeration_type;
      else
         // FIXME use DW_TAG_variant_part once available
         Tag = llvm::dwarf::DW_TAG_union_type;

      break;
   case Decl::UnionDeclID:
      Tag = llvm::dwarf::DW_TAG_union_type;
      break;
   default:
      llvm_unreachable("not a type!");
   }

   return Tag;
}

llvm::DIType* IRGen::getRecordDI(QualType ty)
{
   assert(ty->isRecordType());

   auto flags = llvm::DINode::DIFlags::FlagZero;
   llvm::SmallVector<llvm::Metadata*, 8> containedTypes;

   auto Ty = ty->getRecord();
   auto loc = Ty->getSourceLoc();
   auto File = getFileDI(Ty->getSourceLoc());

   auto LineAndCol = CI.getFileMgr().getLineAndCol(loc);

   string fullName = Ty->getFullName();
   llvm::DICompositeType* forwardDecl = DI->createReplaceableCompositeType(
       getTagForRecord(Ty), fullName, ScopeStack.top(), File, LineAndCol.line);

   DITypeMap[ty] = forwardDecl;

   if (auto S = dyn_cast<StructDecl>(Ty)) {
      size_t offset = 0;

      if (isa<ClassDecl>(S)) {
#ifndef NDEBUG
         auto size = TI.getPointerSizeInBytes() * 8;
         unsigned short align = TI.getPointerAlignInBytes() * 8;

         auto& ASTCtx = CI.getContext();

         auto strongRefcount = DI->createMemberType(
             forwardDecl, "strongRefcount", File, LineAndCol.line, size, align,
             offset, flags, getTypeDI(ASTCtx.getUIntTy()));

         auto weakRefcount = DI->createMemberType(
             forwardDecl, "weakRefcount", File, LineAndCol.line, size, align,
             offset, flags, getTypeDI(ASTCtx.getUIntTy()));

         auto typeInfo = DI->createMemberType(
             forwardDecl, "typeInfo", File, LineAndCol.line, size, align,
             offset, flags, getTypeDI(ASTCtx.getUInt8PtrTy()));

         containedTypes.push_back(strongRefcount);
         containedTypes.push_back(weakRefcount);
         containedTypes.push_back(typeInfo);

#endif

         offset += 3 * TI.getPointerSizeInBytes() * 8;
      }

      std::string FieldName;
      llvm::raw_string_ostream OS(FieldName);

      for (const auto& field : S->getFields()) {
         OS << field->getDeclName();

         QualType Ty = field->getType()->getCanonicalType();

         auto size = TI.getSizeOfType(Ty) * 8;
         auto member = DI->createMemberType(
             forwardDecl, OS.str(), File, LineAndCol.line, size,
             TI.getAlignOfType(Ty) * (unsigned short)8, offset, flags,
             getTypeDI(Ty));

         offset += size;
         containedTypes.push_back(member);

         OS.str().clear();
      }

      auto contained = DI->getOrCreateArray(containedTypes);
      DI->replaceArrays(forwardDecl, contained);

      if (forwardDecl->isForwardDecl())
         forwardDecl = llvm::MDNode::replaceWithPermanent(
             llvm::TempDICompositeType(forwardDecl));
   }
   else if (auto U = dyn_cast<UnionDecl>(Ty)) {
      for (const auto& field : U->getFields()) {
         containedTypes.push_back(getTypeDI(field->getType()));
      }

      auto contained = DI->getOrCreateArray(containedTypes);
      DI->replaceArrays(forwardDecl, contained);

      if (forwardDecl->isForwardDecl())
         forwardDecl = llvm::MDNode::replaceWithPermanent(
             llvm::TempDICompositeType(forwardDecl));
   }
   else if (auto E = dyn_cast<EnumDecl>(Ty)) {
      if (E->isRawEnum()) {
         for (const auto& Case : E->getCases()) {
            containedTypes.push_back(DI->createEnumerator(
                Case->getName(),
                cast<ConstantInt>(Case->getILValue())->getZExtValue()));
         }

         DI->replaceArrays(forwardDecl, DI->getOrCreateArray(containedTypes));
      }
      else {
         std::string CurrentName;
         llvm::raw_string_ostream OS(CurrentName);

         OS << fullName << ".Discriminator";
         llvm::DICompositeType* DiscrimDecl
             = DI->createReplaceableCompositeType(
                 llvm::dwarf::DW_TAG_enumeration_type, OS.str(),
                 ScopeStack.top(), File, LineAndCol.line);

         for (const auto& Case : E->getCases()) {
            containedTypes.push_back(DI->createEnumerator(
                Case->getName(),
                cast<ConstantInt>(Case->getILValue())->getZExtValue()));
         }

         DI->replaceArrays(DiscrimDecl, DI->getOrCreateArray(containedTypes));
         if (DiscrimDecl->isForwardDecl())
            DiscrimDecl = llvm::MDNode::replaceWithPermanent(
                llvm::TempDICompositeType(DiscrimDecl));

         CurrentName.clear();
         containedTypes.clear();

         QualType RawTy = E->getRawType();
         unsigned offset = TI.getSizeOfType(RawTy) * 8;

         auto DiscrimType = DI->createMemberType(
             forwardDecl, "discriminator", File, LineAndCol.line, offset,
             TI.getAlignOfType(RawTy), 0, flags, DiscrimDecl);

         containedTypes.push_back(DiscrimType);

         for (const auto& Case : E->getCases()) {
            if (Case->getArgs().empty())
               continue;

            OS << fullName << "." << Case->getDeclName() << ".Payload";
            llvm::DICompositeType* PayloadDecl
                = DI->createReplaceableCompositeType(
                    llvm::dwarf::DW_TAG_structure_type, OS.str(),
                    ScopeStack.top(), File, LineAndCol.line);

            CurrentName.clear();

            unsigned SizeInBits = 0;
            unsigned short AlignInBits = 8;
            unsigned LocalOffset = offset;

            auto LocalDiscrim = DI->createMemberType(
                PayloadDecl, "discriminator", File, LineAndCol.line, offset,
                TI.getAlignOfType(RawTy), 0, flags, DiscrimDecl);

            llvm::SmallVector<llvm::Metadata*, 4> CaseTypes{LocalDiscrim};
            for (auto& Val : Case->getArgs()) {
               QualType Ty = Val->getType()->getCanonicalType();

               unsigned Size = TI.getSizeOfType(Ty) * 8;
               unsigned short Align = TI.getAlignOfType(Ty) * (unsigned short)8;

               OS << Val->getDeclName();
               CaseTypes.push_back(DI->createMemberType(
                   PayloadDecl, OS.str(), File, LineAndCol.line, Size, Align,
                   LocalOffset, flags, getTypeDI(Ty)));

               LocalOffset += Size;
               SizeInBits += Size;
               if (Align > AlignInBits)
                  AlignInBits = Align;

               CurrentName.clear();
            }

            DI->replaceArrays(PayloadDecl, DI->getOrCreateArray(CaseTypes));
            if (PayloadDecl->isForwardDecl())
               PayloadDecl = llvm::MDNode::replaceWithPermanent(
                   llvm::TempDICompositeType(PayloadDecl));

            llvm::DIType* PayloadTy = PayloadDecl;
            if (Case->isIndirect()) {
               PayloadTy = DI->createPointerType(
                   PayloadTy, TI.getPointerSizeInBytes() * 8,
                   TI.getPointerAlignInBytes() * (unsigned short)8);
            }

            OS << Case->getDeclName();
            auto CasePayload = DI->createMemberType(
                forwardDecl, OS.str(), File, LineAndCol.line, SizeInBits,
                AlignInBits, 0, flags, PayloadTy);

            containedTypes.push_back(CasePayload);
            CurrentName.clear();
         }

         DI->replaceArrays(forwardDecl, DI->getOrCreateArray(containedTypes));
      }

      if (forwardDecl->isForwardDecl())
         forwardDecl = llvm::MDNode::replaceWithPermanent(
             llvm::TempDICompositeType(forwardDecl));
   }
   else if (auto P = dyn_cast<ProtocolDecl>(Ty)) {
      auto contained = DI->getOrCreateArray(containedTypes);
      DI->replaceArrays(forwardDecl, contained);

      if (forwardDecl->isForwardDecl())
         forwardDecl = llvm::MDNode::replaceWithPermanent(
             llvm::TempDICompositeType(forwardDecl));
   }
   else {
      llvm_unreachable("unknown record type");
   }

   return forwardDecl;
}

void IRGen::beginScope(llvm::DIScope* scope)
{
   assert(scope);
   ScopeStack.push(scope);
}

void IRGen::endScope()
{
   assert(!ScopeStack.empty());
   ScopeStack.pop();
}

void IRGen::beginLexicalScope(const SourceLocation& loc)
{
   auto lineAndLoc = CI.getFileMgr().getLineAndCol(loc);
   auto scope = DI->createLexicalBlock(ScopeStack.top(), getFileDI(loc),
                                       lineAndLoc.line, 1);

   beginScope(scope);
}

void IRGen::emitLocalVarDI(const il::DebugLocalInst& Inst, llvm::Value* Val)
{
   auto ILVal = Inst.getVal();
   if (!Val)
      Val = getLlvmValue(ILVal);

   if (!Val) {
      ElidedDebugLocalInst = &Inst;
      return;
   }

   QualType ReferencedTy = ILVal->getType()->removeReference();
   llvm::DIType* DIType;

   if (ReferencedTy->isClass() && !isa<il::Argument>(ILVal)) {
      DIType = getTypeDI(ILVal->getType());
   }
   else {
      DIType = getTypeDI(ReferencedTy);
   }

   auto DebugLoc = Builder.getCurrentDebugLocation();
   if (!DebugLoc)
      return;

   auto DIVar = DI->createAutoVariable(ScopeStack.top(),
                                       Inst.getName()->getIdentifier(), File,
                                       DebugLoc->getLine(), DIType);

   DIVarMap[ILVal] = DIVar;
   DI->insertDeclare(Val, DIVar, DI->createExpression(), DebugLoc,
                     Builder.GetInsertBlock());
}

void IRGen::emitDebugValue(il::Value* Val, llvm::Value* LLVMVal)
{
   auto It = DIVarMap.find(Val);
   if (It == DIVarMap.end())
      return;

   auto DebugLoc = Builder.getCurrentDebugLocation();
   if (!DebugLoc)
      return;

   auto DIVar = It->getSecond();
   DI->insertDbgValueIntrinsic(LLVMVal, DIVar, DI->createExpression(), DebugLoc,
                               Builder.GetInsertBlock());
}

llvm::MDNode* IRGen::emitGlobalVarDI(GlobalVariable const& G,
                                     llvm::GlobalVariable* var)
{
   auto loc = G.getSourceLoc();
   auto File = getFileDI(loc);

   auto LineAndCol = CI.getFileMgr().getLineAndCol(loc);
   auto MD = DI->createGlobalVariableExpression(
       ScopeStack.top(), G.getName(), var->getName(), File, LineAndCol.line,
       getTypeDI(G.getType()), false);

   var->addDebugInfo(MD);
   return MD;
}

llvm::MDNode* IRGen::emitFunctionDI(il::Function const& F, llvm::Function* func)
{
   std::vector<llvm::Metadata*> argTypes;
   for (const auto& arg : F.getEntryBlock()->getArgs()) {
      argTypes.push_back(getTypeDI(arg.getType()));
   }

   llvm::DISubroutineType* funcTy
       = DI->createSubroutineType(DI->getOrCreateTypeArray(argTypes));

   unsigned int scopeStart = 0;
   auto loc = F.getSourceLoc();

   unsigned line;
   if (loc) {
      auto LineAndCol = CI.getFileMgr().getLineAndCol(loc);
      line = LineAndCol.line;
   }
   else {
      line = 0;
   }

   llvm::DISubprogram* MD;

   llvm::DINode::DIFlags Flags = llvm::DINode::FlagZero;
   llvm::DISubprogram::DISPFlags SPFlags = llvm::DISubprogram::SPFlagZero;

   if (!F.isDeclared()) {
      SPFlags |= llvm::DISubprogram::SPFlagDefinition;
   }

   auto FileDI = getFileDI(F.getSourceLoc());
   if (auto M = dyn_cast<Method>(&F)) {
      if (M->isVirtual()) {
         Flags |= llvm::DINode::FlagVirtual;
         SPFlags |= llvm::DISubprogram::SPFlagVirtual;
      }

      auto *RecordDI = getTypeDI(M->getEntryBlock()->getBlockArg(0)->getType()
                                  ->removeMetaType()->removeReference());

      MD = DI->createMethod(RecordDI,
          F.getUnmangledName(), F.getName(), FileDI, line,
          funcTy, M->getVtableOffset() == -1 ? 0 : M->getVtableOffset(), 1,
          nullptr, Flags, SPFlags);
   }
   else {
      MD = DI->createFunction(ScopeStack.top(), F.getUnmangledName(),
                              F.getName(), FileDI, line, funcTy, scopeStart,
                              Flags, SPFlags);
   }

   if (!F.isDeclared()) {
      func->setSubprogram(MD);
   }

   DIFuncMap.try_emplace((uintptr_t)&F, MD);
   return MD;
}

void IRGen::emitArgumentDI(il::Function const& F, llvm::Function* func) {}

} // namespace il
} // namespace cdot