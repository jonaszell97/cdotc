//
// Created by Jonas Zell on 30.04.18.
//

#ifndef CDOT_ACCESSPATHDESCRIPTOR_H
#define CDOT_ACCESSPATHDESCRIPTOR_H

#include <string>

namespace cdot {
namespace ast {
   class ILGenPass;
} // namespace ast

namespace il {

class Instruction;
class Value;

std::string getAccessPathDescriptor(ast::ILGenPass &ILGen,
                                    const Value &I);

} // namespace il
} // namespace cdot

#endif //CDOT_ACCESSPATHDESCRIPTOR_H
