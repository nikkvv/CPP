#include <iostream>

using namespace std;

// const 
// - A compile time constraint that an object cannot be modified

/*
	Why use const
		a) Guards against inadvertent write to the variable. (stop the wrong behaviour at compile time, rather than wait until run-time)
		b) Self documenting. (extra info for the readers that the variable cannot change)
		c) Enables compiler to do more optimization, making code tighter, therefore faster.
		d) const means the variable can be put in ROM. (useful in embedded programming)
*/

int main()
{
	const int i= 9;
	i = 6;					// Compile Error :- const variable cannot be changed.

	const int *p1 = &i;		// data is const, pointer is not...read as 'const int' so int is const
	*p1 = 5;				// Compile error :- const value cannot be changed.
	p1++;					// OK :- pointer can be changed as it is not constant.


	int x = 20;
	int* const p2 = &x;		// pointer is const, but data is not...read as 'const p2' so p2 is const
	*p2 = 25;				// OK :- data is not const
	p2++;


	const int* const p3 = &x;	// data and pointer are both const
	*p3 += 5;					// Compile error :- data is const.
	p3++;						// Compile error :- pointer is const.

	int const *p4 = &i;			// still data is const, pointer is not
	const int *p4 = &i;			// Same as above

	// Rule to remember-
	// If const is on the left of *, data is const
	// If const is on the right of *, pointer is const

	// const-ness of data can be casted away
	const int j = 10;
	j = 9;						// Compile error :- cannot change const value.
	const_cast<int&>(j) = 6;	// OK :- const_cast can cast away the const-ness of variable. 


	int k;		// k is not a const
	// static_cast can be used to cast a data into a const.
	static_cast<const int&>(k) = 5;		// Compile error :- since we have casted to const. 

	return 0;
}
