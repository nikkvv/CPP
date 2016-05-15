/*
	Virtual constructor - Clone() function
	Another advantage of using clone() function.
*/

/****************************************************** EXAMPLE 1 ******************************************************************/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Dog
{
};

class YellowDog: public Dog
{
};

void foo(Dog* d)
{
	Dog *c = new Dog(*d);			// Dog 'c' is copy constructed from Dog 'd', but 'd' is a YellowDog and 'c' is a Dog.
									// So 'c' is not really an identical copy of 'd', which is not good.
	// ... play with dog c
}

int main()
{
	YellowDog d;
	foo(&d);						// the foo() function actually receives a YellowDog, but when we copy construct Dog 'c',
									// we copy construct a Dog.
}

// Note :- Here Dog 'c' is not an identical copy of the YellowDog 'd', which was not what we intended.
// To solve this we can again use a clone function.

class Dog
{
public:
	virtual Dog* clone()
	{
		return (new Dog(*this));		// A Dog copy constructed from itself.
	}
};

class YellowDog
{
public:

	// Note :- IMPORTANT
	// The clone() function in base class returns a Dog*, however the overridden function in this class returns a YelloWdog*.
	// This is a feature called "Co-Variant Return Type"
	// It allows an overridden virtual function, to have a different return type, as long its return type is derived
	// from the base class's return type. This feature allows using the clone() function as a virtual constructor.
	virtual YellowDog* clone()				
	{
		return (new YellowDog(*this));		// A YellowDog copy constructed from itself.
	}
};

void foo(Dog *d)
{
	// Here instead of calling the copy constructor directly, we will call the clone() function.
	Dog *c = d->clone();		// Now Dog 'c' is indeed a YellowDog, identical to 'd'.
}

// Note :- Clone() function is like a virtual constructor, it makes sure that an appropriate type of object is
//		   constructed no matter what kind of type the original object has been casted into.