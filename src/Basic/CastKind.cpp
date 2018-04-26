//
// Created by Jonas Zell on 26.11.17.
//

#include "CastKind.h"

namespace cdot {

const char* CastNames[] = {
   "<noop>", "<invalid>", "<lvalue_to_rvalue>",
   "ibox", "iunbox", "inttofp", "fptoint", "ext",
   "trunc", "ptrtoint", "inttoptr", "sign_cast",
   "fpext", "fptrunc", "fpbox", "fpunbox",
   "dyn_cast", "upcast", "<conv_op>", "bitcast", "proto_wrap", "proto_unwrap",
   "nothrow_to_throw", "mut_ref_to_ref", "mut_ptr_to_ptr",
   "<int_to_enum>", "<enum_to_int>"
};

} // namespace cdot