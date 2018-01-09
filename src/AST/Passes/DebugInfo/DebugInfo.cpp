////
//// Created by Jonas Zell on 12.10.17.
////
//
//#include "DebugInfo.h"
//#include "../CodeGen/CodeGen.h"
//#include "../../../Files/FileUtils.h"
//#include "../../../parse/Parser.h"
//#include "../../../Compiler.h"
//
//#include "../SemanticAnalysis/Record/Class.h"
//#include "../SemanticAnalysis/Record/Enum.h"
//#include "../SemanticAnalysis/Record/Union.h"
//#include "../SemanticAnalysis/Function.h"
//
//#include "../../../Variant/Type/FunctionType.h"
//#include "../../../Variant/Type/TupleType.h"
//#include "../../../Variant/Type/ObjectType.h"
//#include "../../../Variant/Type/GenericType.h"
//#include "../../../Variant/Type/QualType.h"
//
//#include "../../Statement/Declaration/LocalVarDecl.h"
//#include "../../Statement/Declaration/Class/FieldDecl.h"
//#include "../../Statement/Declaration/Class/MethodDecl.h"
//#include "../../Statement/Block/CompoundStmt.h"
//#include "../../Expression/TypeRef.h"
//#include "../../Operator/BinaryOperator.h"
//#include "../../Expression/Literal/LambdaExpr.h"
//
//#include "llvm/IR/Module.h"
//#include "../../Statement/Declaration/CallableDecl.h"
//#include "../../../Variant/Type/IntegerType.h"
//#include "../../../Files/FileManager.h"
//
//using namespace cdot::fs;
//using namespace cdot::ast;
//
//namespace cdot {
//namespace codegen {
//
//using namespace cl;
//
//DebugInfo::DebugInfo(const CompilationUnit &CU, llvm::IRBuilder<> &Builder)
//   : DI(*CU.Module), Builder(Builder), Module(CU.Module)
//{
//   File = DI.createFile(
//      CU.fileName,
//      CU.path
//   );
//
//   ScopeStack.push((llvm::DIScope*)File);
//}
//
//llvm::MDNode* DebugInfo::emitModuleDI()
//{
//   auto CU =  DI.createCompileUnit(
//      llvm::dwarf::DW_LANG_C,
//      File,
//      "cdotc v0.1",
//      false,
//      "",
//      0
//   );
//
//   for (const auto& Sub : Subprograms) {
//      Sub.second->replaceUnit(CU);
//   }
//
//   DI.finalize();
//
//   return CU;
//}
//
//void DebugInfo::setDebugLoc(
//   const SourceLocation &loc)
//{
//   if (ScopeStack.size() == 1) {
//      return;
//   }
//
//   auto lineAndLoc = fs::FileManager::getLineAndCol(loc);
//   Builder.SetCurrentDebugLocation(llvm::DebugLoc::get(
//      lineAndLoc.first,
//      lineAndLoc.second,
//      getCurrentScope()
//   ));
//}
//
//llvm::DIFile* DebugInfo::getFileDI(const SourceLocation &loc)
//{
//   auto sourceID = loc.getSourceId();
//   auto index = FileNodes.find(sourceID);
//
//   if (index != FileNodes.end()) {
//      return (*index).second;
//   }
//
//   string fileName = fs::FileManager::getFileName(sourceID).str();
//   auto path = getPath(fileName);
//
//   return DI.createFile(
//      getFileNameAndExtension(fileName),
//      path
//   );
//}
//
//llvm::DIType* DebugInfo::getTypeDI(Type *ty)
//{
//   auto index = TypeNodes.find((uintptr_t)ty);
//   if (index != TypeNodes.end()) {
//      return (*index).second;
//   }
//
//   llvm::DIType *MD;
//   switch (ty->getTypeID()) {
//      case TypeID::IntegerTypeID: {
//         auto sign = ty->isUnsigned() ? llvm::dwarf::DW_ATE_unsigned
//                                      : llvm::dwarf::DW_ATE_signed;
//
//         string name = "int";
//         if (ty->isUnsigned()) {
//            name = "u" + name;
//         }
//
//         MD = DI.createBasicType(
//            name + std::to_string(ty->getBitwidth()),
//            ty->getBitwidth(),
//            sign
//         );
//
//         break;
//      }
//      case TypeID::FPTypeID: {
//         if (ty->isFloatTy()) {
//            MD = DI.createBasicType(
//               "float",
//               32,
//               llvm::dwarf::DW_ATE_float
//            );
//         }
//         else {
//            MD = DI.createBasicType(
//               "double",
//               64,
//               llvm::dwarf::DW_ATE_float
//            );
//         }
//
//         break;
//      }
//      case TypeID::PointerTypeID: {
//         MD = DI.createPointerType(
//            getTypeDI(*ty->asPointerTy()->getPointeeType()),
//            sizeof(void*) * 8,
//            sizeof(void*)
//         );
//
//         break;
//      }
//      case TypeID::FunctionTypeID: {
//         if (ty->isRawFunctionTy()) {
//            std::vector<llvm::Metadata*> argTypes;
//            for (auto& argTy : ty->asFunctionTy()->getArgTypes()) {
//               argTypes.push_back(getTypeDI(*argTy.type));
//            }
//
//            MD = DI.createPointerType(
//               DI.createSubroutineType(
//                  DI.getOrCreateTypeArray(argTypes)
//               ),
//               sizeof(void*) * 8,
//               sizeof(void*)
//            );
//         }
//         else {
//            MD = DI.createPointerType(
//               getTypeDI(IntegerType::get(8)),
//               sizeof(void*) * 8,
//               sizeof(void*)
//            );
//         }
//
//         break;
//      }
//      case TypeID::ObjectTypeID: {
//         MD = getRecordDI(ty);
//         break;
//      }
//      case TypeID::MetaTypeID: {
//         MD = getRecordDI(ObjectType::get("cdot.TypeInfo"));
//         break;
//      }
//      case TypeID::TupleTypeID: {
//         auto tuple = ty->asTupleTy();
//
//         auto flags = llvm::DINode::DIFlags::FlagZero;
//         std::vector<llvm::Metadata *> containedTypes;
//         size_t size = 0;
//         short align = 0;
//
//         for (auto& cont : tuple->getContainedTypes()) {
//            containedTypes.push_back(getTypeDI(*cont.second));
//            size += cont.second->getSize();
//
//            auto _align = cont.second->getAlignment();
//            if (_align > align) {
//               align = _align;
//            }
//         }
//
//         MD = DI.createStructType(
//           ScopeStack.top(),
//            "tuple",
//            File,
//            0,
//            size * 8,
//            align * 8,
//            flags,
//            nullptr,
//            DI.getOrCreateArray(containedTypes)
//         );
//
//         break;
//      }
//      case TypeID::GenericTypeID: {
//         MD = getTypeDI(ty->asGenericTy()->getActualType());
//         break;
//      }
//      default:
//         llvm_unreachable("should not be possible here");
//   }
//
//   TypeNodes[(uintptr_t)ty] = MD;
//
//   return MD;
//}
//
//llvm::dwarf::Tag DebugInfo::getTagForRecord(Record *rec)
//{
//   llvm::dwarf::Tag Tag;
//   if (rec->isStruct()) {
//      Tag = llvm::dwarf::DW_TAG_structure_type;
//   }
//   else if (rec->isUnion()) {
//      Tag = llvm::dwarf::DW_TAG_union_type;
//   }
//   else {
//      Tag = llvm::dwarf::DW_TAG_class_type;
//   }
//
//   return Tag;
//}
//
//llvm::DIType* DebugInfo::getRecordDI(Type *ty)
//{
//   assert(ty->isObjectTy());
//
//   auto index = StructTypes.find(ty->getClassName());
//   if (index != StructTypes.end()) {
//      return (*index).second;
//   }
//
//   llvm::DIType *MD;
//   auto flags = llvm::DINode::DIFlags::FlagZero;
//   std::vector<llvm::Metadata *> containedTypes;
//
//   auto rec = ty->getRecord();
//   auto fileDI = getFileDI(rec->getSourceLoc());
//
//   auto lineAndLoc = fs::FileManager::getLineAndCol(rec->getSourceLoc());
//   llvm::DICompositeType *forwardDecl = DI.createReplaceableCompositeType(
//      getTagForRecord(rec),
//      rec->getName(),
//      ScopeStack.top(),
//      fileDI,
//      lineAndLoc.first
//   );
//
//   StructTypes[ty->getClassName()] = forwardDecl;
//
//   if (rec->isClass() || rec->isStruct() || (rec->isEnum() && !rec->isRawEnum())) {
//      auto cl = rec->getAs<Class>();
//
//      size_t offset = 0;
//      for (const auto &field : cl->getFields()) {
//         auto size = field.fieldType->getSize() * 8;
//
//         auto lineAndLoc = fs::FileManager::getLineAndCol(
//            field.declaration->getSourceLoc());
//
//         auto member = DI.createMemberType(
//            forwardDecl,
//            field.fieldName,
//            fileDI,
//            lineAndLoc.first,
//            size,
//            field.fieldType->getAlignment() * 8,
//            offset,
//            flags,
//            getTypeDI(*field.fieldType)
//         );
//
//         offset += size;
//         containedTypes.push_back(std::move(member));
//      }
//
//      auto lineAndLoc = fs::FileManager::getLineAndCol(cl->getSourceLoc());
//      auto contained = DI.getOrCreateArray(containedTypes);
//      MD = DI.createClassType(
//         ScopeStack.top(),
//         cl->getName(),
//         fileDI,
//         lineAndLoc.first,
//         cl->getSize() * 8,
//         cl->getAlignment() * 8,
//         CodeGen::ClassInfoSize * 8,
//         flags,
//         nullptr,
//         contained,
//         nullptr,
//         nullptr,
//         cl->getName()
//      );
//   }
//   else if (rec->isProtocol()) {
//      MD = getRecordDI(ObjectType::get("cdot.Protocol"));
//   }
//   else if (rec->isUnion()) {
//      auto un = rec->getAs<Union>();
//
//      for (const auto &field : un->getFields()) {
//         containedTypes.push_back(getTypeDI(field.second));
//      }
//
//      auto lineAndLoc = fs::FileManager::getLineAndCol(un->getSourceLoc());
//      auto contained = DI.getOrCreateArray(containedTypes);
//      MD = DI.createUnionType(
//         ScopeStack.top(),
//         un->getName(),
//         fileDI,
//         lineAndLoc.first,
//         un->getSize() * 8,
//         un->getAlignment() * 8,
//         flags,
//         contained
//      );
//   }
//   else if (rec->isRawEnum()) {
//      auto en = rec->getAs<Enum>();
//      for (const auto &Case : en->getCases()) {
//         containedTypes.push_back(DI.createEnumerator(
//            Case.second.name,
//            Case.second.rawValue
//         ));
//      }
//
//      auto lineAndLoc = fs::FileManager::getLineAndCol(en->getSourceLoc());
//      MD = DI.createEnumerationType(
//         ScopeStack.top(),
//         en->getName(),
//         fileDI,
//         lineAndLoc.first,
//         en->getRawType()->getSize() * 8,
//         en->getRawType()->getAlignment() * 8,
//         DI.getOrCreateArray(containedTypes),
//         getTypeDI(en->getRawType())
//      );
//   }
//   else {
//      llvm_unreachable("unknown record type");
//   }
//
//   StructTypes[ty->getClassName()] = MD;
//   DI.replaceTemporary(llvm::TempDIType(forwardDecl), MD);
//
//   return MD;
//}
//
//llvm::Instruction *DebugInfo::emitLocalVarDI(
//   LocalVarDecl *node,
//   llvm::Value *alloca,
//   llvm::Instruction *inst)
//{
//   std::vector<uint64_t> expr;
//   if (node->getType()->getType()->isRefcounted()) {
//      expr.push_back((uint64_t)llvm::dwarf::DW_OP_deref);
//   }
//
//   auto lineAndLoc = fs::FileManager::getLineAndCol(node->getSourceLoc());
//   return DI.insertDeclare(
//      alloca,
//      DI.createAutoVariable(
//         getCurrentScope(),
//         node->getIdentifiers().front(),
//         getFileDI(node->getSourceLoc()),
//         lineAndLoc.first,
//         getTypeDI(*node->getType()->getType())
//      ),
//      DI.createExpression(expr),
//      Builder.getCurrentDebugLocation(),
//      inst
//   );
//}
//
//void DebugInfo::emitAssignmentDI(
//   BinaryOperator *node,
//   llvm::Value *val)
//{
//
//}
//
//llvm::MDNode* DebugInfo::emitGlobalVarDI(
//   LocalVarDecl *node,
//   llvm::GlobalVariable *var)
//{
//   auto lineAndLoc = fs::FileManager::getLineAndCol(node->getSourceLoc());
//   auto MD = DI.createGlobalVariableExpression(
//      ScopeStack.top(),
//      node->getIdentifiers().front(),
//      var->getName(),
//      File,
//      lineAndLoc.first,
//      getTypeDI(*node->getType()->getType()),
//      node->getAccess() != AccessModifier::PUBLIC
//   );
//
//   var->addDebugInfo(MD);
//   return MD;
//}
//
//void DebugInfo::emitParameterDI(
//   const std::vector<std::shared_ptr<FuncArgDecl>> &args,
//   llvm::Function *func,
//   unsigned int argBegin,
//   bool emitSelf)
//{
////      auto bb = func->getEntryBlock().getNextNode()->getNextNode();
////      auto argIt = func->arg_begin();
////
////      for (int i = 0; i < argBegin; ++i) {
////         assert(i < func->getFunctionType()->getNumParams());
////         ++argIt;
////      }
////
////      for (auto &arg : args) {
////         const auto& loc = arg->getSourceLoc();
////         if (arg->getArgType()->isCStyleVararg()) {
////            break;
////         }
////
////         auto val = &*argIt;
////         if (!val->getType()->isPointerTy()) {
////            continue;
////         }
////
////         auto param = DI.createParameterVariable(
////            func->getSubprogram(),
////            arg->getArgName(),
////            argBegin + 1,
////            File,
////            loc.getLine(),
////            getTypeDI(*arg->getArgType()->getType())
////         );
////
////         DI.insertDeclare(
////            val,
////            param,
////            DI.createExpression(),
////            llvm::DebugLoc::get(loc.getLine(), loc.getCol(), func->getSubprogram()),
////            bb
////         );
////
////         ++argIt;
////         ++argBegin;
////      }
//}
//
//llvm::MDNode* DebugInfo::emitFunctionDI(
//   FunctionDecl *node,
//   llvm::Function *func)
//{
//   std::vector<llvm::Metadata*> argTypes;
//   for (const auto& arg : node->getArgs()) {
//      if (arg->getArgType()->isCStyleVararg()) {
//         break;
//      }
//
//      argTypes.push_back(getTypeDI(*arg->getArgType()->getType()));
//   }
//
//   llvm::DISubroutineType *funcTy = DI.createSubroutineType(
//      DI.getOrCreateTypeArray(argTypes)
//   );
//
//   unsigned int scopeStart = 0;
//   auto lineAndLoc = fs::FileManager::getLineAndCol(node->getSourceLoc());
//   auto MD = DI.createFunction(
//      ScopeStack.top(),
//      node->getName(),
//      func->getName(),
//      getFileDI(node->getSourceLoc()),
//      lineAndLoc.first,
//      funcTy,
//      false,
//      node->hasDefinition(),
//      scopeStart
//   );
//
//   if (node->hasDefinition()) {
//      func->setSubprogram(MD);
//   }
//
//   Subprograms.emplace(func->getName(), MD);
//   emitParameterDI(node->getArgs(), func,
//      node->getReturnType()->getType()->needsStructReturn() ? 1 : 0);
//
//   return MD;
//}
//
//llvm::MDNode* DebugInfo::emitMethodDI(
//   Method *method, llvm::Function *func)
//{
//   auto ownFunc = Module->getFunction(func->getName());
//   if (!ownFunc || ownFunc->isDeclaration()) {
//      return nullptr;
//   }
//
//   std::vector<llvm::Metadata*> argTypes;
//   for (auto& arg : method->getArguments()) {
//      if (arg.cstyleVararg) {
//         break;
//      }
//
//      argTypes.push_back(getTypeDI(*arg.type));
//   }
//
//   llvm::DISubroutineType *funcTy = DI.createSubroutineType(
//      DI.getOrCreateTypeArray(argTypes)
//   );
//
//   auto lineAndLoc = fs::FileManager::getLineAndCol(method->getSourceLoc());
//   auto MD = DI.createMethod(
//      ScopeStack.top(),
//      method->getName(),
//      method->getLinkageName(),
//      getFileDI(method->getSourceLoc()),
//      lineAndLoc.first,
//      funcTy,
//      false,
//      method->hasDefinition,
//      method->is_virtual,
//      method->is_static ? 0 // vtable offset
//                       : (unsigned)method->owningRecord->getAs<Class>()
//                           ->getMethodOffset(method->getLinkageName()),
//      method->is_static ? 0 : 1 // 'this' offset
//   );
//
//   if (method->hasDefinition) {
//      func->setSubprogram(MD);
//   }
//
//   Subprograms.emplace(func->getName(), MD);
//
//   if (method->getDeclaration()) {
//      auto argBegin = method->is_static ? 0 : 1;
//      if (method->getReturnType()->needsStructReturn()) {
//         ++argBegin;
//      }
//
//      emitParameterDI(method->getDeclaration()->getArgs(), func, argBegin, false);
//   }
//
//   if (method->getDeclaration() && !method->is_static) {
//      auto loc = method->getDeclaration()->getSourceLoc();
//      auto lineAndLoc = fs::FileManager::getLineAndCol(loc);
//      auto param = DI.createParameterVariable(
//         MD,
//         "self",
//         1,
//         File,
//         lineAndLoc.first,
//         getTypeDI(ObjectType::get(method->owningRecord->getName()))
//      );
//
//      DI.insertDeclare(
//         &*func->arg_begin(),
//         param,
//         DI.createExpression(),
//         llvm::DebugLoc::get(lineAndLoc.first, lineAndLoc.second,
//                             func->getSubprogram()),
//         func->getEntryBlock().getNextNode()
//      );
//   }
//
//   return MD;
//}
//
//llvm::MDNode* DebugInfo::emitLambdaDI(
//   LambdaExpr *node,
//   llvm::Function *func)
//{
//   std::vector<llvm::Metadata*> argTypes;
//   for (auto& arg : node->getArgs()) {
//      if (arg->getArgType()->isCStyleVararg()) {
//         break;
//      }
//
//      argTypes.push_back(getTypeDI(*arg->getArgType()->getType()));
//   }
//
//   llvm::DISubroutineType *funcTy = DI.createSubroutineType(
//      DI.getOrCreateTypeArray(argTypes)
//   );
//
//   auto lineAndLoc = fs::FileManager::getLineAndCol(node->getSourceLoc());
//   auto scopeStart = lineAndLoc.first;
//   auto MD = DI.createFunction(
//      ScopeStack.top(),
//      func->getName(),
//      func->getName(),
//      getFileDI(node->getSourceLoc()),
//      lineAndLoc.first,
//      funcTy,
//      false,
//      true,
//      scopeStart
//   );
//
//   func->setSubprogram(MD);
//   Subprograms.emplace(func->getName(), MD);
//
//   return MD;
//}
//
//llvm::DIScope* DebugInfo::getFunctionScope(llvm::StringRef funcName)
//{
//   assert(Subprograms.find(funcName) != Subprograms.end());
//   return Subprograms[funcName];
//}
//
//void DebugInfo::beginScope(llvm::DIScope *scope)
//{
//   assert(scope);
//   ScopeStack.push(scope);
//}
//
//void DebugInfo::endScope()
//{
//   assert(ScopeStack.size() > 1);
//   ScopeStack.pop();
//}
//
//void DebugInfo::beginLexicalScope(const SourceLocation &loc)
//{
//   auto lineAndLoc = fs::FileManager::getLineAndCol(loc);
//   auto scope = DI.createLexicalBlock(getCurrentScope(), File,
//                                      lineAndLoc.first, 1);
//   beginScope(scope);
//}
//
//void DebugInfo::beginGeneratedFunctionScope(
//   llvm::StringRef funcName,
//   const std::vector<Type*> &args)
//{
//   std::vector<llvm::Metadata*> argTypes;
//   for (auto& arg : args) {
//      argTypes.push_back(getTypeDI(arg));
//   }
//
//   llvm::DISubroutineType *funcTy = DI.createSubroutineType(
//      DI.getOrCreateTypeArray(argTypes)
//   );
//
//   auto scope = DI.createFunction(
//      ScopeStack.top(),
//      funcName,
//      funcName,
//      File,
//      0,
//      funcTy,
//      false,
//      true,
//      0
//   );
//
//   Subprograms.emplace(funcName, scope);
//
//   beginScope(scope);
//}
//}
//}