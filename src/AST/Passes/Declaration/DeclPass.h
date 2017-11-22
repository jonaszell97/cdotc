//
// Created by Jonas Zell on 08.09.17.
//

#ifndef CDOT_DECLPASS_H
#define CDOT_DECLPASS_H

#include <string>
#include <stack>

#include "../AbstractPass.h"
#include "../SemanticAnalysis/Function.h"
#include "../../Attribute/Attribute.h"

class Parser;

namespace llvm {

class MemoryBuffer;

} // namespace llvm

namespace cdot {

enum class AccessModifier : unsigned int;

namespace cl {

class Record;
struct RecordTemplate;

} // namespace cl
} // namespace cdot

namespace cdot {
namespace ast {

using std::string;
using namespace cdot;

class DeclPass: public AbstractPass {
public:
   explicit DeclPass();
   void run(std::vector<CompilationUnit> &CUs);

   void declareGlobalTypedefs(std::vector<std::shared_ptr<Statement>>& statements);

   static void DeclareClass(ClassDecl *node);
   static void DeclareEnum(EnumDecl *node);
   static void DeclareUnion(UnionDecl *node);

   void DeclareRecordTemplate(RecordTemplateDecl *node);
   void DeclareFunctionTemplate(CallableTemplateDecl *node);
   void DeclareMethodTemplate(const string &recordName,
                              MethodTemplateDecl *node);

   void VisitNode(AstNode *node);
   void VisitNode(const std::shared_ptr<AstNode> &node)
   {
      VisitNode(node.get());
   }

   void visit(AstNode *node)
   {

   }

   void visit(CompoundStmt *node);

   void visit(NamespaceDecl *node);
   void visit(UsingStmt *node);
   void visit(EndOfFileStmt *node);

   void visit(FunctionDecl *node);
   void visit(FuncArgDecl *node);

   void visit(DeclStmt *node);

   void visit(ClassDecl *node);
   void visit(ExtensionDecl *node);
   void visit(UnionDecl *node);
   void visit(EnumDecl *node);

   void visit(FieldDecl *node);
   void visit(PropDecl *node);

   void visit(MethodDecl *node);
   void visit(ConstrDecl *node);
   void visit(DestrDecl *node);

   void visit(RecordTemplateDecl *node);
   void visit(CallableTemplateDecl *node);

   void visit(TypedefDecl *node);
   void visit(TypeRef *node);
   void visit(DeclareStmt *node);

   void visit(DebugStmt *node);
   void visit(Statement *node);
   void visit(Expression *node);

   string ns_prefix()
   {
      return currentNamespace.back().empty() ? "" : currentNamespace.back() + ".";
   }

   string declareVariable(
      const string &name,
      QualType &type,
      AccessModifier access,
      AstNode *cause
   );

   QualType declareFunction(
      ast::Function::UniquePtr &&func,
      bool isExternC = false
   );

   enum ResolveStatus {
      Res_Success,
      Res_SubstituationFailure
   };

   static Type *resolveObjectTy(
      TypeRef *node,
      std::vector<string>& importedNamespaces,
      std::vector<string>& currentNamespace,

      const string &name,
      TemplateArgList *argList,
      ResolveStatus *status = nullptr,
      const std::vector<TemplateArg> *templateArgs = nullptr,
      const std::vector<TemplateConstraint> *constraints = nullptr
   );

   static Type *resolveTemplateTy(
      const TypeRef *node,
      std::vector<string>& importedNamespaces,
      std::vector<string>& currentNamespace,

      const string &name,
      const std::vector<TemplateArg> *templateArgs = nullptr,
      const std::vector<TemplateConstraint> *constraints = nullptr
   );

   static Type *resolveTypedef(
      const TypeRef *node,
      std::vector<string>& importedNamespaces,
      std::vector<string>& currentNamespace,

      const string &name,
      ResolveStatus *status = nullptr,
      const std::vector<TemplateArg> *templateArgs = nullptr,
      const std::vector<TemplateConstraint> *constraints = nullptr
   );

   static Type* getResolvedType(
      TypeRef *node,
      std::vector<string>& importedNamespaces,
      std::vector<string>& currentNamespace,
      ResolveStatus *status = nullptr,
      const std::vector<TemplateArg> *templateArgs = nullptr,
      const std::vector<TemplateConstraint> *constraints = nullptr
   );

   static void resolveType(
      TypeRef *node,
      std::vector<string>& importedNamespaces,
      std::vector<string>& currentNamespace,
      ResolveStatus *status = nullptr,
      const std::vector<TemplateArg> *templateArgs = nullptr,
      const std::vector<TemplateConstraint> *constraints = nullptr
   );

   static std::unique_ptr<Parser> prepareParser(
       cl::Template *Templ,
       std::vector<TemplateArg> const& templateArgs,
       bool isRecord = false
   );

   static cl::Record *declareRecordInstantiation(
      cl::RecordTemplate &Template,
      TemplateArgList *argList,
      bool *isNew = nullptr
   );

   static ast::Function *declareFunctionInstantiation(
      cl::CallableTemplate &Template,
      std::vector<TemplateArg> const& templateArgs,
      bool *isNew = nullptr
   );

   static cl::Method *declareMethodInstantiation(
      cl::MethodTemplate &Template,
      std::vector<TemplateArg> const& templateArgs,
      cl::Record *rec = nullptr,
      bool *isNew = nullptr
   );

   static void resolveTemplateArgs(
      TemplateArgList *&args,
      std::vector<TemplateConstraint> &constraints,
      const std::function<void (TypeRef*)> &resolver,
      AstNode *cause
   );

   static void resolveTemplateConstraints(
      std::vector<TemplateConstraint> &constraints,
      const std::function<void (TypeRef*)> &resolver
   );

   static void checkTemplateConstraintCompatability(
      TemplateArgList *argList,
      const std::vector<TemplateConstraint> &constraints,
      AstNode *cause,
      AstNode *decl
   );

   void pushNamespace(const string &ns, bool declare = true);
   void popNamespace();

   std::vector<string> &getCurrentNamespace()
   {
      return currentNamespace;
   }

   std::vector<string> &getImportedNamespaces()
   {
      return importedNamespaces;
   }

   void importNamespace(const string &ns)
   {
      importedNamespaces.push_back(ns);
   }

protected:
   std::vector<string> currentNamespace = {""};
   std::vector<string> importedNamespaces = {""};

   std::vector<std::vector<TemplateConstraint>*> GenericsStack;

   void CheckThrowsAttribute(Callable *callable, Attribute &attr);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_DECLPASS_H
