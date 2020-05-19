#ifndef CDOT_TYPEVISITOR_H
#define CDOT_TYPEVISITOR_H

#include "cdotc/AST/Type.h"
#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/Sema/Template.h"

#define DISPATCH(Name)                                                         \
   static_cast<SubClass*>(this)->visit##Name(static_cast<Name*>(T))

namespace cdot {

template<class SubClass, class RetTy = void> class TypeVisitor {
public:
   RetTy visit(Type* T)
   {
      switch (T->getTypeID()) {
#define CDOT_TYPE(Name, Parent)                                                \
   case Type::Name##ID:                                                        \
      return DISPATCH(Name);
#include "cdotc/AST/Types.def"
      }
   }

#define CDOT_TYPE(Name, Parent)                                                \
   RetTy visit##Name(Name* T) { return DISPATCH(Parent); }
#include "cdotc/AST/Types.def"

   RetTy visit(QualType Ty) { return visit((Type*)Ty.getBuiltinTy()); }

   RetTy visitType(Type*) { return RetTy(); }
};

template<class SubClass> class RecursiveTypeVisitor {
public:
   void visit(Type* T)
   {
      bool Continue = true;
      switch (T->getTypeID()) {
#define CDOT_TYPE(Name, Parent)                                                \
   case Type::Name##ID:                                                        \
      Continue = DISPATCH(Name);                                               \
      break;
#include "cdotc/AST/Types.def"
      }

      if (Continue)
         visitChildren(T);
   }

   bool visitType(Type *) { return true; }

#define CDOT_TYPE(Name, Parent)                                                \
   bool visit##Name(Name* T) { return DISPATCH(Parent); }
#include "cdotc/AST/Types.def"

private:
   void visitChildren(Type* T)
   {
      switch (T->getTypeID()) {
#define CDOT_TYPE(Name, Parent)                                                \
   case Type::Name##ID:                                                        \
      RecursiveTypeVisitor::visit##Name##Children(static_cast<Name*>(T));      \
      break;
#include "cdotc/AST/Types.def"
      }
   }

   void visitBuiltinTypeChildren(BuiltinType* T) {}

   void visitTokenTypeChildren(TokenType* T) {}

   void visitPointerTypeChildren(PointerType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getPointeeType());
   }

   void visitMutablePointerTypeChildren(MutablePointerType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getPointeeType());
   }

   void visitReferenceTypeChildren(ReferenceType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getReferencedType());
   }

   void visitMutableReferenceTypeChildren(MutableReferenceType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getReferencedType());
   }

   void visitMetaTypeChildren(cdot::MetaType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getUnderlyingType());
   }

   void visitArrayTypeChildren(ArrayType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getElementType());
   }

   void visitDependentSizeArrayTypeChildren(DependentSizeArrayType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getElementType());
   }

   void visitInferredSizeArrayTypeChildren(InferredSizeArrayType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getElementType());
   }

   void visitTupleTypeChildren(TupleType* T)
   {
      for (QualType Cont : T->getContainedTypes()) {
         static_cast<SubClass*>(this)->visit(Cont);
      }
   }

   void visitExistentialTypeChildren(ExistentialType* T)
   {
      for (QualType Cont : T->getExistentials()) {
         static_cast<SubClass*>(this)->visit(Cont);
      }
   }

   void visitFunctionTypeChildren(FunctionType* T)
   {
      for (QualType Cont : T->getParamTypes()) {
         static_cast<SubClass*>(this)->visit(Cont);
      }

      static_cast<SubClass*>(this)->visit(T->getReturnType());
   }

   void visitLambdaTypeChildren(LambdaType* T)
   {
      return visitFunctionTypeChildren(T);
   }

   bool VisitTemplateArg(const sema::TemplateArgument& Arg)
   {
      if (!Arg.isType())
         return true;

      if (Arg.isVariadic()) {
         for (auto& VA : Arg.getVariadicArgs())
            if (!VisitTemplateArg(VA))
               return false;

         return true;
      }

      static_cast<SubClass*>(this)->visit(Arg.getNonCanonicalType());
      return true;
   }

   void visitRecordTypeChildren(RecordType* T)
   {
      if (T->hasTemplateArgs()) {
         for (auto& Arg : T->getTemplateArgs()) {
            if (!VisitTemplateArg(Arg))
               return;
         }
      }
   }

   void visitDependentRecordTypeChildren(DependentRecordType* T)
   {
      for (auto& Arg : T->getTemplateArgs()) {
         if (!VisitTemplateArg(Arg))
            return;
      }
   }

   void visitDependentTypedefTypeChildren(DependentTypedefType* T)
   {
      for (auto& Arg : T->getTemplateArgs()) {
         if (!VisitTemplateArg(Arg))
            return;
      }
   }

   void visitTemplateParamTypeChildren(TemplateParamType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getCovariance());
   }

   void visitNestedNameSpecifier(NestedNameSpecifier* Name)
   {
      if (!Name)
         return;

      switch (Name->getKind()) {
      case NestedNameSpecifier::Type:
         static_cast<SubClass*>(this)->visit(Name->getType());
         break;
      case NestedNameSpecifier::Identifier:
      case NestedNameSpecifier::Namespace:
      case NestedNameSpecifier::Module:
      case NestedNameSpecifier::TemplateParam:
      case NestedNameSpecifier::AssociatedType:
      case NestedNameSpecifier::Alias:
      case NestedNameSpecifier::TemplateArgList:
         break;
         break;
      }

      visitNestedNameSpecifier(Name->getPrevious());
   }

   void visitDependentNameTypeChildren(DependentNameType* T)
   {
      return visitNestedNameSpecifier(T->getNameSpec());
   }

   void visitTypeVariableTypeChildren(TypeVariableType* T) {}

   void visitAssociatedTypeChildren(AssociatedType* T)
   {
      if (auto Outer = T->getOuterAT()) {
         static_cast<SubClass*>(this)->visit(Outer);
      }
   }

   void visitTypedefTypeChildren(TypedefType* T)
   {
      static_cast<SubClass*>(this)->visit(T->getAliasedType());
   }

   void visitBoxTypeChildren(BoxType* T) { static_cast<SubClass*>(this)->visit(T->getBoxedType()); }
};

