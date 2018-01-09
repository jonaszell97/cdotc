//
// Created by Jonas Zell on 27.11.17.
//

#include "Protocol.h"
#include "../../../Statement/Declaration/Class/RecordDecl.h"

namespace cdot {
namespace cl {

Protocol::Protocol(AccessModifier am,
                   const string &name,
                   Namespace *NS,
                   const SourceLocation &loc,
                   ast::ProtocolDecl *decl)
   : Record(ProtocolID, name, NS, am, {}, loc, decl)
{

}

void Protocol::calculateSizeImpl()
{
   occupiedBytes += sizeof(void*) * 3; // ptable ptr, obj ptr, typeinfo ptr
   alignment = sizeof(void*);
}

} // namespace cl
} // namespace cdot