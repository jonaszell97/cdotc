//
// Created by Jonas Zell on 01.02.18.
//

#include "Record.h"
#include "TableGen.h"
#include "Value.h"

#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace tblgen {

Class *RecordKeeper::CreateClass(llvm::StringRef name, SourceLocation loc)
{
   auto C = new (TG) Class(*this, name, loc);
   Classes.try_emplace(name, C);

   return C;
}

void Class::dump()
{
   printTo(llvm::errs());
}

void Class::printTo(llvm::raw_ostream &out)
{
   out << "class " << name << " ";
   if (!parameters.empty()) {
      out << "<";

      size_t i = 0;
      for (auto &param : parameters) {
         if (i++ != 0) out << ", ";
         out << param.getName() << ": ";
         out << param.getType();

         if (auto V = param.getDefaultValue())
            out << " = " << V;
      }

      out << "> ";
   }

   if (!bases.empty()) {
      out << ": ";

      size_t i = 0;
      for (auto &base : bases) {
         if (i++ != 0) out << ", ";
         out << base.getBase()->getName();

         if (!base.getTemplateArgs().empty()) {
            out << "<";

            size_t j = 0;
            for (auto &TA : base.getTemplateArgs()) {
               if (j++ != 0) out << ", ";
               out << TA;
            }

            out << ">";
         }
      }
   }

   if (!fields.empty()) {
      out << " {\n";

      for (auto &F : fields) {
         out << "   " << F.getName() << ": " << F.getType();
         if (auto V = F.getDefaultValue())
            out << " = " << V;

         out << "\n";
      }

      out << "}";
   }
}

void Record::dump()
{
   printTo(llvm::errs());
}

void Record::dumpAllValues()
{
   auto &out = llvm::errs();
   out << "def " << name << " {\n";

   for (auto &F : fieldValues) {
      out << "   " << F.getKey() << " = " << F.getValue() << "\n";
   }

   out << "}";
}

void Record::printTo(llvm::raw_ostream &out)
{
   out << "def " << name << " ";

   if (!bases.empty()) {
      out << ": ";

      size_t i = 0;
      for (auto &base : bases) {
         if (i++ != 0) out << ", ";
         out << base.getBase()->getName();

         if (!base.getTemplateArgs().empty()) {
            out << "<";

            size_t j = 0;
            for (auto &TA : base.getTemplateArgs()) {
               if (j++ != 0) out << ", ";
               out << TA;
            }

            out << ">";
         }
      }
   }

   if (!ownFields.empty()) {
      out << " {\n";

      for (auto &F : ownFields) {
         out << "   " << F.getName() << " = " << F.getDefaultValue() << "\n";
      }

      out << "}";
   }
}

Record* RecordKeeper::CreateRecord(llvm::StringRef name, SourceLocation loc)
{
   auto R = new (TG) Record(*this, name, loc);
   Records.insert(std::make_pair(name, R));

   return R;
}

Record* RecordKeeper::CreateAnonymousRecord(cdot::SourceLocation loc)
{
   return new(TG) Record(*this, loc);
}

void RecordKeeper::dump()
{
   printTo(llvm::errs());
}

void RecordKeeper::printTo(llvm::raw_ostream &out)
{
   for (auto &C : Classes) {
      C.getValue()->printTo(out);
      out << "\n\n";
   }

   for (auto &R : Records) {
      R.getValue()->printTo(out);
      out << "\n\n";
   }

   for (auto &NS : Namespaces) {
      out << "namespace " << NS.getValue().getNamespaceName() << " {\n\n";

      NS.getValue().printTo(out);

      out << "\n}";
   }
}

void RecordKeeper::addValue(llvm::StringRef name,
                            Value *V,
                            SourceLocation loc) {
   Values[name] = ValueDecl(V, loc);
}

} // namespace tblgen
} // namespace cdot