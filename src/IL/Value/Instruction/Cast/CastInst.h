//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CASTINST_H
#define CDOT_CASTINST_H

#include "../../Constant/ConstantExpr.h"

namespace cdot {
namespace il {

class UnionType;
class LandingPadInst;

static const unsigned short FirstCast = Value::BitCastInstID;
extern const char* CastNames[];

class CastInst: public Instruction {
public:
   CastInst(TypeID id,
            Value *target,
            Type *toType,
            BasicBlock *parent,
            const std::string &name = "",
            const SourceLocation &loc = {})
      : Instruction(id, toType, parent, name, loc), target(target)
   {

   }

   Value *getTarget() const
   {
      return target;
   }

protected:
   Value *target;

public:
   static bool classof(CastInst const* T) { return true; }
   static bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_INSTRUCTION(Name) \
         case Name##ID:
#     define CDOT_INCLUDE_CAST_INSTS
#     include "../../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

template<Value::TypeID typeID, bool isConst = false>
class CastBase: public CastInst {
public:
   CastBase(Value *target,
            Type *toType,
            BasicBlock *parent,
            const std::string &name = "",
            const SourceLocation &loc = {})
      : CastInst(typeID, target, toType, parent, name, loc)
   {

   }
};

/// Synopsis
//class BitCastInst: public CastBase<ConstantExpr, Value::BitCastInstID, true> {
//public:
//   BitCastInst(Value *target,
//               Type *toType,
//               BasicBlock *parent,
//               const SourceLocation &loc = {},
//               const std::string &name = "")
//      : CastInst(target, toType, parent, loc, name) {}
//};

#define CDOT_CAST_INST(name, IsConst)                                         \
   class name##Inst: public CastBase<Value::name##InstID, IsConst> {          \
      public:                                                                 \
         name##Inst(Value *target,                                            \
                    Type *toType,                                             \
                    BasicBlock *parent,                                       \
                    const std::string &name_ = "",                            \
                    const SourceLocation &loc = {})                           \
         : CastBase(target, toType, parent, name_, loc) {                     \
            target->addUse();                                                 \
         }                                                                    \
                                                                              \
         static bool classof(Value const* T)                                  \
         {                                                                    \
            return T->getTypeID() == name##InstID;                            \
         }                                                                    \
   };

CDOT_CAST_INST(BitCast, true)
CDOT_CAST_INST(DynamicCast, false)

class IntegerCastInst: public CastBase<Value::IntegerCastInstID, false> {
public:
   IntegerCastInst(Value *target,
                   Type *toType,
                   BasicBlock *parent,
                   const std::string &name = "",
                   const SourceLocation &loc = {});

   enum Kind : unsigned char {
      Ext = 0, Trunc, IntToPtr, IntToFP, PtrToInt, SignFlip, FPToInt, Box,
      Unbox, NoOp
   };

   Kind getKind() const;

protected:
   Kind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == IntegerCastInstID;
   }
};

extern const char* IntCastNames[];

class FPCastInst: public CastBase<Value::FPCastInstID, false> {
public:
   FPCastInst(Value *target,
              Type *toType,
              BasicBlock *parent,
              const std::string &name = "",
              const SourceLocation &loc = {});

   enum Kind : unsigned char {
      FPExt = 0, FPTrunc, Box, Unbox, NoOp
   };

   Kind getKind() const;

protected:
   Kind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == FPCastInstID;
   }
};

extern const char* FPCastNames[];

class UnionCastInst: public CastBase<Value::UnionCastInstID, false> {
public:
   UnionCastInst(Value *target,
                 UnionType *UnionTy,
                 llvm::StringRef fieldName,
                 BasicBlock *parent,
                 const std::string &name = "",
                 const SourceLocation &loc = {});

   UnionType *getUnionTy() const
   {
      return UnionTy;
   }

   const llvm::StringRef &getFieldName() const
   {
      return fieldName;
   }

protected:
   UnionType *UnionTy;
   llvm::StringRef fieldName;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionCastInstID;
   }
};

class ProtoCastInst: public CastBase<Value::ProtoCastInstID, false> {
public:
   ProtoCastInst(Value *target,
                 Type *toType,
                 BasicBlock *parent,
                 const std::string &name = "",
                 const SourceLocation &loc = {});

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

class ExceptionCastInst: public CastBase<Value::ExceptionCastInstID, false> {
public:
   ExceptionCastInst(Value *target,
                     Type *toType,
                     BasicBlock *parent,
                     const std::string &name = "",
                     const SourceLocation &loc = {});

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ExceptionCastInstID;
   }
};

#undef CDOT_CAST_INST

} // namespace il
} // namespace cdot


#endif //CDOT_CASTINST_H
