# Special member functions
compiler provides default versions if not defined by programmer. 
- ctor, dtor, copy ctor, assignment (operator=) , move ctor, move assignment

## Default constructor
Compiler generates these
- primitive types, pointers uninitialized (won't allocate something on the heap for the pointer to point to)
- member objects: initialized using member's default constructor
- Inherited members: initialized using base class default ctor

## Default destructor
- Primitives deallocated 
- Pointers deallocated but not deleted (The pointer is gone, but the memory the pointer references in the heap is not deallocated)
	- May cause memory leak, but not always (Could be other pointers)
	- Should overload the dtor to delete the referenced data, if you are certain it is not referenced by other pointers. 
- Member objects: member destructor
- Inherited member: base class dtor

## Copy ctor
constructs new obj whose value equals an existing obj - copies the values of all the data members in some instance (m), and stores it into the new object

ex. assume `void f(Money m)`  (Function accepting Pass By Value) is defined, and `Money m;` in main. These call CCTR automatically
- `Money n{m};`
- `Money p = m;` (Same as prev)
- `f(p);` (Calls CCTR to make a copy on stack, for PBV)

### Default copy
- pointer, primitive: Bitwise copy
	- Ptrs point to same referenced data as original
	- uses shallow copy; most conservative way, least memory usage
		- deep copy: target and source have diff reference data in the heap, but inside the reference data, should be the same value
- Member object: Member CCTR
- Inherited member: Base class cctr

### Overload CCTR
Needs to take one parameter, a const ref to an ADT inst
```cpp
class Money{
	public:
		Money();
		~Money();
		Money(const Money &m){ // CCTR
			amt_ = m.amt_; // copy into local PFs
		}
	private:
		int amt_;
}
```

## Assignment (Copy assignment) operator
Similar to cctr but destination already exists
`Money m, p; p=m;` 2 ADT insts already declared, assigning one to the other. After this, p and m should have equal values in all data members
- Compiler's default assignment operator is the same copy behavior as default cctr (shallow copy); should overload these if you need deep copy and not bitwise ptr copy

### Overload 
```cpp
// inside public: of class Money
Money& operator=(const Money &m){
	this->amt_ = m.amt_; // copy value
	return *this; // return copied adt inst
}
```


# Copy swap idiom
## Deep copy attempt 1
```cpp
Source(Pass by reference)   |
Base                        |
comp_                       |
ptr_ -----> Referenced Data |
simp_                       |
```
1. A new copy instance implicitly instantiated in assignment function, based on the constructor of myClass; this is returned at the end
2. all non ptrs are copied from the source
3. ptr member of the *local copy instance implicitly created by assignment operator* is deallocated
	- ref data is not a fixed size, it might be a dynamic array. default size definied by the constructor of myClass might not be the right size to hold the reference data
	- delete it then reallocate using a known size from the source's reference data
4. m.psize gets the size of the referenced data; then allocate a pointer to point to the new section, store the address into the ptr_ of the local copy
5. copy the value from the referenced date of the source, to the reference of the local copy instance

```cpp
myClass& operator= (const myClass &m){
	Base:: operator=(m);
	this->simp_ = m.simp_;
	this->comp_ = m.comp_;

	delete this->ptr_;
	this->ptr_ = new type[m.psize];
	memmove(this->ptr, m.ptr_, m.size*sizeof(type));

	return *this;
}
```

### Toxic pointer
Problem: if exception occurs when reallocating `this->ptr_ = new type[m.psize]`, the local copy instance's reference gets deleted but never reallocated
- the reallocation operation could return null, do nothing, or return a section of memory that is not big enough
- it is bad to hold onto a ref of a deleted memory section; you might pass it to a destructor, but the heap can't delete it because it "didn't belong to you" in the first place.
- the memory might have been allocated to other programs, and the toxic pointer can change it (will cause race conditions)

## Copy swap idiom
Change input parameter to PBV; now new copy of source on stack

Once it is PBV, the cctr is called to make copy of the source on the stack. At the end of the function, this copy on the stack is deleted. 
- the operation copying the source to the stack, with PBV, is safer because an improperly allocated object can be deleted properly with the dtor. it has direct access to the dtor

### Swap 
swap pointer member of the local version, and the source on the stack
- just like attempt 1, all non-ptr members are put into the local copy
- now the source (stack) copy points to something probably undefined
	- but it gets deleted anyway
- local copy points to right piece of ref data in the heap
- at the end, the local copy is returned by ref to assignment destination
- modified copy of the source on the stack is destroyed using the default dtor

```cpp
myClass& operator= (const myClass &m){
	Base:: operator=(m);
	this->simp_ = m.simp_;
	this->comp_ = m.comp_;

	swap(this->ptr_, m.ptr_);

	return *this;
}
```

#### BDPs
Advantage: If exception occurs when using CCTR to copy the source onto the stack, the dtor is automatically called, to prevent toxic pointers. Copy swap is more reliable.
- use copyswap in operator= whenever pointer members are present, or any chance of memory related exceptions
- it is possible to `swap(*this, m); return *this'` (STL library swap; the overhead of calling STL might invoke more actions than you think)
- non copyswap is faster. if you have very low chance of running into memory issues, maybe don't need to copyswap. meanwhile if you have low memory available, it is likely for more exceptions, so you should use it.

### Textbook example (item 11: handle assignment to self in operator=)
#### Assignments to self:
- `Widget w; w=w;`
- `a[i]=a[j];` if i = j
- `*px=*py;` if px and py point to the same thing
- `void f(const Base& b, Derived* d)` b and d might be the same object
In general, if there's pointers to multiple objects of the same type / same hierarchy, we need to consider what if the objects are the same

For example you have a `Bitmap *pb;` as a PF in the Widget class
```cpp
Widget& Widget:: operator=(const Widget& rhs){
	delete pb;
	pb = new Bitmap(*rhs.pb);
	return *this;
}
```
`*this` and `rhs` could be the same object. If they are the same, `delete` will destroy the bitmap for the current object and rhs. So the Widget will have ptr to deleted object.
- Can add identity test `if(this == &rhs) return *this;`

#### Making operator= exception safe
One way is to `Bitmap *po = pb; pb = new Bitmap(*rhs.pb); delete po;` so that if `new Bitmap` exceptions, the Widget doesn't change.
- Can do `swap(*this, rhs); return *this;` if rhs is passed by value

## Move constructor
Does same thing as copy (create a new object whose value is equal to existing) but **does not preserve value of existing object**

Calls move constructor upon f2 return, to initialize n. It returns the inst. through the stack. The = makes a copy of the values on the stack to the new instance n. compiler needs to construct n, copy value of whatever is on the stack, with the move constructor. the source will be cleared out anyway, so it doesnt care about preserving the value. compiler will use move constructor. 

```cpp
class Money{...};
Money f2(); // function returning ADT
int main(){
	Money m; // Calls constructor
	Money n = f2(); // Calls move constructor upon f2 return
}
```
### Overload
Input parameter is a **rvalue reference**, tell compiler that this constructor is move. Don't need to care about the integrity of the source content, as long as the value you're copying to the new instance is correct


```cpp
Money(Money &&m){
	amt_ = m.amt_; // copy value
	m.amt_ = 1234; // doesnt matter
}
```
Default
- Primitive, pointer: bitwise/shallow copy
- Member object: member move/copy constructor
- inherited: base class m/c ctr

Deep copy is pointless

 The source gets deleted, since it was on the stack. No deep copy required.  Deep copy creates 2 copies of reference data, but we don't care about the source contents since the source content gets destroyed (was on stack). Don't need copy swap either 

## Move Assignment
Similar to MCtr but destination of move already exists, and also doesn't preserve the source integrity.

Same default behavior as MCtr

```cpp
int main(){
	Money m, p; // calls CTR
	p = Money(); // calls CTR then Move assignment

```
- m and p are defined and initialized
- for p, ask for a new initialization; the ctr will be called to create new instance. move that inst over to the existing instance p. 

```cpp
Money& operator= (Money &&m){
	this->amt_ = m.amt_;
	m.amt_ = 1234; // doesnt matter
	return *this;
}
```
**pay attention to lvalue vs rvalue pass by reference**

# Rule of 5
### If you overloaded one special member fn, you should overload all of them
What does compiler do if programmer overloads some, but not all, of the special member functions

Compiler prefers copy instead of move. Move doesn't care about integrity of source - can't use move to copy, but can use copy to move

1. Nothing: All compiler default (CD)
2. Any Ctor: Default ctor not declared (ND), all others CD
3. Default Ctor: all others CD
4. Dtor: all CD except move ctor and move assign are ND
5. Copy Ctor: Default ctor ND, MCtr and MAsn ND, Dtor and Copy Asn CD
	- The default ctor will be replaced by the copy ctor
6. Copy Asn: All CD except Move ctor and asn ND
7. Move Ctor: default ctor ND, move asn ND, dtor CD, and copy ctor and asn *deleted*
	- copy is replaced by move
8. Move Asn: copy ctor/asn deleted, move ctor ND, others CD

# Equality operator
Shallow equality: does not compare the referenced data

Deep equality: look at the values of the non pointer members, but look at the referenced data for pointer members (*ptr1 == *ptr2) instead of the pointer address themselves
- No compiler default equality operators
- should compare member by member 

```cpp
class myC{...};
bool operator==(const myC &m, const myC &n){
	// call the base class's operator== 
	if(!(Base:: operator==(m, n))) return false; 
	// primitives
	if(m.simp_ != n.simp_) return false;
	// complex data member (objects): call their operator==
	if(!(m.comp_ == n.comp_)) return false;
	// Pointers
	// If both pointers are null, return true
	if(!m.ptr_ && !n.ptr_) return true;
	// if only one is null
	if(!m.ptr_ || !n.ptr_) return false;
	// check reference data
	return *m.ptr_ == *n.ptr_;
}
```
# Rational class updates
### Test cases: Assume f takes Rational as param (copies onto stack), f2 takes in and returns instances of Rational (from stack into a variable) 
- there is one pointer member (the PImpl)
```cpp
void f(Rational r){return;}
void f2(Rational r) {return r;}
Rational a;
Rational b{a}; // Copy ctor
Rational c = a; // cctr also
f(c); // cctr
b=c; // copy asn
Rational d = f2(c); // copy then move ctor called
b = f2(c); // move asn (copy ctor, move ctor, then move assignment
```
ex. for `Rational d = f2(c)`, cctr first called from pass by value into f2, then mctr called to move it into d.

We defined only a def ctor so the compiler will supply the other 5, but we should define the other 5 for examples and deep copy

## Destructor
Mandatory if there are pointer members. Delete the referenced data on the heap to prevent memory leak
`~Rational();`
```cpp
Rational::~Rational(){
	if(rat_ != NULL){
		delete rat_;
	}
}
```
## Copy ctor
`Rational(const Rational&r);`
```cpp
Rational:: Rational(const Rational&r){
	rat_ = new PImpl;
	rat_ -> num_ = r.rat_ -> num_;
	rat_ -> denom_ = r.rat_ -> denom_; // Allocate another PImpl and copy the values of the input.
}
```
## Copy asn
(no copy swap for now)

the operator= by default creates a local container copy of the ADT instance on the stack, then at the end we return it through the reference back to the lvalue of the operator (what's on the LHS of the =)

sometimes, the pfs are not fixed size, so we can't trust the size of the temporary container for the reference data and the pointer member. the default initialized size of the PF might not be correct

`Rational& operator=(const Rational&r);`
```cpp
Rational:: Rational& operator=(const Rational&r){
	delete rat_;
	rat_ = new PImpl; // redundant, but usually done if dynamic members
	rat_ -> num_ = r.rat_ -> num_;
	rat_ -> denom_ = r.rat_ -> denom_;

	return *this; // return the local container instance
}
```
## Move ctor
`Rational(Rational &&r);`  rvalue &&

We can mess around with the input parameter since it is move

Use rvalue, because the lvalue will mess with the source instance one hierarchy level up, and pass by value will make an additional call to copy ctor. 

```cpp
Rational:: Rational(Rational &&r){
	// do copy swap
	// copy is already done with the rvalue (you can change r)
	rat_ = new PImpl; // need to new because you're creating another instance, target doesn't exist yet
	//swap
	PImpl * temp = this -> rat_; // temp pointer to save the auto allocated pointer to the ref data 
	this->rat_ = r.rat_; // redirect the local container's ptr to the source object's ref data
	r.rat_ = temp; // swapped the auto generated ptr member into the source

}
```
## Move asn
`Rational& operator= (Rational &&r); `// rvalue 
```cpp
Rational& Rational:: operator= (Rational &&r){
	// first 3 are same as move stor
	PImpl * temp = this -> rat_; 
	this->rat_ = r.rat_; 
	r.rat_ = temp; 
	return *this; // r is popped from stack, so it's ok to have bad pointers r.rat_
}
```
