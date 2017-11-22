//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_ILGENPASS_H
#define CDOT_ILGENPASS_H

#include <stack>
#include "../AbstractPass.h"
#include "../../../IL/ILBuilder.h"

namespace cdot {

namespace cl {

class Method;
struct Field;
struct Property;

class Record;
class Class;
class Enum;
class Union;

} // namespace cl

namespace ast {

class ILGenPass: public AbstractPass {
public:
   explicit ILGenPass(il::Context &Ctx);
   void run(std::vector<CompilationUnit> &CUs);

   void doInitialPass(const std::shared_ptr<Statement> &statement);
   void doInitialPass(
      const std::vector<std::shared_ptr<Statement>> &statements);

   void visit(AstNode *) {}

   void visit(CompoundStmt *node);

   void visit(DeclStmt *node);
   void visit(FunctionDecl *node);
   void visit(CallableDecl *node);
   void visit(DeclareStmt *node);

   void visit(ClassDecl *node);
   void visit(ExtensionDecl *node);
   void visit(EnumDecl *node);
   void visit(UnionDecl *node);

   void visit(FieldDecl *node);
   void visit(PropDecl *node);

   void visit(MethodDecl *node);
   void visit(ConstrDecl *node);
   void visit(DestrDecl *node);

   void visit(RecordTemplateDecl *node);
   void visit(CallableTemplateDecl *node);
   void visit(MethodTemplateDecl *node);

   void visit(IdentifierRefExpr *node);
   void visit(SubscriptExpr *node);
   void visit(CallExpr *node);
   void visit(MemberRefExpr *node);

   void visit(ForStmt *node);
   void visit(ForInStmt *node);
   void visit(WhileStmt *node);
   void visit(IfStmt *node);
   void visit(MatchStmt *node);
   void visit(CaseStmt *node);
   void visit(LabelStmt *node);
   void visit(GotoStmt *node);

   void visit(ReturnStmt *node);
   void visit(BreakStmt *node);
   void visit(ContinueStmt *node);

   void visit(IntegerLiteral *node);
   void visit(FPLiteral *node);
   void visit(BoolLiteral *node);
   void visit(CharLiteral *node);

   void visit(CollectionLiteral *node);

   void visit(NoneLiteral *node);
   void visit(StringLiteral *node);
   void visit(StringInterpolation *node);
   void visit(TupleLiteral *node);

   void visit(BinaryOperator *node);
   void visit(TertiaryOperator *node);
   void visit(UnaryOperator *node);

   void visit(TryStmt *node);
   void visit(ThrowStmt *node);

   void visit(FuncArgDecl *node);
   void visit(Expression *node);
   void visit(LambdaExpr *node);
   void visit(ImplicitCastExpr *node);
   void visit(TypedefDecl *node);
   void visit(TypeRef *node);
   void visit(LvalueToRvalue *node);

   void visit(DebugStmt *node);

protected:
   void DeclareGlobalVariable(DeclStmt *decl);
   void DeclareFunction(FunctionDecl *func);

   void DeclareClass(cl::Class *cl);
   void DeclareEnum(cl::Enum *cl);
   void DeclareUnion(cl::Union *un);
   void DeclareProtocol(cl::Class *cl);
   void DeclareRecord(cl::Record *rec, il::AggregateType *Ty);

   void DeclareField(const cl::Field &field, il::AggregateType *Ty);
   void DeclareMethod(cl::Method *method, il::AggregateType *Ty);
   void DeclareProperty(const cl::Property &P, il::AggregateType *Ty);

   void DeclareValue(il::Value *Val);

   void DefineProperty(const cl::Property &P, il::AggregateType *Ty);

   void DefineFunction(il::Function *F,
                       const std::shared_ptr<Statement> &body);

   il::Context &getContext();
   il::Module *getModule();
   il::Function *getFunction();

   il::Constant *getBoxedInt(uint64_t value,
                             const string &className = "");

   il::Value *BoxPrimitive(il::Value *V, const string &className = "");

   void ReturnMemberExpr(Expression *node, il::Value *Val);

   il::Value* pop();
   il::Value* VisitNode(AstNode *node);
   il::Value* VisitNode(const std::shared_ptr<AstNode> &node);
   void returnResult(il::Value *val);

   void maybeImportType(Type *ty);

   il::Value *HandleCast(Type *from, Type *to, il::Value *V);
   il::Value *HandleIntegerCast(Type *from, Type *to, il::Value *V);
   il::Value *HandleFPCast(Type *from, Type *to, il::Value *V);
   il::Value *HandlePtrCast(Type *from, Type *to, il::Value *V);
   il::Value *HandleDynCast(Type *from, Type *to, il::Value *V);
   il::Value *HandleProtoCast(Type *from, Type *to, il::Value *V);

   il::Value *CreateLogicalAnd(il::Value *lhs, Expression *rhsNode);
   il::Value *CreateLogicalOr(il::Value *lhs, Expression *rhsNode);

   il::CallInst *CreateCall(il::Function *F, llvm::ArrayRef<il::Value*> args,
                            il::Value *Self = nullptr);

   void GenerateVTable(il::ClassType *Ty);
   void FindVirtualMethods(il::ClassType *Ty, cl::Class *cl);

   Type* Int8PtrTy;
   Type* BoolTy;

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
   il::ILBuilder Builder;

   struct EHScope {
      il::LandingPadInst *LandingPad;
   };

   std::stack<EHScope> EHStack;
};

} // namespace ast
} // namespace cdot


#endif //CDOT_ILGENPASS_H
