//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_ILGENPASS_H
#define CDOT_ILGENPASS_H

#include <stack>
#include <llvm/ADT/DenseSet.h>
#include <unordered_map>

#include "AST/Passes/NullASTVisitor.h"

#include "IL/ILBuilder.h"
#include "Basic/CastKind.h"

namespace cdot {

struct Variable;

namespace ctfe {
   struct CTFEResult;
} // namespace ctfe

namespace ast {

class SemaPass;
class VarDecl;

class ILGenPass: public NullASTVisitor<il::Value*> {
public:
   explicit ILGenPass(il::Context &Ctx, SemaPass &SP);
   void run();

   void outputIL();

   il::Value *visit(Expression *expr);
   void visit(Statement *node);

   ctfe::CTFEResult evaluateStaticExpr(StaticExpr *expr);

   void ForwardDeclareRecord(RecordDecl* R);
   void GenerateTypeInfo();
   void GenerateTypeInfo(RecordDecl *R, bool innerDecls = false);
   void GenerateTypeInfo(il::AggregateType *R);

   void visitCompoundStmt(CompoundStmt *node);
   void visitNamespaceDecl(NamespaceDecl *node);

   void visitLocalVarDecl(LocalVarDecl *node);
   void visitGlobalVarDecl(GlobalVarDecl *node);

   void visitLocalDestructuringDecl(LocalDestructuringDecl *node);
   void visitGlobalDestructuringDecl(GlobalDestructuringDecl *node);

   void visitFunctionDecl(FunctionDecl *node);
   void visitCallableDecl(CallableDecl *node);

   void visitRecordCommon(RecordDecl *node);
   void visitRecordDecl(RecordDecl *node);
   void visitClassDecl(ClassDecl *node);
   void visitStructDecl(StructDecl *node);
   void visitExtensionDecl(ExtensionDecl *node);
   void visitEnumDecl(EnumDecl *node);
   void visitUnionDecl(UnionDecl *node);

   void visitFieldDecl(FieldDecl *node);
   void visitPropDecl(PropDecl *node);

   void visitMethodDecl(MethodDecl *node);
   void visitInitDecl(InitDecl *node);
   void visitDeinitDecl(DeinitDecl *node);

   void visitFuncArgDecl(FuncArgDecl *node);

   il::Value *visitIdentifierRefExpr(IdentifierRefExpr *node);
   il::Value *visitSubscriptExpr(SubscriptExpr *node);
   il::Value *visitCallExpr(CallExpr *node);
   il::Value *visitMemberRefExpr(MemberRefExpr *node);
   il::Value *visitEnumCaseExpr(EnumCaseExpr *node);

   void visitForStmt(ForStmt *node);
   void visitForInStmt(ForInStmt *node);
   void visitWhileStmt(WhileStmt *node);
   void visitIfStmt(IfStmt *node);
   void visitLabelStmt(LabelStmt *node);
   void visitGotoStmt(GotoStmt *node);

   void visitMatchStmt(MatchStmt *node);
   void HandleSwitch(MatchStmt *node);
   void HandleEqualitySwitch(MatchStmt *node);
   void HandleIntegralSwitch(MatchStmt *node,
                             const llvm::SmallVector<il::Value*, 8> &values);

   void HandlePatternSwitch(MatchStmt *node);

   void visitCaseStmt(CaseStmt *node);

   il::Value *visitExpressionPattern(ExpressionPattern *node);
   il::Value *visitCasePattern(CasePattern *node);
   il::Value *visitIsPattern(IsPattern *node);

   void visitReturnStmt(ReturnStmt *node);
   void visitBreakStmt(BreakStmt *node);
   void visitContinueStmt(ContinueStmt *node);

   il::Value *visitIntegerLiteral(IntegerLiteral *node);
   il::Value *visitFPLiteral(FPLiteral *node);
   il::Value *visitBoolLiteral(BoolLiteral *node);
   il::Value *visitCharLiteral(CharLiteral *node);

   il::Value *visitDictionaryLiteral(DictionaryLiteral *node);
   il::Value *visitArrayLiteral(ArrayLiteral *node);

   il::Value *visitNoneLiteral(NoneLiteral *node);
   il::Value *visitStringLiteral(StringLiteral *node);
   il::Value *visitStringInterpolation(StringInterpolation *node);
   il::Value *visitTupleLiteral(TupleLiteral *node);

   il::Value *visitExprSequence(ExprSequence *node);
   il::Value *visitBinaryOperator(BinaryOperator *node);
   il::Value *visitTertiaryOperator(TertiaryOperator *node);
   il::Value *visitUnaryOperator(UnaryOperator *node);

   void visitTryStmt(TryStmt *node);
   void visitThrowStmt(ThrowStmt *node);

   il::Value *visitLambdaExpr(LambdaExpr *node);
   il::Value *visitImplicitCastExpr(ImplicitCastExpr *node);
   il::Value *visitTypeRef(TypeRef *node);
   il::Value *visitLvalueToRvalue(LvalueToRvalue *node);

   void visitDebugStmt(DebugStmt *node);

