/* Logical function */
#ifndef LOGICFUNCTION_H
#define LOGICFUNCTION_H

enum{ Horizontal_Symmetry = 0x1, Vertiacal_Symmetry = 0x1<<1, Rotational_Symmetry = 0x1<<2};
//interface for all type of function used for class LogicProcessor
class FunctionInterface
{
	const char *m_name;
	int m_numinputs;
public:
	FunctionInterface(const char* name, int numinputs);
	virtual char calculate(char *inputs) const= 0;
	//return funciton name
	const char* get_name()const
	{
		return m_name;
	}
	virtual ~FunctionInterface();
	//return  num of input
	int get_numinputs()const
	{
		return m_numinputs;
	}

};
//virtual base class , classes that virtual derived from class B can't be derived.
class B{
protected:
	B(){}
};
//calculate logic of inputs
class LogicFunction: virtual private B, public FunctionInterface{
public:

	// inputs/output are char: 't','f','x'
	// table entries are strings of numinputs inputs + resulting output
	LogicFunction(const char *name, int numinputs, const char **table);

	//find the entry match the inputs and return the logic result 
	char calculate(char *inputs)const;
private:
	const char **m_table;
};

class SymmetryFunction: virtual private B, public FunctionInterface{
public:
	// matrix size is : sqrt(numinputs)*sqrt(numinputs)
	SymmetryFunction(const char *name, int numinputs);
	//return value 
	// *:	0 mean No symmetry
	// *:   "bit or" of	enum{ Horizontal_Symmetry = 0x1, Vertiacal_Symmetry = 0x1<<1, Rotational_Symmetry = 0x1<<2};
	//   
	char calculate(char *inputs)const;
private:
	// rotate the matrix p to p1 
	// p is a char matrix which size is : sqrt(len)*sqrt(len)
	void rotate(char *p, int len ,char * p1)const;
};

class LogicProcessor :virtual private B{
public:
	LogicProcessor( FunctionInterface *function );
	~LogicProcessor();
	//set element "input" value to the result of "lf"
	void setInput(int input, LogicProcessor *lf);
	//set element "input" value to "source" 
	void setInput(int input, char * source);
	//	calculate the result
	char process();
private:

	char **m_inputsources;
	LogicProcessor **m_inputfunctions;
	FunctionInterface *m_logicfunctioninterface;
};

#endif //
