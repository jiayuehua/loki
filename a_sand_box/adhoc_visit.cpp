#include <memory>
#include <iostream>
#include <loki/Sequence.h>
#include <loki_ext/adhocvistor.h>
#include <loki/Typelist.h>
class Animal
{
public:
	virtual void print() const = 0;
};

class Horse:public Animal
{
public:
	virtual void print() const 
	{
		std::cout<<"Horse\n";
	}
	int age()const
	{
		return 11;
	}
};

class Dog:public Animal
{
public:
	virtual void print() const 
	{
		std::cout<<"Dog\n";
	}
	int age1()const
	{
		return 13;
	}
};

class SmallHorse:public Horse
{
public:
	virtual void print() const
	{
		std::cout<<"SmallHorse\n";
	}
	int age2()const
	{
		return 77;
	}
};
class Effector
{
public:
    void Visit(Horse*p)
	{
		if (p)
		{
			p->print();
			std::cout<<p->age();
			std::cout<<"\n\n";
		}
	}
    void Visit(Dog*p)
	{
		if (p)
		{
			p->print();
			std::cout<<p->age1();
			std::cout<<"\n\n";
		}
	}
	 void Visit(SmallHorse*p)
	{
		if (p)
		{
			p->print();
			std::cout<<p->age2();
			std::cout<<"\n\n";
		}
	}
};
int adhoc_main()
{
	std::auto_ptr<Animal> pd(new Dog);
	std::auto_ptr<Animal> ph(new Horse);
	std::auto_ptr<Animal> psh(new SmallHorse);
	Loki::FlexAdHocVisitor<Loki::Seq<SmallHorse, Horse,  Dog>::Type, Effector > v;
	v.StartVisit(ph.get());
	v.StartVisit(psh.get());
	v.StartVisit(pd.get());
	return 0;
}