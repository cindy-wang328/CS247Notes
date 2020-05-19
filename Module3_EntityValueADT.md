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

Always create these on the heap and refer to them by pointer

## Value:
Equality is important: operators should be overloaded
- relationship, copy, assignment operators
- Computation and arithmetic operators

Virtual functions and inheritance are uncommon
- Use the final keyword
- why would you need a new type of rational or vector handler?
