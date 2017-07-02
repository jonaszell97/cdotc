//
// Created by Jonas Zell on 24.06.17.
//

#include "Arithmetic.h"
#include "../Util.h"


namespace cdot {
namespace var {

    // binary ops

    Variant Arithmetic::add(Variant v1, Variant v2) {
        if (v1.type.type == STRING_T) {
            return v1.s_val + v2.to_string();
        }
        if (v2.type.type == STRING_T) {
            return v1.to_string() + v2.s_val;
        }
        if (v1.type.type == CHAR_T) {
            return v1.c_val + v2.to_string();
        }
        if (v2.type.type == CHAR_T) {
            return v1.to_string() + v2.c_val;
        }

        switch (v1.type.type) {
            case INT_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.int_val + v2.int_val };
                    case LONG_T:
                        return { v1.int_val + v2.long_val };
                    case FLOAT_T:
                        return { v1.int_val + v2.float_val };
                    case DOUBLE_T:
                        return { v1.int_val + v2.d_val };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.long_val + v2.int_val };
                    case LONG_T:
                        return { v1.long_val + v2.long_val };
                    case FLOAT_T:
                        return { v1.long_val + v2.float_val };
                    case DOUBLE_T:
                        return { v1.long_val + v2.d_val };
                    default:
                        break;
                }
                break;
            case FLOAT_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.float_val + v2.int_val };
                    case LONG_T:
                        return { v1.float_val + v2.long_val };
                    case FLOAT_T:
                        return { v1.float_val + v2.float_val };
                    case DOUBLE_T:
                        return { v1.float_val + v2.d_val };
                    default:
                        break;
                }
                break;
            case DOUBLE_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.d_val + v2.int_val };
                    case LONG_T:
                        return { v1.d_val + v2.long_val };
                    case FLOAT_T:
                        return { v1.d_val + v2.float_val };
                    case DOUBLE_T:
                        return { v1.d_val + v2.d_val };
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot add values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::minus(Variant v1, Variant v2) {
        return add(v1, -v2);
    }

    Variant Arithmetic::multiply(Variant v1, Variant v2) {
        switch (v1.type.type) {
            case INT_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.int_val * v2.int_val };
                    case LONG_T:
                        return { v1.int_val * v2.long_val };
                    case FLOAT_T:
                        return { v1.int_val * v2.float_val };
                    case DOUBLE_T:
                        return { v1.int_val * v2.d_val };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.long_val * v2.int_val };
                    case LONG_T:
                        return { v1.long_val * v2.long_val };
                    case FLOAT_T:
                        return { v1.long_val * v2.float_val };
                    case DOUBLE_T:
                        return { v1.long_val * v2.d_val };
                    default:
                        break;
                }
                break;
            case FLOAT_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.float_val * v2.int_val };
                    case LONG_T:
                        return { v1.float_val * v2.long_val };
                    case FLOAT_T:
                        return { v1.float_val * v2.float_val };
                    case DOUBLE_T:
                        return { v1.float_val * v2.d_val };
                    default:
                        break;
                }
                break;
            case DOUBLE_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.d_val * v2.int_val };
                    case LONG_T:
                        return { v1.d_val * v2.long_val };
                    case FLOAT_T:
                        return { v1.d_val * v2.float_val };
                    case DOUBLE_T:
                        return { v1.d_val * v2.d_val };
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot add values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::divide(Variant v1, Variant v2) {
        switch (v1.type.type) {
            case INT_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.int_val / v2.int_val };
                    case LONG_T:
                        return { v1.int_val / v2.long_val };
                    case FLOAT_T:
                        return { v1.int_val / v2.float_val };
                    case DOUBLE_T:
                        return { v1.int_val / v2.d_val };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.long_val / v2.int_val };
                    case LONG_T:
                        return { v1.long_val / v2.long_val };
                    case FLOAT_T:
                        return { v1.long_val / v2.float_val };
                    case DOUBLE_T:
                        return { v1.long_val / v2.d_val };
                    default:
                        break;
                }
                break;
            case FLOAT_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.float_val / v2.int_val };
                    case LONG_T:
                        return { v1.float_val / v2.long_val };
                    case FLOAT_T:
                        return { v1.float_val / v2.float_val };
                    case DOUBLE_T:
                        return { v1.float_val / v2.d_val };
                    default:
                        break;
                }
                break;
            case DOUBLE_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.d_val / v2.int_val };
                    case LONG_T:
                        return { v1.d_val / v2.long_val };
                    case FLOAT_T:
                        return { v1.d_val / v2.float_val };
                    case DOUBLE_T:
                        return { v1.d_val / v2.d_val };
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot add values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::modulo(Variant v1, Variant v2) {
        switch (v1.type.type) {
            case INT_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.int_val % v2.int_val };
                    case LONG_T:
                        return { v1.int_val % v2.long_val };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { v1.long_val % v2.int_val };
                    case LONG_T:
                        return { v1.long_val % v2.long_val };
                    default:
                        break;
                }
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot apply modulo to values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::pow(Variant v1, Variant v2) {
        switch (v1.type.type) {
            case INT_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { int(std::pow(v1.int_val, v2.int_val)) };
                    case LONG_T:
                        return { int(std::pow(v1.int_val, v2.long_val)) };
                    case FLOAT_T:
                        return { std::pow(v1.int_val, v2.float_val) };
                    case DOUBLE_T:
                        return { std::pow(v1.int_val, v2.d_val) };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { int(std::pow(v1.long_val, v2.int_val)) };
                    case LONG_T:
                        return { int(std::pow(v1.long_val, v2.long_val)) };
                    case FLOAT_T:
                        return { std::pow(v1.long_val, v2.float_val) };
                    case DOUBLE_T:
                        return { std::pow(v1.long_val, v2.d_val) };
                    default:
                        break;
                }
                break;
            case FLOAT_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { std::pow(v1.float_val, v2.int_val) };
                    case LONG_T:
                        return { std::pow(v1.float_val, v2.long_val) };
                    case FLOAT_T:
                        return { std::pow(v1.float_val, v2.float_val) };
                    case DOUBLE_T:
                        return { std::pow(v1.float_val, v2.d_val) };
                    default:
                        break;
                }
                break;
            case DOUBLE_T:
                switch (v2.type.type) {
                    case INT_T:
                        return { std::pow(v1.d_val, v2.int_val) };
                    case LONG_T:
                        return { std::pow(v1.d_val, v2.long_val) };
                    case FLOAT_T:
                        return { std::pow(v1.d_val, v2.float_val) };
                    case DOUBLE_T:
                        return { std::pow(v1.d_val, v2.d_val) };
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot add values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    // unary ops

    Variant Arithmetic::unary_minus(Variant v1) {
        switch (v1.type.type) {
            case INT_T:
                return { - v1.int_val };
                break;
            case LONG_T:
                return { - v1.long_val };
                break;
            case FLOAT_T:
                return { - v1.float_val };
                break;
            case DOUBLE_T:
                return { - v1.d_val };
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot negate value of type.type " + val::typetostr(v1.type.type));
    }

    Variant Arithmetic::unary_not(Variant v1) {
        switch (v1.type.type) {
            case INT_T:
                return { !v1.int_val };
            case LONG_T:
                return { !v1.long_val };
            case FLOAT_T:
                return { !v1.float_val };
            case DOUBLE_T:
                return { !v1.d_val };
            case BOOL_T:
                return { !v1.b_val };
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot negate value of type.type " + val::typetostr(v1.type.type));
    }

    Variant Arithmetic::bitwise_negate(Variant v1) {
        switch (v1.type.type) {
            case INT_T:
                return { ~v1.int_val };
            case LONG_T:
                return { ~v1.long_val };
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot apply bitwise negation to value of type.type " +
                val::typetostr(v1.type.type));
    }

    // equality ops

    Variant Arithmetic::equal(Variant v1, Variant v2) {
        if (v1.type.type == DOUBLE_T && v1.d_val < INT32_MAX) {
            v1.type.type = INT_T;
            v1.int_val = int(v1.d_val);
        }
        if (v2.type.type == DOUBLE_T && v2.d_val < INT32_MAX) {
            v2.type.type = INT_T;
            v2.int_val = int(v2.d_val);
        }
        if (v1.is_null() && v2.is_null()) {
            return true;
        }
        if (v1.type.type != v2.type.type) {
            return false;
        }

        switch (v1.type.type) {
            case INT_T:
                return { v1.int_val == v2.int_val };
            case LONG_T:
                return { v1.long_val == v2.long_val };
            case FLOAT_T:
                return { v1.float_val == v2.float_val };
            case DOUBLE_T:
                return { v1.d_val == v2.d_val };
            case STRING_T:
                return { v1.s_val == v2.s_val };
            case CHAR_T:
                return { v1.c_val == v2.c_val };
            case BOOL_T:
                return { v1.b_val == v2.b_val };
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot compare values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::greater(Variant v1, Variant v2) {
        if (v1.type.type == DOUBLE_T && v1.d_val < INT32_MAX) {
            v1.type.type = INT_T;
            v1.int_val = int(v1.d_val);
        }
        if (v2.type.type == DOUBLE_T && v2.d_val < INT32_MAX) {
            v2.type.type = INT_T;
            v2.int_val = int(v2.d_val);
        }
        if (v1.type.type != v2.type.type) {
            return false;
        }

        switch (v1.type.type) {
            case INT_T:
                return { v1.int_val > v2.int_val };
            case LONG_T:
                return { v1.long_val > v2.long_val };
            case FLOAT_T:
                return { v1.float_val > v2.float_val };
            case DOUBLE_T:
                return { v1.d_val > v2.d_val };
            case CHAR_T:
                return { v1.c_val > v2.c_val };
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot determine larger element between values of type "
            + val::typetostr(v1.type.type) + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::not_equal(Variant v1, Variant v2) {
        return { !equal(v1, v2) };
    }

    Variant Arithmetic::greater_equal(Variant v1, Variant v2) {
        return { equal(v1, v2) || greater(v1, v2) };
    }

    Variant Arithmetic::lower(Variant v1, Variant v2) {
        return { !greater(v1, v2) && !equal(v1, v2) };
    }

    Variant Arithmetic::lower_equal(Variant v1, Variant v2) {
        return { !greater(v1, v2) };
    }

    // logical operators

    Variant Arithmetic::logical_and(Variant v1, Variant v2) {
        if (v1.type.type == BOOL_T && v2.type.type == BOOL_T) {
            return { v1.b_val && v2.b_val };
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot apply logical and to values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::logical_or(Variant v1, Variant v2) {
        if (v1.type.type == BOOL_T && v2.type.type == BOOL_T) {
            return { v1.b_val || v2.b_val };
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot apply logical or to values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    // bitwise operators
    
    Variant Arithmetic::bitwise_and(Variant v1, Variant v2) {
        switch (v1.type.type) {
            case INT_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.int_val & v2.int_val };
                    case LONG_T:
                        return { v1.int_val & v2.long_val };
                    case CHAR_T:
                        return { v1.int_val & v2.c_val };
                    case BOOL_T:
                        return { v1.int_val & v2.b_val };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.long_val & v2.int_val };
                    case LONG_T:
                        return { v1.long_val & v2.long_val };
                    case CHAR_T:
                        return { v1.long_val & v2.c_val };
                    case BOOL_T:
                        return { v1.long_val & v2.b_val };
                    default:
                        break;
                }
                break;
            case CHAR_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.c_val & v2.int_val };
                    case LONG_T:
                        return { v1.c_val & v2.long_val };
                    case CHAR_T:
                        return { v1.c_val & v2.c_val };
                    case BOOL_T:
                        return { v1.c_val & v2.b_val };
                    default:
                        break;
                }
                break;
            case BOOL_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.b_val & v2.int_val };
                    case LONG_T:
                        return { v1.b_val & v2.long_val };
                    case CHAR_T:
                        return { v1.b_val & v2.c_val };
                    case BOOL_T:
                        return { v1.b_val & v2.b_val };
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot apply bitwise and to values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::bitwise_or(Variant v1, Variant v2) {
        switch (v1.type.type) {
            case INT_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.int_val | v2.int_val };
                    case LONG_T:
                        return { v1.int_val | v2.long_val };
                    case CHAR_T:
                        return { v1.int_val | v2.c_val };
                    case BOOL_T:
                        return { v1.int_val | v2.b_val };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.long_val | v2.int_val };
                    case LONG_T:
                        return { v1.long_val | v2.long_val };
                    case CHAR_T:
                        return { v1.long_val | v2.c_val };
                    case BOOL_T:
                        return { v1.long_val | v2.b_val };
                    default:
                        break;
                }
                break;
            case CHAR_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.c_val | v2.int_val };
                    case LONG_T:
                        return { v1.c_val | v2.long_val };
                    case CHAR_T:
                        return { v1.c_val | v2.c_val };
                    case BOOL_T:
                        return { v1.c_val | v2.b_val };
                    default:
                        break;
                }
                break;
            case BOOL_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.b_val | v2.int_val };
                    case LONG_T:
                        return { v1.b_val | v2.long_val };
                    case CHAR_T:
                        return { v1.b_val | v2.c_val };
                    case BOOL_T:
                        return { v1.b_val | v2.b_val };
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot apply bitwise or to values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::bitwise_xor(Variant v1, Variant v2) {
        switch (v1.type.type) {
            case INT_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.int_val ^ v2.int_val };
                    case LONG_T:
                        return { v1.int_val ^ v2.long_val };
                    case CHAR_T:
                        return { v1.int_val ^ v2.c_val };
                    case BOOL_T:
                        return { v1.int_val ^ v2.b_val };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.long_val ^ v2.int_val };
                    case LONG_T:
                        return { v1.long_val ^ v2.long_val };
                    case CHAR_T:
                        return { v1.long_val ^ v2.c_val };
                    case BOOL_T:
                        return { v1.long_val ^ v2.b_val };
                    default:
                        break;
                }
                break;
            case CHAR_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.c_val ^ v2.int_val };
                    case LONG_T:
                        return { v1.c_val ^ v2.long_val };
                    case CHAR_T:
                        return { v1.c_val ^ v2.c_val };
                    case BOOL_T:
                        return { v1.c_val ^ v2.b_val };
                    default:
                        break;
                }
                break;
            case BOOL_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.b_val ^ v2.int_val };
                    case LONG_T:
                        return { v1.b_val ^ v2.long_val };
                    case CHAR_T:
                        return { v1.b_val ^ v2.c_val };
                    case BOOL_T:
                        return { v1.b_val ^ v2.b_val };
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot apply bitwise and to values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::bitshift_left(Variant v1, Variant v2) {
        switch (v1.type.type) {
            case INT_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.int_val << v2.int_val };
                    case LONG_T:
                        return { v1.int_val << v2.long_val };
                    case CHAR_T:
                        return { v1.int_val << v2.c_val };
                    case BOOL_T:
                        return { v1.int_val << v2.b_val };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.long_val << v2.int_val };
                    case LONG_T:
                        return { v1.long_val << v2.long_val };
                    case CHAR_T:
                        return { v1.long_val << v2.c_val };
                    case BOOL_T:
                        return { v1.long_val << v2.b_val };
                    default:
                        break;
                }
                break;
            case CHAR_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.c_val << v2.int_val };
                    case LONG_T:
                        return { v1.c_val << v2.long_val };
                    case CHAR_T:
                        return { v1.c_val << v2.c_val };
                    case BOOL_T:
                        return { v1.c_val << v2.b_val };
                    default:
                        break;
                }
                break;
            case BOOL_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.b_val << v2.int_val };
                    case LONG_T:
                        return { v1.b_val << v2.long_val };
                    case CHAR_T:
                        return { v1.b_val << v2.c_val };
                    case BOOL_T:
                        return { v1.b_val << v2.b_val };
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot apply left bitshift to values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

    Variant Arithmetic::bitshift_right(Variant v1, Variant v2) {
        switch (v1.type.type) {
            case INT_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.int_val >> v2.int_val };
                    case LONG_T:
                        return { v1.int_val >> v2.long_val };
                    case CHAR_T:
                        return { v1.int_val >> v2.c_val };
                    case BOOL_T:
                        return { v1.int_val >> v2.b_val };
                    default:
                        break;
                }
                break;
            case LONG_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.long_val >> v2.int_val };
                    case LONG_T:
                        return { v1.long_val >> v2.long_val };
                    case CHAR_T:
                        return { v1.long_val >> v2.c_val };
                    case BOOL_T:
                        return { v1.long_val >> v2.b_val };
                    default:
                        break;
                }
                break;
            case CHAR_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.c_val >> v2.int_val };
                    case LONG_T:
                        return { v1.c_val >> v2.long_val };
                    case CHAR_T:
                        return { v1.c_val >> v2.c_val };
                    case BOOL_T:
                        return { v1.c_val >> v2.b_val };
                    default:
                        break;
                }
                break;
            case BOOL_T:
                switch (v1.type.type) {
                    case INT_T:
                        return { v1.b_val >> v2.int_val };
                    case LONG_T:
                        return { v1.b_val >> v2.long_val };
                    case CHAR_T:
                        return { v1.b_val >> v2.c_val };
                    case BOOL_T:
                        return { v1.b_val >> v2.b_val };
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot apply right bitshift to values of type " + val::typetostr(v1.type.type)
                                          + " and " + val::typetostr(v2.type.type));
    }

} // namespace var
} // namespace cdot
