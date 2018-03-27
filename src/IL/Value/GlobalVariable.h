//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_GLOBALVARIABLE_H
#define CDOT_GLOBALVARIABLE_H

#include <string>
#include <llvm/ADT/ilist_node.h>
#include "Constant/Constant.h"
#include "SymbolTableList.h"

namespace cdot {
namespace il {

class Module;

extern const char *LinkageTypeNames[];
extern const char *VisibilityTypeNames[];
extern const char *UnnamedAddrNames[];

class GlobalObject: public Constant {
public:
   GlobalObject(TypeID id,
                QualType ty,
                Module *module,
                llvm::StringRef name);

   Module *getParent() const { return parent; }
   void setParent(Module *m) { parent = m; }

   enum LinkageTypes {
      ExternalLinkage = 0,///< Externally visible function
      AvailableExternallyLinkage, ///< Available for inspection, not emission.
      LinkOnceAnyLinkage, ///< Keep one copy of function when linking (inline)
      LinkOnceODRLinkage, ///< Same, but only replaced by something equivalent.
      WeakAnyLinkage,     ///< Keep one copy of named function when linking (weak)
      WeakODRLinkage,     ///< Same, but only replaced by something equivalent.
      AppendingLinkage,   ///< Special purpose, only applies to global arrays
      InternalLinkage,    ///< Rename collisions when linking (static functions).
      PrivateLinkage,     ///< Like Internal, but omit from symbol table.
      ExternalWeakLinkage,///< ExternalWeak linkage description.
      CommonLinkage       ///< Tentative definitions.
   };

   enum VisibilityTypes {
      DefaultVisibility = 0,  ///< The GV is visible
      HiddenVisibility,       ///< The GV is hidden
      ProtectedVisibility     ///< The GV is protected
   };

   enum class UnnamedAddr {
      None,
      Local,
      Global,
   };

protected:
   Module *parent;

   unsigned Linkage : 4;       // The linkage of this global
   unsigned Visibility : 2;    // The visibility style of this global
   unsigned UnnamedAddrVal : 2; // This value's address is not significant

public:
   static bool classof(Value const *T)
   {
      switch (T->getTypeID()) {
         case FunctionID:
         case GlobalVariableID:
         case MethodID:
         case InitializerID:
         case StructTypeID:
         case ClassTypeID:
         case EnumTypeID:
         case UnionTypeID:
         case ProtocolTypeID:
            return true;
         default:
            return false;
      }
   }

   LinkageTypes getLinkage() const { return (LinkageTypes)Linkage; }
   void setLinkage(LinkageTypes L) { Linkage = L; }

   VisibilityTypes getVisibility() const { return (VisibilityTypes)Visibility; }
   void setVisibility(VisibilityTypes V) { Visibility = V; }

   UnnamedAddr getUnnamedAddr() const { return UnnamedAddr(UnnamedAddrVal); }
   void setUnnamedAddr(UnnamedAddr Val) { UnnamedAddrVal = unsigned(Val); }
};

class GlobalVariable: public GlobalObject,
                      public llvm::ilist_node_with_parent<GlobalVariable,
                         Module> {
public:
   GlobalVariable(QualType ty,
                  bool isConstant,
                  llvm::StringRef name,
                  Module *module,
                  Constant *initializer = nullptr);

   Constant *getInitializer() const { return initializer; }
   bool hasInitializer() const { return initializer != nullptr; }

   GlobalVariable *getDeclarationIn(Module *M);

   void setInitializer(Constant *initializer);

   BasicBlock *getInitBB() const
   {
      return InitBB;
   }

   void setInitBB(BasicBlock *InitBB)
   {
      GlobalVariable::InitBB = InitBB;
   }

   bool isDeclared() const
   {
      return (SubclassData & Flags::Declared) != 0;
   }

protected:
   Constant *initializer;

   BasicBlock *InitBB = nullptr;

   enum Flags {
      Const = 1,
      LateInitialized = Const << 1,
      Declared = LateInitialized << 2,
   };

public:
   bool isConstant() const { return (SubclassData & Flags::Const) != 0; }
   bool isLateInitialized() const
   {
      return (SubclassData & Flags::LateInitialized) != 0;
   }

   void setIsLateInitialized()
   {
      SubclassData |= Flags::LateInitialized;
   }

private:
   GlobalVariable(const GlobalVariable &var);

public:
   static bool classof(GlobalVariable const* T) { return true; }
   static inline bool classof(Value const* T) {
     switch(T->getTypeID()) {
        case GlobalVariableID:
        case FunctionID:
        case MethodID:
           return true;
        default:
           return false;
     }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_GLOBALVARIABLE_H
