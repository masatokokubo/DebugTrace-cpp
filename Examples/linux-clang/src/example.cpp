/// Example.cpp 
/// (C) 2017 Masato Kokubo
#include "example.hpp"
#include "debugtrace.hpp"

DEBUGTRACE_VARIABLES

int main(int argc, const char* argv[]) {
    DEBUGTRACE_ENTER // for Debugging
#if defined CP_ACP
    // sets Windows code page to Japanese Shift-JIS
//  debugtrace::set_code_page(932); // for Debugging

    // sets Windows code page to Japanese EUC
//  debugtrace::set_code_page(20932); // for Debugging

    // sets Windows code page to Japanese JIS
//  debugtrace::set_code_page(50220); // for Debugging

    // sets Windows code page to UTF-8
    debugtrace::set_code_page(CP_UTF8); // for Debugging
#else
    const char* locale = setlocale(LC_CTYPE, nullptr);
    DEBUGTRACE_PRINT(locale) // for Debugging
    locale = setlocale(LC_CTYPE, "");
    DEBUGTRACE_PRINT(locale) // for Debugging
#endif

    ClassA classA;
    classA.func1();
    classA.func2();
    classA.func3();
    classA.func4();
    return 0;
}
