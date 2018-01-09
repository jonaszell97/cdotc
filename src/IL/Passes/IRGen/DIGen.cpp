//
// Created by Jonas Zell on 19.12.17.
//

#include "IRGen.h"

#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/ArrayType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/GenericType.h"

#include "../../../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../../../AST/Passes/SemanticAnalysis/Record/Enum.h"
#include "../../../AST/Passes/SemanticAnalysis/Record/Union.h"

#include "../../../Files/FileUtils.h"
#include "../../../Files/FileManager.h"
#include "../../../Compiler.h"

#include <llvm/IR/DIBuilder.h>

using namespace cdot::fs;
using namespace cdot::support;

namespace cdot {
namespace il {

llvm::MDNode* IRGen::emitModuleDI()
{
   auto CU =  DI->createCompileUnit(
      llvm::dwarf::DW_LANG_C,
      File,
      "cdotc v0.1",
      false,
      "",
      0
   );

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

llvm::DIType* IRGen::getTypeDI(Type *ty)
{
   using TypeID = cdot::TypeID;

   auto it = DITypeMap.find((uintptr_t)ty);
   if (it != DITypeMap.end())
      return it->second;

   llvm::DIType *MD;
   switch (ty->getTypeID()) {
      case TypeID::IntegerTypeID: {
         auto sign = ty->isUnsigned() ? llvm::dwarf::DW_ATE_unsigned
                                      : llvm::dwarf::DW_ATE_signed;

         string name = "int";
         if (ty->isUnsigned()) {
            name = "u" + name;
         }

         MD = DI->createBasicType(
            name + std::to_string(ty->getBitwidth()),
            ty->getBitwidth(),
            sign
         );

         break;
      }
      case TypeID::FPTypeID: {
         if (ty->isFloatTy()) {
            MD = DI->createBasicType(
               "float",
               32,
               llvm::dwarf::DW_ATE_float
            );
         }
         else {
            MD = DI->createBasicType(
               "double",
               64,
               llvm::dwarf::DW_ATE_float
            );
         }

         break;
      }
      case TypeID::PointerTypeID: {
         MD = DI->createPointerType(
            getTypeDI(*ty->asPointerTy()->getPointeeType()),
            sizeof(void*) * 8,
            sizeof(void*)
         );

         break;
      }
      case TypeID::FunctionTypeID: {
         if (ty->isRawFunctionTy()) {
            std::vector<llvm::Metadata*> argTypes;
            for (auto& argTy : ty->asFunctionTy()->getArgTypes()) {
               argTypes.push_back(getTypeDI(*argTy.type));
            }

            MD = DI->createPointerType(
                  DI->createSubroutineType(
                  DI->getOrCreateTypeArray(argTypes)
               ),
               sizeof(void*) * 8,
               sizeof(void*)
            );
         }
         else {
            MD = DI->createPointerType(
               getTypeDI(IntegerType::get(8)),
               sizeof(void*) * 8,
               sizeof(void*)
            );
         }

         break;
      }
      case TypeID::ObjectTypeID: {
         MD = getRecordDI(ty);
         break;
      }
      case TypeID::MetaTypeID: {
         MD = getRecordDI(ObjectType::get("cdot.TypeInfo"));
         break;
      }
      case TypeID::TupleTypeID: {
         auto tuple = ty->asTupleTy();

         auto flags = llvm::DINode::DIFlags::FlagZero;
         std::vector<llvm::Metadata *> containedTypes;
         size_t size = 0;
         short align = 0;

         for (auto& cont : tuple->getContainedTypes()) {
            containedTypes.push_back(getTypeDI(*cont.second));
            size += cont.second->getSize();

            auto _align = cont.second->getAlignment();
            if (_align > align) {
               align = _align;
            }
         }

         MD = DI->createStructType(
            ScopeStack.top(),
            "tuple",
            File,
            0,
            size * 8,
            align * 8,
            flags,
            nullptr,
            DI->getOrCreateArray(containedTypes)
         );

         break;
      }
      case TypeID::ArrayTypeID: {
         auto ArrTy = cast<ArrayType>(ty);
         MD = DI->createArrayType(
            ArrTy->getNumElements(),
            ArrTy->getAlignment(),
            getTypeDI(ArrTy->getElementType()),
            DI->getOrCreateArray({})
         );

         break;
      }
      default:
         llvm_unreachable("should not be possible here");
   }

   DITypeMap[(uintptr_t)ty] = MD;
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

llvm::DIType* IRGen::getRecordDI(Type *ty)
{
   assert(ty->isObjectTy());

   llvm::DIType *MD;
   auto flags = llvm::DINode::DIFlags::FlagZero;
   llvm::SmallVector<llvm::Metadata *, 8> containedTypes;

   auto Ty = ILMod->getType(ty->getClassName());

   auto loc = Ty->getLocation();
   llvm::DICompositeType *forwardDecl = DI->createReplaceableCompositeType(
      getTagForRecord(Ty),
      Ty->getName(),
      ScopeStack.top(),
      File,
      (unsigned)loc->getLine()
   );

   DITypeMap[(uintptr_t)ty] = forwardDecl;

   if (auto S = dyn_cast<StructType>(Ty)) {
      size_t offset = 0;

      for (const auto &field : S->getFields()) {
         auto size = field.type->getSize() * 8;

         auto member = DI->createMemberType(
            forwardDecl,
            field.name,
            File,
            (unsigned)loc->getLine(),
            size,
            field.type->getAlignment() * (unsigned short)8,
            offset,
            flags,
            getTypeDI(*field.type)
         );

         offset += size;
         containedTypes.push_back(member);
      }

      auto contained = DI->getOrCreateArray(containedTypes);
      MD = DI->createClassType(
         ScopeStack.top(),
         S->getName(),
         File,
         (unsigned)loc->getLine(),
         S->getSize() * 8,
         S->getAlignment() * (unsigned short)8,
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
         containedTypes.push_back(getTypeDI(*field.type));
      }

      auto contained = DI->getOrCreateArray(containedTypes);
      MD = DI->createUnionType(
         ScopeStack.top(),
         U->getName(),
         File,
         (unsigned)loc->getLine(),
         U->getSize() * 8,
         U->getAlignment() * (unsigned short)8,
         flags,
         contained
      );
   }
   else if (auto E = dyn_cast<EnumType>(Ty)) {
      for (const auto &Case : E->getCases()) {
         containedTypes.push_back(DI->createEnumerator(
            Case.name,
            Case.caseVal->getU64()
         ));
      }

      MD = DI->createEnumerationType(
         ScopeStack.top(),
         E->getName(),
         File,
         (unsigned)loc->getLine(),
         E->getRawType()->getSize() * 8,
         E->getRawType()->getAlignment() * (unsigned short)8,
         DI->getOrCreateArray(containedTypes),
         getTypeDI(E->getRawType())
      );
   }
   else if (auto P = dyn_cast<ProtocolType>(Ty)) {
      return getRecordDI(ObjectType::get("cdot.Protocol"));
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
   auto lineAndLoc = fs::FileManager::getLineAndCol(loc);
   auto scope = DI->createLexicalBlock(ScopeStack.top(), File,
                                       lineAndLoc.first, 1);

   beginScope(scope);
}

void IRGen::emitLocalVarDI(Instruction const &I, llvm::Value *inst)
{
   llvm::SmallVector<uint64_t, 1> expr;
   if (I.getType()->isClass())
      expr.push_back((uint64_t)llvm::dwarf::DW_OP_deref);

   auto loc = I.getLocation();
   DI->insertDeclare(
      inst,
      DI->createAutoVariable(
         ScopeStack.top(),
         I.getName(),
         File,
         (unsigned)loc->getLine(),
         getTypeDI(*I.getType())
      ),
      DI->createExpression(expr),
      Builder.getCurrentDebugLocation(),
      Builder.GetInsertBlock()
   );
}

llvm::MDNode* IRGen::emitGlobalVarDI(GlobalVariable const& G,
                                     llvm::GlobalVariable *var) {
   auto loc = G.getLocation();
   auto MD = DI->createGlobalVariableExpression(
      ScopeStack.top(),
      G.getName(),
      var->getName(),
      File,
      loc->getLine(),
      getTypeDI(*G.getType()),
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

      argTypes.push_back(getTypeDI(*arg.getType()));
   }

   llvm::DISubroutineType *funcTy = DI->createSubroutineType(
      DI->getOrCreateTypeArray(argTypes)
   );

   unsigned int scopeStart = 0;
   auto loc = F.getLocation();
   auto line = loc ? loc->getLine() : 0;

   llvm::DISubprogram *MD;

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
         getTypeDI(ObjectType::get(M->getRecordType()->getName()))
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

   for (const auto &Arg : F.getEntryBlock()->getArgs()) {
      auto loc = F.getLocation();
      if (!loc)
         continue;

      DI->createParameterVariable(
         ScopeStack.top(), Arg.getName(), i,
         File, (unsigned)loc->getLine(), getTypeDI(*Arg.getType())
      );

      assert(arg_it != func->arg_end());
      if (func->getBasicBlockList().size() < 2)
         continue;

      auto dl = func->getEntryBlock().getInstList().front().getDebugLoc();
      assert(dl);

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
            (unsigned)loc->getLine(),
            getTypeDI(*Arg.getType())
         ),
         Arg.getType()->isObjectTy() ? DI->createExpression(expr)
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