   void visitStaticIfStmt(StaticIfStmt *node);
   void visitStaticAssertStmt(StaticAssertStmt *node);
   void visitStaticForStmt(StaticForStmt *node);

   il::Value* visitStaticExpr(StaticExpr *node);
   il::Value *visitTraitsExpr(TraitsExpr *node);

   void DeclareFunction(FunctionDecl *func);
   void DeclareMethod(MethodDecl *method);

   void DeclareRecord(RecordDecl *R);
   void declareRecordInstantiation(RecordDecl *Inst);

   void DeclareGlobalVariable(GlobalVarDecl *decl);
   void DeclareGlobalVariable(GlobalDestructuringDecl *decl);

   struct ModuleRAII {
      ModuleRAII(ILGenPass &ILGen, RecordDecl *R);
      ModuleRAII(ILGenPass &ILGen, CallableDecl *C);
      ModuleRAII(ILGenPass &ILGen, il::Module *M);

      ~ModuleRAII();

   private:
      ILGenPass &ILGen;
      il::Module *savedModule;
   };

   template <class T>
   il::Module *getTemplateModule(T const* t)
   {
      return Modules[t->getSourceLoc().getSourceId()];
   }

   il::Module *getFileModule(size_t id)
   {
      return Modules[id];
   }

   CallableDecl *getCallableDecl(il::Function const* F);

   struct InsertPointRAII {
      InsertPointRAII(ILGenPass &ILGen) : ILGen(ILGen),
                                          IP(ILGen.Builder.saveIP())
      {

      }

      InsertPointRAII(ILGenPass &ILGen, il::BasicBlock *IB)
         : ILGen(ILGen), IP(ILGen.Builder.saveIP())
      {
         ILGen.Builder.SetInsertPoint(IB);
      }

      InsertPointRAII(ILGenPass &ILGen, il::BasicBlock::iterator IB)
         : ILGen(ILGen), IP(ILGen.Builder.saveIP())
      {
         ILGen.Builder.SetInsertPoint(IB);
      }

      ~InsertPointRAII()
      {
         ILGen.Builder.restoreIP(IP);
      }

   private:
      ILGenPass &ILGen;
      il::ILBuilder::InsertPoint IP;
   };

   il::Function *getFunc(CallableDecl *C);
   il::Method *getFunc(MethodDecl *M);

protected:
   SemaPass &SP;

   void setUnmangledName(il::Function *F);

   void DeclareClassOrStruct(StructDecl *S);
   void DeclareEnum(EnumDecl *E);
   void DeclareUnion(UnionDecl *U);
   void DeclareProtocol(ProtocolDecl *cl);
   void DeclareRecord(RecordDecl *R, il::AggregateType *Ty);

   void DeclareField(FieldDecl *field, il::AggregateType *Ty);
   void DeclareMethod(MethodDecl *method, il::AggregateType *Ty);

   void DeclareProperty(PropDecl *P, il::AggregateType *Ty);

   void DeclareValue(const std::string &name, il::Value *Val);
   void DeclareValue(il::Value *Val);

   void DefineProperty(PropDecl *P, il::AggregateType *Ty);

   void DefineFunction(il::Function *F,
                       Statement* body);

   void DefineGlobal(il::GlobalVariable *G,
                     Expression* const &defaultVal,
                     size_t ordering);

   void FinalizeGlobalInitFn();

   void doDestructure(DestructuringDecl *node);

   il::AggregateType *getType(il::Value *val);
   il::AggregateType *getType(QualType ty);
   il::AggregateType *getType(Type *ty);
   il::AggregateType *getType(RecordDecl *R);

   il::Context &getContext();
   il::Module *getModule();
   il::Function *getCurrentFn();

   il::Function *getPrintf();

   il::Function *getCTFEFunc();

   il::Value *getRValue(il::Value *V);
   il::Value *unboxIfNecessary(il::Value *V, bool load = true);

   il::Value *getBoxedInt(uint64_t value,
                          const std::string &className = "");

   il::Value *BoxPrimitive(il::Value *V, const std::string &className = "");
   il::Function *wrapNonLambdaFunction(il::Function *F);

   il::Value *getDefaultValue(Type *Ty);
   il::Value *getTuple(TupleType *Ty, llvm::ArrayRef<il::Value*> Vals);

   il::Instruction *CreateFieldRef(il::Value *V, llvm::StringRef fieldName);

   llvm::SmallVector<il::Argument*, 4> makeArgVec(
      llvm::ArrayRef<QualType> from);

   il::Value *VisitSubExpr(Expression *node, il::Value *Val);

   il::Value* pop();
   void push(il::Value *val);

   void maybeImportType(Type *ty);

   il::Value *castTo(il::Value *V, QualType to);
   il::Value *HandleCast(const CastResult &res, QualType to,
                         il::Value *V);

   il::Value *CreateLogicalAnd(il::Value *lhs, Expression *rhsNode);
   il::Value *CreateLogicalOr(il::Value *lhs, Expression *rhsNode);

   il::Instruction *CreateCall(CallableDecl *C,
                               llvm::ArrayRef<il::Value*> args);

