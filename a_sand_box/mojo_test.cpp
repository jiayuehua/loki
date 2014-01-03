#include <iostream>
#include <loki_ext/mojo.h>
#include <cstring>
#include <iterator>
#include <cstddef>
#include <algorithm>
#include <loki/yasli/yasli_vector.h>
#include <loki/yasli/yasli_fill_iterator.h>
class String: public mojo::enabled<String>
{
	size_t length_;
	char *data_;
	String(const String&);// notice no copy constructor
public:
	~String()
	{
		if (data_)
		{
			delete[] data_;
			data_ = 0;
			length_ = 0;
		}
	}

	String(): length_(1),data_(new char[1]){ *data_ = 0;}

	String(const char* prhs)
		:  length_(std::strlen(prhs)+1),data_(new char[length_])
	{
		std::cout<<"char * Construct!\n";
		std::copy(prhs, prhs + length_, data_);
	}

	String& operator=(const String&rhs)
	{
		String tmp(rhs);
		std::swap(data_, tmp.data_);
		length_ = tmp.length_;
		std::cout<<"String assign \n";
		return *this;
	}
	size_t size()const
	{
		return length_ -1;
	}
	const char* c_str()const
	{
		//return static_cast<const char*>(data_);
		return (data_);
	}
	String(mojo::temporary<String>tmp):data_(0)
	{
		std::cout<<"String temporary Construct!\n";
	    String& rhs = tmp.get();
		std::swap(data_, rhs.data_);
		length_ = rhs.length_;
	}
	String(mojo::constant<String>tmp):length_(tmp->length_ ),  data_(new char[tmp->length_] )  
	{
		std::cout<<"String constant *************** Construct!\n";
		std::copy(tmp->data_, tmp->data_ + length_, data_);
	}
	String(mojo::fnresult<String>tmp):data_(0)
	{
		std::cout<<"String fnresult Construct!\n";
	    String& rhs = tmp.get();
		std::swap(data_, rhs.data_);
		length_ = rhs.length_;
	}
};

mojo::fnresult<String> getString()
{
	const String a("hello,world!");
	String e("hello,world!");
	std::cout<<a.c_str()<<std::endl;
	mojo::constant<String>c (a);
	String b( c );
	//String h( e );//can't compile
	std::cout<<b.c_str()<<std::endl;
	return b;
}
int mojo_test_main()
{
	String s(getString() );
	std::cout<<s.c_str();
	std::cout<<std::endl;
	std::cout<<s.size();
	return 0;
}
