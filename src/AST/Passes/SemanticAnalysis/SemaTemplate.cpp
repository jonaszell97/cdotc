//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "../../Statement/Declaration/Template/RecordTemplateDecl.h"
#include "../../Statement/Declaration/Template/CallableTemplateDecl.h"
#include "../../Statement/Declaration/Template/MethodTemplateDecl.h"
#include "../../Statement/Declaration/Class/ClassDecl.h"
#include "../../Statement/Declaration/Class/MethodDecl.h"

#include "Record/Class.h"
#include "../../SymbolTable.h"
#include "../../../Message/Diagnostics.h"

using namespace cdot::diag;

cl::Record* SemaPass::getRecord(
   string &name, TemplateArgList *argList)
{
   auto record = SymbolTable::getRecord(
      name,
      argList,
      importedNamespaces
   );

   auto decl = record->getDecl();
   if (!decl->isVisited()) {
      if (!record->isEnum() && record->isNonUnion()) {
         auto cl = record->getAs<Class>();
         auto node = static_cast<ClassDecl*>(decl);
         pushClassScope(cl);

         doInitialPass(node->getInnerDeclarations());
         for (const auto &field : node->fields) {
            DefineField(field.get(), cl);
         }

         popClassScope();
      }
   }

   return record;
}

namespace {

constexpr int MAX_INSTANTIATION_DEPTH = 255;
unsigned instantiationDepth = 0;

void incAndCheckInstantiationDepth(AstNode *cause, size_t by = 1)
{
   instantiationDepth += by;
   if (instantiationDepth > MAX_INSTANTIATION_DEPTH) {
      diag::err(err_generic_error)
         << "maximum template instantiation depth reached ("
            + std::to_string(MAX_INSTANTIATION_DEPTH)
         << ")" << cause << diag::term;
   }
}

void decInstantiationDepth()
{
   --instantiationDepth;
}

}

void SemaPass::visit(RecordTemplateDecl *node)
{
   auto &Instantiations = node->getInstantiations();
   size_t prevSize = 0;
   size_t i;

   while (Instantiations.size() > prevSize) {
      i = prevSize;
      prevSize = Instantiations.size();

      incAndCheckInstantiationDepth(node, prevSize - i);

      for (; i < prevSize; ++i) {
         const auto &inst = Instantiations[i];
         doInitialPass(inst);
         inst->accept(this);
      }
   }

   instantiationDepth = 0;
}

void SemaPass::visit(CallableTemplateDecl *node)
{
   for (const auto &inst : node->getInstantiations()) {
      inst->accept(this);
   }
}

void SemaPass::visit(MethodTemplateDecl *node)
{
   for (const auto &inst : node->getInstantiations()) {
      auto method = std::static_pointer_cast<MethodDecl>(inst);
      pushClassScope(method->getRecord());
      inst->accept(this);
      popClassScope();
   }
}