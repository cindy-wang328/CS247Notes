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
1. A new local copy instance implicitly instantiated in assignment function, based on the constructor of myClass; this is returned at the end by reference (`*this`)
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

## copy swap
Change input parameter to PBV; now new copy of source on stack

Once it is PBV, the cctr is called to make copy of the source on the stack. At the end of the function, this copy on the stack is deleted. 
- the operation copying the source to the stack, with PBV, is safer because an improperly allocated object can be deleted properly with the dtor. it has direct access to the dtor

### swap 
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


