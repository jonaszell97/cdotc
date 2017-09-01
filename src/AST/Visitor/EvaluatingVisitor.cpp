////
//// Created by Jonas Zell on 26.06.17.
////
//
//#include <regex>
//#include <iostream>
//#include "EvaluatingVisitor.h"
//#include "Visitor.cpp"
//#include "CaptureVisitor.h"
//#include "../../StdLib/Objects/Array.h"
//#include "../../Parser.h"
//
//EvaluatingVisitor::EvaluatingVisitor() : context(std::make_shared<Context>()), current_label(nullptr),
//    exports(std::unordered_map<std::string, Variant::SharedPtr>()) {
//
//}
//
//EvaluatingVisitor::EvaluatingVisitor(Context::SharedPtr ctx) : EvaluatingVisitor() {
//    context->set_parent_ctx(ctx);
//    if (ctx != nullptr) {
//        context->class_context = ctx->class_context;
//    }
//}
//
///**
// * Does NOT make a full copy of the object - a new context is created with the provided one as parent context
// * @param v
// */
//EvaluatingVisitor::EvaluatingVisitor(const EvaluatingVisitor &v) : EvaluatingVisitor() {
//    context->set_parent_ctx(v.context);
//}
//
//Variant EvaluatingVisitor::visit(ModuleDecl *root) {
//    int goto_index = 0;
//    for (auto import : root->imports) {
//        import->accept(*this);
//    }
//
//    for (int i = start_index; i < root->_statements.size(); i++) {
//        if (root->_is_lambda_body && i == root->_statements.size() - 1) {
//            return root->_statements[i]->accept(*this);
//        }
//
//        root->_statements[i]->accept(*this);
//        if (context->returnable && context->returned) {
//            return context->return_val;
//        }
//        if ((context->continuable && context->continued) || context->goto_encountered) {
//            context->goto_encountered = false;
//            break;
//        }
//        if (context->breakable && context->broken) {
//            break;
//        }
//        if (current_label != nullptr) {
//            current_label->set_parent_cmpnd(root);
//            current_label->set_visitor(this);
//            root->goto_index = goto_index;
//            current_label = nullptr;
//        }
//
//        goto_index++;
//    }
//
//    if (out_module != nullptr) {
//        out_module->set_name(root->module_name);
//        out_module->set_exports(exports);
//    }
//
//    return {};
//}
//
//Variant EvaluatingVisitor::visit(UsingStmt *node) {
//    GlobalContext::import_module(node->path, node->module_name, node->is_lib_import, node->import_identifiers);
//
//    return {};
//}
//
//Variant EvaluatingVisitor::visit(EndOfFileStmt *node) {
//    exports.emplace(node->target, context->get_variable(node->target));
//
//    return {};
//}
//
///**
// * Evaluates a compound statement by evaluating all of its enclosed statements, until a ReturnStmt is reached
// * @param node
// * @return Variant
// */
//Variant EvaluatingVisitor::visit(CompoundStmt *node) {
//    EvaluatingVisitor ev(*this);
//    ev.context->returnable = node->_returnable;
//
//    // goto index
//    int goto_index = 0;
//    Variant v;
//    for (int i = start_index; i < node->_statements.size(); i++) {
//        v = node->_statements[i]->accept(ev);
//        if (ev.context->returnable && ev.context->returned) {
//            return ev.context->return_val;
//        }
//        if ((context->continuable && context->continued) || context->goto_encountered) {
//            context->goto_encountered = false;
//            break;
//        }
//        if (context->breakable && context->broken) {
//            break;
//        }
//        if (ev.current_label != nullptr) {
//            ev.current_label->set_parent_cmpnd(node);
//            ev.current_label->set_visitor(&ev);
//            node->goto_index = i;
//            ev.current_label = nullptr;
//        }
//
//        goto_index++;
//    }
//
//    // return value of last statement / expression if no return found
//    return ev.context->returned ? ev.context->return_val : implicit_return ? v : Variant();
//}
//
///**
// * Evaluates a compound statement by evaluating all of its enclosed statements, until a ReturnStmt is reached
// * @param node
// * @return Variant
// */
//Variant EvaluatingVisitor::evaluate(CompoundStmt *node) {
//    int goto_index = 0;
//    Variant v;
//    for (int i = start_index; i < node->_statements.size(); i++) {
//        v = node->_statements[i]->accept(*this);
//        if (context->returnable && context->returned) {
//            return context->return_val;
//        }
//        if ((context->continuable && context->continued) || context->goto_encountered) {
//            context->goto_encountered = false;
//            break;
//        }
//        if (context->breakable && context->broken) {
//            break;
//        }
//        if (current_label != nullptr) {
//            current_label->set_parent_cmpnd(node);
//            current_label->set_visitor(this);
//            node->goto_index = i;
//            current_label = nullptr;
//        }
//
//        goto_index++;
//    }
//
//    // return value of last statement / expression if no return found
//    return context->returned ? context->return_val : implicit_return ? v : Variant();
//}
//
///**
// * Evaluates a function declaration and makes it available in the current context
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(FunctionDecl *node) {
//
//    Function::SharedPtr func = std::make_shared<Function>(node->_func_name, node->_return_type);
//    context->set_variable(node->_func_name, Variant(func));
//
//    CaptureVisitor cv(context.get());
//    cv.visit(node->_body.get());
//    cv.capture();
//
//    func->set_body(node->_body);
//    func->set_context(context);
//
//    for (auto arg : node->_args) {
//        arg->accept(*this);
//        func->add_argument(current_arg.name, current_arg.type, current_arg.default_val);
//    }
//
//    return {};
//}
//
///**
// * Evaluates an identifier reference by returning the associated variable
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(IdentifierRefExpr *node) {
//
//    if (GlobalContext::is_declared_class(node->_ident)) {
//        current_ref.push_back(std::make_shared<Variant>(GlobalContext::get_class(node->_ident)));
//    }
//    else if (GlobalContext::is_declared_interface(node->_ident)) {
//        current_ref.push_back(std::make_shared<Variant>(GlobalContext::get_interface(node->_ident)));
//    }
//    else {
//        current_ref.push_back(context->get_variable(node->_ident, node));
//    }
//
//    if (node->_member_expr == nullptr) {
//        auto return_val = node->lvalue ? Variant(current_ref.back()) : *current_ref.back();
//        current_ref.pop_back();
//
//        return return_val;
//    } else {
//        return node->lvalue ? node->_member_expr->accept(*this) : *node->_member_expr->accept(*this);
//    }
//
//    return {};
//}
//
///**
// * Evaluates a variable declaration in the current context
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(DeclStmt *node) {
//
//    for (int i = 0; i < node->decl_identifiers.size(); ++i) {
//        auto& ident = node->decl_identifiers[i];
//        auto& decl = node->declarations[i];
//
//        if (decl.first.raw_array) {
//
//            CollectionLiteral::SharedPtr arr = std::dynamic_pointer_cast<CollectionLiteral>(decl.second);
//            if (arr == nullptr) {
//                RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign variable of type " + val::typetostr(decl.first
//                    ) + " to array of type " + val::typetostr(decl.first), node);
//            }
//            else if (arr->_var_length && !decl.first.is_var_length) {
//                RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign variable length array to array of fixed length",
//                        node);
//            }
//
//            arr->set_type(TypeSpecifier(decl.first.type));
//            if (decl.first.is_var_length) {
//                arr->is_var_length(true);
//            }
//            else {
//                arr->set_length_expr(decl.first.length);
//            }
//
//            context->set_variable(ident, arr->accept(*this));
//
//            continue;
//        }
//
//        Variant res;
//
//        // use default value
//        if (decl.second == nullptr) {
//            res.set_type(decl.first);
//            res.set_default();
//        }
//        else {
//            res = decl.second->accept(*this);
//            res.is_nullable(decl.first.nullable);
//            if (decl.second == nullptr) {
//                res.set_type(decl.first);
//                res.is_null(true);
//            }
//
//            // type inference
//            if (decl.first.type == AUTO_T) {
//                decl.first = res.get_type();
//            }
//            else if (decl.first == ANY_T) {
//                res.is_any_type();
//            }
//        }
//
//        context->set_variable(ident, res);
//    }
//
//    return {};
//}
//
///**
// * Evaluates a for loop
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(ForStmt *node) {
//    if (!node->range_based) {
//        EvaluatingVisitor init_visitor(*this);
//        node->_initialization->accept(init_visitor);
//
//        if (node->_body == nullptr) {
//            return {};
//        }
//
//        EvaluatingVisitor loop_visitor(init_visitor);
//        loop_visitor.context->returnable = context->returnable;
//        loop_visitor.context->breakable = true;
//        loop_visitor.context->continuable = true;
//
//        while (!loop_visitor.context->broken && node->_termination->accept(init_visitor).get<bool>()) {
//            node->_body->accept(loop_visitor);
//            node->_increment->accept(init_visitor);
//            loop_visitor.context->continued = false;
//            loop_visitor.context->reset();
//        }
//    }
//    else {
//        EvaluatingVisitor loop_visitor(*this);
//        loop_visitor.context->returnable = context->returnable;
//        loop_visitor.context->breakable = true;
//        loop_visitor.context->continuable = true;
//
//        Variant res = node->range->accept(*this);
//        Array::SharedPtr range = std::dynamic_pointer_cast<Array>(res.get<Object::SharedPtr>());
//        if (range == nullptr) {
//            RuntimeError::raise(ERR_TYPE_ERROR, "Expected range argument in range based for loop to be array",
//                    node->range.get());
//        }
//
//        int i = 0;
//        while (!loop_visitor.context->broken && i < range->get_length()) {
//            loop_visitor.context->set_variable(node->ident->_ident, *range->at(i++));
//            node->_body->accept(loop_visitor);
//            loop_visitor.context->continued = false;
//            loop_visitor.context->reset();
//        }
//    }
//
//    return {};
//}
//
///**
// * Evaluates a while loop
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(WhileStmt *node) {
//
//    EvaluatingVisitor loop_visitor(*this);
//    loop_visitor.context->returnable = context->returnable;
//    loop_visitor.context->breakable = true;
//    loop_visitor.context->continuable = true;
//
//    while (!loop_visitor.context->broken && node->_condition->accept(loop_visitor).get<bool>()) {
//        node->_while_block->accept(loop_visitor);
//        loop_visitor.context->continued = false;
//        loop_visitor.context->reset();
//    }
//
//    return {};
//}
//
///**
// * Creates an Array from an CollectionLiteral node
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(CollectionLiteral *node) {
//    int length = node->_length;
//
//    if (node->_length_expr != nullptr) {
//        int _length = node->_length_expr->accept(*this).get<int>();
//        if (_length > INT32_MAX) {
//            RuntimeError::raise(ERR_VAL_TOO_LARGE, "An array can hold a maxium of " + std::to_string(INT32_MAX)
//                + " values, tried to allocate " + std::to_string(_length), node->_length_expr.get());
//        }
//
//       length = _length;
//    }
//
//    TypeSpecifier ts = node->type;
//    if (ts.type == AUTO_T) {
//        ts.type = ANY_T;
//    }
//
//    std::shared_ptr<Array> arr = std::make_shared<Array>(ts, length);
//    TypeSpecifier prev;
//    bool any_t = false;
//    for (int i = 0; i < node->_elements.size(); ++i) {
//        auto res = node->_elements[i]->accept(*this);
//        TypeSpecifier current = res.get_type();
//
//        if (res.get_type() == DOUBLE_T && node->type != DOUBLE_T) {
//            res.cast_to(FLOAT_T);
//        }
//
//        if (i != 0 && prev != current) {
//            any_t = true;
//        }
//
//        prev = current;
//
//        arr->push(res);
//    }
//
//    if (!any_t) {
//        ts.type = prev.type;
//        arr->set_type(ts);
//    }
//
//    if (node->_member_expr == nullptr) {
//        return { arr };
//    }
//    else {
//        current_ref.push_back(std::make_shared<Variant>(arr));
//        return node->_member_expr->accept(*this);
//    }
//}
//
///**
// * Evaluates a literal expression
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(NumericLiteral *node) {
//    if (node->_member_expr != nullptr) {
//        current_ref.push_back(std::make_shared<Variant>(node->_value));
//
//        return *node->_member_expr->accept(*this);
//    }
//    else {
//        return node->_value;
//    }
//}
//
///**
// * Evaluates a string literal (with modifiers)
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(StringLiteral *node) {
//    std::string val = node->value;
//    switch (node->modifier) {
//        // evaluate
//        case 'e': {
//            const std::regex interpolated_variables("\\$\\{(.*?)\\}");
//            std::smatch sm;
//            while (regex_search(val, sm, interpolated_variables)) {
//                std::string match = sm[1];
//
//                Parser p(match);
//                Variant v = p.parse_expression()->accept(*this);
//
//                val = val.substr(0, sm.position()) + v.to_string() + val.substr(sm.position() + sm.length());
//            }
//        }
//        // format
//        case 'f': {
//            const std::regex interpolated_variables("\\$([a-zA-Z_\\$0-9]+)");
//            std::smatch sm;
//            while (regex_search(val, sm, interpolated_variables)) {
//                std::string match = sm[1];
//                Variant v = *context->get_variable(match, node);
//                val = val.substr(0, sm.position()) + v.to_string() + val.substr(sm.position() + sm.length());
//            }
//
//            break;
//        }
//        // none
//        case 'n':
//            break;
//    }
//
//    if (node->_member_expr != nullptr) {
//        current_ref.push_back(std::make_shared<Variant>(val));
//
//        return *node->_member_expr->accept(*this);
//    }
//    else {
//        return { val };
//    }
//}
//
///**
// * Evaluates an array access expression
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(SubscriptExpr *node) {
//    Array::SharedPtr arr = std::dynamic_pointer_cast<Array>(current_ref.back()->get<Object::SharedPtr>());
//    if (arr == nullptr) {
//        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access index of non-array element", node);
//    }
//
//    if (current_ref.back()->is_null()) {
//        RuntimeError::raise(ERR_NULL_POINTER_EXC, "Trying to access index on null", node);
//    }
//
//    current_ref.push_back(arr->at(node->_index->accept(*this).get<int>()));
//
//    if (node->_member_expr != nullptr) {
//        return node->_member_expr->accept(*this);
//    }
//    else {
//        auto return_val = current_ref[current_ref.size() - 1];
//        current_ref.pop_back();
//
//        if (return_val->is_ref()) {
//            return *return_val;
//        }
//        else {
//            return Variant(return_val);
//        }
//    }
//}
//
///**
// * Evaluate a function call on an object
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(CallExpr *node) {
//    if (current_ref.back()->is_null()) {
//        RuntimeError::raise(ERR_NULL_POINTER_EXC, "Trying to call null as a function", node);
//    }
//
//    Function::SharedPtr fun = std::dynamic_pointer_cast<Function>(current_ref.back()->get<Object::SharedPtr>());
//
//    if (fun == nullptr) {
//        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call value of type " + val::typetostr(current_ref.back()
//            ->get_type()), node);
//    }
//
//    std::vector<Variant> _real_args;
//    for (auto arg : node->_arguments) {
//        auto arg_val = arg->accept(*this);
//        _real_args.push_back(arg_val);
//    }
//
//    current_ref.push_back(std::make_shared<Variant>(fun->call(_real_args)));
//
//    if (node->_member_expr != nullptr) {
//        return node->_member_expr->accept(*this);
//    }
//    else {
//        auto return_val = current_ref[current_ref.size() - 1];
//        current_ref.pop_back();
//
//        if (return_val->is_ref()) {
//            return *return_val;
//        }
//        else {
//            return Variant(return_val);
//        }
//    }
//}
//
//Variant EvaluatingVisitor::visit(FunctionCallExpr *) {
//
//}
//
///**
// * Evaluates a method call expression
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(MethodCallExpr *node) {
//    std::vector<Variant> _real_args;
//    for (auto arg : node->_arguments) {
//        auto arg_val = arg->accept(*this);
//        _real_args.push_back(arg_val);
//    }
//
//    if (current_ref.back()->is_null()) {
//        RuntimeError::raise(ERR_NULL_POINTER_EXC, "Trying to call method " + node->_ident + " on null", node);
//    }
//
//    if (current_ref.back()->get_type().type == OBJECT_T) {
//        current_ref.push_back(std::make_shared<Variant>(current_ref.back()->get<Object::SharedPtr>()
//                                                                   ->call_method(node->_ident, _real_args,
//                                                                           context->class_context)));
//    }
//    else if (current_ref.back()->get_type().type == CLASS_T) {
//        current_ref.push_back(std::make_shared<Variant>(current_ref.back()->get<Class*>()
//                                                                   ->call_static_method(node->_ident, _real_args,
//                                                                           context->class_context)));
//    }
//    else if (val::base_class(current_ref.back()->get_type().type) != "") {
//        Class* cl = GlobalContext::get_class(val::base_class(current_ref.back()->get_type().type));
//        Object::SharedPtr class_instance = cl->instantiate({ *current_ref.back() });
//        current_ref.push_back(std::make_shared<Variant>(class_instance->call_method(node->_ident, _real_args,
//                context->class_context)));
//    }
//    else {
//        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call method on primitive value", node);
//    }
//
//    if (node->_member_expr == nullptr) {
//        auto return_val = current_ref[current_ref.size() - 1];
//        current_ref.pop_back();
//
//        if (return_val->is_ref()) {
//            return *return_val;
//        }
//        else {
//            return Variant(return_val);
//        }
//    }
//    else {
//        return node->_member_expr->accept(*this);
//    }
//}
//
///**
// * Evaluate a member access expression on an object
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(MemberRefExpr *node) {
//
//    if (current_ref.back()->is_null()) {
//        RuntimeError::raise(ERR_NULL_POINTER_EXC, "Trying to access property " + node->_ident + " on null", node);
//    }
//
//    // object property access
//    if (current_ref.back()->get_type().type == OBJECT_T) {
//        auto obj = current_ref.back()->get<Object::SharedPtr>();
//        current_ref.push_back(obj->access_property(node->_ident, context->get_class_ctx()));
//    }
//    // static method call
//    else if (current_ref.back()->get_type().type == CLASS_T) {
//        auto cl = current_ref.back()->get<Class*>();
//        current_ref.push_back(std::make_shared<Variant>(cl->access_static_property(node->_ident,
//                context->class_context)));
//    }
//    // autoboxing method call
//    else if (val::base_class(current_ref.back()->get_type().type) != "") {
//        current_ref.push_back(std::make_shared<Variant>(GlobalContext::get_class(val::base_class(current_ref
//                .back()->get_type().type))->access_static_property(node->_ident, context->class_context)));
//    }
//    else {
//        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access property on given value", node);
//    }
//
//    if (node->_member_expr == nullptr) {
//        auto return_val = current_ref[current_ref.size() - 1];
//        current_ref.pop_back();
//
//        if (return_val->is_ref()) {
//            return *return_val;
//        }
//        else {
//            return Variant(return_val);
//        }
//    }
//    else {
//        return node->_member_expr->accept(*this);
//    }
//}
//
///**
// * Evaluates an explicit type cast
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(ExplicitCastExpr *node) {
////    Variant res = node->_child->accept(*this).cast_to(util::typemap[node->_operator]);
////    if (node->_member_expr == nullptr) {
////        return res;
////    }
////    else {
////        current_ref.push_back(std::make_shared<Variant>(res));
////        return *node->_member_expr->accept(*this);
////    }
//
//    return {};
//}
//
///**
// * Evaluates a break statement in a for, switch or while loop
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(BreakStmt *node) {
//    context->break_(node);
//
//    return {};
//}
//
///**
// * Evaluates a continue statement in a for or while loop
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(ContinueStmt *node) {
//    context->continue_(node);
//
//    return {};
//}
//
//Variant EvaluatingVisitor::visit(IfStmt *node) {
//    Variant cond = node->_condition->accept(*this);
//    if (cond.get_type() != BOOL_T) {
//        RuntimeError::raise(ERR_TYPE_ERROR, "Value used as 'if' condition is not boolean", node->_condition.get());
//    }
//
//    if (cond.get<bool>() && node->_if_branch != nullptr) {
//        return node->_if_branch->accept(*this);
//    }
//    else if (!cond.get<bool>() && node->_else_branch != nullptr) {
//        return node->_else_branch->accept(*this);
//    }
//
//    return {};
//}
//
///**
// * Evaluates a switch statement
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(MatchStmt *node) {
//    EvaluatingVisitor ev(*this);
//    ev.context->returnable = context->returnable;
//    ev.context->breakable = true;
//
//    Variant switch_val = node->switch_val->accept(*this);
//    CaseStmt::SharedPtr default_case;
//    // fallthrough
//    bool case_entered = false;
//
//    for (auto case_stmt : node->cases) {
//
//        if (case_stmt->is_default) {
//            default_case = case_stmt;
//            continue;
//        }
//
//        if (!case_entered) {
//            Variant case_val = case_stmt->case_val->accept(ev);
//            if ((case_val == switch_val).get<bool>()) {
//                case_entered = true;
//                case_stmt->accept(ev);
//            }
//        }
//        else {
//            case_stmt->accept(ev);
//        }
//
//        if (ev.context->broken || ev.context->returned) {
//            context->broken = ev.context->broken;
//            context->returned = ev.context->returned;
//            context->return_val = ev.context->return_val;
//            break;
//        }
//    }
//
//    if ((!case_entered || !ev.context->broken) && default_case != nullptr) {
//        default_case->accept(ev);
//    }
//
//    return {};
//}
//
///**
// * Evaluates a case label statement
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(CaseStmt *node) {
//    for (auto stmt : node->_statements) {
//        if (context->broken || context->returned) {
//            break;
//        }
//
//        stmt->accept(*this);
//    }
//
//    return {};
//}
//
///**
// * Evaluates a label statement
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(LabelStmt *node) {
//    context->add_label(node->label_name, node);
//    current_label = node;
//
//    return {};
//}
//
///**
// * Evaluates a goto statement
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(GotoStmt *node) {
//    context->goto_(node->label_name, node);
//    LabelStmt* label = context->get_label(node->label_name);
//    CompoundStmt* cmpnd = label->get_cmpnd();
//
//    int last_index = label->visitor->start_index;
//    label->visitor->start_index = cmpnd->goto_index + 1;
//    label->visitor->context->goto_encountered = false;
//
//    cmpnd->accept(*label->visitor);
//
//    label->visitor->start_index = last_index;
//
//    return {};
//}
//
//
///**
// * Evaluates a function argument declaration expression
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(FuncArgDecl *node) {
//    FuncArg fa;
//    fa.type = node->_arg_type;
//    fa.name = node->_arg_name;
//    fa.default_val = node->_default_val == nullptr ? Variant() : node->_default_val->accept(*this);
//
//    current_arg = fa;
//
//    return {};
//}
//
///**
// * Evaluates a lambda expression
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(LambdaExpr *node) {
//    Function::SharedPtr func = std::make_shared<Function>("anonymous", node->_return_type);
//
//    CaptureVisitor cv(context.get());
//    cv.visit(node->_body.get());
//    cv.capture();
//
//    func->set_body(node->_body);
//    func->set_context(context);
//    func->is_lambda(true);
//
//    for (auto arg : node->_args) {
//        arg->accept(*this);
//        func->add_argument(current_arg.name, current_arg.type, current_arg.default_val);
//    }
//
//    if (node->_member_expr == nullptr) {
//        return {func};
//    }
//    else {
//        current_ref.push_back(std::make_shared<Variant>(func));
//        return node->_member_expr->accept(*this);
//    }
//}
//
///**
// * Evaluates a return statement
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(ReturnStmt *node) {
//    context->return_(node->_return_val->accept(*this), node);
//
//    return {};
//}
//
///**
// * Evaluates an input statement
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(InputStmt *node) {
//    std::string s;
//    std::cin >> s;
//
//    context->get_variable(node->_ident)->strict_equals({ s });
//
//    return { };
//}
//
///**
// * Evaluates an output statement
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(OutputStmt *node) {
//    std::cout << node->_value->accept(*this).to_string(true) << std::endl;
//
//    return { };
//}
//
///**
// * Evaluates a generic expression
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(Expression *node) {
//    return node->_child->accept(*this);
//}
//
//
///**
// * Evaluates a struct declaration
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(StructDecl *node) {
//    EvaluatingVisitor field_visitor;
//    field_visitor.context->class_context = std::string(node->class_name);
//
//    Function::SharedPtr constr;
//    if (node->constr == nullptr) {
//        constr = std::make_shared<Function>("construct", TypeSpecifier());
//    }
//    else {
//        constr = std::static_pointer_cast<Function>(node->constr->accept(field_visitor).get<Object::SharedPtr>());
//    }
//
//    field_visitor.current_class = std::make_unique<Class>(node->class_name, *constr, AccessModifier::PUBLIC, false,
//            true);
//
//    for (auto field : node->fields) {
//        field->accept(field_visitor);
//    }
//
//    for (auto method : node->methods) {
//        method->accept(field_visitor);
//    }
//
//    for (auto op : node->unary_operators) {
//        op.second->accept(field_visitor);
//    }
//
//    for (auto op : node->binary_operators) {
//        op.second->accept(field_visitor);
//    }
//
//    GlobalContext::declare_class(std::move(field_visitor.current_class));
//
//    return {};
//}
//
///**
// * Evaluates a class declaration
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(ClassDecl *node) {
//
//    EvaluatingVisitor field_visitor;
//    field_visitor.context->class_context = std::string(node->class_name);
//
//    if (node->is_abstract) {
//        field_visitor.current_class = std::make_unique<Class>(node->class_name, node->am, true);
//    }
//    else {
//        Function::SharedPtr constr;
//        // evaluate constructor
//        if (node->constr == nullptr) {
//            constr = std::make_shared<Function>("construct", TypeSpecifier());
//        }
//        else {
//            constr = std::static_pointer_cast<Function>(node->constr->accept(field_visitor).get<Object::SharedPtr>());
//        }
//
//        field_visitor.current_class = std::make_unique<Class>(node->class_name, *constr, node->am);
//    }
//
//    for (auto field : node->fields) {
//        field->accept(field_visitor);
//    }
//
//    for (auto method : node->methods) {
//        method->accept(field_visitor);
//    }
//
//    for (auto op : node->unary_operators) {
//        op.second->accept(field_visitor);
//    }
//
//    for (auto op : node->binary_operators) {
//        op.second->accept(field_visitor);
//    }
//
//        if (node->extends != "") {
//            field_visitor.current_class->extend(node->extends);
//        }
//        for (auto impl : node->implements) {
//            field_visitor.current_class->implement(impl);
//        }
//
//        field_visitor.current_class->finalize();
//        GlobalContext::declare_class(std::move(field_visitor.current_class));
//
//    return {};
//}
//
///**
// * Evaluates a class constructor declaration
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(ConstrDecl *node) {
//    auto constr = std::make_shared<Function>("construct", VOID_T);
//    Context::SharedPtr method_ctx = std::make_shared<Context>();
//    method_ctx->class_context = context->class_context;
//    constr->set_context(method_ctx);
//
//    for (auto arg : node->args) {
//        Variant default_val = (arg->_default_val == nullptr) ? Variant() : arg->_default_val->accept(*this);
//        constr->add_argument(arg->_arg_name, arg->_arg_type, default_val);
//    }
//
//    constr->set_body(node->body);
//
//    return { constr };
//}
//
///**
// * Evaluates a class field declaration
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(FieldDecl *node) {
//    if (is_interface) {
//        current_interface->declare_field(node->field_name, node->type, node->is_static);
//        return {};
//    }
//
//    Variant::SharedPtr default_val = (node->default_val == nullptr) ? std::make_shared<Variant>() :
//         std::make_shared<Variant>(node->default_val->accept(*this));
//
//    bool public_class = current_class->get_access_modifier() == AccessModifier::PUBLIC;
//
//    // if class is public and no mutators are specified, generate them by default
//    if (node->hasGetter || (public_class && !node->hasGetter && !node->generate_setter)) {
//        std::string get_name = util::generate_getter_name(node->field_name);
//        const std::string field_name = node->field_name;
//
//        current_class->add_method(get_name, Function([field_name, get_name](Object* this_arg, std::vector<Variant> args)
//                -> Variant {
//            if (args.size() != 0) {
//                RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for function " + get_name + " found");
//            }
//
//            return *this_arg->access_property(field_name, "", true);
//        }, std::vector<TypeSpecifier>{}, TypeSpecifier(node->type)));
//    }
//
//    if (node->generate_setter || (public_class && !node->hasGetter && !node->generate_setter)) {
//        std::string set_name = util::generate_setter_name(node->field_name);
//        const std::string field_name = node->field_name;
//        TypeSpecifier type = node->type;
//
//        current_class->add_method(set_name, Function([field_name, set_name](Object* this_arg,
//                std::vector<Variant>args) -> Variant {
//            if (args.size() != 1) {
//                RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for function " + set_name + " found");
//            }
//
//            this_arg->access_property(field_name, "", true)->strict_equals(args[0]);
//
//            return {};
//        }, std::vector<TypeSpecifier>{type}, TypeSpecifier(VOID_T)));
//    }
//
//    current_class->declare_type(node->field_name, node->type);
//    if (node->is_static) {
//        if (!node->type.nullable && default_val->is_null()) {
//            default_val->set_type(node->type);
//            default_val->set_default();
//
//        }
//        current_class->add_static_property(node->field_name, node->type, default_val, node->am);
//    }
//    else {
//        current_class->add_property(node->field_name, node->type, default_val, node->am);
//    }
//
//    return {};
//}
//
///**
// * Evaluates a class method declaration
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(MethodDecl *node) {
//    if (is_interface) {
//        std::vector<TypeSpecifier> args;
//        for (auto arg : node->args) {
//            arg->accept(*this);
//            args.push_back(current_arg.type);
//        }
//
//        current_interface->declare_method(node->method_name, args, node->nested_type, node->is_static);
//        return {};
//    }
//
//    Function fun(node->method_name, node->nested_type);
//    Context::SharedPtr method_ctx = std::make_shared<Context>();
//    method_ctx->class_context = context->class_context;
//    fun.set_context(method_ctx);
//
//    for (auto arg : node->args) {
//        arg->accept(*this);
//        fun.add_argument(current_arg.name, current_arg.type, current_arg.default_val);
//    }
//
//    fun.set_body(node->body);
//
//    if (node->is_static) {
//        current_class->add_static_method(node->method_name, fun, node->am);
//    }
//    else {
//        current_class->add_method(node->method_name, fun, node->am);
//    }
//
//    return {};
//}
//
///**
// * Evaluates an operator method declaration
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(OperatorDecl *node) {
//    if (is_interface) {
//        std::vector<TypeSpecifier> args;
//        for (auto arg : node->args) {
//            arg->accept(*this);
//            args.push_back(current_arg.type);
//        }
//
//        current_interface->declare_method(node->_operator, args, node->nested_type, false);
//        return {};
//    }
//
//    TypeSpecifier nested_type = node->nested_type.type == VOID_T
//        ? (node->is_binary ? util::binary_op_return_types[node->_operator]
//                           : util::unary_op_return_types[node->_operator])
//        : node->nested_type
//    ;
//
//    Function fun(node->_operator, nested_type);
//    Context::SharedPtr method_ctx = std::make_shared<Context>();
//    method_ctx->class_context = context->class_context;
//    fun.set_context(method_ctx);
//
//    for (auto arg : node->args) {
//        arg->accept(*this);
//        fun.add_argument(current_arg.name, current_arg.type, current_arg.default_val);
//    }
//
//    fun.set_body(node->body);
//
//    if (node->is_binary) {
//        current_class->add_binary_operator(node->_operator, fun, node->am);
//    }
//    else {
//        current_class->add_unary_operator(node->_operator, fun, node->am);
//    }
//
//    return {};
//}
//
//Variant EvaluatingVisitor::visit(UnaryOperator *node) {
//    if (node->_member_expr == nullptr) {
//        return evaluate_unary_op(node);
//    }
//    else {
//        current_ref.push_back(std::make_shared<Variant>(evaluate_unary_op(node)));
//        return *node->_member_expr->accept(*this);
//    }
//}
//
///**
// * Evaluates a unary operator expression
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::evaluate_unary_op(UnaryOperator *node) {
//    std::string _operator = node->_operator;
//
//    Variant result;
//
//    // stateful operators first
//    if (_operator == "++" || _operator == "--") {
//        Variant fst;
//        IdentifierRefExpr::SharedPtr ref = std::dynamic_pointer_cast<IdentifierRefExpr>(node->_child);
//        if (ref != nullptr) {
//            ref->return_ref(true);
//            fst = ref->accept(*this);
//        }
//        else {
//            fst = node->_child->accept(*this);
//        }
//
//        // custom operators
//        if (fst.get_type().type == OBJECT_T) {
//            Object::SharedPtr obj = fst.get<Object::SharedPtr>();
//            if (obj->get_class()->has_unary_operator(_operator)) {
//                return obj->get_class()->call_unary_operator(_operator, obj, context->class_context);
//            }
//        }
//
//        if (!fst.is_ref()) {
//            RuntimeError::raise(ERR_OP_UNDEFINED,
//                    "Cannot apply increment operator to value that is not a reference", node->_child.get());
//        }
//
//        if (node->prefix) {
//            fst.get<Variant::SharedPtr>()->strict_equals(*fst + Variant(_operator == "++" ? 1 : -1));
//
//            return *fst;
//        }
//        else {
//            Variant return_val(*fst);
//            fst.get<Variant::SharedPtr>()->strict_equals(*fst + Variant(_operator == "++" ? 1 : -1));
//
//            return return_val;
//        }
//    }
//    else if (_operator == "new") {
//        IdentifierRefExpr::SharedPtr class_ident = std::dynamic_pointer_cast<IdentifierRefExpr>(node->_child);
//        if (class_ident == nullptr) {
//            RuntimeError::raise(ERR_TYPE_ERROR, "Identifier expected after 'new'", class_ident.get());
//        }
//
//        CallExpr::SharedPtr call = std::dynamic_pointer_cast<CallExpr>(class_ident->_member_expr);
//        std::vector<Variant> args;
//        if (call != nullptr) {
//            for (auto arg : call->_arguments) {
//                args.push_back(arg->accept(*this));
//            }
//        }
//
//        Class* class_ = GlobalContext::get_class(class_ident->_ident);
//
//        if (call == nullptr || call->_member_expr == nullptr) {
//            return {class_->instantiate(args)};
//        }
//        else {
//            current_ref.push_back(std::make_shared<Variant>(class_->instantiate(args)));
//            return call->_member_expr->accept(*this);
//        }
//    }
//    else if (_operator == "&") {
//        IdentifierRefExpr::SharedPtr ident = std::static_pointer_cast<IdentifierRefExpr>(node->_child);
//        ident->return_ref(true);
//
//        Variant fst = ident->accept(*this);
//
//        // custom operators
//        if (fst.get_type().type == OBJECT_T) {
//            Object::SharedPtr obj = fst.get<Object::SharedPtr>();
//            if (obj->get_class()->has_unary_operator(_operator)) {
//                return obj->get_class()->call_unary_operator(_operator, obj, context->class_context);
//            }
//        }
//
//        return fst;
//    }
//
//    Variant child = node->_child->accept(*this);
//
//    if (child.get_type().type == OBJECT_T) {
//        Object::SharedPtr obj = child.get<Object::SharedPtr>();
//        if (obj->get_class()->has_unary_operator(_operator)) {
//            return obj->get_class()->call_unary_operator(_operator, obj, context->class_context);
//        }
//    }
//
//    if (_operator == "typeof") {
//        return child.get_type().to_string();
//    }
//    else if (_operator == "*") {
//        return *child;
//    }
//    else if (_operator == "+") {
//        return child;
//    }
//    else if (_operator == "-") {
//        return -child;
//    }
//    else if (_operator == "!") {
//        return !child;
//    }
//    else if (_operator == "~") {
//        return ~child;
//    }
//
//    RuntimeError::raise(ERR_OP_UNDEFINED, "No definition found for unary operator " + _operator, node);
//}
//
//Variant EvaluatingVisitor::visit(BinaryOperator *node) {
//    if (node->_member_expr == nullptr) {
//        return evaluate_binary_op(node);
//    }
//    else {
//        current_ref.push_back(std::make_shared<Variant>(evaluate_binary_op(node)));
//        return *node->_member_expr->accept(*this);
//    }
//}
//
///**
// * Evaluates a binary operator epxression
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::evaluate_binary_op(BinaryOperator *node) {
//    std::string _operator = node->get_operator();
//    if (!(util::in_vector(util::binary_operators, _operator))) {
//        RuntimeError::raise(ERR_OP_UNDEFINED, "Undefined binary operator " + _operator, node);
//    }
//
//    auto fst = node->_first_child->accept(*this);
//    if (fst.get_type().type == OBJECT_T) {
//        Object::SharedPtr obj = fst.get<Object::SharedPtr>();
//        auto snd = node->_second_child->accept(*this);
//        if (obj->get_class()->has_binary_operator(_operator, snd)) {
//            return obj->get_class()->call_binary_operator(_operator, obj, snd, context->class_context);
//        }
//    }
//
//    if (_operator == "=") {
//        if (!fst.is_ref()) {
//            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot reassign non-reference variable", node->_first_child.get());
//        }
//
//        auto snd = node->_second_child->accept(*this);
//
//        fst.get<Variant::SharedPtr>()->strict_equals(snd);
//
//        return fst;
//    }
//    else if (_operator == "+=") {
//        if (!fst.is_ref()) {
//            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot reassign non-reference variable", node->_first_child.get());
//        }
//
//        auto snd = node->_second_child->accept(*this);
//
//        fst.get<Variant::SharedPtr>()->strict_equals(*fst.get<Variant::SharedPtr>() + snd);
//
//        return fst;
//    }
//    else if (_operator == "-=") {
//        if (!fst.is_ref()) {
//            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot reassign non-reference variable", node->_first_child.get());
//        }
//
//        auto snd = node->_second_child->accept(*this);
//
//        fst.get<Variant::SharedPtr>()->strict_equals(*fst.get<Variant::SharedPtr>() - snd);
//
//        return fst;
//    }
//    else if (_operator == "*=") {
//        if (!fst.is_ref()) {
//            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot reassign non-reference variable", node->_first_child.get());
//        }
//
//        auto snd = node->_second_child->accept(*this);
//
//        fst.get<Variant::SharedPtr>()->strict_equals(*fst.get<Variant::SharedPtr>() * snd);
//
//        return fst;
//    }
//    else if (_operator == "/=") {
//        if (!fst.is_ref()) {
//            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot reassign non-reference variable", node->_first_child.get());
//        }
//
//        auto snd = node->_second_child->accept(*this);
//
//        fst.get<Variant::SharedPtr>()->strict_equals(*fst.get<Variant::SharedPtr>() / snd);
//
//        return fst;
//    }
//    // lazy evaluation
//    else if (_operator == "&&") {
//        if (!fst.get<bool>()) {
//            return false;
//        }
//
//        return node->_second_child->accept(*this);
//    }
//    else if (_operator == "||") {
//        if (fst.get<bool>()) {
//            return true;
//        }
//
//        return node->_second_child->accept(*this);
//    }
//    else if (_operator == "??") {
//        if (fst.is_null()) {
//            return node->_second_child->accept(*this);
//        }
//
//        return fst;
//    }
//
//    auto snd = node->_second_child->accept(*this);
//
//    if (_operator == "==") {
//        return fst == snd;
//    }
//    else if (_operator == "!=") {
//        return fst != snd;
//    }
//    else if (_operator == "<=") {
//        return fst <= snd;
//    }
//    else if (_operator == ">=") {
//        return fst >= snd;
//    }
//    else if (_operator == ">>") {
//        return fst >> snd;
//    }
//    else if (_operator == "<<") {
//        return fst << snd;
//    }
//    else if (_operator == "**") {
//        return fst.pow(snd);
//    }
//    else if (_operator == "..") {
//        Array::SharedPtr arr = std::make_shared<Array>(TypeSpecifier(INT_T), abs((snd - fst).get<int>()) + 1);
//        if ((fst <= snd).get<bool>()) {
//            for (int i = fst.get<int>(); i <= snd.get<int>(); i++) {
//                arr->push({i});
//            }
//        }
//        else {
//            for (int i = fst.get<int>(); i >= snd.get<int>(); i--) {
//                arr->push({i});
//            }
//        }
//
//        return { arr };
//    }
//    else switch (_operator[0]) {
//            case '+': {
//                return fst + snd;
//            }
//            case '-': {
//                return fst - snd;
//            }
//            case '*': {
//                return fst * snd;
//            }
//            case '/': {
//                return fst / snd;
//            }
//            case '%': {
//                return fst % snd;
//            }
//            case '<': {
//                return fst < snd;
//            }
//            case '>': {
//                return fst > snd;
//            }
//            case '&': {
//                return fst & snd;
//            }
//            case '|': {
//                return fst | snd;
//            }
//            case '^': {
//                return fst ^ snd;
//            }
//            default:
//                RuntimeError::raise(ERR_OP_UNDEFINED, "Undefined binary operator " + _operator, node);
//        }
//}
//
///**
// * Evaluates a tertiary operator expression (aka ?:)
// * @param node
// * @return
// */
//Variant EvaluatingVisitor::visit(TertiaryOperator *node) {
//    if (node->condition->accept(*this).get<bool>()) {
//        return node->lhs->accept(*this);
//    }
//    else {
//        return node->rhs->accept(*this);
//    }
//}
//
//Variant EvaluatingVisitor::visit(InterfaceDecl *node) {
//
//}
//
//Variant EvaluatingVisitor::visit(ImplicitCastExpr *) {
//
//}