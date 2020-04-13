#include "cdotc/AST/Attr.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Expression.h"
#include "cdotc/Basic/Variant.h"
#include "cdotc/Support/Casting.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {

llvm::StringRef Attr::getSpelling() const
{
   switch (kind) {
#define CDOT_ATTR(Name, Spelling)                                              \
   case AttrKind::Name:                                                        \
      return #Spelling;
#include "cdotc/AST/Attributes.def"

   default:
      llvm_unreachable("bad attr kind");
   }
}

void Attr::printPretty(llvm::raw_ostream& out) const
{
   switch (kind) {
#define CDOT_ATTR(Name, Spelling)                                              \
   case AttrKind::Name:                                                        \
      return cast<Name##Attr>(this)->printPretty(out);
#include "cdotc/AST/Attributes.def"

   default:
      llvm_unreachable("bad attr kind");
   }
}

Attr* Attr::clone(ast::ASTContext& Ctx) const
{
   switch (kind) {
#define CDOT_ATTR(Name, Spelling)                                              \
   case AttrKind::Name:                                                        \
      return cast<Name##Attr>(this)->clone(Ctx);
#include "cdotc/AST/Attributes.def"

   default:
      llvm_unreachable("bad attr kind");
   }
}

#define CDOT_ATTR_IMPL
#include "cdotc/AST/Attr.inc"

} // namespace cdot