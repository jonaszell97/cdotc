//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_ARGUMENT_H
#define CDOT_ARGUMENT_H

#include "../Value.h"

namespace cdot {
namespace il {

class Function;

class Argument: public Value {
public:
   Argument(Type *type,
            bool vararg,
            Function *parent,
            const std::string &name = "",
            const SourceLocation &loc = {});

   Argument(ILType type,
            bool vararg,
            Function *parent,
            const std::string &name = "",
            const SourceLocation &loc = {});

   Function *getParent() const
   {
      return parent;
   }

   bool isVararg() const;

protected:
   Function *parent;
   bool vararg = false;

public:
   static bool classof(Argument const* T) { return true; }
   static inline bool classof(Value const* T) {
     switch(T->getTypeID()) {
        case ArgumentID:
           return true;
        default:
           return false;
     }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_ARGUMENT_H
