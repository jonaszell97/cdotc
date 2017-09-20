
#include <iostream>
#include "Parser.h"
#include "Message/Exceptions.h"
#include "Compiler.h"
#include <fstream>
#include <codecvt>
#include <execinfo.h>
#include <random>

void  handler()
{
   void *trace_elems[20];
   int trace_elem_count(backtrace( trace_elems, 20 ));
   char **stack_syms(backtrace_symbols( trace_elems, trace_elem_count ));
   for ( int i = 0 ; i < trace_elem_count ; ++i )
   {
      std::cout << stack_syms[i] << "\n";
   }
   free( stack_syms );

   exit(1);
}

int main(int argc, char* argv[])
{
   cdot::Compiler comp(argc, argv);
   std::set_terminate(handler);

   try {
      comp.compile();
   } catch(ParseError& p) {
      std::cout << p.what() << std::endl;
   } catch(RuntimeError& r) {
      std::cout << r.what() << std::endl;
   }

   return 0;
}