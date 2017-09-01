//
// Created by Jonas Zell on 11.07.17.
//

#ifndef CDOT_CODEGENVISITOR_H
#define CDOT_CODEGENVISITOR_H


#include <llvm/IR/IRBuilder.h>
#include <unordered_map>
#include <stack>
#include "../Visitor.h"
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
using namespace cdot;

namespace cdot {
   class Type;
   class GenericType;
   class CollectionType;
   class Builtin;
   class BinaryOperator;
   enum class BinaryOperatorType : unsigned int;

   namespace cl {
      class Class;
   }

}

class CodeGenVisitor {
public:
   CodeGenVisitor();

   virtual llvm::Value* visit(NamespaceDecl*);
   virtual llvm::Value* visit(UsingStmt*);
   virtual llvm::Value* visit(EndOfFileStmt*);

   virtual llvm::Value* visit(FunctionDecl*);
   virtual llvm::Value* visit(CompoundStmt*);
   virtual llvm::Value* visit(IdentifierRefExpr*);
   virtual llvm::Value* visit(DeclStmt*);
   virtual llvm::Value* visit(ForStmt*);
   virtual llvm::Value* visit(WhileStmt*);

   virtual llvm::Value* visit(CollectionLiteral*);
   virtual llvm::Value* visit(NumericLiteral*);
   virtual llvm::Value* visit(NoneLiteral*);
   virtual llvm::Value* visit(StringLiteral*);
   virtual llvm::Value* visit(SubscriptExpr*);
   virtual llvm::Value* visit(CallExpr*);
   virtual llvm::Value* visit(MemberRefExpr*);
   virtual llvm::Value* visit(BinaryOperator*);
   virtual llvm::Value* visit(TertiaryOperator*);
   virtual llvm::Value* visit(UnaryOperator*);
   virtual llvm::Value* visit(BreakStmt*);
   virtual llvm::Value* visit(ContinueStmt*);
   virtual llvm::Value* visit(IfStmt*);
   virtual llvm::Value* visit(MatchStmt*);
   virtual llvm::Value* visit(CaseStmt*);
   virtual llvm::Value* visit(LabelStmt*);
   virtual llvm::Value* visit(GotoStmt*);
   virtual llvm::Value* visit(FuncArgDecl*);
   virtual llvm::Value* visit(ReturnStmt*);
   virtual llvm::Value* visit(Expression*);
   virtual llvm::Value* visit(ClassDecl*);
   virtual llvm::Value* visit(MethodDecl*);
   virtual llvm::Value* visit(FieldDecl*);
   virtual llvm::Value* visit(ConstrDecl*);
   virtual llvm::Value* visit(EnumDecl*);
   virtual llvm::Value* visit(LambdaExpr*);
   virtual llvm::Value* visit(ImplicitCastExpr*);
   virtual llvm::Value* visit(ExtendStmt*);
   virtual llvm::Value* visit(TypedefDecl*);
   virtual llvm::Value* visit(TypeRef*);
   virtual llvm::Value* visit(DeclareStmt*);
   virtual llvm::Value* visit(LvalueToRvalue*);
   virtual llvm::Value* visit(DebugStmt*);
   virtual llvm::Value* visit(TupleLiteral*);

   virtual llvm::Value* visit(Statement*);

   void finalize();

   void DeclareClass(ClassDecl*);
   void DeclareEnum(EnumDecl*);
   void DeclareClasses(std::shared_ptr<CompoundStmt>);

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

   // classes
   static llvm::Value* AccessField(string ,string, llvm::Value*);
   static llvm::Value* AccessField(size_t, llvm::Value *);

   static void SetField(size_t, llvm::Value*, llvm::Value*, bool = false);
   static void SetField(string, string, llvm::Value*, llvm::Value*, bool = false);

   static unordered_map<string, pair<unsigned short, size_t>> StructSizes;

   friend class CGType;
   friend class CGMemory;
   friend class TypeCheckVisitor;
   friend class InternalClass;
   friend class CGInternal;
   friend class cdot::cl::Class;
   friend class cdot::Builtin;

