//
// Created by Jonas Zell on 11.07.17.
//

#ifndef CDOT_CODEGENVISITOR_H
#define CDOT_CODEGENVISITOR_H


#include "llvm/IR/IRBuilder.h"

#include <unordered_map>
#include <stack>

#include "../AbstractPass.h"
#include "../../../Variant/Variant.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../Attribute/Attribute.h"

class FuncArgDecl;
class CompoundStmt;
class FieldDecl;
class Statement;
class CGMemory;

using std::string;
using std::pair;
using std::unordered_map;
using std::unique_ptr;

namespace cdot {
   class BuiltinType;
   class GenericType;
   class Builtin;
   class BinaryOperator;
   struct CompilationUnit;

   namespace eh {
      class CGException;
   }

   namespace codegen {
      class CGCast;
   }

   enum class BinaryOperatorType : unsigned int;

   struct Argument;

   struct EHFrame {
      EHFrame(
         llvm::BasicBlock *lpad,
         llvm::BasicBlock *finally,
         llvm::BasicBlock *cont,
         llvm::AllocaInst *branchTarget,
         std::vector<llvm::BasicBlock*> targets)
         : landingPad(lpad), finallyBB(finally), contBB(cont),
           branchTarget(branchTarget), targets(targets)
      {

      }

      llvm::BasicBlock *landingPad;
      llvm::BasicBlock *finallyBB;
      llvm::BasicBlock *contBB;

      llvm::AllocaInst *branchTarget;
      std::vector<llvm::BasicBlock*> targets;
   };

   namespace cl {
      class Class;
      class Record;
      struct Method;
   }

   namespace codegen {
      class DebugInfo;
   }
}

using namespace cdot;
using namespace cdot::codegen;
using namespace cdot::eh;
using namespace cdot::cl;

class CodeGen {
public:
   CodeGen(
      const string &fileName,
      const string &path,
      size_t compUnitID,
      bool isHeader
   );

   ~CodeGen();

   virtual llvm::Value* visit(NamespaceDecl *node);
   virtual llvm::Value* visit(UsingStmt *node);
   virtual llvm::Value* visit(CompoundStmt *node);

   virtual llvm::Value* visit(DeclStmt *node);
   virtual llvm::Value* visit(FunctionDecl *node);
   virtual llvm::Value* visit(DeclareStmt *node);

   virtual llvm::Value* visit(ClassDecl *node);
   virtual llvm::Value* visit(MethodDecl *node);
   virtual llvm::Value* visit(FieldDecl *node);
   virtual llvm::Value* visit(ConstrDecl *node);
   virtual llvm::Value* visit(DestrDecl *node);
   virtual llvm::Value* visit(EnumDecl *node);
   virtual llvm::Value* visit(UnionDecl *node);

   virtual llvm::Value* visit(IdentifierRefExpr *node);
   virtual llvm::Value* visit(SubscriptExpr *node);
   virtual llvm::Value* visit(CallExpr *node);
   virtual llvm::Value* visit(MemberRefExpr *node);

   virtual llvm::Value* visit(ForStmt *node);
   virtual llvm::Value* visit(ForInStmt *node);
   virtual llvm::Value* visit(WhileStmt *node);
   virtual llvm::Value* visit(IfStmt *node);
   virtual llvm::Value* visit(MatchStmt *node);
   virtual llvm::Value* visit(CaseStmt *node);
   virtual llvm::Value* visit(LabelStmt *node);
   virtual llvm::Value* visit(GotoStmt *node);

   virtual llvm::Value* visit(ReturnStmt *node);
   virtual llvm::Value* visit(BreakStmt *node);
   virtual llvm::Value* visit(ContinueStmt *node);

   virtual llvm::Value* visit(CollectionLiteral *node);
   virtual llvm::Value* visit(NumericLiteral *node);
   virtual llvm::Value* visit(NoneLiteral *node);
   virtual llvm::Value* visit(StringLiteral *node);
   virtual llvm::Value* visit(StringInterpolation *node);
   virtual llvm::Value* visit(TupleLiteral *node);

   virtual llvm::Value* visit(BinaryOperator *node);
   virtual llvm::Value* visit(TertiaryOperator *node);
   virtual llvm::Value* visit(UnaryOperator *node);