   il::Value *HandleIntrinsic(CallExpr *node);

   void retainIfNecessary(il::Value *V);
   void releaseIfNecessary(il::Value *V);

   void GenerateVTable(il::ClassType *Ty);
   void GeneratePTable(il::AggregateType *Ty);

   void FindVirtualMethods(il::ClassType *Ty, ClassDecl *cl);

   il::Value *stringify(il::Value *Val);
   il::Value *getString(const llvm::Twine &str);
   il::Constant *getConstantVal(Type *Ty, const cdot::Variant &V);

   il::Value *getCStyleArray(Type *Ty,
                             llvm::ArrayRef<il::Value*> elements);

   il::Function *getGlobalInitFn();

   il::Value *CreateEqualityComp(il::Value *lhs, il::Value *rhs);
   il::Value *CreateTupleComp(il::Value *lhs, il::Value *rhs);
   il::Value *CreateEnumComp(il::Value *lhs, il::Value *rhs);

   il::Value *box(il::Value *val);
   il::Value *unbox(il::Value *val, bool load = true);

   void deinitializeValue(il::Value *Val);

   void AppendDefaultDeinitializer(il::Method *M, il::AggregateType *Ty);
   void DefineDefaultInitializer(StructDecl *S, InitDecl *Init);
   void DefineMemberwiseInitializer(StructDecl *S, il::StructType *Ty);
   void DefineImplicitEquatableConformance(MethodDecl *M, RecordDecl *R);
   void DefineImplicitHashableConformance(MethodDecl *M, RecordDecl *R);
   void DefineImplicitStringRepresentableConformance(MethodDecl *M,
                                                     RecordDecl *R);

   void visitTemplateInstantiations();

   il::Value *HandleUnsafeTupleGet(il::Value *tup, il::Value *idx,
                                   TupleType *Ty);

   il::Function *getBuiltin(llvm::StringRef name);

   void CreateTypeInfo(Type *ty);
   il::GlobalVariable *GetTypeInfo(Type *ty);

   void deinitializeTemporaries();
   void deinitializeLocals();
   void declareLocal(il::Value *V);

   il::StoreInst *CreateStore(il::Value *src, il::Value *dst);

   llvm::SmallDenseMap<Type*, il::GlobalVariable*> TypeInfoMap;

   VoidType *VoidTy;
   PointerType *Int8PtrTy;
   PointerType *UInt8PtrTy;
   IntegerType *BoolTy;
   Type *DeinitializerTy;

   llvm::StringMap<llvm::StringRef> BuiltinFns;

   struct BreakContinueScope {
      il::BasicBlock *BreakTarget;
      il::BasicBlock *ContinueTarget;
   };

   std::stack<BreakContinueScope> BreakContinueStack;

   struct UnresolvedGoto {
      il::BrInst *Inst;
      llvm::StringRef labelName;
   };

   std::stack<llvm::SmallVector<UnresolvedGoto, 2>> UnresolvedGotos;
   llvm::StringMap<il::BasicBlock*> Labels;

   std::stack<il::Value*> ValueStack;
   llvm::StringMap<il::Value*> Values;
   llvm::ArrayRef<il::Value*> CurrentFuncArgs;

   llvm::SmallPtrSet<il::Value*, 8> temporaries;
   std::stack<llvm::SmallPtrSet<il::Value*, 8>> locals;

   std::unordered_map<NamedDecl*, il::Value*> DeclMap;
   std::unordered_map<il::Value*, NamedDecl*> ReverseDeclMap;

   std::unordered_map<size_t, il::BasicBlock*> GlobalInitBBs;

   il::ILBuilder Builder;

   struct EHScope {
      il::LandingPadInst *LandingPad;
   };

   bool emitDI;

   std::stack<EHScope> EHStack;
   llvm::DenseSet<uintptr_t> VisitedDecls;
   llvm::SmallDenseMap<size_t, il::Module*> Modules;

   template<class T>
   bool alreadyVisited(T *ptr)
   {
      return !VisitedDecls.insert((uintptr_t)ptr).second;
   }

   unsigned CTFEStack = 0;
   il::Function *CTFEFunc = nullptr;
   CallableDecl *circularlyDependentFunc = nullptr;
   llvm::SmallPtrSet<CallableDecl*, 8> VisitedCTFEDecls;

   struct CTFEScopeRAII {
      explicit CTFEScopeRAII(ILGenPass &ILGen)
         : ILGen(ILGen),
           encounteredCTFECircularDependence(
              ILGen.circularlyDependentFunc)
      {
         ++ILGen.CTFEStack;
      }

      ~CTFEScopeRAII()
      {
         assert(ILGen.CTFEStack && "mismatched ctfe stack");
         --ILGen.CTFEStack;
         ILGen.circularlyDependentFunc
            = encounteredCTFECircularDependence;
      }

   private:
      ILGenPass &ILGen;
      CallableDecl *encounteredCTFECircularDependence;
   };

   bool inCTFE() const
   {
      return CTFEStack != 0;
   }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_ILGENPASS_H
