#include "gtvq.h"

map<gtvqString, gtvqHandler> handlers;
bool exit_block_requested = false;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    gtvq_link_command_handler("link", load_module_handler);

    gtvqString source = gtvq_load_file(argv[1]);
    gtvq_execute_code(source);
    return 0;
}

void gtvq_error(string message)
{
    gtvq_error(message, "", 0);
}

void gtvq_error(string message, string filename, size_t line)
{
    cout << "gtvq error:" << endl;
    cout << message << endl;
    exit(1);
}

string gtvq_load_file(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        gtvq_error("Could not open file '" + filename + "'");
    }
    string line;
    string source = "";
    while (getline(file, line))
    {
        source += line + "\n";
    }
    file.close();
    return source;
}

gtvqString gtvq_execute_code(gtvqString &code)
{
    gtvqString_trim(code);
    vector<gtvqLine> lines = gtvq_tokenize_source(code);
    gtvqString last_returned_value = "";
    for (gtvqLine &line : lines)
    {
        last_returned_value = gtvq_execute_line(line);
        if (exit_block_requested)
        {
            exit_block_requested = false;
            break;
        }
    }
    return last_returned_value;
}

gtvqString gtvq_execute_line(gtvqLine &line)
{
    gtvqString command = "";
    vector<gtvqString> parameters;
    // First check for any sublines within the line
    for (gtvqWord &word : line.getWords())
    {
        if (word.getType() == GTVQSUBLINE)
        {
            gtvqString return_value = gtvq_execute_code(word.getValue());
            if (command.empty())
            {
                command = return_value;
            }
            else
            {
                parameters.push_back(return_value);
            }
        }
        else
        {
            if (command.empty())
            {
                command = word.getValue();
            }
            else
            {
                parameters.push_back(word.getValue());
            }
        }
    }
    // Try to call a linked function
    return gtvq_call_linked_function(command, parameters);
}

