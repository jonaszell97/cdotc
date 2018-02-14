//
// Created by Jonas Zell on 04.02.18.
//

#ifndef CDOT_TABLEGENBACKENDS_H
#define CDOT_TABLEGENBACKENDS_H

namespace llvm {
   class raw_ostream;
} // namespace llvm

namespace cdot {
namespace tblgen {

class RecordKeeper;

void PrintRecords(llvm::raw_ostream &str, RecordKeeper const& RK);
void EmitClassHierarchy(llvm::raw_ostream &str, RecordKeeper const& RK);

} // namespace tblgen
} // namespace cdot

#endif //CDOT_TABLEGENBACKENDS_H
