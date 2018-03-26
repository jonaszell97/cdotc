//
// Created by Jonas Zell on 05.01.18.
//

#ifndef CDOT_MANGLE_H
#define CDOT_MANGLE_H

#include <llvm/ADT/ArrayRef.h>
#include "../Support/LLVM.h"

namespace cdot {

namespace sema {
   class TemplateArgList;
} // namespace sema

namespace ast {
   class SemaPass;
   class CallableDecl;
   class FunctionDecl;
   class MethodDecl;
   class RecordDecl;
} // namespace ast

class SymbolMangler {
public:
   explicit SymbolMangler(ast::SemaPass &SP) : SP(SP)
   {}

   std::string mangleVariable(llvm::StringRef varName, size_t scope = 0) const;

   std::string mangleAnyFunction(ast::CallableDecl *C) const;
   std::string mangleFunction(ast::FunctionDecl *F) const;
   std::string mangleMethod(ast::MethodDecl *M) const;

   std::string mangleProtocolMethod(ast::RecordDecl *R,
                                    ast::MethodDecl *M) const;

private:
   ast::SemaPass &SP;
};

} // namespace cdot

#endif //CDOT_MANGLE_H