template<class SubClass> class TypeComparer {
public:
   bool visit(QualType LHS, QualType RHS)
   {
      switch (LHS->getTypeID()) {
#define CDOT_TYPE(Name, Parent)                                                \
   case Type::Name##ID:                                                        \
      return static_cast<SubClass*>(this)->visit##Name(                        \
          support::cast<Name>(LHS), RHS);
#include "cdotc/AST/Types.def"
      }
   }

   bool compare(QualType LHS, QualType RHS)
   {
      return static_cast<SubClass*>(this)->compareImpl(LHS, RHS);
   }

protected:
   bool compareImpl(QualType LHS, QualType RHS) { return LHS == RHS; }

   bool visitBuiltinType(BuiltinType* LHS, QualType RHS)
   {
      return compare(RHS->getCanonicalType(), LHS->getCanonicalType());
   }

   bool visitTokenType(TokenType* LHS, QualType RHS)
   {
      return compare(RHS->getCanonicalType(), LHS->getCanonicalType());
   }

   bool visitPointerType(PointerType* LHS, QualType RHS)
   {
      if (auto* Ptr = RHS->asPointerType()) {
         return visit(LHS->getPointeeType(), Ptr->getPointeeType());
      }

      return false;
   }

   bool visitMutablePointerType(MutablePointerType* LHS, QualType RHS)
   {
      if (auto* Ptr = RHS->asMutablePointerType()) {
         return visit(LHS->getPointeeType(), Ptr->getPointeeType());
      }

      return false;
   }

   bool visitReferenceType(ReferenceType* LHS, QualType RHS)
   {
      if (auto* Ptr = RHS->asReferenceType()) {
         return visit(LHS->getReferencedType(), Ptr->getReferencedType());
      }

      return false;
   }

   bool visitMutableReferenceType(MutableReferenceType* LHS, QualType RHS)
   {
      if (auto* Ptr = RHS->asMutableReferenceType()) {
         return visit(LHS->getReferencedType(), Ptr->getReferencedType());
      }

      return false;
   }

   bool visitMetaType(cdot::MetaType* LHS, QualType RHS)
   {
      if (auto* Meta = RHS->asMetaType()) {
         return visit(LHS->getUnderlyingType(), Meta->getUnderlyingType());
      }

      return false;
   }

   bool visitArrayType(ArrayType* LHS, QualType RHS)
   {
      if (auto* Arr = RHS->asArrayType()) {
         if (LHS->getNumElements() != Arr->getNumElements()) {
            return false;
         }

         return visit(LHS->getElementType(), Arr->getElementType());
      }

      return false;
   }

   bool visitDependentSizeArrayType(DependentSizeArrayType* LHS, QualType RHS)
   {
      if (auto* Arr = RHS->asArrayType()) {
         return visit(LHS->getElementType(), Arr->getElementType());
      }

      return false;
   }

   bool visitInferredSizeArrayType(InferredSizeArrayType* LHS, QualType RHS)
   {
      if (auto* Arr = RHS->asArrayType()) {
         return visit(LHS->getElementType(), Arr->getElementType());
      }

      return false;
   }

   bool visitTupleType(TupleType* LHS, QualType RHS)
   {
      auto* Tup = RHS->asTupleType();
      if (!Tup || Tup->getArity() != LHS->getArity()) {
         return false;
      }

      unsigned Arity = LHS->getArity();
      for (unsigned i = 0; i < Arity; ++i) {
         if (!visit(LHS->getContainedType(i), Tup->getContainedType(i))) {
            return false;
         }
      }

      return true;
   }

   bool visitExistentialType(ExistentialType* LHS, QualType RHS)
   {
      auto* Tup = RHS->asExistentialType();
      if (!Tup) {
         return false;
      }

      auto LHSTypes = LHS->getExistentials();
      auto RHSTypes = Tup->getExistentials();

      if (LHSTypes.size() != RHSTypes.size()) {
         return false;
      }

      unsigned Arity = LHSTypes.size();
      for (unsigned i = 0; i < Arity; ++i) {
         if (!visit(LHSTypes[i], RHSTypes[i])) {
            return false;
         }
      }

      return true;
   }

   bool visitFunctionType(FunctionType* LHS, QualType RHS)
   {
      auto* Fun = RHS->asFunctionType();
      if (!Fun || Fun->getNumParams() != LHS->getNumParams()) {
         return false;
      }

      if (LHS->getRawFlags() != Fun->getRawFlags()) {
         return false;
      }

      auto LHSParams = LHS->getParamTypes();
      auto RHSParams = Fun->getParamTypes();

      auto LHSParamInfo = LHS->getParamInfo();
      auto RHSParamInfo = Fun->getParamInfo();

      unsigned Arity = LHS->getNumParams();
      for (unsigned i = 0; i < Arity; ++i) {
         if (LHSParamInfo[i] != RHSParamInfo[i]) {
            return false;
         }
         if (!visit(LHSParams[i], RHSParams[i])) {
            return false;
         }
      }

      return visit(LHS->getReturnType(), Fun->getReturnType());
   }

   bool visitLambdaType(LambdaType* LHS, QualType RHS)
   {
      return static_cast<SubClass*>(this)->visitFunctionType(LHS, RHS);
   }

   bool visitRecordType(RecordType* LHS, QualType RHS)
   {
      return compare(LHS->getCanonicalType(), RHS->getCanonicalType());
   }

   bool visitTemplateArg(const sema::TemplateArgument& LHS,
                         const sema::TemplateArgument& RHS)
   {
      if (LHS.isValue() || RHS.isValue()) {
         return false;
      }

      if (LHS.isVariadic()) {
         if (!RHS.isVariadic()) {
            return false;
         }

         auto& LHSArgs = LHS.getVariadicArgs();
         auto& RHSArgs = RHS.getVariadicArgs();

         auto N1 = LHSArgs.size();
         auto N2 = RHSArgs.size();

         if (N1 != N2) {
            return false;
         }

         for (unsigned i = 0; i < N1; ++i) {
            if (!visitTemplateArg(LHSArgs[i], RHSArgs[i])) {
               return false;
            }
         }

         return true;
      }

      return visit(LHS.getNonCanonicalType(), RHS.getNonCanonicalType());
   }

   bool compareTemplateArgLists(const sema::FinalTemplateArgumentList& LHSArgs,
                                const sema::FinalTemplateArgumentList& RHSArgs)
   {
      auto N1 = LHSArgs.size();
      auto N2 = RHSArgs.size();

      if (N1 != N2) {
         return false;
      }

      for (unsigned i = 0; i < N1; ++i) {
         if (!visitTemplateArg(LHSArgs[i], RHSArgs[i])) {
            return false;
         }
      }

      return true;
   }

   bool visitDependentRecordType(DependentRecordType* LHS, QualType RHS)
   {
      if (!RHS->isRecordType() || !RHS->hasTemplateArgs()) {
         return false;
      }

      auto& LHSArgs = LHS->getTemplateArgs();
      auto& RHSArgs = RHS->getTemplateArgs();

      if (!compareTemplateArgLists(LHSArgs, RHSArgs)) {
         return false;
      }

      ast::RecordDecl* RHSRec;
      if (auto* Dep = RHS->asDependentRecordType()) {
         RHSRec = Dep->getRecord();
      }
      else {
         RHSRec = RHS->getRecord()->getSpecializedTemplate();
      }

      return LHS->getRecord() == RHSRec;
   }

   bool visitDependentTypedefType(DependentTypedefType* LHS, QualType RHS)
   {
      if (!RHS->isTypedefType() || !RHS->hasTemplateArgs()) {
         return false;
      }

      auto& LHSArgs = LHS->getTemplateArgs();
      auto& RHSArgs = RHS->getTemplateArgs();

      if (!compareTemplateArgLists(LHSArgs, RHSArgs)) {
         return false;
      }

      ast::AliasDecl* RHStd;
      if (auto* Dep = RHS->asDependentTypedefType()) {
         RHStd = Dep->getTypedef();
      }
      else {
         RHStd = RHS->asTypedefType()->getTypedef()->getSpecializedTemplate();
      }

      return LHS->getTypedef() == RHStd;
   }

   bool visitTemplateParamType(TemplateParamType* LHS, QualType RHS)
   {
      return compare(LHS->getCanonicalType(), RHS->getCanonicalType());
   }

   bool
   visitNestedNameSpecifier(NestedNameSpecifier* Name, QualType RHS,
                            const sema::FinalTemplateArgumentList* templateArgs
                            = nullptr)
   {
      if (!Name) {
         return false;
      }

      switch (Name->getKind()) {
      case NestedNameSpecifier::Type:
         return compare(Name->getType()->getCanonicalType(),
                        RHS->getCanonicalType());
      case NestedNameSpecifier::TemplateArgList:
         return visitNestedNameSpecifier(Name->getPrevious(), RHS,
                                         Name->getTemplateArgs());
      case NestedNameSpecifier::Alias: {
         auto* alias = Name->getAlias();
         if (auto* td = RHS->asTypedefType()) {
            auto* otherTd = td->getTypedef();
            if (alias == otherTd && !templateArgs) {
               return true;
            }
            if (!otherTd->isInstantiation()
                || otherTd->getSpecializedTemplate() != alias) {
               return false;
            }
            if (!templateArgs) {
               return false;
            }
            if (!compareTemplateArgLists(*templateArgs,
                                         otherTd->getTemplateArgs())) {
               return false;
            }

            return true;
         }

         if (templateArgs) {
            return false;
         }

         return compare(alias->getType()->getCanonicalType(),
                        RHS->getCanonicalType());
      }
      case NestedNameSpecifier::Identifier:
      case NestedNameSpecifier::Namespace:
      case NestedNameSpecifier::Module:
      case NestedNameSpecifier::TemplateParam:
      case NestedNameSpecifier::AssociatedType:
         llvm_unreachable("FIXME: when can this happen?");
      }
   }

   bool visitDependentNameType(DependentNameType* LHS, QualType RHS)
   {
      assert(LHS->getNameSpec()->depth() <= 2 && "cannot handle this yet!");
      return visitNestedNameSpecifier(LHS->getNameSpec(), RHS);
   }

   bool visitTypeVariableType(TypeVariableType* LHS, QualType RHS)
   {
      return compare(LHS->getCanonicalType(), RHS->getCanonicalType());
   }

   bool visitAssociatedType(AssociatedType* LHS, QualType RHS)
   {
      return compare(LHS->getCanonicalType(), RHS->getCanonicalType());
   }

   bool visitTypedefType(TypedefType* LHS, QualType RHS)
   {
      return visit(LHS->getCanonicalType(), RHS);
   }

   bool visitBoxType(BoxType* LHS, QualType RHS)
   {
      return compare(LHS->getCanonicalType(), RHS->getCanonicalType());
   }
};

