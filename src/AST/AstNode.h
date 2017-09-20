//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include "Attribute/Attribute.h"
#include "Visitor/StaticAnalysis/TypeCheckPass.h"
#include "Visitor/CodeGen/CodeGen.h"
#include "Visitor/StaticAnalysis/ConstExprPass.h"
#include "Visitor/Declaration/DeclPass.h"
#include "../Variant/Type/Type.h"

namespace cdot {
   class GenericType;
}

class DeclStmt;
class Expression;

using cdot::Attribute;

class CompoundStmt;

enum class NodeType {
   COLLECTION_LITERAL, LAMBDA_EXPR, LITERAL_EXPR, NONE_LITERAL, STRING_LITERAL, EXPRESSION,
   ARRAY_ACCESS_EXPR, CALL_EXPR, IDENTIFIER_EXPR, MEMBER_EXPR, METHOD_CALL_EXPR, REF_EXPR,
   BINARY_OPERATOR, UNARY_OPERATOR, TERTIARY_OPERATOR, TUPLE_LITERAL, STRING_INTERPOLATION,

   EXPLICIT_CAST_EXPR, IMPLICIT_CAST_EXPR, LVALUE_TO_RVALUE,

   BREAK_STMT, CASE_STMT, CONTINUE_STMT, FOR_STMT, GOTO_STMT, IF_STMT, LABEL_STMT, RETURN_STMT, SWITCH_STMT,
   WHILE_STMT, FOR_IN_STMT,

   CLASS_DECL, ENUM_DECL, ENUM_CASE_DECL, CONSTR_DECL, FIELD_DECL, METHOD_DECL, OPERATOR_DECL, INTERFACE_DECL,
   STRUCT_DECL, FUNC_ARG_DECL, DESTR_DECL,
   FUNCTION_DECL, NAMESPACE_DECL, TYPEDEF_DECL, DECLARATION, DECLARE_STMT,

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

   virtual void setIndex(int start, int end, size_t source) {
      startIndex = start;
      endIndex = end;
      sourceFileId = source;
   }

   virtual int getStartIndex() const {
      return startIndex;
   }

   virtual int getEndIndex() const {
      return endIndex;
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

   std::vector<Attribute>& getAttributes() {
      return attributes;
   }

   virtual bool isUnderscore() {
      return false;
   }

   virtual void isReturnValue();
   virtual void isHiddenReturnValue();

   bool hasAttribute(Attr kind) {
      for (const auto& attr : attributes) {
         if (attr.kind == kind) {
            return true;
         }
      }

      return false;
   }

   Attribute& getAttribute(Attr kind) {
      for (auto& attr : attributes) {
         if (attr.kind == kind) {
            return attr;
         }
      }

      assert(false && "Call hasAttribute first");
      llvm_unreachable("see above");
   }

   virtual void setContextualType(Type *t) {
      contextualType = t;
   }

   virtual bool needsContextualInformation() {
      return false;
   }

   virtual void heapAllocate();

   virtual pair<string, string> getSourceFile();

   virtual NodeType get_type() = 0;

   virtual llvm::Value* accept(CodeGen& v) = 0;
   virtual Type* accept(TypeCheckPass& v) = 0;
   virtual Variant accept(ConstExprPass& v) = 0;
   virtual void accept(DeclPass& v) = 0;

   virtual void __dump(int depth) = 0;
   virtual void __tab(int depth);

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;

protected:
   int startIndex;
   int endIndex;
   size_t sourceFileId;

   AstNode* parent = nullptr;
   std::vector<std::shared_ptr<Expression>*> children;

   DeclStmt* declaration = nullptr;
   bool isGlobal_ = false;

   std::vector<Attribute> attributes;

   Type* contextualType = nullptr; // unowned

   bool isReturnValue_ = false;
   bool isHiddenReturnValue_ = false;

   // codegen
   string binding;
   bool isHeapAllocated = false;
   bool isGeneric = false;
};


#endif //ASTNODE_H
