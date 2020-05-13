# Standard features of ADT
## Rational ADT
- Define `Rational` class, public interface, and private data members.
- All data variables should be private. Internal parameters affecting the implementation shouldn't be seen/manipulated by external programs. 
  - We should be providing accessor and mutators to access these fields
  - Some people put a `_` in the name of the private data members in order to differentiate them from other names / create a variable name the conflicts.

```cpp
class Rational{
  public:
    // Constructors (First thing you want to define)
    // Default constructor
    Rational();

    // Accessors and Mutators (Getter/Setter)
  private:
    // Data members (fields/attributes)
    int num;
    int denom;
};

Rational::Rational(){
      num = 0; denom = 1;
} // Defaults to zero
```
It is possible to define implementations either in or outside of the class definition.
We could have put this in the class definition
```cpp
    Rational(){
      num = 0; denom = 1;
    } 
```

This is the most basic version, with a default constructor and 2 data fields.
### Other types of constructors
Creating different "flavours" of a function with the same name but different arguments is *Function Overloading*
```cpp
Rational(int num); // default denom to 1
Rational(int num, int denom); // set both
...
Rational::Rational(int num){
      num_ = num; denom_ = 1;
} 
Rational::Rational(int num, int denom){
      num_ = num; denom_ = denom;
} 
// Rational a = Rational(5, 12); -> 5/12
// Rational a = Rational(5); -> 5/1
```

## Legal Values
All data types must enforce legal values. Denominator can't be zero. We need to enforce this in the constructor and in mutator functions.
- Throw exception if illegal values
- Enforce explicit initialization
- Set default value in the default constructor
  - if we do `Rational a = Rational();` then we can be sure it is initialized to 0/1

### Throw exception if illegal value
```cpp
Rational(int num, int denom) throw (const char*); // in declaration
```
`throw (const char*)` This means the function generates an exception with a string containing exception info
```cpp
Rational:: Rational(int num, int denom) throw (const char*){
  if(denom == 0) {
    throw "Zero denominator";
    return;
  }
  num_ = num;
  denom_ = denom;
}
```
Now, running `Rational a = Rational(5, 0);` gives a "dynamic exception specifications are deprecated", and running it will say "terminate called after throwing an instance of 'char const\*'; Aborted (core dumped)"

#### Try Catch
```cpp
Rational a;
try{
  a = Rational(5, 0);
} catch (const char* e){
  cout << e << endl;
}
```
This will print out "Zero denominator" or whatever the exception message is. 
"a" will implicitly be initialized to 0/1 but we should never rely on implicit initialization.

The main program can try/catch but then `return 1;` after the catch, instead of returning 0
# Public Accessors and Mutators
If we do `Rational a = Rational(); cout << a.num_ << endl; ` you get Illegal access to private field
```cpp
//accessors
int getNum() const; // add these to public section
int getDenom() const;
// mutators
void setNum(const int num); //the input param will not be changed
void setDenom(const int denom) throw (const char*);
...
...
int Rational:: getNum() const{ return num_;}
int Rational:: getDenom() const{return denom_;}

void Rational::setNum(const int num){num_ = num;}
void Rational::setDenom(const int denom) throw (const char*){
  if(denom == 0){throw "Zero denominator"; return;}
  denom_ = denom;
}
```
Now, we can `Rational a = Rational(); cout << a.getNum()<< endl;`
_Accessor_: Member function allowing client to read private values. _Mutator_: Allows clients to update private values
- Use clear `getX()/setX()` naming conventions
- Pass inputs with `const` reference if possible
- All accessors should be `const` if possible
- Mutators should mandate legal values

### Const
By setting accessors as constant, all the involved data members in the function will be read only. This is good protection for accessors because they are not supposed to change anything.
- Accessors should be `int getX() const;` and mutators should be `void setX(const int x);`
- Anything that needs to check legal values needs a `throw (const char*)` at the end of function signature, before the {} brackets.

## Function overloading
Allows clients to call the same function with different variations in argument signatures. 
- Use whenever functions are semantically related. Constructors are almost always overloaded. 
- Must have different argument signatures; can't overload functions with only different return types. 

### Default values in arguments
If you don't supply a value for the parameters, it will take the default value. This one will replace all 3 constructors.
- Not all types of ADT have merge-able constructors. this is a special case
```cpp
Rational(int num = 0, int denom = 1) throw (const char*);
...
Rational::Rational(int num, int denom) throw (const char*){ // Don't give the default args again
  if(denom == 0){throw "Zero denominator"; return;}
  denom_ = denom;
  num_ = num;
}
```
Watch out:
- Default values must be done only in the declaration, and not in the implementation
- When the number of provided values are less than the number of default values, the first one fills in. (If I do Rational(1) then it fills in num, not denom, because num is the 1st parameter)
- Once one default argument is set, all the arguments have to have default values. You can't do `Rational(int num=0, int denom)`

