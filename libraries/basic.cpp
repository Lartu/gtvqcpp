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

class gtvqFunction
{
private:
    vector<gtvqString> parameters;
    gtvqString body;

public:
    void addParameter(gtvqString &parameter)
    {
        parameters.push_back(parameter);
    }
    void setBody(gtvqString fbody)
    {
        body = fbody;
    }
    vector<gtvqString> &getParameters()
    {
        return parameters;
    }
    gtvqString &getBody()
    {
        return body;
    }
};

// variable scope container
vector<map<gtvqString, gtvqString>> variables;
map<gtvqString, gtvqFunction> functions;
bool return_requested = false;

void add_scope()
{
    map<gtvqString, gtvqString> newScope;
    variables.push_back(newScope);
}

void del_scope()
{
    variables.pop_back();
}

void set_variable(gtvqString name, gtvqString value)
{
    variables.back()[name] = value;
}

// join handler - joins multiple values into a single string
gtvqString join_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqString result = "";
    for (gtvqString &str : parameters)
    {
        result = result + str;
    }
    return result;
}

// display handler - display a value on the screen. Returns an empty string.
gtvqString display_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    for (gtvqString &str : parameters)
    {
        cout << str << flush;
    }
    cout << endl;
    return "";
}

// set handler - set a variable. Returns the value of the variable.
gtvqString set_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error("set expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    set_variable(parameters[0], parameters[1]);
    return parameters[1];
}

// get handler - get a variable. Returns the value of the variable.
// throws an error if the variable wasn't found
gtvqString get_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error("get expects 1 parameter (" + to_string(parameters.size()) + " given)");
    }
    gtvqString var_name = parameters[0];
    for (size_t i = variables.size() - 1; i >= 0; i--)
    {
        if (variables[i].count(var_name) > 0)
        {
            return variables[i][var_name];
        }
        if (i == 0)
            break;
    }
    gtvq_error("Variable " + var_name.str_rep() + " doesn't exist.");
    return "";
}

// global set handler - set a global variable. Returns the value of the variable.
gtvqString glset_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error("glset expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    variables[0][parameters[0]] = parameters[1];
    return parameters[1];
}

// global get handler - get a global variable. Returns the value of the variable.
// throws an error if the variable wasn't found
gtvqString glget_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error("glget expects 1 parameter (" + to_string(parameters.size()) + " given)");
    }
    gtvqString var_name = parameters[0];
    if (variables[0].count(var_name) > 0)
    {
        return variables[0][var_name];
    }
    gtvq_error("Global variable " + var_name.str_rep() + " doesn't exist.");
    return "";
}

// exec - executes a string as gtvq code
// returns whatever the code returns.
gtvqString exec_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error("exec expects 1 parameter (" + to_string(parameters.size()) + " given)");
    }
    return gtvq_execute_code(parameters[0]);
}

// exit - exits the program with an exit code
gtvqString exit_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() > 1)
    {
        gtvq_error("exit expects 1 parameter or less (" + to_string(parameters.size()) + " given)");
    }
    int exit_code = 0;
    if (parameters.size() > 0)
    {
        if (parameters[0].isNumber())
        {
            exit_code = (int)parameters[0].getNumber();
        }
    }
    exit(exit_code);
    return "";
}

// call function handler, calls a function when its name is used as a word.
// returns whatever the function returns.
gtvqString call_function(gtvqString &command, vector<gtvqString> &parameters)
{
    add_scope();
    gtvqString return_value = "";
    if (functions.count(command) > 0)
    {
        gtvqFunction &function = functions[command];
        if (parameters.size() != function.getParameters().size())
        {
            gtvq_error(command.str_rep() + " expects " + to_string(function.getParameters().size()) + " parameter(s) (" + to_string(parameters.size()) + " given)");
        }
        for (size_t i = 0; i < function.getParameters().size(); ++i)
        {
            set_variable(function.getParameters()[i], parameters[i]);
        }
        return_value = gtvq_execute_code(function.getBody());
    }
    else
    {
        gtvq_error("The function '" + command.str_rep() + "' doesn't exist.");
    }
    del_scope();
    return return_value;
}

// def - defines a function. Returns the name of the function.
gtvqString def_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters[1] != ":")
    {
        gtvq_error("def usage: def <name> : <parameters> <body>");
    }
    gtvqString function_name = parameters[0];
    gtvqFunction new_function;
    new_function.setBody(parameters.back());
    for (size_t i = 2; i < parameters.size() - 1; ++i)
    {
        new_function.addParameter(parameters[i]);
    }
    functions[function_name] = new_function;
    gtvq_link_command_handler(function_name, call_function);
    return function_name;
}

