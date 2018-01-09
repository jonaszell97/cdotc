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
#include "../../../Variant/Type/FunctionType.h"

using namespace cdot::support;

namespace cdot {
namespace il {

namespace {

FunctionType *makeMethodTy(AggregateType *SelfTy,
                           QualType &returnType,
                           llvm::ArrayRef<Argument*> args) {
   std::vector<cdot::Argument> realArgs;

   if (SelfTy)
      realArgs.emplace_back("", QualType(ObjectType::get(SelfTy->getName())));

   for (const auto &arg : args) {
      realArgs.emplace_back(arg->getName(), arg->getType(),
                            nullptr, false, arg->isVararg());
   }

   return FunctionType::get(returnType, realArgs, true);
}

} // anonymous namespace

Method::Method(const std::string &name,
               QualType returnType,
               llvm::ArrayRef<Argument *> args,
               AggregateType *forType,
               bool isStatic,
               bool isVirtual,
               bool isProperty,
               bool isOperator,
               bool isConversionOp,
               Module *parent,
               bool mightThrow)
   : Function(MethodID, makeMethodTy(isStatic ? nullptr : forType,
                                     returnType, args),
              name, returnType, args, parent, mightThrow, false),
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
                         llvm::ArrayRef<Argument *> args,
                         AggregateType *forType,
                         Module *parent,
                         bool mightThrow)
   : Method(name, QualType(VoidType::get()), args, forType, false, false,
            false, false, false, parent, mightThrow)
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
