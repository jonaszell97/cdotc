//
// Created by Jonas Zell on 23.12.17.
//

#ifndef CDOT_CONFORMANCECHECKER_H
#define CDOT_CONFORMANCECHECKER_H

#include <llvm/ADT/ArrayRef.h>

namespace cdot {

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

namespace ast {
   class RecordDecl;
   class SemaPass;
} // namespace ast

namespace sema {

class ConformanceCheckerImpl;

class ConformanceChecker {
public:
   explicit ConformanceChecker(ast::SemaPass &SP, ast::RecordDecl *Rec);
   ~ConformanceChecker();

   void checkConformance();

private:
   ConformanceCheckerImpl *pImpl;
};

} // namespace sema
} // namespace cdot

#endif //CDOT_CONFORMANCECHECKER_H
