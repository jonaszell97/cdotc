#include "cdotc/CTFE/Value.h"

using std::string;
using namespace cdot::ast;

namespace cdot {
namespace ctfe {

std::string Value::getString() const { return string(buffer); }

} // namespace ctfe
} // namespace cdot