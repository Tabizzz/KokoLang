grammar KokoLang;


program: (function nlo)+;

function: Sdot FUNC Id nlo funcblock;

funcblock: Bopen nlo funcattrs sentence+ Bclose;

funcattrs: funcattr*;

funcattr : local | margs | args;

local: Sdot LOCALS Number nlr;

args: Sdot ARGS Number nlr;

margs: Sdot MARGS Number nlr;

sentence: (opcode | label) nlr;

label: Dp Id;

opcode: Id value*;

nlr: NL+;

nlo: NL*;

value: Null | bool | String | register | Number | Id;

register: Sper Number;

bool: True | False;

// / from here are the token definitions for the lexer / //

COMMENT: Scomment ANY* -> channel(HIDDEN);

// / fragments / //

fragment LOWER: [a-z];

fragment UPPER: [A-Z];

fragment LETTER: LOWER | UPPER;

fragment DIGIT: [0-9];

fragment ANY: ~[\r\n];

// / lang keywords / //

ARGS: 'args';

MARGS: 'marg';

LOCALS: 'locals';

FUNC: 'func';

VAR: 'var';

STRUCT: 'struct';

IMPORT: 'import';

INCLUDE: 'include';

// / lang values / // 

Null: 'null';

True: 'true';

False: 'false';

Id: LETTER+;

String: '"' ANY*? '"';

Number: '-'? DIGIT+ ('.' DIGIT+)?;

// / lang operators / //

// / lang simbols / //

Sper: '%';

Dp: ':';

Bopen: '{';

Bclose: '}';

Sdot: '.';

Scomment: '//';

Sminus: '-';

NL: '\n';

WS: [ \t\r]+ -> channel(HIDDEN);

ALL: .;