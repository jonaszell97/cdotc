//
// Created by Jonas Zell on 19.06.17.
//

#include "IdentifierRefExpr.h"

namespace cdot {
   unordered_map<string, BuiltinIdentifier> builtinIdentifiers = {
     { "__func__", BuiltinIdentifier::FUNC },
     { "__mangled_func__", BuiltinIdentifier::MANGLED_FUNC },
     { "__f64_sig_nan", BuiltinIdentifier::DOUBLE_SNAN },
     { "__f32_sig_nan", BuiltinIdentifier::FLOAT_SNAN },
     { "__f64_q_nan", BuiltinIdentifier::DOUBLE_QNAN },
     { "__f32_q_nan", BuiltinIdentifier::DOUBLE_QNAN }
   };
}

IdentifierRefExpr::IdentifierRefExpr(string ident) {
   this->ident = ident;
}

std::vector<AstNode::SharedPtr> IdentifierRefExpr::get_children() {
   if (memberExpr != nullptr) {
      return { memberExpr };
   }
   else {
      return { };
   }
}

void IdentifierRefExpr::__dump(int depth) {
   AstNode::__tab(depth);

   std::cout << "Identifier" << "Expr" << " [" << ident << "]" << std::endl;

   if (memberExpr != nullptr) {
      memberExpr->__dump(depth + 1);
   }
}