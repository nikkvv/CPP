/*
	Static initialization fiasco
		- A subtle problem that can crash your program
*/

/****************************************************** EXAMPLE 1 ******************************************************************/

// Consider we have multiple files as shown below

#include <iostream>
#include <string>
using namespace std;

// ------------------
// File :- Cat.cpp
// ------------------

class Cat
{
private:
	string _name;

public:
	void meow()
	{
		cout<<"Cat rules! My name is "<<_name<<endl;
	}

	Cat(char *name):_name(name)
	{
		cout<<"Constructing Dog "<<_name<<endl;
	}
};


// ------------------
// File :- Dog.cpp
// ------------------

Cat c("Smokey");		// Dog class creates a global Cat object 'c'

class Dog
{
private:
	string _name;
public:
	void bark()
	{
		cout<<"Dog rules! My name is "<<_name<<endl;
	}

	Dog(char *name):_name(name)
	{
		cout<<"Constructing Dog "<<_name<<endl;
		c.meow();				// c.meow() called from Dog's constructor
	}
};

// ------------------
// File :- main.cpp
// ------------------

// Global variable 
Dog d("Gunner");				// Main file creates another global Dog object 'd'

int main()
{
	d.bark();

	return 0;
}

// Note :-
//		Since the global objects 'c' and 'd' are in different source files, the order of their initialization is undefined.
//		In other words, it is not known whether 'd' will be constructed before 'c' or vice versa. This causes a problem.
		
/*
		Case 1 : 'c' is constructed first
				 In this case it is fine, since when 'd' is constructed, 'c' is already created and 'd' can call 'c.meow()' from
				 its constructor.

		Case 2 : 'd' is constructed first
				 In this case, the program will crash, since when 'd' is about to be constructed, it will call 'c.meow()'. But 
				 since 'c' is not constructed yet, it will crash the program.
*/

/* 
	Solution :
			The most commonly used solution to this problem is Singleton design pattern
*/

// ----------------------
// File :- Singleton.cpp
// ----------------------

class Dog;
class Cat;

class Singleton
{
private:
	static Dog *pd;				// singleton class has pointers for Dog and Cat
	static Cat *pc;

public:
	~Singleton()				// Destructor will destroy the Dog and Cat objects only if they are initialized, otherwise nothing happens
	{
		if(pd)
			delete pd;
		if(pc)
			delete pc;
		pd = 0;
		pc = 0;
	}
	
	static Dog* getDog()
	{
		if(!pd)								// Initialize 'pd' only if it is null
			pd = new Dog("Gunner");			// Initialize upon First usage idiom : Initialized only when it it is used.
		return pd;							// If already initialized, return the same object.
	}

	static Cat* getCat()
	{
		if(!pc)
			pc = new Cat("Smokey");
		return pc;
	}
};

Dog* Singleton::pd = 0;
Cat* Singleton::pc = 0;


// Note :-
//		Now with the singleton class, we don't need any global variable of Dog or Cat. Whenever we need a Dog, we call 
//		"Singleton::getDog();" and for cat we call "Singleton::getCat();". There will only be one dog named Gunner and one
//		cat named Smokey available.

int main()
{
	Singleton s;		// Note :- IMPORTANT
						//		   To make sure that the Cat and Dog objects used by Singleton class are deleted,
						//		   we create a singleton instance s. When s goes out-of-scope, its destructor will be called.
						//		   If any Dog or Cat is created, it will be destroyed, otherwise nothing happens.

	Singleton::getCat->meow();
	return 0;
}
