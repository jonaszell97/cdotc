//
// Created by Jonas Zell on 16.11.17.
//

#include "Method.h"
#include "Argument.h"
#include "BasicBlock.h"

#include "../../Module/Module.h"
#include "../../Module/Context.h"

#include "../Record/AggregateType.h"

#include "../../../Variant/Type/VoidType.h"
#include "../../../AST/Passes/SemanticAnalysis/Function.h"
#include "../../../AST/Passes/SemanticAnalysis/Record/Record.h"

namespace cdot {
namespace il {

Method::Method(cl::Method *m, Module *parent)
   : Function(m, parent),
     recordType(parent->getType(m->owningClass->getName(), false)),
     Self(nullptr)
{
   id = MethodID;

   if (m->isStatic()) {
      SubclassData |= Flag::Static;
   }

   if (m->isProperty()) {
      SubclassData |= Flag::Property;
   }
}

Method::Method(const std::string &name,
               QualType returnType,
               llvm::ArrayRef<cdot::Argument> args,
               AggregateType *forType,
               bool isStatic,
               Module *parent, SourceLocation loc,
               bool mightThrow)
   : Function(name, returnType, args, parent, loc, mightThrow),
     recordType(forType),
     Self(nullptr)
{
   id = MethodID;
   if (isStatic) {
      SubclassData |= Flag::Static;
   }
}

Method::Method(const std::string &name, FunctionType *ty,
               QualType returnType, llvm::ArrayRef<Argument *> args,
               AggregateType *forType, bool isStatic,
               Module *parent, SourceLocation loc, bool mightThrow)
   : Function(name, ty, returnType, args, parent, loc, mightThrow),
     recordType(forType),
     Self(nullptr)
{
   id = MethodID;
   if (isStatic) {
      SubclassData |= Flag::Static;
   }
}

AggregateType* Method::getRecordType() const
{
   return recordType;
}

bool Method::isStatic() const
{
   return (SubclassData & Flag::Static) != 0;
}

bool Method::isProperty() const
{
   return (SubclassData & Flag::Property) != 0;
}

Argument *Method::getSelf() const
{
   assert(!BasicBlocks.empty());
   return BasicBlocks.front()->getBlockArg(0);
}

Initializer::Initializer(cl::Method *m, Module *parent)
   : Method(m, parent)
{
   id = InitializerID;
}

Initializer::Initializer(const std::string &name,
                         llvm::ArrayRef<cdot::Argument> args,
                         AggregateType *forType, Module *parent,
                         SourceLocation loc, bool mightThrow)
   : Method(name, QualType(VoidType::get()), args, forType, false, parent,
            loc, mightThrow)
{
   id = InitializerID;
}

Initializer::Initializer(const std::string &name, FunctionType *ty,
                         QualType returnType, llvm::ArrayRef<Argument *> args,
                         AggregateType *forType, Module *parent,
                         SourceLocation loc, bool mightThrow)
   : Method(name, ty, returnType, args, forType, false, parent,
            loc, mightThrow)
{
   id = InitializerID;
}

} // namespace il
} // namespace cdot
