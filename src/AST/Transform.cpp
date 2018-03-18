////
//// Created by Jonas Zell on 19.01.18.
////
//
//#include "Transform.h"
//
//#include "AST/Passes/ASTIncludes.h"
//#include "AST/Passes/SemanticAnalysis/SemaPass.h"
//
//using namespace cdot::support;
//
//namespace cdot {
//namespace ast {
//
//#define REPLACE(member) \
//if (stmt->member == orig) { stmt->member = rep; return true; }
//
//#define REPLACE_EXPR(member) \
//if (stmt->member == orig) { stmt->member = cast<Expression>(rep); return true; }
//
//#define REPLACE_OTHER(member, T) \
//if (stmt->member == orig) { stmt->member = cast<T>(rep); return true; }
//
//#define REPLACE_RANGE(member) \
//for (auto &m : stmt->member) if (m == orig) { m = rep; return true; }
//
//#define REPLACE_RANGE_OTHER(member, T) \
//for (auto &m : stmt->member) if (m == orig) { m = cast<T>(rep); return true; }
//
//class TransformImpl {
//public:
//   TransformImpl(SemaPass &SP, Statement *orig, Statement *rep)
//      : SP(SP), orig(orig), rep(rep)
//   { }
//
//   bool doReplacement()
//   {
//      SP.updateParent(rep, SP.getParent(orig));
//      rep->setSourceLoc(orig->getSourceLoc());
//      rep->setContextualType(orig->getContextualType());
//
//      bool success = false;
//      switch (SP.getParent(orig)->getTypeID()) {
//#     define CDOT_ASTNODE(Name)                                      \
//         case AstNode::Name##ID:                                     \
//            success = replaceFrom##Name(cast<Name>(SP.getParent(orig))); break;
//#     include "AstNode.def"
//
//         default:
//            llvm_unreachable("bad node kind");
//      }
//
//      if (success)
//         return true;
//
//      return false;
//   }
//
//private:
//   SemaPass &SP;
//   Statement *orig;
//   Statement *rep;
//
//   bool replaceFromTranslationUnit(TranslationUnit *stmt)
//   {
//      for (auto &import : stmt->getImports()) {
//         if (import == orig) {
//            import = cast<ImportStmt>(rep);
//            return true;
//         }
//      }
//      for (auto &s : stmt->getStatements()) {
//         if (s == orig) {
//            s = rep;
//            return true;
//         }
//      }
//
//      return false;
//   }
//
//   bool replaceFromCompoundStmt(CompoundStmt *stmt)
//   {
//      for (auto &s : stmt->statements) {
//         if (s == orig) {
//            s = rep;
//            return true;
//         }
//      }
//
//      return false;
//   }
//
//   bool replaceFromBreakStmt(BreakStmt *stmt) {return false;}
//   bool replaceFromContinueStmt(ContinueStmt *stmt) {return false;}
//
//   bool replaceFromReturnStmt(ReturnStmt *stmt)
//   {
//      assert(orig == stmt->getReturnValue());
//      stmt->returnValue = cast<Expression>(rep);
//
//      return true;
//   }
//
//   bool replaceFromCaseStmt(CaseStmt *stmt)
//   {
//      if (stmt->getPattern() == orig) {
//         stmt->pattern = cast<PatternExpr>(rep);
//         return true;
//      }
//      if (stmt->getBody() == orig) {
//         stmt->body = rep;
//         return true;
//      }
//
//      return false;
//   }
//
//   bool replaceFromLabelStmt(LabelStmt *stmt) {return false;}
//   bool replaceFromGotoStmt(GotoStmt *stmt) {return false;}
//
//   bool replaceFromForStmt(ForStmt *stmt)
//   {
//      REPLACE(initialization)
//      REPLACE_EXPR(termination)
//      REPLACE(increment)
//      REPLACE(body)
//
//      return false;
//   }
//
//   bool replaceFromIfStmt(IfStmt *stmt)
//   {
//      REPLACE_EXPR(condition)
//      REPLACE(ifBranch)
//      REPLACE(elseBranch)
//
//      return false;
//   }
//
//   bool replaceFromWhileStmt(WhileStmt *stmt)
//   {
//      REPLACE_EXPR(condition)
//      REPLACE(body)
//
//      return false;
//   }
//
//   bool replaceFromForInStmt(ForInStmt *stmt)
//   {
//      REPLACE_EXPR(rangeExpr)
//      REPLACE(body)
//
//      return false;
//   }
//
//   bool replaceFromMatchStmt(MatchStmt *stmt)
//   {
//      REPLACE_EXPR(switchValue)
//      REPLACE_RANGE_OTHER(cases, CaseStmt)
//
//      return false;
//   }
//
//   bool replaceFromLocalVarDecl(LocalVarDecl *stmt)
//   {
//      REPLACE_EXPR(value)
//      REPLACE_OTHER(type, TypeRef)
//
//      return false;
//   }
//
//   bool replaceFromGlobalVarDecl(GlobalVarDecl *stmt)
//   {
//      REPLACE_EXPR(value)
//      REPLACE_OTHER(type, TypeRef)
//
//      return false;
//   }
//
//   bool replaceFromLocalDestructuringDecl(LocalDestructuringDecl *stmt)
//   {
//      REPLACE_EXPR(value)
//      REPLACE_OTHER(type, TypeRef)
//
//      return false;
//   }
//
//   bool replaceFromGlobalDestructuringDecl(GlobalDestructuringDecl *stmt)
//   {
//      REPLACE_EXPR(value)
//      REPLACE_OTHER(type, TypeRef)
//
//      return false;
//   }
//
//   bool replaceFromTemplateParamDecl(TemplateParamDecl *stmt)
//   {
//      REPLACE_OTHER(covariance, TypeRef)
//      REPLACE_OTHER(contravariance, TypeRef)
//      REPLACE_OTHER(defaultValue, Expression)
//
//      return false;
//   }
//
//   bool replaceFromAliasDecl(AliasDecl *stmt)
//   {
//      REPLACE_RANGE_OTHER(constraints, StaticExpr)
//      REPLACE_OTHER(aliasExpr, StaticExpr)
//
//      return false;
//   }
//
//   bool replaceFromEnumCaseDecl(EnumCaseDecl *stmt)
//   {
//      return false;
//   }
//
//   bool replaceFromCallableDecl(CallableDecl *stmt)
//   {
//      REPLACE_OTHER(body, CompoundStmt)
//      REPLACE_OTHER(returnType, TypeRef)
//
//      return false;
//   }
//
//   bool replaceFromInitDecl(InitDecl *stmt)
//   {
//      if (replaceFromCallableDecl(stmt))
//         return true;
//
//      return false;
//   }
//
//   bool replaceFromFieldDecl(FieldDecl *stmt)
//   {
//      REPLACE_EXPR(defaultVal)
//
//      return false;
//   }
//
//   bool replaceFromMethodDecl(MethodDecl *stmt)
//   {
//      if (replaceFromCallableDecl(stmt))
//         return true;
//
//      return false;
//   }
//
//   bool replaceFromAssociatedTypeDecl(AssociatedTypeDecl *stmt)
//   {
//      REPLACE_RANGE_OTHER(constraints, StaticExpr)
//      REPLACE_OTHER(actualType, TypeRef)
//
//      return false;
//   }
//
//   bool replaceFromFuncArgDecl(FuncArgDecl *stmt)
//   {
//      return false;
//   }
//
//   bool replaceFromDeinitDecl(DeinitDecl *stmt)
//   {
//      return replaceFromCallableDecl(stmt);
//   }
//
//   bool replaceFromPropDecl(PropDecl *stmt)
//   {
//      return false;
//   }
//
//   bool replaceFromExtensionDecl(ExtensionDecl *stmt)
//   {
//      if (replaceFromRecordDecl(stmt))
//         return true;
//
//      return false;
//   }
//
//   bool replaceFromFunctionDecl(FunctionDecl *stmt)
//   {
//      if (replaceFromCallableDecl(stmt))
//         return true;
//
//      return false;
//   }
//
//   bool replaceFromNamespaceDecl(NamespaceDecl *stmt)
//   {
//      REPLACE_OTHER(body, CompoundStmt)
//
//      return false;
//   }
//
//   bool replaceFromTypedefDecl(TypedefDecl *stmt)
//   {
//      REPLACE_OTHER(origin, TypeRef)
//
//      return false;
//   }
//
//   bool replaceFromTryStmt(TryStmt *stmt)
//   {
//      REPLACE(body)
//      for (auto &C : stmt->catchBlocks) {
//         if (C.body == orig) {
//            C.body = rep;
//            return true;
//         }
//         if (C.varDecl == orig) {
//            C.varDecl = cast<LocalVarDecl>(rep);
//            return true;
//         }
//      }
//
//      REPLACE(finallyBlock)
//
//      return false;
//   }
//
//   bool replaceFromThrowStmt(ThrowStmt *stmt)
//   {
//      REPLACE_EXPR(thrownVal)
//      return false;
//   }
//
//   bool replaceFromDebugStmt(DebugStmt *stmt) { return false; }
//   bool replaceFromNullStmt(NullStmt *stmt) { return false; }
//
//   bool replaceFromUsingStmt(UsingStmt *stmt) { return false; }
//
//   bool replaceFromModuleStmt(ModuleStmt *stmt) { return false; }
//   bool replaceFromImportStmt(ImportStmt *stmt) { return false; }
//
//   bool replaceFromStaticIfStmt(StaticIfStmt *stmt)
//   {
//      REPLACE_OTHER(condition, StaticExpr)
//      REPLACE(ifBranch)
//      REPLACE(elseBranch)
//
//      return false;
//   }
//
//   bool replaceFromStaticForStmt(StaticForStmt *stmt)
//   {
//      REPLACE_OTHER(range, StaticExpr)
//      REPLACE(body)
//
//      return false;
//   }
//
//   bool replaceFromStaticAssertStmt(StaticAssertStmt *stmt)
//   {
//      REPLACE_OTHER(expr, StaticExpr)
//
//      return false;
//   }
//
//   bool replaceFromStaticPrintStmt(StaticPrintStmt *stmt)
//   {
//      REPLACE_OTHER(expr, StaticExpr)
//
//      return false;
//   }
//
//   bool replaceFromRecordDecl(RecordDecl *stmt)
//   {
//      REPLACE_RANGE_OTHER(conformanceTypes, TypeRef)
//
//      REPLACE_RANGE_OTHER(constraints, StaticExpr)
//      REPLACE_RANGE_OTHER(staticStatements, StaticStmt)
//
//      return false;
//   }
//
//   bool replaceFromClassDecl(ClassDecl *stmt)
//   {
//      if (replaceFromRecordDecl(stmt))
//         return true;
//
//      return false;
//   }
//
//   bool replaceFromStructDecl(StructDecl *stmt)
//   {
//      if (replaceFromRecordDecl(stmt))
//         return true;
//
//      return false;
//   }
//
//   bool replaceFromEnumDecl(EnumDecl *stmt)
//   {
//      if (replaceFromRecordDecl(stmt))
//         return true;
//
//      return false;
//   }
//
//   bool replaceFromUnionDecl(UnionDecl *stmt)
//   {
//      if (replaceFromRecordDecl(stmt))
//         return true;
//
//      return false;
//   }
//
//   bool replaceFromProtocolDecl(ProtocolDecl *stmt)
//   {
//      if (replaceFromRecordDecl(stmt))
//         return true;
//
//      return false;
//   }
//
//   bool replaceFromTypeRef(TypeRef *stmt)
//   {
//      switch (stmt->getKind()) {
//         case TypeRef::ObjectType:
//            return false;
//         case TypeRef::FunctionType:
//            REPLACE_OTHER(returnType, TypeRef)
//            LLVM_FALLTHROUGH;
//         case TypeRef::TupleType:
//            for (auto &cont : stmt->containedTypes)
//               if (cont.col == orig) {
//                  cont.col = cast<TypeRef>(rep);
//                  return true;
//               }
//
//            return false;
//         case TypeRef::ArrayType:
//            REPLACE_OTHER(arraySize, StaticExpr)
//            for (auto &cont : stmt->containedTypes)
//               if (cont.col == orig) {
//                  cont.col = cast<TypeRef>(rep);
//                  return true;
//               }
//
//            return false;
//         case TypeRef::DeclTypeExpr:
//            REPLACE_EXPR(declTypeExpr)
//            return false;
//         case TypeRef::Pointer:
//         case TypeRef::Option:
//            REPLACE_OTHER(subject, TypeRef)
//            return false;
//         default:
//            return false;
//      }
//   }
//
//   bool replaceFromParenExpr(ParenExpr *stmt)
//   {
//      REPLACE_EXPR(ParenthesizedExpr)
//      return false;
//   }
//
//   bool replaceFromIntegerLiteral(IntegerLiteral *stmt) { return false; }
//   bool replaceFromFPLiteral(FPLiteral *stmt) { return false; }
//   bool replaceFromBoolLiteral(BoolLiteral *stmt) { return false; }
//   bool replaceFromCharLiteral(CharLiteral *stmt) { return false; }
//   bool replaceFromNoneLiteral(NoneLiteral *stmt) { return false; }
//   bool replaceFromStringLiteral(StringLiteral *stmt) { return false; }
//
//   bool replaceFromStringInterpolation(StringInterpolation *stmt)
//   {
//      REPLACE_RANGE_OTHER(strings, Expression)
//      return false;
//   }
//
//   bool replaceFromLambdaExpr(LambdaExpr *stmt)
//   {
//      REPLACE_RANGE_OTHER(args, FuncArgDecl)
//      REPLACE_OTHER(body, CompoundStmt)
//      REPLACE_OTHER(returnType, TypeRef)
//
//      return false;
//   }
//
//   bool replaceFromDictionaryLiteral(DictionaryLiteral *stmt)
//   {
//      REPLACE_RANGE_OTHER(keys, Expression)
//      REPLACE_RANGE_OTHER(values, Expression)
//
//      return false;
//   }
//
//   bool replaceFromArrayLiteral(ArrayLiteral *stmt)
//   {
//      REPLACE_RANGE_OTHER(values, Expression)
//
//      return false;
//   }
//
//   bool replaceFromTupleLiteral(TupleLiteral *stmt)
//   {
//      REPLACE_RANGE_OTHER(elements, Expression)
//      return false;
//   }
//
//   bool replaceFromIdentifierRefExpr(IdentifierRefExpr *stmt)
//   {
//      REPLACE_RANGE_OTHER(templateArgs, Expression)
//
//      return false;
//   }
//
//   bool replaceFromBuiltinIdentExpr(BuiltinIdentExpr *stmt)
//   {
//      return false;
//   }
//
//   bool replaceFromSelfExpr(SelfExpr *stmt)
//   {
//      return false;
//   }
//
//   bool replaceFromSuperExpr(SuperExpr *stmt)
//   {
//      return false;
//   }
//
//   bool replaceFromMemberRefExpr(MemberRefExpr *stmt)
//   {
//      REPLACE_RANGE_OTHER(templateArgs, Expression)
//
//      return false;
//   }
//
//   bool replaceFromTupleMemberExpr(TupleMemberExpr *stmt)
//   {
//      return false;
//   }
//
//   bool replaceFromCallExpr(CallExpr *stmt)
//   {
//      REPLACE_RANGE_OTHER(args, Expression)
//      REPLACE_RANGE_OTHER(templateArgs, Expression)
//
//      return false;
//   }
//
//   bool replaceFromEnumCaseExpr(EnumCaseExpr *stmt)
//   {
//      REPLACE_RANGE_OTHER(args, Expression)
//      return false;
//   }
//
//   bool replaceFromSubscriptExpr(SubscriptExpr *stmt)
//   {
//      REPLACE_RANGE_OTHER(indices, Expression)
//      return false;
//   }
//
//   bool replaceFromBuiltinExpr(BuiltinExpr *stmt)
//   {
//      return false;
//   }
//
//   bool replaceFromExpressionPattern(ExpressionPattern *stmt)
//   {
//      REPLACE_EXPR(expr)
//      return false;
//   }
//
//   bool replaceFromCasePattern(CasePattern *stmt)
//   {
//      for (auto &arg : stmt->args) {
//         if (arg.isExpr()) {
//            if (arg.expr == orig) {
//               arg.expr = cast<Expression>(rep);
//               return true;
//            }
//         }
//      }
//
//      return false;
//   }
//
//   bool replaceFromIsPattern(IsPattern *stmt)
//   {
//      REPLACE_OTHER(isType, TypeRef)
//      return false;
//   }
//
//   bool replaceFromExprSequence(ExprSequence *stmt)
//   {
//      for (auto &el : stmt->fragments) {
//         if (el.getKind() == ExprSequence::SequenceElement::EF_Expression) {
//            if (el.expr == orig) {
//               el.expr = cast<Expression>(rep);
//               return true;
//            }
//         }
//      }
//
//      return false;
//   }
//
//   bool replaceFromUnaryOperator(UnaryOperator *stmt)
//   {
//      REPLACE_EXPR(target)
//      return false;
//   }
//
//   bool replaceFromBinaryOperator(BinaryOperator *stmt)
//   {
//      REPLACE_EXPR(lhs)
//      REPLACE_EXPR(rhs)
//      return false;
//   }
//
//   bool replaceFromTypePredicateExpr(TypePredicateExpr *stmt)
//   {
//      REPLACE_EXPR(LHS)
//      REPLACE_EXPR(RHS)
//      return false;
//   }
//
//   bool replaceFromCastExpr(CastExpr *stmt)
//   {
//      REPLACE_EXPR(target);
//      REPLACE_OTHER(targetType, TypeRef);
//
//      return false;
//   }
//
//   bool replaceFromImplicitCastExpr(ImplicitCastExpr *stmt)
//   {
//      REPLACE_EXPR(target)
//      return false;
//   }
//
//   bool replaceFromIfExpr(IfExpr *stmt)
//   {
//      REPLACE_EXPR(Cond);
//      REPLACE_EXPR(TrueVal);
//      REPLACE_EXPR(FalseVal);
//
//      return false;
//   }
//
//   bool replaceFromVarDeclExpr(VarDeclExpr *stmt)
//   {
//      REPLACE_OTHER(Decl, VarDecl);
//      return false;
//   }
//
//   bool replaceFromStaticExpr(StaticExpr *stmt)
//   {
//      REPLACE_EXPR(expr)
//      return false;
//   }
//
//   bool replaceFromConstraintExpr(ConstraintExpr *stmt)
//   {
//      REPLACE_OTHER(typeConstraint, TypeRef)
//      return false;
//   }
//
//   bool replaceFromTraitsExpr(TraitsExpr *stmt)
//   {
//      for (auto &arg : stmt->args) {
//         switch (arg.getKind()) {
//            case TraitsExpr::TraitsArgument::Expr:
//               if (arg.expr == orig) {
//                  arg.expr = cast<Expression>(rep);
//                  return true;
//               }
//
//               break;
//            case TraitsExpr::TraitsArgument::Stmt:
//               if (arg.stmt == orig) {
//                  arg.stmt = cast<Statement>(rep);
//                  return true;
//               }
//
//               break;
//            case TraitsExpr::TraitsArgument::Type:
//               if (arg.type == orig) {
//                  arg.type = cast<TypeRef>(rep);
//                  return true;
//               }
//
//               break;
//            default:
//               break;
//         }
//      }
//
//      return false;
//   }
//
//   bool replaceFromMixinExpr(MixinExpr *stmt)
//   {
//      REPLACE_EXPR(MixinEx)
//      return false;
//   }
//
//   bool replaceFromMixinDecl(MixinDecl *stmt)
//   {
//      REPLACE_EXPR(MixinExpr)
//      return false;
//   }
//};
//
//void replaceStatementWith(SemaPass &SP, Statement *stmt, Statement *rep)
//{
//   auto success = TransformImpl(SP, stmt, rep).doReplacement();
//   assert(success && "invalid transformation");
//}
//
//void replaceExpressionWith(SemaPass &SP, Expression *expr, Expression *rep)
//{
//   auto success = TransformImpl(SP, expr, rep).doReplacement();
//   assert(success && "invalid transformation");
//}
//
//} // namespace ast
//} // namespace cdot