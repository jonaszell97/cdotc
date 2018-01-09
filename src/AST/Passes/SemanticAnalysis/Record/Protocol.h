//
// Created by Jonas Zell on 27.11.17.
//

#ifndef CDOT_PROTOCOL_H
#define CDOT_PROTOCOL_H

#include "Record.h"

namespace cdot {

namespace ast {
class ProtocolDecl;
} // namespace ast

namespace cl {

class Protocol: public Record {
public:
   Protocol(AccessModifier am,
            const string &name,
            Namespace *NS,
            const SourceLocation &loc,
            ast::ProtocolDecl *decl);

   void calculateSizeImpl();

   static bool classof(Record const *T)
   {
      return T->getTypeID() == ProtocolID;
   }
};

} // namespace cl
} // namespace cdot


#endif //CDOT_PROTOCOL_H
