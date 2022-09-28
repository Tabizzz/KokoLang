# KokoLang
This file is a introduction to kokolang.

## Hello World
let's start writing a hello world:
```kl
.import inout
.main
{
    push "Hello world"
    call out
}
```

## Programming

### Types

Kokolang is a dynamically typed language, this means that it is possible to define variables and they can change their value and value type at runtime without any problem, you can also create new data structures.

Kokolang also comes with 4 builtin types that can be used directly in push statements and it is not necessary to create instances directly, these types are:

| Name		| Codename	| Description	| Range		| Example	|
| -------	| ------	| ------		| ------	| ------	|
| int 		| `int`		| A integer numeric value	| -2.147.483.648 to 2.147.483.647	| `10`, `0`, `-1201` |
| float		| `flt`		| A double floating point numeric value	|  1.7E-308 to 1.7E+308 (15 digits)	| `10.7`, `0.0`, `-2313.131` |
| bool 		| `bit`		| A boolean value	| 1 and 0, true and false	| `true`, `false` |
| string	| `str`		| A string value	| Any string test between "" excluding caracters \n and \r	| `"code"`, `"foo"`, `"hello world"` |
###### *the code name is the name of the type in code, for example to use with the `is` opcode

Kokolang also have a special type called `ptr` to hold pointers, use this type to hold generic pointers without need to create custom types, this type cant be directly pushed in the stack to to see hot to use check the opcodes related to pointers.

### Functions
To create a function you need to use this syntax:
```kl
.func <name>
{
	[.args <int>]
	[.locals <int>]
	[.stack <int>]
	instructions+
}
```
`name`: the identifier of your function, must be unique.

`args`: how many arguments your function expects, if not specified, it means that the function can receive any number of parameters, it is recommended to use a fixed number since if not specified, when calling the function, then the entire stack of the caller function will be poped to be passed to the called function and this may not be expected behavior.

`locals`: how many local variables will your function have, if not specified it will be 0 by default.

`stack`: the maximum capacity of the stack of your function, if it is not specified the stack will have no limit.

`instruction`: the list of op codes of your function.

### opcodes
kokolang has a list of opcodes to operate with vars and the stack:

you can see the list of opcodes and the docs in the file [KOpcode.h](https://github.com/Tabizzz/KokoLang/blob/master/src/include/Runtime/KOpcode.h)