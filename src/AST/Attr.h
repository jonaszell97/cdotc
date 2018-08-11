//
// Created by Jonas Zell on 16.03.18.
//

#ifndef CDOT_ATTR_H
#define CDOT_ATTR_H

#include "AST/SourceType.h"
#include "Lex/SourceLocation.h"

#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/StringRef.h>

#include <string>

namespace cdot {

namespace ast {
   class ASTContext;
   class StaticExpr;
}

enum class AttrKind: unsigned char {
   _notAttr,

#  define CDOT_ATTR(Name, Spelling)          \
   Name,
#  define CDOT_ABSTRACT_ATTR(Name)           \
   Name,
#  include "Attributes.def"
};

class Attr {
public:
   AttrKind getKind() const { return kind; }
   SourceRange getSourceRange() const { return SourceLoc; }
   SourceLocation getSourceLoc() const { return SourceLoc.getStart(); }
   llvm::StringRef getSpelling() const;

   void printPretty(llvm::raw_ostream &out) const;
   Attr *clone(ast::ASTContext &Ctx) const;

   enum VisitationPoint: unsigned char {
#  define CDOT_VISITATION_POINT(Name)           \
   Name,
#  include "Attributes.def"
   };

   VisitationPoint getVisitationPoint() const;
   bool isInherited() const;

   static bool classofKind(AttrKind kind) { return true; }
   static bool classof(const Attr *A) { return classofKind(A->getKind()); }

protected:
   explicit Attr(AttrKind kind, SourceRange SR = SourceRange())
      : kind(kind), SourceLoc(SR)
   {
   }

   AttrKind kind;
   SourceRange SourceLoc;
};

class DeclAttr: public Attr {
public:
   static bool classofKind(AttrKind kind)
   {
      return kind > AttrKind::_firstDeclAttr
             && kind < AttrKind::_lastDeclAttr;
   }

   static bool classof(const Attr *A) { return classofKind(A->getKind()); }

protected:
   explicit DeclAttr(AttrKind kind, SourceRange SR = SourceRange())
      : Attr(kind, SR)
   {}
};

class StmtAttr: public Attr {
public:
   static bool classofKind(AttrKind kind)
   {
      return kind > AttrKind::_firstStmtAttr
             && kind < AttrKind::_lastStmtAttr;
   }

   static bool classof(const Attr *A) { return classofKind(A->getKind()); }

protected:
   explicit StmtAttr(AttrKind kind, SourceRange SR = SourceRange())
      : Attr(kind, SR)
   {}
};

class ExprAttr: public Attr {
public:
   static bool classofKind(AttrKind kind)
   {
      return kind > AttrKind::_firstExprAttr
             && kind < AttrKind::_lastExprAttr;
   }

   static bool classof(const Attr *A) { return classofKind(A->getKind()); }

protected:
   explicit ExprAttr(AttrKind kind, SourceRange SR = SourceRange())
      : Attr(kind, SR)
   {}
};

class TypeAttr: public Attr {
public:
   static bool classofKind(AttrKind kind)
   {
      return kind > AttrKind::_firstTypeAttr
             && kind < AttrKind::_lastTypeAttr;
   }

   static bool classof(const Attr *A) { return classofKind(A->getKind()); }

protected:
   explicit TypeAttr(AttrKind kind, SourceRange SR = SourceRange())
      : Attr(kind, SR)
   {}
};

#define CDOT_ATTR_DECL
#include "Attr.inc"

template<class SubClass, class RetTy = void, class ...ParamTys>
class AttrVisitor {
public:
#  define CDOT_ATTR(NAME, SPELLING) RetTy visit##NAME##Attr(NAME##Attr*)   \
   { return RetTy(); }
#  include "Attributes.def"

   RetTy visit(Attr *A)
   {
      switch (A->getKind()) {
#     define CDOT_ATTR(NAME, SPELLING)                                     \
      case AttrKind::NAME:                                                 \
         return static_cast<SubClass*>(this)->visit##NAME##Attr(           \
            static_cast<NAME##Attr*>(A));
#     include "Attributes.def"
      default:
         llvm_unreachable("bad attr kind");
      }
   }
};

} // namespace cdot


#endif //CDOT_ATTR_H
