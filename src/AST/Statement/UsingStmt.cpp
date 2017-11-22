//
// Created by Jonas Zell on 02.07.17.
//

#include "UsingStmt.h"

namespace cdot {
namespace ast {

UsingStmt::UsingStmt(
   string&& importNamespace,
   std::vector<string>&& items)
   : Statement(UsingStmtID), importNamespace(move(importNamespace)),
     importedItems(move(items))
{
   
}

UsingStmt::UsingStmt(std::vector<string> &&fullNames)
   : Statement(UsingStmtID), fullNames(fullNames)
{

}

} // namespace ast
} // namespace cdot