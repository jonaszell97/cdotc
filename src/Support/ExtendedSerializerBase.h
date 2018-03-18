//
// Created by Jonas Zell on 25.01.18.
//

#ifndef CDOT_EXTENDEDSERIALIZERBASE_H
#define CDOT_EXTENDEDSERIALIZERBASE_H

#include <llvm/ADT/SmallString.h>
#include "SerializerBase.h"

#include "AST/Passes/Serialization/Serialize.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/NamedDecl.h"
#include "AST/ASTContext.h"
#include "Variant/Type/Type.h"

namespace cdot {
namespace support {

template<class Derived>
class ExtendedSerializerBase: public SerializerBase<Derived> {
protected:
   serial::ASTSerializer serializer;

   void serializeStmt(ast::Statement *Stmt)
   {
      llvm::SmallString<256> str;
      llvm::raw_svector_ostream outs(str);

      serializer.serialize(Stmt, outs);
      this->WriteStringWithoutSize(str.str());
   }

   void maybeSerializeStmt(ast::Statement *Stmt)
   {
      if (Stmt) {
         this->WriteBool(true);
         serializeStmt(Stmt);
      }
      else {
         this->WriteBool(false);
      }
   }

   void WriteQualType(QualType Ty)
   {
      WriteType(*Ty);
   }

   void WriteType(Type *Ty)
   {
      this->Writer.WriteByte((char) Ty->getTypeID());
      switch (Ty->getTypeID()) {
         case TypeID::BuiltinTypeID:
            this->Writer.WriteByte(Ty->asBuiltinType()->getKind());
            break;
         case TypeID::PointerTypeID:
            WriteQualType(Ty->getPointeeType());
            break;
         case TypeID::ReferenceTypeID:
            WriteQualType(Ty->asReferenceType()->getReferencedType());
            break;
         case TypeID::MovedTypeID:
            WriteQualType(Ty->asMovedType()->getReferencedType());
            break;
         case TypeID::ArrayTypeID: {
            ArrayType *ArrTy = Ty->asArrayType();
            this->Writer.WriteULEB128(ArrTy->getNumElements());
            WriteQualType(ArrTy->getElementType());

            break;
         }
         case TypeID::InferredArrayTypeID: {
            InferredArrayType *arr = Ty->asInferredArrayType();
            serializeStmt(arr->getDependentExpr());
            WriteQualType(arr->getElementType());

            break;
         }
         case TypeID::TupleTypeID: {
            TupleType *Tup = Ty->asTupleType();
            this->WriteList(Tup->getContainedTypes(),
                            &ExtendedSerializerBase::WriteQualType);

            break;
         }
         case TypeID::FunctionTypeID:
         case TypeID::LambdaTypeID: {
            FunctionType *Fun = Ty->asFunctionType();

            this->WriteBool(Fun->isLambdaType());
            WriteQualType(Fun->getReturnType());
            this->WriteList(Fun->getArgTypes(),
                            &ExtendedSerializerBase::WriteQualType);
            this->Writer.WriteULEB128(Fun->getRawFlags());

            break;

         }
         case TypeID::ObjectTypeID: {
            this->WriteString(Ty->getClassName());
            break;
         }
         case TypeID::InconcreteObjectTypeID: {
            this->WriteString(Ty->getClassName());
            WriteTemplateArgList(Ty->asInconcreteObjectType()
                                   ->getTemplateArgs());

            break;
         }
         case TypeID::GenericTypeID: {
            auto Gen = Ty->asGenericType();
            this->WriteString(Gen->getGenericTypeName());
            WriteQualType(Gen->getActualType());

            break;
         }
         case TypeID::TypedefTypeID: {
            auto td = Ty->asRealTypedefType();
            this->WriteString(td->getAliasName());
            WriteQualType(td->getAliasedType());

            break;
         }
         case TypeID::MetaTypeID:
            WriteQualType(Ty->asMetaType()->getUnderlyingType());
            break;
         case TypeID::NamespaceTypeID:
            this->WriteString(Ty->asNamespaceType()->getNamespaceName());
            break;
      }
   }

   void WriteSourceLoc(SourceLocation loc)
   {
      this->Writer.WriteULEB128(loc.getOffset());
   }

   void WriteTemplateArgList(sema::TemplateArgList const &list)
   {
      this->WriteSize(list);
      for (auto it = list.begin(); it != list.end(); ++it) {
         this->WriteString(it.getParam()->getName());
         WriteResolvedTemplateArg(*it);
      }
   }