   virtual llvm::Value* visit(FuncArgDecl *node);
   virtual llvm::Value* visit(Expression *node);
   virtual llvm::Value* visit(LambdaExpr *node);
   virtual llvm::Value* visit(ImplicitCastExpr *node);
   virtual llvm::Value* visit(TypedefDecl *node);
   virtual llvm::Value* visit(TypeRef *node);
   virtual llvm::Value* visit(LvalueToRvalue *node);

   virtual llvm::Value* visit(TryStmt *node);
   virtual llvm::Value* visit(ThrowStmt *node);

   virtual llvm::Value* visit(EndOfFileStmt *node);
   virtual llvm::Value* visit(DebugStmt *node);

   virtual llvm::Value* visit(Statement *node);

   void finalize();
   static void linkAndEmit(std::vector<CompilationUnit>& CUs);

   void DeclareClass(ClassDecl*);
   void DeclareEnum(EnumDecl*);
   void DeclareClasses(std::vector<std::shared_ptr<Statement>>& statements);

   unsigned short getAlignment(llvm::Value*);
   unsigned short getAlignment(llvm::Type*);

   llvm::Value* GetFieldOffset(string&, unsigned);
   llvm::Value* GetStructSize(llvm::Type*);

   llvm::Value* CreateStore(llvm::Value* val, llvm::Value* ptr);
   llvm::Value* CreateLoad(llvm::Value* ptr);
   llvm::BasicBlock* CreateBasicBlock(string name, llvm::Function* func = nullptr);

   void doProtocolCopy(llvm::Value *lhs, llvm::Value *rhs);

   llvm::ConstantInt* wordSizedInt(int val);

   llvm::Value* GetString(const string &str, bool cstr = false, bool isConst = false);
   llvm::Value* getStaticVal(
      Variant& v,
      BuiltinType*& ty,
      bool global = false
   );

   llvm::Value* getStaticVal(
      std::shared_ptr<Expression> &expr,
      BuiltinType*& ty,
      bool global = false
   );

   llvm::Value *valueToString(BuiltinType *ty, llvm::Value *val);

   // classes
   llvm::Value* AccessField(string ,string, llvm::Value*);
   llvm::Value* AccessField(size_t, llvm::Value *);

   void SetField(size_t, llvm::Value*, llvm::Value*, bool = false);
   void SetField(string, string, llvm::Value*, llvm::Value*, bool = false);

   llvm::Value* ExtractFromOption(llvm::Value* opt, Type& destTy);

   void DebugPrint(llvm::Value* val, string msg = "");

   llvm::Value *toInt8Ptr(llvm::Value *val);
   llvm::Constant* getTypeInfo(BuiltinType *ty);

   bool addStrMetadata(llvm::Value* inst, string str);

   const string &getFileName()
   {
      return fileName;
   }

   const string &getPath()
   {
      return path;
   }

   size_t getCUID()
   {
      return CUID;
   }

   static void declareStructTy(
      const string &name,
      llvm::StructType *ty
   );

   static bool hasStructTy(const string &name);
   static llvm::StructType *getStructTy(const string &name);

   friend class CGMemory;
   friend class CGCast;
   friend class CGException;
   friend class TypeCheckPass;
   friend class DeclPass;

   friend class cdot::cl::Class;
   friend class cdot::Builtin;

   static llvm::LLVMContext Context;

   llvm::IRBuilder<> Builder;
   std::unique_ptr<llvm::Module> Module;

   static llvm::StructType* ClassInfoType;
   static llvm::StructType* TypeInfoType;
   static llvm::StructType* OpaqueTy;
   static llvm::StructType* LambdaTy;
   static llvm::StructType* VTablePairTy;
   static llvm::StructType* RefcountedType;

   static llvm::IntegerType* WordTy;
   static llvm::IntegerType* Int1Ty;
   static llvm::PointerType* Int8PtrTy;

   static void initGlobalTypes();

   static size_t LambdaFuncPtrPos;
   static size_t LambdaEnvPos;
   static size_t ClassInfoSize;

protected:
   static unordered_map<string, llvm::StructType*> StructTypes;
   static unordered_map<string, llvm::FunctionType*> FunctionTypes;
   static unordered_map<string, llvm::Constant*> Functions;
   static unordered_map<string, llvm::Value*> MutableValues;

   unordered_map<string, llvm::Constant*> OwnFunctions;
   unordered_map<string, llvm::Value*> OwnValues;
   std::vector<pair<Expression*, BuiltinType*>> global_initializers;

   void declareFunction(
      const string &name,
      llvm::Function *func
   );

   llvm::Constant *getFunction(const string &name);
   llvm::Function *getOwnDecl(const Method *method);

