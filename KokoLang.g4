grammar KokoLang;


program: (function nlo)+;

function: Sdot FUNC Id nlo funcblock;

funcblock: Bopen nlo local stack? sentence+ Bclose;

stack: Sdot STACK Number nlr;

local: Sdot LOCALS Number nlr;

sentence: (opcode | label) nlr;

label: Id Dp;

opcode: Id value? value?;

nlr: NL+;

nlo: NL*;

value: Null | bool | String | Number | Id;

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

STACK: 'stack';

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

Dp: ':';

Bopen: '{';

Bclose: '}';

Sdot: '.';

Scomment: '//';

Sminus: '-';

NL: '\n';

WS: [ \t\r]+ -> channel(HIDDEN);

ALL: .;