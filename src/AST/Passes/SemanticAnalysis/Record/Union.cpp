//
// Created by Jonas Zell on 10.10.17.
//

#include "Union.h"

#include "../../../../Variant/Type/BuiltinType.h"
#include "../../../../Util.h"

namespace cdot {
namespace cl {

   Union::Union(
      const string& name,
      bool isConst,
      const SourceLocation &loc)
      : Record(name, AccessModifier::PUBLIC, loc), is_const(isConst)
   {

   }

   void Union::declareField(
      const string &fieldName,
      BuiltinType *fieldTy)
   {
      fields.emplace(fieldName, fieldTy);
      if (fieldTy->getSize() > occupiedBytes) {
         occupiedBytes = fieldTy->getSize();
      }
      if (fieldTy->getAlignment() > alignment) {
         alignment = fieldTy->getAlignment();
      }
   }

   bool Union::initializableWith(BuiltinType *ty)
   {
      for (const auto& field : fields) {
         if (ty->implicitlyCastableTo(field.second)) {
            return true;
         }
      }

      return false;
   }

   const unordered_map<string, cdot::BuiltinType *> &Union::getFields() const {
      return fields;
   }

   void Union::setFields(const unordered_map<string, cdot::BuiltinType *> &types) {
      Union::fields = types;
   }

   bool Union::isConst() const {
      return is_const;
   }

   void Union::isConst(bool is_const) {
      Union::is_const = is_const;
   }

   bool Union::hasField(string &name)
   {
      return fields.find(name) != fields.end();
   }

   BuiltinType* Union::getFieldTy(string &name)
   {
      return fields[name];
   }

}
}