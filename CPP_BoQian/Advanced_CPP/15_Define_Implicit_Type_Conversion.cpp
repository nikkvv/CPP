/*
	User-Defined Implicit Type Conversion

	Categories of Type Conversion :
										|	Implicit  |	Explicit (also called Casting) |
	Standard Type Conversion			|		A	  |		B						   |
	User Defined Type Conversion		|		C	  |		D						   |

	Standard Type Conversion :- Conversion among built-in types eg. int, float, pointers etc.
	User Defined Type Conversion :- Conversion among two user defined types (class) or between classes and built-in types.

*/


/*
	Category A : Implicit Standard Type Conversion
*/
// Examples
char c = 'A';
int i = c;		// Integral promotion

char *pc = 0;	// int -> Null Pointer

void f(int i);
f(c);			// Implicit type conversion used in parameter passing

dog* pd = new yellowdog();	// pointer type conversion :- Note that this is not a user-defined type conversion, because here we 
							// only convert the pointers. So its a pointer type conversion.


/*
	Category C : Implicit User Defind Type Conversion

	It is defined inside its own class (user defined type).

	There are 2 methods to define implicit user defined type conversion :-
	
	Method 1 :- Use constructor that can accept a single paramater. This can convert other types of objects into your class.

	Method 2 :- Use the type conversion function. This can convert an object of your class into other types.

*/

/****************************************************** EXAMPLE 1 ******************************************************************/

// Method 1

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Dog
{
private:
	string m_name;
public:
	Dog(string name) { m_name = name; }		
	// IMPORTANT :- 
	//		A constructor that takes a single parameter is not just a constructor. It is a constructor plus a type convertor.
	//		In this case it converts a string into a Dog. This constructor can implicitly convert a string to a Dog.
	//		If all you want to define is a constructor, and no implicit type conversion, always put keyword "explicit" before
	//		the constructor to avoid any inadvertent type conversion.
	//		eg.
	//			explicit dog(string name) { m_name = name; }		// This tells the compiler that this is a constructor plus an 
	//																// explicit type convertor. (but not implicit)
	//
	//		Without the explicit keyword, it behaves as a constructor + explicit convertor + implicit convertor.

	string getName() { return m_name; }
};

void main()
{
	string dogName = "Bob";
	Dog dog1 = dogName;				// This will implicitly convert the string to a Dog
	cout<<"My name is "<<dog1.getName()<<endl;
	return;
}

// Output :-
//		My name is Bob


/****************************************************** EXAMPLE 2 ******************************************************************/

// Method 2

// Say we don't want to call dog.getName() everytime and just convert the Dog to a string. So we need a type convertor function.

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Dog
{
private:
	string m_name;
public:
	Dog(string name) { m_name = name; }	
	
	//string getName() { return m_name; }		// We don't need the getName() function now.
	
	operator string() const						// Type convertor function
	{
		return m_name;
	}
};

void main()
{
	string dogName = "Bob";
	Dog dog1 = dogName;							// This will implicitly convert the string to a Dog
	cout<<"My name is "<<(string)dog1<<endl;	// This will use the type convertor function to convert Dog to string.

	string dog2 = dog1;							// Again dog1 is converted to a string dog2.
	cout<<"My name is "<<dog2<<endl;

	// Note :- 
	//		The above case "string dog2 = dog1" could have been a bug as somebody may have mistakenly used a string instead of Dog.
	//		And our type conversion function has masked this bug as it automatically converted a Dog to a string.
	//		Refer to the Note that follows to avoid such situations.

	return;
}

// Output :-
//		My name is Bob
//		My name is Bob


// Note :-
// IMPORTANT DESIGN PRINCIPLE :- make interface easy to use correctly and hard to use incorrectly. (Ideally uncompilable)
// 
// General guidelines :-
//		1) Avoid defining seemingly unexpected conversion.	(like Dog to string above)
//		2) Avoid defining two-way implicit conversions.		( A->B defined, then you probably should not define conversion B->A )



/****************************************************** EXAMPLE 3 ******************************************************************/

// Implicit type conversion is useful when creating numerical types of class, such as a rational class.

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Rational
{
public:
	int num;	// Numerator
	int den;	// Denominator

	// Since no keyword "explicit", this constructor is a constructor + explicit type convertor + implicit type convertor
	Rational(int numerator = 0, int denominator = 1):num(numerator), den(denominator) {}	

	const Rational operator*(const Rational& rhs)	// product of two rational numbers
	{
		return Rational(num*rhs.num, den*rhs.den);
	}
};

int main()
{
	Rational r1 = 23;			// Converts an integer to a Rational number using the constructor.
	Rational r2 = r1 * 2;		// Converts 2 to a Rational number and then uses operater*() to find product. 

	Rational r3 = 3 * r1;		// This is a compile error
								// Reason :- since the first parameter is integer ('3'), it will find the global operator * and then
								//			 converts 'r1' into an integer and then multiply. And apparently we don't have a type
								//			 convertor to convert a Rational into an integer so this code does not compile.
								//			 Continue to example 4.

	return 0;
}


/****************************************************** EXAMPLE 4 ******************************************************************/

// Implicit type conversion is useful when creating numerical types of class, such as a rational class.

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Rational
{
public:
	int num;	// Numerator
	int den;	// Denominator

	// Since no keyword "explicit", this constructor is a constructor + explicit type convertor + implicit type convertor
	Rational(int numerator = 0, int denominator = 1):num(numerator), den(denominator) {}	

	const Rational operator*(const Rational& rhs)	// product of two rational numbers
	{
		return Rational(num*rhs.num, den*rhs.den);
	}

	operator int() const		// Now we have defined a type conversion from Rational to an integer.
	{
		return num/den;
	}
};

int main()
{
	Rational r1 = 23;			// Converts an integer to a Rational number using the constructor.

	Rational r2 = r1 * 2;		// Note :- Compile error
								// Now this is a compile error. When compiler looks at the operator *, it finds out that there
								// are more than one way to interpret the *. 
								//	   1) Use operator*() defined in Rational class.
								//	   2) convert 'r1' into an integer and then use the global operator * to multiply with integer '2'
								// So this is an ambiguity.
								// And this confirms to the design guideline that a one-way type conversion is preferred over
								// a two-way conversion.
								// Continue to Example 5 to find how we can make both lines to work.


	Rational r3 = 3 * r1;		// This is OK now.
	return 0;
}


/****************************************************** EXAMPLE 5 ******************************************************************/

// We can use a non-member operator*() to multiply two Rationals together.

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Rational
{
public:
	int num;	// Numerator
	int den;	// Denominator

	// Since no keyword "explicit", this constructor is a constructor + explicit type convertor + implicit type convertor
	Rational(int numerator = 0, int denominator = 1):num(numerator), den(denominator) {}	

	// Note :-
	//		Now we remove the class's own operator*() and define a non-member operator*(), as well as the type conversion function
	
	/*
	const Rational operator*(const Rational& rhs)	// product of two rational numbers
	{
		return Rational(num*rhs.num, den*rhs.den);
	}

	operator int() const		// Now we have defined a type conversion from Rational to an integer.
	{
		return num/den;
	}
	*/
};

const Rational operator*(const Rational& lhs, const Rational& rhs)
{
	return Rational(lhs.num*rhs.num, lhs.den*rhs.den);
}

int main()
{
	Rational r1 = 23;			// Converts an integer to a Rational number using the constructor.

	Rational r2 = r1 * 2;		// This is OK now. Constructor will convert integer to Rational and then multiplication is done by 
								// non-member operator*();

	Rational r3 = 3 * r1;		// This is OK now. Same as above.

	return 0;
}
