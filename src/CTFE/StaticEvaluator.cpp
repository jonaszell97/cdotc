//
// Created by Jonas Zell on 07.02.18.
//

#include "StaticEvaluator.h"

#include "AST/Passes/AbstractPass.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"

#include "AST/Passes/ILGen/ILGenPass.h"

#include "CTFE/CTFEEngine.h"
#include "CTFE/Value.h"

#include "Support/Format.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace {

struct ExprResult {
   ExprResult() : Valid(false) {}
   ExprResult(Variant &&V) : Valid(true), V(move(V)) {}

   bool isValid() const
   {
      return Valid;
   }

   Variant &getValue()
   {
      return V;
   }

   operator bool() const
   {
      return isValid();
   }

private:
   bool Valid;
   Variant V;
};

class EvaluatorImpl: public AbstractPass<EvaluatorImpl, ExprResult>,
                     public DiagnosticIssuer {
public:
   explicit EvaluatorImpl(SemaPass &SP)
      : SP(SP), Engine(SP)
   {}

   using StaticExprResult = StaticEvaluator::StaticExprResult;

   StaticExprResult evaluate(Expression *Expr)
   {
      SemaPass::DiagnosticScopeRAII diagnosticScopeRAII(SP);

      auto semaRes = SP.visitExpr(Expr, Expr);
      if (semaRes.isTypeDependent() || semaRes.isValueDependent())
         return StaticExprResult(semaRes.isTypeDependent(),
                                 semaRes.isValueDependent());

      if (semaRes.hadError())
         return StaticExprResult(diagnosticScopeRAII.getAddedDiagnostics());

      auto res = visit(Expr);
      if (!res) {
         if (typeDependent || valueDependent)
            return StaticExprResult(typeDependent, valueDependent);

         return StaticExprResult(diagnostics);
      }

      return StaticExprResult(move(res.getValue()));
   }

   friend class AbstractPass<EvaluatorImpl, ExprResult>;

private:
   SemaPass &SP;
   ctfe::CTFEEngine Engine;

   bool encounteredError = false;
   bool typeDependent    = false;
   bool valueDependent   = false;

   DiagnosticBuilder &err(MessageKind E)
   {
      encounteredError = true;
      return DiagnosticIssuer::err(E);
   }

#  define CDOT_EXPR(Name)              \
   ExprResult visit##Name(Name *Expr);
#  include "AST/AstNode.def"

   ExprResult visitMemberRefExpr(MemberRefExpr* expr, Variant &&V);
   ExprResult visitCallExpr(CallExpr* expr, Variant &&V);
   ExprResult visitSubscriptExpr(SubscriptExpr* expr, Variant &&V);

   ExprResult visit(Expression *Expr)
   {
      auto res = AbstractPass::visit(Expr);
      if (Expr->hadError()) {
         encounteredError = true;
         return ExprResult();
      }
      if (Expr->isDependent()) {
         typeDependent = Expr->isTypeDependent();
         valueDependent = Expr->isValueDependent();

         return ExprResult();
      }

      return res;
   }

   Variant makeStruct(std::vector<Variant> &&vals)
   {
      return Variant(VariantType::Struct, move(vals));
   }

   Variant makeArray(std::vector<Variant> &&vals)
   {
      return Variant(VariantType::Array, move(vals));
   }

   ExprResult makeEnum(EnumDecl *E,
                       llvm::StringRef Case,
                       llvm::ArrayRef<Expression*> associatedValues) {
      auto C = E->lookupSingle<EnumCaseDecl>(Case);
      assert(C && "Case not found!");

      Variant caseVal(uint64_t(C->getRawValue()));
      std::vector<Variant> vals{ move(caseVal) };

      for (auto &expr : associatedValues) {
         auto res = visit(expr);
         if (!res)
            return res;

         vals.push_back(res.getValue());
      }

      return makeStruct(move(vals));
   }

   ExprResult call(CallableDecl *C, llvm::ArrayRef<Expression*> args)
   {
      llvm::SmallVector<ctfe::Value, 8> Vals;

      for (auto &arg : args) {
         auto res = visit(arg);
         if (!res)
            return res;

         Vals.push_back(Engine.CTFEValueFromVariant(res.getValue(),
                                                    arg->getExprType()));
      }

      auto F = SP.getILGen().getFunc(C);
      auto ctfeRes = Engine.evaluateFunction(F, Vals);
      if (!ctfeRes)
         return ExprResult();

      return move(ctfeRes.getVal());
   }

   Variant getDefaultValue(QualType Ty)
   {
      switch (Ty->getTypeID()) {
         case TypeID::IntegerTypeID:
            return Variant(uint64_t(0));
         case TypeID::FPTypeID:
            if (Ty->isDoubleTy())
               return Variant(0.0);
            else
               return Variant(0.0f);
         case TypeID::ArrayTypeID: {
            std::vector<Variant> vec;
            ArrayType *ArrTy = Ty->asArrayType();

            size_t numElements = ArrTy->getNumElements();
            for (size_t i = 0; i < numElements; ++i) {
               vec.push_back(getDefaultValue(ArrTy->getElementType()));
            }

            return makeArray(move(vec));
         }
         case TypeID::TupleTypeID: {
            std::vector<Variant> vec;

            TupleType *tup = Ty->asTupleType();
            for (auto &el : tup->getContainedTypes()) {
               vec.push_back(getDefaultValue(el));
            }

            return makeStruct(move(vec));
         }
         case TypeID::ObjectTypeID: {
         }
         default:
            llvm_unreachable("");
      }
   }
};

} // anonymous namespace

