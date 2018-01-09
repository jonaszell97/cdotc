//
// Created by Jonas Zell on 28.11.17.
//

#ifndef CDOT_MODULE2_H
#define CDOT_MODULE2_H

#include <string>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {

class SymbolTableEntry;

namespace module {

class Module {
public:
   static Module *get(std::string &&Name,
                      std::string &&fileName,
                      std::string &&md5);

   void addEntry(SymbolTableEntry *Entry) { Entries.push_back(Entry); }

   llvm::StringRef getName() const { return name; }
   const std::string &getFileName() const { return fileName; }
   const std::string &getMd5() const { return md5; }

   const llvm::SmallVector<SymbolTableEntry *, 8> &getEntries() const
   { return Entries; }

private:
   explicit Module(std::string &&Name,
                   std::string &&fileName,
                   std::string &&md5)
      : name(move(Name)), fileName(move(fileName)),
        md5(move(md5)), Entries {}
   {}

   static llvm::StringMap<Module*> Instances;

   std::string name;
   std::string fileName;
   std::string md5;

   llvm::SmallVector<SymbolTableEntry*, 8> Entries;
};

} // namespace module
} // namespace cdot

#endif //CDOT_MODULE2_H