template<class CallbackFn, class... TypeClasses>
class SpecificTypeVisitor
    : public RecursiveTypeVisitor<SpecificTypeVisitor<CallbackFn, TypeClasses...>> {
   CallbackFn fn;

   template<class T> bool tryVisit(QualType Ty)
   {
      if (auto* specificNode = support::dyn_cast<T>(Ty)) {
         return this->fn(specificNode);
      }

      return true;
   }

   bool all() { return true; }

   template <class ...Bools>
   bool all(bool b, Bools... bools)
   {
      if (!b)
         return false;

      return all(bools...);
   }

public:
   explicit SpecificTypeVisitor(CallbackFn&& fn) : fn(std::move(fn)) {}

   bool visit(QualType Ty)
   {
      if (!all(tryVisit<TypeClasses>(Ty)...))
         return false;

      RecursiveTypeVisitor<
          SpecificTypeVisitor<CallbackFn, TypeClasses...>>::visit(Ty);

      return true;
   }

   bool visit(Type *Ty)
   {
      if (!all(tryVisit<TypeClasses>(Ty)...))
         return false;

      RecursiveTypeVisitor<
         SpecificTypeVisitor<CallbackFn, TypeClasses...>>::visit(Ty);

      return true;
   }
};

template<class... TypeClasses, class CallbackFn>
void visitSpecificType(CallbackFn&& fn, QualType Ty)
{
   SpecificTypeVisitor<CallbackFn, TypeClasses...>(std::move(fn)).visit(Ty);
}

} // namespace cdot

#undef DISPATCH

#endif // CDOT_TYPEVISITOR_H
