//
// Created by Jonas Zell on 16.11.17.
//

#include "Method.h"
#include "Argument.h"
#include "BasicBlock.h"

#include "../../Module/Module.h"
#include "../../Module/Context.h"

#include "../Record/AggregateType.h"

using namespace cdot::support;

namespace cdot {
namespace il {

Method::Method(const std::string &name,
               FunctionType *FuncTy,
               AggregateType *forType,
               bool isStatic,
               bool isVirtual,
               bool isProperty,
               bool isOperator,
               bool isConversionOp,
               Module *parent,
               bool addSelfArg)
   : Function(MethodID, FuncTy, name, parent, false),
     recordType(forType), Self(nullptr)
{
   if (isStatic) {
      SubclassData |= Flag::Static;
   }
   if (isVirtual) {
      SubclassData |= Flag::Virtual;
   }
   if (isProperty) {
      SubclassData |= Flag::Property;
   }
   if (isOperator) {
      SubclassData |= Flag::Operator;
   }
   if (isConversionOp) {
      SubclassData |= Flag::ConversionOp;
      checkIfPrimitiveOp();
   }
}

Method::Method(const Method &other)
   : Function(other), recordType(other.recordType), Self(nullptr)
{
   id = MethodID;
}

AggregateType* Method::getRecordType() const
{
   return recordType;
}

bool Method::isProtocolMethod() const
{
   return isa<ProtocolType>(recordType);
}

Argument *Method::getSelf()
{
   assert(!BasicBlocks.empty());
   return &BasicBlocks.front().getArgs().front();
}

namespace {

const char *BoxedPrimitives[] = {
   "Float", "Double", "Char", "Bool", "Int64", "UInt64", "Int32", "UInt32",
   "Int16", "UInt16", "Int8", "UInt8", "Int1", "UInt1"
};

} // anonymous namespace

void Method::checkIfPrimitiveOp()
{
   if (!isOperator()) {
      return;
   }

   bool isPrimitive = false;
   auto nameRef = recordType->getName();
   for (const auto &name : BoxedPrimitives) {
      if (nameRef.equals(name)) {
         isPrimitive = true;
         break;
      }
   }

   if (isPrimitive) {
      SubclassData |= Flag::BoxedOperator;
   }
}

Initializer::Initializer(const std::string &name,
                         FunctionType *FuncTy,
                         AggregateType *forType,
                         Module *parent,
                         bool addSelfArg)
   : Method(name, FuncTy, forType, false, false,
            false, false, false, parent, addSelfArg)
{
   id = InitializerID;
}

Initializer::Initializer(const Initializer &other)
   : Method(other)
{
   id = InitializerID;
}

} // namespace il
} // namespace cdot
