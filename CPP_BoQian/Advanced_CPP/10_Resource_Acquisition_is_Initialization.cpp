/* 
	Resource Acquistion is Initialization (RAII)

	Use objects to manage resources :
		memory, hardware device, network handle, etc.
*/

/****************************************************** EXAMPLE 1 ******************************************************************/

#include <iostream>
#include <string>
using namespace std;

Mutex_t mu = MUTEX_INITIALIZER;

void functionA()
{
	Mutex_lock(&mu);				// Lock Mutex
	.....;							// Do a bunch of stuff
	Mutex_unlock(&mu);				// Unlock Mutex	
}

// The problem with this code is that the Mutex_unlock() may never be executed, because doing a bunch of stuff in between the 
// lock and unlock, may throw some exception and may return from functionA() prematurely. In this case the mutex will be locked
// forever.


/*
 *  Solution : RAII technique
 */

class Lock							
{
private:
	Mutex_t *m_pm;							// Pointer to mutex
public:
	explicit Lock(Mutex_t *pm)				
	{
		Mutex_lock(pm);						// In the cinstructor of the Lock class, the mutex will be locked.
		m_pm = pm;
	}

	~Lock()
	{
		Mutex_unlock(m_pm);					// In the destructor of the Lock, the mutex will be unlocked.
	}
};


void functionA()
{
	Lock myLock(&mu);			// Construct a Lock object. It will also lock the mutex.
	...;						// Do a bunch of stuff
}	

// Note :-
// In the end of functionA(), the stack unwinds and when the Lock object is destroyed, its destructor will be invoked and the
// mutex will be unlocked.

/* Conclusion :-
 *	
 *		The only code that can be gauranteed to be executed after exception is thrown are the destructor of objects
 *		residing on the stack.
 *
 *		Resource management therefore needs to be tied to the lifespan of suitable objects in order to gain automatic
 *		deallocation and reclamation.
 */

// Note :-
//		Since mutexes are used for mutual exclusion, we should ensure that nobody can copy the resource management object (i.e Lock)
//		We can do that by disallowing the copy constructor and copy assignment operator from being used.
//		eg. 
//			Lock L1(&mu);
//			Lock L2(L1);		// We should disallow such copy


/****************************************************** EXAMPLE 2 ******************************************************************/

// Another good example of RAII is shared pointer (tr1:shared_ptr)
// A shared pointer is a reference counting smart pointer. It counts the number of pointers pointing to itself and when that
// number reaches 0, the smart pointer will be destroyed.

#include <iostream>
#include <string>
using namespace std;

void functionA()
{
	std::tr1::shared_ptr<dog> pd(new dog());
	.... ;
}

// Note :-
//		The dog is destructed when pd goes out of scope (no more pointer points to pd)


// ============================================
// INCOMPLETE
// ============================================

