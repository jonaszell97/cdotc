//
// Created by Jonas Zell on 19.11.17.
//

#ifndef CDOT_CASTING_H
#define CDOT_CASTING_H

#include <llvm/Support/Casting.h>

namespace cdot {
namespace support {

using llvm::cast;
using llvm::dyn_cast;
using llvm::isa;
using llvm::cast_or_null;
using llvm::dyn_cast_or_null;

} // namespace support
} // namespace cdot

#endif //CDOT_CASTING_H
