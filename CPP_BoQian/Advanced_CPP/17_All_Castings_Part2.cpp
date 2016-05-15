/*
 * All Castings Considered - Part 2
 */

/****************************************************** EXAMPLE 1 ******************************************************************/

/*
 * Casting could be risky.
 *
 * Example with dynamic_cast:
 */

class dog {
   public:
   virtual ~dog() {}
};

class yellowdog : public dog {
   int age;
   public:
   void bark() { cout<<"woof"<< endl; }
};

int main() {
   dog* pd = new dog();
   ...
   yellowdog* py = dynamic_cast<yellowdog*>(pd);		
   py->bark();
   cout << "py = " << py << endl;
   cout << "pd = " << pd << endl;
   ...
}

/* OUTPUT:
		woof.
		py = 0
		pd = 57873400
*/

/* Note :-
 *		In the above code the yellowdog is derived from dog. But all dogs are not yellowdogs. So when we cast 'pd' (which is a dog)
 *		to a yellowdog, it would fail and as a result 'py' will become 0 (as we see in output).
 *		When we call bark() on 'py', it will try to interpret bark() to be a static function and in this program, since the bark()
 *		function did not access any member of the class, so it can indeed be treated as a static function and it runs successfully.
 *		So it covers up for the bug caused by the dynamic cast.
 */


/****************************************************** EXAMPLE 2 ******************************************************************/

/*
 * Casting could be risky.
 *
 * Example with dynamic_cast:
 */

class dog {
   public:
   virtual ~dog() {}
};

class yellowdog : public dog {
   int age;
   public:
   void bark() { cout<<"woof. I am "<< age << endl; }		// Now bark() uses a data member.
};

int main() {
   dog* pd = new dog();
   ...
   yellowdog* py = dynamic_cast<yellowdog*>(pd);		
   py->bark();
   cout << "py = " << py << endl;
   cout << "pd = " << pd << endl;
   ...
}

/* OUTPUT:
		Program Crashes
*/

/* Note :-
 *		In the above code the yellowdog is derived from dog. But all dogs are not yellowdogs. So when we cast 'pd' (which is a dog)
 *		to a yellowdog, it would fail and as a result 'py' will become 0 (as we see in output).
 *		Now since bark() function accesses a data member of the class, the compiler can no longer treat it as a static
 *		function and in this program since py==0, when it tries to access its data member, the program will crash.
 */



/****************************************************** EXAMPLE 2 ******************************************************************/

/*
 * Static cast example
 */

class dog {
   public:
   virtual ~dog() {}
};

class yellowdog : public dog {
   int age;
   public:
   void bark() { cout<<"woof. I am "<< age << endl; }
};

int main() {
   dog* pd = new dog();
   ...
   yellowdog* py = static_cast<yellowdog*>(pd);			// Now we use static_cast instead of dynamic_cast
   py->bark();
   cout << "py = " << py << endl;
   cout << "pd = " << pd << endl;
   ...
}

/* Note :-
 *		Unlike dynamic cast, static cast will not perform run-time type check. So the static cast of a dog's pointer to a 
 *		yellowdog pointer will always succeed. So 'py' will never be a null pointer. So when we call py->bark(), it will try to 
 *		access a memory which does not belong to 'py' and the result is undefined. Sometime it will crash and sometime it will 
 *		run through and print out a random/junk number for data member age.
 *	
 *		This is a more serious and more elusive. This is why we need to be very careful while using cast, and apparently
 *		static cast is more risky to use than dynamic cast.
 *		Continue to example 3,4 for correct ways to use.
 */


/****************************************************** EXAMPLE 3 ******************************************************************/

/*
 * Correct way to use.
 * Method 1 : Null check before use.
 */

class dog {
   public:
   virtual ~dog() {}
};

class yellowdog : public dog {
   int age;
   public:
   void bark() { cout<<"woof. I am "<< age << endl; }
};

