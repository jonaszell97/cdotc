//
// Created by Jonas Zell on 05.01.18.
//

#ifndef CDOT_PRETTYPRINT_H
#define CDOT_PRETTYPRINT_H

#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace ast {

class Expression;
class Statement;

class PrettyPrinterImpl;

class PrettyPrinter {
public:
   enum Options : unsigned {
      None = 0,
   };

   PrettyPrinter(llvm::raw_ostream &out, Options opts = None);
   ~PrettyPrinter();

   void print(const Expression* expr);
   void print(const Statement* stmt);

private:
   PrettyPrinterImpl *pImpl;
};

} // namespace ast
} // namespace cdot


#endif //CDOT_PRETTYPRINT_H
