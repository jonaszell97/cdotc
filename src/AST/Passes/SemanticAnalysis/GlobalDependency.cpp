//
// Created by Jonas Zell on 22.12.17.
//

#include "GlobalDependency.h"

#include "../ASTIncludes.h"
#include "SemaPass.h"
#include "../Declaration/DeclPass.h"

#include "../../../Basic/DependencyGraph.h"
#include "../../../Message/Diagnostics.h"

using namespace cdot::support;
using namespace cdot::diag;

namespace cdot {
namespace ast {

class GlobalDependencyImpl {
public:
   explicit GlobalDependencyImpl(SemaPass &SP,
                                 llvm::ArrayRef<Statement*> Decls);

   bool isValid() const
   {
      return valid;
   }

   const llvm::SmallVector<Statement *, 8> &getOrder() const
   {
      return Order;
   }

   std::pair<Statement*, Statement*> getOffendingPair()
   {
      return DG.getOffendingPair();
   };

private:
   using VecTy = llvm::SmallVector<Statement*, 8>;

   SemaPass &SP;
   DependencyGraph<Statement> DG;

   bool valid;
   llvm::SmallVector<Statement*, 8> Order;

   void visit(VecTy &vec, Expression *expr)
   {
      switch (expr->getTypeID()) {
#     define CDOT_EXPR(Name)     \
         case AstNode::Name##ID: \
            return visit##Name(vec, static_cast<Name*>(expr));
#     include "../../AstNode.def"

         default:
            llvm_unreachable("not an expression");
      }
   }

   void visitIdentifierRefExpr(VecTy &vec, IdentifierRefExpr *node);
   void visitNonTypeTemplateArgExpr(VecTy &vec, NonTypeTemplateArgExpr *node) {}
   void visitSubscriptExpr(VecTy &vec, SubscriptExpr *node) {}
   void visitCallExpr(VecTy &vec, CallExpr *node);
   void visitMemberRefExpr(VecTy &vec, MemberRefExpr *node) {}
   void visitEnumCaseExpr(VecTy &vec, EnumCaseExpr *node);

   void visitIntegerLiteral(VecTy &vec, IntegerLiteral *node) {}
   void visitFPLiteral(VecTy &vec, FPLiteral *node) {}
   void visitBoolLiteral(VecTy &vec, BoolLiteral *node) {}
   void visitCharLiteral(VecTy &vec, CharLiteral *node) {}

   void visitDictionaryLiteral(VecTy &vec, DictionaryLiteral *node);
   void visitArrayLiteral(VecTy &vec, ArrayLiteral *node);

   void visitNoneLiteral(VecTy &vec, NoneLiteral *node) {}
   void visitStringLiteral(VecTy &vec, StringLiteral *node) {}
   void visitStringInterpolation(VecTy &vec, StringInterpolation *node);
   void visitTupleLiteral(VecTy &vec, TupleLiteral *node);

   void visitExprSequence(VecTy &vec, ExprSequence *node);
   void visitBinaryOperator(VecTy &vec, BinaryOperator *node);
   void visitTertiaryOperator(VecTy &vec, TertiaryOperator *node);
   void visitUnaryOperator(VecTy &vec, UnaryOperator *node);

   void visitLambdaExpr(VecTy &vec, LambdaExpr *node) {}


   void visitExpressionPattern(VecTy &vec, ExpressionPattern *node) {}
   void visitCasePattern(VecTy &vec, CasePattern *node) {}
   void visitIsPattern(VecTy &vec, IsPattern *node) {}
   void visitImplicitCastExpr(VecTy &vec, ImplicitCastExpr *node) {}
   void visitConstraintExpr(VecTy &vec, ConstraintExpr *node) {}
   void visitLvalueToRvalue(VecTy &vec, LvalueToRvalue *node) {}
   void visitTypeRef(VecTy &vec, TypeRef *node) {}
   void visitStaticExpr(VecTy &vec, StaticExpr *node) {}

