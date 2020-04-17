#ifdef CDOT_ATTR_DECL
class ExternAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Extern; }


   void printPretty(llvm::raw_ostream &out) const;
   ExternAttr *clone(ast::ASTContext &Ctx) const;


   enum LangKind {
      C,
      CXX
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

class AutoClosureAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::AutoClosure; }


   void printPretty(llvm::raw_ostream &out) const;
   AutoClosureAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit AutoClosureAttr(SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::AutoClosure, SR)
   {}
};

class EscapingAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Escaping; }


   void printPretty(llvm::raw_ostream &out) const;
   EscapingAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit EscapingAttr(SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::Escaping, SR)
   {}
};

class TestableAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Testable; }


   void printPretty(llvm::raw_ostream &out) const;
   TestableAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit TestableAttr(SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::Testable, SR)
   {}
};

class CompileTimeAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::CompileTime; }


   void printPretty(llvm::raw_ostream &out) const;
   CompileTimeAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit CompileTimeAttr(SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::CompileTime, SR)
   {}
};

class NoDeriveAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::NoDerive; }


   void printPretty(llvm::raw_ostream &out) const;
   NoDeriveAttr *clone(ast::ASTContext &Ctx) const;


   enum KindKind {
      Equatable,
      Copyable,
      Hashable,
      RawRepresentable,
      StringRepresentable,
      _All
   };

private:
   KindKind kind;

public:
   KindKind getKind() const { return kind; }

public:
   explicit NoDeriveAttr(KindKind kind = _All, SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::NoDerive, SR), kind(kind)
   {}
};

class StrongAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::Strong; }


   void printPretty(llvm::raw_ostream &out) const;
   StrongAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit StrongAttr(SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::Strong, SR)
   {}
};

class VersionDeclAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::VersionDecl; }


   void printPretty(llvm::raw_ostream &out) const;
   VersionDeclAttr *clone(ast::ASTContext &Ctx) const;


   enum VersionKind {
      None,
      Windows,
      Darwin,
      macOS,
      Linux,
      POSIX
   };

private:
   VersionKind version;

public:
   VersionKind getVersion() const { return version; }

public:
   explicit VersionDeclAttr(VersionKind version, SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::VersionDecl, SR), version(version)
   {}
};

class VersionStmtAttr: public StmtAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::VersionStmt; }


   void printPretty(llvm::raw_ostream &out) const;
   VersionStmtAttr *clone(ast::ASTContext &Ctx) const;


   enum VersionKind {
      None,
      Windows,
      Darwin,
      macOS,
      Linux,
      POSIX
   };

private:
   VersionKind version;

public:
   VersionKind getVersion() const { return version; }

public:
   explicit VersionStmtAttr(VersionKind version, SourceRange SR = SourceRange())
      : StmtAttr(AttrKind::VersionStmt, SR), version(version)
   {}
};

class _BuiltinAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::_Builtin; }


   void printPretty(llvm::raw_ostream &out) const;
   _BuiltinAttr *clone(ast::ASTContext &Ctx) const;


private:
   std::string builtinName;

public:
   llvm::StringRef getBuiltinName() const { return builtinName; }

public:
   explicit _BuiltinAttr(llvm::StringRef builtinName = "", SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::_Builtin, SR), builtinName(builtinName)
   {}
};

class _SemanticsAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::_Semantics; }


   void printPretty(llvm::raw_ostream &out) const;
   _SemanticsAttr *clone(ast::ASTContext &Ctx) const;


private:
   std::string semanticsKind;

public:
   llvm::StringRef getSemanticsKind() const { return semanticsKind; }

public:
   explicit _SemanticsAttr(llvm::StringRef semanticsKind, SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::_Semantics, SR), semanticsKind(semanticsKind)
   {}
};

class _DebugAttr: public DeclAttr {
public:
   static bool classof(const Attr* A) { return classofKind(A->getKind()); }
   static bool classofKind(AttrKind kind){ return kind == AttrKind::_Debug; }


   void printPretty(llvm::raw_ostream &out) const;
   _DebugAttr *clone(ast::ASTContext &Ctx) const;


private:


public:
   explicit _DebugAttr(SourceRange SR = SourceRange())
      : DeclAttr(AttrKind::_Debug, SR)
   {}
};
#endif
#undef CDOT_ATTR_DECL

