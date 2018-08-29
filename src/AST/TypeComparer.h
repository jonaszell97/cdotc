////
//// Created by Jonas Zell on 22.08.18.
////
//
//#ifndef CDOT_TYPECOMPARER_H
//#define CDOT_TYPECOMPARER_H
//
//#include "AST/Type.h"
//#include "Basic/NestedNameSpecifier.h"
//#include "Sema/Template.h"
//
//#define DISPATCH(Name)                                  \
//  static_cast<SubClass*>(this)->                        \
//           visit##Name(static_cast<const Name*>(T))
//
//namespace cdot {
//namespace ast {
//
//template<class SubClass>
//class TypeComparer {
//public:
//   bool compare(QualType LHS, QualType RHS)
//   {
//      if (LHS == RHS)
//         return true;
//
//      switch (LHS->getTypeID()) {
//#  define CDOT_TYPE(NAME, PARENT)                                              \
//      case Type::NAME##ID:                                                     \
//         return static_cast<SubClass*>(this)->visit##NAME(cast<NAME>(LHS), RHS);
//      }
//#  include "Types.def"
//   }
//
//   bool visitBuiltinType(BuiltinType *T, QualType RHS)
//   {
//      return RHS.getCanonicalType() == T;
//   }
//
//   bool visitTokenType(TokenType *T, QualType RHS)
//   {
//      return RHS.getCanonicalType() == T;
//   }
//
//   bool visitPointerType(PointerType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::PointerTypeID)
//         return false;
//
//      return compare(T->getPointeeType(), RHS->getPointeeType());
//   }
//
//   bool visitMutablePointerType(MutablePointerType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::MutablePointerTypeID)
//         return false;
//
//      return compare(T->getPointeeType(), RHS->getPointeeType());
//   }
//
//   bool visitReferenceType(ReferenceType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::ReferenceTypeID)
//         return false;
//
//      return compare(T->getReferencedType(), RHS->getReferencedType());
//   }
//
//   bool visitMutableReferenceType(MutableReferenceType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::MutableReferenceTypeID)
//         return false;
//
//      return compare(T->getReferencedType(), RHS->getReferencedType());
//   }
//
//   bool visitMutableBorrowType(MutableBorrowType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::MutableBorrowTypeID)
//         return false;
//
//      return compare(T->getReferencedType(), RHS->getReferencedType());
//   }
//
//   bool visitMetaType(MetaType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::MetaTypeID)
//         return false;
//
//      return compare(T->getUnderlyingType(),
//                     RHS->uncheckedAsMetaType()->getUnderlyingType());
//   }
//
//   bool visitArrayType(ArrayType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::ArrayTypeID)
//         return false;
//
//      return compare(T->getElementType(),
//                     RHS->uncheckedAsArrayType()->getElementType());
//   }
//
//   bool visitDependentSizeArrayType(DependentSizeArrayType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::DependentSizeArrayTypeID)
//         return false;
//
//      return compare(T->getElementType(),
//                     RHS->uncheckedAsArrayType()->getElementType());
//   }
//
//   bool visitInferredSizeArrayType(InferredSizeArrayType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::InferredSizeArrayTypeID)
//         return false;
//
//      return compare(T->getElementType(),
//                     RHS->uncheckedAsArrayType()->getElementType());
//   }
//
//   bool visitTupleType(TupleType *T, QualType RHS)
//   {
//      if (RHS->getTypeID() != Type::TupleTypeID)
//         return false;
//
//      for (QualType Cont : T->getContainedTypes()) {
//         visit(Cont);
//      }
//   }
//
//   bool visitFunctionType(FunctionType *T, QualType RHS)
//   {
//      for (QualType Cont : T->getParamTypes()) {
//         visit(Cont);
//      }
//
//      return visit(T->getReturnType());
//   }
//
//   bool visitLambdaType(LambdaType *T, QualType RHS)
//   {
//      return visitFunctionType(T);
//   }
//
//   bool VisitTemplateArg(sema::TemplateArgument &Arg)
//   {
//      if (!Arg.isType())
//         return true;
//
//      if (Arg.isVariadic()) {
//         for (auto &VA : Arg.getVariadicArgs())
//            if (!VisitTemplateArg(VA))
//               return false;
//
//         return true;
//      }
//
//      visit(Arg.getType());
//      return true;
//   }
//
//   bool visitRecordType(RecordType *T, QualType RHS)
//   {
//      if (T->hasTemplateArgs()) {
//         for (auto &Arg : T->getTemplateArgs()) {
//            if (!VisitTemplateArg(Arg))
//               return;
//         }
//      }
//   }
//
//   bool visitDependentRecordType(DependentRecordType *T, QualType RHS)
//   {
//      for (auto &Arg : T->getTemplateArgs()) {
//         if (!VisitTemplateArg(Arg))
//            return;
//      }
//   }
//
//   bool visitGenericType(GenericType *T, QualType RHS)
//   {
//      return visit(T->getCovariance());
//   }
//
//   bool visitNestedNameSpecifier(NestedNameSpecifier *Name)
//   {
//      if (!Name)
//         return;
//
//      switch (Name->getKind()) {
//      case NestedNameSpecifier::Type:
//         visit(Name->getType());
//         break;
//      case NestedNameSpecifier::Identifier:
//      case NestedNameSpecifier::Namespace:
//      case NestedNameSpecifier::Module:
//      case NestedNameSpecifier::TemplateParam:
//         break;
//      case NestedNameSpecifier::AssociatedType:
//         break;
//      }
//
//      visitNestedNameSpecifier(Name->getPrevious());
//   }
//
//   bool visitDependentNameType(DependentNameType *T, QualType RHS)
//   {
//      return visitNestedNameSpecifier(T->getNameSpec());
//   }
//
//   bool visitAssociatedType(AssociatedType *T, QualType RHS)
//   {
//      if (QualType Ty = T->getActualType())
//         visit(Ty);
//   }
//
//   bool visitTypedefType(TypedefType *T, QualType RHS)
//   {
//      return visit(T->getAliasedType());
//   }
//
//   bool visitBoxType(BoxType *T, QualType RHS)
//   {
//      return visit(T->getBoxedType());
//   }
//};
//
//} // namespace ast
//} // namespace cdot
//
//#undef DISPATCH
//
//#endif //CDOT_TYPECOMPARER_H