ExprResult EvaluatorImpl::visitTemplateArgExpr(TemplateArgExpr *expr)
{
   llvm_unreachable("should not be visited by Evaluator");
}

ExprResult EvaluatorImpl::visitTypeRef(TypeRef* expr)
{
   return Variant(expr->getType());
}

ExprResult EvaluatorImpl::visitIntegerLiteral(IntegerLiteral* expr)
{
   if (!expr->getType()->isIntegerType()) {
      return makeStruct({ Variant(llvm::APSInt(expr->getValue())) });
   }

   return Variant(llvm::APSInt(expr->getValue()));
}

ExprResult EvaluatorImpl::visitFPLiteral(FPLiteral* expr)
{
   if (!expr->getType()->isFPType()) {
      return makeStruct({ Variant(llvm::APFloat(expr->getValue())) });
   }

   return Variant(llvm::APFloat(expr->getValue()));
}

ExprResult EvaluatorImpl::visitBoolLiteral(BoolLiteral* expr)
{
   if (!expr->getType()->isIntegerType()) {
      return makeStruct({ Variant(llvm::APInt(1, uint64_t(expr->getValue()),
                                              false)) });
   }

   return Variant(llvm::APInt(1, uint64_t(expr->getValue()), false));
}

ExprResult EvaluatorImpl::visitCharLiteral(CharLiteral* expr)
{
   if (!expr->getType()->isIntegerType()) {
      return makeStruct({ Variant(llvm::APInt(8, uint64_t(expr->getNarrow()),
                                              false)) });
   }

   return Variant(llvm::APInt(8, uint64_t(expr->getNarrow()), false));
}

ExprResult EvaluatorImpl::visitNoneLiteral(NoneLiteral* expr)
{
   return makeEnum(cast<EnumDecl>(expr->getExprType()->getRecord()),
                   "None", {});
}

ExprResult EvaluatorImpl::visitStringLiteral(StringLiteral* expr)
{
   Variant Str(expr->getValue());
   if (expr->isCString())
      return ExprResult(move(Str));

   // Size (UInt)
   Variant Size = makeStruct({ Variant(uint64_t(expr->getValue().length())) });

   // Capacity (UInt)
   Variant Cap = Size;

   return makeStruct({ move(Size), move(Cap), move(Str) });
}

