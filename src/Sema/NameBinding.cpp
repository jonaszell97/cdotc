////
//// Created by Jonas Zell on 23.08.18.
////
//
//#include "NameBinding.h"
//
//#include "cdotc/Basic/NestedNameSpecifier.h"
//#include "cdotc/Sema/SemaPass.h"//#include "cdotc/Support/StringSwitch.h"
//
// using namespace cdot;
// using namespace cdot::ast;
// using namespace cdot::diag;
// using namespace cdot::support;
//
// NameBinder::NameBinder(SemaPass &Sema,
//                       SmallPtrSetImpl<NamedDecl *> *ReferencedDecls)
//   : Sema(Sema), ReferencedDecls(ReferencedDecls)
//{}
//
// bool NameBinder::bindNames(Expression *E)
//{
//   // Reset error.
//   HadError = false;
//
//   visit(E);
//   return HadError;
//}
//
// bool NameBinder::visitTemplateArgListExpr(TemplateArgListExpr *E)
//{
//   // We don't actually instantiate anything during this stage, just visit
//   // the parent expression.
//   return visit(E->getParentExpr());
//}
//
// static bool CheckBuiltinType(SemaPass &SP, IdentifierRefExpr *Ident)
//{
//   if (!Ident->getDeclName().isSimpleIdentifier())
//      return false;
//
//   if (SP.getBuiltinType(Ident->getDeclName()))
//      return true;
//
//   return StringSwitch<bool>(Ident->getDeclName().getIdentifierInfo()
//                                  ->getIdentifier())
//        .Case("MetaType", true)
//        .Case("UnsafePtr", true)
//        .Case("UnsafeMutablePtr", true)
//        .Case("UnsafeRawPtr", true)
//        .Case("UnsafeMutableRawPtr", true)
//        .Default(false);
//}
//
// static ExprResult makeNestedNameSpec(SemaPass &SP,
//                                     ASTContext &Context,
//                                     NestedNameSpecifier *NameSpec,
//                                     IdentifierRefExpr *Ident) {
//   NameSpec = NestedNameSpecifier::Create(Context.getDeclNameTable(),
//                                          Ident->getIdentInfo(),
//                                          NameSpec);
//
//   SmallVector<SourceRange, 4> Locs{ Ident->getSourceRange() };
//   Expression *Curr = Ident->getParentExpr();
//
//   while (Curr) {
//      auto *IE = dyn_cast<IdentifierRefExpr>(Curr->ignoreTemplateArgs());
//      if (!IE || !IE->getNameSpec())
//         break;
//
//      Locs.push_back(IE->getSourceRange());
//      Curr = Curr->getParentExpr();
//   }
//
//   std::reverse(Locs.begin(), Locs.end());
//
//   auto WithLoc =
//   NestedNameSpecifierWithLoc::Create(Context.getDeclNameTable(),
//                                                     NameSpec, Locs);
//
//   auto Ty = Context.getDependentNameType(WithLoc);
//   Ident->setNameSpec(NameSpec);
//   Ident->setKind(IdentifierKind::MetaType);
//   Ident->setExprType(Context.getMetaType(Ty));
//   Ident->setIsTypeDependent(true);
//
//   return Ident;
//}
//
// bool NameBinder::visitIdentifierRefExpr(IdentifierRefExpr *Ident)
//{
//   // Check if this expressions parent expr refers to a namespace.
//   auto *NameSpec = Sema.checkNamespaceRef(Ident);
//   if (Ident->isInvalid()) {
//      HadError = true;
//      return false;
//   }
//
//   // This declaration reference is dependent, stop for now.
//   if (NameSpec && NameSpec->isAnyNameDependent()) {
//      makeNestedNameSpec(Sema, Sema.getContext(), NameSpec, Ident);
//      return true;
//   }
//
//   // Fail if this identifier used an arrow.
//   if (Ident->isPointerAccess()) {
//      Sema.diagnose(Ident, err_generic_error,
//                    "does not refer to a declaration",
//                    Ident->getSourceRange());
//
//      HadError = true;
//      return false;
//   }
//
//   // Check the parent expression.
//   QualType ParentType;
//   auto *PE = Ident->getParentExpr();
//   if (PE && !Sema.refersToNamespace(PE)) {
//      auto ParentRes = Sema.visitExpr(Ident, PE);
//      if (!ParentRes)
//         return ExprError();
//
//      PE = ParentRes.get();
//      Ident->setParentExpr(PE);
//
//      // If the parent expression refers to a template, stop.
//      if (isa<TemplateArgListExpr>(PE)) {
//         return true;
//      }
//
//      ParentType = PE->getExprType()->removeReference();
//      if (ParentType->isUnknownAnyType()) {
//         makeNestedNameSpec(Sema, Sema.getContext(), NameSpec, Ident);
//         return true;
//      }
//
//      // If it's not a meta type, this identifier cannot reference a
//      // declaration.
//      if (!ParentType->isMetaType()) {
//         Sema.diagnose(Ident, err_generic_error,
//                       "does not refer to a declaration",
//                       Ident->getSourceRange());
//
//         HadError = true;
//         return false;
//      }
//
//      // If it's a meta type, it can either:
//      //   - refer to a DeclContext if it's a record type
//      //   - refer to a static type member otherwise
//      auto Underlying = ParentType->asMetaType()->getUnderlyingType();
//      if (!Underlying->isRecordType()) {
//         Sema.diagnose(Ident, err_generic_error,
//                       "does not refer to a declaration",
//                       Ident->getSourceRange());
//
//         HadError = true;
//         return false;
//      }
//
//      ParentType = Underlying;
//      Ident->setStaticLookup(true);
//
//      auto *R = ParentType->removeReference()->getRecord();
//      Ident->setDeclCtx(R);
//   }
//   else if (Ident->hasLeadingDot()) {
//      auto CtxTy = Ident->getContextualType();
//      if (!CtxTy || !CtxTy->isRecordType()) {
//         Sema.diagnose(Ident, err_requires_contextual_type, 2,
//                      Ident->getSourceRange());
//
//         HadError = true;
//         return false;
//      }
//
//      auto *R = CtxTy->getRecord();
//      Ident->setDeclCtx(R);
//   }
//
//   if (!Ident->getDeclCtx())
//      Ident->setDeclCtx(&Sema.getDeclContext());
//
//   // Now perform the lookup.
//   auto LookupResult = Sema.MultiLevelLookup(*Ident->getDeclCtx(),
//                                             Ident->getDeclName(), true,
//                                             false);
//
//   if (!LookupResult) {
//      if (CheckBuiltinType(Sema, Ident)) {
//         return true;
//      }
//
//      // If this expression is type dependent, new declarations might be
//      // visible at instantiation time, so don't report an error for now.
//      if (Ident->isTypeDependent() || Sema.inUnboundedTemplate()) {
//         Ident->setIsTypeDependent(true);
//         return false;
//      }
//
//      auto DeclName = Ident->getDeclName();
//      if (Ident->isSynthesized()) {
//         unsigned DiagIdx;
//         if (DeclName.isStr("Array")) {
//            DiagIdx = 5;
//         }
//         else if (DeclName.isStr("Dictionary")) {
//            DiagIdx = 6;
//         }
//         else if (DeclName.isStr("Option")) {
//            DiagIdx = 7;
//         }
//         else {
//            llvm_unreachable("bad synthesized identifier!");
//         }
//
//         Sema.diagnose(Ident, err_no_builtin_decl, Ident->getSourceRange(),
//                       DiagIdx);
//
//         return ExprError();
//      }
//
//      MessageKind diagId;
//      bool WithContext = false;
//
//      // provide appropriate diagnostic for type lookup, member lookup and
//      // normal identifier lookup
//      if (Ident->isInTypePosition()) {
//         diagId = diag::err_type_not_found;
//      }
//      else if (Ident->getParentExpr()) {
//         diagId = diag::err_member_not_found;
//         WithContext = true;
//      }
//      else {
//         diagId = diag::err_undeclared_identifer;
//      }
//
//      Sema.diagnoseMemberNotFound(WithContext ? Ident->getDeclCtx() : nullptr,
//                                  Ident, DeclName, diagId);
//
//      HadError = true;
//      return false;
//   }
//
//   if (ReferencedDecls) {
//      for (auto *D : LookupResult.allDecls()) {
//         ReferencedDecls->insert(D);
//      }
//   }
//
//   return true;
//}
//
// bool NameBinder::visitCallExpr(CallExpr *E)
//{
//   return true;
//}
