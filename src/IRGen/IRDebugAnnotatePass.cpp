#include "cdotc/Basic/FileUtils.h"
#include "cdotc/Support/Casting.h"

#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot;
using namespace cdot::support;

namespace {

class IRDebugAnnotatePass : public llvm::ModulePass {
   std::unique_ptr<llvm::raw_fd_ostream> OS;
   llvm::StringRef OutFile;

   unsigned CurLine = 1;
   llvm::DICompileUnit* CU = nullptr;
   llvm::DIFile* DebugScope = nullptr;
   std::unique_ptr<llvm::DIBuilder> DI;
   const llvm::DataLayout* DL = nullptr;
   llvm::DenseMap<llvm::Type*, llvm::DIType*> TypeMap;
   llvm::DenseMap<uintptr_t, llvm::DISubprogram*> DIFuncMap;

   void writeModule(llvm::Module& M);

   llvm::DIType* getTypeDIImpl(llvm::Type* T);
   llvm::DIType* getTypeDI(llvm::Type* T);

   llvm::DISubprogram* annotateFunction(llvm::Function& F);
   void annotateInstruction(llvm::Instruction& I, llvm::DIScope* FnDI,
                            unsigned& i);

public:
   IRDebugAnnotatePass()
      : ModulePass(ID), OutFile(fs::getTmpFileName("ll"))
   {
      std::error_code EC;
      OS = std::make_unique<llvm::raw_fd_ostream>(OutFile, EC);

      if (EC) {
         llvm::report_fatal_error(EC.message());
      }
   }

   IRDebugAnnotatePass(std::unique_ptr<llvm::raw_fd_ostream> &&OS,
                       llvm::StringRef OutFile)
       : ModulePass(ID), OS(std::move(OS)), OutFile(OutFile)
   {
   }

   static char ID;
   bool runOnModule(llvm::Module& M) override;
};

} // anonymous namespace

void IRDebugAnnotatePass::writeModule(llvm::Module& M)
{
   llvm::AssemblyAnnotationWriter AAW;
   M.print(*OS, &AAW);
}

bool IRDebugAnnotatePass::runOnModule(llvm::Module& M)
{
   DL = &M.getDataLayout();
   DI = std::make_unique<llvm::DIBuilder>(M);
   DebugScope = DI->createFile(OutFile, fs::getPath(OutFile));

   CU = DI->createCompileUnit(llvm::dwarf::DW_LANG_C, DebugScope, "cdotc v0.1",
                              false, "", 0);

   // Every module starts with a '; Module ID:' line, followed by the
   // source file name.
   CurLine += 2;

   // Skip the data layout line, if present.
   if (!M.getDataLayoutStr().empty()) {
      ++CurLine;
   }

   // Skip the target triple, if present.
   if (!M.getTargetTriple().empty()) {
      ++CurLine;
   }

   // One empty line after the meta info.
   ++CurLine;

   // One line for each named structure type.
   CurLine += (unsigned)M.getIdentifiedStructTypes().size();

   // One empty line after the types.
   ++CurLine;

   // Start annotating globals, one on each line.
   for (auto& G : M.getGlobalList()) {
      (void)G;
      ++CurLine;
   }

   // One empty line after the globals.
   ++CurLine;

   // Now annotate functions.
   for (llvm::Function& F : M) {
      auto* FnDI = annotateFunction(F);

      // One line function header.
      ++CurLine;

      // ; Materializable
      if (F.isMaterializable())
         ++CurLine;

      const llvm::AttributeList& Attrs = F.getAttributes();
      if (Attrs.hasAttributes(llvm::AttributeList::FunctionIndex)) {
         llvm::AttributeSet AS = Attrs.getFnAttributes();
         std::string AttrStr;

         for (const llvm::Attribute& Attr : AS) {
            if (!Attr.isStringAttribute()) {
               if (!AttrStr.empty())
                  AttrStr += ' ';
               AttrStr += Attr.getAsString();
            }
         }

         if (!AttrStr.empty())
            ++CurLine;
      }

      unsigned i = 0;
      unsigned UnnamedInstCount = 0;

      if (!F.isDeclaration()) {
         for (auto& A : F.args()) {
            if (A.getName().empty())
               ++UnnamedInstCount;
         }
      }

      for (llvm::BasicBlock& B : F) {
         // One line break between basic blocks.
         if (i++ != 0)
            ++CurLine;

         // One line for the basic block name.
         ++CurLine;

         for (llvm::Instruction& I : B) {
            annotateInstruction(I, FnDI, UnnamedInstCount);
         }
      }

      // Closing brace.
      if (!F.isDeclaration()) {
         ++CurLine;
      }

      // One line break.
      ++CurLine;
   }

   for (const auto& Sub : DIFuncMap) {
      Sub.second->replaceUnit(CU);
   }

   DI->finalize();
   M.addModuleFlag(llvm::Module::Warning, "Dwarf Version",
                   llvm::dwarf::DWARF_VERSION);
   M.addModuleFlag(llvm::Module::Warning, "Debug Info Version",
                   (uint32_t)llvm::DEBUG_METADATA_VERSION);

   writeModule(M);
   return true;
}

