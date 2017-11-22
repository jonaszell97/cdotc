//
// Created by Jonas Zell on 03.11.17.
//

#ifndef CDOT_RECORDTEMPLATEDECL_H
#define CDOT_RECORDTEMPLATEDECL_H

#include "../../Statement.h"

#include "../../../../Template/TokenStore.h"
#include "../../../../Variant/Type/Generic.h"

namespace cdot {

struct TemplateConstraint;
class TokenStore;

namespace cl {

struct RecordTemplateInstantiation;
struct RecordTemplate;

} // namespace cl

} // namespace cdot

namespace cdot {
namespace ast {

class FuncArgDecl;

enum class RecordTemplateKind : unsigned char {
   STRUCT,
   CLASS,
   ENUM,
   UNION,
   PROTOCOL
};

class RecordTemplateDecl: public Statement {
public:
   struct Initializer {
      Initializer(std::vector<std::shared_ptr<FuncArgDecl>> &&args)
         : args(args) {}
      std::vector<std::shared_ptr<FuncArgDecl>> args;
   };

   RecordTemplateDecl(
      RecordTemplateKind kind,
      string &&name,
      std::unique_ptr<TokenStore> &&Store,
      std::vector<TemplateConstraint> &&constraints,
      std::vector<Initializer> && initializers
   );

   typedef std::shared_ptr<RecordTemplateDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == RecordTemplateDeclID;
   }

protected:
   RecordTemplateKind kind;
   string name;

   std::unique_ptr<TokenStore> Store;

   std::vector<TemplateConstraint> constraints;
   std::vector<Initializer> initializers;

   cdot::cl::RecordTemplateInstantiation *outerTemplate = nullptr;

   cdot::cl::RecordTemplate *templ;
   std::vector<std::shared_ptr<Statement>> Instantiations;

public:
   RecordTemplateKind getKind() const;
   void setKind(RecordTemplateKind kind);

   const string &getName() const;
   void setName(const string &name);

   std::unique_ptr<TokenStore> &getStore();

   std::vector<TemplateConstraint> &getConstraints();
   void setConstraints(const std::vector<TemplateConstraint> &constraints);

   cdot::cl::RecordTemplateInstantiation *getOuterTemplate() const;
   void setOuterTemplate(cdot::cl::RecordTemplateInstantiation *outerTemplate);

   std::vector<Initializer> &getInitializers();
   void setInitializers(const std::vector<Initializer> &initializers);

   cdot::cl::RecordTemplate *getTempl() const;
   void setTempl(cdot::cl::RecordTemplate *Template);

   const std::vector<std::shared_ptr<Statement>> &getInstantiations() const;
   void setInstantiations(
      const std::vector<std::shared_ptr<Statement>> &Instantiations);
   void addInstantiation(std::shared_ptr<Statement> &&inst);
};

} // namespace ast
} // namespace cdot


#endif //CDOT_RECORDTEMPLATEDECL_H
