
/* Use of virtual destructors in polymorphic base classes */

/****************************************************** EXAMPLE 1 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	~Dog()
	{
		cout<<"Dog destroyed"<<endl;
	}
};

class YellowDog : public Dog
{
public:
	~YellowDog()
	{
		cout<<"Yellow Dog destroyed"<<endl;
	}
};

class DogFactory
{
public:
	static Dog* createYellowDog()
	{
		return (new YellowDog());
	}

	//.... create other Dogs
};

int main()
{
	Dog* pd = DogFactory::createYellowDog();
	//... Do something with pd

	delete pd;	// Only the Dog's destructor is called, which is bad as we had created a YellowDog.
				// We have to ensure that YellowDog's own destructor is also invoked.
				// To do that we use 'virtual' destructor for the base class as in example 2

	return 0;
}

/* 
	Output :-
	Dog destroyed
*/



/****************************************************** EXAMPLE 2 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	virtual ~Dog()				// Virtual base class destructor
	{
		cout<<"Dog destroyed"<<endl;
	}
};

class YellowDog : public Dog
{
public:
	~YellowDog()
	{
		cout<<"Yellow Dog destroyed"<<endl;
	}
};

class DogFactory
{
public:
	static Dog* createYellowDog()
	{
		return (new YellowDog());
	}

	//.... create other Dogs
};

int main()
{
	Dog* pd = DogFactory::createYellowDog();
	//... Do something with pd

	delete pd;	// Since now the Dog's destructor is virtual, the VirtualDog's destructor is also invoked.

	return 0;
}

/* 
	Output :-
	Yellow Dog destroyed
	Dog destroyed
*/

/*
If a class is meant to be used in a polymorphic way, (i.e we use the base class reference/pointer to refer to a derived class),
then we should declare the base class destructor as virtual, to ensure the destructors are invoked appropriately.
*/



/****************************************************** EXAMPLE 3 ******************************************************************/

/* If for some reason we really dont want to use a virtual destructor, then we have a second way to do it using shared pointers */ 
/* Note :- only valid for C++11 */

#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	~Dog()				// Non-virtual destructor
	{
		cout<<"Dog destroyed"<<endl;
	}
};

class YellowDog : public Dog
{
public:
	~YellowDog()
	{
		cout<<"Yellow Dog destroyed"<<endl;
	}
};

class DogFactory
{
public:
	static shared_ptr<Dog> createYellowDog()
	{
		return shared_ptr<YellowDog>(new YellowDog());		// we create a shared pointer of YellowDog and then 
	}														// cast it into shared pointer of Dog

	//.... create other Dogs
};

int main()
{
	shared_ptr<Dog> pd = DogFactory::createYellowDog();
	//... Do something with pd

	//delete pd;	// We don't need to delete a shared pointer, as shared pointer is responsible to delete the object

	return 0;
}

/* 
	Output :-
	Yellow Dog destroyed
	Dog destroyed
*/


/* 
Note :-
	- All the classes in STL have no virtual destructor, so be very careful inheriting from them. So we should use shared_ptr
	  for classes derived from the STL classes.
*/