ExprResult EvaluatorImpl::visitStringInterpolation(StringInterpolation* expr)
{
   llvm::SmallString<128> str;
   for (auto &s : expr->getStrings()) {
      auto res = visit(s);
      if (!res)
         return res;

      auto &V = res.getValue();
      if (V.isStr()) {
         str += V.getString();
      }
      else {
         // 0 = Size, 1 = Capacity, 2 = CString
         str += V[2].getString();
      }
   }

   return Variant(str.str());
}

ExprResult EvaluatorImpl::visitLambdaExpr(LambdaExpr* expr)
{
   llvm_unreachable("not implemented!");
}

ExprResult EvaluatorImpl::visitTupleLiteral(TupleLiteral* expr)
{
   std::vector<Variant> vals;
   for (auto &el : expr->getElements()) {
      auto res = visit(el.second);
      if (!res)
         return res;

      vals.push_back(res.getValue());
   }

   return makeStruct(move(vals));
}

ExprResult EvaluatorImpl::visitArrayLiteral(ArrayLiteral* expr)
{
   std::vector<Variant> vals;
   for (auto &el : expr->getValues()) {
      auto res = visit(el);
      if (!res)
         return res;

      vals.push_back(res.getValue());
   }

   return makeStruct(move(vals));
}

ExprResult EvaluatorImpl::visitDictionaryLiteral(DictionaryLiteral* expr)
{
   llvm_unreachable("not implemented!");
}

ExprResult EvaluatorImpl::visitIdentifierRefExpr(IdentifierRefExpr* expr)
{
   using IK = IdentifierRefExpr::IdentifierKind;

   switch (expr->getKind()) {
      case IK::Unknown:
      default:
         llvm_unreachable("bad identifier kind");
      case IK::MetaType:
      case IK::Namespace:
         return Variant();
      case IK::LocalVar: {
         auto L = expr->getLocalVar();

         if (!L->isConst()) {
            err(err_generic_error)
               << "cannot reference non-const local variable in static "
                  "expression"
               << expr->getSourceLoc() << diag::end;

            return ExprResult();
         }

         if (auto Init = L->getValue()) {
            EvaluatorImpl evaluator(SP);
            return evaluator.visit(Init);
         }
         else {
            // value must be uninitialized
            return getDefaultValue(L->getTypeRef()->getType());
         }
      }
      case IK::GlobalVar: {
         auto G = expr->getGlobalVar();

         if (!G->isConst()) {
            err(err_generic_error)
               << "cannot reference non-const global variable in static "
                  "expression"
               << expr->getSourceLoc() << diag::end;

            return ExprResult();
         }

         if (auto Init = G->getValue()) {
            EvaluatorImpl evaluator(SP);
            return evaluator.visit(Init);
         }
         else {
            // value must be uninitialized
            return getDefaultValue(G->getTypeRef()->getType());
         }
      }
      case IK::FunctionArg:
         err(err_generic_error)
            << "cannot reference function argument in static expression"
            << expr->getSourceLoc() << diag::end;

         return ExprResult();
      case IK::Alias:
         return ExprResult(Variant(expr->getAliasVal()));
      case IK::Self:
         err(err_generic_error)
            << "cannot reference 'self' in static expression"
            << expr->getSourceLoc() << diag::end;

         return ExprResult();
      case IK::Function:
         llvm_unreachable("TODO!");
      case IK::BuiltinValue: {
         switch (expr->getBuiltinKind()) {
            case BuiltinIdentifier::NULLPTR:
               return Variant(uint64_t(0));
            case BuiltinIdentifier::DOUBLE_SNAN:
               return Variant(std::numeric_limits<double>::signaling_NaN());
            case BuiltinIdentifier::DOUBLE_QNAN:
               return Variant(std::numeric_limits<double>::quiet_NaN());
            case BuiltinIdentifier::FLOAT_SNAN:
               return Variant(std::numeric_limits<float>::signaling_NaN());
            case BuiltinIdentifier::FLOAT_QNAN:
               return Variant(std::numeric_limits<float>::quiet_NaN());
            case BuiltinIdentifier::__ctfe:
               return Variant(uint64_t(true));
            default:
               llvm_unreachable("Unsupported builtin identifier");
         }
      }
   }
}

