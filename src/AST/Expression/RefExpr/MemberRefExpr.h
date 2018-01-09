//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_MEMBERREFEXPR_H
#define CDOT_MEMBERREFEXPR_H

#include "../Expression.h"
#include "../../Passes/SemanticAnalysis/Record/Record.h"

namespace cdot {
namespace ast {

class CallExpr;

enum class MemberKind : unsigned {
   Unknown,
   Alias,
   Accessor,
   TupleAccess,
   UnionAccess,
   Type,
   GlobalVariable,
   Namespace,
   Field,
   EnumRawValue,
   AssociatedType,
   Function,
};

class MemberRefExpr: public IdentifiedExpr {
public:
   explicit MemberRefExpr(string &&, bool pointerAccess = false);
   explicit MemberRefExpr(size_t, bool pointerAccess = false);

   ~MemberRefExpr();

   typedef std::shared_ptr<MemberRefExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == MemberRefExprID;
   }

protected:
   MemberKind kind = MemberKind::Unknown;
   std::vector<TemplateArg> templateArgs;

   Record *record = nullptr;
   QualType fieldType;

   bool is_pointer_access : 1;
   bool is_tuple_access   : 1;

   Variant aliasVal;

   union {
      Type *metaType = nullptr;
      Callable *callable;
   };

   size_t tupleIndex;

   std::shared_ptr<CallExpr> getterOrSetterCall = nullptr;

public:
   const std::shared_ptr<CallExpr> &getGetterOrSetterCall() const;
   void setGetterOrSetterCall(
      const std::shared_ptr<CallExpr> &getterOrSetterCall);

   Type *getMetaType() const;
   void setMetaType(Type *metaType);

   bool isPointerAccess() const;
   void setIsPointerAccess(bool is_pointer_access);

   Record *getRecord() const
   {
      return record;
   }

   void setRecord(Record *record)
   {
      MemberRefExpr::record = record;
   }

   const QualType &getFieldType() const;
   void setFieldType(const QualType &fieldType);

   size_t getTupleIndex() const;

   std::vector<TemplateArg> const& getTemplateArgs() const
   {
      return templateArgs;
   }

   void setTemplateArgs(std::vector<TemplateArg> &&templateArgs)
   {
      MemberRefExpr::templateArgs = std::move(templateArgs);
   }

   const Variant &getAliasVal() const
   {
      return aliasVal;
   }

   void setAliasVal(Variant &&aliasVal)
   {
      MemberRefExpr::aliasVal = std::move(aliasVal);
      kind = MemberKind::Alias;
   }

   MemberKind getKind() const
   {
      return kind;
   }

   void setKind(MemberKind kind)
   {
      MemberRefExpr::kind = kind;
   }

   Callable *getCallable() const
   {
      return callable;
   }

   void setCallable(Callable *callable)
   {
      MemberRefExpr::callable = callable;
   }

   bool isTupleAccess() const
   {
      return is_tuple_access;
   }

   void isTupleAccess(bool is_tuple_access)
   {
      MemberRefExpr::is_tuple_access = is_tuple_access;
   }
};

class EnumCaseExpr: public IdentifiedExpr {
public:
   using ArgList = std::vector<std::shared_ptr<Expression>>;

   EnumCaseExpr(std::string &&caseName,
                ArgList &&args = {})
      : IdentifiedExpr(EnumCaseExprID, move(caseName)),
        args(move(args)), en(nullptr)
   {}

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == EnumCaseExprID;
   }

private:
   ArgList args;
   cl::Enum *en;

public:
   ArgList &getArgs()
   {
      return args;
   }

   ArgList const& getArgs() const
   {
      return args;
   }

   cl::Enum *getEnum() const
   {
      return en;
   }

   void setEnum(cl::Enum *en)
   {
      EnumCaseExpr::en = en;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_MEMBERREFEXPR_H
