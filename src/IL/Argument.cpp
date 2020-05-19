#include "cdotc/IL/Argument.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/IL/BasicBlock.h"

namespace cdot {
namespace il {

Argument::Argument(ValueType type, ArgumentConvention Conv, BasicBlock* parent,
                   llvm::StringRef name)
    : Value(ArgumentID, type), parent(parent)
{
   setConvention(Conv);
   this->name = name.str();
}

Argument::Argument(const il::Argument& Arg, il::BasicBlock& Parent)
    : Value(ArgumentID, Arg.type), parent(&Parent)
{
   ArgBits = Arg.ArgBits;
   setName(Arg.getName());
}

} // namespace il
} // namespace cdot