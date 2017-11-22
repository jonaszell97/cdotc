//
// Created by Jonas Zell on 19.06.17.
//

#include "IdentifierRefExpr.h"
#include "../../../Variant/Type/Generic.h"

namespace cdot {

unordered_map<string, BuiltinIdentifier> builtinIdentifiers = {
   { "__func__", BuiltinIdentifier::FUNC },
   { "__mangled_func__", BuiltinIdentifier::MANGLED_FUNC },
   { "__f64_sig_nan", BuiltinIdentifier::DOUBLE_SNAN },
   { "__f32_sig_nan", BuiltinIdentifier::FLOAT_SNAN },
   { "__f64_q_nan", BuiltinIdentifier::DOUBLE_QNAN },
   { "__f32_q_nan", BuiltinIdentifier::DOUBLE_QNAN }
};

namespace ast {

IdentifierRefExpr::IdentifierRefExpr(string &&ident)
   : Expression(IdentifierRefExprID),
     templateArgs(new ResolvedTemplateArgList({})),
     is_let_expr(false),
     is_var_expr(false),
     is_namespace(false),
     is_super(false),
     is_self(false),
     is_function(false),
     is_metatype(false),
     functionArg(false)
{
   this->ident = std::move(ident);
}

void IdentifierRefExpr::setTemplateArgs(TemplateArgList *templateArgs)
{
   delete IdentifierRefExpr::templateArgs;
   IdentifierRefExpr::templateArgs = templateArgs;
}

} // namespace ast
} // namespace cdot