#ifdef CDOT_ATTR_IMPL

   Attr::VisitationPoint Attr::getVisitationPoint() const {
      switch (kind) {
      default: llvm_unreachable("bad attr kind!");
         case AttrKind::Opaque:
   case AttrKind::DiscardableResult:
   case AttrKind::AutoClosure:
   case AttrKind::Escaping:
   case AttrKind::Testable:
   case AttrKind::CompileTime:
   case AttrKind::NoDerive:
      return Never;
   case AttrKind::Align:
      return AfterSema;
   case AttrKind::Thin:
   case AttrKind::_Builtin:
   case AttrKind::_Semantics:
   case AttrKind::_Debug:
      return Immediate;
   case AttrKind::Inline:
   case AttrKind::Implicit:
      return AfterDeclaration;
   case AttrKind::Extern:
   case AttrKind::Strong:
   case AttrKind::VersionDecl:
   case AttrKind::VersionStmt:
      return BeforeDeclaration;

      }
   }

   bool Attr::isInherited() const {
      switch (kind) {
      default: llvm_unreachable("bad attr kind!");
         case AttrKind::_Builtin:
      return false;
   case AttrKind::VersionStmt:
      return false;
   case AttrKind::VersionDecl:
      return false;
   case AttrKind::Strong:
      return false;
   case AttrKind::NoDerive:
      return false;
   case AttrKind::CompileTime:
      return true;
   case AttrKind::Testable:
      return false;
   case AttrKind::Align:
      return false;
   case AttrKind::Escaping:
      return false;
   case AttrKind::DiscardableResult:
      return true;
   case AttrKind::Thin:
      return false;
   case AttrKind::Implicit:
      return false;
   case AttrKind::AutoClosure:
      return false;
   case AttrKind::Inline:
      return true;
   case AttrKind::Opaque:
      return false;
   case AttrKind::_Debug:
      return false;
   case AttrKind::_Semantics:
      return false;
   case AttrKind::Extern:
      return false;

      }
   }
void ExternAttr::printPretty(llvm::raw_ostream &out) const {
llvm::StringRef langStr;
switch (lang) {
   case C: langStr = "C"; break;
   case CXX: langStr = "CXX"; break;

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



void AutoClosureAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

AutoClosureAttr *AutoClosureAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) AutoClosureAttr(SourceLoc);
}



void EscapingAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

EscapingAttr *EscapingAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) EscapingAttr(SourceLoc);
}



void TestableAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

TestableAttr *TestableAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) TestableAttr(SourceLoc);
}



void CompileTimeAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

CompileTimeAttr *CompileTimeAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) CompileTimeAttr(SourceLoc);
}



void NoDeriveAttr::printPretty(llvm::raw_ostream &out) const {
llvm::StringRef kindStr;
switch (kind) {
   case Equatable: kindStr = "Equatable"; break;
   case Copyable: kindStr = "Copyable"; break;
   case Hashable: kindStr = "Hashable"; break;
   case RawRepresentable: kindStr = "RawRepresentable"; break;
   case StringRepresentable: kindStr = "StringRepresentable"; break;
   case _All: kindStr = "_All"; break;

}
   out << "@" << getSpelling();
out << '(' << kindStr << ')';

}

NoDeriveAttr *NoDeriveAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) NoDeriveAttr(kind, SourceLoc);
}



void StrongAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

StrongAttr *StrongAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) StrongAttr(SourceLoc);
}



void VersionDeclAttr::printPretty(llvm::raw_ostream &out) const {
llvm::StringRef versionStr;
switch (version) {
   case None: versionStr = "None"; break;
   case Windows: versionStr = "Windows"; break;
   case Darwin: versionStr = "Darwin"; break;
   case macOS: versionStr = "macOS"; break;
   case Linux: versionStr = "Linux"; break;
   case POSIX: versionStr = "POSIX"; break;

}
   out << "@" << getSpelling();
out << '(' << versionStr << ')';

}

VersionDeclAttr *VersionDeclAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) VersionDeclAttr(version, SourceLoc);
}



void VersionStmtAttr::printPretty(llvm::raw_ostream &out) const {
llvm::StringRef versionStr;
switch (version) {
   case None: versionStr = "None"; break;
   case Windows: versionStr = "Windows"; break;
   case Darwin: versionStr = "Darwin"; break;
   case macOS: versionStr = "macOS"; break;
   case Linux: versionStr = "Linux"; break;
   case POSIX: versionStr = "POSIX"; break;

}
   out << "@" << getSpelling();
out << '(' << versionStr << ')';

}

VersionStmtAttr *VersionStmtAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) VersionStmtAttr(version, SourceLoc);
}



void _BuiltinAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();
out << '(' << builtinName << ')';

}

_BuiltinAttr *_BuiltinAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) _BuiltinAttr(builtinName, SourceLoc);
}



void _SemanticsAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();
out << '(' << semanticsKind << ')';

}

_SemanticsAttr *_SemanticsAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) _SemanticsAttr(semanticsKind, SourceLoc);
}



void _DebugAttr::printPretty(llvm::raw_ostream &out) const {

   out << "@" << getSpelling();

}

_DebugAttr *_DebugAttr::clone(ast::ASTContext &Ctx) const {
   return new(Ctx) _DebugAttr(SourceLoc);
}


#endif
#undef CDOT_ATTR_IMPL


#ifdef CDOT_ATTR_SEMA

void SemaPass::checkOpaqueAttr(Decl*, OpaqueAttr*) {}
void SemaPass::checkDiscardableResultAttr(Decl*, DiscardableResultAttr*) {}
void SemaPass::checkAutoClosureAttr(Decl*, AutoClosureAttr*) {}
void SemaPass::checkEscapingAttr(Decl*, EscapingAttr*) {}
void SemaPass::checkTestableAttr(Decl*, TestableAttr*) {}
void SemaPass::checkCompileTimeAttr(Decl*, CompileTimeAttr*) {}
void SemaPass::checkNoDeriveAttr(Decl*, NoDeriveAttr*) {}

#endif
#undef CDOT_ATTR_SEMA

