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
   class ExtensionDecl;
   class ProtocolDecl;
   class RecordDecl;
   class SemaPass;
} // namespace ast

namespace sema {

void checkConformance(ast::SemaPass &SP, ast::RecordDecl *Rec);
void checkConformanceToProtocol(ast::SemaPass &SP,
                                ast::RecordDecl *Rec,
                                ast::ProtocolDecl *Proto);
void checkConformanceToProtocol(ast::SemaPass &SP,
                                ast::RecordDecl *Rec,
                                ast::ProtocolDecl *Proto,
                                ast::ExtensionDecl *Ext);

} // namespace sema
} // namespace cdot

#endif //CDOT_CONFORMANCECHECKER_H
