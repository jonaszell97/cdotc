//
// Created by Jonas Zell on 16.03.18.
//

#include "Attr.h"

#include "AST/ASTContext.h"
#include "AST/Expression.h"
#include "Support/Casting.h"
#include "Variant/Variant.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {

llvm::StringRef Attr::getSpelling() const
{
   switch (kind) {
#     define CDOT_ATTR(Name, Spelling)        \
      case AttrKind::Name: return #Spelling;
#     include "Attributes.def"

   default:
      llvm_unreachable("bad attr kind");
   }
}

void Attr::printPretty(llvm::raw_ostream &out) const
{
   switch (kind) {
#  define CDOT_ATTR(Name, Spelling)                                           \
      case AttrKind::Name: return cast<Name##Attr>(this)->printPretty(out);
#  include "Attributes.def"

   default:
      llvm_unreachable("bad attr kind");
   }
}

Attr* Attr::clone(ast::ASTContext &Ctx) const
{
   switch (kind) {
#  define CDOT_ATTR(Name, Spelling)                                           \
      case AttrKind::Name: return cast<Name##Attr>(this)->clone(Ctx);
#  include "Attributes.def"

   default:
      llvm_unreachable("bad attr kind");
   }
}

#define CDOT_ATTR_IMPL
#include "Attr.inc"

} // namespace cdot