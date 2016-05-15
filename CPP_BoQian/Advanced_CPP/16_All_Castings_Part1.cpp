/*
 * All Castings Considered - Part 1
 *
 * C++ is a strongly typed language, so an object can be converted to one type to another.
 *
 * Generally speaking there are 2 types of Type Conversion:
 *    1. Implicit Type Conversion
 *    2. Explicit Type Conversion (also called Casting)
 */

// There are 4 types of casting operators in C++

/*
 * 1. static_cast 
 *			- Can convert an object from one type to another.
 *			- Can work on any kind of types, as long as the type conversion is defined for those types.
 *			- Can also work on pointers and reference, but can only cast an object from one type 
 *			  to a related type (down/up cast i.e Base class -> Derived class or vice versa)
 */
int i = 9;
float f = static_cast<float>(i);					// convert object from one type to another
dog d1 = static_cast<dog>(string("Bob"));			// Type conversion needs to be defined. (string->Dog conversion must be defined here.)
dog* pd = static_cast<dog*>(new yellowdog());		// convert pointer/reference from one type to a related type (down/up cast)
dog* pd = new yellowdog();							// Note :- this is a static cast even though we are not using the static_cast operator



/*
 * 2. dynamic_cast 
 *		- It can only work on pointers and references, it cannot work on objects.
 *		- It converts from one type to a related type (typically it will be used in down cast i.e Base Class -> Derived Class)
 *		- Unlike static cast, it performs a run-time type check. It will check whether the types are compatible for casting to succeed.
 *		- It requires the 2 types to be polymorphic (i.e they need to have atleast one virtual function)
 */
dog* pd = new yellowdog();							// Note :- this is a static cast.
yellowdog py = dynamic_cast<yellowdog*>(pd); 
// a. It convert pointer/reference from one type to a related type (down cast). Here from 'dog' to 'yellowdog'
// b. run-time type check. It will check if pd is pointing to a yellowdog and not just a dog. If succeed, py=pd; if fail, py=0;
// c. It requires the 2 types to be polymorphic (have virtual function).



/*
 * 3. const_cast
 *		- Used to cast away the constness of object being pointed to.
 *		- Only works on pointers/reference.
 *		- Only works on same types. (only difference being one is const and other is non-const)
 */                                        
const char* str = "Hello, world.";			// const
char* modifiable = const_cast<char*>(str);  // cast away the constness and assign to a modifiable pointer.



/*
 * 4. reinterpret_cast
 *		- Re-interpret the bits of the object pointed to.
 *		- Only works on pointers and references.
 *		- The ultimate cast that can cast one pointer to any other type of pointer. (Not necessarily related types)
 *		- Use extra caution while using reinterpret_cast.
 */
long p = 51110980;						// Value of some address
dog* dd = reinterpret_cast<dog*>(p);	// reinterpret cast is used to re-interpret the memory content in that address into a 'dog'


/*
 * C-Style Casting:  
 *		- C++ also inherited the casting used in C i.e. C-Style casting
 *		- There are two types (both are same) :-
 *					1) c-like cast notation (eg. "int i = (int)a;")
 *					2) functional notation (eg. "int j = int(a);")
 *		- C-style casting is really a mixture of static_cast, const_cast and reinterpret_cast. Only cast it doesn't support
 *		  is dynamic_cast.
 */
short a = 2000;
int i = (int)a;			// c-like cast notation
int j = int(a);			// functional notation



/*
 * Generally C++ style of casts are preferred over the C-style, because:
 * 1. Easier to identify in the code.
 * 2. Less usage error. C++ style provides:
 *    a. Narrowly specified purpose of each cast
 *    b. Run-time type check capability.
 */