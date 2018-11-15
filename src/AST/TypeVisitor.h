//
// Created by Jonas Zell on 30.03.18.
//

#ifndef CDOT_TYPEVISITOR_H
#define CDOT_TYPEVISITOR_H

#include "Type.h"
#include "Basic/NestedNameSpecifier.h"
#include "Sema/Template.h"

#define DISPATCH(Name)                                  \
  static_cast<SubClass*>(this)->                        \
           visit##Name(static_cast<const Name*>(T))

namespace cdot {

template<class SubClass, class RetTy = void>
class TypeVisitor {
public:
   RetTy visit(const Type *T)
   {
      switch (T->getTypeID()) {
#     define CDOT_TYPE(Name, Parent)                              \
      case Type::Name##ID: return DISPATCH(Name);
#     include "Types.def"
      }
   }

#  define CDOT_TYPE(Name, Parent)                                 \
   RetTy visit##Name(const Name *T) { return DISPATCH(Parent); }
#  include "Types.def"

   RetTy visit(QualType Ty)
   {
      return visit((const Type*)Ty.getBuiltinTy());
   }

   RetTy visitType(const Type*) { return RetTy(); }
};

template<class SubClass>
class RecursiveTypeVisitor {
public:
   void visit(const Type *T)
   {
      bool Continue = true;
      switch (T->getTypeID()) {
#     define CDOT_TYPE(Name, Parent)                              \
      case Type::Name##ID: Continue = DISPATCH(Name); break;
#     include "Types.def"
      }

      if (Continue)
         visitChildren(T);
   }

#  define CDOT_TYPE(Name, Parent)                                 \
   bool visit##Name(const Name *T) { return DISPATCH(Parent); }
#  include "Types.def"

   bool visitType(const Type*) { return true; }
   
private:
   void visitChildren(const Type *T)
   {
      switch (T->getTypeID()) {
#     define CDOT_TYPE(Name, Parent)                                                \
      case Type::Name##ID:                                                          \
         RecursiveTypeVisitor::visit##Name##Children(static_cast<const Name*>(T));  \
         break;
#     include "Types.def"
      }
   }

   void visitBuiltinTypeChildren(const BuiltinType *T)
   {
   }

   void visitTokenTypeChildren(const TokenType *T)
   {
   }

   void visitPointerTypeChildren(const PointerType *T)
   {
      return visit(T->getPointeeType());
   }

   void visitMutablePointerTypeChildren(const MutablePointerType *T)
   {
      return visit(T->getPointeeType());
   }

   void visitReferenceTypeChildren(const ReferenceType *T)
   {
      return visit(T->getReferencedType());
   }

   void visitMutableReferenceTypeChildren(const MutableReferenceType *T)
   {
      return visit(T->getReferencedType());
   }

   void visitMutableBorrowTypeChildren(const MutableBorrowType *T)
   {
      return visit(T->getReferencedType());
   }

   void visitMetaTypeChildren(const cdot::MetaType *T)
   {
      return visit(T->getUnderlyingType());
   }

   void visitArrayTypeChildren(const ArrayType *T)
   {
      return visit(T->getElementType());
   }

   void visitDependentSizeArrayTypeChildren(const DependentSizeArrayType *T)
   {
      return visit(T->getElementType());
   }

   void visitInferredSizeArrayTypeChildren(const InferredSizeArrayType *T)
   {
      return visit(T->getElementType());
   }

   void visitTupleTypeChildren(const TupleType *T)
   {
      for (QualType Cont : T->getContainedTypes()) {
         visit(Cont);
      }
   }

   void visitExistentialTypeChildren(const ExistentialType *T)
   {
      for (QualType Cont : T->getExistentials()) {
         visit(Cont);
      }
   }

   void visitFunctionTypeChildren(const FunctionType *T)
   {
      for (QualType Cont : T->getParamTypes()) {
         visit(Cont);
      }

      return visit(T->getReturnType());
   }

   void visitLambdaTypeChildren(const LambdaType *T)
   {
      return visitFunctionTypeChildren(T);
   }

   bool VisitTemplateArg(const sema::TemplateArgument &Arg)
   {
      if (!Arg.isType())
         return true;

      if (Arg.isVariadic()) {
         for (auto &VA : Arg.getVariadicArgs())
           if (!VisitTemplateArg(VA))
              return false;

         return true;
      }

      visit(Arg.getNonCanonicalType());
      return true;
   }

   void visitRecordTypeChildren(const RecordType *T)
   {
      if (T->hasTemplateArgs()) {
         for (auto &Arg : T->getTemplateArgs()) {
            if (!VisitTemplateArg(Arg))
               return;
         }
      }
   }

   void visitDependentRecordTypeChildren(const DependentRecordType *T)
   {
      for (auto &Arg : T->getTemplateArgs()) {
         if (!VisitTemplateArg(Arg))
            return;
      }
   }

   void visitGenericTypeChildren(const GenericType *T)
   {
      return visit(T->getCovariance());
   }

   void visitNestedNameSpecifier(const NestedNameSpecifier *Name)
   {
      if (!Name)
         return;

      switch (Name->getKind()) {
      case NestedNameSpecifier::Type:
         visit(Name->getType());
         break;
      case NestedNameSpecifier::Identifier:
      case NestedNameSpecifier::Namespace:
      case NestedNameSpecifier::Module:
      case NestedNameSpecifier::TemplateParam:
         break;
      case NestedNameSpecifier::AssociatedType:
         break;
      }

      visitNestedNameSpecifier(Name->getPrevious());
   }

   void visitDependentNameTypeChildren(const DependentNameType *T)
   {
      return visitNestedNameSpecifier(T->getNameSpec());
   }

   void visitAssociatedTypeChildren(const AssociatedType *T)
   {
      if (auto Outer = T->getOuterAT()) {
         visit(Outer);
      }
   }

   void visitTypedefTypeChildren(const TypedefType *T)
   {
      return visit(T->getAliasedType());
   }

   void visitBoxTypeChildren(const BoxType *T)
   {
      return visit(T->getBoxedType());
   }
};

} // namespace cdot

#undef DISPATCH

#endif //CDOT_TYPEVISITOR_H
