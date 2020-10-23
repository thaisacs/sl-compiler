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

body: OPEN_BRACE statement_list CLOSE_BRACE { }
    ;

labels: LABELS identifier_list SEMI_COLON
      | empty
      ;

types: TYPES types_definition
     | empty
     ;

types_definition: identifier ASSIGN type SEMI_COLON
                | types_definition identifier ASSIGN type SEMI_COLON
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

function_list: function_list function
             | function
             ;

type: identifier type_array
    ;

type_array:
          | type_array OPEN_BRACKET integer CLOSE_BRACKET
          ;

formal_parameters: OPEN_PAREN formal_parameter_opt CLOSE_PAREN { genEmpty(); }
                 ;

formal_parameter_opt:
                    | formal_parameter_list
                    ;

formal_parameter_list: formal_parameter_opt COMMA formal_parameter
                     | formal_parameter
                     ;

formal_parameter: expression_parameter
                | function_parameter
                ;

function_parameter: function_header
                  ;

expression_parameter: VAR identifier_list COLON identifier
                    | identifier_list COLON identifier
                    ;

statement_list: statement_list statement
              | empty
              ;

statement: unlabeled_statement
         | identifier COLON unlabeled_statement
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

assignment: variable ASSIGN expression SEMI_COLON
          ;

empty_statement: SEMI_COLON
               ;

variable: identifier variable_expression
        ;

variable_expression:
                   | variable_expression OPEN_BRACKET expression CLOSE_BRACKET

expression_opt:
              | expression_list
              ;

expression_list: expression_list COMMA expression
               | expression
               ;

expression: simple_expression expression_end
          | unop_expression expression_end
          ;

expression_end:
              | relational_operator simple_expression
              ;

relational_operator: LESS_OR_EQUAL
                   | LESS
                   | EQUAL
                   | DIFFERENT
                   | GREATER_OR_EQUAL
                   | GREATER
                   ;

identifier_list: identifier_list COMMA identifier { insertTopList(); }
               | identifier
               ;

identifier: IDENTIFIER { genIdent(copy_str(yytext)); }
          ;

goto: GOTO identifier SEMI_COLON
    ;

return: RETURN SEMI_COLON
      | RETURN expression SEMI_COLON
      ;

compound: OPEN_BRACE CLOSE_BRACE
        | OPEN_BRACE compound_statement CLOSE_BRACE
        ;

compound_statement: compound_statement unlabeled_statement
                  | unlabeled_statement
                  ;

conditional: IF OPEN_PAREN expression CLOSE_PAREN compound
           | IF OPEN_PAREN expression CLOSE_PAREN compound ELSE compound
           ;

repetitive: WHILE OPEN_PAREN expression CLOSE_PAREN compound
          ;

simple_expression: term
                 | simple_expression additive_operator term
                 ;

additive_operator: PLUS
                 | MINUS
                 | OR
                 ;

unop_expression: unary_operator term
               | unop_expression additive_operator term
               ;

unary_operator: PLUS
              | MINUS
              | NOT
              ;

factor: variable
      | integer
      | function_call
      | OPEN_PAREN expression CLOSE_PAREN
      ;

term: factor
    | term multiplicative_operator factor
    ;

multiplicative_operator: MULTIPLY
                       | DIV
                       | AND
                       ;

function_call_statement: function_call SEMI_COLON
                       ;

function_call: identifier OPEN_PAREN expression_opt CLOSE_PAREN
             ;

integer: INTEGER
       ;

empty: { genEmpty(); }
%%
