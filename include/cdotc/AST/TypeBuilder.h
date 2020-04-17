#ifndef CDOT_TYPEBUILDER_H
#define CDOT_TYPEBUILDER_H

#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/Sema/SemaPass.h"

#define DISPATCH(Name)                                                         \
   static_cast<SubClass*>(this)->visit##Name(static_cast<Name*>(*T))

namespace cdot {

template<class SubClass> class TypeBuilder {
protected:
   ast::SemaPass& SP;
   ast::ASTContext& Ctx;
   SourceRange SR;

   void visit(QualType T, SmallVectorImpl<QualType>& VariadicTypes)
   {
      if (auto* G = T->asTemplateParamType()) {
         static_cast<SubClass*>(this)->visitTemplateParamType(G, VariadicTypes);

         return;
      }

      switch (T->getTypeID()) {
#define CDOT_TYPE(Name, Parent)                                                \
   case Type::Name##ID:                                                        \
      VariadicTypes.push_back(DISPATCH(Name));                                 \
      break;
#include "cdotc/AST/Types.def"
      }
   }

public:
   explicit TypeBuilder(ast::SemaPass& SP, SourceRange SR)
       : SP(SP), Ctx(SP.getContext()), SR(SR)
   {
   }

   QualType visit(QualType T)
   {
      switch (T->getTypeID()) {
#define CDOT_TYPE(Name, Parent)                                                \
   case Type::Name##ID:                                                        \
      return DISPATCH(Name);                                                   \
      break;
#include "cdotc/AST/Types.def"
      }
   }

   QualType visitBuiltinType(BuiltinType* T) { return T; }

   QualType visitTokenType(TokenType* T) { return T; }

   QualType visitPointerType(PointerType* T)
   {
      return Ctx.getPointerType(visit(T->getPointeeType()));
   }

   QualType visitMutablePointerType(MutablePointerType* T)
   {
      return Ctx.getMutablePointerType(visit(T->getPointeeType()));
   }

   QualType visitReferenceType(ReferenceType* T)
   {
      return Ctx.getReferenceType(visit(T->getReferencedType()));
   }

   QualType visitMutableReferenceType(MutableReferenceType* T)
   {
      return Ctx.getMutableReferenceType(visit(T->getReferencedType()));
   }

   QualType visitMetaType(cdot::MetaType* T)
   {
      return Ctx.getMetaType(visit(T->getUnderlyingType()));
   }

   QualType visitArrayType(ArrayType* T)
   {
      return Ctx.getArrayType(visit(T->getElementType()), T->getNumElements());
   }

   QualType visitDependentSizeArrayType(DependentSizeArrayType* T)
   {
      return Ctx.getValueDependentSizedArrayType(visit(T->getElementType()),
                                                 T->getSizeExpr());
   }

   QualType visitInferredSizeArrayType(InferredSizeArrayType* T)
   {
      return Ctx.getInferredSizeArrayType(visit(T->getElementType()));
   }

   QualType visitTupleType(TupleType* T)
   {
      SmallVector<QualType, 4> ContainedTys;
      for (QualType Cont : T->getContainedTypes()) {
         visit(Cont, ContainedTys);
      }

      return Ctx.getTupleType(ContainedTys);
   }

   QualType visitExistentialType(ExistentialType* T)
   {
      SmallVector<QualType, 4> ContainedTys;
      for (QualType Cont : T->getExistentials()) {
         visit(Cont, ContainedTys);
      }

      return Ctx.getExistentialType(ContainedTys);
   }

   QualType visitFunctionType(FunctionType* T)
   {
      SmallVector<QualType, 4> ParamTys;
      for (QualType Cont : T->getParamTypes()) {
         visit(Cont, ParamTys);
      }

      return Ctx.getFunctionType(visit(T->getReturnType()), ParamTys,
                                 T->getParamInfo(), T->getRawFlags(),
                                 T->isLambdaType());
   }

   QualType visitLambdaType(LambdaType* T) { return visitFunctionType(T); }

   template<class Vector>
   void VisitTemplateArg(const sema::TemplateArgument& Arg, Vector& ArgList,
                         bool& Dependent)
   {
      if (!Arg.isType()) {
         ArgList.emplace_back(Arg.clone());
         Dependent |= ArgList.back().isStillDependent();

         return;
      }

      if (Arg.isVariadic()) {
         std::vector<sema::TemplateArgument> Cloned;
         for (auto& VA : Arg.getVariadicArgs()) {
            VisitTemplateArg(VA, Cloned, Dependent);
         }

         ArgList.emplace_back(Arg.getParam(), true, move(Cloned), Arg.getLoc());
         return;
      }

      SmallVector<QualType, 4> CopiedTypes;
      visit(Arg.getNonCanonicalType(), CopiedTypes);

      for (auto& Ty : CopiedTypes) {
         ArgList.emplace_back(Arg.getParam(), Ty, Arg.getLoc());
         Dependent |= ArgList.back().isStillDependent();
      }
   }

   sema::TemplateArgument VisitTemplateArg(const sema::TemplateArgument& Arg)
   {
      SmallVector<sema::TemplateArgument, 1> Args;
      bool Dependent;

      VisitTemplateArg(Arg, Args, Dependent);
      return std::move(Args.front());
   }

   QualType
   visitRecordTypeCommon(QualType T, ast::RecordDecl* R,
                         const sema::FinalTemplateArgumentList& TemplateArgs)
   {
      SmallVector<sema::TemplateArgument, 0> Args;

      bool Dependent = false;
      for (auto& Arg : TemplateArgs) {
         VisitTemplateArg(Arg, Args, Dependent);
      }

      auto FinalList = sema::FinalTemplateArgumentList::Create(
          SP.getContext(), Args, !Dependent);

      if (Dependent)
         return Ctx.getDependentRecordType(R, FinalList);

      auto* Template = R->isTemplate() ? R : R->getSpecializedTemplate();
      auto Inst = SP.InstantiateRecord(SR.getStart(), Template, FinalList);

      if (Inst)
         return Ctx.getRecordType(Inst);

      return T;
   }

   QualType
   visitTypedefTypeCommon(QualType T, ast::AliasDecl* td,
                          const sema::FinalTemplateArgumentList& TemplateArgs)
   {
      SmallVector<sema::TemplateArgument, 0> Args;

      bool Dependent = false;
      for (auto& Arg : TemplateArgs) {
         VisitTemplateArg(Arg, Args, Dependent);
      }

      auto FinalList = sema::FinalTemplateArgumentList::Create(
          SP.getContext(), Args, !Dependent);

      if (Dependent)
         return Ctx.getDependentTypedefType(td, FinalList);

      auto* Template = td->isTemplate() ? td : td->getSpecializedTemplate();
      auto Inst = SP.InstantiateAlias(SR.getStart(), Template, FinalList);

      if (Inst)
         return Ctx.getTypedefType(Inst);

      return T;
   }

   QualType visitRecordType(RecordType* T)
   {
      auto R = T->getRecord();
      if (R->isInstantiation()) {
         return visitRecordTypeCommon(T, R->getSpecializedTemplate(),
                                      R->getTemplateArgs());
      }

      return T;
   }

   QualType visitDependentRecordType(DependentRecordType* T)
   {
      return visitRecordTypeCommon(T, T->getRecord(), T->getTemplateArgs());
   }

   QualType visitDependentTypedefType(DependentTypedefType* T)
   {
      return visitTypedefTypeCommon(T, T->getTypedef(), T->getTemplateArgs());
   }

   QualType visitTemplateParamType(TemplateParamType* T)
   {
      SmallVector<QualType, 1> Args;
      visitTemplateParamType(T, Args);

      return Args.empty() ? QualType(T) : Args.front();
   }

   void visitTemplateParamType(TemplateParamType* T,
                               SmallVectorImpl<QualType>& Args)
   {
      Args.emplace_back(T);
   }

   NestedNameSpecifier* visitNestedNameSpecifier(NestedNameSpecifier* Name)
   {
      if (!Name)
         return nullptr;

      switch (Name->getKind()) {
      case NestedNameSpecifier::Type:
         return NestedNameSpecifier::Create(
             Ctx.getDeclNameTable(), visit(Name->getType()),
             visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::Identifier:
         return NestedNameSpecifier::Create(
             Ctx.getDeclNameTable(), Name->getIdentifier(),
             visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::Namespace:
         return NestedNameSpecifier::Create(
             Ctx.getDeclNameTable(), Name->getNamespace(),
             visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::TemplateParam:
         return NestedNameSpecifier::Create(
             Ctx.getDeclNameTable(), Name->getParam(),
             visitNestedNameSpecifier(Name->getPrevious()));

      case NestedNameSpecifier::AssociatedType:
         return NestedNameSpecifier::Create(
             Ctx.getDeclNameTable(), Name->getAssociatedType(),
             visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::Alias:
         return NestedNameSpecifier::Create(
             Ctx.getDeclNameTable(), Name->getAlias(),
             visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::Module:
         return NestedNameSpecifier::Create(
             Ctx.getDeclNameTable(), Name->getModule(),
             visitNestedNameSpecifier(Name->getPrevious()));
      case NestedNameSpecifier::TemplateArgList:
         return NestedNameSpecifier::Create(
             Ctx.getDeclNameTable(), Name->getTemplateArgs(),
             visitNestedNameSpecifier(Name->getPrevious()));
      }
   }

   QualType visitDependentNameType(DependentNameType* T)
   {
      auto* Name = visitNestedNameSpecifier(T->getNameSpec());
      if (Name != T->getNameSpec()) {
         auto* WithLoc = NestedNameSpecifierWithLoc::Create(
             Ctx.getDeclNameTable(), Name,
             T->getNameSpecWithLoc()->getSourceRanges());

         return this->Ctx.getDependentNameType(WithLoc);
      }

      return T;
   }

   QualType visitTypeVariableType(TypeVariableType* T) { return T; }

   QualType visitAssociatedType(AssociatedType* T)
   {
      if (auto OuterAT = T->getOuterAT()) {
         return this->Ctx.getAssociatedType(T->getDecl(), visit(OuterAT));
      }

      return T;
   }

   QualType visitTypedefType(TypedefType* T)
   {
      auto td = T->getTypedef();
      if (td->isInstantiation()) {
         return visitTypedefTypeCommon(T, td->getSpecializedTemplate(),
                                       td->getTemplateArgs());
      }

      return visit(T->getDesugaredType());
   }

   QualType visitBoxType(BoxType* T)
   {
      return Ctx.getBoxType(visit(T->getBoxedType()));
   }
};

#undef DISPATCH

} // namespace cdot

#endif // CDOT_TYPEBUILDER_H
