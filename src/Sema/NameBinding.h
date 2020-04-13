#ifndef CDOT_NAMEBINDING_H
#define CDOT_NAMEBINDING_H

#include "AST/ASTVisitor.h"

namespace cdot {
namespace ast {

class SemaPass;

class NameBinder: public RecursiveASTVisitor<NameBinder> {
   /// \brief The Sema instance.
   SemaPass &Sema;

   /// \brief A set that referenced declarations get added to,
   /// if the client wants it.
   llvm::SmallPtrSetImpl<NamedDecl*> *ReferencedDecls;

   /// \brief Keeps track of whether or not we encountered an error during
   /// name binding.
   bool HadError = false;

public:
   /// \brief Create a new NameBinder instance.
   explicit NameBinder(SemaPass &Sema,
                       SmallPtrSetImpl<NamedDecl*> *ReferencedDecls = nullptr);

   /// \brief Perform name-binding on the given expression and all of its
   /// sub-expressions.
   /// \return true on error, false on success.
   bool bindNames(Expression *E);

   /// RecursiveASTVisitor overrides.
   bool visitIdentifierRefExpr(IdentifierRefExpr *E);
   bool visitTemplateArgListExpr(TemplateArgListExpr *E);
   bool visitCallExpr(CallExpr *E);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_NAMEBINDING_H
