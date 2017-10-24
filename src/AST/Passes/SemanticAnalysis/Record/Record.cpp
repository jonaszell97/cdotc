//
// Created by Jonas Zell on 10.10.17.
//

#include "Record.h"
#include "../../../../Util.h"

namespace cdot {
namespace cl {

   size_t Record::lastTypeID = 0;

   Record::Record(
      const string &name,
      AccessModifier access,
      const SourceLocation &loc) :
        recordName(name),
        declarationNamespace(name.substr(0, name.rfind('.'))),
        typeID(lastTypeID++), loc(loc)
   {

   }

   AccessModifier Record::getAccess() {
      return access;
   }

   bool Record::isPrivate() {
      return access == AccessModifier::PRIVATE;
   }

   bool Record::isProtected() {
      return access == AccessModifier::PROTECTED;
   }

   string Record::getTypeName() {
      if (isStruct()) {
         return "struct";
      }
      if (isProtocol()) {
         return "protocol";
      }
      if (isNonUnion()) {
         return "class";
      }
      if (isEnum()) {
         return "enum";
      }
      if (isUnion()) {
         return "union";
      }

      llvm_unreachable(0);
   }

   int Record::getNameSelector() {
      if (isNonUnion()) {
         return 0;
      }
      if (isStruct()) {
         return 1;
      }
      if (isEnum()) {
         return 2;
      }
      if (isUnion()) {
         return 3;
      }
      if (isProtocol()) {
         return 4;
      }

      llvm_unreachable(0);
   }

}
}