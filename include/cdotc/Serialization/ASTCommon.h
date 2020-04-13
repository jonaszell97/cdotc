#ifndef CDOT_ASTCOMMON_H
#define CDOT_ASTCOMMON_H

#include "cdotc/Basic/DeclarationName.h"

namespace cdot {
namespace ast {
class NamedDecl;
} // namespace ast

namespace serial {

template<typename T, typename Allocator>
llvm::StringRef bytes(const std::vector<T, Allocator>& v)
{
   if (v.empty())
      return llvm::StringRef();
   return llvm::StringRef(reinterpret_cast<const char*>(&v[0]),
                          sizeof(T) * v.size());
}

template<typename T> llvm::StringRef bytes(const llvm::SmallVectorImpl<T>& v)
{
   return llvm::StringRef(reinterpret_cast<const char*>(v.data()),
                          sizeof(T) * v.size());
}

template<typename T> llvm::StringRef bytes(llvm::ArrayRef<T> v)
{
   return llvm::StringRef(reinterpret_cast<const char*>(v.data()),
                          sizeof(T) * v.size());
}

unsigned ComputeHash(DeclarationName Name);

} // namespace serial
} // namespace cdot

#endif // CDOT_ASTCOMMON_H
