//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_ILGENPASS_H
#define CDOT_ILGENPASS_H

#include <stack>
#include <llvm/ADT/DenseSet.h>
#include "../AbstractPass.h"
#include "../../../IL/ILBuilder.h"
#include "../../../Basic/CastKind.h"

namespace cdot {

namespace cl {

struct Method;
struct Field;
class Property;

class Record;
class Class;
class Enum;
class Struct;
class Protocol;
class Union;

} // namespace cl

namespace ctfe {

struct CTFEResult;

} // namespace ctfe

namespace ast {

class VarDecl;

class ILGenPass: public AbstractPass<ILGenPass, il::Value*> {
public:
   explicit ILGenPass(il::Context &Ctx);
   void run(std::vector<CompilationUnit> &CUs);

   void outputIL(const CompilationUnit &CU);

   il::Value *visit(Expression *expr);

   void visit(Statement *node)
   {
      AbstractPass::visit(node);
   }

   template<class T>
   auto visit(const std::shared_ptr<T> &node) -> decltype(visit(node.get()))
   {
      return visit(node.get());
   }

   ctfe::CTFEResult evaluateStaticExpr(StaticExpr *expr);

   void ForwardDeclareRecord(cl::Record* Rec);
   void GenerateTypeInfo();
   void GenerateTypeInfo(cl::Record *R, bool innerDecls = false);
   void GenerateTypeInfo(il::AggregateType *R);

   void CreateModules(std::vector<CompilationUnit> &CUs);

   void doInitialPass(CompilationUnit &CU);
   void doInitialPass(std::shared_ptr<Statement> const& statement);
   void doInitialPass(
      std::vector<std::shared_ptr<Statement>> const& statements);

   void visitCompoundStmt(CompoundStmt *node);
   void visitNamespaceDecl(NamespaceDecl *node);

   void visitLocalVarDecl(LocalVarDecl *node);
   void visitGlobalVarDecl(GlobalVarDecl *node);

   void visitFunctionDecl(FunctionDecl *node);
   void visitCallableDecl(CallableDecl *node);
   void visitDeclareStmt(DeclareStmt *node);

   void visitRecordCommon(RecordDecl *node);
   void visitRecordDecl(RecordDecl *node);
   void visitClassDecl(ClassDecl *node);
   void visitExtensionDecl(ExtensionDecl *node);
   void visitEnumDecl(EnumDecl *node);
   void visitUnionDecl(UnionDecl *node);

   void visitFieldDecl(FieldDecl *node);
   void visitPropDecl(PropDecl *node);

   void visitMethodDecl(MethodDecl *node);
   void visitConstrDecl(ConstrDecl *node);
   void visitDestrDecl(DestrDecl *node);

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
   void DeclareMethod(cl::Method *method);

   void DeclareRecord(cl::Record *R);
   void declareRecordInstantiation(cl::Record *Inst);

   void DeclareGlobalVariable(GlobalVarDecl *decl);

   struct ModuleRAII {
      ModuleRAII(ILGenPass &ILGen, cl::Record *R);
      ModuleRAII(ILGenPass &ILGen, Callable *C);
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

protected:

   void setUnmangledName(il::Function *F);

   void DeclareClassOrStruct(cl::Struct *Str);
   void DeclareEnum(cl::Enum *cl);
   void DeclareUnion(cl::Union *un);
   void DeclareProtocol(cl::Protocol *cl);
   void DeclareRecord(cl::Record *rec, il::AggregateType *Ty);

   void DeclareField(const cl::Field &field, il::AggregateType *Ty);
   void DeclareMethod(cl::Method *method, il::AggregateType *Ty);

   void DeclareProperty(const cl::Property &P, il::AggregateType *Ty);

   void DeclareValue(const string &name, il::Value *Val);
   void DeclareValue(il::Value *Val);

   void DefineProperty(const cl::Property &P, il::AggregateType *Ty);

   void DefineFunction(il::Function *F,
                       const std::shared_ptr<Statement> &body);

   void DefineGlobal(il::GlobalVariable *G,
                     std::shared_ptr<Expression> const &defaultVal,
                     size_t ordering);

   void FinalizeGlobalInitFn();

   void doDestructure(VarDecl *node);

   il::AggregateType *getType(il::Value *val);
   il::AggregateType *getType(QualType ty);
   il::AggregateType *getType(Type *ty);
   il::AggregateType *getType(cl::Record *R);

