#include "SourceLocation.h"

#include <llvm/Support/raw_ostream.h>

llvm::raw_ostream &cdot::operator<<(llvm::raw_ostream &OS,
                                    const FullSourceLoc &loc) {
   OS << loc.getSourceFileName()
      << ":" << loc.getLine()
      << ":" << loc.getColumn();

   return OS;
}