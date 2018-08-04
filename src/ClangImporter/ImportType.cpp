//
// Created by Jonas Zell on 30.07.18.
//

#include "ImporterImpl.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Driver/Compiler.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

using ImporterImpl = ClangImporter::ImporterImpl;

QualType ImporterImpl::getType(clang::QualType Ty)
{
   auto &Ctx = CI.getContext();

   switch (Ty->getTypeClass()) {
   case clang::Type::Builtin: {
      clang::TargetInfo &TI = Instance->getTarget();
      switch (Ty->castAs<clang::BuiltinType>()->getKind()) {
      case clang::BuiltinType::Void:
         return Ctx.getEmptyTupleType();
      case clang::BuiltinType::NullPtr:
         return Ctx.getPointerType(Ctx.getVoidType());
      case clang::BuiltinType::Dependent:
      case clang::BuiltinType::UnknownAny:
         return Ctx.getUnknownAnyTy();

      /// Integer types.
      case clang::BuiltinType::Bool:
         return Ctx.getBoolTy();
      case clang::BuiltinType::UChar:
      case clang::BuiltinType::Char_U:
      case clang::BuiltinType::SChar:
      case clang::BuiltinType::Char_S:
         return Ctx.getIntegerTy(TI.getCharWidth(),
                                 Ty->isUnsignedIntegerType());
      case clang::BuiltinType::Char16:
         return Ctx.getIntegerTy(16, Ty->isUnsignedIntegerType());
      case clang::BuiltinType::Char32:
      case clang::BuiltinType::WChar_S:
      case clang::BuiltinType::WChar_U:
         return Ctx.getIntegerTy(32, Ty->isUnsignedIntegerType());
      case clang::BuiltinType::Short:
      case clang::BuiltinType::UShort:
         return Ctx.getIntegerTy(TI.getShortWidth(),
                                 Ty->isUnsignedIntegerType());
      case clang::BuiltinType::Int:
      case clang::BuiltinType::UInt:
         return Ctx.getIntegerTy(TI.getIntWidth(),
                                 Ty->isUnsignedIntegerType());
      case clang::BuiltinType::Long:
      case clang::BuiltinType::ULong:
         return Ctx.getIntegerTy(TI.getLongWidth(),
                                 Ty->isUnsignedIntegerType());
      case clang::BuiltinType::LongLong:
      case clang::BuiltinType::ULongLong:
         return Ctx.getIntegerTy(TI.getLongLongWidth(),
                                 Ty->isUnsignedIntegerType());
      case clang::BuiltinType::Int128:
      case clang::BuiltinType::UInt128:
         return Ctx.getIntegerTy(128,
                                 Ty->isUnsignedIntegerType());

      /// Floating point types.
      case clang::BuiltinType::Half:
         return Ctx.getf16Ty();
      case clang::BuiltinType::LongDouble:
         switch (TI.getLongDoubleWidth()) {
         case 64: return Ctx.getf64Ty();
         case 128: return Ctx.getf128Ty();
         default:
            llvm_unreachable("unexpected long double width");
         }
      case clang::BuiltinType::Float16:
         return Ctx.getf16Ty();
      case clang::BuiltinType::Float128:
         return Ctx.getf128Ty();
      case clang::BuiltinType::Float:
         return Ctx.getf32Ty();
      case clang::BuiltinType::Double:
         return Ctx.getf64Ty();
      // Can't translate type.
      default:
         return QualType();
      }
   }
   case clang::Type::Complex: {
      auto *C = cast<clang::ComplexType>(Ty);
      auto ElTy = getType(C->getElementType());
      if (!ElTy)
         return QualType();

      return Ctx.getTupleType({ElTy, ElTy});
   }
   case clang::Type::Pointer: {
      if (Ty->getPointeeType()->isCharType()) {
         // import any char* as UnsafePtr<UInt8>
         return Ctx.getUInt8PtrTy();
      }

      auto Pointee = getType(Ty->getPointeeType());
      if (!Pointee) {
         // Import as an opaque pointer.
         return Ctx.getUInt8PtrTy();
      }

      if (Ty.isConstQualified()) {
         return Ctx.getPointerType(Pointee);
      }

      return Ctx.getMutablePointerType(Pointee);
   }
   case clang::Type::BlockPointer:
      return Ctx.getUInt8PtrTy();
   case clang::Type::LValueReference:
   case clang::Type::RValueReference: {
      auto Pointee = getType(Ty->getPointeeType());
      if (!Pointee) {
         // Import as an opaque pointer.
         return Ctx.getUInt8PtrTy();
      }

      if (Ty.isConstQualified()) {
         return Ctx.getReferenceType(Pointee);
      }

      return Ctx.getMutableReferenceType(Pointee);
   }
   case clang::Type::ConstantArray: {
      auto ElementTy = getType(Ty->getAsArrayTypeUnsafe()->getElementType());
      if (!ElementTy)
         return QualType();

      auto &Size = cast<clang::ConstantArrayType>(Ty->getAsArrayTypeUnsafe())
         ->getSize();

      return Ctx.getArrayType(ElementTy, Size.getZExtValue());
   }
   case clang::Type::VariableArray:
   case clang::Type::IncompleteArray: {
      auto ElementTy = getType(Ty->getAsArrayTypeUnsafe()->getElementType());
      if (!ElementTy)
         return QualType();

      return Ctx.getPointerType(ElementTy);
   }
   case clang::Type::FunctionNoProto: {
      auto *Fn = Ty->castAs<clang::FunctionNoProtoType>();
      auto Ret = getType(Fn->getReturnType());
      if (!Ret)
         return QualType();

      return Ctx.getFunctionType(Ret, {}, {});
   }
   case clang::Type::FunctionProto: {
      auto *Fn = Ty->castAs<clang::FunctionProtoType>();

      SmallVector<QualType, 4> Params;
      SmallVector<FunctionType::ParamInfo, 4> ParamInfo;

      for (auto &ParamTy : Fn->getParamTypes()) {
         auto Param = getType(ParamTy);
         if (!Param)
            return QualType();

         Params.push_back(Param);
         ParamInfo.emplace_back();
      }

      auto Ret = getType(Fn->getReturnType());
      if (!Ret)
         return QualType();

      return Ctx.getFunctionType(Ret, Params, ParamInfo);
   }
   case clang::Type::Paren:
   case clang::Type::Adjusted:
   case clang::Type::Typedef:
   case clang::Type::Decayed:
   case clang::Type::TypeOf:
   case clang::Type::TypeOfExpr:
   case clang::Type::Decltype:
   case clang::Type::UnaryTransform:
   case clang::Type::Elaborated:
   case clang::Type::Attributed:
      return getType(Ty.getCanonicalType());
   case clang::Type::Record: {
      auto *RecTy = Ty->castAs<clang::RecordType>();
      if (RecTy->getDecl()->isStruct()) {
         auto *Rec = importStruct(RecTy->getDecl());
         if (!Rec)
            return QualType();

         return Ctx.getRecordType(cast<RecordDecl>(Rec));
      }

      if (RecTy->getDecl()->isUnion()) {
         clang::ASTContext &ASTCtx = Instance->getASTContext();
         unsigned Size = 0;

         for (clang::FieldDecl *F : RecTy->getDecl()->fields()) {
            Size += ASTCtx.getTypeSize(F->getType());
         }

         return Ctx.getArrayType(Ctx.getUInt8Ty(), Size);
      }

      return QualType();
   }
   case clang::Type::Enum: {
      auto *Rec = importEnum(Ty->castAs<clang::EnumType>()->getDecl());
      if (!Rec)
         return QualType();

      return Ctx.getRecordType(cast<EnumDecl>(Rec));
   }
   case clang::Type::Auto:
      return Ctx.getAutoTy();
   case clang::Type::Atomic:
      return getType(Ty->castAs<clang::AtomicType>()->getValueType());
   case clang::Type::MemberPointer:
   case clang::Type::DependentSizedArray:
   case clang::Type::Vector:
   case clang::Type::ExtVector:
   case clang::Type::DependentSizedExtVector:
   default:
      return QualType();
   }
}