llvm::DISubprogram* IRDebugAnnotatePass::annotateFunction(llvm::Function& F)
{
   std::vector<llvm::Metadata*> argTypes;
   for (auto& arg : F.args()) {
      argTypes.push_back(getTypeDI(arg.getType()));
   }

   llvm::DISubroutineType* funcTy
       = DI->createSubroutineType(DI->getOrCreateTypeArray(argTypes));

   unsigned int scopeStart = 0;
   llvm::DISubprogram* MD
       = DI->createFunction(CU, F.getName(), F.getName(), DebugScope, CurLine,
                            funcTy, scopeStart,
                            F.isDeclaration()
                               ? llvm::DINode::FlagFwdDecl
                               : llvm::DINode::FlagZero);

   if (!F.isDeclaration()) {
      F.setSubprogram(MD);
   }

   DIFuncMap.try_emplace((uintptr_t)&F, MD);
   return MD;
}

void IRDebugAnnotatePass::annotateInstruction(llvm::Instruction& I,
                                              llvm::DIScope* FnDI, unsigned& i)
{
   I.setDebugLoc(llvm::DebugLoc::get(CurLine, 1, FnDI));

   switch (I.getOpcode()) {
   case llvm::Instruction::Switch: {
      auto& Sw = cast<llvm::SwitchInst>(I);
      CurLine += 2; // Header, closing bracket
      CurLine += Sw.getNumCases();

      break;
   }
   case llvm::Instruction::Invoke:
      CurLine += 3; // Header, normal dest., unwind dest.
      break;
   case llvm::Instruction::Alloca:
   case llvm::Instruction::GetElementPtr: {
      std::string Name = I.getName();
      if (Name.empty()) {
         Name += "_";
         Name += std::to_string(i);
      }

      auto DIVar = DI->createAutoVariable(
          FnDI, Name, DebugScope, I.getDebugLoc()->getLine(),
          getTypeDI(I.getType()->getPointerElementType()));

      assert(I.getNextNode() && "terminator with non-void type?");
      DI->insertDeclare(&I, DIVar, DI->createExpression(), I.getDebugLoc(),
                        I.getNextNode());

      ++CurLine;

      // Don't increase the line conunt here because the dbg.declare call
      // will be visited next anyway.
      break;
   }
   default:
      ++CurLine;
      break;
   }

   if (I.getName().empty() && !I.getType()->isVoidTy())
      ++i;
}

llvm::DIType* IRDebugAnnotatePass::getTypeDI(llvm::Type* T)
{
   auto It = TypeMap.find(T);
   if (It != TypeMap.end())
      return It->getSecond();

   auto* DI = getTypeDIImpl(T);
   TypeMap[T] = DI;

   return DI;
}

