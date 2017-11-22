//
// Created by Jonas Zell on 16.11.17.
//

#include "ILGenPass.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

#include "../ASTIncludes.h"
#include "../SemanticAnalysis/Function.h"
#include "../SemanticAnalysis/Record/Class.h"
#include "../SemanticAnalysis/Record/Enum.h"
#include "../SemanticAnalysis/Record/Union.h"

#include "../../../Support/Casting.h"

#include "../../../IL/ILBuilder.h"
#include "../../../IL/Module/Context.h"
#include "../../../IL/Module/Module.h"
#include "../../../IL/Value/Record/AggregateType.h"

#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/TupleType.h"

#define CDOT_VALUE_INCLUDE
#include "../../../IL/Value/ValueIncludes.def"


namespace cdot {
namespace ast {

using il::Value;
using il::Constant;

ILGenPass::ILGenPass(il::Context &Ctx)
   : AbstractPass(ILGenPassID),
     Builder(Ctx), Int8PtrTy(IntegerType::getCharTy()->getPointerTo()),
     BoolTy(IntegerType::getBoolTy())
{

}

void ILGenPass::returnResult(il::Value *val)
{
   ValueStack.push(val);
}

il::Value* ILGenPass::pop()
{
   if (ValueStack.empty()) {
      return nullptr;
   }

   auto top = ValueStack.top();
   ValueStack.pop();

   return top;
}

il::Value*  ILGenPass::VisitNode(AstNode *node)
{
   switch (node->getTypeID()) {
#     define CDOT_ASTNODE(Name)              \
         case AstNode::Name##ID:             \
            visit(static_cast<Name*>(node)); \
            break;
#     define CDOT_INCLUDE_ALL
#     include "../../AstNode.def"
   }

   if (ValueStack.empty()) {
      return nullptr;
   }

   auto top = ValueStack.top();
   ValueStack.pop();

   return top;
}

il::Value* ILGenPass::VisitNode(const std::shared_ptr<AstNode>& node)
{
   return VisitNode(node.get());
}

void ILGenPass::ReturnMemberExpr(Expression *node, il::Value *Val)
{
   if (node->getMemberExpr()) {
      if (node->isLvalueCast()) {
         Val = Builder.CreateLoad(Val);
      }

      returnResult(Val);
      Val = VisitNode(node->getMemberExpr());
   }

   returnResult(Val);
}

il::Context& ILGenPass::getContext()
{
   return Builder.getContext();
}

il::Module* ILGenPass::getModule()
{
   return Builder.getModule();
}

il::Function* ILGenPass::getFunction()
{
   return Builder.GetInsertBlock()->getParent();
}

il::Constant* ILGenPass::getBoxedInt(uint64_t value, const string &className)
{
   Type *Ty;
   if (className.empty()) {
      Ty = ObjectType::get("Int" + std::to_string(sizeof(int*)*8));
   }
   else {
      Ty = ObjectType::get(className);
   }

   auto Val = Builder.CreateConstantInt(Ty->unbox(), value);
   auto StructTy = getContext().getType(Ty->getClassName(), getModule());

   return Builder.CreateConstantStruct(StructTy, { Val });
}

il::Value* ILGenPass::BoxPrimitive(il::Value *V,
                                   const string &className) {
   Type *Ty;
   if (className.empty()) {
      Ty = ObjectType::get("Int" + std::to_string(sizeof(int*)*8));
   }
   else {
      Ty = ObjectType::get(className);
   }

   auto StructTy = getContext().getType(Ty->getClassName(), getModule());
   auto Init = StructTy->getMethod(Ty->getRecord()->getAs<Class>()
                                     ->getMemberwiseInitializer()
                                     ->getMangledName());

   return Builder.CreateInit(cast<il::StructType>(StructTy),
                             Init, { V });
}

void ILGenPass::maybeImportType(Type *ty)
{
   if (!ty->isObjectTy()) {
      return;
   }

   getContext().getType(ty->getClassName(), getModule());
}

void ILGenPass::run(std::vector<CompilationUnit> &CUs)
{
   llvm::SmallVector<il::Module*, 8> Modules;
   for (auto &CU : CUs) {
      auto Mod = new il::Module(Builder.getContext(), CU);
      Modules.push_back(Mod);

      Builder.SetModule(Mod);
      doInitialPass(CU.root);
   }

   for (auto &Ty : getContext().getTypes()) {
      if (auto ClassTy = dyn_cast<il::ClassType>(Ty.second)) {
         GenerateVTable(ClassTy);
      }
   }

   std::string s;
   llvm::raw_string_ostream sstream(s);

   size_t i = 0;
   for (const auto &CU : CUs) {
      Builder.SetModule(Modules[i]);
      VisitNode(CU.root);

      ++i;

      if (!CU.isHeader) {
         Builder.getModule()->writeTo(sstream);
      }
   }

   std::error_code EC;
   llvm::raw_fd_ostream fd("/Users/Jonas/CDotProjects/ex/main.cdotil",
                           EC, llvm::sys::fs::F_RW);

   fd << sstream.str();
   fd.flush();
}

void ILGenPass::doInitialPass(
   const std::vector<std::shared_ptr<Statement>> &statements) {
   for (const auto &stmt : statements) {
      doInitialPass(stmt);
   }
}

void ILGenPass::doInitialPass(
   const std::shared_ptr<Statement> &stmt) {
   switch (stmt->getTypeID()) {
      case AstNode::CompoundStmtID: {
         auto compound = std::static_pointer_cast<CompoundStmt>(stmt);
         doInitialPass(compound->getStatements());

         break;
      }
      case AstNode::DeclareStmtID: {
         auto decl = std::static_pointer_cast<DeclareStmt>(stmt);
         doInitialPass(decl->getDeclarations());

         break;
      }
      case AstNode::NamespaceDeclID: {
         auto ns = std::static_pointer_cast<NamespaceDecl>(stmt);
         doInitialPass(ns->getContents());

         break;
      }
      case AstNode::ClassDeclID: {
         auto clDecl = std::static_pointer_cast<ClassDecl>(stmt);
         DeclareClass(clDecl->getDeclaredClass());

         break;
      }
      case AstNode::EnumDeclID: {
         auto enDecl = std::static_pointer_cast<EnumDecl>(stmt);
         DeclareEnum(enDecl->getDeclaredEnum());

         break;
      }
      case AstNode::UnionDeclID: {
         auto decl = std::static_pointer_cast<UnionDecl>(stmt);
         DeclareUnion(decl->getDeclaredUnion());

         break;
      }
      case AstNode::DeclStmtID: {
         auto decl = std::static_pointer_cast<DeclStmt>(stmt);
         DeclareGlobalVariable(decl.get());

         break;
      }
      case AstNode::FunctionDeclID: {
         auto decl = std::static_pointer_cast<FunctionDecl>(stmt);
         DeclareFunction(decl.get());

         break;
      }
      default:
         return;
   }
}

void ILGenPass::DeclareGlobalVariable(DeclStmt *decl)
{
   auto glob = Builder.CreateGlobalVariable(*decl->getType()->getTypeRef(),
                                            decl->isConst(),
                                            nullptr,
                                            decl->getBinding(),
                                            decl->getSourceLoc());

   glob->setIsLvalue(true);
}

void ILGenPass::DeclareFunction(FunctionDecl *node)
{
   auto func = Builder.CreateFunction(node->getCallable());

   size_t i = 0;
   for (const auto &arg : func->getArgs()) {
      auto &name = node->getArgs()[i]->getBinding();
      if (name.empty()) {
         continue;
      }

      arg->setName(name);
      DeclareValue(arg);

      ++i;
   }
}

void ILGenPass::DefineFunction(il::Function *func,
                               const std::shared_ptr<Statement> &body) {
   func->addDefinition();
   Builder.SetInsertPoint(func->getEntryBlock());
   UnresolvedGotos.emplace();

   VisitNode(body);

   for (const auto &Goto : UnresolvedGotos.top()) {
      Goto.Inst->setTargetBranch(Labels.find(Goto.labelName)->second);
   }

   UnresolvedGotos.pop();

   if (!Builder.GetInsertBlock()->getTerminator()) {
      if (func->getName() == "main") {
         Builder.CreateRet(Builder.CreateConstantInt(IntegerType::get(),
                                                     EXIT_SUCCESS));
      }
      else if (func->getReturnType()->isVoidTy()) {
         Builder.CreateRetVoid();
      }
      else {
         Builder.CreateUnreachable();
      }
   }

   Builder.SetInsertPoint((il::BasicBlock*)nullptr);
}

void ILGenPass::DeclareValue(Value *Val)
{
   assert(Values.find(Val->getName()) == Values.end());
   Values.try_emplace(Val->getName(), Val);
}

il::CallInst* ILGenPass::CreateCall(il::Function *F,
                                    llvm::ArrayRef<il::Value *> args,
                                    il::Value *Self) {
   if (!F->mightThrow() || EHStack.empty()) {
      if (Self) {
         return Builder.CreateMethodCall(cast<il::Method>(F), Self, args);
      }

      return Builder.CreateCall(F, args);
   }

   auto &lpad = EHStack.top().LandingPad;
   auto contBB = Builder.CreateBasicBlock("try.cont");

   if (Self) {
      return Builder.CreateMethodInvoke(cast<il::Method>(F), Self, args,
                                        contBB, lpad->getParent());
   }

   auto invoke = Builder.CreateInvoke(F, args, contBB, lpad->getParent());
   Builder.SetInsertPoint(contBB);

   return invoke;
}

void ILGenPass::visit(CompoundStmt *node)
{
   auto Stmts = node->getStatements();
   auto numStmts = Stmts.size();
   size_t i = 0;

   for (const auto &stmt : Stmts) {
      VisitNode(stmt);

      // BB with no predecessors will be removed later
      if (i != numStmts - 1 && Builder.GetInsertBlock()
          && Builder.GetInsertBlock()->getTerminator()) {
         auto nextBB = Builder.CreateBasicBlock("");
         Builder.SetInsertPoint(nextBB);
      }

      ++i;
   }
}

void ILGenPass::visit(DeclStmt *node)
{
   if (!node->getValue()) {
      return;
   }

   auto val = VisitNode(node->getValue());
   if (node->isGlobal()) {
      auto glob = Builder.getModule()->getGlobal(node->getIdentifier());
      if (isa<il::Constant>(val)) {
         glob->setInitializer(cast<il::Constant>(val));
      }

      return;
   }

   if (isa<il::AllocaInst>(val)) {
      val->setName(node->getBinding());
      val->setLocation(node->getSourceLoc());
      DeclareValue(val);
      return;
   }

   auto Alloca = Builder.CreateAlloca(*val->getType(), 0,
                                      node->isCaptured(),
                                      node->getBinding(),
                                      node->getSourceLoc());

   Builder.CreateStore(val, Alloca);
   DeclareValue(Alloca);
}

void ILGenPass::visit(FunctionDecl *node)
{
   if (!node->getBody()) {
      return;
   }

   auto func = Builder.getModule()->getFunction(node->getCallable()
                                                    ->getMangledName());

   assert(func && "func should be declared");
   DefineFunction(func, node->getBody());
}

void ILGenPass::visit(CallableDecl *node) {}

void ILGenPass::visit(DeclareStmt *node) {}

void ILGenPass::visit(RecordTemplateDecl *node) {}
void ILGenPass::visit(CallableTemplateDecl *node) {}
void ILGenPass::visit(MethodTemplateDecl *node) {}

void ILGenPass::visit(IdentifierRefExpr *node)
{
   if (node->isNamespace()) {
      return returnResult(VisitNode(node->getMemberExpr()));
   }

   auto &ident = node->getBinding();
   Value *V;

   if (node->isSelf()) {
      V = getFunction()->getEntryBlock()->getBlockArg(0);
   }
   else if (node->isFunctionArg()) {
      auto fun = getFunction();
      auto idx = node->getArgNo();
      if (auto M = dyn_cast<il::Method>(fun)) {
         if (!M->isStatic()) {
            ++idx;
         }
      }

      V = getFunction()->getEntryBlock()->getBlockArg(idx);
   }
   else if (node->isFunction()) {
      auto Fun = getModule()->getFunction(node->getBinding());

      if (node->wrapLambda()) {
         V = Builder.CreateLambda(Fun, { });
      }
      else {
         V = Fun;
      }
   }
   else {
      auto it = Values.find(ident);
      if (it != Values.end()) {
         V = it->second;
      }
      else {
         auto Global = getModule()->getGlobal(ident);
         assert(Global && "unknown identifier");

         V = Global;
      }
   }

   ReturnMemberExpr(node, V);
}

void ILGenPass::visit(SubscriptExpr *node) {}

void ILGenPass::visit(CallExpr *node)
{
   llvm::SmallVector<Value*, 8> args;
   for (const auto &arg : node->getArgs()) {
      args.push_back(VisitNode(arg.second));
   }

   Value *V;
   if (node->isAnonymousCall()) {

   }
   else if (node->getType() == CallType::FUNC_CALL) {
      auto fun = getModule()->getFunction(node->getBinding());
      V = CreateCall(fun, args);
   }
   else if (node->getType() == CallType::ANON_CALL) {

   }
   else if (node->isUnionConstr()) {
      auto AggrTy = getModule()->getType(node->getIdent());
      assert(args.size() == 1);

      V = Builder.CreateUnionInit(cast<il::UnionType>(AggrTy),
                                  args.front());
   }
   else if (node->getType() == CallType::CONSTR_CALL) {
      auto method = node->getMethod();
      auto AggrTy = getContext().getType(method->owningClass->getName(),
                                         getModule());

      auto Init = getModule()->getFunction(method->getMangledName());
      assert(isa<il::Method>(Init));

      V = Builder.CreateInit(cast<il::StructType>(AggrTy),
                             cast<il::Method>(Init), args);
   }
   // enum case
   else if (node->getType() == CallType::METHOD_CALL && node->isNsMember()
            && !node->isStatic()) {
      auto EnumTy = getModule()->getType(node->getClassName());
      assert(isa<il::EnumType>(EnumTy));

      V = Builder.CreateEnumInit(cast<il::EnumType>(EnumTy),
                                 node->getIdent(), args);
   }
   else if (node->getType() == CallType::METHOD_CALL && !node->isStatic()) {
      auto target = pop();
      auto AggrTy = getContext().getType(target->getType()->getClassName(),
                                         getModule());

      auto M = AggrTy->getMethod(node->getBinding());
      V = CreateCall(M, args, target);
   }
   else if (node->getType() == CallType::METHOD_CALL) {
      auto AggrTy = getModule()->getType(node->getClassName());
      auto func = AggrTy->getMethod(node->getBinding());

      V = CreateCall(func, args);
   }
   else {
      llvm_unreachable("bad call kind");
   }

   V->setLocation(node->getSourceLoc());
   ReturnMemberExpr(node, V);
}

void ILGenPass::visit(MemberRefExpr *node)
{
   Value *V;
   if (node->isTupleAccess()) {
      V = Builder.CreateTupleExtract(pop(),
                                     node->getTupleIndex());
   }
   else if (node->isEnumCase()) {
      auto EnumTy = getModule()->getType(node->getClassName());
      assert(isa<il::EnumType>(EnumTy));

      V = Builder.CreateEnumInit(cast<il::EnumType>(EnumTy),
                                 node->getIdent(), { });
   }
   else if (node->isUnionAccess()) {
      auto UnionTy = cast<il::UnionType>(getModule()
                                            ->getType(node->getClassName()));

      V = Builder.CreateUnionCast(pop(), UnionTy, node->getIdent());
   }
   else if (node->isNsMember()) {
      V = getModule()->getGlobal(node->getBinding());
   }
   else {
      auto cl = SymbolTable::getClass(node->getClassName());
      auto ty = getModule()->getType(cl->getName());

      V = pop();
      V = Builder.CreateStructGEP(ty, V, cl->getFieldOffset(node->getIdent()));
   }

   ReturnMemberExpr(node, V);
}

void ILGenPass::visit(ForStmt *node)
{
   if (auto Init = node->getInitialization()) {
      VisitNode(Init);
   }

   auto CondBB = Builder.CreateBasicBlock("for.cond");
   auto BodyBB = Builder.CreateBasicBlock("for.body");
   auto MergeBB = Builder.CreateBasicBlock("for.merge");

   BreakContinueStack.push({ MergeBB, CondBB });

   Builder.CreateBr(CondBB);
   Builder.SetInsertPoint(CondBB);

   if (node->getTermination()) {
      auto val = VisitNode(node->getTermination());
      Builder.CreateCondBr(val, BodyBB, MergeBB);
   }
   else {
      Builder.CreateBr(BodyBB);
   }

   Builder.SetInsertPoint(BodyBB);
   if (auto Body = node->getBody()) {
      VisitNode(Body);
   }

   if (!Builder.GetInsertBlock()->getTerminator()) {
      if (auto Inc = node->getIncrement()) {
         VisitNode(Inc);
      }

      Builder.CreateBr(CondBB);
   }

   BreakContinueStack.pop();
   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visit(ForInStmt *node) {}

void ILGenPass::visit(WhileStmt *node)
{
   auto CondBB = Builder.CreateBasicBlock("while.cond");

   Builder.CreateBr(CondBB);
   Builder.SetInsertPoint(CondBB);

   auto Condition = VisitNode(node->getCondition());
   auto BodyBB = Builder.CreateBasicBlock("while.body");
   auto MergeBB = Builder.CreateBasicBlock("while.merge");

   Builder.CreateCondBr(Condition, BodyBB, MergeBB);
   Builder.SetInsertPoint(BodyBB);

   BreakContinueStack.push({ MergeBB, CondBB });

   if (auto Body = node->getBody()) {
      VisitNode(Body);
   }

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(CondBB);
   }

   BreakContinueStack.pop();
   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visit(IfStmt *node)
{
   auto Condition = VisitNode(node->getCondition());
   auto IfBranch = Builder.CreateBasicBlock("if.body");
   auto MergeBB = Builder.CreateBasicBlock("if.merge");

   il::BasicBlock *FalseBB;
   if (auto &Else = node->getElseBranch()) {
      auto Guard = Builder.MakeInsertPointGuard();
      auto ElseBranch = Builder.CreateBasicBlock("if.else");
      Builder.SetInsertPoint(ElseBranch);

      FalseBB = ElseBranch;

      VisitNode(Else);
      if (!Builder.GetInsertBlock()->getTerminator()) {
         Builder.CreateBr(MergeBB);
      }
   }
   else {
      FalseBB = MergeBB;
   }

   Builder.CreateCondBr(Condition, IfBranch, FalseBB, {}, {},
                        "", node->getSourceLoc());

   Builder.SetInsertPoint(IfBranch);
   VisitNode(node->getIfBranch());

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(MergeBB);
   }

   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visit(MatchStmt *node)
{
   auto SwitchVal = VisitNode(node->getSwitchValue());
   auto Switch = Builder.CreateSwitch(SwitchVal);
   auto MergeBB = Builder.CreateBasicBlock("switch.merge");

   llvm::SmallVector<il::BasicBlock*, 4> Cases;
   for (const auto &C : node->getCases()) {
      if (!C->getBody()) {
         Cases.push_back(nullptr);
      }
      else if (C->isIsDefault()) {
         Cases.push_back(Builder.CreateBasicBlock("switch.default"));
      }
      else {
         Cases.push_back(Builder.CreateBasicBlock("switch.case"));
      }
   }

   size_t i = 0;
   for (const auto &C : node->getCases()) {
      Value *V = nullptr;
      if (!C->isIsDefault()) {
         V = VisitNode(C->getCaseVal());
      }

      bool isDirectJump = !Cases[i];
      if (isDirectJump) {
         assert(i < Cases.size() + 1);
         Cases[i] = Cases[i + 1];
      }

      Switch->addCase(V, Cases[i]);

      if (isDirectJump) {
         ++i;
         continue;
      }

      if (i < Cases.size() - 1) {
         BreakContinueStack.push({ MergeBB, Cases[i + 1] });
      }
      else {
         BreakContinueStack.push({ MergeBB, nullptr });
      }

      Builder.SetInsertPoint(Cases[i]);

      if (auto Body = C->getBody()) {
         VisitNode(Body);

         if (!Builder.GetInsertBlock()->getTerminator()) {
            Builder.CreateBr(MergeBB);
         }
      }
      else {
         assert(i < Cases.size() - 1);
         Builder.CreateBr(Cases[i + 1]);
      }

      ++i;
   }

   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visit(CaseStmt *node) {}

void ILGenPass::visit(LabelStmt *node)
{
   auto BB = Builder.CreateBasicBlock(node->getLabelName());
   Builder.CreateBr(BB);
   Builder.SetInsertPoint(BB);

   Labels.try_emplace(node->getLabelName(), BB);
}

void ILGenPass::visit(GotoStmt *node)
{
   auto it = Labels.find(node->getLabelName());
   if (it == Labels.end()) {
      auto UnresolvedBr = Builder.CreateUnresolvedBr();

      assert(!UnresolvedGotos.empty());
      UnresolvedGotos.top().emplace_back(
         UnresolvedGoto{ UnresolvedBr, node->getLabelName() });
   }
   else {
      Builder.CreateBr(it->second);
   }
}

void ILGenPass::visit(ReturnStmt *node)
{
   if (node->getReturnValue()) {
      auto Val = VisitNode(node->getReturnValue());
      Builder.CreateRet(Val, "", node->getSourceLoc());
   }
   else {
      Builder.CreateRetVoid("", node->getSourceLoc());
   }
}

void ILGenPass::visit(BreakStmt *node)
{
   assert(!BreakContinueStack.empty() && "no target for break");
   auto &top = BreakContinueStack.top();

   assert(top.BreakTarget && "no target for break");
   Builder.CreateBr(top.BreakTarget);
}

void ILGenPass::visit(ContinueStmt *node)
{
   assert(!BreakContinueStack.empty() && "no target for continue");
   auto &top = BreakContinueStack.top();

   assert(top.ContinueTarget && "no target for continue");
   Builder.CreateBr(top.ContinueTarget);
}

void ILGenPass::visit(CollectionLiteral *node) {}
void ILGenPass::visit(IntegerLiteral *node)
{
   auto ty = node->getType();
   il::Value *val = nullptr;
   il::Constant *IntVal;

   if (ty->isIntegerTy()) {
      IntVal = Builder.CreateConstantInt(ty, node->getValue().intVal);
   }
   else {
      IntVal = Builder.CreateConstantInt(ty->unbox(),
                                         node->getValue().intVal);
   }

   if (ty->isIntegerTy()) {
      val = IntVal;
   }
   else {
      auto Ty = Builder.getContext().getType(ty->getClassName(),
                                             Builder.getModule());

      val = Builder.CreateConstantStruct(Ty, { IntVal });
   }

   returnResult(val);
}

void ILGenPass::visit(FPLiteral *node)
{
   auto ty = node->getType();
   il::Value *val = nullptr;
   il::Constant *FloatVal;

   if (ty->isBoxedPrimitive()) {
      ty = ty->unbox();
   }

   if (ty->isFloatTy()) {
      FloatVal = Builder.CreateConstantFloat((float)node->getValue().floatVal);
   }
   else {
      FloatVal = Builder.CreateConstantDouble(node->getValue().floatVal);
   }

   if (ty->isFPType()) {
      val = FloatVal;
   }
   else {
      auto Ty = Builder.getContext().getType(ty->getClassName(),
                                             Builder.getModule());

      val = Builder.CreateConstantStruct(Ty, { FloatVal });
   }

   returnResult(val);
}

void ILGenPass::visit(BoolLiteral *node)
{
   il::Constant* Val = Builder.CreateConstantInt(IntegerType::getBoolTy(),
                                                 (uint64_t)node->getValue());

   if (!node->getType()->isIntegerTy()) {
      auto Ty = Builder.getContext().getType("Bool", Builder.getModule());
      Val = Builder.CreateConstantStruct(Ty, { Val });
   }

   returnResult(Val);
}

void ILGenPass::visit(CharLiteral *node)
{
   il::Constant* Val = Builder.CreateConstantInt(IntegerType::getCharTy(),
                                                 (uint64_t)node->getNarrow());

   if (!node->getType()->isIntegerTy()) {
      auto Ty = Builder.getContext().getType("Char", Builder.getModule());
      Val = Builder.CreateConstantStruct(Ty, { Val });
   }

   returnResult(Val);
}

void ILGenPass::visit(NoneLiteral *node) {}

void ILGenPass::visit(StringLiteral *node)
{
   Value *Str = Builder.CreateConstantString(node->getValue());
   if (!node->isRaw()) {
      auto StringTy = getContext().getType("String", getModule());
      auto Len = getBoxedInt(node->getValue().length());
      auto Init = StringTy->getMethod("init5int8*5Int64");

      Str = Builder.CreateInit(cast<il::StructType>(StringTy),
                               Init, { Str, Len });
   }

   returnResult(Str);
}

void ILGenPass::visit(StringInterpolation *node)
{
   auto &Strings = node->getStrings();
   assert(!Strings.empty() && "empty string interpolation");

   auto StringTy = getModule()->getType("String");
   auto PlusEquals =
      StringTy->getMethod("infix +=19StringRepresentable");

   Value *Str = VisitNode(Strings.front());
   for (size_t i = 1; i < Strings.size(); ++i) {
      auto val = VisitNode(Strings[i]);
      Builder.CreateMethodCall(PlusEquals, Str, { val });
   }

   returnResult(Str);
}

void ILGenPass::visit(TupleLiteral *node)
{
   auto Alloc = Builder.CreateAlloca(node->getTupleType());
   size_t i = 0;

   for (const auto &El : node->getElements()) {
      auto val = VisitNode(El.second);
      auto gep = Builder.CreateGEP(Alloc, i);

      Builder.CreateStore(val, gep);
      ++i;
   }

   returnResult(Alloc);
}

#define CDOT_UNARY_OP(Name, Op)                                      \
   if (op == Op) {                                                   \
      Res = Builder.Create##Name(val, "", node->getSourceLoc());     \
   }

void ILGenPass::visit(UnaryOperator *node)
{
   auto val = VisitNode(node->getTarget());
   if (node->getOverridenCall()) {
      returnResult(val);
      return returnResult(VisitNode(node->getOverridenCall()));
   }

   auto &op = node->getOp();
   Value *Res = nullptr;

   CDOT_UNARY_OP(Min, "-")
   CDOT_UNARY_OP(Neg, "~")
   CDOT_UNARY_OP(Neg, "!")

   assert(Res && "bad unary op kind");
   ReturnMemberExpr(node, Res);
}

#undef CDOT_UNARY_OP

#define CDOT_BINARY_OP(Name, Op)                                        \
   else if (op == Op) {                                                 \
      Res = Builder.Create##Name(lhs, rhs, "", node->getSourceLoc());   \
   }

void ILGenPass::visit(BinaryOperator *node)
{
   auto lhs = VisitNode(node->getLhs());
   if (node->getOverridenCall()) {
      returnResult(lhs);
      return returnResult(VisitNode(node->getOverridenCall()));
   }

   auto &op = node->getOp();
   Value *Res = nullptr;

   if (op == "as") {
      auto rhsType = std::static_pointer_cast<TypeRef>(node->getRhs())
         ->getTypeRef();

      Res = HandleCast(*lhs->getType(), *rhsType, lhs);
      return ReturnMemberExpr(node, Res);
   }

   if (op == "&&") {
      Res = CreateLogicalAnd(lhs, node->getRhs().get());
   }
   else if (op == "||") {
      Res = CreateLogicalOr(lhs, node->getRhs().get());
   }
   else {
      auto rhs = VisitNode(node->getRhs());

      if (op == "=") {
         Res = Builder.CreateStore(rhs, lhs);
      }

      CDOT_BINARY_OP(Add, "+")
      CDOT_BINARY_OP(Sub, "-")
      CDOT_BINARY_OP(Mul, "*")
      CDOT_BINARY_OP(Div, "/")
      CDOT_BINARY_OP(Mod, "%")
      CDOT_BINARY_OP(Exp, "**")

      CDOT_BINARY_OP(And, "&")
      CDOT_BINARY_OP(Or, "|")
      CDOT_BINARY_OP(Xor, "^")
      CDOT_BINARY_OP(AShr, ">>")
      CDOT_BINARY_OP(LShr, ">>>")
      CDOT_BINARY_OP(Shl, "<<")

      CDOT_BINARY_OP(CompEQ, "==")
      CDOT_BINARY_OP(CompEQ, "===")
      CDOT_BINARY_OP(CompNE, "!=")
      CDOT_BINARY_OP(CompLE, "<=")
      CDOT_BINARY_OP(CompLT, "<")
      CDOT_BINARY_OP(CompGE, ">=")
      CDOT_BINARY_OP(CompGT, ">")
   }

   assert(Res && "bad binary op kind");

   if (node->isBoxedPrimitiveOp() && !node->getBoxedResultType().empty()) {
      Res = BoxPrimitive(Res, node->getBoxedResultType());
   }

   ReturnMemberExpr(node, Res);
}

il::Value* ILGenPass::CreateLogicalAnd(il::Value *lhs, Expression *rhsNode)
{
   auto MergeBB = Builder.CreateBasicBlock("land.merge");
   MergeBB->addBlockArg({ "res", IntegerType::get(1) });

   auto EvalRhsBB = Builder.CreateBasicBlock("land.rhs");
   Builder.CreateCondBr(lhs, EvalRhsBB, MergeBB, {}, { Builder.CreateFalse() });

   Builder.SetInsertPoint(EvalRhsBB);
   auto rhs = VisitNode(rhsNode);

   Builder.CreateBr(MergeBB, { rhs });
   Builder.SetInsertPoint(MergeBB);

   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::CreateLogicalOr(il::Value *lhs, Expression *rhsNode)
{
   auto MergeBB = Builder.CreateBasicBlock("lor.merge");
   MergeBB->addBlockArg({ "res", IntegerType::get(1) });

   auto EvalRhsBB = Builder.CreateBasicBlock("lor.rhs");
   Builder.CreateCondBr(lhs, MergeBB, EvalRhsBB, {}, { Builder.CreateTrue() });

   Builder.SetInsertPoint(EvalRhsBB);
   auto rhs = VisitNode(rhsNode);

   Builder.CreateBr(MergeBB, { rhs });
   Builder.SetInsertPoint(MergeBB);

   return MergeBB->getBlockArg(0);
}

void ILGenPass::visit(TertiaryOperator *node)
{
   auto MergeBB = Builder.CreateBasicBlock("tertiary.merge");

   auto Condition = VisitNode(node->getCondition());
   auto TrueBB = Builder.CreateBasicBlock("tertiary.lhs");
   auto FalseBB = Builder.CreateBasicBlock("tertiary.rhs");

   Builder.CreateCondBr(Condition, TrueBB, FalseBB);

   Builder.SetInsertPoint(TrueBB);
   auto lhs = VisitNode(node->getLhs());
   MergeBB->addBlockArg({ "res", *lhs->getType() });

   Builder.CreateBr(MergeBB, { lhs });

   Builder.SetInsertPoint(FalseBB);
   Builder.CreateBr(MergeBB, { VisitNode(node->getRhs()) });

   Builder.SetInsertPoint(MergeBB);
   returnResult(MergeBB->getBlockArg(0));
}

void ILGenPass::visit(FuncArgDecl *node) {}
void ILGenPass::visit(Expression *node) {}

void ILGenPass::visit(LambdaExpr *node)
{
   auto IP = Builder.saveIP();
   auto lambdaTy = node->getLambdaType();
   auto func = Builder.CreateFunction("__anonymous_lambda",
                                      node->getReturnType()->getTypeRef(),
                                      lambdaTy->getArgTypes(), false);

   llvm::SmallVector<Value*, 4> Captures;
   for (const auto &capt : node->getCaptures()) {
      auto val = Values.find(capt);
      assert(val != Values.end());
      Captures.push_back(val->second);

      func->addArgumentAtBegin(capt, val->second->getType());
   }

   DefineFunction(func, node->getBody());

   Builder.restoreIP(IP);
   returnResult(Builder.CreateLambda(func, std::move(Captures)));
}

void ILGenPass::visit(ImplicitCastExpr *node)
{
   auto Val = VisitNode(node->getTarget());
   auto &from = node->getFrom();
   auto &to = node->getTo();
   Value *Res = HandleCast(*from, *to, Val);

   ReturnMemberExpr(node, Res);
}

void ILGenPass::visit(TypedefDecl *node) {}
void ILGenPass::visit(TypeRef *node) {}

void ILGenPass::visit(LvalueToRvalue *node)
{
   auto Val = VisitNode(node->getTarget());
   auto LtoR = Builder.CreateLoad(Val, "",
                                  Val->getLocation());

   returnResult(LtoR);
}

void ILGenPass::visit(DebugStmt *node) {}

void ILGenPass::visit(TryStmt *node)
{
   if (node->getCatchBlocks().empty()) {
      VisitNode(node->getBody());
      return;
   }

   auto IP = Builder.saveIP();
   auto landBB = Builder.CreateBasicBlock("try.lpad");
   auto mergeBB = Builder.CreateBasicBlock("try.merge");

   Builder.SetInsertPoint(landBB);

   auto LPad = Builder.CreateLandingPad();
   EHStack.push(EHScope{ LPad });

   for (const auto &Catch : node->getCatchBlocks()) {
      auto BB = Builder.CreateBasicBlock("try.catch");
      BB->addBlockArg({ "", Int8PtrTy });
      LPad->addCatch({ *Catch.caughtType->getTypeRef(), BB });

      Builder.SetInsertPoint(BB);
      auto Cast = Builder.CreateExceptionCast(BB->getBlockArg(0),
                                              *Catch.caughtType->getTypeRef(),
                                              Catch.identifier);

      DeclareValue(Cast);
      VisitNode(Catch.body);

      if (!Builder.GetInsertBlock()->getTerminator()) {
         Builder.CreateBr(mergeBB);
      }
   }

   Builder.restoreIP(IP);
   VisitNode(node->getBody());

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(mergeBB);
   }

   EHStack.pop();
   Builder.SetInsertPoint(mergeBB);
}

void ILGenPass::visit(ThrowStmt *node)
{
   Builder.CreateThrow(VisitNode(node->getThrownVal()));
}

} // namespace ast
} // namespace cdot