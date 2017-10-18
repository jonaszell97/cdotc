//
// Created by Jonas Zell on 04.10.17.
//

#ifndef CDOT_DIAGNOSTICPARSER_H
#define CDOT_DIAGNOSTICPARSER_H

#include <fstream>

using std::string;

namespace cdot {
namespace diag {

class DiagnosticParser {
public:
   DiagnosticParser() = default;
   void doParse();

protected:
   void parseFile(std::ifstream& file, string& base);
};

}
}

#endif //CDOT_DIAGNOSTICPARSER_H
