/*
	Classes  with resource handles.
	In this lecture we see the pitfall when a class has another object through pointer.
*/

/****************************************************** EXAMPLE 1 ******************************************************************/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person
{
private:
	string* pName;					// Person owns a string object through pointer. Owning means it is responsible for creating 
									// and deleting of this object.
public:
	Person(string name)
	{
		pName = new string(name);
	}

	~Person()
	{
		delete pName;
	}

	void printName()
	{
		cout<<*pName;
	}
};

int main()
{
	vector<Person> persons;
	persons.push_back(Person("George"));

	persons.back().printName();			// vector.back() will return the reference to the last member in the vector
	cout<<"Goodbye"<<endl;

	return 0;
}


// Note :- The above compile looks fine, and it compiles fine. But when we run the program it crashes.
// Reason :- 
//		It crashes at the point when printName() function is called.
//		
//		persons.push_back(Person("George"));
//		The above line in the program invokes 3 operations :-
//			1) This person "George" is constructed
//			2) A copy of "George" is saved in the vector persons.
//			3) This person "George" is destroyed.
//
//		The person that we created ("George") and the person in the vector are not the same person. They are identical but are
//		different objects. When we create a copy of the person, it creates a shallow copy of it via the default copy
//		constructor. So the pointers of both the objects (i.e pName) are pointing to same string object.
//		When the person "George" is destroyed at the end of this statement, it will delete the string object. As a result of 
//		that, the person in the vector will point to a deleted string object. So when we access the object in the 
//		printName() function, it is already deleted and the program crashes.




// Solution 1: Define copy constructor and copy assignment operator for deep copy.

/****************************************************** EXAMPLE 2 ******************************************************************/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person
{
private:
	string* pName;				
public:
	Person(string name)
	{
		pName = new string(name);
	}

	~Person()
	{
		delete pName;
	}

	Person(const Person& rhs)						// Define copy constructor for deep copy
	{
		pName = new string(*(rhs.pName));
	}

	Person& operator=(const Person& rhs)			// Define copy assignment operator for deep copy
	{
		pName = new string(*(rhs.pName));
		return *this;
	}

	void printName()
	{
		cout<<*pName;
	}
};

int main()
{
	vector<Person> persons;
	persons.push_back(Person("George"));

	persons.back().printName();	
	cout<<"Goodbye"<<endl;

	return 0;
}



// Solution 2: Delete copy constructor and copy assignment operator, by making them private so nobody else can use them.

/****************************************************** EXAMPLE 3 ******************************************************************/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person
{
private:
	string* pName;					

	Person(const Person& rhs);						// Delete copy constructor
	Person& operator=(const Person& rhs);			// Delete copy assignment operator
	
	// Note :- We have not even defined them, so that the class's children and friends will not be able to use them.

public:
	Person(string name)
	{
		pName = new string(name);
	}

	~Person()
	{
		delete pName;
	}

	void printName()
	{
		cout<<*pName;
	}
};

int main()
{
	vector<Person> persons;
	persons.push_back(Person("George"));		// Now this statement will give a compile time error.

	persons.back().printName();	
	cout<<"Goodbye"<<endl;

	return 0;
}


// Solution 2 is mostly safer, as the copy constructor and copy assignment are not very frequently used. One common place where
// we have to define these 2 functions is with STL containers. STL containers require their containees to be copy constructable
// and copy assignable. But we can easily work around this by keeping the pointer to the object in the container, instead of
// the object itself.
//		eg. 
//			vector<Person*> persons;
//			persons.push_back(new Person("George"));
//			persons.back()->printName();
//			Remember to delete all objects constructed.



// Note :-
//		Sometimes we do need to make copy of the objects, how to do that if we don't have these 2 functions.
//		We can define a clone function.

/****************************************************** EXAMPLE 4 ******************************************************************/

class Person
{
private:
	string* pName;					
	Person(const Person& rhs);						// Delete copy constructor
	Person& operator=(const Person& rhs);			// Delete copy assignment operator

public:
	Person(string name)
	{
		pName = new string(name);
	}

	~Person()
	{
		delete pName;
	}

	void printName()
	{
		cout<<*pName;
	}

	Person* clone()									// Clone function to return the clone(identical copy) of current object.
	{
		return (new Person(*p_name));
	}
};

int main()
{
	vector<Person> persons;
	persons.push_back(Person("George"));			// Now this statement will give a compile time error.

	persons.back().printName();	
	cout<<"Goodbye"<<endl;

	return 0;
}

// The problem with copy constructor is that it is making the copying implicitly i.e. the copying can happen implicitly when an
// object is passed as a paramater to another function or being returned from another function. This can be a source of bug if not 
// examined carefully. So defining a clone function helps in these cases by deleting the copy constructor and defining a clone
// function and ensures that object copying only happens explicitly.
