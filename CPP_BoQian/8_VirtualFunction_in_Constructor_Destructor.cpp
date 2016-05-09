/* Pitfalls in calling Virtual Functions in Constructor or Destructor */


/****************************************************** EXAMPLE 1 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	Dog()
	{
		cout<<"Dog is born."<<endl;
	}

	~Dog()
	{
		cout<<"Dog is destroyed"<<endl;
	}

	void bark() 
	{
		cout<<"I am just a dog"<<endl;
	}
	
	void seeCat()
	{
		bark();
	}
};

class YellowDog : public Dog
{
public:
	YellowDog()
	{
		cout<<"YellowDog is born"<<endl;
	}

	void bark()
	{
		cout<<"I am a yellow dog"<<endl;
	}
};

int main()
{
	YellowDog d;
	d.seeCat();

	return 0;
}

// <IMPORTANT>

/* Output :-
		Dog is born.					// First the Base class constructor is called i.e Dog's constructor.
		YellowDog is born.				// Then the YellowDog's constructor is called.
		I am just a dog.				// seeCat() is called. It calls bark() function of Dog, not YellowDog, even
		Dog is destroyed				// though the object is of type YellowDog.
*/										


/****************************************************** EXAMPLE 2 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	Dog()
	{
		cout<<"Dog is born."<<endl;
	}

	~Dog()
	{
		cout<<"Dog is destroyed"<<endl;
	}

	virtual void bark()							// defined as a Virtual function so that the Derived Class's function is called.
	{
		cout<<"I am just a dog"<<endl;
	}
	
	void seeCat()
	{
		bark();
	}
};

class YellowDog : public Dog
{
public:
	YellowDog()
	{
		cout<<"YellowDog is born"<<endl;
	}

	// Note :-
	// The virtual-ness of a method is automatically inherited, so the bark() method in YellowDog is virtual by default.
	// However it is a good practice to mark the function explicitly as virtual.
	virtual void bark()	
	{
		cout<<"I am a yellow dog"<<endl;
	}
};

int main()
{
	YellowDog d;
	d.seeCat();

	return 0;
}

/* Output :-
		Dog is born.
		YellowDog is born.
		I am a yellow dog.
		Dog is destroyed
*/

// This is the power of dynamic binding, as long as 'd' is a YellowDog, its methods will be called.



/****************************************************** EXAMPLE 3 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	Dog()
	{
		cout<<"Dog is born."<<endl;
		bark();									// bark() is invoked inside the constructor of Dog
	}

	~Dog()
	{
		cout<<"Dog is destroyed"<<endl;
	}

	virtual void bark()
	{
		cout<<"I am just a dog"<<endl;
	}
	
	void seeCat()
	{
		bark();
	}
};

class YellowDog : public Dog
{
public:
	YellowDog()
	{
		cout<<"YellowDog is born"<<endl;
	}

	virtual void bark()	
	{
		cout<<"I am a yellow dog"<<endl;
	}
};

int main()
{
	YellowDog d;
	d.seeCat();

	return 0;
}


/* Output :-
		Dog is born.
		I am just a dog.
		YellowDog is born.
		I am a yellow dog.
		Dog is destroyed
*/

// Note :- 
// Even though the bark() method is virtual and we know that the object being created is a YellowDog, 
// it still executes Dog's bark() from the constructor. 
//
// Reason :- When YellowDog d is created, before calling the YellowDog's constructor, it first calls the Dog's constructor.
//			 So at this point YellowDog is not constructed yet. It is dangerous to call a member function of an object that is not 
//			 constructed yet, unless that function is a static function. So the compiler does the second best thing, which is
//			 calling Dog's own bark() function. So, this bark() function behaves like a non-virtual function when invoked
//			 inside a constructor.
//
// Conclusion :- We should avoid calling virtual functions in a constructor. The constructor should do as little as possible,
//				 just to put the object in valid state.


/****************************************************** EXAMPLE 4 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	Dog()
	{
		cout<<"Dog is born."<<endl;
		bark();									// bark() is invoked inside the constructor of Dog
	}

	~Dog()
	{
		cout<<"Dog is destroyed"<<endl;
		bark();									// bark() is invoked inside the destructor of Dog
	}

	virtual void bark()
	{
		cout<<"I am just a dog"<<endl;
	}
	
	void seeCat()
	{
		bark();
	}
};

class YellowDog : public Dog
{
public:
	YellowDog()
	{
		cout<<"YellowDog is born"<<endl;
	}

	~YellowDog()
	{
		cout<<"YellowDog is destroyed"<<endl;
	}

	virtual void bark()	
	{
		cout<<"I am a yellow dog"<<endl;
	}
};

int main()
{
	YellowDog d;
	d.seeCat();

	return 0;
}


/* Output :-
		Dog is born.
		I am just a dog.
		YellowDog is born.
		I am a yellow dog.
		YellowDog is destroyed.			// First the YellowDog's destructor is invoked.
		Dog is destroyed.				// Then Dog's (Base class's) destructor is invoked.
		I am just a dog.				// Dog's bark() method is called.
*/

// Note :-
// Similar thing happens with destructor. When object 'd' gets destroyed, first the destructor of YellowDog is called and then 
// destructor of Dog is called. So by the time Dog's destructor is called, the YellowDog is already destroyed.

