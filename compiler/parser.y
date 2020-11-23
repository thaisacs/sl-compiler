%{
#include <stdio.h>
#include "tree.h"

char *yytext;
int yylex();
int yyerror();
%}

%token EQUAL
%token DIFFERENT
%token LESS
%token LESS_OR_EQUAL
%token GREATER
%token GREATER_OR_EQUAL
%token PLUS
%token MINUS
%token OR
%token MULTIPLY
%token DIV
%token AND
%token CLOSE_BRACE
%token CLOSE_BRACKET
%token CLOSE_PAREN
%token COLON
%token COMMA
%token ELSE
%token END_OF_FILE
%token FUNCTIONS
%token GOTO
%token IDENTIFIER
%token ASSIGN
%token IF
%token INTEGER
%token LABELS
%token NOT
%token OPEN_BRACE
%token OPEN_BRACKET
%token OPEN_PAREN
%token RETURN
%token SEMI_COLON
%token TYPES
%token VAR
%token VARS
%token VOID
%token WHILE
%token LEXICAL_ERROR

%%

program: function END_OF_FILE { return 0; }
       ;

function: function_header block { genNode(C_FUNCTION, 4); }
        ;

function_header: VOID { genEmpty(); } identifier formal_parameters
               | identifier identifier formal_parameters
               ;

block: labels types variables functions body { genNode(C_BLOCK, 5); }
     ;

body: OPEN_BRACE statement_list CLOSE_BRACE
    | OPEN_BRACE CLOSE_BRACE { genEmpty(); }
    ;

labels: LABELS identifier_list SEMI_COLON
      | empty
      ;

types: TYPES types_definition
     | empty
     ;

types_definition: identifier ASSIGN type SEMI_COLON { genNode(C_TYPES, 2); }
                | types_definition identifier ASSIGN type SEMI_COLON { genNode(C_TYPES, 2); insertTopList(); }
                ;

variable_declarations: identifier_list COLON type SEMI_COLON { genNode(C_VARS, 2); }
                     | variable_declarations identifier_list COLON type SEMI_COLON { genNode(C_VARS, 2); insertTopList(); }
                     ;

variables: VARS variable_declarations
         | empty
         ;

functions: FUNCTIONS function_list
         | empty
         ;

function_list: function_list function { insertTopList(); }
             | function
             ;

type: identifier type_array
    ;

type_array:
          | type_array OPEN_BRACKET integer CLOSE_BRACKET
          ;

formal_parameters: OPEN_PAREN formal_parameter_opt CLOSE_PAREN
                 ;

formal_parameter_opt: formal_parameter_list
                    | empty
                    ;

formal_parameter_list: formal_parameter_list COMMA formal_parameter { insertTopList(); }
                     | formal_parameter
                     ;

formal_parameter: expression_parameter
                | function_parameter
                ;

function_parameter: function_header { genNode(C_PARAM, 3); }
                  ;

expression_parameter: VAR identifier_list COLON identifier { genNode(C_PARAM, 2); }
                    | identifier_list COLON identifier     { genNode(C_PARAM, 2); }
                    ;

statement_list: statement_list statement { insertTopList(); }
              | statement
              ;

statement: unlabeled_statement
         | identifier COLON unlabeled_statement { genNode(C_LABEL, 2); }
         ;

unlabeled_statement: assignment
                   | compound
                   | function_call_statement
                   | goto
                   | return
                   | empty_statement
                   | repetitive
                   | conditional
                   ;

assignment: variable ASSIGN expression SEMI_COLON { genNode(C_ASSIGN, 2);}
          ;

empty_statement: SEMI_COLON { genEmpty(); }
               ;

variable: identifier { genNode(C_VAR, 1); }
        ;
/*
variable_expression: variable_expression OPEN_BRACKET expression CLOSE_BRACKET { insertTopList(); }
                   | empty
                   ;
*/
expression_opt: expression_list
              | empty
              ;

expression_list: expression_list COMMA expression { insertTopList(); }
               | expression
               ;

expression: simple_expression expression_end { genNode(C_EXPR, 2); }
          | unop_expression expression_end   { genNode(C_EXPR, 2);  }
          ;

expression_end: empty
              | relational_operator simple_expression { genNode(C_REL_EXPR, 2); }
              ;

relational_operator: LESS_OR_EQUAL     { genOpSymbol(C_LESS_EQUAL);    }
                   | LESS              { genOpSymbol(C_LESS);          }
                   | EQUAL             { genOpSymbol(C_EQUAL);         }
                   | DIFFERENT         { genOpSymbol(C_DIFFERENT);     }
                   | GREATER_OR_EQUAL  { genOpSymbol(C_GREATER_EQUAL); }
                   | GREATER           { genOpSymbol(C_GREATER);       }
                   ;

identifier_list: identifier_list COMMA identifier { insertTopList(); }
               | identifier
               ;

identifier: IDENTIFIER { genIdent(copy_str(yytext)); }
          ;

goto: GOTO identifier SEMI_COLON { genNode(C_GOTO, 1); }
    ;

return: RETURN SEMI_COLON { genEmpty(); genNode(C_RETURN, 1); }
      | RETURN expression SEMI_COLON { genNode(C_RETURN, 1); }
      ;

compound: OPEN_BRACE CLOSE_BRACE { genEmpty(); }
        | OPEN_BRACE compound_statement CLOSE_BRACE
        ;

compound_statement: compound_statement unlabeled_statement { insertTopList(); }
                  | unlabeled_statement
                  ;

conditional: IF OPEN_PAREN expression CLOSE_PAREN compound { genNode(C_IF, 2); }
           | IF OPEN_PAREN expression CLOSE_PAREN compound ELSE compound { genNode(C_IF, 3); }
           ;

repetitive: WHILE OPEN_PAREN expression CLOSE_PAREN compound { genNode(C_WHILE, 2); }
          ;

simple_expression: term
                 | simple_expression additive_operator term { genNode(C_BIN_EXPR, 3); }
                 ;

unop_expression: unary_operator term { genNode(C_UN_EXPR, 2); }
               | unop_expression additive_operator term { genNode(C_UN_EXPR, 2); insertTopList(); }
               ;

additive_operator: PLUS  { genOpSymbol(C_SUM);  }
                 | MINUS { genOpSymbol(C_MINUS); }
                 | OR    { genOpSymbol(C_OR);   }
                 ;

unary_operator: PLUS  { genOpSymbol(C_SUM);  }
              | MINUS { genOpSymbol(C_MINUS); }
              | NOT   { genOpSymbol(C_NOT);  }
              ;

factor: variable
      | integer
      | function_call
      | OPEN_PAREN expression CLOSE_PAREN
      ;

term: factor
    | term multiplicative_operator factor { genNode(C_BIN_EXPR, 3); }
    ;

multiplicative_operator: MULTIPLY { genOpSymbol(C_MUL); }
                       | DIV      { genOpSymbol(C_DIV); }
                       | AND      { genOpSymbol(C_AND); }
                       ;

function_call_statement: function_call SEMI_COLON
                       ;

function_call: identifier OPEN_PAREN expression_opt CLOSE_PAREN { genNode(C_FUNCTION_CALL, 2); }
             ;

integer: INTEGER { genInt(copy_str(yytext)); }
       ;

empty: { genEmpty(); }
%%
