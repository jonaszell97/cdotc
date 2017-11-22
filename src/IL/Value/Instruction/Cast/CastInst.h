//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CASTINST_H
#define CDOT_CASTINST_H

#include "../../Constant/ConstantExpr.h"
#include "../Operator/OperatorInst.h"

namespace cdot {
namespace il {

class UnionType;
class LandingPadInst;

static const unsigned short FirstCast = Value::BitCastInstID;
extern const char* CastNames[];

class CastInst: public UnaryInstruction {
public:
   CastInst(TypeID id,
            Value *target,
            Type *toType,
            BasicBlock *parent,
            const std::string &name = "",
            const SourceLocation &loc = {})
      : UnaryInstruction(id, target, toType, parent, name, loc)
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

#define CDOT_CAST_INST(name)                                                  \
   class name##Inst: public CastInst {                                        \
      public:                                                                 \
         name##Inst(Value *target,                                            \
                    Type *toType,                                             \
                    BasicBlock *parent,                                       \
                    const std::string &name_ = "",                            \
                    const SourceLocation &loc = {})                           \
         : CastInst(name##InstID, target, toType, parent, name_, loc) {       \
         }                                                                    \
                                                                              \
         static bool classof(Value const* T)                                  \
         {                                                                    \
            return T->getTypeID() == name##InstID;                            \
         }                                                                    \
   };

CDOT_CAST_INST(BitCast)
CDOT_CAST_INST(DynamicCast)

#undef CDOT_CAST_INST

class IntegerCastInst: public CastInst {
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

class FPCastInst: public CastInst {
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

class UnionCastInst: public CastInst {
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

class ProtoCastInst: public CastInst {
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

class ExceptionCastInst: public CastInst {
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

} // namespace il
} // namespace cdot


#endif //CDOT_CASTINST_H
