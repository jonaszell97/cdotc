//
// Created by Jonas Zell on 08.09.17.
//

#ifndef CDOT_DECLPASS_H
#define CDOT_DECLPASS_H

#include <string>

#include "../AbstractPass.h"
#include "../SemanticAnalysis/Function.h"
#include "../../Attribute/Attribute.h"
#include "../../../Basic/Mangle.h"

namespace llvm {

class MemoryBuffer;

} // namespace llvm

namespace cdot {

struct Namespace;
enum class AccessModifier : unsigned int;

namespace cl {

class Record;
class AssociatedType;

} // namespace cl

namespace ast {

class SemaPass;

using std::string;
using namespace cdot;

class DeclPass: public AbstractPass<DeclPass, void> {
public:
   explicit DeclPass(SemaPass &SP);
   explicit DeclPass(SemaPass &SP, size_t sourceId);

   explicit DeclPass(SemaPass &SP, cl::Record *R, bool includeLast = true);
   explicit DeclPass(SemaPass &SP,Callable *C, bool includeLast = true);

   explicit DeclPass(SemaPass &SP, Namespace *NS);

   void setCurrentNamespace(Namespace *NS);
   void setRecordCtx(cl::Record *R, bool includeLast = true);

   void resetCtx()
   {
      currentNamespace.clear();
      currentSourceId = 0;
      SelfStack.clear();
   }

   void run(std::vector<CompilationUnit> &CUs);

   static void addGlobalStatement(Statement *node)
   {
      GlobalStatements.push_back(node);
   }

   void visitRecordDeclIfNotAlreadyVisited(RecordDecl *node);

   void visitGlobalStmts();
   void resolveTemplateParams();
   void resolveExtensions();

   void DeclareRecord(const std::shared_ptr<RecordDecl> &node);
   void DeclareClass(ClassDecl *node);
   void DeclareProto(ProtocolDecl *node);
   void DeclareEnum(EnumDecl *node);
   void DeclareUnion(UnionDecl *node);
   void DeclareExtension(std::shared_ptr<ExtensionDecl> node);

   void DeclareFunction(FunctionDecl *node);
   void DeclareTypedef(TypedefDecl *node);

   void VisitNode(AstNode *node);
   void VisitNode(const std::shared_ptr<AstNode> &node)
   {
      VisitNode(node.get());
   }

   void visitCompoundStmt(CompoundStmt *node);

   void visitNamespaceDecl(NamespaceDecl *node);
   void visitUsingStmt(UsingStmt *node);

   void visitFunctionDecl(FunctionDecl *node);
   void visitFuncArgDecl(FuncArgDecl *node);

   void visitLocalVarDecl(LocalVarDecl *node);
   void visitGlobalVarDecl(GlobalVarDecl *node);

   void visitRecordDecl(RecordDecl *node);

   void visitClassDecl(ClassDecl *node);
   void visitProtocolDecl(ProtocolDecl *node);
   void visitExtensionDecl(ExtensionDecl *node);
   void visitUnionDecl(UnionDecl *node);
   void visitEnumDecl(EnumDecl *node);

   void visitFieldDecl(FieldDecl *node);
   void visitPropDecl(PropDecl *node);
   void visitAssociatedTypeDecl(AssociatedTypeDecl *node);

   void visitMethodDecl(MethodDecl *node);
   void visitConstrDecl(ConstrDecl *node);
   void visitDestrDecl(DestrDecl *node);

   void visitTypedefDecl(TypedefDecl *node);
   void visitAliasDecl(AliasDecl *node);

   void visitTypeRef(TypeRef *node);
   void visitDeclareStmt(DeclareStmt *node);

   void visitDebugStmt(DebugStmt *node);

   void visitStaticIfStmt(StaticIfStmt *node);
   void visitStaticAssertStmt(StaticAssertStmt *node);
   void visitStaticForStmt(StaticForStmt *node);

   string ns_prefix();

   string declareVariable(
      const string &name,
      QualType type,
      AccessModifier access,
      Statement *node
   );

   enum ResolveStatus {
      Res_Success,
      Res_SubstituationFailure,
   };

