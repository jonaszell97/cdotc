//
// Created by Jonas Zell on 23.12.17.
//

#ifndef CDOT_CONFORMANCECHECKER_H
#define CDOT_CONFORMANCECHECKER_H

#include <llvm/ADT/ArrayRef.h>

namespace cdot {

namespace cl {

class Record;

} // namespace cl

namespace diag {

class DiagnosticBuilder;

} // namespace diag

namespace ast {

class SemaPass;

} // namespace ast

namespace sema {

class ConformanceCheckerImpl;

class ConformanceChecker {
public:
   explicit ConformanceChecker(ast::SemaPass &SP, cl::Record *Rec);
   ~ConformanceChecker();

   void checkConformance();
   llvm::SmallVector<diag::DiagnosticBuilder, 4> &getDiagnostics();

private:
   ConformanceCheckerImpl *pImpl;
};

} // namespace sema
} // namespace cdot

#endif //CDOT_CONFORMANCECHECKER_H
