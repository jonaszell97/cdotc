//
// Created by Jonas Zell on 11.07.17.
//

#ifndef CDOT_CODEGENVISITOR_H
#define CDOT_CODEGENVISITOR_H


#include <llvm/IR/IRBuilder.h>
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

using std::string;
using std::pair;
using std::unordered_map;
using std::unique_ptr;

namespace cdot {
   class Type;
   class GenericType;
   class CollectionType;
   class Builtin;
   class BinaryOperator;
   enum class BinaryOperatorType : unsigned int;
   struct Argument;

   namespace cl {
      class Class;
      struct Method;
   }
}

using namespace cdot;
using cdot::cl::Method;
using cdot::cl::Class;

class CodeGen {
public:
   CodeGen();

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

   virtual llvm::Value* visit(EndOfFileStmt *node);
   virtual llvm::Value* visit(DebugStmt *node);

   virtual llvm::Value* visit(Statement *node);

   void finalize();

   void DeclareClass(ClassDecl*);
   void DeclareEnum(EnumDecl*);
   void DeclareClasses(std::vector<std::shared_ptr<Statement>>& statements);

   static llvm::Constant* getFunction(string& name) {
      return Functions[name];
   }

   static unsigned short getAlignment(llvm::Value*);
   static unsigned short getAlignment(llvm::Type*);

   static llvm::Value* GetFieldOffset(string&, unsigned);
   static llvm::Value* GetStructSize(string&);
   static llvm::Value* GetStructSize(llvm::Type*);

   static llvm::Value* CreateStore(llvm::Value* val, llvm::Value* ptr);
   static llvm::Value* CreateLoad(llvm::Value* ptr);
   static llvm::BasicBlock* CreateBasicBlock(string name, llvm::Function* func = nullptr);

   static void doProtocolCopy(llvm::Value *lhs, llvm::Value *rhs);

   static llvm::ConstantInt* wordSizedInt(int val);

   static llvm::Value* GetString(string&, bool = false);
   static llvm::Value* getStaticVal(Variant& v, Type*& ty, bool global = false);

   // classes
   static llvm::Value* AccessField(string ,string, llvm::Value*);
   static llvm::Value* AccessField(size_t, llvm::Value *);

   static void SetField(size_t, llvm::Value*, llvm::Value*, bool = false);
   static void SetField(string, string, llvm::Value*, llvm::Value*, bool = false);

   static llvm::Value* ExtractFromOption(llvm::Value* opt, Type* destTy);

   static void DebugPrint(llvm::Value* val, string msg = "");

   static llvm::Value* toInt8Ptr(llvm::Value *val);

   static bool addStrMetadata(llvm::Value* inst, string str);

   static unordered_map<string, pair<unsigned short, size_t>> StructSizes;

   friend class CGType;
   friend class CGMemory;
   friend class TypeCheckPass;
   friend class DeclPass;
   friend class InternalClass;
   friend class CGInternal;
   friend class cdot::cl::Class;
   friend class cdot::Builtin;

   static llvm::LLVMContext Context;
   static llvm::IRBuilder<> Builder;
   static llvm::StructType* ClassInfoType;
   static llvm::StructType* TypeInfoType;
   static llvm::StructType* OpaqueTy;
   static llvm::IntegerType* WordTy;
   static llvm::StructType* LambdaTy;
   static llvm::StructType* VTablePairTy;

protected:
   static unique_ptr<llvm::Module> Module;
   static unordered_map<string, llvm::Value*> MutableValues;
   static unordered_map<string, llvm::Constant*> Functions;
   static unordered_map<string, llvm::FunctionType*> FunctionTypes;
   static std::vector<pair<Expression*, Type*>> global_initializers;
   static llvm::Function* MALLOC;
   static llvm::Function* FREE;
   static llvm::Function* PRINTF;

   static llvm::Function* ARC_INC;
   static llvm::Function* ARC_DEC;
   static llvm::StructType* RefcountedType;


   // function stack
   static std::vector<llvm::Function*> functions;

   // value stack
   std::stack<llvm::Value*> valueStack;
   unordered_map<string, llvm::BasicBlock*> labels;

   std::stack<llvm::Value*> EnvStack;
   std::stack<unordered_map<string, size_t>> EnvIndices;

   std::stack<pair<llvm::BasicBlock*, llvm::BasicBlock*>> BreakContinueStack;

   void push(llvm::Value* val) {
      valueStack.push(val);
   }

   llvm::Value* pop() {
      auto val = valueStack.top();
      valueStack.pop();

      return val;
   }

   // function captures
   unordered_map<string, std::vector<pair<string,string>>> function_captures = {};

   bool broken = false;

   // identifier expressions
   llvm::Value* ReturnMemberRef(Expression*, llvm::Value*);

   // functions
   unordered_map<unsigned int, llvm::Type*> LambdaTypes = {};
   unordered_map<string, int> LambdaEnvOrder = {};

   llvm::Function *DeclareFunction(
      string &bound_name,
      std::vector<std::shared_ptr<FuncArgDecl>> args,
      Type *return_type,
      bool set_this_arg = false,
      llvm::StructType *this_val = nullptr,
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
      llvm::StructType *this_val = nullptr,
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
      Type* return_type,
      llvm::StructType *this_arg,
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
      Type *return_type,
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
      Type *protoTy,
      std::vector<llvm::Value*>& args,
      Type* returnType,
      cl::Method* method,
      bool skipDefaultCheck = false,
      llvm::Value* originalSelf = nullptr,
      llvm::Value* vMethodPair = nullptr
   );

   llvm::Value* ApplyStaticUpCast(Type *, string&, llvm::Value *);

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

   // utility
   llvm::ConstantInt* ONE;
   llvm::ConstantInt* ZERO;
   llvm::ConstantInt* ONE_64;
   llvm::ConstantInt* ZERO_64;

   static llvm::Value* GetInteger(llvm::Value* val);
   static llvm::Value* GetInteger(long val, unsigned short bits = 64);

   static llvm::Value* GetFloat(double val, unsigned short bits = 64);

   llvm::Value* CopyByVal(llvm::Value*);

   static llvm::Function* DefineIncrementRefCount();
   static llvm::Function* DefineDecrementRefCount();

   static llvm::Function* DeclareIncrementRefCount();
   static llvm::Function* DeclareDecrementRefCount();

   static void IncrementRefCount(llvm::Value*, const string& className);
   static void DecrementRefCount(llvm::Value*, string& className);
   void CreateCleanup(long count);

   unordered_map<string, llvm::Value*> hiddenParams;
   std::stack<llvm::Value*> HiddenParamStack;

   // reusable values
   static unordered_map<string, llvm::Value*> Strings;

   // collections
   llvm::Value* CreateCStyleArray(Type* type, std::vector<std::shared_ptr<Expression>>& elements);
   llvm::Value* CreateArray(CollectionType* type, std::vector<std::shared_ptr<Expression>>& elements);
   
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
      std::vector<Type*>& assocTypes, bool neq = false);
   llvm::Value* HandleTupleComp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node, bool neq = false);

   // pattern matching
   llvm::Value* CreateCompEQ(llvm::Value *&lhs, llvm::Value *&rhs, Type *&compTy,
      llvm::Function* operatorEquals = nullptr);

   llvm::Value* HandleDictionaryLiteral(CollectionLiteral *node);
};


#endif //CDOT_CODEGENVISITOR_H
