////
//// Created by Jonas Zell on 11.07.17.
////
//
//#ifndef CDOT_CODEGENVISITOR_H
//#define CDOT_CODEGENVISITOR_H
//
//
//#include "llvm/IR/IRBuilder.h"
//
//#include <unordered_map>
//#include <stack>
//#include <set>
//
//#include "../AbstractPass.h"
//#include "../../../Variant/Variant.h"
//#include "../../../Variant/Type/PointerType.h"
//#include "../../Attribute/Attribute.h"
//#include "../../Operator/UnaryOperator.h"
//#include "../../Statement/ControlFlow/IfStmt.h"
//#include "../../Expression/Literal/LambdaExpr.h"
//
//using std::string;
//using std::pair;
//using std::unordered_map;
//using std::unique_ptr;
//
//namespace cdot {
//
//class Type;
//class GenericType;
//class PrimitiveType;
//class Builtin;
//class Callable;
//
//struct CompilationUnit;
//struct FunctionTemplateInstantiation;
//
//namespace eh {
//   class CGException;
//}
//
//namespace ast {
//class CGMemory;
//enum class BinaryOperatorType : unsigned int;
//}
//
//namespace codegen {
//   class CGCast;
//}
//
//struct Argument;
//
//struct EHFrame {
//   EHFrame(
//      llvm::BasicBlock *lpad,
//      llvm::BasicBlock *finally,
//      llvm::BasicBlock *cont,
//      llvm::AllocaInst *branchTarget,
//      std::vector<llvm::BasicBlock*> targets)
//      : landingPad(lpad), finallyBB(finally), contBB(cont),
//        branchTarget(branchTarget), targets(targets)
//   {
//
//   }
//
//   llvm::BasicBlock *landingPad;
//   llvm::BasicBlock *finallyBB;
//   llvm::BasicBlock *contBB;
//
//   llvm::AllocaInst *branchTarget;
//   std::vector<llvm::BasicBlock*> targets;
//};
//
//namespace cl {
//
//class Class;
//class Record;
//struct Method;
//struct Field;
//class Property;
//
//} // namespace cl
//
//namespace codegen {
//
//class DebugInfo;
//
//} // namespace codegen
//
//struct CompilationUnit;
//
//} // namespace cdot
//
//using namespace cdot;
//using namespace cdot::codegen;
//using namespace cdot::eh;
//using namespace cdot::cl;
//
//namespace cdot {
//namespace ast {
//
//class CodeGen: public AbstractPass<CodeGen, llvm::Value*> {
//public:
//   CodeGen();
//   ~CodeGen();
//
//   void setup(CompilationUnit &CU);
//   void save(CompilationUnit &CU);
//   void restore(CompilationUnit &CU);
//
//   void run(std::vector<CompilationUnit> &CUs);
//
//   void visitNamespaceDecl(NamespaceDecl *node) {}
//   void visitUsingStmt(UsingStmt *node) {}
//   void visitCompoundStmt(CompoundStmt *node) {}
//
//   void visitDeclStmt(LocalVarDecl *node) {}
//   void visitFunctionDecl(FunctionDecl *node) {}
//   void visitDeclareStmt(DeclareStmt *node) {}
//
//   void visitClassDecl(ClassDecl *node) {}
//   void visitEnumDecl(EnumDecl *node) {}
//   void visitUnionDecl(UnionDecl *node) {}
//
//   void visitFieldDecl(FieldDecl *node) {}
//
//   void visitMethodDecl(MethodDecl *node) {}
//   void visitInitDecl(ConstrDecl *node) {}
//   void visitDeinitDecl(DestrDecl *node) {}
//
//   llvm::Value *visitIdentifierRefExpr(IdentifierRefExpr *node) {return{};}
//   llvm::Value *visitSubscriptExpr(SubscriptExpr *node) {return{};}
//   llvm::Value *visitCallExpr(CallExpr *node) {return{};}
//   llvm::Value *visitMemberRefExpr(MemberRefExpr *node) {return{};}
//
//   void visitForStmt(ForStmt *node) {}
//   void visitForInStmt(ForInStmt *node) {}
//   void visitWhileStmt(WhileStmt *node) {}
//   void visitIfStmt(IfStmt *node) {}
//   void visitMatchStmt(MatchStmt *node) {}
//   void visitCaseStmt(CaseStmt *node) {}
//   void visitLabelStmt(LabelStmt *node) {}
//   void visitGotoStmt(GotoStmt *node) {}
//
//   void visitReturnStmt(ReturnStmt *node) {}
//   void visitBreakStmt(BreakStmt *node) {}
//   void visitContinueStmt(ContinueStmt *node) {}
//
//   llvm::Value *visitIntegerLiteral(IntegerLiteral *node) {return{};}
//   llvm::Value *visitFPLiteral(FPLiteral *node) {return{};}
//   llvm::Value *visitBoolLiteral(BoolLiteral *node) {return{};}
//   llvm::Value *visitCharLiteral(CharLiteral *node) {return{};}
//
//   llvm::Value *visitNoneLiteral(NoneLiteral *node) {return{};}
//   llvm::Value *visitStringLiteral(StringLiteral *node) {return{};}
//   llvm::Value *visitStringInterpolation(StringInterpolation *node) {return{};}
//   llvm::Value *visitTupleLiteral(TupleLiteral *node) {return{};}
//
//   llvm::Value *visitBinaryOperator(BinaryOperator *node) {return{};}
//   llvm::Value *visitTertiaryOperator(TertiaryOperator *node) {return{};}
//   llvm::Value *visitUnaryOperator(UnaryOperator *node) {return{};}
//
//   void visitFuncArgDecl(FuncArgDecl *node) {}
//   llvm::Value *visitExpression(Expression *node) {return{};}
//   llvm::Value *visitLambdaExpr(LambdaExpr *node) {return{};}
//   llvm::Value *visitImplicitCastExpr(ImplicitCastExpr *node) {return{};}
//   void visitTypedefDecl(TypedefDecl *node) {}
//   llvm::Value *visitTypeRef(TypeRef *node) {return{};}
//   llvm::Value *visitLvalueToRvalue(LvalueToRvalue *node) {return{};}
//
//   void visitTryStmt(TryStmt *node) {}
//   void visitThrowStmt(ThrowStmt *node) {}
//
//   void visitDebugStmt(DebugStmt *node) {}
//
//   void finalize(const CompilationUnit &CU);
//   static void linkAndEmit(std::vector<CompilationUnit>& CUs);
//
//   void outputIR(const CompilationUnit &CU);
//
//   void ForwardDeclareRecord(Record *cl);
//
//   void DeclareClass(Class *cl);
//   void DeclareEnum(EnumDecl*);
//   void DeclareClasses(
//      const std::vector<Statement* >&statements);
//
//   void VisitFunctionDecl(FunctionDecl *node);
//
//   unsigned short getAlignment(llvm::Value*);
//   unsigned short getAlignment(llvm::Type*);
//
//   llvm::Value* GetFieldOffset(string&, unsigned);
//   llvm::Value* GetStructSize(llvm::Type*);
//
//   llvm::Instruction *CreateCopy(llvm::Value *dst, llvm::Value *src,
//                                 Type *ty);
//
//   llvm::Value* CreateStore(llvm::Value* val, llvm::Value* ptr);
//   llvm::Value* CreateLoad(llvm::Value* ptr);
//   llvm::BasicBlock* CreateBasicBlock(string name, llvm::Function* func = nullptr);
//
//   void doProtocolCopy(llvm::Value *lhs, llvm::Value *rhs);
//
//   llvm::ConstantInt* wordSizedInt(int val);
//
//   llvm::Value* GetString(const string &str, bool cstr = false, bool isConst = false);
//
//   llvm::Value *getVariantValue(const Variant &v);
//   llvm::Value* getStaticVal(
//      Variant& v,
//      Type*& ty,
//      bool global = false
//   );
//
//   llvm::Value* getStaticVal(
//      Expression* &expr,
//      Type*& ty,
//      bool global = false
//   );
//
//   llvm::Value *valueToString(Type *ty, llvm::Value *val);
//
//   // classes
//   llvm::Value* AccessField(string ,string, llvm::Value*);
//   llvm::Value* AccessField(size_t, llvm::Value *);
//
//   void SetField(size_t, llvm::Value*, llvm::Value*, bool = false);
//   void SetField(string, string, llvm::Value*, llvm::Value*, bool = false);
//
//   llvm::Value* ExtractFromOption(llvm::Value* opt, QualType& destTy);
//
//   void DebugPrint(llvm::Value* val, string msg = "");
//
//   llvm::Value *toInt8Ptr(llvm::Value *val);
//
//   llvm::Constant* getTypeInfo(Type *ty);
//
//   llvm::Constant *createBuiltinTypeInfo(const string &name,
//                                         llvm::Constant *deinitializer,
//                                         size_t typeID);
//
//   llvm::Constant* createTupleTypeInfo(TupleType *ty);
//   llvm::Constant* createFunctionTypeInfo(FunctionType *ty);
//   llvm::Constant* createPrimitiveTypeInfo(PrimitiveType *ty);
//
//   bool addStrMetadata(llvm::Value* inst, string str);
//
//   static void declareStructTy(
//      const string &name,
//      llvm::StructType *ty
//   );
//
//   static bool hasStructTy(const string &name);
//   static llvm::StructType *getStructTy(const string &name);
//
//   friend class CGMemory;
//   friend class codegen::CGCast;
//   friend class CGException;
//   friend class SemaPass;
//   friend class DeclPass;
//
//   friend class cdot::cl::Class;
//   friend class cdot::Builtin;
//
//   static llvm::LLVMContext Context;
//
//   llvm::IRBuilder<> Builder;
//   llvm::Module *Module;
//
//   static llvm::StructType* ClassInfoType;
//   static llvm::StructType* TypeInfoType;
//   static llvm::StructType* OpaqueTy;
//   static llvm::StructType* LambdaTy;
//   static llvm::StructType* VTablePairTy;
//   static llvm::StructType* RefcountedType;
//
//   static llvm::IntegerType* WordTy;
//   static llvm::IntegerType* Int1Ty;
//   static llvm::PointerType* Int8PtrTy;
//
//   static void initGlobalTypes();
//
//   static size_t LambdaFuncPtrPos;
//   static size_t LambdaEnvPos;
//   static size_t ClassInfoSize;
//
//protected:
//   static unordered_map<string, llvm::StructType*> StructTypes;
//   static unordered_map<string, llvm::FunctionType*> FunctionTypes;
//   static unordered_map<string, llvm::Constant*> Functions;
//   static unordered_map<string, llvm::Value*> MutableValues;
//
//   unordered_map<string, llvm::Constant*> OwnFunctions;
//   unordered_map<string, llvm::Value*> OwnValues;
//   std::vector<pair<Expression*, Type*>> global_initializers;
//   std::stack<llvm::Value*> Results;
//
//   llvm::Value *getResult();
//   llvm::Value *getResult(AstNode *node);
//   llvm::Value *getResult(AstNode* node);
//   void returnResult(llvm::Value *v);
//
//   void declareFunction(
//      const string &name,
//      llvm::Function *func
//   );
//
//   llvm::Constant *getFunction(const string &name);
//   llvm::Function *getOwnDecl(const Method *method);
//
//   void declareVariable(
//      const string &name,
//      llvm::Value *var
//   );
//
//   llvm::Value *getVariable(const string &name);
//
//   std::vector<Class*> ownTypes;
//
//   llvm::Function* MALLOC;
//   llvm::Function* TERMINATE;
//   llvm::Function* FREE;
//   llvm::Function* PRINTF;
//
//   llvm::Function* ARC_INC;
//   llvm::Function* ARC_DEC;
//
//   llvm::Constant *getNative(const string &name);
//
//   static unordered_map<string, llvm::Function*> NativeFunctions;
//   static bool hasNative(const string &name);
//   static void declareNative(
//      const string &name,
//      llvm::Function *funcTy
//   );
//
//   // function stack
//   std::vector<llvm::Function*> functions;
//
//   // arc
//   std::stack<llvm::Value*> Cleanups;
//   std::stack<pair<llvm::Value*, Record*>> Temporaries;
//
//   void CleanupTemporaries();
//
//   // memory
//   ast::CGMemory *Mem;
//
//   // excn
//   CGException *Exc;
//
//   // cast
//   CGCast *Cast;
//
//   // debug info
//   bool emitDI;
//   DebugInfo *DI = nullptr;
//
//   // value stack
//   std::stack<llvm::Value*> valueStack;
//   unordered_map<string, llvm::BasicBlock*> labels;
//
//   std::stack<llvm::Value*> EnvStack;
//   std::stack<unordered_map<string, size_t>> EnvIndices;
//
//   std::stack<pair<llvm::BasicBlock*, llvm::BasicBlock*>> BreakContinueStack;
//
//   bool allBranchesTerminated = false;
//
//   void push(llvm::Value* val) {
//      valueStack.push(val);
//   }
//
//   llvm::Value* pop() {
//      auto val = valueStack.top();
//      valueStack.pop();
//
//      return val;
//   }
//
//   // function captures
//   unordered_map<string, std::vector<pair<string,string>>> function_captures;
//   bool broken = false;
//
//   // identifier expressions
//   void ReturnMemberRef(Expression*, llvm::Value*);
//
//   // functions
//   unordered_map<unsigned int, llvm::Type*> LambdaTypes = {};
//   unordered_map<string, int> LambdaEnvOrder = {};
//
//   llvm::Function *DeclareFunction(
//      const string &bound_name,
//      const std::vector<Argument> &args,
//      const std::vector<pair<string, string>> &argBindings,
//      QualType return_type,
//      bool throws,
//      bool set_this_arg = false,
//      llvm::Type *selfTy = nullptr,
//      const string &this_binding = "",
//      const std::vector<Attribute> &attrs = {},
//      bool hiddenParam = false,
//      bool envParam = false,
//      bool isVirtualOrProtocolMethod = false,
//      bool hasDefinition = true
//   );
//
//   llvm::Function *DeclareFunction(
//      const string &bound_name,
//      const std::vector<FuncArgDecl* > &args,
//      QualType return_type,
//      bool throws,
//      bool set_this_arg = false,
//      llvm::Type *selfTy = nullptr,
//      const string &this_binding = "",
//      const std::vector<Attribute> &attrs = {},
//      bool hiddenParam = false,
//      bool envParam = false,
//      bool isVirtualOrProtocolMethod = false,
//      bool hasDefinition = true
//   );
//
//   llvm::Function *DeclareFunction(
//      const string &bound_name,
//      const std::vector<Argument> &args,
//      const std::vector<pair<string, string>> &argBindings,
//      llvm::Type *return_type,
//      bool throws,
//      bool set_this_arg = false,
//      llvm::Type *selfTy = nullptr,
//      const string &this_binding = "",
//      const std::vector<Attribute> &attrs = {},
//      bool hiddenParam = false,
//      bool envParam = false,
//      bool noByVal = false,
//      bool hasDefinition = true
//   );
//
//   llvm::Function* DeclareMethod(
//      const string &bound_name,
//      const std::vector<Argument> &args,
//      const std::vector<pair<string, string>> &argBindings,
//      QualType return_type,
//      bool throws,
//      llvm::Type *selfTy,
//      const string &this_binding,
//      const std::vector<Attribute> &attrs = {},
//      bool hiddenParam = false,
//      bool isVirtualOrProtocolMethod = false,
//      bool hasDefinition = true
//   );
//
//   llvm::Function* DeclareMethod(
//      const string &bound_name,
//      const std::vector<FuncArgDecl* > &args,
//      QualType return_type,
//      bool throws,
//      llvm::Type *selfTy,
//      const string &this_binding,
//      const std::vector<Attribute> &attrs = {},
//      bool hiddenParam = false,
//      bool isVirtualOrProtocolMethod = false,
//      bool hasDefinition = true
//   );
//
//   void DefineFunction(
//      llvm::Function *func,
//      Statement* body,
//      const string &name = ""
//   );
//
//   void DefineFunction(
//      const string& bound_name,
//      Statement* body
//   );
//
//   pair<std::vector<Argument>, std::vector<pair<string, string>>>
//   getArgBindings(const std::vector<FuncArgDecl* > &args);
//
//   llvm::Function* DeclareDefaultConstructor(
//      const string &bound_name,
//      llvm::StructType *this_arg,
//      cdot::cl::Class *cl
//   );
//
//   void DefineDefaultConstructor(
//      const string &bound_name,
//      cdot::cl::Class *cl
//   );
//
//   llvm::Function* DeclareDefaultDestructor(
//      llvm::StructType* selfArg,
//      cdot::cl::Class* cl
//   );
//
//   void DefineDefaultDestructor(
//      const string &selfBinding,
//      cdot::cl::Class* cl,
//      CompoundStmt* body = nullptr
//   );
//
//   llvm::Value* DispatchProtocolCall(
//      QualType &protoTy,
//      std::vector<llvm::Value*>& args,
//      Type* returnType,
//      cl::Method* method,
//      bool skipDefaultCheck = false,
//      llvm::Value* originalSelf = nullptr,
//      llvm::Value* vMethodPair = nullptr
//   );
//
//   llvm::Value* ApplyStaticUpCast(Type *, string&, llvm::Value *);
//
//   llvm::CallInst* DispatchVirtualCall(string &className, string &methodName,
//      std::vector<llvm::Value*>& args);
//
//   llvm::Value* HandleBuiltinCall(CallExpr *node);
//
//   // declarations
//
//   void DefineClass(Class *cl);
//
//   void DeclareField(Field *field);
//   void DefineField(Field *field);
//
//   void DeclareProp(cl::Property *prop);
//   void DefineProp(cl::Property *prop);
//
//   void DeclareMethod(Method *method);
//   void DefineMethod(Method *method, CompoundStmt* body);
//
////   void DeclareConstr(Method *method);
////   void DefineConstr(Method *method, CompoundStmt* body);
//
//   void DeclareMemberwiseInitializer(cdot::cl::Class *cl);
//   void DefineMemberwiseInitializer(cdot::cl::Class *cl);
//
//   void DeclareStringRepresentableConformance(cl::Class *cl);
//   void ImplementStringRepresentableConformance(cl::Class *cl);
//
//   void DeclareEquatableConformance(cl::Class *cl);
//   void ImplementEquatableConformance(cl::Class *cl);
//
//   void DeclareHashableConformance(cl::Class *cl);
//   void ImplementHashableConformance(cl::Class *cl);
//
//   // utility
//   llvm::ConstantInt* ONE;
//   llvm::ConstantInt* ZERO;
//
//   llvm::Value* GetInteger(llvm::Value* val);
//   llvm::Value* GetInteger(long val, unsigned short bits = 64);
//
//   llvm::Value* CopyByVal(llvm::Value*);
//
//   llvm::Function* DeclareIncrementRefCount();
//   llvm::Function* DeclareDecrementRefCount();
//
//   void IncrementRefCount(llvm::Value*);
//   void DecrementRefCount(llvm::Value*);
//
//   void CreateCleanup(long count);
//
//   llvm::Instruction* CreateCall(
//      llvm::Value *func,
//      llvm::ArrayRef<llvm::Value*> args
//   );
//
//   llvm::ReturnInst *CreateRet(
//      Expression* retVal = nullptr,
//      bool sret = false,
//      bool incRefCount = false
//   );
//
//   llvm::ReturnInst *DoRet(
//      Expression* retVal = nullptr,
//      bool sret = false,
//      bool incRefCount = false
//   );
//
//   unordered_map<string, llvm::Value*> hiddenParams;
//   std::stack<llvm::Value*> HiddenParamStack;
//   std::vector<EHFrame> EHStack;
//
//   // reusable values
//   unordered_map<string, llvm::Value*> Strings;
//
//   // collections
//   llvm::Value* CreateCStyleArray(Type* type, std::vector<Expression* >& elements);
//   llvm::Value* CreateArray(RecordType* type, std::vector<Expression* >& elements);
//
//   // binary operators
//   llvm::Value* HandleBinaryOperator(llvm::Value *lhs, llvm::Value *rhs, BinaryOperatorType, BinaryOperator* node);
//   llvm::Value* HandleCastOp(llvm::Value *lhs, BinaryOperator *node);
//   llvm::Value* HandleAssignmentOp(llvm::Value*lhs, BinaryOperator *node);
//   llvm::Value* HandleArithmeticOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);
//   llvm::Value* HandleBitwiseOp(llvm::Value*lhs, llvm::Value*rhs, string& op);
//   llvm::Value* HandleLogicalOp(llvm::Value*lhs, BinaryOperator* node);
//   llvm::Value* HandleComparisonOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);
//   llvm::Value* HandleOtherOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);
//
//   // tuple comparison
//   llvm::Value* HandleTupleComp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node, bool neq = false);
//
//   // pattern matching
//   llvm::Value* CreateCompEQ(llvm::Value *&lhs, llvm::Value *&rhs, Type *&compTy,
//      llvm::Function* operatorEquals = nullptr);
//
//   // eh
//   void EmitTryNoClauses(TryStmt *node);
//   void EmitFinally(TryStmt *node);
//   void EmitCatchClauses(TryStmt *node);
//};
//
//} // namespace ast
//} // namespace cdot
//
//#endif //CDOT_CODEGENVISITOR_H