   il::Context &getContext();
   il::Module *getModule();
   il::Function *getCurrentFn();

   il::Function *getPrintf();

   il::Module *getCTFEModule();

   il::Value *getRValue(il::Value *V);
   il::Value *unboxIfNecessary(il::Value *V, bool load = true);

   il::Value *getBoxedInt(uint64_t value,
                          const string &className = "");

   il::Function *getFunc(Callable *C);
   il::Method *getFunc(cl::Method *M);

   il::Value *BoxPrimitive(il::Value *V, const string &className = "");
   il::Function *wrapNonLambdaFunction(il::Function *F);

   il::Value *getDefaultValue(Type *Ty);
   il::Value *getTuple(TupleType *Ty, llvm::ArrayRef<il::Value*> Vals);

   llvm::SmallVector<il::Argument*, 4> makeArgVec(
      const std::vector<cdot::Argument> &from);

   il::Value *VisitSubExpr(Expression *node, il::Value *Val);

   il::Value* pop();
   void push(il::Value *val);

   void maybeImportType(Type *ty);

   il::Value *castTo(il::Value *V, QualType to);
   il::Value *HandleCast(const CastResult &res, QualType to,
                         il::Value *V);

   il::Value *CreateLogicalAnd(il::Value *lhs, Expression *rhsNode);
   il::Value *CreateLogicalOr(il::Value *lhs, Expression *rhsNode);

   il::Instruction *CreateCall(Callable *C,
                               llvm::ArrayRef<il::Value*> args);

   il::Value *HandleIntrinsic(CallExpr *node);

   void retainIfNecessary(il::Value *V);
   void releaseIfNecessary(il::Value *V);

   void GenerateVTable(il::ClassType *Ty);
   void GeneratePTable(il::AggregateType *Ty);

   void FindVirtualMethods(il::ClassType *Ty, cl::Class *cl);

   il::Value *stringify(il::Value *Val);
   il::Value *getString(const llvm::Twine &str);
   il::Constant *getConstantVal(const cdot::Variant &V);

   il::Value *getCStyleArray(Type *Ty,
                             llvm::ArrayRef<il::Value*> elements);

   il::Function *getGlobalInitFn();

   il::Value *CreateEqualityComp(il::Value *lhs, il::Value *rhs);
   il::Value *CreateTupleComp(il::Value *lhs, il::Value *rhs);
   il::Value *CreateEnumComp(il::Value *lhs, il::Value *rhs);

   il::Value *box(il::Value *val);
   il::Value *unbox(il::Value *val, bool load = true);

   void deinitializeValue(il::Value *Val);

   void MaybeDefineDefaultDeinitializer(il::AggregateType *Ty);
   void DefineDefaultInitializer(il::StructType *Ty);
   void DefineMemberwiseInitializer(cl::Struct *S, il::StructType *Ty);
   void DefineImplicitEquatableConformance(cl::Method *M, cl::Record *R);
   void DefineImplicitHashableConformance(cl::Method *M, cl::Record *R);
   void DefineImplicitStringRepresentableConformance(cl::Method *M,
                                                     cl::Record *R);

   il::Value *HandleUnsafeTupleGet(il::Value *tup, il::Value *idx,
                                   TupleType *Ty);

   il::Function *getBuiltin(llvm::StringRef name);

   void CreateTypeInfo(Type *ty);
   il::GlobalVariable *GetTypeInfo(Type *ty);

   Callable *getCallable(il::Function *F);
   void collectDependencies(il::Function *fn,
                            llvm::SmallPtrSet<Callable*, 8> &deps);

   void deinitializeTemporaries();
   void deinitializeLocals();
   void declareLocal(il::Value *V);

   il::StoreInst *CreateStore(il::Value *src, il::Value *dst);

   llvm::StringMap<Callable*> FuncDeclMap;
   llvm::SmallDenseMap<Type*, il::GlobalVariable*> TypeInfoMap;

   Type *VoidTy;
   Type *Int8PtrTy;
   Type *UInt8PtrTy;
   Type *BoolTy;
   Type *DeinitializerTy;

   il::Module *CTFEMod = nullptr;
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
};

} // namespace ast
} // namespace cdot


#endif //CDOT_ILGENPASS_H