ExprResult EvaluatorImpl::visitMemberRefExpr(MemberRefExpr* expr)
{
   return visitMemberRefExpr(expr, Variant());
}

ExprResult EvaluatorImpl::visitMemberRefExpr(MemberRefExpr *expr,
                                             Variant &&V) {
   switch (expr->getKind()) {
      default:
         llvm_unreachable("bad member kind!");
      case MemberKind::Alias:
         return Variant(expr->getAliasVal());
      case MemberKind::Accessor: {
//         auto M = getFunc(node->getAccessorMethod());
//         auto target = getRValue(pop());
//         if (node->isPointerAccess())
//            target = Builder.CreateLoad(target);
//
//
//         V = Builder.CreateCall(M, { target });
//
//         break;
         llvm_unreachable("TODO!");
      }
      case MemberKind::Function: {
//         auto Fun = getFunc(node->getCallable());
//         if (!node->getExprType()->isRawFunctionTy()) {
//            V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun),
//                                         SP.getObjectTy("cdot.Lambda"), { });
//         }
//         else {
//            V = Fun;
//         }
//
//         break;
         llvm_unreachable("TODO!");
      }
      case MemberKind::EnumRawValue:
         return move(V[0]);
      case MemberKind::TupleAccess:
         return move(V[expr->getTupleIndex()]);
      case MemberKind::UnionAccess:
         llvm_unreachable("TODO");
      case MemberKind::Namespace:
         return Variant();
      case MemberKind::GlobalVariable: {
         auto G = expr->getGlobalVar();
         if (auto Init = G->getValue()) {
            EvaluatorImpl evaluator(SP);
            return evaluator.visit(Init);
         }
         else {
            // value must be uninitialized
            return getDefaultValue(G->getTypeRef()->getType());
         }
      }
      case MemberKind::StaticField: {
         auto G = expr->getStaticFieldDecl();
         if (auto Init = G->getDefaultVal()) {
            EvaluatorImpl evaluator(SP);
            return evaluator.visit(Init);
         }
         else {
            // value must be uninitialized
            return getDefaultValue(G->getType()->getType());
         }
      }
      case MemberKind::Type:
         llvm_unreachable("TODO");
      case MemberKind::Field:
         llvm_unreachable("TODO");
   }
}

ExprResult EvaluatorImpl::visitCallExpr(CallExpr* expr)
{
   return visitCallExpr(expr, Variant());
}

