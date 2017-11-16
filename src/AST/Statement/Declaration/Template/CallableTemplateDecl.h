//
// Created by Jonas Zell on 04.11.17.
//

#ifndef CDOT_CALLABLETEMPLATEDECL_H
#define CDOT_CALLABLETEMPLATEDECL_H

#include "../../Statement.h"
#include "../../../../Template/TokenStore.h"
#include "../../../../Variant/Type/Generic.h"

namespace cdot {

class TokenStore;

namespace cl {

class CallableTemplate;

} // namespace cl

} // namespace cdot

class CallableTemplateDecl: public Statement {
public:
   CallableTemplateDecl(
      string &&name,
      std::unique_ptr<TokenStore> &&Store,
      std::vector<TemplateConstraint> &&constraints,
      std::vector<std::shared_ptr<FuncArgDecl>> &&args,
      std::shared_ptr<TypeRef> &&returnType
   );

   typedef std::shared_ptr<CallableTemplateDecl> SharedPtr;

   std::vector<AstNode::SharedPtr> get_children() override
   {
      return {};
   }

   NodeType get_type() override
   {
      return NodeType::CALLABLE_TEMPLATE_DECL;
   }

   ASTNODE_ACCEPT_PASSES

protected:
   string name;
   std::unique_ptr<TokenStore> Store;

   std::vector<TemplateConstraint> constraints;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   std::shared_ptr<TypeRef> returnType;

   cl::CallableTemplate *templ;
   std::vector<std::shared_ptr<Statement>> Instantiations;

public:
   const string &getName() const;
   void setName(const string &name);

   const std::vector<TemplateConstraint> &getConstraints() const;
   void setConstraints(const std::vector<TemplateConstraint> &constraints);

   std::vector<std::shared_ptr<FuncArgDecl>> &getArgs();
   void setArgs(const std::vector<std::shared_ptr<FuncArgDecl>> &args);

   std::shared_ptr<TypeRef> &getReturnType();
   void setReturnType(const std::shared_ptr<TypeRef> &returnType);

   cl::CallableTemplate *getTempl() const;
   void setTempl(cl::CallableTemplate *templ);

   const std::vector<std::shared_ptr<Statement>> &getInstantiations() const;
   void setInstantiations(
      const std::vector<std::shared_ptr<Statement>> &Instantiations);
   void addInstantiation(std::shared_ptr<Statement> &&inst);

   std::unique_ptr<TokenStore> &getStore();


};


#endif //CDOT_CALLABLETEMPLATEDECL_H
