

#ifndef TYPE_INFERENCE_H_
#define TYPE_INFERENCE_H_

#include "Type.h"
#include "SymbolTable.h"

class Expression;

class TypeInference {
private:
	Expression* program;
public:
	TypeInference(Expression* p);
	Type* eval(Expression* e);

};

#endif /* TYPE_INFERENCE_H_ */
