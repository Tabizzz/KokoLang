 # KokoLang
This file is an introduction to kokolang.

## Hello World
let's start writing a hello world:
```kl
.using ksl.inout
.func main
{
    .locals 1
    .args -1
    push "Hello world" %0
    call out %-0 %0
}
```

## Programming

### Types

Kokolang is dynamically typed, this means that it is possible to define variables, and they can change their value 
and value type at runtime without any problem, you can also create new data structures.

Kokolang also comes with builtin types that can be used directly in push statements, and it is not necessary to create
instances directly, these types are:

| Name		  | Codename	 | Description	                           | Range		                                                   | Example	                           |
|---------|-----------|----------------------------------------|-----------------------------------------------------------|------------------------------------|
| int 		  | `int`		   | A integer numeric value	               | -9.223.372.036.854.775.808 to 9.223.372.036.854.775.807	  | `10`, `0`, `-1201`                 |
| float		 | `flt`		   | A double floating point numeric value	 | 1.7E-308 to 1.7E+308 (15 digits)	                         | `10.7`, `0.0`, `-2313.131`         |
| bool 		 | `bit`		   | A boolean value	                       | 1 and 0, true and false	                                  | `true`, `false`                    |
| string	 | `str`		   | A string value	                        | Any string test between "" excluding caracters \n and \r	 | `"code"`, `"foo"`, `"hello world"` |
###### *the code name is the name of the type in code, for example to use with the `is` opcode

Kokolang also have a special type called `ptr` to hold pointers, use this type to hold generic pointers without need to 
create custom types, this type cant be directly pushed in a register to see how to use check the opcodes related to pointers.

### Package
Kokolang programs are divided by packages, a package contain definitions for functions, types and variables and can also
contain subpackages.

In kl a package is created by placing code files inside a folder with the name of the package, this is different only 
with the main package since it always has the same name and its directory is the working directory which can have any
name.

A package can contain definitions and custom metadata, to add a definition you use one of the following keywords:

#### Metadata
Metadata is defined with two dots `..` followed by the key of the data and then the value like this:

`..mydata [values]*`

A metadata key can contain zero or multiples values of type string, float, int, bool and null.

The key can be anything you want to add as metadata.

example:

`..version <number>`: set the version of the package you can have different versions of the same package.
`..runtimevars`: set the package to use dynamic var, that is, if a program try to get or set a not existing var then
it will be defined at runtime.

#### Definitions:
Definitions are created with a single dot `.` and the type of definitions, you cant create custom definitions, you can
only the following 4 definitions:

`.var`: add a definition of a variable to the package.

`.func`: add a definition of a function to the package.

`.type`: add a definition of a type to the package.

`.pack`: add a defintion of a subpackage to the package.

###### *each definition is explained in separate sections bellow

### Variables
A variable can hold any value and can be changed at any time.
To define a variable you need to use this syntax:

```kl
.var <name>
{
	[metadata]*
}
```

The metadata block is optional, so you can define variables without the block:

```kl
.var <name>
```

### Functions
To create a function you need to use this syntax:
```kl
.func <name>
{
	[metadata]+
	[.args <int>]
	[.margs <int>]
	[.locals <int>]
	instructions+
}
```
`name`: the identifier of your function, must be unique.

`args`: how many arguments your function expects, if not specified or if set to a negative value then the function can
receive any number of parameters.

`margs`: the minimum amount of arguments that can be passed to the function, by default is 0.

`locals`: how many registers will your function have, if not specified it will be 1 by default.

`instructions`: the list of op codes of your function. A function must have at least a ret opcode or a runtime error
can occur.

### opcodes
kokolang has a list of opcodes to operate with vars and the stack:

you can see the list of opcodes and the docs in the file [KOpcode.h](https://github.com/Tabizzz/KokoLang/blob/master/src/include/Runtime/KOpcode.h)