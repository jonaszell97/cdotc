//
// Created by Jonas Zell on 23.12.17.
//

#ifndef CDOT_CONFORMANCECHECKER_H
#define CDOT_CONFORMANCECHECKER_H

namespace cdot {

namespace ast {
   class ProtocolDecl;
   class RecordDecl;
   class SemaPass;
} // namespace ast

namespace sema {

bool checkConformance(ast::SemaPass &SP, ast::RecordDecl *Rec);
bool checkConformanceToProtocol(ast::SemaPass &SP,
                                ast::RecordDecl *Rec,
                                ast::ProtocolDecl *Proto);

} // namespace sema
} // namespace cdot

#endif //CDOT_CONFORMANCECHECKER_H
