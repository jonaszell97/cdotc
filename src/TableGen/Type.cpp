#include "Type.h"
#include "Record.h"
#include "Support/Casting.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {
namespace tblgen {

std::string Type::toString() const
{
   std::string s;
   llvm::raw_string_ostream ss(s);

   ss << this;
   ss.flush();

   return s;
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &str, Type const* Ty)
{
   switch (Ty->getTypeID()) {
      case Type::IntTypeID: {
         auto I = cast<IntType>(Ty);
         str << llvm::StringRef(I->isUnsigned() ? "u" : "i")
             << I->getBitWidth();
         break;
      }
      case Type::FloatTypeID:
         str << "f32";
         break;
      case Type::DoubleTypeID:
         str << "f64";
         break;
      case Type::StringTypeID:
         str << "string";
         break;
      case Type::ListTypeID:
         str << "list<";
         str << cast<ListType>(Ty)->getElementType();
         str << ">";
         break;
      case Type::DictTypeID:
         str << "dict<";
         str << cast<DictType>(Ty)->getElementType();
         str << ">";
         break;
      case Type::ClassTypeID:
         str << cast<ClassType>(Ty)->getClass()->getName();
         break;
      case Type::RecordTypeID:
         str << cast<RecordType>(Ty)->getRecord()->getName();
         break;
      case Type::CodeTypeID:
         str << "code";
         break;
      default:
         llvm_unreachable("unhandled type");
   }

   return str;
}

} // namespace tblgen
} // namespace cdot