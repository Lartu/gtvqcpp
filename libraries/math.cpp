#include <iostream>
#include <map>
#include <stack>
#include <string>
#include "../gtvqString.h"

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

gtvqString add_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    return parameters[0].getNumber() + parameters[1].getNumber();
}

gtvqString sub_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    return parameters[0].getNumber() - parameters[1].getNumber();
}

gtvqString mul_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    return parameters[0].getNumber() * parameters[1].getNumber();
}

gtvqString div_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    return parameters[0].getNumber() / parameters[1].getNumber();
}

gtvqString intdiv_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    return floor(parameters[0].getNumber() / parameters[1].getNumber());
}

gtvqString mod_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    long val_1 = parameters[0].getNumber();
    long val_2 = parameters[1].getNumber();
    return (double)(val_1 % val_2);
}

gtvqString gt_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    if (parameters[0].getNumber() > parameters[1].getNumber())
    {
        return "t";
    }
    return "f";
}

gtvqString lt_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    if (parameters[0].getNumber() < parameters[1].getNumber())
    {
        return "t";
    }
    return "f";
}

gtvqString ge_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    if (parameters[0].getNumber() >= parameters[1].getNumber())
    {
        return "t";
    }
    return "f";
}

gtvqString le_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error(command.str_rep() + " expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber() || !parameters[1].isNumber())
    {
        gtvq_error(command.str_rep() + " expects numeric parameters.");
    }
    if (parameters[0].getNumber() <= parameters[1].getNumber())
    {
        return "t";
    }
    return "f";
}

gtvqString floor_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error(command.str_rep() + " expects 1 parameter (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber())
    {
        gtvq_error(command.str_rep() + " expects a numeric parameter.");
    }
    return (double)(floor(parameters[0].getNumber()));
}

gtvqString ceil_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error(command.str_rep() + " expects 1 parameter (" + to_string(parameters.size()) + " given)");
    }
    if(!parameters[0].isNumber())
    {
        gtvq_error(command.str_rep() + " expects a numeric parameter.");
    }
    return (double)(ceil(parameters[0].getNumber()));
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