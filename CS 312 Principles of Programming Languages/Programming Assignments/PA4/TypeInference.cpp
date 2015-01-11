#include "ConstantType.h"
#include "VariableType.h"
#include "FunctionType.h"
#include "ast/expression.h"
#include "TypeInference.h"
#include "Type.h"


/*
Sets several constant type variables, a symbol table and two vectors to be used later
*/
Type* t1 = ConstantType::make("Int");
Type* t2 = ConstantType::make("String");
Type* t3 = ConstantType::make("Int");
Type* var1 = VariableType::make("x");
Type* fail = ConstantType::make("Failure");
Type* nil = ConstantType::make("Nil");
Type* listInt = ConstantType::make("ListInt");
Type* listString = ConstantType::make("ListString");
SymbolTable sym = SymbolTable();
vector<AstIdentifier*> LambdaID;
vector<Expression*> LambdaArgs;

TypeInference::TypeInference(Expression * e)
{
	this->program = e;

	cout << t1->to_string() << " " << t1 << endl;
	cout << t2->to_string() << " " << t2 << endl;
	cout << t3->to_string() << " " << t3 << endl;
	vector<Type*> v1;
	v1.push_back(var1);
	v1.push_back(t2);

	vector<Type*> v2;
	v2.push_back(t3);
	v2.push_back(t2);

	Type* t4 = FunctionType::make("fun", v1);
	Type* t5 = FunctionType::make("fun", v2);
	cout << t4->to_string() << " " << t4 << endl;
	cout << t5->to_string() << " " << t5 << endl;
	
	//prints all types & reps in table
	Type::print_all_types();

	{
		Type* t1 = t4;
		Type* t2 = t5;

		cout << "Type 1:" << t1->to_string() << "Rep: " << t1->find()->to_string() << endl;
		cout <<	"Type 2:" << t2->to_string() << "Rep: " << t2->find()->to_string() << endl;

		cout << "unify: " << t1->unify(t2) << endl;

		cout << "Type 1:" << t1->to_string() << "Rep: " << t1->find()->to_string() << endl;
		cout <<	"Type 2:" << t2->to_string() << "Rep: " << t2->find()->to_string() << endl;
	}
		cout << "Type 1:" << t1->to_string() << "Rep: " << t1->find()->to_string() << endl;
		cout <<	"Type 2:" << var1->to_string() << "Rep: " << var1->find()->to_string() << endl;

	cout << " *****************NOW TYPING PROGRAM*****************" << endl;

	//Runs type inference on the program
	eval(program);

}

