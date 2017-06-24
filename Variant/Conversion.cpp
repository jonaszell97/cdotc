//
// Created by Jonas Zell on 24.06.17.
//

#include "Conversion.h"
#include "../Util.h"

namespace cdot {
namespace var {

    Variant Converter::cast(Variant v, ValueType target_type) {
        if (v.type == target_type || target_type == ANY_T) {
            return v;
        }
        if (v.type == REF_T) {
            return v.ref->cast_to(target_type);
        }

        switch (v.type) {
            case STRING_T: {
                if (target_type == CHAR_T) {
                    if (v.s_val.length() > 1) {
                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to char");
                    }

                    v.c_val = v.s_val.at(0);
                }
                else if (target_type == INT_T) {
                    try {
                        v.int_val = std::stoi(v.s_val);
                    } catch(std::invalid_argument) {
                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to number");
                    }
                }
                else if (target_type == LONG_T) {
                    try {
                        v.long_val = std::stol(v.s_val);
                    } catch(std::invalid_argument) {
                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to number");
                    }
                }
                else if (target_type == FLOAT_T) {
                    try {
                        v.float_val = std::stof(v.s_val);
                    } catch(std::invalid_argument) {
                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to number");
                    }
                }
                else if (target_type == DOUBLE_T) {
                    try {
                        v.d_val = std::stod(v.s_val);
                    } catch(std::invalid_argument) {
                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to number");
                    }
                }
                else if (target_type == BOOL_T) {
                    v.b_val = v.s_val != "false" && v.s_val != "0";
                }
                else {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of v.type " + util::types[v.type]
                                                      + " to " + util::types[target_type]);
                }

                break;
            }
            case CHAR_T: {
                if (target_type == STRING_T) {
                    v.s_val = std::string(1, v.c_val);
                }
                else if (target_type == INT_T) {
                    v.int_val = v.c_val;
                }
                else if (target_type == LONG_T) {
                    v.long_val = v.c_val;
                }
                else if (target_type == FLOAT_T) {
                    v.float_val = v.c_val;
                }
                else if (target_type == DOUBLE_T) {
                    v.d_val = v.c_val;
                }
                else if (target_type == BOOL_T) {
                    v.b_val = v.c_val != '0';
                }
                else {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of v.type " + util::types[v.type]
                                                      + " to " + util::types[target_type]);
                }

                break;
            }
            case INT_T:
            case LONG_T: {
                if (target_type == STRING_T) {
                    v.s_val = std::to_string(v.type == INT_T ? v.int_val : v.long_val);
                }
                else if (target_type == CHAR_T) {
                    if ((v.type == INT_T ? v.int_val : v.long_val) > UINT8_MAX) {
                        RuntimeError::raise(ERR_VAL_TOO_LARGE, "Value " + std::to_string(v.type == INT_T ? v.int_val : v.long_val)
                                                               + " is too large for v.type char");
                    }

                    v.c_val = char(v.type == INT_T ? v.int_val : v.long_val);
                }
                else if (target_type == BOOL_T) {
                    v.b_val = (v.type == INT_T ? v.int_val : v.long_val) != 0;
                }
                else if (target_type == FLOAT_T) {
                    v.float_val = (v.type == INT_T ? v.int_val : v.long_val);
                }
                else if (target_type == DOUBLE_T) {
                    v.d_val = (v.type == INT_T ? v.int_val : v.long_val);
                }
                else {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of v.type " + util::types[v.type]
                                                      + " to " + util::types[target_type]);
                }

                break;
            }
            case FLOAT_T:
            case DOUBLE_T:
                if (target_type == STRING_T) {
                    v.s_val = std::to_string(v.type == FLOAT_T ? v.float_val : v.d_val);
                }
                else if (target_type == CHAR_T) {
                    if ((v.type == FLOAT_T ? v.float_val : v.d_val) > UINT8_MAX) {
                        RuntimeError::raise(ERR_VAL_TOO_LARGE, "Value "
                                                               + std::to_string(v.type == FLOAT_T ? v.float_val : v.d_val)
                                                               + " is too large for v.type char");
                    }

                    v.c_val = char(v.type == FLOAT_T ? v.float_val : v.d_val);
                }
                else if (target_type == BOOL_T) {
                    v.b_val = (v.type == FLOAT_T ? v.float_val : v.d_val) != 0;
                }
                else if (target_type == FLOAT_T) {
                    v.float_val = (v.type == FLOAT_T ? v.float_val : float(v.d_val));
                }
                else if (target_type == DOUBLE_T) {
                    v.d_val = (v.type == FLOAT_T ? v.float_val : v.d_val);
                }
                else {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of v.type " + util::types[v.type]
                                                      + " to " + util::types[target_type]);
                }
                break;
            case BOOL_T: {
                if (target_type == STRING_T) {
                    v.s_val = (v.b_val ? "true" : "false");
                }
                else if( target_type == CHAR_T) {
                    v.c_val = v.b_val ? '1' : '0';
                }
                else if (target_type == INT_T) {
                    v.int_val = v.b_val;
                }
                else if (target_type == LONG_T) {
                    v.long_val = v.b_val;
                }
                else if (target_type == FLOAT_T) {
                    v.float_val = v.b_val;
                }
                else if (target_type == DOUBLE_T) {
                    v.d_val = v.b_val;
                }
                else {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of v.type " + util::types[v.type]
                                                      + " to " + util::types[target_type]);
                }

                break;
            }
            default:
                RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of v.type " + util::types[v.type]
                                                  + " to " + util::types[target_type]);
        }

        v.destroy();
        v.type = target_type;
        v.check_numeric();

        return v;
    }

} // namespace var
} // namespace cdot