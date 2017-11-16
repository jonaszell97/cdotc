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
}

IdentifierRefExpr::IdentifierRefExpr(string &&ident)
   : templateArgs(new ResolvedTemplateArgList({}))
{
   this->ident = std::move(ident);
}

IdentifierRefExpr::~IdentifierRefExpr()
{
   delete templateArgs;
}

void IdentifierRefExpr::setTemplateArgs(TemplateArgList *templateArgs)
{
   delete IdentifierRefExpr::templateArgs;
   IdentifierRefExpr::templateArgs = templateArgs;
}

std::vector<AstNode::SharedPtr> IdentifierRefExpr::get_children() {
   if (memberExpr != nullptr) {
      return { memberExpr };
   }
   else {
      return { };
   }
}