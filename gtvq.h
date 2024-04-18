#ifndef gtvq_includes
#define gtvq_includes 1
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <dlfcn.h>

using namespace std;

#include "gtvqString.h"
#include "gtvqLine.h"

// This is the handler passed to the lookup table to handle a command
typedef gtvqString (*gtvqHandler)(gtvqString &, std::vector<gtvqString> &);
// This is the function passed to a module so they can link their handlers
typedef void (*gtvqLinkingHandler)(gtvqString command, gtvqHandler functionPointer);
// This is the function passed to a module so they can execute gtvq code
typedef gtvqString (*gtvqCodeExecutor)(gtvqString &code);
// This is the function passed to a module so they can throw errors
typedef void (*gtvqError)(string message);
// This is the function passed to a module so they can exit a block prematurely
typedef void (*gtvqRequestBlockExit)();
// This is the function to be called on modules so they link all their handlers
typedef void (*gtvqLinkHandlers)(gtvqLinkingHandler functionPointer, gtvqCodeExecutor codeExecutor, gtvqError errorMessage, gtvqRequestBlockExit requestBlockExit);

int main(int argc, char *argv[]);
void gtvq_error(string message);
void gtvq_error(string message, string filename, size_t line);
string gtvq_load_file(string filename);
gtvqString gtvq_execute_code(gtvqString &code);
gtvqString gtvq_execute_line(gtvqLine &line);
vector<gtvqLine> gtvq_tokenize_source(gtvqString source);
void gtvq_link_command_handler(gtvqString command, gtvqHandler functionPointer);
gtvqString gtvq_call_linked_function(gtvqString &command, vector<gtvqString> &parameters);
void gtvq_request_block_exit();

// Handlers
gtvqString load_module_handler(gtvqString &command, vector<gtvqString> &parameters);

#endif