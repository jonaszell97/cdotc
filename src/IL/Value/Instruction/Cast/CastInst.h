//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CASTINST_H
#define CDOT_CASTINST_H

#include "../../Constant/ConstantExpr.h"
#include "../Operator/OperatorInst.h"

namespace cdot {

enum class CastKind : unsigned char;

namespace il {

class UnionType;
class LandingPadInst;

static const unsigned short FirstCast = Value::BitCastInstID;
extern const char* CastNames[];

class CastInst: public UnaryInstruction {
public:
   CastInst(TypeID id,
            Value *target,
            QualType toType,
            BasicBlock *parent)
      : UnaryInstruction(id, target, toType, parent)
   {

   }

public:
   static bool classof(CastInst const* T) { return true; }
   static bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_CAST_INST(Name) \
         case Name##ID:
#     include "../../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

class IntegerCastInst: public CastInst {
public:
   IntegerCastInst(CastKind kind,
                   Value *target,
                   QualType toType,
                   BasicBlock *parent);

   CastKind getKind() const { return kind; }

protected:
   CastKind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == IntegerCastInstID;
   }
};

class IntToEnumInst: public CastInst {
public:
   IntToEnumInst(Value *target,
                 QualType toType,
                 BasicBlock *parent)
      : CastInst(IntToEnumInstID, target, toType, parent)
   {}

   static bool classof(Value const* T)
   {
      return T->getTypeID() == IntToEnumInstID;
   }
};

class FPCastInst: public CastInst {
public:
   FPCastInst(CastKind kind,
              Value *target,
              QualType toType,
              BasicBlock *parent);

   CastKind getKind() const { return kind; }

protected:
   CastKind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == FPCastInstID;
   }
};

class UnionCastInst: public CastInst {
public:
   UnionCastInst(Value *target,
                 UnionType *UnionTy,
                 std::string const& fieldName,
                 BasicBlock *parent);

   UnionType *getUnionTy() const { return UnionTy; }
   llvm::StringRef getFieldName() const { return fieldName; }

protected:
   UnionType *UnionTy;
   std::string fieldName;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionCastInstID;
   }
};

class ProtoCastInst: public CastInst {
public:
   ProtoCastInst(Value *target,
                 QualType toType,
                 BasicBlock *parent);

   bool isWrap() const;
   bool isUnwrap() const;

protected:
   enum Flag {
      Wrap = 0x1
   };

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ProtoCastInstID;
   }
};

class ExceptionCastInst: public CastInst {
public:
   ExceptionCastInst(Value *target,
                     QualType toType,
                     BasicBlock *parent);

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ExceptionCastInstID;
   }
};

class BitCastInst: public CastInst {
public:
   BitCastInst(CastKind kind,
               Value *target,
               QualType toType,
               BasicBlock *parent);

   CastKind getKind() const { return kind; }

protected:
   CastKind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == BitCastInstID;
   }
};

class DynamicCastInst: public CastInst {
public:
   DynamicCastInst(Value *target,
                   QualType toType,
                   BasicBlock *parent);

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == DynamicCastInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CASTINST_H
