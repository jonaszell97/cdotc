//
// Created by Jonas Zell on 24.06.17.
//

#ifndef CDOT_OPTION_H
#define CDOT_OPTION_H

#include <utility>
#include <map>

namespace cdot {
namespace opt {

enum CompilerOption {
    OPT_MAX_WHILE_COUNT,
    OPT_MAX_RECURSE_DEPTH
};

extern std::map<CompilerOption, int> options;

void set_option(CompilerOption opt, int val);

} // namespace opt
} // namespace cdot


#endif //CDOT_OPTION_H
