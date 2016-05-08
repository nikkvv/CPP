#include <iostream>
#include <string>

using namespace std;

class Dog
{
private:
	int age;
	string name;
public:
	Dog()
	{
		age = 3;
		name = "dummy";
	}

	/************************************ const parameters *************************************/

	void setAge(int& a)		// Taking parameter by reference.
	{
		age = a;
		a++;				// This will change the value of the original variable 'i', 
							// which may not be as required by the caller.
	}

	// To avoid this, we can pass the argument as const reference.
	// Const reference parameters are widely used in c++.
	void setAge(const int& a)
	{
		age = a;
		a++;				// Compile Error :- const value cannot be changed.
	}

	// Note :-	The function which takes a const reference parameter can be overloaded with a function that takes a non-const reference parameter.
	//			eg. The above two functios :-
	//				1) void setAge(const int& a)
	//				2) void setAge(int& a)
	//			These two are different functions.
	//			Function 1 is invoked when the parameter is non-const (eg. Dog d; int i=12; d.setAge(i); )
	//			Function 2 is invoked when the parameter is const (eg. Dog d; const int j=15; d.setAge(j); d.setAge(20); )

	void setAge(const int a)	// a is not a reference but a const variable.
	{
		age = a;
		a++;				// Compile Error :- still cant change const value.
							// However this is not that useful, because the paramater is passed by value. 
							// So the caller doesn't care, if its const or not.
	}

	// Note :- The caller cannot differentiate between these functions
	//		void setAge(const int a);
	//		void setAge(int a);
	//		void setAge(int &a);
	// From the caller's point, these functions are the same.


	/************************************ const return value *************************************/
	
	const string& getName()		// We return by reference to improve the efficiency of program/
								// And we return a const so that the caller cannot change the variable we returned.
	{
		return name;
	}

	const string getName()		// Here the name is returned by value, so the const is useless,
								// because what is returned is a copy of the name which is a temporary.
	{
		return name;
	}


	/************************************ const function ****************************************/

	// const functions cannot change any of the member variables of this class.
	// Also it can only call another const function to ensure and maintian the const correctness.

	void printDogName() const	// 'const' at the end of function parameter list.
	{
		cout<<name<<endl;
		age++;					// Compile error :- cannot change a member variable
		
		cout<<getName()<<endl;	// Compile error :- cannot call another function which is not a const function.
								//					(even though that function does not change any member variable)
	}

	void printDogName()
	{
		cout<<getName()<<endl;
	}

	// Note :- Function const-ness can be used to overload a function
	//		1) void printDogName() const
	//		2) void printDogName()
	// These two are two different functions. Const function can be overloaded with a non-const function.
	// Function 1 will be invoked when the object is const. (i.e const Dog d1)
	// Function 2 will be incoked when the object is non-const (i.e Dog d2)

};

int main()
{
	Dog d;

	int i=9;
	d.setAge(i);		// Compile error :- ambiguity amongst "void setAge(const int a)" and "void setAge(int &a)"
	cout<<i<<endl;
	
	const string& n = d.getName();
	cout<<n<<endl;

	return 0;
}