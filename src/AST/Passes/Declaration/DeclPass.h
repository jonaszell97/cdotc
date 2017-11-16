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

enum class AccessModifier : unsigned int;
class Parser;

namespace llvm {

class MemoryBuffer;

} // namespace llvm

namespace cdot {
namespace cl {

class Record;
struct RecordTemplate;

} // namespace cl
} // namespace cdot

using std::string;
using namespace cdot;

class DeclPass: public AbstractPass {
public:
   explicit DeclPass();
   void run(std::vector<std::shared_ptr<CompoundStmt>> &roots) override;

   void declareGlobalTypedefs(std::vector<std::shared_ptr<Statement>>& statements);

   static void DeclareClass(ClassDecl *node);
   static void DeclareEnum(EnumDecl *node);
   static void DeclareUnion(UnionDecl *node);

   void DeclareRecordTemplate(RecordTemplateDecl *node);
   void DeclareFunctionTemplate(CallableTemplateDecl *node);
   void DeclareMethodTemplate(const string &recordName,
                              MethodTemplateDecl *node);

   void visit(CompoundStmt *node) override;

   void visit(NamespaceDecl *node) override;
   void visit(UsingStmt *node) override;
   void visit(EndOfFileStmt *node) override;

   void visit(FunctionDecl *node) override;
   void visit(FuncArgDecl *node) override;

   void visit(DeclStmt *node) override;

   void visit(ClassDecl *node) override;
   void visit(ExtensionDecl *node) override;
   void visit(UnionDecl *node) override;
   void visit(EnumDecl *node) override;

   void visit(FieldDecl *node) override;
   void visit(PropDecl *node) override;

   void visit(MethodDecl *node) override;
   void visit(ConstrDecl *node) override;
   void visit(DestrDecl *node) override;

   void visit(RecordTemplateDecl *node) override;
   void visit(CallableTemplateDecl *node) override;

   void visit(TypedefDecl *node) override;
   void visit(TypeRef *node) override;
   void visit(DeclareStmt *node) override;

   void visit(DebugStmt *node) override;
   void visit(Statement *node) override;
   void visit(Expression *node) override;

   string ns_prefix()
   {
      return currentNamespace.back().empty() ? "" : currentNamespace.back() + ".";
   }

   string declareVariable(
      const string &name,
      Type &type,
      AccessModifier access,
      AstNode *cause
   );

   Type declareFunction(
      Function::UniquePtr &&func,
      AstNode *cause
   );

   enum ResolveStatus {
      Res_Success,
      Res_SubstituationFailure
   };

   static BuiltinType *resolveObjectTy(
      TypeRef *node,
      std::vector<string>& importedNamespaces,
      std::vector<string>& currentNamespace,

      const string &name,
      TemplateArgList *argList,
      ResolveStatus *status = nullptr,
      const std::vector<TemplateArg> *templateArgs = nullptr,
      const std::vector<TemplateConstraint> *constraints = nullptr
   );

   static BuiltinType *resolveTemplateTy(
      const TypeRef *node,
      std::vector<string>& importedNamespaces,
      std::vector<string>& currentNamespace,

      const string &name,
      const std::vector<TemplateArg> *templateArgs = nullptr,
      const std::vector<TemplateConstraint> *constraints = nullptr
   );

   static BuiltinType *resolveTypedef(
      const TypeRef *node,
      std::vector<string>& importedNamespaces,
      std::vector<string>& currentNamespace,

      const string &name,
      ResolveStatus *status = nullptr,
      const std::vector<TemplateArg> *templateArgs = nullptr,
      const std::vector<TemplateConstraint> *constraints = nullptr
   );

   static BuiltinType* getResolvedType(
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

   static Function *declareFunctionInstantiation(
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


#endif //CDOT_DECLPASS_H