   void declareVariable(
      const string &name,
      llvm::Value *var
   );

   llvm::Value *getVariable(const string &name);

   std::vector<Class*> ownTypes;

   llvm::Function* MALLOC;
   llvm::Function* TERMINATE;
   llvm::Function* FREE;
   llvm::Function* PRINTF;

   llvm::Function* ARC_INC;
   llvm::Function* ARC_DEC;

   llvm::Constant *getNative(const string &name);

   static unordered_map<string, llvm::Function*> NativeFunctions;
   static bool hasNative(const string &name);
   static void declareNative(
      const string &name,
      llvm::Function *funcTy
   );

   size_t CUID;


   // compile unit
   const string &fileName;
   const string &path;
   bool isHeader;

   // function stack
   std::vector<llvm::Function*> functions;

   // arc
   std::stack<llvm::Value*> Cleanups;
   std::stack<pair<llvm::Value*, Record*>> Temporaries;

   void CleanupTemporaries();

   // memory
   CGMemory *Mem;

   // excn
   CGException *Exc;

   // cast
   CGCast *Cast;

   // debug info
   bool emitDI;
   DebugInfo *DI;

   // value stack
   std::stack<llvm::Value*> valueStack;
   unordered_map<string, llvm::BasicBlock*> labels;

   std::stack<llvm::Value*> EnvStack;
   std::stack<unordered_map<string, size_t>> EnvIndices;

   std::stack<pair<llvm::BasicBlock*, llvm::BasicBlock*>> BreakContinueStack;

   bool allBranchesTerminated = false;

   void push(llvm::Value* val) {
      valueStack.push(val);
   }

   llvm::Value* pop() {
      auto val = valueStack.top();
      valueStack.pop();

      return val;
   }

   // function captures
   unordered_map<string, std::vector<pair<string,string>>> function_captures;
   bool broken = false;

   // identifier expressions
   llvm::Value* ReturnMemberRef(Expression*, llvm::Value*);

   // functions
   unordered_map<unsigned int, llvm::Type*> LambdaTypes = {};
   unordered_map<string, int> LambdaEnvOrder = {};

   llvm::Function *DeclareFunction(
      string &bound_name,
      std::vector<std::shared_ptr<FuncArgDecl>> args,
      Type return_type,
      bool set_this_arg = false,
      llvm::Type *selfTy = nullptr,
      string this_binding = "",
      std::vector<Attribute> attrs = {},
      bool hiddenParam = false,
      bool envParam = false,
      bool isVirtualOrProtocolMethod = false,
      bool hasDefinition = true
   );

   llvm::Function *DeclareFunction(
      string &bound_name,
      std::vector<std::shared_ptr<FuncArgDecl>> args,
      llvm::Type *return_type,
      bool set_this_arg = false,
      llvm::Type *selfTy = nullptr,
      string this_binding = "",
      std::vector<Attribute> attrs = {},
      bool hiddenParam = false,
      bool envParam = false,
      bool noByVal = false,
      bool hasDefinition = true
   );

   llvm::Function* DeclareMethod(
      string &bound_name,
      std::vector<std::shared_ptr<FuncArgDecl>> args,
      Type return_type,
      llvm::Type *selfTy,
      string &this_binding,
      std::vector<Attribute> attrs = {},
      bool hiddenParam = false,
      bool isVirtualOrProtocolMethod = false,
      bool hasDefinition = true
   );

   void DefineFunction(llvm::Function*, std::shared_ptr<Statement> body, string name = "");
   void DefineFunction(string& bound_name, std::shared_ptr<Statement> body);

   llvm::Function* DeclareDefaultConstructor(
      string &bound_name,
      std::vector<std::shared_ptr<FuncArgDecl>> args,
      Type& return_type,
      llvm::StructType *this_arg,
      string &this_binding,
      cdot::cl::Class *cl
   );

   void DefineDefaultConstructor(
      string &bound_name,
      string &this_binding,
      cdot::cl::Class *cl
   );

   llvm::Function* DeclareDefaultDestructor(
      llvm::StructType* selfArg,
      string& selfBinding,
      cdot::cl::Class* cl
   );

   void DefineDefaultDestructor(
      string& selfBinding,
      cdot::cl::Class* cl,
      std::shared_ptr<CompoundStmt> body = nullptr
   );

