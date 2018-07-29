//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_GLOBALVARIABLE_H
#define CDOT_GLOBALVARIABLE_H

#include "IL/Constant.h"
#include "SymbolTableList.h"

#include <llvm/ADT/ilist_node.h>

namespace cdot {
namespace serial {
   class LazyILGlobalInfo;
} // namespace serial

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
   SourceLocation SourceLoc;
   Module *parent;

   unsigned Linkage : 4;       // The linkage of this global
   unsigned Visibility : 2;    // The visibility style of this global
   unsigned UnnamedAddrVal : 2; // This value's address is not significant

public:
   static bool classof(Value const *T)
   {
      auto kind = T->getTypeID();
      return kind > _firstGlobalValue && kind < _lastGlobalValue;
   }

   LinkageTypes getLinkage() const { return (LinkageTypes)Linkage; }
   void setLinkage(LinkageTypes L) { Linkage = L; }

   bool isExternallyVisible() const;

   VisibilityTypes getVisibility() const { return (VisibilityTypes)Visibility; }
   void setVisibility(VisibilityTypes V) { Visibility = V; }

   UnnamedAddr getUnnamedAddr() const { return UnnamedAddr(UnnamedAddrVal); }
   void setUnnamedAddr(UnnamedAddr Val) { UnnamedAddrVal = unsigned(Val); }

   SourceLocation getSourceLoc() const { return SourceLoc; }
   void setSourceLoc(SourceLocation Loc) { SourceLoc = Loc; }

   bool wasDeserialized() const { return InstBits.Deserialized; }
   void setDeserialized(bool b) { InstBits.Deserialized = b; }

   bool isVerified() const { return InstBits.Verified; }
   void setVerified(bool V) { InstBits.Verified = V; }

   bool isCanonicalized() const { return InstBits.Canonicalized; }
   void setCanonicalized(bool V) { InstBits.Canonicalized = V; }

   bool isOptimized() const { return InstBits.Optimized; }
   void setOptimized(bool V) { InstBits.Optimized = V; }
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

   Function *getInitFn() const { return InitFn; }
   void setInitFn(Function *InitFn) { GlobalVariable::InitFn = InitFn; }

   Function *getDeinitFn() const { return DeinitFn; }
   void setDeinitFn(Function *V) { DeinitFn = V; }

   GlobalVariable *getInitializedFlag() const { return InitializedFlag; }
   void setInitializedFlag(GlobalVariable *F) { InitializedFlag = F; }

   bool needsDeinit() const { return GVBits.NeedsDeinit; }
   void setNeedsDeinit(bool B) { GVBits.NeedsDeinit = B; }

   serial::LazyILGlobalInfo* getLazyGlobalInfo() const{ return LazyGlobalInfo; }
   void setLazyGlobalInfo(serial::LazyILGlobalInfo* V) { LazyGlobalInfo = V; }

   void makeMutable();

   bool overridePreviousDefinition() const { return GVBits.OverridePrevious; }
   void setOverridePreviousDefinition(bool b) { GVBits.OverridePrevious = b; }

   op_iterator op_begin_impl() { return &initializer; }
   op_const_iterator op_begin_impl() const { return &initializer; }
   unsigned getNumOperandsImpl() const { return initializer ? 1 : 0; }

protected:
   Constant *initializer;
   GlobalVariable *InitializedFlag = nullptr;
   serial::LazyILGlobalInfo *LazyGlobalInfo = nullptr;

   Function *InitFn = nullptr;
   Function *DeinitFn = nullptr;

public:
   bool isDeclared() const { return GVBits.Declared; }
   void setDeclared(bool b) { GVBits.Declared = b; }

   bool isConstant() const { return GVBits.Const; }
   bool isLazilyInitialized() const { return GVBits.LazilyInitialized; }
   void setIsLazilyInitialized() { GVBits.LazilyInitialized = true; }

private:
   GlobalVariable(const GlobalVariable &var, Module &M);

public:
   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == GlobalVariableID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_GLOBALVARIABLE_H
