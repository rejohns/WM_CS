%{
#include "parser-defs.h"

int yylex();
extern int yy_scan_string(const char* c);
int yyerror(const char* p)
{ 
  if(parser_error_fn != NULL) {
    parser_error_fn("At line " + int_to_string(curr_lineno) + ": " + string(p));
  }
  return 1;
};

 



%}
/* BISON Declarations */
%token 
TOKEN_READSTRING 
TOKEN_READINT 
TOKEN_PRINT 
TOKEN_ISNIL
TOKEN_HD 
TOKEN_TL 
TOKEN_CONS 
TOKEN_NIL 
TOKEN_DOT 
TOKEN_WITH 
TOKEN_LET 
TOKEN_PLUS 
TOKEN_MINUS 
TOKEN_IDENTIFIER 
TOKEN_TIMES 
TOKEN_DIVIDE 
TOKEN_INT 
TOKEN_LPAREN 
TOKEN_RPAREN 
TOKEN_AND 
TOKEN_OR 
TOKEN_EQ 
TOKEN_NEQ 
TOKEN_GT 
TOKEN_GEQ 
TOKEN_LT 
TOKEN_LEQ 
TOKEN_IF 
TOKEN_THEN 
TOKEN_ELSE 
TOKEN_LAMBDA 
TOKEN_FUN 
TOKEN_COMMA 
TOKEN_STRING 
TOKEN_ERROR 
TOKEN_IN



%nonassoc EXPR
%left TOKEN_PLUS



%%







program: expression
{
	res_expr = $$;
}



expression: TOKEN_INT 
{
  	string lexeme = GET_LEXEME($1);
  	long int val = string_to_int(lexeme);
  	Expression* e = AstInt::make(val);
  	$$ = e;
} 
|
TOKEN_STRING 
{
	string lexeme = GET_LEXEME($1);
  	Expression* e = AstString::make(lexeme);
  	$$ = e;
}
|
TOKEN_IDENTIFIER
{
	string lexeme = GET_LEXEME($1);
  	$$ =  AstIdentifier::make(lexeme);
}  
|
expression TOKEN_PLUS expression 
{
	$$ = AstBinOp::make(PLUS, $1, $3);
}
| TOKEN_LPAREN expression_application TOKEN_RPAREN
{
	$$ = $2;
}
|
TOKEN_ERROR 
{
   // do not change the error rule
   string lexeme = GET_LEXEME($1);
   string error = "Lexing error: ";
   if(lexeme != "") error += lexeme;
   yyerror(error.c_str());
   YYERROR;
}




expression_application: expression expression
{
	AstExpressionList *l = AstExpressionList::make($1);
	l = l->append_exp($2);
	$$ = l;
}
|
expression_application expression
{
	Expression* _l = $1;
	assert(_l->get_type() == AST_EXPRESSION_LIST);
	AstExpressionList* l = static_cast<AstExpressionList*>(_l);
	$$ = l->append_exp($2);
}


  
