//
// Created by Jonas Zell on 08.12.17.
//

#include <sstream>
#include "StaticExprEvaluator.h"

#include "../ILGen/ILGenPass.h"

#include "../SemanticAnalysis/Record/Record.h"
#include "../SemanticAnalysis/Function.h"
#include "../SemanticAnalysis/SemaPass.h"
#include "../SemanticAnalysis/Template.h"

#include "../../Statement/Declaration/CallableDecl.h"

#include "../../Expression/StaticExpr.h"
#include "../../Expression/PatternExpr.h"

#include "../../Expression/Literal/IntegerLiteral.h"
#include "../../Expression/Literal/StringLiteral.h"
#include "../../Expression/Literal/StringInterpolation.h"
#include "../../Expression/Literal/TupleLiteral.h"
#include "../../Expression/Literal/LambdaExpr.h"
#include "../../Expression/Literal/NoneLiteral.h"
#include "../../Expression/Literal/DictionaryLiteral.h"

#include "../../Operator/Conversion/ImplicitCastExpr.h"
#include "../../Operator/Conversion/LvalueToRvalue.h"

#include "../../Operator/UnaryOperator.h"
#include "../../Operator/BinaryOperator.h"
#include "../../Operator/TertiaryOperator.h"
#include "../../Operator/ExprSequence.h"

#include "../../Expression/RefExpr/IdentifierRefExpr.h"
#include "../../Expression/RefExpr/CallExpr.h"
#include "../../Expression/RefExpr/MemberRefExpr.h"
#include "../../Expression/RefExpr/SubscriptExpr.h"

#include "../../Expression/TypeRef.h"

#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/MetaType.h"

#include "../../../Message/Diagnostics.h"

#include "../Declaration/DeclPass.h"
#include "../SemanticAnalysis/TemplateInstantiator.h"
#include "../SemanticAnalysis/Builtin.h"

#include "../../../CTFE/CTFEEngine.h"
#include "../../../Basic/DependencyGraph.h"

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::cl;

namespace cdot {
namespace ast {

class EvaluatorImpl: public DiagnosticIssuer {
public:
   explicit EvaluatorImpl(SemaPass &SP, Record *Ctx, Callable *FuncCtx,
                          llvm::ArrayRef<size_t> importedNamespaces,
                          sema::TemplateArgList const* templateArgs)
      : SP(SP), RecordCtx(Ctx), FuncCtx(FuncCtx),
        importedNamespaces(importedNamespaces),
        templateArgs(templateArgs), typeDependant(false), had_error(false),
        prevDiagnosticSize(SP.getDiagnostics().size())
   {}

   StaticExprResult visit(StaticExpr *expr, SourceLocation requestedFrom)
   {
      auto SPres = SP.visitExpr(expr, expr->getExpr());
      if (SPres.hadError()) {
         return StaticExprResult(expr->isTypeDependant(), true, {});
      }
      if (expr->isValueDependant()) {
         return StaticExprResult(true, false, {});
      }

      auto res = visitStaticExpr(expr);
      if (hadError()) {
         if (requestedFrom)
            diagnostics.push_back(diag::note(note_generic_note)
                                     << "ctfe requested here"
                                     << requestedFrom);

         return StaticExprResult(expr->isTypeDependant(), true,
                                 std::move(diagnostics));
      }

      return StaticExprResult(std::move(res));

//      auto res = SP.ILGen->evaluateStaticExpr(expr);
//      if (!res.Dependencies.empty()) {
//         handleDependencies(expr, res);
//         return visit(expr, requestedFrom);
//      }
//
//      if (res.hadError) {
//         if (requestedFrom)
//            res.diagnostics.push_back(diag::note(note_generic_note)
//                                      << "ctfe requested here"
//                                      << requestedFrom);
//
//         return StaticExprResult(expr->isTypeDependant(), true,
//                                 std::move(res.diagnostics));
//      }
//
//      return StaticExprResult(std::move(res.val));
   }

   struct ExprResult {
      explicit ExprResult(Variant &&V)
         : error(false), typeDependant(false), valueDependant(false),
           ctfeDependant(false), result(std::move(V))
      {}

      ExprResult(bool error, bool typeDependant = false,
                 bool valueDependant = false, bool ctfeDependant = false)
         : error(error), typeDependant(typeDependant),
           valueDependant(valueDependant), ctfeDependant(ctfeDependant)
      {}

