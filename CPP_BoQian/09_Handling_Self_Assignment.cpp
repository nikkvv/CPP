/* 
	Handling self-assignment in operator=()

	Operator overload : it can exploit people's intuition and reduce their learning curve for new libraries.
*/

/*
	We look to avoid pitfalls in '=' operator overload. i.e. the pitfall of self-assignment
	Assignments of the form "dog[i] = dog[j]" can result in self-assignment if 'i' is equal to 'j'
*/

/****************************************************** EXAMPLE 1 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

class collar
{
};

class Dog
{
public:
	collar* pCollar;

	Dog& operator=(const Dog& rhs)						// This is mostly the general approach for assignment operator
	{
		delete pCollar;									// First delete self's members and then copy from rhs object.
		pCollar = new collar(*rhs.pCollar);				// Note :- This will use the default copy constructor
		return *this;
	}

	// Note :- 
	//			Here the problem arises in self-assignment. In case of self-assignment (*this = rhs). So when we call
	//			"delete pCollar;" we are also deleting the pCollar of rhs. Then, when we use copy constructor to construct from
	//			the rhs.pCollar, we are accessing an object that is deleted.
};



// Solution 1 :- 

// One solution is to place a check in operator=().
Dog& operator=(const Dog& rhs)
{
	if(this == &rhs)
		return *this;

	delete pCollar;
	pCollar = new collar(*rhs.pCollar);				// Note :- This will use the default copy constructor
	return *this;
}


// There is still a problem with the above code.
// What if the collar class's copy constructor throws an exception. In that case the Dog has deleted its own pCollar, but it has
// failed to create a new pCollar. So Dog is holding a pointer to an invalid object, which is a problem if it is further used by 
// any client. Even though if it is not used further by client, when the Dog object is going out of scope, the Dog's destructor may
// delete the pCollar again, for which the result is undefined.

// So what we want to do is delete pCollar only if we are able to get a new pCollar. So we create a copy of the pCollar.
Dog& operator=(const Dog& rhs)
{
	if(this == &rhs)
		return *this;

	collar *pOrigCollar = pCollar;
	pCollar = new collar(*rhs.pCollar);	
	delete pOrigCollar;
	return *this;
}

// Now if the new operator throws an exception, the Dog object will still hold a pointer pointing to a valid collar.


// Solution 2 :-
// This solution delegates the assignment operation to the caller class.

class collar
{
};

class Dog
{
public:
	collar* pCollar;

	Dog& operator=(const Dog& rhs)
	{
		*pcollar = *rhs.pCollar;		// member by member copying of collars or call collar's operator=()

		return *this;
	}
};

// Note :-
//			We don't necesseraly need to check if 'this' Dog is same as 'rhs' Dog (i.e. self-assignment). 
//			If they are the same, we are making a copy of itself anyway. This might incur some runtime cost, but not by much.
