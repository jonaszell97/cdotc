//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_DECLSTATEMENT_H
#define CDOT_DECLSTATEMENT_H


#include "NamedDecl.h"
#include "../../../Util.h"

namespace cdot {
namespace ast {

class TypeRef;
class Expression;
class NamespaceDecl;
class CallableDecl;

class VarDecl: public NamedDecl {
public:
   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      switch (kind) {
         case LocalVarDeclID:
         case GlobalVarDeclID:
         case FuncArgDeclID:
            return true;
         default:
            return false;
      }
   }

protected:
   VarDecl(NodeType id,
           AccessModifier access,
           bool isConst,
           std::string &&ident,
           TypeRef* type,
           Expression* value)
      : NamedDecl(id, access, move(ident)), type(type),
        value(value), is_const(isConst)
   {}

   TypeRef* type;
   Expression* value = nullptr;
   bool is_const : 1;

   friend class TransformImpl;

public:
   TypeRef* getTypeRef() const
   {
      return type;
   }

   void setType(TypeRef *type)
   {
      VarDecl::type = type;
   }

   Expression* getValue() const
   {
      return value;
   }

   bool isConst() const
   {
      return is_const;
   }
};

class LocalVarDecl: public VarDecl {
public:
   LocalVarDecl(AccessModifier access,
                bool isConst,
                std::string &&ident,
                TypeRef* type,
                Expression* value)
      : VarDecl(LocalVarDeclID, access, isConst, move(ident), type, value),
        captured(false)
   {}

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == LocalVarDeclID;
   }

   friend class TransformImpl;

protected:
   bool captured : 1;

public:
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
                 bool isConst,
                 std::string &&ident,
                 TypeRef* type,
                 Expression* value)
      : VarDecl(GlobalVarDeclID, access, isConst, move(ident), type, value)
   {}

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == GlobalVarDeclID;
   }

   friend class TransformImpl;

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

class FuncArgDecl: public VarDecl {
public:
   FuncArgDecl(std::string &&argName,
               TypeRef* argType,
               Expression* defaultValue,
               bool variadicArgPackExpansion,
               bool isConst, bool cstyleVararg = false)
      : VarDecl(FuncArgDeclID, AccessModifier::PUBLIC, isConst, move(argName),
                argType, defaultValue),
        variadicArgPackExpansion(variadicArgPackExpansion),
        is_const(isConst), vararg(cstyleVararg), cstyleVararg(cstyleVararg)
   {}

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == FuncArgDeclID;
   }

   friend class TransformImpl;

protected:
   bool variadicArgPackExpansion : 1;
   bool is_const : 1;
   bool vararg : 1;
   bool cstyleVararg : 1;

public:
   const std::string &getArgName() const
   {
      return name;
   }

   TypeRef* const& getArgType() const
   {
      return type;
   }

   Expression* getDefaultVal() const
   {
      return value;
   }

   void setDefaultVal(Expression *defaultVal)
   {
      value = defaultVal;
   }

   bool isVariadicArgPackExpansion() const
   {
      return variadicArgPackExpansion;
   }

   bool isConst() const
   {
      return is_const;
   }

   bool isVararg() const
   {
      return vararg;
   }

   bool isCstyleVararg() const
   {
      return cstyleVararg;
   }
};

class DestructuringDecl: public Statement {
public:
   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      switch (kind) {
         case LocalDestructuringDeclID:
         case GlobalDestructuringDeclID:
            return true;
         default:
            return false;
      }
   }

   friend class TransformImpl;

protected:
   DestructuringDecl(NodeType typeID,
                     AccessModifier access,
                     bool isConst,
                     VarDecl **decls,
                     size_t numDecls,
                     TypeRef *type,
                     Expression *value)
      : Statement(typeID),
        access(access), is_const(isConst),
        decls(decls), numDecls(numDecls), type(type), value(value)
   {}

   AccessModifier access;
   bool is_const;

   VarDecl **decls;
   size_t numDecls;

   TypeRef *type;
   Expression *value;

   CallableDecl *destructuringFn = nullptr;

public:
   using decl_range = llvm::iterator_range<VarDecl**>;

   decl_range getDecls() const
   {
      return decl_range(decls, decls + numDecls + 1);
   }

   size_t getNumDecls() const
   {
      return numDecls;
   }

   VarDecl *getFirstDecl() const
   {
      return decls[0];
   }

   AccessModifier getAccess() const
   {
      return access;
   }

   void setAccess(AccessModifier access)
   {
      DestructuringDecl::access = access;
   }

   bool isConst() const
   {
      return is_const;
   }

   TypeRef *getType() const
   {
      return type;
   }

   void setType(TypeRef *type)
   {
      DestructuringDecl::type = type;
   }

   Expression *getValue() const
   {
      return value;
   }

   CallableDecl *getDestructuringFn() const
   {
      return destructuringFn;
   }

   void setDestructuringFn(CallableDecl *destructuringFn)
   {
      DestructuringDecl::destructuringFn = destructuringFn;
   }
};

class LocalDestructuringDecl: public DestructuringDecl {
public:
   LocalDestructuringDecl(AccessModifier access,
                          bool isConst,
                          VarDecl **decls,
                          size_t numDecls,
                          TypeRef *type,
                          Expression *value)
      : DestructuringDecl(LocalDestructuringDeclID, access, isConst,
                          decls, numDecls, type, value)
   {}

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == LocalDestructuringDeclID;
   }

   friend class TransformImpl;
};

class GlobalDestructuringDecl: public DestructuringDecl {
public:
   GlobalDestructuringDecl(AccessModifier access,
                           bool isConst,
                           VarDecl **decls,
                           size_t numDecls,
                           TypeRef *type,
                           Expression *value)
      : DestructuringDecl(GlobalDestructuringDeclID, access, isConst,
                          decls, numDecls, type, value)
   {}

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == GlobalDestructuringDeclID;
   }

   friend class TransformImpl;

private:
   size_t globalOrdering = 0;

public:
   size_t getGlobalOrdering() const
   {
      return globalOrdering;
   }

   void setGlobalOrdering(size_t globalOrdering)
   {
      GlobalDestructuringDecl::globalOrdering = globalOrdering;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_DECLSTATEMENT_H
