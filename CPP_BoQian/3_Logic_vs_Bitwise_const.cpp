#include <iostream>
#include <vector>
using namespace std;

class BigArray
{
private:
	vector<int> v;		// a huge vector of int
	int accessCounter;	// to keep track of how many times array is accessed

public:
	// From programming model point of vies this function should be a const function, as it only fetches the value at an index.
	// The vector v is the primary datastructure that we are concerned with and this function does not change any value in v.
	// This is our logic constant-ness of what this function would mean.

	int getItem(int index) const		
	{
		accessCounter++;	// Compile error :- However the compiler disagrees as it maintains a concept of bitwise constant-ness.
							// The compiler just sees that we are changing a member variable, and it disallows.
		return v[index];
	}

	/* 
		This conflict in our logic constness and compiler's bitwise constness can be solved by following :-
		1) By making the member as a 'mutable' member
		   Mutable members can be changed in a const function.
				eg. mutable int accessCounter;

		2) By const_cast to take away the constness (This is a hacky way of coding)
				int getItem(int index) const		
				{
					const_cast<BigArray*>(this)->accessCounter++;
					return v[index];
				}
	*/ 

private:
	int *v2;	// another int array

public:

	// Note :-  This is an example of another conflict between logical and bitwise constness.
	//			Although this function changes the value inside the array referred by v2, and by programming logic it is not a const function,
	//			but it maintains the bitwise constant-ness of this class, as it does not modify any of the members directly.
	//			Therefore the compiler is happy to take it as a const function, even though logically it is not.

	void setV2Item(int index, int x) const
	{
		*(v2+index) = x;
	}

	// This conflict can be easily resolved by removing the const, and let the function be non-const, as it is logically.

	// In summary :-
	//		If you strive for logical constness, then use mutable members to implement the logical constness.
};

int main()
{
	BigArray b;

	return 0;
}