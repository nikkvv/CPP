/*
	Struct vs Class
*/

/****************************************************** EXAMPLE 1 ******************************************************************/

#include <iostream>
#include <string>

using namespace std;

struct Person_t
{
	string name;
	unsigned int age;
};

class Person
{
	string name_;
	unsigned int age_;

public:
	unsigned getAge() const				// Getter or Accessor : Almost always a const function
	{
		return age_;
	}

	void setAge(unsigned int a)			// Setter or Mutator 
	{
		age_ = a;
	}
};

// Note :- 
//		Having too many getter and setter functions is a bad design. Avoid making setter/getter if possible.
//		If we have to provide too many getter and setter functions for our private data, it means that my data 
//		needs to be used by somebody else. If that is the case, why the data belong to me, and why it does not 
//		belong to somebody else. So having many setter and getter indicates there is some problem with our design.


// Difference :-
//		1) By default the data members of a struct are public and data members of a class are private. This is the only difference
//		   as far as the language is concerned.
//
//		2) Semantically :- 
//				Struct is used for small passive objects that carry public data and have no or few basic member functions.
//				In other words, struct is used for data containers.
//
//				Class is usually used for bigger and therefore more complicated active objects that carry private data, and it interfaces
//				with the outside world through member functions.
//				In other words, a class is used for more complex data structure.
//				
//				Note :- this is just a convention. You can also declare private data in a structure and public data in class.


int main()
{
	Person_t pt;
	cout<<pt.age;			// Accessible.

	Person p;
	cout<<p.age_;			// Compile error :- trying to access private member. This can be done through some public member function.
	cout<<p.getAge();		// OK

	return 0;
}