      bool stopEvaluation() const
      {
         return (error | typeDependant | valueDependant | ctfeDependant) != 0;
      }

      Variant &getResult()
      {
         assert(!stopEvaluation());
         return result;
      }

   private:
      bool error           : 1;
      bool typeDependant   : 1;
      bool valueDependant  : 1;
      bool ctfeDependant   : 1;

      Variant result;
   };

   ExprResult visitExpr(Statement *dependantStmt, Expression *expr)
   {
      auto res = visit(expr);

      if (expr->hadError()) {
         had_error = true;
         dependantStmt->setHadError(true);

         return ExprResult(true);
      }
      if (expr->isTypeDependant()) {
         typeDependant = true;
         dependantStmt->setIsTypeDependent(true);

         return ExprResult(false, true);
      }
      if (expr->isValueDependant()) {
         dependantStmt->setIsValueDependent(true);

         return ExprResult(false, false, true);
      }

      if (auto subExpr = expr->getMemberExpr())
         res = VisitSubExpr(expr, std::move(res));

      return ExprResult(std::move(res));
   }

   void handleDependencies(StaticExpr *expr,
                           ctfe::CTFEResult &result) {
//      DependencyGraph<Statement> DG;
//      auto &deps = result.Dependencies;
//      auto &vert = DG.getOrAddVertex(expr);
//
//      for (auto &F : deps) {
//         auto decl = F->getDeclaration();
//         if (!decl)
//            continue;
//
//         auto &dep = DG.getOrAddVertex(decl);
//         dep.addOutgoing(&vert);
//      }
//
//      auto res = DG.constructOrderedList();
//      assert(res.second);

      for (auto F : result.Dependencies) {
         SP.visit(F->getDeclaration());
         SP.ILGen->visit(F->getDeclaration());
      }
   }

   void reset()
   {
      typeDependant = false;
      diagnostics.clear();
   }

   bool isTypeDependant() const
   {
      return typeDependant;
   }

   bool hadError() const
   {
      return had_error;
   }

   llvm::SmallVector<DiagnosticBuilder, 4> &getDiagnostics()
   {
      diagnostics.insert(diagnostics.end(),
                         std::make_move_iterator(SP.getDiagnostics().begin()
                                                 + prevDiagnosticSize),
                         std::make_move_iterator(SP.getDiagnostics().end()));

      SP.getDiagnostics().resize(prevDiagnosticSize);
      return diagnostics;
   }

private:
   SemaPass &SP;
   Record *RecordCtx;
   Callable *FuncCtx;
   llvm::ArrayRef<size_t> importedNamespaces;
   sema::TemplateArgList const* templateArgs;

   bool typeDependant : 1;
   bool had_error : 1;

   size_t prevDiagnosticSize;

   std::stack<Variant> resultStack;

   DependencyGraph<Statement> DG;

   ResolvedTemplateArg const* getTemplateArg(llvm::StringRef name)
   {
      if (templateArgs) {
         if (auto res = templateArgs->getNamedArg(name))
            return res;
      }

      TemplateArgList const* TAs = nullptr;
      if (FuncCtx)
         TAs = &FuncCtx->getTemplateArgs();

      if (RecordCtx)
         TAs = &RecordCtx->getTemplateArgs();

      if (!TAs)
         return nullptr;

      return TAs->getNamedArg(name);
   }

   TemplateParameter const* getTemplateParam(llvm::StringRef name)
   {
      return SP.hasTemplateParam(name);
   }

   Variant HandleBuiltinFn(CallExpr *node);

   Variant visit(std::shared_ptr<Expression> const& expr)
   {
      return visit(expr.get());
   }

   Variant visit(Expression* expr)
   {
      switch (expr->getTypeID()) {
#        define CDOT_EXPR(Name)                             \
         case AstNode::Name##ID:                            \
            return visit##Name(static_cast<Name*>(expr));
#        include "../../AstNode.def"

         default:
            break;
      }

      err(err_generic_error)
         << "unsupported static expression: " + expr->getNodeTypeAsString()
         << expr;

      return {};
   }

   void push(Variant &&V)
   {
      resultStack.push(std::move(V));
   }

   Variant pop()
   {
      auto &&V = std::move(resultStack.top());
      resultStack.pop();

      return V;
   }

   Variant VisitSubExpr(Expression *node, Variant &&V)
   {
      if (auto &SubExpr = node->getMemberExpr()) {
         push(std::move(V));
         V = visit(SubExpr.get());
      }

      return V;
   }