   void WriteResolvedTemplateArg(sema::ResolvedTemplateArg const& arg)
   {
      this->WriteBools(arg.isType(), arg.isVariadic());

      if (arg.isVariadic()) {
         this->WriteList(arg.getVariadicArgs(),
                         &ExtendedSerializerBase::WriteResolvedTemplateArg);
      }
      else if (arg.isValue()) {
         WriteVariant(arg.getValue());
      }
      else if (arg.isType()) {
         WriteType(arg.getType());
      }
   }

   void WritePrecedenceGroup(PrecedenceGroup const& PG)
   {
      this->Writer.WriteSLEB128(PG.getPrecedence());
      this->WriteByte(PG.getAssociativity());
   }

   void WriteOperatorInfo(OperatorInfo const& Op)
   {
      WritePrecedenceGroup(Op.getPrecedenceGroup());
      this->WriteByte(Op.getFix());
      this->WriteBool(Op.isImplicit());
   }

   void WriteVariant(Variant const& V)
   {
      this->WriteByte(V.getKind());

      switch (V.getKind()) {
         case VariantType::Int:
            this->WriteBool(V.getAPSInt().isUnsigned());
            WriteAPInt(V.getAPSInt(), V.getAPSInt().isUnsigned());
            break;
         case VariantType::Floating: {
            WriteAPFloat(V.getAPFloat(), false);
            break;
         }
         case VariantType::String: {
            this->WriteString(V.getString());
            break;
         }
         case VariantType::Array:
         case VariantType::Struct: {
            this->WriteList(V.getVec(), &ExtendedSerializerBase::WriteVariant);
            break;
         }
         case VariantType::MetaType: {
            WriteQualType(V.getMetaType());
            break;
         }
         case VariantType::Void:
            break;
         default:
            llvm_unreachable("can't serialize Variant!");
      }
   }

   void WriteAPInt(llvm::APInt const& Int, bool isUnsigned)
   {
      static constexpr unsigned availableBits = sizeof(size_t) * 8;
      if (Int.getBitWidth() <= availableBits) {
         this->WriteBool(true);
         if (isUnsigned)
            this->Writer.WriteULEB128(Int.getZExtValue());
         else
            this->Writer.WriteSLEB128(Int.getSExtValue());
      }
      else {
         this->WriteBool(false);

         llvm::SmallString<128> str;
         Int.toString(str, 10, !isUnsigned);

         this->WriteString(str.str());
      }
   }

   void WriteAPFloat(llvm::APFloat const& Float, bool isFloat)
   {
      static constexpr unsigned availableBits = sizeof(size_t) * 8;
      unsigned neededBits;

      if (isFloat) {
         neededBits = Float.getSizeInBits(llvm::APFloat::IEEEsingle());
      }
      else {
         neededBits = Float.getSizeInBits(llvm::APFloat::IEEEdouble());
      }

      if (neededBits <= availableBits) {
         this->WriteBool(true);

         if (isFloat) {
            union {
               float f;
               size_t s;
            } u;

            u.f = Float.convertToFloat();
            this->Writer.WriteULEB128(u.s);
         }
         else {
            union {
               double d;
               size_t s;
            } u;

            u.d = Float.convertToDouble();
            this->Writer.WriteULEB128(u.s);
         }
      }
      else {
         this->WriteBool(false);

         llvm::SmallString<128> str;
         Float.toString(str);

         this->WriteString(str.str());
      }
   }
};

template<class Derived>
class ExtendedDeserializerBase: public DeserializerBase<Derived> {
protected:
   ast::SemaPass &SP;
   serial::ASTDeserializer deserializer;

   ExtendedDeserializerBase(ast::SemaPass &SP, llvm::MemoryBuffer *Buf)
      : DeserializerBase<Derived>(Buf), SP(SP), deserializer(SP, this->Reader)
   {}

   template<class T>
   T* deserializeStmt()
   {
      return deserializer.ReadSpecific<T>();
   }

   template<class T>
   T* maybeDeserializeStmt()
   {
      if (this->ReadBool())
         return deserializeStmt<T>();

      return nullptr;
   }

   QualType ReadQualType()
   {
      return QualType(ReadType());
   }