vector<gtvqLine> gtvq_tokenize_source(gtvqString source)
{
    source += ";";
    vector<gtvqLine> lines;
    gtvqLine current_line;
    bool next_char_is_escaped = false;
    gtvqString current_token = "";
    bool parsing_string = false;
    bool parsing_comment = false;
    bool parsing_line_comment = false;
    size_t block_quote_level = 0;
    size_t subline_level = 0;
    gtvqString last_two_chars = ";;";

    for (size_t i = 0; i < source.length(); ++i)
    {
        char character = source[i];
        last_two_chars = (string) "" + last_two_chars[1] + character;

        // Comments
        if (last_two_chars == "!*" && !parsing_string && !parsing_line_comment)
        {
            current_token = current_token.substr(1);
            parsing_comment = true;
        }
        else if (last_two_chars == "*!" && parsing_comment)
        {
            current_token = current_token.substr(0, current_token.length() - 1);
            parsing_comment = false;
        }
        // Line Comments
        else if (character == '#' && !parsing_string && !parsing_comment)
        {
            parsing_line_comment = true;
        }
        else if (character == '\n' && parsing_line_comment)
        {
            parsing_line_comment = false;
        }
        // Strings
        else if (character == '"' && !next_char_is_escaped && !parsing_comment && !parsing_line_comment && !parsing_string)
        {
            parsing_string = true;
            if (subline_level > 0 || block_quote_level > 0)
            {
                current_token += character;
            }
        }
        else if (character == '"' && !next_char_is_escaped && parsing_string)
        {
            parsing_string = false;
            if (subline_level > 0 || block_quote_level > 0)
            {
                current_token += character;
            }
            else
            {
                gtvqWord stringword = gtvqWord(current_token, GTVQWORD);
                current_line.addWord(stringword);
                // cout << "String: " << stringword.getValue() << endl;
                current_token = "";
            }
        }
        // Sublines
        else if (character == '(' && !parsing_comment && !parsing_line_comment && !parsing_string)
        {
            if (subline_level > 0 || block_quote_level > 0)
            {
                current_token += character;
            }
            subline_level += 1;
        }
        else if (character == ')' && !parsing_comment && !parsing_line_comment && !parsing_string)
        {
            subline_level -= 1;
            if (subline_level > 0 || block_quote_level > 0)
            {
                current_token += character;
            }
            if (subline_level == 0 && block_quote_level == 0)
            {
                gtvqString_trim(current_token);
                gtvqWord subline = gtvqWord(current_token, GTVQSUBLINE);
                current_line.addWord(subline);
                // cout << "Subline: " << subline.getValue() << endl;
                current_token = "";
            }
        }
        // Block Quotes
        else if (character == '{' && !parsing_comment && !parsing_line_comment && !parsing_string)
        {
            if (block_quote_level > 0 || subline_level > 0)
            {
                current_token += character;
            }
            block_quote_level += 1;
        }
        else if (character == '}' && !parsing_comment && !parsing_line_comment && !parsing_string)
        {
            block_quote_level -= 1;
            if (block_quote_level > 0 || subline_level > 0)
            {
                current_token += character;
            }
            if (subline_level == 0 && block_quote_level == 0)
            {
                gtvqString_trim(current_token);
                gtvqWord block_quote = gtvqWord(current_token, GTVQWORD);
                current_line.addWord(block_quote);
                // cout << "Block Quote: " << block_quote.getValue() << endl;
                current_token = "";
            }
        }
        // Whitespace
        else if ((isspace(character) || character == ';') && !parsing_comment && !parsing_line_comment && !parsing_string && subline_level == 0 && block_quote_level == 0)
        {
            // Push word
            gtvqString_trim(current_token);
            if (!current_token.empty())
            {
                gtvqWord word = gtvqWord(current_token, GTVQWORD);
                current_line.addWord(word);
                // cout << "Word: " << word.getValue() << endl;
            }
            current_token = "";

            // Push line
            if (character == '\n' || character == ';')
            {
                if (!current_line.getWords().empty())
                {
                    lines.push_back(current_line);
                    current_line = gtvqLine();
                    // cout << "NEW LINE!!!!!" << endl;
                }
            }
        }
        // Other characters
        else if (!parsing_comment && !parsing_line_comment)
        {
            if (subline_level == 0 && block_quote_level == 0 && parsing_string)
            {
                if (next_char_is_escaped)
                {
                    if (character == 'n')
                    {
                        character = '\n';
                    }
                    else if (character == 't')
                    {
                        character = '\t';
                    }
                    next_char_is_escaped = false;
                }
                else
                {
                    if (parsing_string && character == '\\')
                    {
                        next_char_is_escaped = true;
                        continue;
                    }
                }
            }
            current_token += character;
        }
    }

    // Check that the code is well written
    if (parsing_string)
    {
        gtvq_error("Missing \" - open \"...\"");
    }
    if (parsing_comment)
    {
        gtvq_error("Missing *! - open comment !*...*!");
    }
    if (subline_level > 0)
    {
        gtvq_error("Missing ) - open (...)");
    }
    if (block_quote_level > 0)
    {
        gtvq_error("Missing } - open {...}");
    }

    return lines;
}

gtvqString gtvq_call_linked_function(gtvqString &command, vector<gtvqString> &parameters)
{
    if (handlers.count(command) > 0)
    {
        return handlers[command](command, parameters);
    }
    else
    {
        gtvq_error("Command '" + command + "' not registered as a valid command.");
    }
    return "";
}

void gtvq_link_command_handler(gtvqString command, gtvqHandler functionPointer)
{
    handlers[command] = functionPointer;
}

void gtvq_request_block_exit()
{
    exit_block_requested = true;
}

gtvqString load_module_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    for (gtvqString &parameter : parameters)
    {
        string lib_name = "./" + parameter + ".gvdl";
        void *handle = dlopen(lib_name.c_str(), RTLD_LAZY);
        if (!handle)
        {
            char* error = dlerror();
            gtvq_error("Cannot link library: " + lib_name + (error? error : ""));
        }

        // Load the symbol
        dlerror(); // Reset errors
        gtvqLinkHandlers call_link_handlers = (gtvqLinkHandlers)dlsym(handle, "gtvq_link_handlers");
        const char *dlsym_error = dlerror();
        if (dlsym_error)
        {
            dlclose(handle);
            gtvq_error("Cannot load symbol 'gtvq_link_handlers': " + (string)dlsym_error);
        }
        call_link_handlers(gtvq_link_command_handler, gtvq_execute_code, gtvq_error, gtvq_request_block_exit);
    }
    return "";
}
