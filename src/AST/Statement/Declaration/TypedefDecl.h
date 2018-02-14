//
// Created by Jonas Zell on 01.08.17.
//

#ifndef CDOT_TYPEDEFDECL_H
#define CDOT_TYPEDEFDECL_H

#include "NamedDecl.h"
#include "AST/Expression/StaticExpr.h"

namespace cdot {
namespace ast {

class ConstraintExpr;
class TypeRef;

class TypedefDecl : public NamedDecl {
public:
   TypedefDecl(AccessModifier access,
               std::string&& name,
               TypeRef* origin)
      : NamedDecl(TypedefDeclID, access, move(name), {}),
        origin(origin)
   {}

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TypedefDeclID;
   }

   friend class TransformImpl;

protected:
   TypeRef* origin;
   std::vector<TemplateParamDecl*> templateParams;

public:
   TypeRef* getOriginTy() const
   {
      return origin;
   }

   void setTemplateParams(std::vector<TemplateParamDecl *> &&templateParams)
   {
      TypedefDecl::templateParams = move(templateParams);
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }
};

class AliasDecl: public NamedDecl,
                 public DeclContext,
                 public llvm::FoldingSetNode {
public:
   AliasDecl(std::string &&name,
             std::vector<StaticExpr* > &&constraints,
             StaticExpr* aliasExpr)
      : NamedDecl(AliasDeclID, (AccessModifier)0, move(name),
                  move(constraints)),
        DeclContext(AliasDeclID),
        aliasExpr(aliasExpr)
   { }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == AliasDeclID;
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, instantiationInfo->specializedTemplate,
              instantiationInfo->templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, AliasDecl *Template,
                       sema::TemplateArgList &list) {
      ID.AddPointer(Template);
      list.Profile(ID);
   }

   friend class TransformImpl;

private:
   StaticExpr* aliasExpr;

   std::vector<TemplateParamDecl*> templateParams;
   InstantiationInfo<AliasDecl> *instantiationInfo = nullptr;

public:
   StaticExpr* getAliasExpr()
   {
      return aliasExpr;
   }

   void setAliasExpr(StaticExpr *aliasExpr)
   {
      AliasDecl::aliasExpr = aliasExpr;
   }

   StaticExpr* const& getAliasExpr() const
   {
      return aliasExpr;
   }

   const Variant &getVal() const
   {
      return aliasExpr->getEvaluatedExpr();
   }

   void setTemplateParams(std::vector<TemplateParamDecl *> &&templateParams)
   {
      AliasDecl::templateParams = move(templateParams);
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }

   InstantiationInfo <AliasDecl> *getInstantiationInfo() const
   {
      return instantiationInfo;
   }

   void setInstantiationInfo(InstantiationInfo <AliasDecl> *instantiationInfo)
   {
      AliasDecl::instantiationInfo = instantiationInfo;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TYPEDEFDECL_H
