%option noyywrap nounput
%option caseless

%{
#include "parser.h"

int fileno(FILE *);
int yyerror(char *);
int line_num = 1;
int lexerror;
%}

linefeed   \n
comment    "//".*\n

%%

"("  { return OPEN_PAREN;       }
")"  { return CLOSE_PAREN;      }
"{"  { return OPEN_BRACE;       }
"}"  { return CLOSE_BRACE;      }
"["  { return OPEN_BRACKET;     }
"]"  { return CLOSE_BRACKET;    }
":"  { return COLON;            }
";"  { return SEMI_COLON;       }
"="  { return ASSIGN;           }
"!"  { return NOT;              }
"!=" { return DIFFERENT;        }
"+"  { return PLUS;             }
"-"  { return MINUS;            }
"*"  { return MULTIPLY;         }
"/"  { return DIV;              }
","  { return COMMA;            }
"&&" { return AND;              }
"||" { return OR;               }
"<=" { return LESS_OR_EQUAL;    }
"<"  { return LESS;             }
"==" { return EQUAL;            }
">=" { return GREATER_OR_EQUAL; }
">"  { return GREATER;          }

"else"      { return ELSE;      }
"functions" { return FUNCTIONS; }
"goto"      { return GOTO;      }
"if"        { return IF;        }
"labels"    { return LABELS;    }
"return"    { return RETURN;    }
"types"     { return TYPES;     }
"var"       { return VAR;       }
"vars"      { return VARS;      }
"void"      { return VOID;      }
"while"     { return WHILE;     }

"integer" { return IDENTIFIER; }
"boolean" { return IDENTIFIER; }
"true"    { return IDENTIFIER; }
"false"   { return IDENTIFIER; }
"read"    { return IDENTIFIER; }
"write"   { return IDENTIFIER; }

<<EOF>> { return END_OF_FILE; }

[0-9]+               { return INTEGER;    }
[a-z]+([0-9]|[a-z])* { return IDENTIFIER; }

{linefeed}   { line_num++; }
{comment}    { line_num++; }

[[:space:]]  {                                        }
[[:^space:]] { lexerror=1; return LEXICAL_ERROR; }

%%
