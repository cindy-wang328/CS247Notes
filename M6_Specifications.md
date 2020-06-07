# Interface Specifications
Useful in larger teams to communicate ideas between programmers. "Not the most technical lecture"

A contract between the modle provider and the client programmer: contains preconditions and postconditions. 
- For correct usage of an existing module (user manual). 
- What to put into design of a future module
- Integration conditions (interface specification), for diff. teams to work on diff. modules. Mandatory for project management

# Modules and Interfaces
Module: like an object of ADT

Interface: abstract public description of some module. How other people can use your module. Supports information hiding. 
- ex. on(), off() of a lightbulb
- BDP: specify the syntactic requirements
- BDP: qualititatively describe the behavior of the code, so the client can understand how to use your module. 

Components of specification
- Preconditions
  - Requires: assumptions about program state (ex. input is a reference to a vector of integers)
  - The requirement in the precondition should not include a condition that throws an exception. You really need another meeting with the client if they want you to throw exception... 
- Postconditions
  - Modifies: Which variables change 
  	- (ex. modifies nothing -> input params are *const*)
  	- ex. The element specified by oldElm in vector is replaced by new Elm
  - Throws: Exception conditions 
  	- ex. throws null pointer exception when input is null, throws DivideByZeroException if denom is 0
  - Ensures: Guaranteed effects on modified objects
  	- ex. the correct element is replaced in the vector, specified by oldElm
  - Returns: return value 
  	- (ex. returns the sum of all vector elements)
  	- ex. returns the position of the replaced element: if old element is not found, return -1

![spec example](https://github.com/cindy-wang328/CS247Notes/blob/master/screenshots/Screen%20Shot%202020-06-07%20at%2012.35.55%20PM.png)
![example2](https://github.com/cindy-wang328/CS247Notes/blob/master/screenshots/Screen%20Shot%202020-06-07%20at%2012.36.23%20PM.png)

Specifying Derivations (Inheritance)
Should specify all the fields, especially in methods like the constructor. Not only the derived ones. 


# Verifying Implementations
Whether the specification and the implementation match each other.

A implementation can be verifiable but not validatable: it can satisfy everything in the specification, but the code doesn't work.
- ex. the spec doesn't say anything about what if the index is not found, so you don't return anything when it's not found; but that doesn't compile
- if you return -1 or return v.size() when no value is found, it still satisfies the spec. 

Specification should be sufficiently restrictive but also sufficiently general:
- Rules out all unacceptable implementations (Specificand set is subset of acceptable solutions)
  - ex. add a "return an invalid array index if not found"
- Does not rule out desirable implementations
  - ex. don't add "return -1 if not found" since returning v.size() also works

## Comparing Specifications
A is stronger than B (A => B) if: 
1. Pre of A is weaker than Pre of B (less restrictive)
  - Requires B => Requires A
2. Post of A is stronger than / equal to (promises more features then) Post of B
  - Requires B => (Ensures A & Returns A) => (Ensures B & Returns B)
3. A modifies the same or more objects 
  - Requires B => (Modifies B subset Modifies A)
4. A throws the same or fewer exceptions
  - Requires B => (Throws A subset Throws B)

Checking Specifications: is still the client's obligation to ensure the preconditions are met before using the module. (Read the documentation) 
- Make the preconditions easy to do; if the condition is hard to check, detect the problem and report error.
- Throw exception if the preconditions are not satisfied. 
