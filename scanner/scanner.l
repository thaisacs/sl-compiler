%{
#include "parser.h"
#include <stdio.h>
%}

%option caseless

%%

"("  { printf("%i\n", OPEN_PAREN);       }
")"  { printf("%i\n", CLOSE_PAREN);      }
"{"  { printf("%i\n", OPEN_BRACE);       }
"}"  { printf("%i\n", CLOSE_BRACE);      }
"["  { printf("%i\n", OPEN_BRACKET);     }
"]"  { printf("%i\n", CLOSE_BRACKET);    }
":"  { printf("%i\n", COLON);            }
";"  { printf("%i\n", SEMI_COLON);       }
"="  { printf("%i\n", ASSIGN);           }
"!"  { printf("%i\n", NOT);              }
"!=" { printf("%i\n", DIFFERENT);        }
"+"  { printf("%i\n", PLUS);             }
"-"  { printf("%i\n", MINUS);            }
"*"  { printf("%i\n", MULTIPLY);         }
"/"  { printf("%i\n", DIV);              }
","  { printf("%i\n", COMMA);            }
"&&" { printf("%i\n", AND);              }
"||" { printf("%i\n", OR);               }
"<=" { printf("%i\n", LESS_OR_EQUAL);    }
"<"  { printf("%i\n", LESS);             }
"==" { printf("%i\n", EQUAL);            }
">=" { printf("%i\n", GREATER_OR_EQUAL); }
">"  { printf("%i\n", GREATER);          }

"else" { printf("%i\n", ELSE);           }
"functions" { printf("%i\n", FUNCTIONS); }

"integer" { printf("%i\n", IDENTIFIER); }
"boolean" { printf("%i\n", IDENTIFIER); }
"true"    { printf("%i\n", IDENTIFIER); }
"false"   { printf("%i\n", IDENTIFIER); }
"read"    { printf("%i\n", IDENTIFIER); }
"write"   { printf("%i\n", IDENTIFIER); }


[0-9]+               { printf("%i\n", INTEGER);    }
[a-z]+([0-9]|[a-z])* { printf("%i\n", IDENTIFIER); }

[[:space:]]
[[:^space:]]

%%

int main() {
  yylex();
  return 0;
}
