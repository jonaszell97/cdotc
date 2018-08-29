//
// Created by Jonas Zell on 26.11.17.
//

#include "CastKind.h"

namespace cdot {

const char* CastNames[] = {
   "<noop>", "<lvalue_to_rvalue>",
   "inttofp", "fptoint", "ext",
   "trunc", "ptrtoint", "inttoptr", "sign_cast", "is_null",
   "fpext", "fptrunc",
   "dyn_cast", "upcast", "<conv_op>",
   "bitcast", "proto_wrap", "proto_unwrap",
   "existential_cast", "try_existential_cast",
   "existential_unwrap", "try_existential_unwrap", "existential_ref",
   "nothrow_to_throw", "thin_to_thick",
   "<meta_type_cast>",
   "<forward>", "<move>", "<copy>",
   "mut_ref_to_ref", "mut_ptr_to_ptr",
   "<rvalue_to_const_ref>",
   "int_to_enum", "enum_to_int", "<to_void>", "<to_()>", "<to_meta_type>"
};

} // namespace cdot