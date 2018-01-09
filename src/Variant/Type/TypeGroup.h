//
// Created by Jonas Zell on 13.11.17.
//

#ifndef CDOT_TYPEGROUP_H
#define CDOT_TYPEGROUP_H

#include "Type.h"
#include "QualType.h"
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
      switch (T->getTypeID()) {
#     define CDOT_TYPE_GROUP(Name) \
         case TypeID::Name##ID: return true;
#     include "Types.def"

         default:
            return false;
      }
   }

   Type *getGroupDefault() const;

protected:
   explicit TypeGroup(TypeID id)
   {
      this->id = id;
   }
};

class IntegerTypeGroup: public TypeGroup {
public:
   static IntegerTypeGroup *getAll();
   static IntegerTypeGroup *getUnsigned();
   static IntegerTypeGroup *getSigned();

   std::string toString() const;
   Type *getGroupDefault() const;

   static bool classof(Type const *T)
   {
      return T->getTypeID() == TypeID::IntegerTypeGroupID;
   }

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

   std::string toString() const;
   Type *getGroupDefault() const;

   static bool classof(Type const *T)
   {
      return T->getTypeID() == TypeID::FPTypeGroupID;
   }

protected:
   FPTypeGroup();
   static FPTypeGroup *Instance;
};

class StringTypeGroup: public TypeGroup {
public:
   static StringTypeGroup *get();

   std::string toString() const;
   Type *getGroupDefault() const;

   static bool classof(Type const *T)
   {
      return T->getTypeID() == TypeID::StringTypeGroupID;
   }

protected:
   StringTypeGroup();
   static StringTypeGroup *Instance;
};

class EnumTypeGroup: public TypeGroup {
public:
   static EnumTypeGroup *get(llvm::StringRef caseName);

   llvm::StringRef getCaseName() const { return caseName; }

   static bool classof(Type const *T)
   {
      return T->getTypeID() == TypeID::EnumTypeGroupID;
   }

protected:
   explicit EnumTypeGroup(llvm::StringRef caseName)
      : TypeGroup(TypeID::EnumTypeGroupID), caseName(caseName)
   {}

   llvm::StringRef caseName;
};

//class LambdaTypeGroup: public TypeGroup {
//public:
//   static LambdaTypeGroup *get(QualType returnType,
//                               std::vector<QualType> &&argTypes);
//
//   QualType getReturnType() const
//   {
//      return returnType;
//   }
//
//   const std::vector<QualType> &getArgTypes() const
//   {
//      return argTypes;
//   }
//
//   static bool classof(Type const *T)
//   {
//      return T->getTypeID() == TypeID::LambdaTypeGroupID;
//   }
//
//private:
//   LambdaTypeGroup(QualType returnType,
//                   std::vector<QualType> &&argTypes)
//      : TypeGroup(TypeID::LambdaTypeGroupID), returnType(returnType),
//        argTypes(std::move(argTypes))
//   { }
//
//   QualType returnType;
//   std::vector<QualType> argTypes;
//};

class LambdaTypeGroup: public TypeGroup {
public:
   static LambdaTypeGroup *get(size_t numArgs);

   size_t getNumArgs() const
   {
      return numArgs;
   }

   static bool classof(Type const *T)
   {
      return T->getTypeID() == TypeID::LambdaTypeGroupID;
   }

private:
   explicit LambdaTypeGroup(size_t numArgs)
      : TypeGroup(TypeID::LambdaTypeGroupID), numArgs(numArgs)
   {}

   size_t numArgs;
};

} // namespace cdot

#endif //CDOT_TYPEGROUP_H
