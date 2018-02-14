//
// Created by Jonas Zell on 19.06.17.
//

#include "IdentifierRefExpr.h"

namespace cdot {

std::unordered_map<string, BuiltinIdentifier> builtinIdentifiers = {
   { "__nullptr", BuiltinIdentifier::NULLPTR },
   { "__func__", BuiltinIdentifier::FUNC },
   { "__mangled_func__", BuiltinIdentifier::MANGLED_FUNC },
   { "__f64_sig_nan", BuiltinIdentifier::DOUBLE_SNAN },
   { "__f32_sig_nan", BuiltinIdentifier::FLOAT_SNAN },
   { "__f64_q_nan", BuiltinIdentifier::DOUBLE_QNAN },
   { "__f32_q_nan", BuiltinIdentifier::DOUBLE_QNAN },
   { "__ctfe", BuiltinIdentifier::__ctfe }
};

} // namespace cdot