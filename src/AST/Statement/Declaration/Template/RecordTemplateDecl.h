//
// Created by Jonas Zell on 03.11.17.
//

#ifndef CDOT_RECORDTEMPLATEDECL_H
#define CDOT_RECORDTEMPLATEDECL_H

#include "../../Statement.h"

#include "../../../../Template/TokenStore.h"
#include "../../../../Variant/Type/Generic.h"

enum class RecordTemplateKind : unsigned char {
   STRUCT,
   CLASS,
   ENUM,
   UNION,
   PROTOCOL
};

namespace cdot {

struct TemplateConstraint;
class TokenStore;

namespace cl {

struct RecordTemplateInstantiation;
struct RecordTemplate;

} // namespace cl

} // namespace cdot

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

   std::vector<AstNode::SharedPtr> get_children() override
   {
      return {};
   }

   NodeType get_type() override
   {
      return NodeType::RECORD_TEMPLATE_DECL;
   }

   ASTNODE_ACCEPT_PASSES

protected:
   RecordTemplateKind kind;
   string name;

   std::unique_ptr<TokenStore> Store;

   std::vector<TemplateConstraint> constraints;
   std::vector<Initializer> initializers;

   cl::RecordTemplateInstantiation *outerTemplate = nullptr;

   cl::RecordTemplate *templ;
   std::vector<std::shared_ptr<Statement>> Instantiations;

public:
   RecordTemplateKind getKind() const;
   void setKind(RecordTemplateKind kind);

   const string &getName() const;
   void setName(const string &name);

   std::unique_ptr<TokenStore> &getStore();

   std::vector<TemplateConstraint> &getConstraints();
   void setConstraints(const std::vector<TemplateConstraint> &constraints);

   cl::RecordTemplateInstantiation *getOuterTemplate() const;
   void setOuterTemplate(cl::RecordTemplateInstantiation *outerTemplate);

   std::vector<Initializer> &getInitializers();
   void setInitializers(const std::vector<Initializer> &initializers);

   cl::RecordTemplate *getTempl() const;
   void setTempl(cl::RecordTemplate *Template);

   const std::vector<std::shared_ptr<Statement>> &getInstantiations() const;
   void setInstantiations(
      const std::vector<std::shared_ptr<Statement>> &Instantiations);
   void addInstantiation(std::shared_ptr<Statement> &&inst);
};


#endif //CDOT_RECORDTEMPLATEDECL_H
