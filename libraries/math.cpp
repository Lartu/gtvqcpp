#include <iostream>
#include <map>
#include <stack>
#include <string>
#include "../gtvqStringAlt.h"

using namespace std;
typedef gtvqString (*gtvqHandler)(gtvqString &, std::vector<gtvqString> &);
typedef void (*gtvqLinkingHandler)(gtvqString command, gtvqHandler functionPointer);
typedef gtvqString (*gtvqCodeExecutor)(gtvqString &code);
typedef void (*gtvqRequestBlockExit)();
typedef void (*gtvqError)(string message);

gtvqError gtvq_error;
gtvqCodeExecutor gtvq_execute_code;
gtvqLinkingHandler gtvq_link_command_handler;
gtvqRequestBlockExit gtvq_request_block_exit;

bool isNumber(const std::string& str) {
    std::istringstream iss(str);
    double d;
    char c;
    return iss >> d && !(iss >> c);
}

string toString(double value)
{
    std::string str = to_string(value);
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    str.erase(str.find_last_not_of('.') + 1, std::string::npos);
    return str;
}

gtvqString add_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    return toString(stod(parameters[0]) + stod(parameters[1]));
}

gtvqString sub_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    return toString(stod(parameters[0]) - stod(parameters[1]));
}

gtvqString mul_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    return toString(stod(parameters[0]) * stod(parameters[1]));
}

gtvqString div_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    return toString(stod(parameters[0]) / stod(parameters[1]));
}

gtvqString intdiv_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    return toString(floor(stod(parameters[0]) / stod(parameters[1])));
}

gtvqString mod_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    long val_1 = stod(parameters[0]);
    long val_2 = stod(parameters[1]);
    return toString(val_1 % val_2);
}

gtvqString gt_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    if (stod(parameters[0]) > stod(parameters[1]))
    {
        return "t";
    }
    return "f";
}

gtvqString lt_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    if (stod(parameters[0]) < stod(parameters[1]))
    {
        return "t";
    }
    return "f";
}

gtvqString ge_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    if (stod(parameters[0]) >= stod(parameters[1]))
    {
        return "t";
    }
    return "f";
}

gtvqString le_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command + " expects 2 parameters (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    if (stod(parameters[0]) <= stod(parameters[1]))
    {
        return "t";
    }
    return "f";
}

gtvqString floor_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error(command + " expects 1 parameter (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]))
    {
        gtvq_error(command + " expects a numeric parameter.");
    }
    return toString(floor(stod(parameters[0])));
}

gtvqString ceil_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error(command + " expects 1 parameter (" + toString(parameters.size()) + " given)");
    }
    if(!isNumber(parameters[0]))
    {
        gtvq_error(command + " expects a numeric parameter.");
    }
    return toString(ceil(stod(parameters[0])));
}

extern "C" void gtvq_link_handlers(
    gtvqLinkingHandler linkCommandToHandler,
    gtvqCodeExecutor codeExecutor,
    gtvqError errorMessage,
    gtvqRequestBlockExit requestBlockExit)
{
    // Set up
    gtvq_error = errorMessage;
    gtvq_execute_code = codeExecutor;
    gtvq_link_command_handler = linkCommandToHandler;
    gtvq_request_block_exit = requestBlockExit;

    // Add Handlers
    gtvq_link_command_handler("+", add_handler);
    gtvq_link_command_handler("-", sub_handler);
    gtvq_link_command_handler("/", div_handler);
    gtvq_link_command_handler("*", mul_handler);
    gtvq_link_command_handler("//", intdiv_handler);
    gtvq_link_command_handler("%", mod_handler);
    gtvq_link_command_handler(">", gt_handler);
    gtvq_link_command_handler("<", lt_handler);
    gtvq_link_command_handler(">=", ge_handler);
    gtvq_link_command_handler("<=", le_handler);
    gtvq_link_command_handler("ceil", ceil_handler);
    gtvq_link_command_handler("floor", floor_handler);
}