//
// Created by Jonas Zell on 29.08.17.
//

#ifndef CDOT_ENUMCASEDECL_H
#define CDOT_ENUMCASEDECL_H

#include "../CallableDecl.h"

namespace cdot {
namespace ast {

class Expression;
class TypeRef;
class StaticExpr;

class EnumCaseDecl: public CallableDecl {
public:
   typedef std::vector<FuncArgDecl*> AssociatedTypeVec;
   EnumCaseDecl(std::string&& name, StaticExpr* rawValue,
                AssociatedTypeVec&& associatedTypes)
      : CallableDecl(EnumCaseDeclID, (AccessModifier)0, move(name), nullptr,
                     move(associatedTypes), {}, nullptr, OperatorInfo()),
        rawValExpr(rawValue)
   {}

   EnumCaseDecl(std::string&& name, AssociatedTypeVec&& associatedTypes)
      : CallableDecl(EnumCaseDeclID, (AccessModifier)0, move(name), nullptr,
                     move(associatedTypes), {}, nullptr, OperatorInfo()),
        rawValExpr(nullptr)
   {}

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == EnumCaseDeclID;
   }

   friend class TransformImpl;

protected:
   StaticExpr* rawValExpr;

   long long rawValue = 0;

public:
   StaticExpr* getRawValExpr() const
   {
      return rawValExpr;
   }

   void setRawValExpr(StaticExpr *rawValExpr)
   {
      EnumCaseDecl::rawValExpr = rawValExpr;
   }

   long long getRawValue() const
   {
      return rawValue;
   }

   void setRawValue(long rawValue)
   {
      EnumCaseDecl::rawValue = rawValue;
   }

   bool hasRawValue() const
   {
      return rawValExpr != nullptr;
   }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_ENUMCASEDECL_H