   void visitTraitsExpr(VecTy &vec, TraitsExpr *node) {}
};

//void printDepStmt(Statement *Stmt)
//{
//   if (auto Decl = dyn_cast<DeclStmt>(Stmt)) {
//      llvm::outs() << Decl->getIdentifier();
//   }
//   else if (auto FDecl = dyn_cast<FieldDecl>(Stmt)) {
//      llvm::outs() << FDecl->getRecord()->getName() << "."
//                   << FDecl->getName();
//   }
//}

GlobalDependencyImpl::GlobalDependencyImpl(SemaPass &SP,
                                           llvm::ArrayRef<Statement*> Decls)
   : SP(SP), DG(), valid(true)
{
   using Vertex = DependencyGraph<Statement>::Vertex;

   VecTy vec;
   llvm::SmallDenseMap<Statement*, Vertex *> Verts;

   for (auto &Stmt : Decls) {
      Verts.try_emplace(Stmt, &DG.getOrAddVertex(Stmt));
   }

   for (auto &Stmt : Decls) {
      SP.declPass->importFileImports(Stmt->getSourceLoc().getSourceId());

      if (auto decl = dyn_cast<GlobalVarDecl>(Stmt)) {
         SP.declPass->setCurrentNamespace(decl->getDeclarationNamespace());
         if (auto val = decl->getValue().get())
            visit(vec, val);
      }
      else {
         auto fdecl = dyn_cast<FieldDecl>(Stmt);
         SP.declPass->setCurrentNamespace(fdecl->getRecord()
                                               ->getDeclarationNamespace());
         SP.declPass->pushNamespace(fdecl->getRecord()
                                         ->getNameWitoutNamespace());

         if (auto val = fdecl->getDefaultVal().get())
            visit(vec, val);
      }

      auto &vert = Verts[Stmt];
      for (auto &dep : vec) {
         auto it = Verts.find(dep);
         assert(it != Verts.end());

         it->second->addOutgoing(vert);
      }

      SP.clearCurrentNamespace();
      vec.clear();
   }

   auto res = DG.constructOrderedList();
   Order = std::move(res.first);
   valid = res.second;
}

void GlobalDependencyImpl::visitIdentifierRefExpr(VecTy &vec,
                                                  IdentifierRefExpr *node) {
   if (auto G = SP.getGlobalVariable(node->getIdent())) {
      vec.push_back(G->getDecl());
   }
}

void GlobalDependencyImpl::visitEnumCaseExpr(VecTy &vec, EnumCaseExpr *node)
{
   for (const auto &arg : node->getArgs())
      visit(vec, arg.get());
}

void GlobalDependencyImpl::visitCallExpr(VecTy &vec, CallExpr *node)
{
   for (const auto &arg : node->getArgs())
      visit(vec, arg.get());
}

void GlobalDependencyImpl::visitStringInterpolation(VecTy &vec,
                                                    StringInterpolation *node)
{
   for (const auto &str : node->getStrings())
      visit(vec, str.get());
}

void GlobalDependencyImpl::visitTupleLiteral(VecTy &vec, TupleLiteral *node)
{
   for (const auto &el : node->getElements())
      visit(vec, el.second.get());
}

void GlobalDependencyImpl::visitDictionaryLiteral(VecTy &vec,
                                                  DictionaryLiteral *node) {
   for (const auto &key : node->getKeys())
      visit(vec, key.get());
   for (const auto &val : node->getValues())
      visit(vec, val.get());
}

void GlobalDependencyImpl::visitArrayLiteral(VecTy &vec, ArrayLiteral *node)
{
   for (const auto &val : node->getValues())
      visit(vec, val.get());
}

void GlobalDependencyImpl::visitUnaryOperator(VecTy &vec, UnaryOperator *node)
{
   visit(vec, node->getTarget().get());
}

void GlobalDependencyImpl::visitBinaryOperator(VecTy &vec, BinaryOperator *node)
{
   visit(vec, node->getLhs().get());
   visit(vec, node->getRhs().get());
}

void GlobalDependencyImpl::visitTertiaryOperator(VecTy &vec,
                                                 TertiaryOperator *node) {
   visit(vec, node->getCondition().get());
   visit(vec, node->getLhs().get());
   visit(vec, node->getRhs().get());
}

void GlobalDependencyImpl::visitExprSequence(VecTy &vec, ExprSequence *node)
{
   for (const auto &F : node->getFragments()) {
      switch (F.getKind()) {
         case ExprSequence::SequenceElement::EF_Expression:
            visit(vec, F.getExpr().get());
            break;
         case ExprSequence::SequenceElement::EF_PossibleOperator:
            if (auto G = SP.getGlobalVariable(F.getOp()))
               vec.push_back(G->getDecl());

            break;
         default:
            break;
      }
   }
}

GlobalDependency::GlobalDependency(SemaPass &SP,
                                   llvm::ArrayRef<Statement *> Decls)
   : pImpl(new GlobalDependencyImpl(SP, Decls))
{

}

bool GlobalDependency::hasValidOrder()
{
   return pImpl->isValid();
}

llvm::ArrayRef<Statement*> GlobalDependency::getEvaluationOrder()
{
   return pImpl->getOrder();
}

std::pair<Statement*, Statement*> GlobalDependency::getOffendingPair()
{
   return pImpl->getOffendingPair();
}

GlobalDependency::~GlobalDependency()
{
   delete pImpl;
}

} // namespace ast
} // namespace cdot