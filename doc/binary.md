# KokoLang Binary Format

The kokolang runtime can only load native library files that extends the runtime or kokolang binary files that include
definitions to add to the runtime.

kokolang binary files (.kll) are just a group of definitions that are member of a package:


## Header 
The file start header, contains general info about the package:

| Offset    | Bytes    | Description               |
|-----------|----------|---------------------------|
| 0         | 2        | file signature = 0x4b4c   |
| 2         | 2        | the package version       |
| 4         | 1        | package name size (n)     |
| 5         | 1        | author name size (m)      |
| 6         | n        | package name              |
| 6 + n     | m        | author name               |
| 6 + n + m | variable | metadata block            |

### Metadata blocks

metadatablocks are used to store custom info inside definitions, metadata works on key-value pairs where the keys are 
strings, but the values can be ints, doubles, bools, strings or null.

| Offset | Bytes    | Description               |
|--------|----------|---------------------------|
| 0      | 2        | size of block, min 3      |
| 2      | 1        | number of metadata values |
| 3      | variable | key value pairs           |

every entry in the metadata is a string followed by a value.

#### Metadata Key-Value pairs
The binary format can have dynamic values of 5 types, these value are writen in binary with a code follow by the actual
value:

| value type | code | bytes | extra bytes info                                                                    |
|------------|------|-------|-------------------------------------------------------------------------------------|
| null       | 0x00 | 1     |
| false      | 0x01 | 1     |
| true       | 0x02 | 1     |
| integer    | 0x03 | 9     |
| double     | 0x04 | 9     |
| string     | 0x05 | var   | a string is preceded by a 1 byte integer <br/>that indicates the size of the string |


## Definitions

after the main header of the file come the definitions, a definition starts with an id code followed by the body of 
the definition

### Close definition

this is not an actual definition, it is just a code used to indicate that the definitions have finished for the current
package

| Offset | Bytes | Description           |
|--------|-------|-----------------------|
| 0      | 1     | definition code = 0x0 |

### Variable definition

adds a variable to the package with an optional default value

| Offset      | Bytes    | Description              |
|-------------|----------|--------------------------|
| 0           | 1        | definition code = 0x1    |
| 1           | 1        | name size (n)            |
| 2           | 1        | has default value        |
| 3           | n        | name of the variable     |
| 3 + n       | var      | default value if present |
| 3 + n + var | variable | metadata block           |

### Function definition

adds a function definition to the package

| Offset      | Bytes    | Description           |
|-------------|----------|-----------------------|
| 0           | 1        | definition code = 0x2 |
| 1           | 1        | name size (n)         |
| 2           | 1        | min arg count         |
| 3           | 1        | max arg count         |
| 4           | 1        | register count        |
| 5           | 2        | function size         |
| 7           | n        | function name         |
| 7 + n       | var      | function body         |
| 7 + n + var | variable | metadata block        |

#### Function Body

the function body is a list of instructions that are executed in order when the function run

| Offset | Bytes | Description    |
|--------|-------|----------------|
| 0      | 1     | opcode         |
| 1      | 2     | operands count |
| 3      | 1     | label size (n) |
| 4      | n     | label          |
| 4 + n  | var   | operands       |

operands are like metadata values but strings use a 4 bytes integer to determine the size and there is an extra 
code 0x06 that works like ints and is used for registers.

### Type Definition

A type is like a mini-package, it can contain variables and other functions, but unlike packages, types must be 
instantiated to access them.

| Offset | Bytes    | Description           |
|--------|----------|-----------------------|
| 0      | 1        | definition code = 0x3 |
| 1      | 1        | type name size (n)    |
| 2      | n        | type name             |
| 2 + n  | variable | metadata block        |

then the type is followed by variable or function definitions and wnd with a close definition.

function defined inside types must have at least 1 min arg, this args is the instance itself calling the method.

### Subpackage definition

A sub-package y a package inside another package and can contain any type of definitions.

| Offset | Bytes    | Description           |
|--------|----------|-----------------------|
| 0      | 1        | definition code = 0x3 |
| 1      | 1        | package name size (n) |
| 2      | n        | package name          |
| 2 + n  | variable | metadata block        |