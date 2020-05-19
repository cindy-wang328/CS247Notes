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
