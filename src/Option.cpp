//
// Created by Jonas Zell on 24.06.17.
//

#include "Option.h"


namespace cdot {
namespace opt {

std::map<CompilerOption, int> options = {
        {OPT_MAX_RECURSE_DEPTH, 1000},
        {OPT_MAX_WHILE_COUNT, 6000}
};

void set_option(CompilerOption opt, int val) {
    if (options.find(opt) != options.end()) {
        options[opt] = val;
    }
    else {
        options.insert(std::pair<CompilerOption, int>(opt, val));
    }
}

} // namespace opt
} // namespace cdot