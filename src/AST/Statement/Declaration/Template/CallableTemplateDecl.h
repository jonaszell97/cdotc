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

namespace cdot {
namespace ast {

class FuncArgDecl;

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

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == CallableTemplateDeclID
              || T->getTypeID() == MethodTemplateDeclID;
   }

protected:
   string name;
   std::unique_ptr<TokenStore> Store;

   std::vector<TemplateConstraint> constraints;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   std::shared_ptr<TypeRef> returnType;

   cdot::cl::CallableTemplate *templ;
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

   cdot::cl::CallableTemplate *getTempl() const;
   void setTempl(cdot::cl::CallableTemplate *templ);

   const std::vector<std::shared_ptr<Statement>> &getInstantiations() const;
   void setInstantiations(
      const std::vector<std::shared_ptr<Statement>> &Instantiations);
   void addInstantiation(std::shared_ptr<Statement> &&inst);

   std::unique_ptr<TokenStore> &getStore();
};

} // namespace ast
} // namespace cdot


#endif //CDOT_CALLABLETEMPLATEDECL_H