int main() {
   dog* pd = new dog();
   ...
   yellowdog* py = dynamic_cast<yellowdog*>(pd);
   
   if(py)												// Check first that py is not a null pointer
	py->bark();

   cout << "py = " << py << endl;
   cout << "pd = " << pd << endl;
   ...
}


/****************************************************** EXAMPLE 4 ******************************************************************/

/*
 * Correct way to use.
 * Method 2 : Use polymorphism to minimize castings. Use virtual function in base class.
 */

class dog {
   public:
   virtual ~dog() {}
   virtual bark() {}		// bark() is defined as virtual in base class
};

class yellowdog : public dog {
   int age;
   public:
   void bark() { cout<<"woof. I am "<< age << endl; }
};

int main() {
   dog* pd = new dog();
   ...
   // yellowdog* py = dynamic_cast<yellowdog*>(pd);			// Now, cast is not required to call the bark() method
   // if(py)
   // py->bark();
   pd->bark();								// We could now simply call pd->bark(). 

   //cout << "py = " << py << endl;
   cout << "pd = " << pd << endl;
   ...
}

// Method 2 is better as we have replaced 3 lines of code to just 1 line. So we have reduced the cyclomatic complexity of the code.
// And secondly this is a better object oriented design, because we are delegating the resposibility of invoking the right bark()
// function to the object itself (whether it is a dog or a yellowdog). Also we have removed the run-time type check done by 
// dynamic cast, improving the performance.



/****************************************************** EXAMPLE 5 ******************************************************************/

/* 
 * casting could be a handy hack tool 
 */

class dog {
	public:
	std::string m_name;
	dog():m_name("Bob") {}
	void bark() const				// *this is const here
	{ 
	  std::cout << "My name is " << m_name << std::endl;

	  //m_name = "Henry";			// Not allowed by compiler as the function is a const function
	  const_cast<dog*>(this)->m_name = "Henry";		// This is a hack that has changed the data member inside the const function.
	}
};




/*
 * Summary :-
 * The table shows all types of castings. Also grouped as object castings and pointer castings.
 *
 *	Generate_Code : whether the casting will generate a substantial runnable code that will get executed during runtime.
 *	Geenrate_Data : whether the casting will generate a different data object and saved in memory (Pointer is not counted as data, only the pointee).
 *	Risky		  : Risky level, how risky it is to use the cast ( higher number means highly risky)
 *	Data_type	  : What kind of data types can the cast perform on.
 *
 * =========================================  C++ Style Casting ================================
 *                             Generate_Code   Generate_data      Risky   Data_type
 *
 * Object Casting:            
 *    static_cast                 yes              yes             2     any types
 *                         (either constructor                           (as long as type 
 *                        or the type convertor)                         conversion is defined)
 *
 * Pointer/Reference Casting:
 *    static_cast                 no               no              4     related types
 *    dynamic_cast                yes              no              3     related types(down-cast)
 *    const_cast                  no               no              1     same type
 *    reinterpret_cast            no               no              5     any types
 *
 *
 *
 *
 * =========================================  C Style Casting ================================
 *                             Generate_Code  Generate_data      Risky   Data_type
 * Object Casting:               yes              yes             5      any types
 *                                                                       (as long as type 
 *                                                                       conversion is defined)

 * Pointer/Reference Casting:    no               no              5      any types
 *
 *
 *
 * Notes :- <IMPORTANT>
 * 1. const_cast, dynamic_cast and reinterpret_cast can only work on pointers/references.
 * 2. static_cast of objects is very different from static_cast of pointers. 
 * 3. reinterpret_cast basically reassigning the type information of the bit pattern. 
 *    It should only be used in low-level coding.
 * 4. dynamic_cast does static_cast plus run-time type check.
 * 5. dynamic_cast and static_cast for pointers can only work on related type (e.g., 
 *    base <-> derived).
 */