   llvm::Value* DispatchProtocolCall(
      Type &protoTy,
      std::vector<llvm::Value*>& args,
      BuiltinType* returnType,
      cl::Method* method,
      bool skipDefaultCheck = false,
      llvm::Value* originalSelf = nullptr,
      llvm::Value* vMethodPair = nullptr
   );

   llvm::Value* ApplyStaticUpCast(BuiltinType *, string&, llvm::Value *);

   llvm::CallInst* DispatchVirtualCall(string &className, string &methodName,
      std::vector<llvm::Value*>& args);

   llvm::Value* HandleBuiltinCall(CallExpr *node);

   // declarations

   void DefineClass(ClassDecl*);

   void DeclareField(FieldDecl*);
   void DefineField(FieldDecl*);

   void DeclareMethod(MethodDecl*);
   void DefineMethod(MethodDecl*);

   void DeclareConstr(ConstrDecl*);
   void DefineConstr(ConstrDecl*);

   void DeclareMemberwiseInitializer(cdot::cl::Class *cl);
   void DefineMemberwiseInitializer(cdot::cl::Class *cl);

   void DeclareStringRepresentableConformance(cl::Class *cl);
   void ImplementStringRepresentableConformance(cl::Class *cl);

   void DeclareEquatableConformance(cl::Class *cl);
   void ImplementEquatableConformance(cl::Class *cl);

   void DeclareHashableConformance(cl::Class *cl);
   void ImplementHashableConformance(cl::Class *cl);

   // utility
   llvm::ConstantInt* ONE;
   llvm::ConstantInt* ZERO;

   llvm::Value* GetInteger(llvm::Value* val);
   llvm::Value* GetInteger(long val, unsigned short bits = 64);

   llvm::Value* CopyByVal(llvm::Value*);

   llvm::Function* DeclareIncrementRefCount();
   llvm::Function* DeclareDecrementRefCount();

   void IncrementRefCount(llvm::Value*);
   void DecrementRefCount(llvm::Value*);

   void CreateCleanup(long count);

   llvm::Instruction* CreateCall(
      llvm::Value *func,
      llvm::ArrayRef<llvm::Value*> args
   );

   llvm::ReturnInst *CreateRet(
      std::shared_ptr<Expression> retVal = nullptr,
      bool sret = false,
      bool incRefCount = false
   );

   llvm::ReturnInst *DoRet(
      std::shared_ptr<Expression> retVal = nullptr,
      bool sret = false,
      bool incRefCount = false
   );

   unordered_map<string, llvm::Value*> hiddenParams;
   std::stack<llvm::Value*> HiddenParamStack;
   std::vector<EHFrame> EHStack;

   // reusable values
   unordered_map<string, llvm::Value*> Strings;

   // collections
   llvm::Value* CreateCStyleArray(BuiltinType* type, std::vector<std::shared_ptr<Expression>>& elements);
   llvm::Value* CreateArray(ObjectType* type, std::vector<std::shared_ptr<Expression>>& elements);
   
   // binary operators
   llvm::Value* HandleBinaryOperator(llvm::Value *lhs, llvm::Value *rhs, BinaryOperatorType, BinaryOperator* node);
   llvm::Value* HandleCastOp(llvm::Value *lhs, BinaryOperator *node);
   llvm::Value* HandleAssignmentOp(llvm::Value*lhs, BinaryOperator *node);
   llvm::Value* HandleArithmeticOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);
   llvm::Value* HandleBitwiseOp(llvm::Value*lhs, llvm::Value*rhs, string& op);
   llvm::Value* HandleLogicalOp(llvm::Value*lhs, BinaryOperator* node);
   llvm::Value* HandleComparisonOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);
   llvm::Value* HandleOtherOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);

   // enum & tuple comparison
   llvm::Value* HandleEnumComp(llvm::Value*lhs, llvm::Value*rhs, std::vector<llvm::Value*>& assocValues,
      std::vector<BuiltinType*>& assocTypes, bool neq = false);
   llvm::Value* HandleTupleComp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node, bool neq = false);

   // pattern matching
   llvm::Value* CreateCompEQ(llvm::Value *&lhs, llvm::Value *&rhs, BuiltinType *&compTy,
      llvm::Function* operatorEquals = nullptr);

   llvm::Value* HandleDictionaryLiteral(CollectionLiteral *node);

   // eh
   void EmitTryNoClauses(TryStmt *node);
   void EmitFinally(TryStmt *node);
   void EmitCatchClauses(TryStmt *node);
};


#endif //CDOT_CODEGENVISITOR_H
