# Exception Handling
"The question is whether or not we will arrange for our compiler and tools to help us find errors"
- Bugs
- Invariant violations
- Exposure of data representation
- Client violations of preconditions
- External problems: file permissions, out of memory, overflow
## Assertions
A boolean expression to check a program condition: does file exist, is input negative, is array index out of bounds
- if false, dump the code filename and the line number onto command line. Linux has built in core dumping

Use assertions to document and check assumptions 
- (preconditions, postconditions, invariants)
- Null references
- Assert as close to error as possible
- Don't assert in production code if:
  - the downstream programmers don't know how to deal with the assertion error
  - you don't want to stop the entire program
  - bug doesn't have too bad consequences
- Do assert in production code if it is a critical data
- Remove all asserts with `g++ -DNDEBUG ...`
- **Don't change any variables inside the assert!** ex. `assert(i++ == 10)`

## Assert in istream& operator>>
If the denominator was 0 and you threw exception, it would be half modified (maybe the denominator is not changed but the numerator is changed)

Add `assert(r.rat_->denom != 0)` after the `sin >> r.rat_ -> num_ >> slash >> r.rat_ -> denom_;` and include `<assert.h>`

# Exception handling
Ex. Traditional file opening, uses error codes (exit(1)) to communicate info between programs. 
```
infile.open(data);
if(!infile) {cout << "can't open file! << endl;" exit(1);}
while(!infile.eof()){
	string name;
	infile >> name;
	if(infile.fail()) -> file format error
	else if(infile.bad()) -> data loss corruption error
	else {process data}
}
```
Exception = object thrown to represent the occurrence of the error, to pass information to where it can be handled. Components throw exceptions to report a failure, and handle exceptions with try-catch blocks. 

```
try{
	
}catch(type1 &e1) {...handle e1}
catch(type2 &e2) {... handle e1}
```

## Throwing and Creating Exceptions
Throwing a string or an integer (constant) is deprecated; it is not very helpful. Should throw an exception object. Define the exception object in the "public" of the class.

```cpp
class Rational{
	public:
	  class DivideBy0Exception{
	    public:
	      DivideBy0Exception(int n): num(n){} // constructor
	      int num() const {return num;} // accessor
	    private:
	      int num; // the numerator
	  };
};
// Don't need a "throw" keyword in the signature anymore
Rational::Rational(int num, int denom){ 
	if(denom == 0) throw DivideBy0Exception(num);
}
```
## Stack unwinding
Any statement that happens after the throw statement won't happen. The curent routine is popped off the call stack. If local handler not found, it goes up the call stack by popping, until a matching handler is found. 
## Catching Exceptions
Keep exception safe code, like "Rational r" and the cout, outside the try block. Exceptions are caught by the nearest handler that matches the type. 

As the control gets transferred to an exception handler
- Stop function execution on the erroneous statement 
- Pop the current routine off from the call stack for the exception handler
- If local handler not found, continue migrating up the call hierarchy by call stack popping, until a matching handler is found
- Java exception handling deploys the same mechanism

What happens during each call stack popping?
- Objects pertaining to the current routine are deleted automatically through invoking the appropriate destructors
- Partially constructed stack-based objects (e.g., objects from failed constructors) are properly destroyed
- If a destructor throws an exception that’s not handled locally, the program terminates
	- if destructor fails the memory is probably corrupted
- should use "smart pointers" so that exceptions can delete heap objects

```cpp
Rational r;
cout << "enter a/b: "
try{
	cin >> r;
}catch(Rational::DivideBy0Exception &e){
	cout <<e.num() << "/0 divided by 0" << endl;
}
```
## Rethrowing
Sometimes an exception needs to be handled by multiple functions up the call stack. Rethrow the same exception up the callstack with the `throw;`
`catch(SomeEx &e) {cout << "caught "<< e << endl; throw;}`

## Safety Guarantees
Exception safe: if it leaves the program in a valid state after terminating by throwing. All routines should have some kind of exception safety guarantee. 
- Basic Guarantee: legal values of all objects are maintained, no memory leaks
- Strong Guarantee: revert the state of the program to before the call 
- No throw guarantee: will not throw an exception
	- add "noexcept" after the signature
	- destructor, swap, cleanup must be no throw (have to do with memory)
