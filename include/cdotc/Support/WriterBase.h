#ifndef CDOT_WRITERBASE_H
#define CDOT_WRITERBASE_H

#include "cdotc/Support/Format.h"
#include <llvm/Support/raw_ostream.h>

namespace cdot {

template<class SubClass, short TabSize = 3> class WriterBase {
public:
   explicit WriterBase(llvm::raw_ostream& out) : out(out) {}

   void NewLine() { out << '\n'; }

   void ApplyTab(short by = -1)
   {
      if (by < 0) {
         by = CurrentTab;
      }

      for (unsigned i = 0; i < by; ++i) {
         out << ' ';
      }
   }

   void WriteEscapedString(llvm::StringRef str)
   {
      for (unsigned char c : str) {
         if (isprint(c) && c != '\\' && c != '"') {
            out << c;
         }
         else {
            out << '\\' << support::hexdigit(c >> 4)
                << support::hexdigit(c & 0x0Fu);
         }
      }
   }

   void WriteIdentifier(llvm::StringRef str)
   {
      bool first = true;
      for (auto c : str) {
         if ((!isalnum(c) && c != '_' && c != '.') || (first && isdigit(c))) {
            out << '`' << str << '`';

            return;
         }

         first = false;
      }

      out << str;
   }

   void WriteHex(uint64_t val)
   {
      out << support::formatInteger<support::Base16Traits>(val);
   }

   void WriteHex(double d) { out << support::formatAsHexInteger(d); }

   void WriteHex(float f) { out << support::formatAsHexInteger(f); }

   void WritePointer(void const* Ptr)
   {
      out << support::formatAsHexInteger(Ptr);
   }

   void WriteString(llvm::StringRef str) { out << str; }

   void WriteUnsigned(size_t val) { out << val; }

   void WriteSigned(unsigned long long val) { out << val; }

   template<class T, class Writer>
   void WriteList(const T& arr, Writer write, const char* begin = "(",
                  const char* delim = ", ", const char* end = ")",
                  bool skipIfEmpty = false)
   {
      auto it = arr.begin();
      auto end_it = arr.end();

      if (it == end_it && skipIfEmpty)
         return;

      out << begin;

      unsigned i = 0;
      for (; it != end_it; ++it) {
         if (i++ != 0)
            out << delim;
         (static_cast<SubClass*>(this)->*write)(*it);
      }

      out << end;
   }

   struct TabGuard {
      explicit TabGuard(WriterBase* B, short increaseBy = TabSize)
          : B(B), increaseBy(increaseBy)
      {
         B->CurrentTab += increaseBy;
      }

      ~TabGuard() { B->CurrentTab -= increaseBy; }

   protected:
      WriterBase* B;
      short increaseBy;
   };

   TabGuard makeTabGuard(short increaseBy = TabSize)
   {
      return TabGuard(this, increaseBy);
   }

protected:
   short CurrentTab = TabSize;
   llvm::raw_ostream& out;
};

} // namespace cdot

#endif // CDOT_WRITERBASE_H
