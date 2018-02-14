//
// Created by Jonas Zell on 08.09.17.
//

#ifndef CDOT_DECLPASS_H
#define CDOT_DECLPASS_H

#include <string>
#include <llvm/ADT/SmallPtrSet.h>

#include "AST/AstNode.h"
#include "AST/Attribute/Attribute.h"
#include "AST/Passes/NullASTVisitor.h"
#include "Basic/Mangle.h"
#include "lex/SourceLocation.h"
#include "Support/Casting.h"

namespace cdot {

enum class AccessModifier : unsigned char;

namespace module {
   class Module;
} // namespace module

class ObjectType;

namespace ast {

class ASTContext;
class DeclContext;
class SemaPass;

using std::string;
using namespace cdot;

class DeclPass: public NullASTVisitor<> {
public:
   explicit DeclPass(SemaPass &SP);

   ObjectType *getObjectTy(llvm::StringRef name);

   DeclContext &getDeclContext()
   {
      return *declContext;
   }

   DeclContext const& getDeclContext() const
   {
      return *declContext;
   }

   void visit(Statement *node);

   void doInitialPass();
   void run();

   void DeclareRecord(RecordDecl* R);
   void DeclareStruct(StructDecl *S);
   void DeclareProto(ProtocolDecl *P);
   void DeclareEnum(EnumDecl *E);
   void DeclareUnion(UnionDecl *U);
   void DeclareExtension(ExtensionDecl* node);

   void DeclareFunction(FunctionDecl *node);
   void DeclareTypedef(TypedefDecl *TD);
   void DeclareAlias(AliasDecl *node);

   module::Module *getModuleBeingCompiled();

   void doInitialPass(Statement* const& Stmt);

   void visitImportStmt(ImportStmt *node);
   void visitModule(module::Module &M);

   void visitCompoundStmt(CompoundStmt *node);

   void visitNamespaceDecl(NamespaceDecl *NS);
   void visitUsingStmt(UsingStmt *node);

   void visitTemplateParamDecl(TemplateParamDecl *decl);

   void visitFunctionDecl(FunctionDecl *F);
   void visitFuncArgDecl(FuncArgDecl *node);

   void visitGlobalVarDecl(GlobalVarDecl *node);
   void visitGlobalDestructuringDecl(GlobalDestructuringDecl *node);

   void visitRecordDecl(RecordDecl *node);

   void visitStructDecl(StructDecl *node);
   void visitClassDecl(ClassDecl *node);
   void visitProtocolDecl(ProtocolDecl *node);
   void visitExtensionDecl(ExtensionDecl *node);
   void visitUnionDecl(UnionDecl *node);
   void visitEnumDecl(EnumDecl *node);

   void visitFieldDecl(FieldDecl *F);
   void visitPropDecl(PropDecl *node);
   void visitAssociatedTypeDecl(AssociatedTypeDecl *node);

   void visitMethodDecl(MethodDecl *M);
   void visitInitDecl(InitDecl *Init);
   void visitDeinitDecl(DeinitDecl *node);

   void visitTypedefDecl(TypedefDecl *node);
   void visitAliasDecl(AliasDecl *node);

   void visitTypeRef(TypeRef *node);

   void visitDebugStmt(DebugStmt *node);

   void visitStaticIfStmt(StaticIfStmt *node);
   void visitStaticAssertStmt(StaticAssertStmt *node);
   void visitStaticForStmt(StaticForStmt *node);

   void visitScoped(Statement *Stmt);

   void declareMemberwiseInitializer(StructDecl *S);
   void declareDefaultInitializer(StructDecl *S);
   void declareDefaultDeinitializer(StructDecl *S);

   enum ResolveStatus {
      Res_Success,
      Res_SubstituationFailure,
   };

   Type *resolveObjectTy(TypeRef *TS);

   Type *resolveTemplateTy(TypeRef *node, const string &name);

   Type *resolveTypedef(TypedefDecl *td,
                        TypeRef *node,
                        const string &name,
                        std::vector<TemplateArgExpr*>& templateArgs,
                        ResolveStatus *status = nullptr);

   Type *resolveAlias(TypeRef *node,
                      const string &name,
                      std::vector<TemplateArgExpr*>& templateArgs);

   Type* getResolvedType(TypeRef *node, ResolveStatus *status = nullptr);

   void resolveType(TypeRef *node, ResolveStatus *status = nullptr);

   void resolveMethodTemplateParams(MethodDecl *M);

   SemaPass &getSema() { return SP; }

   llvm::ArrayRef<size_t> getImportedNamespaces()
   { return importedNamespaces(); }

   void importNamespace(size_t id);