   Type *resolveObjectTy(TypeRef *node,
                         const string &name,
                         std::vector<TemplateArg>& templateArgs,
                         ResolveStatus *status = nullptr);

   Type *resolveTemplateTy(TypeRef *node, const string &name);

   Type *resolveTypedef(TypeRef *node,
                        const string &name,
                        std::vector<TemplateArg>& templateArgs,
                        ResolveStatus *status = nullptr);

   Type *resolveAlias(TypeRef *node,
                      const string &name,
                      std::vector<TemplateArg>& templateArgs);

   Type* getResolvedType(TypeRef *node, ResolveStatus *status = nullptr);

   void resolveType(TypeRef *node, ResolveStatus *status = nullptr);

   void resolveMethodTemplateParams(std::vector<TemplateParameter> &Params);

   void resolveTemplateParams(std::vector<TemplateParameter> &params);

   void pushNamespace(const string &ns,
                      bool isAnonymous = false);

   void popNamespace();

   SemaPass &getSema() { return SP; }

   Namespace *getDeclNamespace() const;

   llvm::ArrayRef<size_t> getCurrentNamespaceVec() { return currentNamespace; }
   llvm::ArrayRef<size_t> getImportedNamespaces()
   { return importedNamespaces(); }

   size_t getCurrentNamespace() const
   {
      return currentNamespace.empty() ? 0 : currentNamespace.back();
   }

   void importNamespace(const string &ns);
   void importNamespace(size_t id);

   void pushTemplateParams(std::vector<TemplateParameter>* params)
   {
      templateParamStack.push_back(params);
   }

   void popTemplateParams()
   {
      templateParamStack.pop_back();
   }

   const TemplateParamStack &getTemplateParamStack() const
   {
      return templateParamStack;
   }

   TemplateParameter *getTemplateParam(llvm::StringRef name);

   void beginRecordScope(cl::Record *Rec);
   void endRecordScope();

   static llvm::ArrayRef<size_t> getImportsForFile(size_t sourceId)
   {
      return FileImports.find(sourceId)->second;
   }

   llvm::ArrayRef<size_t> importedNamespaces() const
   {
      return FileImports[currentSourceId];
   }

   void importFileImports(size_t sourceId)
   {
      currentSourceId = sourceId;
   }

   size_t getCurrentSourceId() const
   {
      return currentSourceId;
   }

   static void beginFile(size_t sourceId);

   Namespace *getPrivateFileNamespace(AstNode *node);

   const SymbolMangler &getMangler() const
   {
      return mangle;
   }

   friend class SemaPass;

protected:
   SemaPass &SP;
   size_t currentSourceId;
   llvm::SmallVector<size_t, 8> currentNamespace;

   std::vector<cl::Record*> SelfStack;
   TemplateParamStack templateParamStack;

   SymbolMangler mangle;

   static llvm::SmallVector<ast::Statement*, 8> GlobalVariableDecls;

   static std::unordered_map<size_t, llvm::SmallVector<size_t, 8>>
      FileImports;

   static llvm::SmallPtrSet<RecordDecl*, 4> VisitedRecordDecls;
   static std::vector<Statement*> GlobalStatements;
   static std::vector<cl::Record*> RecordsWithUnresolvedTemplateParams;
   static std::vector<std::shared_ptr<ExtensionDecl>> DanglingExtensions;

   void CheckThrowsAttribute(Callable *callable, Attribute &attr);
   bool checkProtocolConformance(cl::Record *R);

   cl::Record *getRecord(llvm::StringRef name);
   cl::Struct *getStruct(llvm::StringRef name);
   cl::Enum *getEnum(llvm::StringRef name);
   cl::Union *getUnion(llvm::StringRef name);

   Namespace *getNamespace(llvm::StringRef name);
   Function *getAnyFn(llvm::StringRef name);
   Variable *getVariable(llvm::StringRef name);
   llvm::ArrayRef<Alias*> getAliases(llvm::StringRef name);
   Typedef *getTypedef(llvm::StringRef name);
   llvm::ArrayRef<Function*> getFunctionOverloads(llvm::StringRef name);

   size_t isNamespace(llvm::StringRef name);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_DECLPASS_H
