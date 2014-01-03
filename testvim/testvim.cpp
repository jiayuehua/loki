/* Logical function */

#include <stdio.h>
#include <>
#include <>
#include <math.h>
#include <algorithm>
#ifdef 1
#include "logicfunction.h"



	struct LogicFunctionElm {
	};

	static struct LogicFunctionElm *head;

public:
	static void insert(FunctionInterface *f);
	static void remove(FunctionInterface *f);

};
#endif


		free((void*)m_name);
		LogicFunctionList::remove(this);
	}
FunctionInterface::FunctionInterface(const char* name, int numinputs):
		m_name ( strdup(name)), m_numinputs(numinputs)
{ }
LogicFunctionList::LogicFunctionElm *LogicFunctionList::head = 0;

void LogicFunctionList::insert(FunctionInterface *f)
{
	struct LogicFunctionElm *oldhead=head;
	head = new struct LogicFunctionElm;
	head->m_function = f;
	head->m_next = oldhead;
}


void LogicFunctionList::remove(FunctionInterface *f)
{
	for ()
	{
		//origin source code don't update head and don't iterator *elm  correctly.
		if ( (*elm)->m_function == f)
		{
		}
		else
		{
			elm=&((*elm)->m_next);
		}
	}
}

FunctionInterface *LogicFunctionList::find(const char *name)
{
	for (LogicFunctionElm *elm=head; elm; elm=elm->m_next)
	{
		if (0 == strcmp(name, elm->m_function->get_name()) )
		{
			return elm->m_function;
		}
	}
	return 0;
}
void SymmetryFunction::rotate(char *p, int len ,char * p1)const
{
	for (int i = 0; i<len; ++i)
	{
		for (int j = 0; j<len; ++j)
		{
			*(p1 + j*len +   (len-1 -i) ) = *(p + i*len+j) ;
		}
	}
}
char SymmetryFunction::calculate(char *inputs)const
{
	int len = get_numinputs();
	int l = sqrt((double)len);
	if (l*l != len)
	{
		return 0;
	}
	int h = 0x1, v = 0x1<<1, r = 0x1<<2;
	for ( int i =0; h&&(i<l); ++i )
	{
		for (int j = 0;h&& (j<l/2); ++j)
		{
			if ( *( inputs+ i*l+j) != *( inputs + i*l + l-1 -j))
			{
				h = 0;
			}
		}
	}
	
	for (int j = 0;v&& (j<l); ++j)
	{
		for ( int i =0; v&&(i<l/2); ++i )
		{
			if ( *( inputs+ i*l+j) != *( inputs + (l-1-i)*l + j))
			{
				v = 0;
			}
		}
	}
	char * p1 = new char[len+1];
	char * p2 = new char[len+1];
	std::copy(inputs, inputs+len+1, p1);
	for (int k = 0; (k<3) &&r; ++k)
	{
		rotate(p1, l, p2 );
		if (strcmp(p1,p2) != 0)
		{
			r = 0;
		}
		std::swap(p1,p2);
	}
	delete []p1;
	delete []p2;
	return h|v|r;
}
SymmetryFunction::SymmetryFunction(const char *name1, int numinputs) :
FunctionInterface(name1, numinputs)
{
	if (FunctionInterface *lf = LogicFunctionList::find(name1))
	{
		fprintf(stderr, "Warning: Duplicate definition of LogicFunction \"%s\"\n", name1);
		delete lf;
	}
	LogicFunctionList::insert(this);
}

void rotate(char *p, int len ,char * p1)
{
	for (int i = 0; i<len; ++i)
	{
		for (int j = 0; j<len; ++j)
		{
			*(p1 + j*len +   (len-1 -i) ) = *(p + i*len+j) ;
		}
	}
}

LogicFunction::LogicFunction(const char *name1, int numinputs, const char **table) :
	FunctionInterface(name1, numinputs), m_table(table)
{
	if (FunctionInterface *lf = LogicFunctionList::find(name1))
	{
		fprintf(stderr, "Warning: Duplicate definition of LogicFunction \"%s\"\n", name1);
		delete lf;
	}
	LogicFunctionList::insert(this);
}

char LogicFunction::calculate(char *inputs)const
{
	for (const char **t=m_table; *t ; t++)
	{
		int i;
		for (i=0; (*t)[i] == 'x' || inputs[i] == (*t)[i] ; )
		{
			if (++i == get_numinputs() )
				return (*t)[i];
		}
	}
	return 'x';
}


LogicProcessor::LogicProcessor( FunctionInterface *function )
	: m_logicfunctioninterface ( function )
{
	m_inputsources = new char * [ function->get_numinputs() ];
	m_inputfunctions = new LogicProcessor * [ function->get_numinputs() ];
	for (int i=0; i<function->get_numinputs(); i++)
	{
		m_inputsources[i] = 0;
		m_inputfunctions[i] = 0;
	}
}


LogicProcessor::~LogicProcessor()
{
	delete [] m_inputsources;
	delete [] m_inputfunctions;
}

void LogicProcessor::setInput(int input, LogicProcessor *lf)
{
	m_inputfunctions[input] = lf;
}

void LogicProcessor::setInput(int input, char * source)
{
	m_inputsources[input] = source;
}

char LogicProcessor::process()
{
	char *inputs = new char [ m_logicfunctioninterface->get_numinputs() ];

	for (int i=0;i<m_logicfunctioninterface->get_numinputs();i++)
	{
		inputs[i] =  m_inputsources[i] ? *m_inputsources[i] :
			m_inputfunctions[i] ? m_inputfunctions[i]->process() : 'x';
	}
	char output=m_logicfunctioninterface->calculate(inputs);
	delete [] inputs;
	return output;
}
