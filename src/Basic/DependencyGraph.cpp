//
// Created by Jonas Zell on 22.12.17.
//

#include <llvm/Support/raw_ostream.h>
#include "DependencyGraph.h"

namespace cdot {

void testDG()
{
   DependencyGraph<const char> DG;

   auto &N2 = DG.getOrAddVertex("2");
   auto &N3 = DG.getOrAddVertex("3");
   auto &N5 = DG.getOrAddVertex("5");
   auto &N7 = DG.getOrAddVertex("7");
   auto &N8 = DG.getOrAddVertex("8");
   auto &N9 = DG.getOrAddVertex("9");
   auto &N10 = DG.getOrAddVertex("10");
   auto &N11 = DG.getOrAddVertex("11");

   N3.addOutgoing(&N8);
   N3.addOutgoing(&N10);

   N5.addOutgoing(&N11);

   N7.addOutgoing(&N8);
   N7.addOutgoing(&N11);

   N8.addOutgoing(&N9);

   N11.addOutgoing(&N2);
   N11.addOutgoing(&N9);
   N11.addOutgoing(&N10);

   bool valid = DG.actOnGraphInOrder([](const char *const ptr) {
      llvm::outs() << ptr << "\n";
   });

   llvm::outs() << "valid: " << valid << "\n";
}

} // namespace cdot