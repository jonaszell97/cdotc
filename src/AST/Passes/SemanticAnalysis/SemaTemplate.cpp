//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"

#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Declaration/CallableDecl.h"
#include "AST/Statement/Declaration/TypedefDecl.h"

using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {

RecordDecl* SemaPass::InstantiateRecord(RecordDecl *R,
                                        TemplateArgList &&TAs,
                                        SourceLocation loc) {
   auto Inst = TemplateInstantiator::InstantiateRecord(*this, loc,
                                                       R, std::move(TAs));

   return Inst;
}

} // namespace ast
} // namespace cdot