ExprResult EvaluatorImpl::visitCallExpr(CallExpr *expr, Variant &&V)
{
   llvm::SmallVector<Expression*, 8> args(expr->getArgs().begin(),
                                          expr->getArgs().end());

   switch (expr->getKind()) {
      case CallKind::Unknown:
      default:
         llvm_unreachable("bad call kind!");
      case CallKind::PrimitiveInitializer:
         if (expr->getReturnType()->isVoidType())
            return Variant();
         else
            return visit(expr->getArgs().front());
      case CallKind::UnsafeTupleGet: {
         auto idx = visit(expr->getArgs().front());
         if (!idx)
            return idx;

         return move(V[idx.getValue().getZExtValue()]);
      }
      case CallKind::CallOperator: {
         auto identExpr = expr->getIdentExpr();

         if (identExpr) {
            args.insert(args.begin(), identExpr);
         }

         return call(expr->getMethod(), args);
      }
      case CallKind::AnonymousCall: {
//         auto funcTy = node->getFunctionType();
//         auto identExpr = node->getIdentExpr();
//         auto func = identExpr ? visitIdentifierRefExpr(identExpr)
//                               : getRValue(Self);
//
//         if (funcTy->isRawFunctionTy()) {
//            V = Builder.CreateIndirectCall(getRValue(func), args);
//         }
//         else {
//            V = Builder.CreateLambdaCall(getRValue(func), args);
//         }

         llvm_unreachable("TODO");

         break;
      }
      case CallKind::NamedFunctionCall:
         return call(expr->getFunc(), args);
      case CallKind::StaticMethodCall:
         return call(expr->getMethod(), args);
      case CallKind::InitializerCall: {
//         auto method = node->getMethod();
//         auto AggrTy = getType(method->getRecord());
//
//         auto Init = getFunc(method);
//         assert(isa<il::Method>(Init));
//
//         V = Builder.CreateInit(cast<il::StructType>(AggrTy),
//                                cast<il::Method>(Init), args);
//
//         temporaries.insert(V);
//
//         break;
         llvm_unreachable("TODO");
      }
      case CallKind::UnionInitializer: {
//         auto AggrTy = getType(node->getUnion());
//         assert(args.size() == 1);
//
//         V = Builder.CreateUnionInit(cast<il::UnionType>(AggrTy),
//                                     args.front());
//
//         temporaries.insert(V);
//
//         break;
         llvm_unreachable("TODO");
      }
      case CallKind::MethodCall: {
//         auto target = Self;
//         if (!target)
//            target = getCurrentFn()->getEntryBlock()->getBlockArg(0);
//
//         if (node->isPointerAccess())
//            target = Builder.CreateLoad(target);
//
//         if (target->isLvalue() && !node->getMethod()->hasMutableSelf())
//            target = Builder.CreateLoad(target);
//
//         args.insert(args.begin(), target);
//
//         V = CreateCall(node->getMethod(), args);
//         break;
         llvm_unreachable("TODO");
      }
   }
}

ExprResult EvaluatorImpl::visitEnumCaseExpr(EnumCaseExpr* expr)
{
   return makeEnum(expr->getEnum(), expr->getIdent(), expr->getArgs());
}

ExprResult EvaluatorImpl::visitSubscriptExpr(SubscriptExpr *expr,
                                             Variant &&V) {
   assert(expr->hasSingleIndex() && "should have been replaced by a call!");

   auto idx = visit(expr->getIndices().front());
   if (!idx)
      return idx;

   return move(V[idx.getValue().getZExtValue()]);
}

ExprResult EvaluatorImpl::visitSubscriptExpr(SubscriptExpr* expr)
{
   llvm_unreachable("cannot appear without a parent expression!");
}

ExprResult EvaluatorImpl::visitBuiltinExpr(BuiltinExpr* expr)
{
   llvm_unreachable("not implemented!");
}

ExprResult EvaluatorImpl::visitExpressionPattern(ExpressionPattern* expr)
{
   llvm_unreachable("not implemented!");
}

ExprResult EvaluatorImpl::visitCasePattern(CasePattern* expr)
{
   llvm_unreachable("not implemented!");
}

ExprResult EvaluatorImpl::visitIsPattern(IsPattern* expr)
{
   llvm_unreachable("not implemented!");
}

