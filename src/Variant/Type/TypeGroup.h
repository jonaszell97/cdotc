//
// Created by Jonas Zell on 13.11.17.
//

#ifndef CDOT_TYPEGROUP_H
#define CDOT_TYPEGROUP_H

#include "BuiltinType.h"
#include <vector>

class SemaPass;

namespace cdot {

struct Argument;

namespace cl {
class Enum;
}

class TypeGroup: public BuiltinType {
public:
   bool isTypeGroup() const override
   {
      return true;
   }

   static inline bool classof(TypeGroup const*) { return true; }
   static inline bool classof(BuiltinType const* T) {
      switch(T->getTypeID()) {
         case TypeID::TypeGroupID:
            return true;
         default:
            return false;
      }
   }

protected:
   TypeGroup();
};

class IntegerTypeGroup: public TypeGroup {
public:
   static IntegerTypeGroup *getAll();
   static IntegerTypeGroup *getUnsigned();
   static IntegerTypeGroup *getSigned();

   bool implicitlyCastableTo(BuiltinType*) const override;
   string toString() const override;
   BuiltinType *getGroupDefault() const override;

protected:
   enum Kind : int {
      All, Signed, Unsigned
   };

   explicit IntegerTypeGroup(Kind kind);

   static llvm::SmallDenseMap<int, IntegerTypeGroup*> Instances;

   Kind kind;
};

class FPTypeGroup: public TypeGroup {
public:
   static FPTypeGroup *get();

   bool implicitlyCastableTo(BuiltinType*) const override;
   string toString() const override;
   BuiltinType *getGroupDefault() const override;

protected:
   FPTypeGroup();
   static FPTypeGroup *Instance;
};

//class EnumCaseTypeGroup: public TypeGroup {
//public:
//   typedef std::function<bool(cl::Enum*, const string&,
//      const std::vector<Argument>&)> DeciderFunction;
//
//   static EnumCaseTypeGroup *get(const string &caseName,
//                                 const DeciderFunction &func,
//                                 const std::vector<Argument> &argTypes = {});
//
//   bool implicitlyCastableTo(BuiltinType*) const override;
//   string toString() const override;
//   BuiltinType *getGroupDefault() const override;
//
//protected:
//   EnumCaseTypeGroup(const string &caseName,
//                     const std::vector<Argument> &argTypes = {});
//
//   string caseName;
//   const DeciderFunction &func;
//   std::vector<Argument> argTypes;
//
//   static llvm::StringMap<EnumCaseTypeGroup*> Instances;
//};

} // namespace cdot

#endif //CDOT_TYPEGROUP_H
