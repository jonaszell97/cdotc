
#include <iostream>
#include "Parser.h"
#include "Message/Exceptions.h"
#include <fstream>
#include <codecvt>
#include <execinfo.h>

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

int main() {
    std::set_terminate(handler);
    std::ifstream t("/Users/Jonas/CDotProjects/ex/main.dot");
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    try {
        Parser x(str);
        x.run(1);
    } catch(ParseError& p) {
        std::cout << p.what() << std::endl;
    } catch(RuntimeError& r) {
        std::cout << r.what() << std::endl;
    }

    return 0;
}