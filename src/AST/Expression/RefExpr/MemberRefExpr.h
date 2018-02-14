//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_MEMBERREFEXPR_H
#define CDOT_MEMBERREFEXPR_H

#include "../Expression.h"

namespace cdot {
namespace ast {

class CallExpr;
class EnumDecl;
class CallableDecl;
class VarDecl;
class FieldDecl;

enum class MemberKind : unsigned {
   Unknown,
   Alias,
   Accessor,
   TupleAccess,
   UnionAccess,
   Type,
   GlobalVariable,
   StaticField,
   Namespace,
   Field,
   EnumRawValue,
   AssociatedType,
   Function,
};

class MemberRefExpr: public IdentifiedExpr {
public:
   explicit MemberRefExpr(std::string &&, bool pointerAccess = false);
   explicit MemberRefExpr(size_t, bool pointerAccess = false);

   ~MemberRefExpr();

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == MemberRefExprID;
   }

   friend class TransformImpl;

protected:
   MemberKind kind = MemberKind::Unknown;
   std::vector<TemplateArgExpr*> templateArgs;

   RecordDecl *record = nullptr;
   QualType fieldType;

   bool pointerAccess : 1;
   bool tupleAccess   : 1;
   bool lhsOfAssignment   : 1;

   union {
      Type *metaType = nullptr;
      CallableDecl *callable;
      VarDecl *globalVar;
      FieldDecl *staticFieldDecl;
      Variant *aliasVal;
      MethodDecl *accessorMethod;
   };

   size_t tupleIndex;

public:
   RecordDecl *getRecord() const
   {
      return record;
   }

   void setRecord(RecordDecl *record)
   {
      this->record = record;
   }
   
   std::vector<TemplateArgExpr*> const& getTemplateArgs() const
   {
      return templateArgs;
   }

   std::vector<TemplateArgExpr*>& getTemplateArgRef()
   {
      return templateArgs;
   }

   void setTemplateArgs(std::vector<TemplateArgExpr*> &&templateArgs);

   const Variant &getAliasVal() const
   {
      assert(aliasVal && "not an alias");
      return *aliasVal;
   }

   void setAliasVal(Variant *aliasVal)
   {
      this->aliasVal = aliasVal;
      kind = MemberKind::Alias;
   }

   MemberKind getKind() const
   {
      return kind;
   }

   void setKind(MemberKind kind)
   {
      this->kind = kind;
   }

   CallableDecl *getCallable() const
   {
      return callable;
   }

   void setCallable(CallableDecl *callable)
   {
      this->callable = callable;
   }

   bool isTupleAccess() const
   {
      return tupleAccess;
   }

   void isTupleAccess(bool is_tuple_access)
   {
      this->tupleAccess = is_tuple_access;
   }

   const QualType &getFieldType() const
   {
      return fieldType;
   }

   void setFieldType(const QualType &fieldType)
   {
      this->fieldType = fieldType;
   }

   size_t getTupleIndex() const
   {
      return tupleIndex;
   }

   bool isPointerAccess() const
   {
      return pointerAccess;
   }

   void setIsPointerAccess(bool is_pointer_access)
   {
      this->pointerAccess = is_pointer_access;
   }

   Type *getMetaType() const
   {
      return metaType;
   }

   void setMetaType(Type *metaType)
   {
      this->metaType = metaType;
   }

   void setGlobalVar(VarDecl *globalVar)
   {
      kind = MemberKind::GlobalVariable;
      MemberRefExpr::globalVar = globalVar;
   }

   FieldDecl *getStaticFieldDecl() const
   {
      return staticFieldDecl;
   }

   void setFieldDecl(FieldDecl *staticFieldDecl)
   {
      kind = MemberKind::StaticField;
      MemberRefExpr::staticFieldDecl = staticFieldDecl;
   }

   MethodDecl *getAccessorMethod() const
   {
      return accessorMethod;
   }

   void setAccessorMethod(MethodDecl *accessorMethod)
   {
      MemberRefExpr::accessorMethod = accessorMethod;
   }

   bool isLhsOfAssignment() const
   {
      return lhsOfAssignment;
   }

   void setLhsOfAssignment(bool lhsOfAssignment)
   {
      MemberRefExpr::lhsOfAssignment = lhsOfAssignment;
   }

   VarDecl *getGlobalVar() const
   {
      return globalVar;
   }
};

class EnumCaseExpr: public IdentifiedExpr {
public:
   using ArgList = std::vector<Expression* >;

   EnumCaseExpr(std::string &&caseName,
                ArgList &&args = {})
      : IdentifiedExpr(EnumCaseExprID, move(caseName), true),
        args(move(args)), en(nullptr)
   {}

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == EnumCaseExprID;
   }

   friend class TransformImpl;

private:
   ArgList args;
   EnumDecl *en;

public:
   ArgList &getArgs()
   {
      return args;
   }

   ArgList const& getArgs() const
   {
      return args;
   }

   EnumDecl *getEnum() const
   {
      return en;
   }

   void setEnum(EnumDecl *en)
   {
      EnumCaseExpr::en = en;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_MEMBERREFEXPR_H
