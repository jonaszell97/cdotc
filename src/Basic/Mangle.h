//
// Created by Jonas Zell on 05.01.18.
//

#ifndef CDOT_MANGLE_H
#define CDOT_MANGLE_H

#include <llvm/ADT/ArrayRef.h>

namespace cdot {

namespace ast {
   class Function;
} // namespace ast

namespace cl {
   class Record;
   struct Method;
   class AssociatedType;
} // namespace cl

class SymbolMangler {
public:
   SymbolMangler() = default;

   std::string mangleVariable(llvm::StringRef varName, size_t scope = 0) const;

   std::string mangleFunction(ast::Function *F) const;
   std::string mangleMethod(cl::Method *M) const;


   std::string mangleProtocolMethod(cl::Record *R,
                                    cl::Method *M,
                                    llvm::ArrayRef<cl::AssociatedType>
                                                         associatedTypes) const;

};

} // namespace cdot

#endif //CDOT_MANGLE_H
