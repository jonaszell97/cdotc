//
// Created by Jonas Zell on 13.11.17.
//

#ifndef CDOT_TYPEGROUP_H
#define CDOT_TYPEGROUP_H

#include "Type.h"
#include <vector>

class SemaPass;

namespace cdot {

struct Argument;

namespace cl {
class Enum;
}

class TypeGroup: public Type {
public:
   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::TypeGroupID;
   }

protected:
   TypeGroup();
};

class IntegerTypeGroup: public TypeGroup {
public:
   static IntegerTypeGroup *getAll();
   static IntegerTypeGroup *getUnsigned();
   static IntegerTypeGroup *getSigned();

   bool implicitlyCastableTo(Type*) const override;
   string toString() const override;
   Type *getGroupDefault() const override;

protected:
   enum Kind : int {
      All, Signed, Unsigned
   };

   explicit IntegerTypeGroup(Kind kind);
   Kind kind;
};

class FPTypeGroup: public TypeGroup {
public:
   static FPTypeGroup *get();

   bool implicitlyCastableTo(Type*) const override;
   string toString() const override;
   Type *getGroupDefault() const override;

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
