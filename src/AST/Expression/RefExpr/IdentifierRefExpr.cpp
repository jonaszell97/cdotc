//
// Created by Jonas Zell on 19.06.17.
//

#include "IdentifierRefExpr.h"
#include "../../../Variant/Type/Generic.h"

namespace cdot {

unordered_map<string, BuiltinIdentifier> builtinIdentifiers = {
   { "__nullptr", BuiltinIdentifier::NULLPTR },
   { "__func__", BuiltinIdentifier::FUNC },
   { "__mangled_func__", BuiltinIdentifier::MANGLED_FUNC },
   { "__f64_sig_nan", BuiltinIdentifier::DOUBLE_SNAN },
   { "__f32_sig_nan", BuiltinIdentifier::FLOAT_SNAN },
   { "__f64_q_nan", BuiltinIdentifier::DOUBLE_QNAN },
   { "__f32_q_nan", BuiltinIdentifier::DOUBLE_QNAN },
   { "__ctfe", BuiltinIdentifier::__ctfe }
};

namespace ast {

IdentifierRefExpr::IdentifierRefExpr(string &&ident)
   : IdentifiedExpr(IdentifierRefExprID, move(ident)),
     is_let_expr(false),
     is_var_expr(false),
     is_namespace(false),
     is_super(false),
     is_self(false),
     is_function(false),
     is_metatype(false),
     functionArg(false),
     is_capture(false),
     is_alias(false)
{

}

IdentifierRefExpr::~IdentifierRefExpr() = default;

NonTypeTemplateArgExpr::NonTypeTemplateArgExpr(const TemplateParameter &Param)
   : Expression(NonTypeTemplateArgExprID), Param(Param)
{
   assert(Param.kind == TemplateParameter::Value);
   setIsValueDependent(true);
}

NonTypeTemplateArgExpr::~NonTypeTemplateArgExpr() = default;

} // namespace ast
} // namespace cdot