   static llvm::LLVMContext Context;
   static llvm::IRBuilder<> Builder;

protected:
   static unique_ptr<llvm::Module> Module;
   static unordered_map<string, llvm::Value*> MutableValues;
   static unordered_map<string, llvm::Constant*> Functions;
   static unordered_map<string, llvm::FunctionType*> FunctionTypes;
   static std::vector<Expression*> global_initializers;
   static llvm::Function* MALLOC;


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
      bool envParam = false
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
      bool envParam = false
   );

   llvm::Function* DeclareMethod(
      string &bound_name,
      std::vector<std::shared_ptr<FuncArgDecl>> args,
      Type* return_type,
      llvm::StructType *this_arg,
      string &this_binding,
      std::vector<Attribute> attrs = {},
      bool hiddenParam = false
   );

   void DefineFunction(llvm::Function*, std::shared_ptr<Statement> body, string name = "");
   void DefineFunction(string& bound_name, std::shared_ptr<Statement> body);

   void InitializeFields(llvm::Function *func, cdot::cl::Class *cl);

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

   llvm::Value* ApplyProtocolShift(Type *, string&, llvm::Value *);
   llvm::CallInst* DispatchProtocolCall(Type *, string&, llvm::Value *, std::vector<llvm::Value*>& args);

   llvm::Value* ApplyStaticUpCast(Type *, string&, llvm::Value *);
   llvm::Value* ApplyStaticDownCast(Type *, llvm::Value *);
   llvm::Value* ApplyDynamicDownCast(Type *, llvm::Value *);

   llvm::CallInst* DispatchVirtualCall(string &className, string &methodName,
      std::vector<llvm::Value*>& args);

   void DefineClass(ClassDecl*);

   void DeclareField(FieldDecl*);
   void DefineField(FieldDecl*);

   void DeclareMethod(MethodDecl*);
   void DefineMethod(MethodDecl*);

   void DeclareConstr(ConstrDecl*);
   void DefineConstr(ConstrDecl*);
   void DefineMemberwiseInitializer(cdot::cl::Class*);

   // utility
   llvm::ConstantInt* ONE;
   llvm::ConstantInt* ZERO;
   llvm::ConstantInt* ONE_64;
   llvm::ConstantInt* ZERO_64;

   llvm::Value* GetInteger(long val, unsigned short bits = 64, bool isUnsigned = false);
   llvm::Value* GetFloat(double val, unsigned short bits = 64);

   llvm::Value* CopyByVal(llvm::Value*);

   unordered_map<string, llvm::Value*> hiddenParams;
   std::stack<llvm::Value*> HiddenParamStack;

   // reusable values
   llvm::Value* GetString(string&, bool = false, bool = false);
   unordered_map<string, llvm::Value*> Strings = {};

   // collections
   llvm::Value* CreateCStyleArray(Type* type, std::vector<std::shared_ptr<Expression>>& elements);
   llvm::Value* CreateCStyleArray(Type* type, std::vector<llvm::Value*>& elements);
   llvm::Value* CreateCStyleArray(Type* type, llvm::Value* size, bool heapAlloc = false);

   llvm::Value* CreateArray(CollectionType* type, std::vector<std::shared_ptr<Expression>>& elements);
   llvm::Value* CreateArray(CollectionType* type, std::vector<llvm::Value*>& elements);
   
   // binary operators
   llvm::Value* HandleBinaryOperator(llvm::Value *lhs, llvm::Value *rhs, BinaryOperatorType, BinaryOperator* node);
   llvm::Value* HandleCastOp(llvm::Value *lhs, BinaryOperator *node);
   llvm::Value* HandleAssignmentOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator *node);
   llvm::Value* HandleArithmeticOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);
   llvm::Value* HandleBitwiseOp(llvm::Value*lhs, llvm::Value*rhs, string& op);
   llvm::Value* HandleLogicalOp(llvm::Value*lhs, BinaryOperator* node);
   llvm::Value* HandleComparisonOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);
   llvm::Value* HandleOtherOp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);

   // enum & tuple comparison
   llvm::Value* HandleEnumComp(llvm::Value*lhs, llvm::Value*rhs, std::vector<llvm::Value*>& assocValues,
      std::vector<Type*>& assocTypes);
   llvm::Value* HandleTupleComp(llvm::Value*lhs, llvm::Value*rhs, BinaryOperator* node);

   // pattern matching
   llvm::Value* CreateCompEQ(llvm::Value *&lhs, llvm::Value *&rhs, Type *&compTy);
};


#endif //CDOT_CODEGENVISITOR_H
