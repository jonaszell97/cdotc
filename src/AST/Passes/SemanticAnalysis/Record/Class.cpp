//
// Created by Jonas Zell on 09.07.17.
//

#include <llvm/IR/Module.h>
#include "Class.h"

#include "../../../SymbolTable.h"
#include "../../../Expression/Expression.h"

#include "../../../../Variant/Type/ObjectType.h"
#include "../../../../Util.h"
#include "../../../../Variant/Type/GenericType.h"
#include "../Function.h"

#include "../../../Statement/Declaration/Class/MethodDecl.h"
#include "../../../Statement/Declaration/Class/RecordDecl.h"

#include "../../../Expression/TypeRef.h"
#include "../../../../Variant/Type/VoidType.h"
#include "../../../../Message/Diagnostics.h"
#include "../../CodeGen/CodeGen.h"
#include "../../Declaration/DeclPass.h"

using namespace cdot::support;

namespace cdot {
namespace cl {

using namespace cdot::diag;

string ExtensionConstraint::reportFailure() const
{
//   switch (kind) {
//      case ExtensionConstraint::IS_STRUCT:
//         return "type must be a struct";
//      case ExtensionConstraint::IS_CLASS:
//         return "type must be a class";
//      case ExtensionConstraint::IS_ENUM:
//         return "type must be an enum";
//      case ExtensionConstraint::IS_PROTOCOL:
//         return "type must be a protocol";
//      case ExtensionConstraint::CONFORMANCE:
//         return "type must conform to " + typeConstraint->toString();
//      case ExtensionConstraint::TYPE_EQUALITY:
//         return "type must be " + typeConstraint->toString();
//      case ExtensionConstraint::TYPE_INEQUALITY:
//         return "type must not be " + typeConstraint->toString();
//      case ExtensionConstraint::DEFAULT_CONSTRUCTIBLE:
//         return "type must have a parameterless constructor";
//   }

   llvm_unreachable("unknown constraint");
}

Struct::Struct(AccessModifier am,
               const string& className,
               Namespace *NS,
               std::vector<TemplateParameter> &&templateParams,
               RecordDecl *decl,
               const SourceLocation &loc)
   : Record(StructID, className, NS, am, move(templateParams), loc, decl)
{

}

Struct::~Struct() = default;

void Struct::pushConstraintSet(std::vector<ExtensionConstraint> &&constraints)
{
   activeConstraints = true;
   ConstraintSets.push_back(constraints);
}

void Struct::popConstraintSet()
{
   activeConstraints = false;
}

const std::vector<ExtensionConstraint>& Struct::getConstraintSet(unsigned i)
{
   assert( ConstraintSets.size() > i && "Constraint set doesn't exist");
   return ConstraintSets[i];
}

void Struct::addInitializer(Method *init)
{
   initializers.insert(init);
}

bool Struct::declareMemberwiseInitializer()
{
   std::vector<Argument> args;

   for (const auto& field : fields) {
      if (field.hasDefaultValue() || field.isStatic) {
         continue;
      }

      args.emplace_back(field.fieldName,
                        QualType(field.fieldType));
   }

   auto method = declareInitializer("init", AccessModifier::PUBLIC,
                                    std::move(args), {}, nullptr);

   method->setSourceLoc(loc);
   method->setMemberwiseInitializer(true);

   initializers.insert(method);
   memberwiseInitializer = method;

   return true;
}

size_t Struct::getMethodOffset(const string &methodName) const
{
   auto it = methodOffsets.find(methodName);
   if (it == methodOffsets.end()) {
      return 0;
   }

   return it->second;
}

void Struct::calculateSizeImpl()
{
   if (occupiedBytes)
      return;

   for (const auto &f : fields) {
      if (f.isStatic)
         continue;

      auto &ty = f.fieldType;
      if (ty->isObjectTy()) {
         auto rec = SymbolTable::getRecord(ty->getClassName());
         assert(rec->getSize() && "circular dependency!");

         switch (rec->getTypeID()) {
            case Record::ClassID:
               occupiedBytes += sizeof(void*);
               break;
            default:
               occupiedBytes += rec->getSize();
               break;
         }
      }
      else {
         occupiedBytes += ty->getSize();
      }

      if (ty->getAlignment() > alignment)
         alignment = ty->getAlignment();
   }

   if (!occupiedBytes) {
      occupiedBytes = sizeof(void*);
      alignment = alignof(void*);
   }
}

Class::Class(AccessModifier am, const string &className, Namespace *NS,
             std::vector<TemplateParameter> &&templateParams,
             RecordDecl *decl,
             const SourceLocation &loc, bool isAbstract)
   : Struct(am, className, NS, move(templateParams), decl, loc),
     is_abstract(isAbstract)
{
   typeID = ClassID;
}

bool Class::isBaseClassOf(const string &child) const
{
   if (!SymbolTable::hasClass(child)) {
      return false;
   }

   auto base = SymbolTable::getClass(child);
   auto current = base;
   while (current != nullptr) {
      if (this == current) {
         return true;
      }
      current = current->parentClass;
   }

   return false;
}

bool Class::isBaseClassOf(Class *child) const
{
   if (!child->isClass()) {
      return false;
   }

   auto current = child;
   while (current != nullptr) {
      if (this == current) {
         return true;
      }
      current = current->parentClass;
   }

   return false;
}

} // namespace cl
} // namespace cdot