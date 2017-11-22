//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTANT_H
#define CDOT_CONSTANT_H


#include "../Value.h"

namespace cdot {
namespace il {

class Constant: public Value {
public:
   static bool classof(Constant const* T) { return true; }
   static bool classof(Value const* T) {
      switch (T->getTypeID()) {
#     define CDOT_INSTRUCTION(Name) \
         case Name##ID:
#     define CDOT_INCLUDE_CONSTANTS
#     define CDOT_INCLUDE_CONSTEXPRS
#     include "../Instructions.def"
            return true;
         default:
            return false;
      }
   }

protected:
   Constant(TypeID id, Type *ty,
            const std::string &name = "",
            const SourceLocation &loc = {});

   Constant(TypeID id, ILType ty,
            const std::string &name = "",
            const SourceLocation &loc = {});
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANT_H
