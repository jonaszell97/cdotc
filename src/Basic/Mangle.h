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
   std::string mangleVariable(llvm::StringRef varName, size_t scope = 0) const;

   std::string mangleAnyFunction(ast::CallableDecl *C) const;
   std::string mangleFunction(ast::FunctionDecl *F) const;
   std::string mangleMethod(ast::MethodDecl *M) const;

   void mangleMethod(ast::SemaPass &SP,
                     llvm::SmallString<128> &Buf,
                     ast::MethodDecl *M,
                     ast::RecordDecl *recordInst) const;

   std::string mangleProtocolMethod(ast::RecordDecl *R,
                                    ast::MethodDecl *M) const;
};

} // namespace cdot

#endif //CDOT_MANGLE_H
