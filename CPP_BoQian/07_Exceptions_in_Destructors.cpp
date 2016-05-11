/*
Prevent Exceptions from leaving Destructors.
*/

/*
Exceptions allow your program to react to exceptional circumstances such as a run time error.
They do that by transferring the control to a special portion for the code called exception handler.
Destructors, like any part of the code can throw exceptions.
However, you have to be very careful that the exceptions thrown by destructors are contained inside he destructors, and not thrown outside.

Refer to example 1 to understand why.
*/


/****************************************************** EXAMPLE 1 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

class dog
{
public:
	string name;
	
	dog(string n)
	{
		name = n;
		cout<<name<<" is born."<<endl;
	}

	~dog()
	{
		cout<<name<<" is destroyed"<<endl;
	}

	void bark() {...}		// Some method bark()
	...
};


int main()
{
	try
	{
		dog dog1("Henry");
		dog dog2("Bob");
		throw 20;
		dog1.bark();		// Before calling any method an exception is thrown out.
		dog2.bark();
	}
	catch(int e)
	{
		cout<<e<<" is caught."<<endl;
	}

	return 0;
}


// Output :-
//		Henry is born.
//		Bob is born.						
//		Bob is destroyed.			// Note :- Bob is destroyed first, as they are stored in stack and when stack unwinds, it does in LIFO manner.
//		Henry is destroyed.
//		20 is caught.


// Note :- Before the exception is caught by the catch block, the stack will unwind and all the local variables inside the 
//		   try block will have to be destroyed. Hence both Bob and Henry are destroyed before exception is caught.



/****************************************************** EXAMPLE 2 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

class dog
{
public:
	string name;
	
	dog(string n)
	{
		name = n;
		cout<<name<<" is born."<<endl;
	}

	~dog()
	{
		cout<<name<<" is destroyed"<<endl;
		throw 20;									// Destructor throws an exception
	}

	void bark() {...}
	...
};


int main()
{
	try
	{
		dog dog1("Henry");
		dog dog2("Bob");
		dog1.bark();
		dog2.bark();
	}
	catch(int e)
	{
		cout<<e<<" is caught."<<endl;
	}

	return 0;
}

/*
Note :- 
	This program will crash. The execution ends at the try block, the stack will unwind and try to destroy all the local variables.
	On destroying dog2, the destructor for dog2 will throw an exception. Before the exception is caught by the catch block, dog1 
	also needs to be destroyed. dog1 destructor also executes and also throws an exception. So as a result we have 2 exceptions 
	pending at the same time, and c++ does not allow more than 1 exception pending at one time, so it will just crash.
	This is why we should not throw exceptions out of destructor.
*/


/****************************************************** EXAMPLE 3 ******************************************************************/

/*
 * Solution 1 : Destructor swallows the exception.
 */

#include <iostream>
#include <string>
using namespace std;

class dog
{
public:
	string name;
	
	dog(string n)
	{
		name = n;
		cout<<name<<" is born."<<endl;
	}

	~dog()
	{
		cout<<name<<" is destroyed"<<endl;
		try
		{
			// Enclose all the exception prone code here
		}
		catch(Exception e)
		{
			// Catch specific exceptions
		}
		catch(...)
		{
			// Catch all other exceptions, to prevent leakage of exceptions from destructor.
			// But use this cautiously as it will mask of any kind of exception.
		}
	}

	void bark() {...}		// Some method bark()
	...
};

int main()
{
	try
	{
		dog dog1("Henry");
		dog dog2("Bob");
		dog1.bark();		
		dog2.bark();
	}
	catch(int e)
	{
		cout<<e<<" is caught."<<endl;
	}

	return 0;
}

/*
Note :- The downside of this approach is that the Dog's client will not get the exception, so they will not know what went wrong and 
		wont be able to do appropriate thing to handle that.
		
		To solve that check Example 4.
*/


/****************************************************** EXAMPLE 4 ******************************************************************/

/*
 * Solution 2 : Move the exception-prone code to a different function.
 */

#include <iostream>
#include <string>
using namespace std;

class dog
{
public:
	string name;
	
	dog(string n)
	{
		name = n;
		cout<<name<<" is born."<<endl;
	}

	~dog()
	{
		cout<<name<<" is destroyed"<<endl;
	}

	void prepareToDestroy()			// Function that may throw exception
	{
		...;						// We do bunch of things here before actual destroy
		throw 20;					// Throws exception
	}

	void bark() {...}
	...
};


int main()
{
	try
	{
		dog dog1("Henry");
		dog dog2("Bob");
		dog1.bark();
		dog2.bark();
		dog1.prepareToDestroy();		// Before the objects get destroyed, they need to call the prepareToDestroy() method.
		dog2.prepareToDestroy();
	}
	catch(int e)
	{
		cout<<e<<" is caught."<<endl;
	}

	return 0;
}

/*
Note :-
	This program will not crash as the destructor does not throw any exception and
	the dog's client will get the exception and take appropriate steps to handle the exception.
	The downside is that there is one extra API for the dog's client to call.
*/


/*
Note:-
	The choice of using either solution depends on who is the better person to handle the exception.
	If it is the class itself, then use Solution 1.
	If it is the client, then use Solution 2.
*/