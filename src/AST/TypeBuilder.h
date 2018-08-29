//
// Created by Jonas Zell on 09.04.18.
//

#ifndef CDOT_TYPEBUILDER_H
#define CDOT_TYPEBUILDER_H

#include "Basic/NestedNameSpecifier.h"
#include "Sema/SemaPass.h"

#define DISPATCH(Name)                                  \
  static_cast<SubClass*>(this)->                        \
           visit##Name(static_cast<Name*>(*T))

namespace cdot {

template<class SubClass>
class TypeBuilder {
protected:
   ast::SemaPass &SP;
   ast::ASTContext &Ctx;
   StmtOrDecl SOD;

public:
   explicit TypeBuilder(ast::SemaPass &SP, StmtOrDecl SOD)
      : SP(SP), Ctx(SP.getContext()), SOD(SOD)
   {}

   QualType visit(QualType T)
   {
      switch (T->getTypeID()) {
#     define CDOT_TYPE(Name, Parent)                              \
      case Type::Name##ID: return DISPATCH(Name); break;
#     include "Types.def"
      }
   }

   QualType visitBuiltinType(BuiltinType *T)
   {
      return T;
   }

   QualType visitTokenType(TokenType *T)
   {
      return T;
   }

   QualType visitPointerType(PointerType *T)
   {
      return Ctx.getPointerType(visit(T->getPointeeType()));
   }

   QualType visitMutablePointerType(MutablePointerType *T)
   {
      return Ctx.getMutablePointerType(visit(T->getPointeeType()));
   }

   QualType visitReferenceType(ReferenceType *T)
   {
      return Ctx.getReferenceType(visit(T->getReferencedType()));
   }

   QualType visitMutableReferenceType(MutableReferenceType *T)
   {
      return Ctx.getMutableReferenceType(visit(T->getReferencedType()));
   }

   QualType visitMutableBorrowType(MutableBorrowType *T)
   {
      return Ctx.getMutableBorrowType(visit(T->getReferencedType()));
   }

   QualType visitMetaType(cdot::MetaType *T)
   {
      return Ctx.getMetaType(visit(T->getUnderlyingType()));
   }

   QualType visitArrayType(ArrayType *T)
   {
      return Ctx.getArrayType(visit(T->getElementType()), T->getNumElements());
   }

   QualType visitDependentSizeArrayType(DependentSizeArrayType *T)
   {
      return Ctx.getValueDependentSizedArrayType(visit(T->getElementType()),
                                                 T->getSizeExpr());
   }

   QualType visitInferredSizeArrayType(InferredSizeArrayType *T)
   {
      return Ctx.getInferredSizeArrayType(visit(T->getElementType()));
   }

   QualType visitTupleType(TupleType *T)
   {
      llvm::SmallVector<QualType, 4> ContainedTys;
      for (QualType Cont : T->getContainedTypes())
         ContainedTys.push_back(visit(Cont));

      return Ctx.getTupleType(ContainedTys);
   }

   QualType visitFunctionType(FunctionType *T)
   {
      SmallVector<QualType, 4> ParamTys;
      for (QualType Cont : T->getParamTypes())
         ParamTys.push_back(visit(Cont));

      return Ctx.getFunctionType(visit(T->getReturnType()), ParamTys,
                                 T->getParamInfo(), T->getRawFlags(),
                                 T->isLambdaType());
   }

   QualType visitLambdaType(LambdaType *T)
   {
      return visitFunctionType(T);
   }

   sema::TemplateArgument
   VisitTemplateArg(const sema::TemplateArgument &Arg)
   {
      if (!Arg.isType())
         return Arg.clone();

      if (Arg.isVariadic()) {
         std::vector<sema::TemplateArgument> Cloned;
         for (auto &VA : Arg.getVariadicArgs())
            Cloned.push_back(VisitTemplateArg(VA));

         return sema::TemplateArgument(Arg.getParam(), true, move(Cloned),
                                          Arg.getLoc());
      }

      QualType Ty = visit(Arg.getNonCanonicalType());
      return sema::TemplateArgument(Arg.getParam(), Ty, Arg.getLoc());
   }

   QualType visitRecordTypeCommon(QualType T, ast::RecordDecl *R,
                          const sema::FinalTemplateArgumentList &TemplateArgs) {
      SmallVector<sema::TemplateArgument, 0> Args;

      bool Dependent = false;
      for (auto &Arg : TemplateArgs) {
         auto Copy = VisitTemplateArg(Arg);
         Dependent |= Copy.isStillDependent();

         Args.emplace_back(move(Copy));
      }

      auto FinalList = sema::FinalTemplateArgumentList::Create(
         SP.getContext(), Args, !Dependent);

      if (Dependent)
         return Ctx.getDependentRecordType(R, FinalList);

      auto *Template = R->isTemplate() ? R : R->getSpecializedTemplate();
      auto Inst = SP.InstantiateRecord(SOD.getSourceLoc(), Template, FinalList);

      if (Inst)
         return Ctx.getRecordType(Inst);

      return T;
   }

   QualType visitRecordType(RecordType *T)
   {
      auto  R = T->getRecord();
      if (R->isInstantiation()) {
         return visitRecordTypeCommon(T, R, R->getTemplateArgs());
      }

      return T;
   }

   QualType visitDependentRecordType(DependentRecordType *T)
   {
      return visitRecordTypeCommon(T, T->getRecord(), T->getTemplateArgs());
   }

   QualType visitGenericType(GenericType *T)
   {
      return T;
   }

   NestedNameSpecifier *visitNestedNameSpecifier(NestedNameSpecifier *Name)
   {
      if (!Name)
         return nullptr;

      switch (Name->getKind()) {
      case NestedNameSpecifier::Type:
         return NestedNameSpecifier::Create(Ctx.getDeclNameTable(),
                                visit(Name->getType()),
                                visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::Identifier:
         return NestedNameSpecifier::Create(Ctx.getDeclNameTable(),
                                Name->getIdentifier(),
                                visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::Namespace:
         return NestedNameSpecifier::Create(Ctx.getDeclNameTable(),
                              Name->getNamespace(),
                              visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::TemplateParam:
         return NestedNameSpecifier::Create(Ctx.getDeclNameTable(),
                                Name->getParam(),
                                visitNestedNameSpecifier(Name->getPrevious()));

      case NestedNameSpecifier::AssociatedType:
         return NestedNameSpecifier::Create(Ctx.getDeclNameTable(),
                                Name->getAssociatedType(),
                                visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::Module:
         return NestedNameSpecifier::Create(Ctx.getDeclNameTable(),
                                Name->getModule(),
                                visitNestedNameSpecifier(Name->getPrevious()));
      }
   }

   QualType visitDependentNameType(DependentNameType *T)
   {
      auto *Name = visitNestedNameSpecifier(T->getNameSpec());
      if (Name != T->getNameSpec()) {
         auto *WithLoc = NestedNameSpecifierWithLoc::Create(
            Ctx.getDeclNameTable(), Name,
            T->getNameSpecWithLoc()->getSourceRanges());

         return this->Ctx.getDependentNameType(WithLoc);
      }

      return T;
   }

   QualType visitAssociatedType(AssociatedType *T)
   {
      return T;
   }

   QualType visitTypedefType(TypedefType *T)
   {
      return T;
   }

   QualType visitBoxType(BoxType *T)
   {
      return Ctx.getBoxType(visit(T->getBoxedType()));
   }
};

#undef DISPATCH

} // namespace cdot


#endif //CDOT_TYPEBUILDER_H