Type* TypeInference::eval(Expression* e)
{
	/*
	Returns INT if expression is just an int
	*/
	if (e->get_type() == AST_INT) {	
		return t1;
	}

	/*
	Returns STRING if expression is just a string
	*/
	else if (e->get_type() == AST_STRING) {
		return t2;
	}
	
	/*
	Returns NIL if expression is just Nil
	*/
	else if (e->get_type() == AST_NIL) {
		return nil;
	}

	/*
	Performs type inference on binops
	*/
	else if (e->get_type() == AST_BINOP) {
		AstBinOp* b = static_cast<AstBinOp*> (e);
		Expression* e1 = b->get_first();
		Expression* e2 = b->get_second();
		Type* type1 = eval(e1);
		Type* type2 = eval(e2);
		//CONS only works on objects of the same type
		if (b->get_binop_type() == CONS) {
			if (type1 == type2) {
				if (type1 == t1) {
					cout << "Great success. Your type is: listInt" << endl;
					return listInt;
				}
				else if (type1 == t2) {
					cout << "Great success. Your type is: listString" << endl;
					return listString;
				}
				else if (type1 == listInt || type2 == listInt) {
					cout << "Great success. Your type is: listInt" << endl;
					return listInt;
				}
				else if (type1 == listString || type2 == listString) {
					cout << "Great success. Your type is: listString" << endl;
					return listString;
				}
				else {
					cout << "Typing failure: invalid concatenation" << endl;
					return fail;
				}
			}
			else if ((type1 == t1 || type1 == listInt) && (type2 == t1 || type2 == listInt)) {
				cout << "Great success. Your type is: listInt" << endl;
				return listInt;
			}
			else if ((type1 == t2 || type1 == listString) &&
			    (type2 == t2 || type2 == listString)) {
				cout << "Great success. Your type is: listString" << endl;
				return listString;
			}
			else {
				cout << "Typing failure: can only concatenate objects of the same type"
					 << endl;
				return fail;
			}
		}
		//GT, LT, GEQ, LEQ, +, -, * and / only work on integers
		else if (b->get_binop_type() == GT || b->get_binop_type() == LT ||
		  b->get_binop_type() == LEQ || b->get_binop_type() == GEQ ||
		  b->get_binop_type() == MINUS || b->get_binop_type() == TIMES  ||
		  b->get_binop_type() == DIVIDE || b->get_binop_type() == AND ||
		  b->get_binop_type() == OR) {
			if (type1 == t2 && type2 == t2) {
				cout << "Typing failure: that binary operation is only operable on integers" 
					<< endl;
				return fail;
			}
		}
		//As long as type1 = type2, EQ and NEQ are fine
		else if (b->get_binop_type() == NEQ || b->get_binop_type() == EQ) {
			if (type1 == type2) {
				cout << "Great success. Your type is: Int" << endl;
				return t1;
			}
		}
		//Takes care of any potentially missed legal input		
		else if (type1 == type2 && type1 != nil && type1 != nil) {
			cout << "Great success. Your type is: " << type1->to_string() << endl;
			return type1;
		}
		//Failure script
		else {
			cout << "Typing failure: binop can only be applied to objects of the same type"
				 << endl;
			return fail;
		}
	}

	/*
	Performs type inference on Unary Operations
	*/
	else if (e->get_type() == AST_UNOP) {
		AstUnOp* u = static_cast <AstUnOp*> (e);
		Expression* e1 = u->get_expression();
		Type* type1 = eval(e1);
		//Returns an int for isNil
		if (u->get_unop_type() == ISNIL) {
			cout << "Great success. Your type is int." << endl;
			return t1;
		}
		//Returns int for Print
		else if (u->get_unop_type() == PRINT) {
			cout << "Great success. Your type is int." << endl;
			return t1;
		}
		//Returns int or string for lists of int and string
		else if ((u->get_unop_type() == HD || u->get_unop_type() == TL) && 
		   (type1 != listInt && type1 != listString)) {
			if (type1 == t1) {
				cout << "Great success. Your type is int" << endl;
				return t1;
			}
			else if (type1 == t2) {
				cout << "Great success. Your type is string" << endl;;
				return t2;
			}
			else if (type1 == nil) {
				cout << "Great success. Your type is Nil" << endl;
				return nil;
			}
			else {
				cout << "Typing failure: head or tail" << endl;
				return fail;
			}
		}
		//Returns int or string for HD
		else if (u->get_unop_type() == HD) {
			if (type1 == listInt) {
				cout << "Great success. Your type is Int" << endl;
				return t1;
			}
			else if (type1 == listString) {
				cout << "Great success. Your type is String" << endl;
				return t2;
			}
			else {
				cout << "Type failure: head" << endl;
				return fail;
			}
		}
		//Returns int, string or listint or liststring
		else if (u->get_unop_type() == TL) {
			AstList* elist = static_cast<AstList*>(e1);
			Expression* tl = elist->get_tl();
			if (tl->get_type() == AST_LIST) {
				cout << "Great success. Your type is List." << endl;
				return type1;
			}
			else if (tl->get_type() == AST_INT) {
				cout << "Great success. Your type is Int" << endl;
				return eval(tl);
			}
			else if (tl->get_type() == AST_STRING) {
				cout << "Great success. Your type is string" << endl;
				return eval(tl);
			}
			else {
				cout << "Typing failure: tail" << endl;
				return fail;
			}
		}
	}
	/*
	Returns int or string, depending on readint or readstring
	*/
	else if (e->get_type() == AST_READ) {
		AstRead* r = static_cast<AstRead*>(e);
		string input;
		getline(cin, input);
		if(r->read_integer()) {
			cout << "Great success. Your type is int" << endl;
			return t1;
		}
		cout << "Great success. Your type is string" << endl;
		return t2;
	}
	/*
	Performs type inference on branch statements
	*/
	else if (e->get_type() == AST_BRANCH) {
		AstBranch* br = static_cast<AstBranch*>(e);
		Expression* pred = br->get_pred();
		Expression* e1 = br->get_then_exp();
		Expression* e2 = br->get_else_exp();
		Type* type1 = eval(e1);
		Type* type2 = eval(e2);
		if (eval(pred) == t1 && type1 == type2) {
			cout << "Great success. Your type is: " << type1->to_string() << endl;
			return type1;
		}
		else {
			cout << "Typing failure: branch" << endl;
			return fail;
		}
	}

	/*
	Returns type of value of identifier, or a failure statement
	*/
	else if (e->get_type() == AST_IDENTIFIER) {
		AstIdentifier* i = static_cast<AstIdentifier*>(e);
		Expression* val = sym.find(i);		
		if (val != NULL) {
			Type* type1 = eval(val);
			if (type1 != fail) {
				cout << "Great success. Your type is: " << type1->to_string() << endl;
				return type1;
			}
			else {
				cout << "Typing failure: can't map an identifier to a failure" << endl;
				return fail;
			}
		}
		else {
			cout << "Typing failure: identifier not mapped" << endl;
			return fail;;
		}
	}

	/*
	Performs type inference on let statements
	*/
	else if (e->get_type() == AST_LET) {
		AstLet* let = static_cast<AstLet*>(e);
		Expression* id = let->get_id();
		AstIdentifier* id2 = static_cast<AstIdentifier*>(id);
		Expression* val = let->get_val();
		Expression* body = let->get_body();
		sym.push();
		sym.add(id2, val);
		Type* bodytype = eval(body);
		sym.pop();
		if (bodytype != fail) {
			cout << "Great success. Your type is: " << bodytype->to_string() << endl;
			return bodytype;
		}
		cout << "Typing failure: can't map an identifier to a failure" << endl;
		return fail;
	}

	/*
	Performs type inference on lambda statements, works with expression list below
	*/
	else if (e->get_type() == AST_LAMBDA) {
		AstLambda* freeman = static_cast<AstLambda*>(e);
		AstIdentifier* formal = freeman->get_formal();
		LambdaID.push_back(formal);
		Expression* body = freeman->get_body();
		//Evaluate the body and return its type if more arguments exist
		if (LambdaArgs.size() > 0) {
			Expression* arg = LambdaArgs.back();
			sym.push();
			sym.add(formal, arg);
			Type* bodytype = eval(body);
			return bodytype;
		}
		//Create a function type if there are no more arguments
		else {
			vector<Type*> losTypos;
			losTypos.push_back(var1);
			cout << "Great success. Your type is: " 
				<< FunctionType::make("Lambda", losTypos) << endl;
			return FunctionType::make("Lambda", losTypos);
		}
		
	}

	/*
	Performs type inference on expression lists, when combined with lambdas
	*/
	else if (e->get_type() == AST_EXPRESSION_LIST) {
		//Create a vector of arguments, evaluate the argument with a properly mapped identifier
		AstExpressionList* exp = static_cast<AstExpressionList*>(e);
		vector<Expression*> expv = exp->get_expressions();
		if (LambdaID.size() + 1 < expv.size()) {
			LambdaArgs.push_back(expv[LambdaID.size() + 1]);
		}		
		Type* exty = eval(expv[0]);
		if (expv[0]->get_type() != AST_LAMBDA) {
			return exty;
		}
	}

	//Account for erroneous input
	else {
		cout << "Typing failure: unknown" << endl;
		return fail;
	}
}

