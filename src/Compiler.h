//
// Created by Jonas Zell on 03.09.17.
//

#ifndef CDOT_CDOT_H
#define CDOT_CDOT_H

#include <string>
#include <vector>

using std::string;

namespace cdot {

   class Compiler {
   public:
      Compiler(int argc, char *argv[]);
      void compile();

   protected:
      std::vector<string> sourceFiles;
      string compilerLocation;

      bool astDump = false;
      bool linkStdLib = true;
   };

}

#endif //CDOT_CDOT_H
