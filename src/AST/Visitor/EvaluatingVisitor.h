////
//// Created by Jonas Zell on 26.06.17.
////
//
//#ifndef CDOT_LITERALEXPRVISITOR_H
//#define CDOT_LITERALEXPRVISITOR_H
//
//
//#include "Visitor.h"
//#include "../Statement/Declaration/FuncArgDecl.h"
//#include "../Statement/ControlFlow/LabelStmt.h"
//#include "../../StdLib/Module.h"
//#include "../../StdLib/Class/Interface.h"
//
//class EvaluatingVisitor : public Visitor {
//public:
//    EvaluatingVisitor();
//    EvaluatingVisitor(std::shared_ptr<Context> ctx);
//    EvaluatingVisitor(const EvaluatingVisitor&);
//
//    inline void set_out_module(Module* mod) {
//        out_module = mod;
//    }
//
//    virtual Variant evaluate(CompoundStmt*);
//
//    virtual Variant visit(ModuleDecl*);
//    virtual Variant visit(UsingStmt*);
//    virtual Variant visit(EndOfFileStmt*);
//
//    virtual Variant visit(FunctionDecl*);
//    virtual Variant visit(CompoundStmt*);
//    virtual Variant visit(IdentifierRefExpr*);
//    virtual Variant visit(DeclStmt*);
//    virtual Variant visit(ForStmt*);
//    virtual Variant visit(WhileStmt*);
//
//    virtual Variant visit(CollectionLiteral*);
//    virtual Variant visit(LiteralExpr*);
//    virtual Variant visit(StringLiteral*);
//    virtual Variant visit(SubscriptExpr*);
//    virtual Variant visit(MethodCallExpr*);
//    virtual Variant visit(FunctionCallExpr*);
//    virtual Variant visit(CallExpr*);
//    virtual Variant visit(MemberRefExpr*);
//    virtual Variant visit(BinaryOperator*);
//    virtual Variant visit(ExplicitCastExpr*);
//    virtual Variant visit(TertiaryOperator*);
//    virtual Variant visit(UnaryOperator*);
//    virtual Variant visit(BreakStmt*);
//    virtual Variant visit(ContinueStmt*);
//    virtual Variant visit(IfStmt*);
//    virtual Variant visit(SwitchStmt*);
//    virtual Variant visit(CaseStmt*);
//    virtual Variant visit(LabelStmt*);
//    virtual Variant visit(GotoStmt*);
//    virtual Variant visit(FuncArgDecl*);
//    virtual Variant visit(ReturnStmt*);
//    virtual Variant visit(InputStmt*);
//    virtual Variant visit(OutputStmt*);
//    virtual Variant visit(Expression*);
//    virtual Variant visit(ClassDecl*);
//    virtual Variant visit(MethodDecl*);
//    virtual Variant visit(FieldDecl*);
//    virtual Variant visit(ConstrDecl*);
//    virtual Variant visit(LambdaExpr*);
//    virtual Variant visit(OperatorDecl*);
//    virtual Variant visit(StructDecl*);
//    virtual Variant visit(InterfaceDecl*);
//    virtual Variant visit(ImplicitCastExpr*);
//
//    virtual Variant evaluate_unary_op(UnaryOperator*);
//    virtual Variant evaluate_binary_op(BinaryOperator*);
//
//    inline Context* get_ctx() {
//        return context.get();
//    }
//
//    inline void set_implicit_return(bool ret) {
//        implicit_return = ret;
//    }
//
//protected:
//    /** Context */
//    Module* out_module;
//    std::shared_ptr<Context> context;
//    std::unordered_map<std::string, Variant::SharedPtr> exports;
//
//    bool implicit_return = true;
//
//    /** Identifier evaluation */
//    std::vector<Variant::SharedPtr> current_ref = {};
//
//    /** Object and function declarations */
//    FuncArg current_arg;
//    Class::UniquePtr current_class;
//
//    bool is_interface = false;
//    Interface::UniquePtr current_interface = {};
//
//    /** Goto label */
//    LabelStmt* current_label;
//    int start_index = 0;
//};
//
//
//#endif //CDOT_LITERALEXPRVISITOR_H
