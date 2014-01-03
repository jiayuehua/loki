/*
 * Test software
 */

#include <cmath>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include "testcases.h"
#include "logicfunction.h"

const char *or2_table [] =
{
"txt",
"xtt",
"fff",
0
};

const char *or3_table [] =
{
"txxt",
"xtxt",
"xxtt",
"ffff",
0
};

const char *and2_table [] =
{
"ttt",
"fxf",
"xff",
0
};

const char *and3_table [] =
{
"tttt",
"fxxf",
"xfxf",
"xxff",
0
};

const char *xor2_table  [] =
{
"tft",
"ftt",
"fff",
"ttf",
0
};

const char *xor3_table  [] =
{
"fftt",
"ftft",
"tfft",
"tttt",
"ffff",
"fttf",
"ttff",
"tftf",
0
};

const char *impl_table [] =
{
"xtt",
"fxt",
"tff",
0
};

const char *not_table [] =
{
"tf",
"ft",
0
};

const char *incl_table [] =
{
"txxt",
0
};

void processor_test(LogicProcessor *proc, int n, char *inp)
{
	int i;
	for (i=0;i<n;i++)
	{
		inp[i] = 'f';
	}

	bool done=false;
	do {
		for (i=0 ; i<n; i++)
			printf("%c ", inp[i]);
		printf(" -> %c\n", proc->process());

		for (i=0 ; i<n; i++)
		{
			if (inp[i] == 'f')
			{
				inp[i] = 't';
				break;
			}
			else
			{
				inp[i] = 'f';
			}
		}
		done = i==n;
	} while (!done);

}


void function_test ( LogicFunction *func )
{
	char *inp;
	char n=func->get_numinputs();
	LogicProcessor proc(func);

	printf("Testing function: %s\n", func->get_name());
	inp = new char [n];
	for (int i=0; i<n; i++)
	{
		proc.setInput(i, inp+i);
	}

	processor_test(&proc, n, inp);

	delete [] inp;
}


void SymmetryFunction_test( std::ostream& ofs)
{
	ofs<<"============= SymmetryFunction test ================"<<std::endl;
	for (int i1 = 0; i1< sizeof(testcases)/sizeof(const char*); ++i1)
	{	
		ofs<<"********** testcase "<<i1<<" *****begin*******"<<std::endl;
		int len = std::strlen(testcases[i1]);
		int l = std::sqrt((double)len);
		if (l*l != len)
		{
			continue;;
		}
		for (int i = 0; i<l; ++i)
		{
			for (int j = 0; j<l; ++j)
			{
				ofs<<*(testcases[i1]+i*l+j);
			}
			ofs<<std::endl;
		}
		SymmetryFunction sf("symmetryfunction", len);
		LogicProcessor proc(&sf);
		for (int i=0; i<sf.get_numinputs(); i++)
		{
			proc.setInput(i, (char*) testcases[i1]+i);
		}
		int r = proc.process();
		
		if (r&Horizontal_Symmetry)
		{
			ofs<<"H";
		}
		else
		{
			ofs<<"-";
		}
		if (r&Vertiacal_Symmetry)
		{
			ofs<<"V";
		}
		else
		{
			ofs<<"-";
		}
		if (r&Rotational_Symmetry)
		{
			ofs<<"R";
		}
		else
		{
			ofs<<"-";
		}
		ofs<<std::endl;
		ofs<<"^^^^^^^^^^ testcase "<<i1<<" ^^^^^^end.^^^^"<<std::endl;
	}
	
}
int main()
{
	LogicFunction
		f_not("not",1,not_table),
		f_and2("and2",2,and2_table),
		f_and3("and3",3,and3_table),
		f_or2("or2",2,or2_table),
		f_or3("or3",3,or3_table),
		f_xor2("xor2",2,xor2_table),
		f_xor3("xor3",3,xor3_table),
		f_implies("implies", 2, impl_table);

	LogicFunction
		f_incomplete("incomplete",3, incl_table);

// Basic table tests
	function_test(&f_not);
	function_test(&f_and2);
	function_test(&f_and3);
	function_test(&f_or2);
	function_test(&f_or3);
	function_test(&f_xor2);
	function_test(&f_xor3);
	function_test(&f_implies);

	function_test(&f_incomplete);


// Combinatorial tests

	{
		printf("Testing combinatorial not (P and Q)\n");
		char inputs[2];
		LogicProcessor p_not(&f_not),  p_and(&f_and2);
		p_and.setInput(0,inputs);
		p_and.setInput(1,inputs + 1);
		p_not.setInput(0,&p_and);

		processor_test(&p_not, 2, inputs);
	}

	{
		printf("Testing combinatorial P and not (Q or not R)\n");
		//  A && !(B || !C)
		char inputs[3];
		LogicProcessor p_not0(&f_not), p_not1(&f_not), p_or(&f_or2), p_and(&f_and2);
		p_not0.setInput(0,inputs+2);
		p_or.setInput(0,inputs+1);
		p_or.setInput(1,&p_not0);
		p_not1.setInput(0,&p_or);
		p_and.setInput(0,inputs);
		p_and.setInput(1, &p_not1);

		processor_test(&p_and, 3, inputs);
	}
	{
		SymmetryFunction_test(std::cout);
	}
}
