//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include "Attribute/Attribute.h"
#include "Passes/AbstractPass.h"

#include "../Variant/Type/Type.h"
#include "../Token.h"

#define ASTNODE_ACCEPT_PASSES \
   void accept(AbstractPass *pass) override {\
      pass->visit(this); \
   }

namespace cdot {

class BuiltinType;
class GenericType;

class Callable;

class TemplateArgList;
struct TemplateConstraint;

namespace cl {

struct Method;
class Class;
class Record;
class Enum;
class Union;

} // namespace cl
} // namespace cdot


class Function;

class DeclStmt;
class Expression;

using cdot::Attribute;

class CompoundStmt;
class HeaderGen;

class ConstExprPass;
class CodeGen;
class SemaPass;
class DeclPass;

enum class AccessModifier : unsigned int;

enum class NodeType {
   COLLECTION_LITERAL, LAMBDA_EXPR, INTEGER_LITERAL,
   FLOATING_LITERAL, NONE_LITERAL,
   STRING_LITERAL, EXPRESSION, BOOL_LITERAL, CHAR_LITERAL,
   ARRAY_ACCESS_EXPR, CALL_EXPR, IDENTIFIER_EXPR, MEMBER_EXPR,
   METHOD_CALL_EXPR, REF_EXPR,
   BINARY_OPERATOR, UNARY_OPERATOR, TERTIARY_OPERATOR, TUPLE_LITERAL,
   STRING_INTERPOLATION,

   EXPLICIT_CAST_EXPR, IMPLICIT_CAST_EXPR, LVALUE_TO_RVALUE,

   BREAK_STMT, CASE_STMT, CONTINUE_STMT, FOR_STMT, GOTO_STMT, IF_STMT,
   LABEL_STMT, RETURN_STMT, SWITCH_STMT,
   WHILE_STMT, FOR_IN_STMT,

   CLASS_DECL, ENUM_DECL, ENUM_CASE_DECL, CONSTR_DECL, FIELD_DECL, METHOD_DECL,
   OPERATOR_DECL, INTERFACE_DECL,
   STRUCT_DECL, FUNC_ARG_DECL, DESTR_DECL, UNION_DECL, PROP_DECL,
   EXTENSION_DECL,
   FUNCTION_DECL, NAMESPACE_DECL, TYPEDEF_DECL, DECLARATION, DECLARE_STMT,

   RECORD_TEMPLATE_DECL, CALLABLE_TEMPLATE_DECL, METHOD_TEMPLATE_DECL,

   TRY_STMT, THROW_STMT,

   USING_STMT, EOF_STMT, DEBUG_STMT,

   COMPOUND_STMT, STATEMENT, TYPE_REF
};

using namespace cdot;

class AstNode {
public:
   AstNode();
   virtual ~AstNode() {

   }

   typedef std::shared_ptr<AstNode> SharedPtr;

   virtual std::vector<AstNode::SharedPtr> get_children();

   virtual void setIndex(size_t start, size_t end, size_t line, size_t source)
   {
      loc = SourceLocation(start, line, end - start, source);
   }

   void setSourceLoc(const SourceLocation &loc)
   {
      this->loc = loc;
   }

   const SourceLocation& getSourceLoc() const
   {
      return loc;
   }

   virtual size_t getStartIndex() const {
      return loc.getCol();
   }

   virtual size_t getEndIndex() const {
      return loc.getCol() + loc.getLength();
   }

   virtual size_t getLine() const {
      return loc.getLine();
   }

   virtual size_t getSourceId() const
   {
      return loc.getSourceId();
   }

   virtual bool isExpression() const
   {
      return false;
   }

   virtual void bind(std::string id) {
      binding = id;
   }

   virtual void setDeclaration(DeclStmt *decl) {
      declaration = decl;
   }

   virtual void setAttributes(std::vector<Attribute> attr) {
      attributes = attr;
   }

   void replaceUsesWith(Expression *expr);
   virtual void replaceChildWith(AstNode *child, Expression *replacement)
   {
      llvm_unreachable("not implemented for node");
   }

   std::vector<Attribute>& getAttributes() {
      return attributes;
   }

   virtual bool isUnderscore() const
   {
      return false;
   }

   virtual void isReturnValue();
   virtual void isHiddenReturnValue();

   bool hasAttribute(Attr kind) const;

   Attribute& getAttribute(Attr kind) {
      for (auto& attr : attributes) {
         if (attr.kind == kind) {
            return attr;
         }
      }

      assert(false && "Call hasAttribute first");
      llvm_unreachable("see above");
   }

   virtual void setContextualType(const Type& t)
   {
      contextualType = t;
   }

   virtual bool needsContextualInformation() const
   {
      return false;
   }

   virtual NodeType get_type() = 0;

   virtual void accept(AbstractPass* v) = 0;

   template<typename T>
   T *getAs()
   {
      return static_cast<T*>(this);
   }

   ADD_FRIEND_PASSES

protected:
   SourceLocation loc;

   AstNode* parent = nullptr;
   std::vector<std::shared_ptr<Expression>*> children;
   DeclStmt* declaration = nullptr;

   std::vector<Attribute> attributes;

   Type contextualType;

   bool returned_value = false;
   bool sret_value = false;

   // codegen
   string binding;

public:
   const SourceLocation &getLoc() const
   {
      return loc;
   }

   void setLoc(const SourceLocation &loc)
   {
      AstNode::loc = loc;
   }

   AstNode *getParent() const
   {
      return parent;
   }

   void setParent(AstNode *parent)
   {
      AstNode::parent = parent;
   }

   const std::vector<std::shared_ptr<Expression> *> &getChildren() const
   {
      return children;
   }

   void setChildren(const std::vector<std::shared_ptr<Expression> *> &children)
   {
      AstNode::children = children;
   }

   DeclStmt *getDeclaration() const
   {
      return declaration;
   }

   const Type &getContextualType() const
   {
      return contextualType;
   }

   bool isReturnedValue() const
   {
      return returned_value;
   }

   void isReturnedValue(bool return_value)
   {
      AstNode::returned_value = return_value;
   }

   bool isStructRetVal() const
   {
      return sret_value;
   }

   void isStructRetVal(bool sret_value)
   {
      AstNode::sret_value = sret_value;
   }

   const string &getBinding() const
   {
      return binding;
   }

   void setBinding(const string &binding)
   {
      AstNode::binding = binding;
   }
};


#endif //ASTNODE_H
