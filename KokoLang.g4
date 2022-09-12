grammar KokoLang;


program: (function nlo)+;

function: Sdot FUNC Id nlo funcblock;

funcblock: Bopen nlo local stack? sentence+ Bclose;

stack: Sdot STACK Number nlr;

local: Sdot LOCALS Number nlr;

sentence: (opcode | label) nlr;

label: Id Dp;

opcode: Id value?;

nlr: NL+;

nlo: NL*;

value: Null | Bool | String | Number | Id;

// / instructions / //

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

// / lang values / // 

Null: 'null';

Bool: 'true' | 'false';

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