# Syntax and Parsing

The lenguage is parsed in the following way:
1. A source code is obtained. That source code is converted into a string.
2. The string is separated into lines. Lines start at the beginning of a line and end at the first \n or ; found.
3. The first line is parsed into words. Words are separated by whitespace. `this is a line` will be split into `this`, `is`, `a` and `line`. Words can be surrounded by `"` or `{}` to prevent them from being split or to prevent a line from ending at a \n or ; found within them. For example: `this "is a line"` will be split into `this` and `is a line`. Likewise, `This line {doesn't; end}` will be split into `This`, `line` and `doesn't; end`. `"` quoting allows you to add some escaped characters, like `\n` and `\t`, while `{}` quoting (*block quoting*) leaves your source as-is.
4. Comments can be surrounded by `!` to prevent gtvq from seeing them. `this !is a! line` will be split into `this` and `line`, the comment is ignored.
5. Words surrounded by `()` are executed before the rest of the line (just like any other line would be executed), and they are replaced by their return value before the line executes. We call these *sublines*, as they are lines within a line.
6. Once the line has been split into words and its sublines have been executed, the first word is set to be the *command*, while the rest of the words are the *parameters*. gtvq looks for the command in a lookup table that links commands to function pointers (called *handlers*). Lookup searchs are *case-sensitive*. If a match is found, the handler is executed with the *parameters* as its parameters. If a match isn't found, an error is displayed.
6. Then the following line of the source is parsed like in point 3, and so on, and so on.

So, you might be wondering, if that's the whole syntax of the language, how can I do *X*? As you can see, the language doesn't include any built-in commands to do anything. gtvq just provides you with an interpreter that can interpret source code and act upon it, but no specific behaviours are defined.

This is where **gtvq linking** comes into place.

# gtvq linking

In gtvq, we use the term *linking* to describe the association of a function pointer (called a *handler*) to a particular word (to be used as a command, as descrived in the previous section). The gtvq interpreter provides utility functions to allow you to define all the commands you need.

(TODO, explain how to link and the provided functions)

# Foreign Function Invocation in gtvq

(TODO)