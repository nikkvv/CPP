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

// These compiler generated functions are not always useful and sometimes even dangerous to use.


/****************************************************** EXAMPLE 1 ******************************************************************/

class OpenFile : public File				// Inheriting some other class File
{
public:
	OpenFile(string filename)				// Default constructor disallowed
	{
		cout<<"Open file : "<<filename<<endl;
	}

	OpenFile& g(OpenFile& rhs)				// Some function that may use the copy constructor.
	{										
		OpenFile f2(rhs);					// Link error :- the definition of the copy constructor is missing.
		return f2;							// So the copy constructor cannot be used even via some other class function.
	}

private:
	OpenFile(const OpenFile& rhs);				// Copy constructor disallowed.
	OpenFile& operator=(const OpenFile& f);		// Copy assignment operator disallowed.
	void writeLine(string str);					// Some inherited function disallowed
	~OpenFile();								// Destructor disallowed (However not a good idea, as it may result in memory leaks)
												// Check Example 2 for private destructors.
};

int main()
{
	OpenFile f;		// This will call the compiler generated default constructor, if we don't provide a constructor.
					// In case of files, we might not want to call the default constructor without filename.
					
	// 1) We can disallow the default constructor by just defining a constructor in the class as above. 
	// So this call will fail at compile time.

	OpenFile f(string("File1.txt"));	// This is valid

	// Suppose we have opened the file File1.txt in write mode.
	// And we create another OpenFile.

	OpenFile f2(f);			// This will call the compiler generated copy constructor for f2, and it may happen that 
							// f2 will also write to File1.txt. So we have two Openfiles writing to the same file at the same time,
							// which can be dangerous. So in this case we need to disallow the copy constructor from being used.
	
	// 2) We can disallow a copy constructor by declaring it private and not defining it, as shown above.
	// So using a copy constructor will fail at compile time now. Even if copy constructor is used via some other class function,
	// it will fail, as we have not defined it. So it will give a linking error. As a result nobody can use the copy constructor anymore.

	// 3) We can use the same method to disallow the copy assignment operator also. ( As shown in the class)

	/* 
		Note :- This method can be used to disable any function (default constructor, destructor or any inherited function)
		Say OpenFile has inherited a function writeLine() and we dont want to use that function, we can declare the function private
		with no definition.

		private:
			void writeLine(string str);

	*/

	return 0;
}



/****************************************************** EXAMPLE 2 ******************************************************************/

class OpenFile
{
public:
	OpenFile(string filename)					// Default constructor disallowed
	{
		cout<<"Open file : "<<filename<<endl;
	}

private:
	~OpenFile();					// Destructor disallowed. We have declared the function as private and not defined it.
									// So now nobody can use it. It is not a good idea.
									// An object must be destroyed one way or the another otherwise some resource is bound
									// to be leaked. So we must define the body of the destructor.

	// Therefore :-

	~OpenFile()
	{
		cout<<"OpenFile Destroying."<<endl;
	}
				
	// However it does make sense to have a private destructor. It can ensure that the object cannot be destroyed by anyone else
	// apart from the object itself and its friends. 
	// This kind of mechanism is used by reference counting shared pointers.
};

int main()
{
	OpenFile f(string("File1.txt"));		// Compile error :- As the f is local object, so when the stack unwinds at the end of the function main()
											// it will try to destroy f. But since the destructor is private this cannot happen.
											// Continue to example 3.

	return 0;
}



/****************************************************** EXAMPLE 3 ******************************************************************/

class OpenFile
{
public:
	OpenFile(string filename)					// Default constructor disallowed
	{
		cout<<"Open file : "<<filename<<endl;
	}

	void destroyMe()							// Public interface provided for destroying itself, since the destructor is private.
	{
		delete this;
	}

private:
	~OpenFile()									// Destructor private
	{
		cout<<"OpenFile Destroying."<<endl;
	}
};

int main()
{
	OpenFile f(string("File1.txt"));			// Still compile error.	
	f.destroyMe();								// Reason :- Since the object f is stored on stack and when the function ends
	return 0;									// the stack unwinds and calls destructor of f, which is private and not allowed.
}												// Even though we have called f.destroyMe() and destroyed the object, but once the
												// stack unwinds, the destructor will be invoked anyway.
												// Refer to example 4 to see how to use an object with private destructor.



/****************************************************** EXAMPLE 4 ******************************************************************/

class OpenFile
{
public:
	OpenFile(string filename)					// Default constructor disallowed
	{
		cout<<"Open file : "<<filename<<endl;
	}

	void destroyMe()							// Public interface provided for destroying itself, since the destructor is private.
	{
		delete this;
	}

private:
	~OpenFile()									// Destructor private
	{
		cout<<"OpenFile Destroying."<<endl;
	}
};

int main()
{
	OpenFile* f = new OpenFile(string("File1.txt"));		// To use an object with private destructor, store the object on heap.
	f->destroyMe();
	return 0;
}

// So in summary, a class with a private destructor can only be stored on heap, it cannot be stored on stack.
// This can be useful in embedded programming where stack size is small. We can let the heavyweight classes have private
// destructor and not being stored on stack.


/* Summary of Disallowing Functions :-
	1) Declare the functions to be private, and not define it.
	2) Private Destructor : stay out of stack.
*/