llvm::DIType* IRDebugAnnotatePass::getTypeDIImpl(llvm::Type* T)
{
   switch (T->getTypeID()) {
   case llvm::Type::IntegerTyID: {
      std::string name = "i";
      name += std::to_string(T->getIntegerBitWidth());

      return DI->createBasicType(name, T->getScalarSizeInBits(),
                                 llvm::dwarf::DW_ATE_unsigned);
   }
   case llvm::Type::FloatTyID:
      return DI->createBasicType("float", T->getScalarSizeInBits(),
                                 llvm::dwarf::DW_ATE_float);
   case llvm::Type::DoubleTyID:
      return DI->createBasicType("double", T->getScalarSizeInBits(),
                                 llvm::dwarf::DW_ATE_float);
   case llvm::Type::HalfTyID:
      return DI->createBasicType("half", T->getScalarSizeInBits(),
                                 llvm::dwarf::DW_ATE_float);
   case llvm::Type::FP128TyID:
      return DI->createBasicType("fp128", T->getScalarSizeInBits(),
                                 llvm::dwarf::DW_ATE_float);
   case llvm::Type::PPC_FP128TyID:
      return DI->createBasicType("ppc_fp128", T->getScalarSizeInBits(),
                                 llvm::dwarf::DW_ATE_float);
   case llvm::Type::X86_FP80TyID:
      return DI->createBasicType("fp80", T->getScalarSizeInBits(),
                                 llvm::dwarf::DW_ATE_float);
   case llvm::Type::X86_MMXTyID:
      return DI->createBasicType("mmx", T->getScalarSizeInBits(),
                                 llvm::dwarf::DW_ATE_unsigned);
   case llvm::Type::PointerTyID:
      return DI->createPointerType(
          getTypeDI(T->getPointerElementType()),
          DL->getPointerTypeSizeInBits(T),
          DL->getPointerPrefAlignment(T->getPointerAddressSpace()).value());
   case llvm::Type::VoidTyID:
   case llvm::Type::TokenTyID:
   case llvm::Type::LabelTyID:
   case llvm::Type::MetadataTyID:
      return nullptr;
   case llvm::Type::FunctionTyID: {
      std::vector<llvm::Metadata*> argTypes;
      for (unsigned i = 0; i < T->getFunctionNumParams(); ++i) {
         argTypes.push_back(getTypeDI(T->getFunctionParamType(i)));
      }

      return DI->createPointerType(
          DI->createSubroutineType(DI->getOrCreateTypeArray(argTypes)),
          DL->getPointerSizeInBits(0), DL->getPointerPrefAlignment(0).value());
   }
   case llvm::Type::StructTyID: {
      auto* S = cast<llvm::StructType>(T);
      auto flags = llvm::DINode::DIFlags::FlagZero;
      llvm::SmallVector<llvm::Metadata*, 4> containedTypes;

      llvm::DICompositeType* forwardDecl = DI->createReplaceableCompositeType(
          llvm::dwarf::DW_TAG_structure_type, "", CU, DebugScope, 0);

      TypeMap[T] = forwardDecl;

      unsigned offset = 0;

      std::string TypeName;
      llvm::raw_string_ostream OS(TypeName);

      if (S->hasName()) {
         OS << S->getName();
      }
      else {
         OS << "<anonymous struct>";
      }

      for (unsigned i = 0; i < S->getNumContainedTypes(); ++i) {
         auto* cont = S->getContainedType(i);

         auto size = DL->getTypeSizeInBits(cont);
         auto elementDI = DI->createMemberType(
             forwardDecl, std::to_string(i++), DebugScope, 1, size,
             DL->getPrefTypeAlignment(cont), offset, flags, getTypeDI(cont));

         offset += size;
         containedTypes.push_back(elementDI);
      }

      auto* MD = DI->createStructType(
          CU, OS.str(), DebugScope, 1, DL->getTypeSizeInBits(T),
          DL->getPrefTypeAlignment(T), flags, nullptr,
          DI->getOrCreateArray(containedTypes));

      DI->replaceTemporary(llvm::TempDIType(forwardDecl), MD);
      return MD;
   }
   case llvm::Type::ArrayTyID: {
      return DI->createArrayType(
          T->getArrayNumElements(), DL->getPrefTypeAlignment(T),
          getTypeDI(T->getArrayElementType()), DI->getOrCreateArray({}));
   }
   case llvm::Type::VectorTyID: {
      return DI->createVectorType(
          T->getVectorNumElements(), DL->getPrefTypeAlignment(T),
          getTypeDI(T->getVectorElementType()), DI->getOrCreateArray({}));
   }
   }
}

char IRDebugAnnotatePass::ID = 0;

static llvm::RegisterPass<IRDebugAnnotatePass>
    X("ir-debug-info", "Annotate an LLVM module with meta debug information",
      false, false);

namespace cdot {

void addIRDebugInfo(llvm::Module& M)
{
   std::string fileName;
   auto OS = fs::openTmpFile("ll", &fileName);

   llvm::legacy::PassManager PM;
   PM.add(new IRDebugAnnotatePass(std::move(OS), fileName));
   PM.run(M);
}

void addIRDebugInfo(llvm::Module& M, llvm::StringRef FileName)
{
   std::error_code EC;
   auto OS = std::make_unique<llvm::raw_fd_ostream>(FileName, EC);
   if (EC) {
      llvm::report_fatal_error(EC.message());
   }

   llvm::legacy::PassManager PM;
   PM.add(new IRDebugAnnotatePass(std::move(OS), FileName));
   PM.run(M);
}

} // namespace cdot