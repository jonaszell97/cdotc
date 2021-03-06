#ifndef CDOT_IMPORTERIMPL_H
#define CDOT_IMPORTERIMPL_H

#include "cdotc/AST/Type.h"
#include "cdotc/Basic/DeclarationName.h"
#include "cdotc/ClangImporter/ClangImporter.h"
#include "cdotc/Lex/SourceLocation.h"
#include "cdotc/Lex/Token.h"

#include <clang/AST/Type.h>
#include <clang/Basic/SourceLocation.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>

#include <vector>

namespace clang {
   class CompilerInstance;
   class CompilerInvocation;
   class Decl;
   class DeclarationName;
   class DiagnosticsEngine;
   class FrontendInputFile;
   class IdentifierInfo;
   class Token;
   class VarDecl;
} // namespace clang

namespace cdot {

namespace ast {
   class Decl;
   class EnumDecl;
   class Expression;
   class FunctionDecl;
   class GlobalVarDecl;
   class Statement;
   class StructDecl;
}

class ClangImporter::ImporterImpl {
public:
   /// The cdot compiler instance.
   CompilerInstance &CI;

   /// Invocation arguments for clang.
   std::vector<std::string> InvocationArgStrings;

   /// The clang compiler invocation.
   std::shared_ptr<clang::CompilerInvocation> Invocation;

   /// The clang diagnostic options.
   clang::DiagnosticOptions *ClangDiagnosticOpts;

   /// The clang diagnostics engine.
   clang::DiagnosticsEngine *ClangDiags = nullptr;

   /// The current clang compiler instance.
   std::unique_ptr<clang::CompilerInstance> Instance;

   /// True iff the current instance is C++.
   bool IsCXX = false;

   /// The names of macros that were defined in this module.
   std::vector<clang::IdentifierInfo*> MacroNames;

   /// Map from clang source file IDs to our own ones.
   llvm::DenseMap<clang::FileID, unsigned> FileIDMap;

   /// Map from clang decls to translated ones.
   llvm::DenseMap<clang::Decl*, ast::Decl*> DeclMap;

   /// Map from names to imported functions with C linkage.
   llvm::StringMap<ast::FunctionDecl*> CLinkageFuncs;

   /// The source ID of the file we're currently importing.
   unsigned SourceID = 0;

   /// The base offset of the file we're currently importing.
   unsigned BaseOffset = 0;

   /// The number of unnamed structs we've imported.
   unsigned NumUnnamedStructs = 0;

   /// The number of unnamed enums we've imported.
   unsigned NumUnnamedEnums = 0;

   /// The number of unnamed other decls we've imported.
   unsigned NumUnnamedDecls = 0;

   /// C'tor.
   explicit ImporterImpl(CompilerInstance &CI);

   /// Setup the clang compiler invocation.
   void Initialize();

   /// Import the module in File into the given cdot module.
   bool importModule(StringRef File, ast::DeclContext *IntoMod,
                     clang::FrontendInputFile &InputFile,
                     SourceLocation ImportLoc,
                     bool IsCXX,
                     bool Optional = false);

   /// Import the currently parsed macros.
   void importMacros(ast::DeclContext *DC);

   /// Import the declarations of the file.
   void importDecls(clang::ASTContext &C, ast::DeclContext *DC);

   /// Import the declarations of the this decl context.
   void importDecls(clang::DeclContext *ClangDC, ast::DeclContext *DC);

   /// Translate a clang source location into a CDot one.
   SourceLocation getSourceLoc(clang::SourceLocation Loc);

   /// Translate a clang source range into a CDot one.
   SourceRange getSourceLoc(clang::SourceRange Loc);

   /// Translate a clang type into a CDot one.
   QualType getType(clang::QualType Ty);

   /// Translate a clang token into a CDot one.
   lex::Token getToken(const clang::Token &Tok);

   /// Translate a declaration name.
   DeclarationName getName(const clang::DeclarationName &Name);

   /// Transform a clang token into an expression.
   ast::Expression *expressionFromLiteralToken(const clang::Token &Tok);

   /// Parse an expression from a sequence of clang tokens.
   ast::Expression *parseExpression(ArrayRef<clang::Token> Toks);

   /// Import a declaration from a clang Decl.
   ast::Decl *importDecl(clang::Decl *D);

   /// Import a struct declaration.
   ast::StructDecl *importStruct(clang::RecordDecl *ClangRec);

   /// Import an enum declaration.
   ast::EnumDecl *importEnum(clang::EnumDecl *ClangE);

   /// Import an union declaration.
   ast::StructDecl *importUnion(clang::RecordDecl *ClangU);

   /// Import a typedef declaration.
   ast::AliasDecl *importTypedef(clang::TypedefNameDecl *ClangTD);

   /// Import a global variable declaration.
   ast::GlobalVarDecl* importGlobalVar(clang::VarDecl *ClangVar);
};

} // namespace cdot

#endif //CDOT_IMPORTERIMPL_H
