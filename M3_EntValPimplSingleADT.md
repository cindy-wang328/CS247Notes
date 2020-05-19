# Value ADT vs Entity ADT
## Entity
Embodiment of real world entity; each object has distinct identity
- Objects with the same attributes are **not equal**
- ex. airplanes, cars, buildings, people, customers, player stats, score boards, transactions

These objects should not be equal to each other, even if their attributes are all the same values. Ex. You interact with a red apple, doesn't mean you also interacted with all the other red apples

## Value 
Represents a set of tangible values, such as fractions, matrices, vectors. Objects with **same attributes are equal**
- Mathematical and measurement types
  - rational numbers, polynomials, matrices
  - size, distance, weight
- Monetary quantities
- Color, location, datetime
- Names, addresses, postal codes

# Design guidelines
"these are only general guidelines, it always depends"
## Entity:
- Operations on object should reflect a real world event
- Can't copy an entity
  - prohibit assignment, copy constructor, type conversions (don't convert a car into a bus)
  - should clone (deep copy) 
**avoid arithmetic computations and equality/logical comparisons**
- logical operators useful iff overloaded to apply to name, ID etc 
  - ex. you hit game character A but B also takes damage; then you need to compare some kind of ID

Don't overload operators except new and delete

Always create these on the **heap** and refer to them by **pointers**

## Value:
Equality is important: operators should be overloaded
- relationship, copy, assignment operators
- Computation and arithmetic operators

Virtual functions and inheritance are uncommon
- Use the final keyword
- why would you need a new type of rational or vector handler?


# Mutability design guidelines
Strings are immutable in Java and C#, Primitives and entity types are usually mutable.

Mutable = Objects allowing attributes to be changed after initialization, by mutators/setters or other functions that change the private attributes. Some ADTs have public attributes (are mutable) but that is a bad practice. Anyone with a reference to a mutable object can modify its values.

```cpp
Person p ("Peter", new Date(31,1,1999));
// Create a new instance of Person, with a name, date of birth
cout << p.DOB() << endl;

Date d = p.DOB(); // Copy the reference of DOB
// Should not have assumed it is copy by value! 
d.setMonth(d.getMonth()+1); // now d is 31, 2, 1999
cout << p.DOB() << endl; // will be 31/02/1999
```
`Date d = p.DOB()` copied the reference of the person's DOB date object (which is also an ADT). d and `p.DOB()` are not separate independent objects - it was a shallow copy. Date object was mutable, so when it set the month, `p.DOB()` is also changed. `p` referred to the same object as `p.DOB()`
![Test Image](https://github.com/cindy-wang328/CS240-notes/blob/master/screenshots/Screen%20Shot%202020-05-18%20at%2011.38.02%20PM.png)

## Entity and value mutability
Entity objects are mutable - real world objcets and events are usually mutable

Value objects are *usually immutable*
- values can't be changed after initialization
- new values are assigned to a new value ADT instance (ex. string in java)
  - do the operation, create a new instance, and load the value into it
- no virtual (Member functions cannot be overridden, or else the child class can make it mutable)
- no mutators
- deep copy/cloning only
  - Copy and assignment operations are deep copy
- All data members are private
- All data members are primitive or immutable types
  - make a copy of any mutable attribute parameter or output return values

## Rational class updates
Since the Rational ADT is a value ADT, make it final to prevent it from being derived.
`class Rational final{`

Make Rational immutable
- Should not allow mutators; deprecate both the mutators, because the fields shouldn't be changed after initialization in the constructor.
- Arithmetic and logic operator overloads should stay; they are necessary for value ADTs. 
- instream stays...so far... the only friended non-member function that can mutate. 
  - So far it is the only thing allowed to mutate the Rational ADT, once. (ex .create an instance, then let instream initialize it once)
  - could possibly create a `bool initialized_` private field but not for now


![Test Image 2](https://github.com/cindy-wang328/CS240-notes/blob/master/screenshots/Screen%20Shot%202020-05-18%20at%2011.48.12%20PM.png)


# Information Hiding

Hide private data members from the object implementation.

What is not private implementation (Exposed implementation)
- clear view of private fields in the header file
- why leave these to be visible, if they are intended to not be explicitly changed?
  - Right now in the rational class the private fields are exposed to the programmer in the class definition. They can see the fields by scrolling through the code

## Private Implementation
Encapsulate all private data members in a separate struct
- maybe even put it in a separate header file
```cpp
struct PImpl{
  // private representation of a Rational
  int num_;
  int denom_;
}
...
private:
  //int num_; int denom_; 
  PImpl* rat_; // pointer to a rational private implementation
  void _reduce();
```
Change the constructor, reduce function, getters, instream >>
```cpp
Rational::Rational(int num, int denom) throw (const char*){
  rat_ = new PImpl;
  rat_-> num_ = num;
  rat_ -> denom_ = denom;
  ...
}
void Rational:: _reduce(){
  // replace num_ and denom_ with rat_ -> num_ and rat_ -> denom_ 
  for(int i - 1; i <= rat_ -> num_ && i <= rat_ -> denom_; i++){...}
}
int Rational::getNum() const{
  return rat_ -> num_;
} // same with getDenom()
istream& operator>> (istream& sin, Rational& r){
  char slash;
  sin >> r.rat_->num_ >> slash >> r.rat_ -> denom_; // don't forget the r.rat_ 
  return sin; 
}
```
We do not need to change the +-\*/ arithmetic operators because they are nonmember and don't need to access the private fields.

# Singleton Design Pattern
Exactly one object of the ADT, for the entire runtime of the program.
- Player in a single player game
- Global time
- Game controller (maps hardware to the game), game settings (won't need 2 sets of settings)

ex. Basic Player (Entity) ADT: Regular implementation
```cpp
class Player{
  public:
    Player(int val=0){val_ = val;}
    int getVal(){return val_;}
    void setVal(int val){val_ = val;}
  private:
    int val_;
}
int main(int argc, char** argv){
  Player* p = new Player();
  cout << p->getVal() << endl; // 0
  p->setVal(2);
  cout << p->getVal() << endl; // 
  // but you can create multiple instances of player! then when you move your mouse all of these will move
  Player* p2 = new Player(10);
  Player* p3 = new Player(20);
  return 0;
}
```

Turning an ADT object into Singleton
1. Create a static instance in private scope 
2. Move constructor to private scope
3. Static accessor in public scope
4. Prohibit copy and assign
5. Initialize before runtime

```cpp
class Player{
  // don't overload copy and assign operators
  public:
    // Step 3: Static AC to the single instance
    static Player* inst(){
      return &pl_; 
    }
    // ACs/MTs to the PFs
    int getVal(){return val_;}
    void setVal(int val){val_ = val;}
  private:
    // step 1
    static Player pl_; // the only instance 
    // static = (readily available/allocated in memory when program starts)
    // dont need to allocate on the heap

    // step 2: move ctor
    Player(int val=0){val_ = val;}
    int val_;
}
// step 5: initialize outside the class, also outside main function
// Basically a Global Variable
//Tell "pl" to call its constructor and feed value 42
Player Player::p(42);

int main(int argc, char** argv){
  //Player* p = new Player();  will not compile
  cout << Player::instance() -> getVal() << endl; // 42
  Player::instance() -> setVal(20);
  cout << Player::instance() -> getVal() << endl; // 20
  return 0;
}
```
