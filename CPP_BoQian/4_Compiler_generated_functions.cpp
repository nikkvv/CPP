#include <iostream>
#include <string>
using namespace std;

/*********************************************************************************
 Compiler silently writes 4 functions if they are not explicitly declared :-
	1) Copy constructor
	2) Copy Assignment Operator
	3) Destructor
	4) Default Constructor (only if there is no constructor declared)
*********************************************************************************/

class Dog{};

// The above empty class Dog is equivalent to below class. The compiler has filled in the 4 functions for us :-

class Dog
{
public:
	Dog(const Dog& rhs)
	{
		// Member-by-member Initialization : each member of rhs dog will be used to initialize members of this dog.
	}

	Dog& operator=(const Dog& rhs)
	{
		// Member-by-member copying : copy each member of rhs dog to this dog.
	}

	Dog()
	{
		// 1) Call Base Class's default constructor.
		// 2) Call data members' default constructor.
	}

	~Dog()
	{
		// 1) Call Base Class's destructor
		// 2) Call data members' destructor
	}
};

/*
Note :- 
	1) All the compiler generated functions are public and inline.
	2) They are generated only if they are needed i.e. if they are not used in other functions then they will not be generated.
*/

/*
Note :-
There might be cases when the compiler is unable to generate these functions :-

	Case 1 :- Copy assignment operator does a member-by-member copy, but if a member is a const or a reference, then it cannot be copied.
			  Since a const or a reference can only be initialized and cannot be copied, so the compiler will simply not generate this function.

	Case 2 :- Default Constructor calls the base class's default constructor. But if the base class does not have a default constructor,
			  this function will not be generated.

	Case 3 :- Destructor calls the base class's destructor. If the base class has a private destructor, then again this function cannot be generated.
	..
	..
	so on
*/

/****************************************************** EXAMPLE 1 ******************************************************************/

class Dog2
{
public:
	string m_name;

	// Note :- A default constructor is one that can work without any arguments. So this is our default constructor.
	//		   Default constructor may not necessarily be the one that does not have any input parameters.
	Dog2(string name = "Bob")
	{
		m_name = name;
		cout<<name<<" is born."<<endl;
	}

	~Dog2()
	{
		 cout<<m_name<<" is destroyed."<<endl;
	}
};

int main()
{
	Dog2 d1("Henry");
	Dog2 d2;
	d2 = d1;
}

/* 
For example above, let's see which functions are compiler generated :-
	1) Copy Constructor						- NO  , since we are not using the copy constructor
	2) Copy Assignment Operator				- YES , since we use assignment operation in main() and we have not defined it explicitly in class.
	3) Destructor							- NO  , since it is already defined in our class explicitly
	4) Default Constructor					- NO  , since we have already defined explicitly. (Refer to below note)
*/



/****************************************************** EXAMPLE 2 ******************************************************************/

class Dog2
{
public:
	string& m_name;

	Dog2(string name = "Bob")				// Compile Error :- m_name is a reference and it cannot be copied but only initialized.
	{										// This can be resolved by using an initialization list in constructor.
		m_name = name;						//		Dog2(string name = "Bob"):m_name(name) {...}
		cout<<name<<" is born."<<endl;
	}

	~Dog2()
	{
		 cout<<m_name<<" is destroyed."<<endl;
	}
};

int main()
{
	Dog2 d1("Henry");
	Dog2 d2;
	d2 = d1;				// Compile Error :- Since the class has a reference and the default copy assignment operator does a 
}							// member by member copy, which does not apply to a reference. Hence the compiler will not generate
							// the Copy Assignment Operator.

/* 
For example above, let's see which functions are compiler generated :-
	1) Copy Constructor						- NO  , since we are not using the copy constructor
	2) Copy Assignment Operator				- NO  , since the class has a reference variable which cannot be copied.
	3) Destructor							- NO  , since it is already defined in our class explicitly
	4) Default Constructor					- NO  , since we have already defined explicitly.
*/

/*
Note :- 
This kind of class cannot be used with Standard Library Container, because the Standard Library Containers require the containee
to be copy-assignable and copy-constructable.
*/



/****************************************************** EXAMPLE 3 ******************************************************************/

class collar
{
public:
	collar(string color)
	{
		cout<<"Collar is born.\n";
	}
};

class Dog
{
	collar m_collar;
	// Compile Error :- The compiler will generate the default constructor for Dog, as it is used in main().
	// The compiler generated default constructor will call the member's default constructor.
	// However, the collar class does not have a default constructor either, so compiler will try to generate
	// the default constructor for collar class first. But since the collar class already has a constructor with a string 
	// parameter, the compiler is not able to generate the default constructor for collar class, and hence not able to generate
	// a default constructor for Dog class. Therefore it throws a compile time error.
};

int main()
{
	Dog dog1;
}

/* 
For example above, let's see which functions are compiler generated :-
	1) Copy Constructor						- NO  , since we are not using the copy constructor
	2) Copy Assignment Operator				- NO  , since we are not using the copy constructor
	3) Destructor							- YES , since it is not defined by us
	4) Default Constructor					- NO  , since a member variable does not have a defualt constructor. (Refer to note along with code)
*/


/****************************************************** EXAMPLE 4 ******************************************************************/

class collar
{
public:
	collar()
	{
		cout<<"Collar is born.\n";
	}
};

class Dog
{
	collar m_collar;
	string& m_name;
};

int main()
{
	Dog dog1;				// Compile Error :- no appropriate default constructor available for Dog.
							// Reason :- The default constructor only calls the data members default constructors, but it does not initialize the data members.
							// So the m_name reference can be constructed but not initialized.
							// By C++ standards, all references need to be initialized because it cannot be reassigned later.
							// So the only way to assign a value to a reference is through initialization.
							// Since m_name cannot be initialized by default constructor, so it will not be generated by compiler,
							// and this code will not compile.
}

/* 
For example above, let's see which functions are compiler generated :-
	1) Copy Constructor						- NO  , since we are not using the copy constructor
	2) Copy Assignment Operator				- NO  , since we are not using the copy constructor
	3) Destructor							- YES , since it is not defined by us
	4) Default Constructor					- NO  , since a member variable is a reference. (Refer to note along with code)
*/



/****************************************************** EXAMPLE 5 ******************************************************************/

class Dog2
{
public:
	string& m_name;

	Dog2(string name = "Bob"):m_name(name)
	{
		cout<<name<<" is born."<<endl;
	}

	~Dog2()
	{
		 cout<<m_name<<" is destroyed."<<endl;
	}

	Dog2& operator=(Dog2& rhs)
	{
		m_name = rhs.m_name;
		cout<<"Inside operator=()"<<endl;
		return *this;
	}

	Dog2(const Dog2& rhs):m_name(rhs.m_name)		// Note :- Since m_name is a reference it can only be initialized in the initializer list
	{
		cout<<"Inside copy constructor"<<endl;
	}
};

int main()
{
	int a = 10;
	int& x = a;
	x = 25;

	Dog2 d1("Henry");
	Dog2 d2 = d1;						// Note :- On initialization with an object with '=', the copy constructor is called.
	d2 = d1;							// Otherwise the operator=() function will be called
	return 0;
}

/* 
Output of above program will be :-

Henry is born.
Inside copy constructor
Inside operator=()

*/