   DiagnosticBuilder &err(MessageKind msg, bool isError = true)
   {
      had_error |= isError;
      return DiagnosticIssuer::err(msg);
   }

#  define CDOT_EXPR(Name)              \
      Variant visit##Name(Name *node);
#  include "../../AstNode.def"
};

Variant EvaluatorImpl::visitStaticExpr(StaticExpr *node)
{
   auto val = visitExpr(node, node->getExpr().get());
   if (val.stopEvaluation())
      return {};

   return val.getResult();
}

namespace {

string stringifyConstraint(Type *Ty, ConstraintExpr *node)
{
   std::ostringstream s;
   s << Ty->toString() << " ";

   static const char *constraintNames[] = {
      "struct", "class", "enum", "union", "function", "default constructible",
      "pointer", "reference"
   };

   switch (node->getKind()) {
      case ConstraintExpr::Type:
         s << "must be " << node->getTypeConstraint()->getTypeRef()
                                ->toString();
         break;
      case ConstraintExpr::Class:
      case ConstraintExpr::Struct:
      case ConstraintExpr::Union:
         s << "must be a " << constraintNames[node->getKind()];
         break;
      case ConstraintExpr::Enum:
         s << "must be an " << constraintNames[node->getKind()];
         break;
      case ConstraintExpr::DefaultConstructible:
         s << "must be " << constraintNames[node->getKind()];
         break;
      case ConstraintExpr::Function:
      case ConstraintExpr::Pointer:
      case ConstraintExpr::Reference:
         s << "must be a " << constraintNames[node->getKind()] << " type";
         break;
      default:
         llvm_unreachable("no.");
   }

   return s.str();
}

} // anonymous namespace

Variant EvaluatorImpl::visitConstraintExpr(ConstraintExpr *node)
{
   auto val = pop();

   if (!val.isMetaType()) {
      err(err_generic_error)
         << "invalid left hand side operand to ':' operator"
         << node;

      return nullptr;
   }

   bool result = false;

   switch (node->getKind()) {
      case ConstraintExpr::Type: {
         auto ty = visitTypeRef(node->getTypeConstraint().get());
         result = val.applyBinaryOp(ty, ":").getInt() != 0;

         break;
      }
      case ConstraintExpr::Class:
      case ConstraintExpr::Struct:
      case ConstraintExpr::Enum:
      case ConstraintExpr::Union: {
         if (!val.getType()->isObjectTy()) {
            result = false;
         }
         else {
            auto rec = val.getType()->getRecord();
            result = rec->getTypeID() == (Record::TypeID)node->getKind();
         }

         break;
      }
      case ConstraintExpr::DefaultConstructible:
         result = val.getType()->hasDefaultValue();
         break;
      case ConstraintExpr::Function:
         result = isa<FunctionType>(val.getType());
         break;
      case ConstraintExpr::Pointer:
         result = isa<PointerType>(val.getType())
                || val.getType()->isRawFunctionTy();
         break;
      case ConstraintExpr::Reference:
         llvm_unreachable("Hmmm....");
   }

   if (!result)
      err(err_generic_error, false)
         << stringifyConstraint(val.getType(), node) << node;

   return Variant(result);
}

Variant EvaluatorImpl::visitTraitsExpr(TraitsExpr *node)
{
   if (auto B = dyn_cast<BoolLiteral>(node->getResultExpr()))
      return Variant(B->getValue());

   if (auto I = dyn_cast<IntegerLiteral>(node->getResultExpr()))
      return I->getValue();

   if (auto FP = dyn_cast<FPLiteral>(node->getResultExpr()))
      return FP->getValue();

   return {};
}

Variant EvaluatorImpl::visitIntegerLiteral(IntegerLiteral *node)
{
   return node->getValue();
}

Variant EvaluatorImpl::visitFPLiteral(FPLiteral *node)
{
   return node->getValue();
}

Variant EvaluatorImpl::visitBoolLiteral(BoolLiteral *node)
{
   return Variant(node->getValue());
}

Variant EvaluatorImpl::visitCharLiteral(CharLiteral *node)
{
   return Variant(node->getNarrow());
}

Variant EvaluatorImpl::visitStringLiteral(StringLiteral *node)
{
   return Variant(string(node->getValue()));
}

Variant EvaluatorImpl::visitStringInterpolation(StringInterpolation *node)
{
   string s;
   for (const auto &value : node->getStrings()) {
      auto val = visit(value.get());
      if (val.isVoid())
         return {};

      s += val.toString();
   }

   return Variant(move(s));
}

Variant EvaluatorImpl::visitNoneLiteral(NoneLiteral *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitNonTypeTemplateArgExpr(NonTypeTemplateArgExpr *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitArrayLiteral(ArrayLiteral *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitDictionaryLiteral(DictionaryLiteral *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitIsPattern(IsPattern *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitCasePattern(CasePattern *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitExpressionPattern(ExpressionPattern *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitIdentifierRefExpr(IdentifierRefExpr *node)
{
   if (auto TA = getTemplateArg(node->getIdent())) {
      if (TA->isVariadic()) {
         std::vector<Variant> vec;
         if (TA->isType())
            for (const auto &el : TA->getVariadicArgs()) {
               vec.emplace_back(el.getType());
            }
         else
            for (const auto &el : TA->getVariadicArgs()) {
               vec.emplace_back(el.getValue());
            }

         return Variant(move(vec));
      }
      else if (TA->isType())
         return Variant(TA->getType());
      else
         return TA->getValue();
   }
   else if (getTemplateParam(node->getIdent())) {
      typeDependant = true;
      return {};
   }

   if (node->getExprType()->isMetaType()) {
      return { cast<MetaType>(*node->getExprType())->getUnderlyingType() };
   }
   else if (node->isNamespace()) {
      return {};
   }

   Variant V;

   if (node->isAlias()) {
      V = node->getAliasVal();
   }
   else if (node->isSelf()) {
      // TODO
   }
   else if (node->isCaptured()) {
      // TODO
   }
   else if (node->isFunctionArg()) {
      // TODO
   }
   else if (node->isFunction()) {
      // TODO
   }
   else if (!node->getBuiltinValue().isVoid()) {
      V = node->getBuiltinValue();
   }
   else if (auto builtinTy = node->getBuiltinType()) {
      switch (node->getBuiltinKind()) {
         case BuiltinIdentifier::NULLPTR:
            // TODO
            break;
         case BuiltinIdentifier::DOUBLE_SNAN:
            V = { std::numeric_limits<double>::signaling_NaN() };
            break;
         case BuiltinIdentifier::DOUBLE_QNAN:
            V = { std::numeric_limits<double>::quiet_NaN() };
            break;
         case BuiltinIdentifier::FLOAT_SNAN:
            V = { std::numeric_limits<float>::signaling_NaN() };
            break;
         case BuiltinIdentifier::FLOAT_QNAN:
            V = { std::numeric_limits<float>::quiet_NaN() };
            break;
         case BuiltinIdentifier::__ctfe:
            V = Variant(true);
            break;
         default:
            llvm_unreachable("Unsupported builtin identifier");
      }
   }
   else if (RecordCtx) {
      if (auto AT = RecordCtx->getAssociatedType(node->getIdent()))
         return Variant(*AT->getType());
   }

   if (V.isVoid())
      err(err_undeclared_identifer)
         << node->getIdent() << node;

   return V;
}

Variant EvaluatorImpl::visitMemberRefExpr(MemberRefExpr *node)
{
   Variant V;
   switch (node->getKind()) {
      default:
         llvm_unreachable("bad member kind!");
      case MemberKind::Alias:
         V = node->getAliasVal();
         break;
      case MemberKind::Accessor:
         break;
      case MemberKind::EnumRawValue:
         break;
      case MemberKind::TupleAccess:
         break;
      case MemberKind::UnionAccess:
         break;
      case MemberKind::Namespace:
         return V;
      case MemberKind::GlobalVariable:
         break;
      case MemberKind::Type:
         break;
      case MemberKind::Field:
         break;
      case MemberKind::AssociatedType:
         V = Variant(cast<MetaType>(*node->getFieldType())
                        ->getUnderlyingType());

         break;
   }

   return V;
}

Variant EvaluatorImpl::visitSubscriptExpr(SubscriptExpr *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitEnumCaseExpr(EnumCaseExpr *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::HandleBuiltinFn(CallExpr *node)
{
   auto ty = node->getBuiltinArgType();
   switch (node->getBuiltinFnKind()) {
      case BuiltinFn::SIZEOF:
         return Variant(ty->getSize());
      case BuiltinFn::ALIGNOF:
         return Variant((size_t)ty->getAlignment());
      default:
         break;
   }

   err(err_generic_error)
      << "function not callable in a static context"
      << node;

   return {};
}

Variant EvaluatorImpl::visitCallExpr(CallExpr *node)
{
   if (node->getBuiltinFnKind() != BuiltinFn::None)
      return HandleBuiltinFn(node);

   if (node->getIdent() == "decltype") {
      std::vector<Variant> vec;

      for (const auto &arg : node->getArgs()) {
         auto ty = SP.visit(arg);
         vec.emplace_back(*ty);
      }

      if (vec.empty()) {
         err(err_generic_error)
            << "decltype expects at least one argument"
            << node;

         return {};
      }
      else if (vec.size() == 1) {
         return vec.front();
      }

      return Variant(move(vec));
   }

   return {};
}

Variant EvaluatorImpl::visitUnaryOperator(UnaryOperator *node)
{
   auto res = visitExpr(node, node->getTarget().get());
   if (res.stopEvaluation())
      return {};

   auto &expr = res.getResult();
   auto &op = node->getOp();
   auto val = expr.applyUnaryOp(op);

   if (val.getKind() == VariantType::VOID)
      err(err_unary_op_not_applicable)
         << op << 0 << Variant::typeToString(expr.getKind())
         << node;

   return val;
}

Variant EvaluatorImpl::visitBinaryOperator(BinaryOperator *node)
{
   if (node->getOp() == ":") {
      auto result = node->getTypePredicateResult();
      if (!result) {
         auto constraint = cast<ConstraintExpr>(node->getRhs().get());
         err(err_generic_error, false)
            << stringifyConstraint(*node->getLhs()->getExprType(),
                                   constraint)
            << node;
      }

      return Variant(result);
   }

   auto lhsRes = visitExpr(node, node->getLhs().get());
   if (lhsRes.stopEvaluation())
      return {};


   auto rhsRes = visitExpr(node, node->getRhs().get());
   if (rhsRes.stopEvaluation())
      return {};

   auto &lhs = lhsRes.getResult();
   auto &rhs = rhsRes.getResult();

   auto res = lhs.applyBinaryOp(rhs, node->getOp());
   if (res.getKind() == VariantType::VOID)
      err(err_binop_not_applicable)
         << node->getOp() << Variant::typeToString(lhs.getKind())
         << Variant::typeToString(rhs.getKind()) << node;

   return res;
}

Variant EvaluatorImpl::visitTertiaryOperator(TertiaryOperator *node)
{
   auto condRes = visitExpr(node, node->getCondition().get());
   if (condRes.stopEvaluation())
      return {};

   auto &cond = condRes.getResult();
   if (cond.getKind() != VariantType::INT)
      err(err_cond_not_boolean)
         << 3 /*tertiary op*/ << node;

   if (cond.getInt() != 0)
      return visit(node->getLhs().get());

   return visit(node->getRhs().get());
}

Variant EvaluatorImpl::visitExprSequence(ExprSequence *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitTypeRef(TypeRef *node)
{
   return Variant(*node->getTypeRef());
}

Variant EvaluatorImpl::visitTupleLiteral(TupleLiteral *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitLambdaExpr(LambdaExpr *node)
{
   llvm_unreachable("not yet");
}

Variant EvaluatorImpl::visitLvalueToRvalue(LvalueToRvalue *node)
{
   return visit(node->getTarget().get());
}

Variant EvaluatorImpl::visitImplicitCastExpr(ImplicitCastExpr *node)
{
   return visit(node->getTarget().get());
}

StaticExprEvaluator::StaticExprEvaluator(SemaPass &S,
                                         sema::TemplateArgList const* TAList)
   : pImpl(new EvaluatorImpl(S, S.currentClass(), S.currentScope()->function,
                             S.importedNamespaces(), TAList))
{

}

StaticExprEvaluator::StaticExprEvaluator(SemaPass &S,
                                         cl::Record *ClassContext,
                                         Callable *FuncCtx,
                                      llvm::ArrayRef<size_t> importedNamespaces,
                                         sema::TemplateArgList const* TAList)
   : pImpl(new EvaluatorImpl(S, ClassContext, FuncCtx,
                             importedNamespaces, TAList))
{

}

StaticExprResult StaticExprEvaluator::evaluate(StaticExpr *expr,
                                               SourceLocation requestedFrom) {
   return pImpl->visit(expr, requestedFrom);
}

} // namespace ast
} // namespace cdot