   Type *ReadType()
   {
      auto typeID = (TypeID)this->Reader.ReadByte();
      switch (typeID) {
         case TypeID::BuiltinTypeID: {
            auto kind = this->template ReadEnum<BuiltinType::Kind>();
            return SP.getContext().getBuiltinType(kind);
         }
         case TypeID::PointerTypeID:
            return SP.getContext().getPointerType(ReadQualType());
         case TypeID::ReferenceTypeID:
            return SP.getContext().getReferenceType(ReadQualType());
         case TypeID::MovedTypeID:
            return SP.getContext().getMovedType(ReadQualType());
         case TypeID::ArrayTypeID: {
            auto numElements = this->Reader.ReadULEB128();
            auto elementTy = ReadQualType();

            return SP.getContext().getArrayType(elementTy, numElements);
         }
         case TypeID::TupleTypeID: {
            return SP.getContext().getTupleType(
               this->template ReadList<QualType>
                  (&ExtendedDeserializerBase::ReadQualType));
         }
         case TypeID::FunctionTypeID:
         case TypeID::LambdaTypeID: {
            bool lambda = this->ReadBool();
            auto ret = ReadQualType();
            auto args = this->template ReadList<QualType>(
               &ExtendedDeserializerBase::ReadQualType);
            unsigned flags = unsigned(this->Reader.ReadULEB128());

            return SP.getContext().getFunctionType(ret, move(args), flags,
                                                   lambda);
         }
         case TypeID::ObjectTypeID: {
            return SP.getObjectTy(this->ReadString());
         }
         case TypeID::GenericTypeID: {
            auto name = this->ReadString();
            auto cov = ReadQualType();

            return SP.getContext().getTemplateArgType(cov, name);
         }
         case TypeID::MetaTypeID:
            return SP.getContext().getMetaType(ReadQualType());
         case TypeID::NamespaceTypeID:
            return SP.getContext().getNamespaceType(
               SP.getNamespace(this->ReadString()));
         default:
            llvm_unreachable("bad type!");
      }
   }

   sema::TemplateArgList ReadTemplateArgList()
   {
      auto numParams = this->ReadSize();
      sema::TemplateArgList list(SP);

      for (size_t i = 0; i < numParams; ++i) {
         auto name = this->ReadString();
         auto arg = ReadResolvedTemplateArg();

         list.insert(name, move(arg));
      }

      return list;
   }

   sema::ResolvedTemplateArg ReadResolvedTemplateArg()
   {
      bool isTypeName, isVariadic;
      this->ReadBools(isTypeName, isVariadic);

      if (isVariadic) {
         auto variadicArgs = this->template ReadList<sema::ResolvedTemplateArg>(
            &ExtendedDeserializerBase::ReadResolvedTemplateArg);

         return sema::ResolvedTemplateArg(isTypeName, move(variadicArgs));
      }
      else if (!isTypeName) {
         return sema::ResolvedTemplateArg(ReadVariant());
      }
      else {
         return sema::ResolvedTemplateArg(ReadType());
      }
   }

   SourceLocation ReadSourceLoc()
   {
      auto offset = unsigned(this->Reader.ReadULEB128());
      return SourceLocation(offset);
   }

   PrecedenceGroup ReadPrecedenceGroup()
   {
      long prec = long(this->Reader.ReadSLEB128());
      Associativity assoc = (Associativity)this->Reader.ReadByte();

      return PrecedenceGroup(prec, assoc);
   }

   OperatorInfo ReadOperatorInfo()
   {
      auto PG = ReadPrecedenceGroup();
      auto fix = (FixKind)this->Reader.ReadByte();
      auto implicit = this->ReadBool();

      return OperatorInfo(PG, fix, implicit);
   }

   Variant ReadVariant()
   {
      auto kind = this->template ReadEnum<VariantType>();
      switch (kind) {
         case VariantType::Int:
            return Variant(readAPInt(this->ReadBool()));
         case VariantType::Floating: {
            return readAPFloat(false);
         }
         case VariantType::String:
            return Variant(this->ReadString());
         case VariantType::Array:
         case VariantType::Struct:
            return Variant(kind, this->template ReadList<Variant>
                              (&ExtendedDeserializerBase::ReadVariant));
         case VariantType::MetaType:
            return Variant(ReadQualType());
         case VariantType::Void:
            return Variant();
         default:
            llvm_unreachable("can't deserialize Variant!");
      }
   }

   llvm::APInt readAPInt(bool isUnsigned)
   {
      static constexpr unsigned bits = sizeof(size_t) * 8;
      if (this->ReadBool()) {
         return llvm::APInt(bits, this->Reader.ReadULEB128(), !isUnsigned);
      }
      else {
         return llvm::APSInt(this->ReadString());
      }
   }

   llvm::APFloat readAPFloat(bool isSingle)
   {
      if (this->ReadBool()) {
         if (isSingle) {
            union {
               size_t s;
               float f;
            } u;

            u.s = this->Reader.ReadULEB128();
            return llvm::APFloat(u.f);
         }
         else {
            union {
               size_t s;
               double d;
            } u;

            u.s = this->Reader.ReadULEB128();
            return llvm::APFloat(u.d);
         }
      }
      else {
         llvm::APFloat APF(0.0);
         APF.convertFromString(this->ReadString(),
                               llvm::APFloat::rmNearestTiesToEven);

         return APF;
      }
   }
};

} // namespace support
} // namespace cdot


#endif //CDOT_EXTENDEDSERIALIZERBASE_H
