#include <limits>
#include <algorithm>
#include <cmath>
#include "../gtvqStringAlt.h"

using namespace std;

#define GTVQ_PARAMETERS gtvqLinkingHandler linkCommandToHandler, gtvqCodeExecutor codeExecutor, gtvqError errorMessage, gtvqRequestBlockExit requestBlockExit
#define GTVQ_PAR_CALL linkCommandToHandler, codeExecutor, errorMessage, requestBlockExit

typedef gtvqString (*gtvqHandler)(gtvqString &, vector<gtvqString> &);
typedef void (*gtvqLinkingHandler)(gtvqString command, gtvqHandler functionPointer);
typedef gtvqString (*gtvqCodeExecutor)(gtvqString &code);
typedef void (*gtvqRequestBlockExit)();
typedef void (*gtvqError)(string message);

gtvqError gtvq_error;
gtvqCodeExecutor gtvq_execute_code;
gtvqLinkingHandler gtvq_link_command_handler;
gtvqRequestBlockExit gtvq_request_block_exit;

void gtvqLibSetup(GTVQ_PARAMETERS)
{
    gtvq_error = errorMessage;
    gtvq_execute_code = codeExecutor;
    gtvq_link_command_handler = linkCommandToHandler;
    gtvq_request_block_exit = requestBlockExit;
}

bool isNumber(const gtvqString &str)
{
    istringstream iss(str);
    double d;
    char c;
    return iss >> d && !(iss >> c);
}

string toString(double value)
{
    string str = to_string(value);
    str.erase(str.find_last_not_of('0') + 1, string::npos);
    str.erase(str.find_last_not_of('.') + 1, string::npos);
    return str;
}

int toInteger(gtvqString &str)
{
    return stoi(str);
}

double toDouble(gtvqString &str)
{
    return stod(str);
}

const char* toCString(gtvqString & str)
{
    return str.c_str();
}

void gtvqCheckArguments(gtvqString command, size_t min_arguments, size_t max_arguments, size_t argument_count)
{
    if (!(argument_count >= min_arguments && argument_count <= max_arguments))
    {
        if (min_arguments == max_arguments)
        {
            gtvq_error(command + " expects " + toString(min_arguments) + " parameters (" + toString(argument_count) + " given)");
        }
        else if (argument_count < min_arguments)
        {
            gtvq_error(command + " expects at least " + toString(min_arguments) + " parameters (" + toString(argument_count) + " given)");
        }
        else if (argument_count > max_arguments)
        {
            gtvq_error(command + " expects at most " + toString(max_arguments) + " parameters (" + toString(argument_count) + " given)");
        }
    }
}

void gtvqCheckArguments(gtvqString command, size_t min_arguments, size_t argument_count)
{
    gtvqCheckArguments(command, min_arguments, numeric_limits<size_t>::max(), argument_count);
}

bool isTrue(gtvqString &value)
{
    gtvqString_toLower(value);
    return value != "f" && value != "" && value != "0" && value != "false";
}
