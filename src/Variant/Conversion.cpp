////
//// Created by Jonas Zell on 24.06.17.
////
//
//#include "Conversion.h"
//#include "../Util.h"
//
//namespace cdot {
//namespace var {
//
//    Variant Converter::cast(Variant v, ValueType target_type) {
//        if (v._type.type == target_type || target_type == ANY_T) {
//            return v;
//        }
//
//        switch (v._type.type) {
//            case STRING_T: {
//                if (target_type == CHAR_T) {
//                    if (v.s_val.length() > 1) {
//                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to char");
//                    }
//
//                    v.c_val = v.s_val.at(0);
//                }
//                else if (target_type == INT_T) {
//                    try {
//                        v.int_val = std::stoi(v.s_val);
//                    } catch(std::invalid_argument) {
//                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to number");
//                    }
//                }
//                else if (target_type == LONG_T) {
//                    try {
//                        v.long_val = std::stol(v.s_val);
//                    } catch(std::invalid_argument) {
//                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to number");
//                    }
//                }
//                else if (target_type == FLOAT_T) {
//                    try {
//                        v.float_val = std::stof(v.s_val);
//                    } catch(std::invalid_argument) {
//                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to number");
//                    }
//                }
//                else if (target_type == DOUBLE_T) {
//                    try {
//                        v.d_val = std::stod(v.s_val);
//                    } catch(std::invalid_argument) {
//                        RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + v.s_val + "' to number");
//                    }
//                }
//                else if (target_type == BOOL_T) {
//                    v.b_val = v.s_val != "false" && v.s_val != "0";
//                }
//                else {
//                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + val::typetostr(v._type.type)
//                                                      + " to " + val::typetostr(target_type));
//                }
//
//                break;
//            }
//            case CHAR_T: {
//                if (target_type == STRING_T) {
//                    v._type.computed_length = 1;
//                    v.s_val = std::string(1, v.c_val);
//                }
//                else if (target_type == INT_T) {
//                    v.int_val = v.c_val;
//                }
//                else if (target_type == LONG_T) {
//                    v.long_val = v.c_val;
//                }
//                else if (target_type == FLOAT_T) {
//                    v.float_val = v.c_val;
//                }
//                else if (target_type == DOUBLE_T) {
//                    v.d_val = v.c_val;
//                }
//                else if (target_type == BOOL_T) {
//                    v.b_val = v.c_val != '0';
//                }
//                else {
//                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + val::typetostr(v._type.type)
//                                                      + " to " + val::typetostr(target_type));
//                }
//
//                break;
//            }
//            case INT_T:
//            case LONG_T: {
//                if (target_type == STRING_T) {
//                    v.s_val = std::to_string(v._type.type == INT_T ? v.int_val : v.long_val);
//                    v._type.computed_length = v.s_val.length();
//                }
//                else if (target_type == CHAR_T) {
//                    if ((v._type.type == INT_T ? v.int_val : v.long_val) > UINT8_MAX) {
//                        RuntimeError::raise(ERR_VAL_TOO_LARGE, "Value " + std::to_string(v._type.type == INT_T ? v.int_val : v.long_val)
//                                                               + " is too large for type char");
//                    }
//
//                    v.c_val = char(v._type.type == INT_T ? v.int_val : v.long_val);
//                }
//                else if (target_type == BOOL_T) {
//                    v.b_val = (v._type.type == INT_T ? v.int_val : v.long_val) != 0;
//                }
//                else if (target_type == FLOAT_T) {
//                    v.float_val = (v._type.type == INT_T ? v.int_val : v.long_val);
//                }
//                else if (target_type == DOUBLE_T) {
//                    v.d_val = (v._type.type == INT_T ? v.int_val : v.long_val);
//                }
//                else {
//                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type.type " + val::typetostr(v._type.type)
//                                                      + " to " + val::typetostr(target_type));
//                }
//
//                break;
//            }
//            case FLOAT_T:
//            case DOUBLE_T:
//                if (target_type == STRING_T) {
//                    v.s_val = std::to_string(v._type.type == FLOAT_T ? v.float_val : v.d_val);
//                    v._type.computed_length = v.s_val.length();
//                }
//                else if (target_type == CHAR_T) {
//                    if ((v._type.type == FLOAT_T ? v.float_val : v.d_val) > UINT8_MAX) {
//                        RuntimeError::raise(ERR_VAL_TOO_LARGE, "Value "
//                                                               + std::to_string(v._type.type == FLOAT_T ? v.float_val : v.d_val)
//                                                               + " is too large for type char");
//                    }
//
//                    v.c_val = char(v._type.type == FLOAT_T ? v.float_val : v.d_val);
//                }
//                else if (target_type == BOOL_T) {
//                    v.b_val = (v._type.type == FLOAT_T ? v.float_val : v.d_val) != 0;
//                }
//                else if (target_type == FLOAT_T) {
//                    v.float_val = (v._type.type == FLOAT_T ? v.float_val : float(v.d_val));
//                }
//                else if (target_type == INT_T) {
//                    v.int_val = int(v._type.type == FLOAT_T ? v.float_val : v.d_val);
//                }
//                else if (target_type == LONG_T) {
//                    v.long_val = long(v._type.type == FLOAT_T ? v.float_val : v.d_val);
//                }
//                else if (target_type == DOUBLE_T) {
//                    v.d_val = (v._type.type == FLOAT_T ? v.float_val : v.d_val);
//                }
//                else {
//                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + val::typetostr(v._type.type)
//                                                      + " to " + val::typetostr(target_type));
//                }
//                break;
//            case BOOL_T: {
//                if (target_type == STRING_T) {
//                    v.s_val = (v.b_val ? "true" : "false");
//                    v._type.computed_length = v.s_val.length();
//                }
//                else if( target_type == CHAR_T) {
//                    v.c_val = v.b_val ? '1' : '0';
//                }
//                else if (target_type == INT_T) {
//                    v.int_val = v.b_val;
//                }
//                else if (target_type == LONG_T) {
//                    v.long_val = v.b_val;
//                }
//                else if (target_type == FLOAT_T) {
//                    v.float_val = v.b_val;
//                }
//                else if (target_type == DOUBLE_T) {
//                    v.d_val = v.b_val;
//                }
//                else {
//                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + val::typetostr(v._type.type)
//                                                      + " to " + val::typetostr(target_type));
//                }
//
//                break;
//            }
//            default:
//                RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + val::typetostr(v._type.type)
//                                                  + " to " + val::typetostr(target_type));
//        }
//
//        v._type.type = target_type;
//        v.check_numeric();
//
//        return v;
//    }
//
//} // namespace var
//} // namespace cdot