// for - for loop. Returns the last value of the cycle.
gtvqString for_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 6)
    {
        gtvq_error("for expects 6 parameter (" + to_string(parameters.size()) + " given)");
    }

    if (parameters[1] != "from" || parameters[3] != "to")
    {
        gtvq_error("def usage: for <var> from <min (included)> to <max (excluded)> <body>");
    }

    if (!parameters[2].isNumber() || !parameters[4].isNumber())
    {
        gtvq_error("for expects numeric min and max values.");
    }

    double min_value = parameters[2].getNumber();
    double max_value = parameters[4].getNumber();

    size_t increment = 1;
    if (max_value < min_value)
        increment = -1;

    long i;

    for (long i = min_value; i < max_value; i += increment)
    {
        set_variable(parameters[0], (double)i);
        gtvq_execute_code(parameters[5]);
    }

    return (double)i;
}

// skip - exits the current block. Returns the passed value or "".
gtvqString skip_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvq_request_block_exit();
    if (parameters.size() > 1)
    {
        gtvq_error("skip expects 1 or less (" + to_string(parameters.size()) + " given)");
    }
    else if (parameters.size() == 1)
    {
        return parameters[0];
    }
    return "";
}

bool isTrue(gtvqString &value)
{
    gtvqString_toLower(value);
    return value != "f" && value != "" && value != "0" && value != "false";
}

// if - if selector. Returns the value of the executed branch or "".
gtvqString if_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2 && parameters.size() != 4)
    {
        gtvq_error("if expects 2 or 3 parameters (" + to_string(parameters.size()) + " given)");
    }
    gtvqString &conditional = parameters[0];
    gtvqString &true_branch = parameters[1];
    gtvqString result = gtvq_execute_code(conditional);
    if (parameters.size() == 4)
    {
        if (parameters[2] != "else")
        {
            gtvq_error("if usage: if <condition> <true branch> [else <false branch>]");
        }
    }
    if (isTrue(result))
    {
        return gtvq_execute_code(true_branch);
    }
    else
    {
        if (parameters.size() == 4)
        {
            gtvqString &false_branch = parameters[3];
            return gtvq_execute_code(false_branch);
        }
    }
    return "";
}

// equality check - returns t if the values are equal, otherwise f
gtvqString eq_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error("= expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if (parameters[0] == parameters[1])
    {
        return "t";
    }
    return "f";
}

// inequality check - returns f if the values are equal, otherwise t
gtvqString neq_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error("!= expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if (parameters[0] == parameters[1])
    {
        return "f";
    }
    return "t";
}

// and - returns t if all the values are true, otherwise f
gtvqString and_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error("and expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if (isTrue(parameters[0]) && isTrue(parameters[1]))
    {
        return "t";
    }
    return "f";
}

// or - returns t if at least one value is true, otherwise f
gtvqString or_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error("or expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    if (isTrue(parameters[0]) || isTrue(parameters[1]))
    {
        return "t";
    }
    return "f";
}

// not - returns t if the value is false, or f is the value is true
gtvqString not_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error("not expects 1 parameter (" + to_string(parameters.size()) + " given)");
    }
    if (isTrue(parameters[0]))
    {
        return "f";
    }
    return "t";
}

// varex - returns t if the variable exists, else f
gtvqString varex_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error("varex expects 1 parameter (" + to_string(parameters.size()) + " given)");
    }
    gtvqString var_name = parameters[0];
    for (size_t i = variables.size() - 1; i >= 0; --i)
    {
        if (variables[i].count(var_name) > 0)
        {
            return "t";
        }
        if (i == 0)
            break;
    }
    return "f";
}

// val - returns the value passed to it
gtvqString val_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 1)
    {
        gtvq_error("vl expects 1 parameter (" + to_string(parameters.size()) + " given)");
    }
    return parameters[0];
}

// while - while selector. Returns "".
gtvqString while_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    if (parameters.size() != 2)
    {
        gtvq_error("while expects 2 parameters (" + to_string(parameters.size()) + " given)");
    }
    gtvqString &conditional = parameters[0];
    gtvqString &body = parameters[1];
    while (true)
    {
        gtvqString returnValue = gtvq_execute_code(conditional);
        if (!isTrue(returnValue))
        {
            break;
        }
        else
        {
            gtvq_execute_code(body);
        }
    }
    return "";
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

    // Add the global scope
    add_scope();

    // Add Handlers
    gtvq_link_command_handler("display", display_handler);
    gtvq_link_command_handler("set", set_handler);
    gtvq_link_command_handler("get", get_handler);
    gtvq_link_command_handler("glset", glset_handler);
    gtvq_link_command_handler("glget", glget_handler);
    gtvq_link_command_handler("exec", exec_handler);
    gtvq_link_command_handler("exit", exit_handler);
    gtvq_link_command_handler("def", def_handler);
    gtvq_link_command_handler("for", for_handler);
    gtvq_link_command_handler("while", while_handler);
    gtvq_link_command_handler("skip", skip_handler);
    gtvq_link_command_handler("if", if_handler);
    gtvq_link_command_handler("=", eq_handler);
    gtvq_link_command_handler("!=", neq_handler);
    gtvq_link_command_handler("or", or_handler);
    gtvq_link_command_handler("and", and_handler);
    gtvq_link_command_handler("not", not_handler);
    gtvq_link_command_handler("val", val_handler);
    gtvq_link_command_handler("varex", varex_handler);
    gtvq_link_command_handler("join", join_handler);
}