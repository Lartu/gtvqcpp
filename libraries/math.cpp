#include <iostream>
#include <map>
#include <stack>
#include <string>
#include "gtvq_utils.h"

gtvqString add_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, parameters.size());
    double result = 0;
    for (gtvqString &par : parameters)
    {
        if (!isNumber(par))
        {
            gtvq_error(command + " expects numeric parameters (" + par + ")");
        }else{
            result += toDouble(par);
        }
    }
    return toString(result);
}

gtvqString sub_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    if (!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    return toString(stod(parameters[0]) - stod(parameters[1]));
}

gtvqString mul_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    double result = 1;
    for (gtvqString &par : parameters)
    {
        if (!isNumber(par))
        {
            gtvq_error(command + " expects numeric parameters (" + par + ")");
        }else{
            result *= toDouble(par);
        }
    }
    return toString(result);
}

gtvqString div_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    if (!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    return toString(stod(parameters[0]) / stod(parameters[1]));
}

gtvqString intdiv_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    if (!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    return toString(floor(stod(parameters[0]) / stod(parameters[1])));
}

gtvqString mod_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    if (!isNumber(parameters[0]) || !isNumber(parameters[1]))
    {
        gtvq_error(command + " expects numeric parameters.");
    }
    long val_1 = stod(parameters[0]);
    long val_2 = stod(parameters[1]);
    return toString(val_1 % val_2);
}

gtvqString gt_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    if (!isNumber(parameters[0]) || !isNumber(parameters[1]))
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
    gtvqCheckArguments(command, 2, 2, parameters.size());
    if (!isNumber(parameters[0]) || !isNumber(parameters[1]))
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
    gtvqCheckArguments(command, 2, 2, parameters.size());
    if (!isNumber(parameters[0]) || !isNumber(parameters[1]))
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
    gtvqCheckArguments(command, 2, 2, parameters.size());
    if (!isNumber(parameters[0]) || !isNumber(parameters[1]))
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
    gtvqCheckArguments(command, 1, 1, parameters.size());
    if (!isNumber(parameters[0]))
    {
        gtvq_error(command + " expects a numeric parameter.");
    }
    return toString(floor(stod(parameters[0])));
}

gtvqString ceil_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    if (!isNumber(parameters[0]))
    {
        gtvq_error(command + " expects a numeric parameter.");
    }
    return toString(ceil(stod(parameters[0])));
}

extern "C" void gtvq_link_handlers(GTVQ_PARAMETERS)
{
    // Set up
    gtvqLibSetup(GTVQ_PAR_CALL);

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