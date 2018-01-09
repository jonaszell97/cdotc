//
// Created by Jonas Zell on 10.10.17.
//

#include "Union.h"

#include "../../../../Variant/Type/Type.h"
#include "../../../../Util.h"
#include "../../../Statement/Declaration/Class/RecordDecl.h"

namespace cdot {
namespace cl {

Union::Union(
   const string& name,
   Namespace *NS,
   bool isConst,
   std::vector<TemplateParameter> &&templateParams,
   const SourceLocation &loc,
   UnionDecl *decl)
   : Record(UnionID, name, NS, AccessModifier::PUBLIC, move(templateParams),
            loc, decl),
     is_const(isConst)
{

}

void Union::declareField(const string &fieldName, QualType fieldTy)
{
   fields.emplace(fieldName, fieldTy);
}

Type* Union::initializableWith(Type *ty)
{
   for (const auto& field : fields) {
      if (ty->implicitlyCastableTo(field.second)) {
         return field.second;
      }
   }

   return nullptr;
}

const unordered_map<string, cdot::Type *> &Union::getFields() const {
   return fields;
}

void Union::setFields(const unordered_map<string, cdot::Type *> &types) {
   Union::fields = types;
}

bool Union::isConst() const {
   return is_const;
}

void Union::isConst(bool is_const) {
   Union::is_const = is_const;
}

bool Union::hasField(llvm::StringRef name)
{
   return fields.find(name) != fields.end();
}

Type* Union::getFieldTy(llvm::StringRef name)
{
   return fields[name];
}

void Union::calculateSizeImpl()
{
   for (const auto &f : fields) {
      auto fieldTy = f.second;
      if (fieldTy->getSize() > occupiedBytes) {
         occupiedBytes = fieldTy->getSize();
      }
      if (fieldTy->getAlignment() > alignment) {
         alignment = fieldTy->getAlignment();
      }
   }
}

}
}