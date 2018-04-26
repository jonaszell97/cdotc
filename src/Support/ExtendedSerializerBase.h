//
// Created by Jonas Zell on 25.01.18.
//

#ifndef CDOT_EXTENDEDSERIALIZERBASE_H
#define CDOT_EXTENDEDSERIALIZERBASE_H

#include "SerializerBase.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/Type.h"
#include "Sema/SemaPass.h"
#include "Serialization/ASTSerialize.h"

#include <llvm/ADT/SmallString.h>

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
      case Type::BuiltinTypeID:
         this->Writer.WriteByte(Ty->asBuiltinType()->getKind());
         break;
      case Type::PointerTypeID:
      case Type::MutablePointerTypeID:
         WriteQualType(Ty->getPointeeType());
         break;
      case Type::ReferenceTypeID:
         WriteQualType(Ty->asReferenceType()->getReferencedType());
         break;
      case Type::ArrayTypeID: {
         ArrayType *ArrTy = Ty->asArrayType();
         this->Writer.WriteULEB128(ArrTy->getNumElements());
         WriteQualType(ArrTy->getElementType());

         break;
      }
      case Type::DependentSizeArrayTypeID: {
         DependentSizeArrayType *arr = Ty->asDependentSizeArrayType();
         serializeStmt(arr->getSizeExpr());
         WriteQualType(arr->getElementType());

         break;
      }
      case Type::InferredSizeArrayTypeID: {
         WriteQualType(Ty->uncheckedAsArrayType()->getElementType());
         break;
      }
      case Type::TupleTypeID: {
         TupleType *Tup = Ty->asTupleType();
         this->WriteList(Tup->getContainedTypes(),
                         &ExtendedSerializerBase::WriteQualType);

         break;
      }
      case Type::FunctionTypeID:
      case Type::LambdaTypeID: {
         FunctionType *Fun = Ty->asFunctionType();

         this->WriteBool(Fun->isLambdaType());
         WriteQualType(Fun->getReturnType());
         this->WriteList(Fun->getParamTypes(),
                         &ExtendedSerializerBase::WriteQualType);
         this->Writer.WriteULEB128(Fun->getRawFlags());

         break;

      }
      case Type::RecordTypeID: {
         llvm_unreachable("TODO!");
//         this->WriteString(Ty->getClassName());
//         break;
      }
      case Type::DependentRecordTypeID: {
         llvm_unreachable("TODO!");
//         this->WriteString(Ty->getClassName());
//         WriteTemplateArgList(Ty->asDependentRecordType()
//                                ->getTemplateArgs());
//
//         break;
      }
      case Type::GenericTypeID: {
         auto Gen = cast<GenericType>(Ty);
         this->WriteString(Gen->getGenericTypeName());
         break;
      }
      case Type::TypedefTypeID: {
         auto td = Ty->asRealTypedefType();
         this->WriteString(td->getAliasName());
         WriteQualType(td->getAliasedType());

         break;
      }
      case Type::MetaTypeID:
         WriteQualType(Ty->asMetaType()->getUnderlyingType());
         break;
      default:
         llvm_unreachable("unimplemented!");
      }
   }

   void WriteSourceLoc(SourceLocation loc)
   {
      this->Writer.WriteULEB128(loc.getOffset());
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
      auto typeID = (Type::TypeID)this->Reader.ReadByte();
      switch (typeID) {
      case Type::BuiltinTypeID: {
         auto kind = this->template ReadEnum<Type::BuiltinKind>();
         return SP.getContext().getBuiltinType(kind);
      }
      case Type::PointerTypeID:
         return SP.getContext().getPointerType(ReadQualType());
      case Type::ReferenceTypeID:
         return SP.getContext().getReferenceType(ReadQualType());
      case Type::ArrayTypeID: {
         auto numElements = this->Reader.ReadULEB128();
         auto elementTy = ReadQualType();

         return SP.getContext().getArrayType(elementTy, numElements);
      }
      case Type::DependentSizeArrayTypeID: {
         auto expr = deserializeStmt<ast::StaticExpr>();
         auto elTy = ReadQualType();

         return SP.getContext().getValueDependentSizedArrayType(elTy, expr);
      }
      case Type::InferredSizeArrayTypeID: {
         auto elTy = ReadQualType();
         return SP.getContext().getInferredSizeArrayType(elTy);
      }
      case Type::TupleTypeID: {
         return SP.getContext().getTupleType(
            this->template ReadList<QualType>
               (&ExtendedDeserializerBase::ReadQualType));
      }
      case Type::FunctionTypeID:
      case Type::LambdaTypeID: {
         bool lambda = this->ReadBool();
         auto ret = ReadQualType();
         auto args = this->template ReadList<QualType>(
            &ExtendedDeserializerBase::ReadQualType);
         unsigned flags = unsigned(this->Reader.ReadULEB128());

         return SP.getContext().getFunctionType(ret, move(args), flags,
                                                lambda);
      }
      case Type::RecordTypeID: {
//         return SP.getObjectTy(this->ReadString());
      }
      case Type::DependentRecordTypeID: {
//         auto R = SP.getRecord(this->ReadString());
//         auto TAs = ReadTemplateArgList();
//
//         return SP.getContext().getDependentRecordType(R, move(TAs));
         llvm_unreachable("unimplemented");
      }
      case Type::GenericTypeID: {
         auto name = this->ReadString();
         return SP.getContext().getTemplateArgType(nullptr); // FIXME
      }
      case Type::MetaTypeID:
         return SP.getContext().getMetaType(ReadQualType());
//      case Type::NamespaceTypeID:
//         return SP.getContext().getNamespaceType(
//            SP.getNamespace(this->ReadString()));
//      case Type::TypedefTypeID:
//         return SP.getContext().getTypedefType(
//            SP.getTypedef(this->ReadString()));
      default:
         llvm_unreachable("unimplemented type");
      }
   }

   sema::ResolvedTemplateArg ReadResolvedTemplateArg()
   {
      llvm_unreachable("TODO!");
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
