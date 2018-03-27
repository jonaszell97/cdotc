//
// Created by Jonas Zell on 19.12.17.
//

#include "IRGen.h"

#include "AST/ASTContext.h"
#include "Compiler.h"
#include "Basic/FileUtils.h"
#include "Basic/FileManager.h"
#include "AST/Type.h"

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/Module.h>

using std::string;
using namespace cdot::fs;
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
                               llvm::StringRef path) {
   auto ID = fileID;
   auto it = DIFileMap.find(ID);
   if (it != DIFileMap.end())
      return it->second;

   auto File = DI->createFile(
      fileName,
      path
   );

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

   auto &FileMgr = CI.getFileMgr();
   auto fileNameAndPath = FileMgr.getFileName(ID).str();

   llvm::SmallString<64> Path;
   Path += fs::getPath(fileNameAndPath);

   fs::makeAbsolute(Path);

   auto fileName = fs::getFileNameAndExtension(fileNameAndPath);

   auto File = DI->createFile(
      fileName,
      Path.str()
   );

   DIFileMap.try_emplace(ID, File);
   return File;
}

llvm::DIType* IRGen::getTypeDI(QualType ty)
{
   using TypeID = cdot::TypeID;

   auto it = DITypeMap.find(ty);
   if (it != DITypeMap.end())
      return it->second;

   auto &TI = CI.getContext().getTargetInfo();

   llvm::DIType *MD;
   switch (ty->getTypeID()) {
   case TypeID::BuiltinTypeID: {
      switch (ty->asBuiltinType()->getKind()) {
#        define CDOT_BUILTIN_INT(Name, BW, Unsigned)              \
         case BuiltinType::Name: {                                \
            string name("int");                                   \
            if (Unsigned) name = "u" + name;                      \
            auto sign = Unsigned ? llvm::dwarf::DW_ATE_unsigned   \
                                 : llvm::dwarf::DW_ATE_signed;    \
            MD = DI->createBasicType(                             \
               name + std::to_string(BW),                         \
               BW,                                                \
               sign                                               \
            );                                                    \
            break;                                                \
         }
#        include "Basic/BuiltinTypes.def"
         case BuiltinType::f32:
            MD = DI->createBasicType(
               "float",
               32,
               llvm::dwarf::DW_ATE_float
            );
            break;
         case BuiltinType::f64:
            MD = DI->createBasicType(
               "double",
               64,
               llvm::dwarf::DW_ATE_float
            );
            break;
         default:
            llvm_unreachable("bad builtin type");
      }

      break;
   }
   case TypeID::PointerTypeID: {
      MD = DI->createPointerType(
         getTypeDI(ty->getPointeeType()),
         TI.getPointerSizeInBytes() * 8,
         TI.getPointerAlignInBytes()
      );

      break;
   }
   case TypeID::ReferenceTypeID: {
      MD = DI->createReferenceType(
         llvm::dwarf::DW_TAG_reference_type,
         getTypeDI(ty->getReferencedType()),
         TI.getPointerSizeInBytes() * 8u,
         TI.getPointerAlignInBytes()
      );

      break;
   }
   case TypeID::FunctionTypeID: {
      std::vector<llvm::Metadata*> argTypes;
      for (auto& argTy : ty->asFunctionType()->getParamTypes()) {
         argTypes.push_back(getTypeDI(*argTy));
      }

      MD = DI->createPointerType(
         DI->createSubroutineType(DI->getOrCreateTypeArray(argTypes)
         ),
         TI.getPointerSizeInBytes() * 8u,
         TI.getPointerAlignInBytes()
      );

      break;
   }
   case TypeID::LambdaTypeID:
      llvm_unreachable("TODO!");
   case TypeID::RecordTypeID: {
      MD = getRecordDI(ty);
      break;
   }
   case TypeID::MetaTypeID: {
      llvm_unreachable("should not be possible here!");
   }
   case TypeID::TupleTypeID: {
      auto tuple = ty->asTupleType();

      auto flags = llvm::DINode::DIFlags::FlagZero;
      std::vector<llvm::Metadata *> containedTypes;

      for (auto& cont : tuple->getContainedTypes()) {
         containedTypes.push_back(getTypeDI(*cont));
      }

      MD = DI->createStructType(
         ScopeStack.top(),
         "tuple",
         File,
         0,
         TI.getSizeOfType(ty) * 8u,
         TI.getAlignOfType(ty) * 8u,
         flags,
         nullptr,
         DI->getOrCreateArray(containedTypes)
      );

      break;
   }
   case TypeID::ArrayTypeID: {
      ArrayType *ArrTy = ty->asArrayType();
      MD = DI->createArrayType(
         ArrTy->getNumElements(),
         TI.getAlignOfType(ArrTy),
         getTypeDI(*ArrTy->getElementType()),
         DI->getOrCreateArray({})
      );

      break;
   }
   default:
      llvm_unreachable("should not be possible here");
   }

   DITypeMap[ty] = MD;
   return MD;
}