# Operator Overloading
Enables ADT objects to accept arithmetic operators (+, -, \*, /), the in/out stream operators, the equality and comparison operators, etc. We need to implement how the operators should behave on ADT objects. Or else it says "No match for operator+"
For example we want to do `Rational a = Rational(5, 2); Rational b = Rational(3, 7); a = a+b; cout << a << endl;`

_Operator overloading declaration and implementation should be outside the class definition. They are non member functions._

We have to recreate the same signature we're trying to overload, from the official C++ documentation. We can't create some new "===" operator etc
- ex. for logical operators, return a bool and follow the conventional signature
- cannot create new operators or change the number of arguments.

Design decisions:
- Argument and return type
- Const
- Pass by value or reference

Pass by value vs reference
Value - Parameter that is passed in, any changes to it are only reflected within the scope of the function. It doesn't affect the original copy.
Reference - Any change to the input param will affect the original param one hierarchy level up

## Arithmetic operators
Input params should be const (makes them read only to the implementation; we can't modify the value of `a` and `b` inside the function body), and pass by value only.

```cpp
// put this outside the class definition
// Arithmetic operator prototype (overloaded)
// defined as NON MEMBER FUNCTIONS
Rational operator+ (const Rational a, const Rational b){
  Rational result = Rational(
    a.getNum() * b.getDenom() + b.getNum() * a.getDenom(), 
    a.getDenom() * b.getDenom()
  );
  return result;
} // a1/a2 + b1/b2 = (a1b2 + b1a2) / a2b2
Rational operator- (const Rational a, const Rational b){
    Rational result = Rational(
      a.getNum() * b.getDenom() - b.getNum() * a.getDenom(), 
      a.getDenom() * b.getDenom()
    );
  return result;
}
Rational operator* (const Rational a, const Rational b){
    Rational result = Rational(a.getNum() *  b.getNum(), a.getDenom() * b.getDenom());
  return result;
}
Rational operator/ (const Rational a, const Rational b){
      Rational result = Rational(a.getNum() *  b.getDenom(), a.getDenom() * b.getNum());
  return result;
}
```
## Boolean (logic) operators
```cpp
bool operator== (const Rational a, const Rational b){
  // if a/b = c/d then ad = bc
  return (a.getNum() * b.getDenom() == b.getNum() * a.getDenom());
}
```

## Nonmember functions
ADT-specific functions outside of class definition. Do not have direct access to private data members. Functions designed to read ADT values without mutating them should be nonmember, to enhance encapsulation.
- Operator overloads are all defined outside the Rational class
- Standard arithmetic and boolean operators are not supposed to modify any contents of the operands, but using the inputs to return a new value. So they do not need to access the private data members, and they should be nonmember functions. 
- in/out stream are mandatory to be nonmember (>> , <<)

# IO Stream Operators (cin/cout)
These must be non member
1. Grab the reference to the output/input stream buffer. `ostream& sout` or `istream& sin` is the mandatory first parameter
2. Modify the buffer
  - Output: Attach the output strings to the buffer
  - Input: Acquire the num/denom from the input and save it to the buffer. Have to make variables to capture everything in the buffer. 
3. Return the reference of the modified output/input stream buffer.

The in stream operator needs to set the private field!
## Friend
Allows nonmember functions to access private fields
add `friend istream& operator>>(istream& sin, Rational& r) throw (const char*);` to the public section of the class declaration.
- Have to check legal values if the friended functions modify private fields.

```cpp
ostream& operator<< (ostream& sout, const Rational& r){
  sout << r.getNum() << "/" << r.getDenom();
  return sout;
}

istream& operator>> (istream& sin, const Rational& r) throw (const char*){
  char slash;
  int denom;
  int num;
  // we are using variables to capture things from the buffer. can't use >> "/" >>
  // we have to store it somewhere
  sin >> num >> slash >> denom;
  if(denom == 0) throw "Zero denominator";
  r.denom_ = denom;
  r.num_ = num;
  return sin;
}
```
After defining this, we should be able to do 
```cpp
cout << "a/b: ";
cin >> a;
cout << a + a << endl;
```

# ADT Type Conversion
If you do something like `short c = 5; if(c == 3) cout "yes";` the compiler will implicitly typecase 3 to a short, then compare. 
If you declare a Rational a; then `if(a == 3)`, the compiler will implicitly convert 3 to a Rational using the default constructor.
- Currently this will work because the default constructor will set denom to 1 if you give one parameter.
- What if it has multiple constructors? You don't know which constructor the compiler will pick
We should force the programmer to explicitly typecast 3 to a Rational
Compiler looks for one of these:
- Single argument constructors
- Multi argument constructors with default values

## Explicit Keyword in Constructor
```cpp
explicit Rational(int num = 0, int denom = 1) throw (const char*);
```
1. Forces the compiler to use this constructor to type case
2. Forces client to explicitly type cast. Implicit conversion not allowed.
`if(a==3)`, it will say No match for operator== between a Rational and an int.
need to `if(a == (Rational)3)` 

We should use explicit, when it is possible for the implementation to misinterpret a conversion outcome.