#define UNARY_INT_OP(Op)                             \
   if (op == #Op)                                    \
      return Variant(Op V.getAPSInt())

ExprResult EvaluatorImpl::visitUnaryOperator(UnaryOperator* expr)
{
   auto res = visit(expr->getTarget());
   if (!res)
      return res;

   auto &V = res.getValue();
   auto &op = expr->getOp();

   if (expr->getTarget()->getExprType()->isIntegerType()) {
      UNARY_INT_OP(-);
      UNARY_INT_OP(~);
      UNARY_INT_OP(!);
   }
   else {
      if (op == "-") {
         llvm::APFloat F(V.getAPFloat());
         F.changeSign();

         return Variant(std::move(F));
      }
   }

   if (op == "*") {
      llvm_unreachable("not yet");
   }
   else if (op == "&") {
      llvm_unreachable("not yet");
   }
   else if (op == "++" || op == "--") {
      llvm_unreachable("not yet");
   }
   else if (op == "typeof") {
      llvm_unreachable("not yet");
   }

   llvm_unreachable("bad unary op");
}

#undef UNARY_INT_OP

#define BINARY_INT_OP(Op)                                   \
   if (op == #Op)                                           \
      return Variant(lhs.getAPSInt() Op rhs.getAPSInt());

#define BINARY_FP_OP(Op)                                    \
   if (op == #Op)                                           \
      return Variant(lhs.getAPFloat() Op rhs.getAPFloat());

ExprResult EvaluatorImpl::visitBinaryOperator(BinaryOperator* expr)
{
   if (auto Acc = expr->getAccessorMethod()) {
      llvm_unreachable("xx");
   }

   if (expr->isTypePredicate()) {
      auto val = expr->getTypePredicateResult();
      if (expr->getExprType()->isIntegerType())
         return Variant(val);

      return makeStruct({ Variant(val) });
   }

   auto &op = expr->getOp();

   auto lhsRes = visit(expr->getLhs());
   if (!lhsRes)
      return lhsRes;

   auto &lhs = lhsRes.getValue();
   bool isAssignment = op == "=";

   if (op == "as" || op == "as?" || op == "as!") {
      llvm_unreachable("not yet");
   }

//   if (!isAssignment) {
//      if (lhs->getType()->isRawEnum())
//         lhs = Builder.CreateEnumRawValue(lhs);
//      else
//         lhs = unboxIfNecessary(lhs);
//   }

   if (isAssignment) {
//      auto rhs = getRValue(visit(node->getRhs()));
//
//      retainIfNecessary(rhs);
//      releaseIfNecessary(getRValue(lhs));
//
//      Res = CreateStore(rhs, lhs);
      llvm_unreachable("not yet");
   }
   else if (op == "&&") {
      if (!lhsRes.getValue().getZExtValue()) {
         return Variant(uint64_t(false));
      }

      auto rhsRes = visit(expr->getRhs());
      if (!rhsRes)
         return rhsRes;

      return Variant(uint64_t(rhsRes.getValue().getZExtValue() != 0));
   }
   else if (op == "||") {
      if (lhsRes.getValue().getZExtValue()) {
         return Variant(uint64_t(true));
      }

      auto rhsRes = visit(expr->getRhs());
      if (!rhsRes)
         return rhsRes;

      return Variant(uint64_t(rhsRes.getValue().getZExtValue() != 0));
   }
   else {
      auto rhsRes = visit(expr->getRhs());
      if (!rhsRes)
         return rhsRes;

      auto &rhs = rhsRes.getValue();
      auto lhsTy = expr->getLhs()->getExprType();
      auto rhsTy = expr->getRhs()->getExprType();
      (void)rhsTy;

      QualType castResultTo;
      if (op == "**") {
//         if (lhsTy->isIntegerType() && rhsTy->isIntegerType()) {
//            lhs = castTo(lhs, SP.getContext().getIntTy());
//            rhs = castTo(rhs, SP.getContext().getIntTy());
//
//            castResultTo = lhs->getType();
//         }
//         else {
//            if (rhs->getType()->isIntegerType())
//               rhs = castTo(rhs, SP.getContext().getInt32Ty());
//            else
//               rhs = castTo(rhs, SP.getContext().getDoubleTy());
//         }
      }
      else {
//         castResultTo = node->getExprType();
      }

      if (lhsTy->isIntegerType()) {
         BINARY_INT_OP(+);
         BINARY_INT_OP(-);
         BINARY_INT_OP(*);
         BINARY_INT_OP(/);
         BINARY_INT_OP(%);

         BINARY_INT_OP(&);
         BINARY_INT_OP(|);
         BINARY_INT_OP(^);

         BINARY_INT_OP(==);
         BINARY_INT_OP(!=);
         BINARY_INT_OP(<=);
         BINARY_INT_OP(<);
         BINARY_INT_OP(>=);
         BINARY_INT_OP(>);

         if (op == "**") {
            auto lhsVal = lhs.getAPSInt().getZExtValue();
            auto rhsVal = rhs.getAPSInt().getZExtValue();

            return Variant(uint64_t(support::intPower(lhsVal, rhsVal)));
         }

         if (op == "<<") {
            auto lhsVal = lhs.getAPSInt().getZExtValue();
            auto rhsVal = rhs.getAPSInt().getZExtValue();

            return Variant(uint64_t(lhsVal << rhsVal));
         }

         if (op == ">>") {
            auto lhsVal = lhs.getAPSInt().getZExtValue();
            auto rhsVal = rhs.getAPSInt().getZExtValue();

            return Variant(uint64_t(lhsVal << rhsVal));
         }

         if (op == ">>>") {
            auto lhsVal = lhs.getAPSInt().getSExtValue();
            auto rhsVal = rhs.getAPSInt().getSExtValue();

            return Variant(uint64_t(lhsVal << rhsVal));
         }

         if (op == "===")
            llvm_unreachable("not yet");

         if (op == "!==")
            llvm_unreachable("not yet");
      }
      else {
         BINARY_FP_OP(+);
         BINARY_FP_OP(-);
         BINARY_FP_OP(*);
         BINARY_FP_OP(/);

         auto compRes = lhs.getAPFloat().compare(rhs.getAPFloat());
         switch (compRes) {
            case llvm::APFloat::cmpEqual:
               return Variant(op == "==" || op == "<=" || op == ">=");
            case llvm::APFloat::cmpLessThan:
               return Variant(op == "<" || op == "<=" || op == "!=");
            case llvm::APFloat::cmpGreaterThan:
               return Variant(op == "<" || op == "<=" || op == "!=");
            case llvm::APFloat::cmpUnordered:
               return Variant(false);
         }
      }
   }

   llvm_unreachable("bad binary op");
}

#undef BINARY_INT_OP
#undef BINARY_FP_OP

ExprResult EvaluatorImpl::visitTertiaryOperator(TertiaryOperator* expr)
{
   auto condRes = visit(expr->getCondition());
   if (!condRes)
      return condRes;

   if (condRes.getValue().getZExtValue()) {
      return visit(expr->getLhs());
   }
   else {
      return visit(expr->getRhs());
   }
}

ExprResult EvaluatorImpl::visitExprSequence(ExprSequence* expr)
{
   return visit(expr->getResolvedExpression());
}

ExprResult EvaluatorImpl::visitImplicitCastExpr(ImplicitCastExpr* expr)
{
   llvm_unreachable("not implemented!");
}

ExprResult EvaluatorImpl::visitLvalueToRvalue(LvalueToRvalue* expr)
{
   llvm_unreachable("not implemented!");
}

ExprResult EvaluatorImpl::visitStaticExpr(StaticExpr* expr)
{
   return visit(expr->getExpr());
}

ExprResult EvaluatorImpl::visitConstraintExpr(ConstraintExpr* expr)
{
   llvm_unreachable("should never be visited!");
}

ExprResult EvaluatorImpl::visitTraitsExpr(TraitsExpr* expr)
{
   return visit(expr->getResultExpr());
}

StaticEvaluator::StaticEvaluator(SemaPass &SP)
   : pImpl(new EvaluatorImpl(SP))
{

}

StaticEvaluator::~StaticEvaluator()
{
   delete reinterpret_cast<EvaluatorImpl*>(pImpl);
}

StaticEvaluator::StaticExprResult StaticEvaluator::evaluate(Expression *Expr)
{
   return reinterpret_cast<EvaluatorImpl*>(pImpl)->evaluate(Expr);
}

} // namespace cdot