llvm::dwarf::Tag IRGen::getTagForRecord(AggregateType *Ty)
{
   llvm::dwarf::Tag Tag;
   switch (Ty->getTypeID()) {
      case Value::StructTypeID:
      case Value::ProtocolTypeID:
         Tag = llvm::dwarf::DW_TAG_structure_type;
         break;
      case Value::ClassTypeID:
         Tag = llvm::dwarf::DW_TAG_class_type;
         break;
      case Value::EnumTypeID:
         Tag = llvm::dwarf::DW_TAG_enumeration_type;
         break;
      case Value::UnionTypeID:
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

   llvm::DIType *MD;
   auto flags = llvm::DINode::DIFlags::FlagZero;
   llvm::SmallVector<llvm::Metadata *, 8> containedTypes;

   auto Ty = ILMod->getType(ty->getClassName());

   auto loc = Ty->getSourceLoc();
   auto File = getFileDI(Ty->getSourceLoc());

   auto LineAndCol = CI.getFileMgr().getLineAndCol(loc);

   llvm::DICompositeType *forwardDecl = DI->createReplaceableCompositeType(
      getTagForRecord(Ty),
      Ty->getName(),
      ScopeStack.top(),
      File,
      LineAndCol.line
   );

   DITypeMap[ty] = forwardDecl;

   if (auto S = dyn_cast<StructType>(Ty)) {
      size_t offset = 0;

      for (const auto &field : S->getFields()) {
         auto size = field.type->getSize() * 8;

         auto member = DI->createMemberType(
            forwardDecl,
            field.name,
            File,
            LineAndCol.line,
            size,
            field.type->getAlignment() * (unsigned short)8,
            offset,
            flags,
            getTypeDI(field.type)
         );

         offset += size;
         containedTypes.push_back(member);
      }

      auto contained = DI->getOrCreateArray(containedTypes);
      MD = DI->createClassType(
         ScopeStack.top(),
         S->getName(),
         File,
         LineAndCol.line,
         ty->getSize() * 8,
         ty->getAlignment() * (unsigned short)8,
         0,
         flags,
         nullptr,
         contained,
         nullptr,
         nullptr,
         S->getName()
      );
   }
   else if (auto U = dyn_cast<UnionType>(Ty)) {
      for (const auto &field : U->getFields()) {
         containedTypes.push_back(getTypeDI(field.type));
      }

      auto contained = DI->getOrCreateArray(containedTypes);
      MD = DI->createUnionType(
         ScopeStack.top(),
         U->getName(),
         File,
         LineAndCol.line,
         ty->getSize() * 8,
         ty->getAlignment() * (unsigned short)8,
         flags,
         contained
      );
   }
   else if (auto E = dyn_cast<EnumType>(Ty)) {
      for (const auto &Case : E->getCases()) {
         containedTypes.push_back(DI->createEnumerator(
            Case.name,
            Case.caseVal->getZExtValue()
         ));
      }

      MD = DI->createEnumerationType(
         ScopeStack.top(),
         E->getName(),
         File,
         LineAndCol.line,
         E->getRawType()->getSize() * 8,
         E->getRawType()->getAlignment() * (unsigned short)8,
         DI->getOrCreateArray(containedTypes),
         getTypeDI(E->getRawType())
      );
   }
   else if (auto P = dyn_cast<ProtocolType>(Ty)) {
//      auto vtbl = DI->createMemberType(
//         forwardDecl,
//         "vtbl",
//         File,
//         (unsigned)loc->getLine(),
//         sizeof(void*) * CHAR_BIT,
//         alignof(void*) * CHAR_BIT,
//         0,
//         flags,
//         getTypeDI(IntegerType::getCharTy()->getPointerTo())
//      );
//
//      auto obj = DI->createMemberType(
//         forwardDecl,
//         "obj",
//         File,
//         (unsigned)loc->getLine(),
//         sizeof(void*) * CHAR_BIT,
//         alignof(void*) * CHAR_BIT,
//         0,
//         flags,
//         getTypeDI(IntegerType::getCharTy()->getPointerTo())
//      );
//
//      auto size = DI->createMemberType(
//         forwardDecl,
//         "size",
//         File,
//         (unsigned)loc->getLine(),
//         sizeof(uint64_t) * CHAR_BIT,
//         alignof(uint64_t) * CHAR_BIT,
//         0,
//         flags,
//         getTypeDI(IntegerType::getUnsigned())
//      );

//      containedTypes.push_back(vtbl);
//      containedTypes.push_back(obj);
//      containedTypes.push_back(size);

      auto contained = DI->getOrCreateArray(containedTypes);
      MD = DI->createClassType(
         ScopeStack.top(),
         "cdot.Protocol",
         File,
         LineAndCol.line,
         ty->getSize() * 8,
         ty->getAlignment() * (unsigned short)8,
         0,
         flags,
         nullptr,
         contained,
         nullptr,
         nullptr,
         "cdot.Protocol"
      );
   }
   else {
      llvm_unreachable("unknown record type");
   }


   DI->replaceTemporary(llvm::TempDIType(forwardDecl), MD);
   return MD;
}

void IRGen::beginScope(llvm::DIScope *scope)
{
   assert(scope);
   ScopeStack.push(scope);
}

void IRGen::endScope()
{
   assert(!ScopeStack.empty());
   ScopeStack.pop();
}

void IRGen::beginLexicalScope(const SourceLocation &loc)
{
   auto lineAndLoc = CI.getFileMgr().getLineAndCol(loc);
   auto scope = DI->createLexicalBlock(ScopeStack.top(), getFileDI(loc),
                                       lineAndLoc.line, 1);

   beginScope(scope);
}

void IRGen::emitLocalVarDI(Instruction const &I, llvm::Value *inst)
{
   llvm::SmallVector<uint64_t, 1> expr;
   if (I.getType()->getReferencedType()->isClass())
      expr.push_back((uint64_t)llvm::dwarf::DW_OP_deref);

   auto loc = I.getSourceLoc();

   auto LineAndCol = CI.getFileMgr().getLineAndCol(loc);
   auto File = getFileDI(loc);

   DI->insertDeclare(
      inst,
      DI->createAutoVariable(
         ScopeStack.top(),
         I.getName(),
         File,
         LineAndCol.line,
         getTypeDI(I.getType()->getReferencedType())
      ),
      DI->createExpression(expr),
      Builder.getCurrentDebugLocation(),
      Builder.GetInsertBlock()
   );
}

llvm::MDNode* IRGen::emitGlobalVarDI(GlobalVariable const& G,
                                     llvm::GlobalVariable *var) {
   auto loc = G.getSourceLoc();
   auto File = getFileDI(loc);

   auto LineAndCol = CI.getFileMgr().getLineAndCol(loc);
   auto MD = DI->createGlobalVariableExpression(
      ScopeStack.top(),
      G.getName(),
      var->getName(),
      File,
      LineAndCol.line,
      getTypeDI(G.getType()),
      false
   );

   var->addDebugInfo(MD);
   return MD;
}

llvm::MDNode* IRGen::emitFunctionDI(il::Function const& F, llvm::Function *func)
{
   std::vector<llvm::Metadata*> argTypes;
   for (const auto& arg : F.getEntryBlock()->getArgs()) {
      if (arg.isVararg()) {
         break;
      }

      argTypes.push_back(getTypeDI(arg.getType()));
   }

   llvm::DISubroutineType *funcTy = DI->createSubroutineType(
      DI->getOrCreateTypeArray(argTypes)
   );

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

   llvm::DISubprogram *MD;

   auto File = getFileDI(F.getSourceLoc());
   if (auto M = dyn_cast<Method>(&F)) {
      MD = DI->createMethod(
         ScopeStack.top(),
         F.getUnmangledName(),
         F.getName(),
         File,
         line,
         funcTy,
         false,
         !F.isDeclared(),
         M->isVirtual(),
         M->getVtableOffset(),
         1,
         nullptr //FIXME
      );
   }
   else {
      MD = DI->createFunction(
         ScopeStack.top(),
         F.getUnmangledName(),
         F.getName(),
         File,
         line,
         funcTy,
         false,
         !F.isDeclared(),
         scopeStart
      );
   }

   if (!F.isDeclared()) {
      func->setSubprogram(MD);
   }

   DIFuncMap.try_emplace((uintptr_t)&F, MD);
   return MD;
}

void IRGen::emitArgumentDI(il::Function const &F, llvm::Function *func)
{
   auto arg_it = func->arg_begin();
   Builder.SetInsertPoint(&func->getEntryBlock().getInstList().front());

   uint64_t expr[] = { (uint64_t)llvm::dwarf::DW_OP_deref };

   unsigned i = 0;
   if (F.hasStructReturn()) {
      ++i;
      ++arg_it;
   }

   auto File = getFileDI(F.getSourceLoc());

   for (const auto &Arg : F.getEntryBlock()->getArgs()) {
      auto loc = F.getSourceLoc();
      if (!loc)
         continue;

      auto LineAndCol = CI.getFileMgr().getLineAndCol(loc);
      DI->createParameterVariable(
         ScopeStack.top(), Arg.getName(), i,
         File, LineAndCol.line, getTypeDI(Arg.getType())
      );

      assert(arg_it != func->arg_end());
      if (func->getBasicBlockList().size() < 2)
         continue;

      assert(!func->getEntryBlock().getInstList().empty());

      auto dl = func->getEntryBlock().getInstList().front().getDebugLoc();
      if (!dl) {
         // compiler inserted instruction
         continue;
      }

      llvm::Value *Val;
      if (!Arg.getType()->isClass()) {
         auto alloca = Builder.CreateAlloca((*arg_it).getType());
         Builder.CreateStore(&*arg_it, alloca);

         Val = alloca;
      }
      else {
         Val = &*arg_it;
      }

      DI->insertDeclare(
         Val,
         DI->createParameterVariable(
            ScopeStack.top(),
            Arg.getName(),
            i,
            File,
            LineAndCol.line,
            getTypeDI(Arg.getType())
         ),
         Arg.getType()->isRecordType() ? DI->createExpression(expr)
                                     : DI->createExpression(),
         dl,
         &func->getEntryBlock()
      );

      ++i;
      ++arg_it;
   }
}

} // namespace il
} // namespace cdot