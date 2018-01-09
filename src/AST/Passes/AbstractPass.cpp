
#include "AbstractPass.h"
#include "ASTIncludes.h"

#include "SemanticAnalysis/SemaPass.h"
#include "Declaration/DeclPass.h"
#include "ILGen/ILGenPass.h"
#include "CodeGen/CodeGen.h"

namespace cdot {

namespace il {
class Value;
}

namespace ast {

void ASTPass::RunOn(std::vector<CompilationUnit> &CUs)
{
   switch (typeID) {
#  define CDOT_AST_PASS(Name)                         \
      case Name##ID:                                  \
         return static_cast<Name*>(this)->run(CUs);
#  define CDOT_INCLUDE_ALL
#  include "Passes.def"
   }
}

template<class SubClass, class RetTy>
RetTy AbstractPass<SubClass, RetTy>::visit(Expression *node)
{
   switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                \
         case AstNode::Name##ID:                            \
            return static_cast<SubClass*>(this)             \
               ->visit##Name(static_cast<Name*>(node));
#     include "../AstNode.def"

      default:
         llvm_unreachable("not an expression!");
   }
}

template<class SubClass, class RetTy>
void AbstractPass<SubClass, RetTy>::visit(Statement *node)
{
   switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                \
         case AstNode::Name##ID:                            \
            static_cast<SubClass*>(this)                    \
               ->visit##Name(static_cast<Name*>(node));     \
            return;
#     define CDOT_STMT(Name)                                \
         case AstNode::Name##ID:                            \
            return static_cast<SubClass*>(this)             \
               ->visit##Name(static_cast<Name*>(node));
#     include "../AstNode.def"
   }
}

template<class SubClass>
void AbstractPass<SubClass, void>::visit(AstNode *node)
{
   switch (node->getTypeID()) {
#     define CDOT_ASTNODE(Name)                             \
         case AstNode::Name##ID:                            \
            return static_cast<SubClass*>(this)             \
               ->visit##Name(static_cast<Name*>(node));
#     include "../AstNode.def"
   }
}

template class AbstractPass<SemaPass, QualType>;
template class AbstractPass<ILGenPass, il::Value*>;
template class AbstractPass<DeclPass, void>;

} // namespace ast
} // namespace cdot