   TemplateParamDecl const* getTemplateParam(llvm::StringRef name);
   AssociatedTypeDecl const* getAssociatedType(llvm::StringRef name);

   llvm::ArrayRef<size_t> getImportsForFile(size_t sourceId) const
   {
      auto it = FileImports.find(sourceId);
      if (it == FileImports.end())
         return {};

      return it->second;
   }

   llvm::ArrayRef<module::Module*>
   getModuleImportsForFile(size_t sourceId) const
   {
      auto it = ModuleImportsByFile.find(sourceId);
      if (it == ModuleImportsByFile.end())
         return {};

      return it->second;
   }

   llvm::ArrayRef<size_t> importedNamespaces() const
   {
      return getImportsForFile(getCurrentSourceId());
   }

   llvm::ArrayRef<module::Module*> getImportedModules() const
   {
      return getModuleImportsForFile(getCurrentSourceId());
   }

   size_t getCurrentSourceId() const;
   RecordDecl *getCurrentRecord() const;
   NamespaceDecl *getCurrentNamespace() const;

   void pushDeclContext(DeclContext *Ctx);
   void popDeclContext();

   NamespaceDecl *getPrivateFileNamespace(AstNode *node)
   {
      return getPrivateFileNamespace(node->getSourceLoc().getSourceId());
   }

   NamespaceDecl *getPrivateFileNamespace(SourceLocation loc)
   {
      return getPrivateFileNamespace(loc.getSourceId());
   }

   NamespaceDecl *getPrivateFileNamespace(size_t sourceId);

   const SymbolMangler &getMangler() const
   {
      return mangle;
   }

   struct DeclScopeRAII {
      DeclScopeRAII(DeclPass &DP, DeclContext *Ctx)
         : DP(DP), declContext(DP.declContext)
      {
         DP.declContext = Ctx;
      }

      ~DeclScopeRAII()
      {
         DP.declContext = declContext;
      }

   private:
      DeclPass &DP;
      DeclContext *declContext;
   };

   struct DeclContextRAII {
      DeclContextRAII(DeclPass &DP, DeclContext *Ctx) : DP(DP)
      {
         DP.pushDeclContext(Ctx);
      }

      ~DeclContextRAII()
      {
         DP.popDeclContext();
      }

   private:
      DeclPass &DP;
   };

   friend class SemaPass;

protected:
   SemaPass &SP;
   DeclContext *declContext = nullptr;

   SymbolMangler mangle;

   llvm::SmallVector<ast::Statement*, 8> GlobalVariableDecls;

   std::unordered_map<size_t, std::vector<size_t>> FileImports;
   std::unordered_map<size_t, std::vector<module::Module*>> ModuleImportsByFile;

   // during parsing, statements that need to be resolved first (Typedefs,
   // dangling Extensions, NamedDecls with unresolved template parameters,
   // etc.) will be stored here. After parsing, this serves as a set storing
   // all decls that were already visited
   llvm::SmallPtrSet<NamedDecl*, 16> DeclSet;

   void CheckThrowsAttribute(CallableDecl *callable, Attribute &attr);

   template<class T>
   T *lookup(llvm::StringRef name, unsigned lvl = 2) const
   {
      return support::dyn_cast_or_null<T>(lookup(name, lvl));
   }

   ast::NamedDecl *lookup(llvm::StringRef name, unsigned lvl = 2) const;

   ast::RecordDecl *getRecord(llvm::StringRef name) const;
   ast::StructDecl *getStruct(llvm::StringRef name) const;
   ast::ClassDecl *getClass(llvm::StringRef name) const;
   ast::EnumDecl *getEnum(llvm::StringRef name) const;
   ast::UnionDecl *getUnion(llvm::StringRef name) const;
   ast::ProtocolDecl *getProtocol(llvm::StringRef name) const;

   NamespaceDecl *getNamespace(llvm::StringRef name) const;

   FunctionDecl *getAnyFn(llvm::StringRef name) const;
   GlobalVarDecl *getVariable(llvm::StringRef name) const;
   AliasDecl* getAlias(llvm::StringRef name) const;
   TypedefDecl *getTypedef(llvm::StringRef name) const;

   bool inGlobalDeclContext() const;

   void checkDuplicateFunctionDeclaration(CallableDecl *C,
                                          llvm::StringRef fnKind) const;

   const llvm::SmallPtrSet<NamedDecl*, 16> &getDeferredDecls() const
   {
      return DeclSet;
   }

   bool alreadyVisited(NamedDecl *decl);
   void addDanglingExtension(ExtensionDecl *decl);
   void deferTemplateParamResolving(NamedDecl *decl);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_DECLPASS_H
