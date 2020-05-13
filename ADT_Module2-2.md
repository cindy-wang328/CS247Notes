# Standard features of ADT part 1
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


