//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_DECLSTATEMENT_H
#define CDOT_DECLSTATEMENT_H


#include "../Statement.h"
#include "../../SymbolTable.h"

namespace cdot {
namespace ast {

class TypeRef;
class Expression;

class VarDecl: public Statement {
public:
   static bool classof(AstNode const* T)
   {
      switch (T->getTypeID()) {
         case LocalVarDeclID:
         case GlobalVarDeclID:
            return true;
         default:
            return false;
      }
   }

protected:
   VarDecl(NodeType id,
           AccessModifier access,
           bool isConst,
           std::vector<string> &&identifiers,
           std::shared_ptr<TypeRef> &&type,
           std::shared_ptr<Expression> &&value)
      : Statement(id), access(access), identifiers(move(identifiers)),
        type(move(type)), value(move(value)), is_const(isConst)
   {}

   AccessModifier access;

   std::vector<string> identifiers;
   std::vector<string> bindings;

   std::shared_ptr<TypeRef> type;
   std::shared_ptr<Expression> value = nullptr;
   bool is_const : 1;

   Callable *destructuringFn = nullptr;
   TupleType *destructuredType = nullptr;

   Namespace *declarationNamespace;

public:
   AccessModifier getAccess() const
   {
      return access;
   }

   void setAccess(AccessModifier access)
   {
      VarDecl::access = access;
   }

   std::vector<string> &getIdentifiers()
   {
      return identifiers;
   }

   std::vector<string> const& getIdentifiers() const
   {
      return identifiers;
   }

   std::shared_ptr<TypeRef> &getType()
   {
      return type;
   }

   std::shared_ptr<Expression> &getValue()
   {
      return value;
   }

   std::shared_ptr<TypeRef> const& getType() const
   {
      return type;
   }

   std::shared_ptr<Expression> const& getValue() const
   {
      return value;
   }

   std::vector<string> &getBindings()
   {
      return bindings;
   }

   bool isConst() const
   {
      return is_const;
   }

   Namespace *getDeclarationNamespace() const
   {
      return declarationNamespace;
   }

   void setDeclarationNamespace(Namespace *declarationNamespace)
   {
      VarDecl::declarationNamespace = declarationNamespace;
   }

   Callable *getDestructuringFn() const
   {
      return destructuringFn;
   }

   void setDestructuringFn(Callable *destructuringFn)
   {
      VarDecl::destructuringFn = destructuringFn;
   }

   TupleType *getDestructuredType() const
   {
      return destructuredType;
   }

   void setDestructuredType(TupleType *destructuredType)
   {
      VarDecl::destructuredType = destructuredType;
   }
};

class LocalVarDecl: public VarDecl {
public:
   LocalVarDecl(std::vector<string> &&identifiers,
                std::shared_ptr<TypeRef> &&type,
                bool isConst,
                std::shared_ptr<Expression> &&val = nullptr);

   ~LocalVarDecl();

   typedef std::shared_ptr<LocalVarDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == LocalVarDeclID;
   }

protected:
   bool declared : 1;
   bool captured : 1;

public:

   bool isDeclared() const
   {
      return declared;
   }

   void setDeclared(bool declared)
   {
      LocalVarDecl::declared = declared;
   }

   bool isCaptured() const
   {
      return captured;
   }

   void setCaptured(bool captured)
   {
      LocalVarDecl::captured = captured;
   }
};

class GlobalVarDecl: public VarDecl {
public:
   GlobalVarDecl(AccessModifier access,
                 std::vector<string> &&identifiers,
                 std::shared_ptr<TypeRef> &&type,
                 bool isConst,
                 std::shared_ptr<Expression> &&val = nullptr);

   typedef std::shared_ptr<GlobalVarDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == GlobalVarDeclID;
   }

protected:
   size_t globalOrdering = 0;

public:
   size_t getGlobalOrdering() const
   {
      return globalOrdering;
   }

   void setGlobalOrdering(size_t globalOrdering)
   {
      GlobalVarDecl::globalOrdering = globalOrdering;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_DECLSTATEMENT_H
