
#include <execinfo.h>
#include <llvm/Support/raw_ostream.h>

#include "Compiler.h"

void  handler()
{
//   void *trace_elems[20];
//   int trace_elem_count(backtrace( trace_elems, 20 ));
//   char **stack_syms(backtrace_symbols( trace_elems, trace_elem_count ));
//   for ( int i = 0 ; i < trace_elem_count ; ++i )
//   {
//      llvm::outs() << stack_syms[i] << "\n";
//   }
//   free( stack_syms );

   exit(1);
}

int main(int argc, char* argv[])
{
   std::set_terminate(handler);

   cdot::Compiler::init(argc, argv);
   cdot::Compiler::compile();

   return 0;
}