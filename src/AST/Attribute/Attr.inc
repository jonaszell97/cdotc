#ifdef CDOT_ATTR_DECL
class ExternAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Extern; }


   void printPretty(llvm::raw_ostream &out) const;
   ExternAttr *clone(ast::ASTContext &Ctx) const;


   enum LangKind {
      C,
      CXX,
      CPP
   };

private:
   LangKind lang;

public:
   LangKind getLang() const { return lang; }

public:
   explicit ExternAttr(LangKind lang, SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::Extern, SR), lang(lang)
   {}
};

class InlineAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Inline; }


   void printPretty(llvm::raw_ostream &out) const;
   InlineAttr *clone(ast::ASTContext &Ctx) const;


   enum LevelKind {
      always,
      never,
      hint
   };

private:
   LevelKind level;

public:
   LevelKind getLevel() const { return level; }

public:
   explicit InlineAttr(LevelKind level = hint, SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::Inline, SR), level(level)
   {}
};

class ImplicitAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Implicit; }


   void printPretty(llvm::raw_ostream &out) const;
   ImplicitAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit ImplicitAttr(SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::Implicit, SR)
   {}
};

class ThinAttr: public TypeAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Thin; }


   void printPretty(llvm::raw_ostream &out) const;
   ThinAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit ThinAttr(SourceRange SR = SourceRange())
      : TypeAttr(AttrKind::Thin, SR)
   {}
};

class OpaqueAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Opaque; }


   void printPretty(llvm::raw_ostream &out) const;
   OpaqueAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit OpaqueAttr(SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::Opaque, SR)
   {}
};

class AlignAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Align; }


   void printPretty(llvm::raw_ostream &out) const;
   AlignAttr *clone(ast::ASTContext &Ctx) const;


private:
   ast::StaticExpr* alignment;

public:
   ast::StaticExpr* getAlignment() const { return alignment; }

public:
   explicit AlignAttr(ast::StaticExpr* alignment, SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::Align, SR), alignment(alignment)
   {}
};

class DiscardableResultAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::DiscardableResult; }


   void printPretty(llvm::raw_ostream &out) const;
   DiscardableResultAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit DiscardableResultAttr(SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::DiscardableResult, SR)
   {}
};
#endif
#undef CDOT_ATTR_DECL

#ifdef CDOT_ATTR_IMPL

   Attr::VisitationPoint Attr::getVisitationPoint() const {
      switch (kind) {
      default: llvm_unreachable("bad attr kind!");
         case AttrKind::Align:
      return AfterSema;
   case AttrKind::Inline:
   case AttrKind::Implicit:
   case AttrKind::Opaque:
   case AttrKind::DiscardableResult:
      return AfterDeclaration;
   case AttrKind::Extern:
   case AttrKind::Thin:
      return BeforeDeclaration;

      }
   }
void ExternAttr::printPretty(llvm::raw_ostream &out) const {
llvm::StringRef langStr;
switch (lang) {
   case C: langStr = "C"; break;
   case CXX: langStr = "CXX"; break;
   case CPP: langStr = "CPP"; break;

}
   out << "@" << getSpelling();
out << '(' << langStr << ')';

}

ExternAttr *ExternAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) ExternAttr(lang, SourceLoc);
}



void InlineAttr::printPretty(llvm::raw_ostream &out) const {
llvm::StringRef levelStr;
switch (level) {
   case always: levelStr = "always"; break;
   case never: levelStr = "never"; break;
   case hint: levelStr = "hint"; break;

}
   out << "@" << getSpelling();
out << '(' << levelStr << ')';

}

InlineAttr *InlineAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) InlineAttr(level, SourceLoc);
}



void ImplicitAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

ImplicitAttr *ImplicitAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) ImplicitAttr(SourceLoc);
}



void ThinAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

ThinAttr *ThinAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) ThinAttr(SourceLoc);
}



void OpaqueAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

OpaqueAttr *OpaqueAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) OpaqueAttr(SourceLoc);
}



void AlignAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();
out << '(' << alignment->getEvaluatedExpr() << ')';

}

AlignAttr *AlignAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) AlignAttr(alignment, SourceLoc);
}



void DiscardableResultAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

DiscardableResultAttr *DiscardableResultAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) DiscardableResultAttr(SourceLoc);
}


#endif
#undef CDOT_ATTR_IMPL

