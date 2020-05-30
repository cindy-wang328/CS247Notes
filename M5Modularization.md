# Program compilation and decomposition
Build process, headers, dependencies

1. Preprocessing: C++ source code and header files
  - looks at #include, #define, to include the header files. the header files are copy and pasted in place of the #include. 
  - preprocessed source code is 95% not done by you, if you include a lot of things
2. 1st stage translation (compiler)
  - C++ can be compiled onto different CPU platforms
  - Compiler translates into CPU specific machine instructions (assembly)
3. 2nd stage translation (assembler) 
  - Converts assembly code into binary
4. Linking
  - Libraries and other object code files (`.o`) are brought together
  - C++ `.cpp` files compile to `.o`, and an executable is produced

Linker is evil? the linker, in the black box IDEs, use a computer generated 
makefile and linking file, and sometimes these linkers have bugs. 
Sometimes you have to make a new project and copy over the files. The linker messed up the linking file and makefile in the background

## Program decomposition
This file is too big:
```cpp
#include ...
using namespace ...
int a, b;
...
struct PimplA();
struct PimplB();
...
class A{};
class B{};
...
A::A();
A::getter();
...
B::B();
B::~B();
...
A& operator=(const A& a);
ostream& operator<<...;
...
int main(){...}
```
Separate into A.cpp, B.cpp, and main.cpp: A contains the PimplA, class A{}, A::getter(), etc. Main.cpp has only the main function. 

Problem: class A and B must be **defined before main() to be used in main**

### Method 1: share the variables using non-local declaration, using `extern int b=35` 

Extern keyword: 
- if the variable has an initial value, indicates the variable is nonlocal and can be used in other cpp files.
- if it has no initial value, indicates the definition is in another cpp file.
- be careful, it can lead to modularization meltdown; you will need to locate which file the extern variable is defined. Don't extern a class.
```cpp
main.cpp:
extern int b;
cout << B; // 35
...
B.cpp:
extern int b=35;
```
### Method 2: header file
- in `A.h` (same name as the cpp file), put the declarations
  - `struct PimplA{}; class A{};`
  - these are the protytpes and structures I will be using in A.cpp
- in `A.cpp`:
  - `#include "A.h"` tells the compiler to copy and paste the content of A.h into where the include is
  - This should contain the concrete implementation of all the A class functions
    - `A::A(); A::getter();`
- in `main.cpp`:
  - `#include "A.h"` -> the preprocessor copies everything from `A.h`
    - after compiling, there is `a.o` and `main.o` and they are linked together, so that the main function can use the A class (A is defined)
  - contains the `int main()` function
- BDP: header file should be the recommended modularization method, instead of nonlocal. 

### Duplicated header inclusions, header guards
- C.cpp includes C.h
- D.h includes C.h
- D.cpp includes D.h
- main.cpp includes C.h and D.h
- then C.h has a duplicated inclusion in D.h and main.cpp, copy and pasted twice. It will not compile.
Use `#ifndef C_H #define C_H .... #endif`
- if C_H is not defined, then everything from #define C_H to #endif, copy and paste it into wherever included "C.h"
- Then if main.cpp #include "C.h" then #include "D.h", in the first include, C_H flag is already defined, so when compiler sees the #include "C.h" inside `D.h`, it will not define it again. 

### Circular dependencies, Forward declaration
- if E.h includes F.h, and F.h includes E.h  -> circular
- Forward declaration: inside E.h, put `class F;` and inside F.h, put `class E;`. 
- Then the compiler will know to break the loop even if the #includes are still there

### Namespace etiquette
- Never `using namespace` in a header file
  - someone can accidentally include a namespace they don't know about when they include your header
- Never `using namespace` before the `#include` directives
  - some header files have their own visibility of their own namespaces, it will affect the set of names visible in the header files
  - can mess up the preprocessor


# Update Rational ADT
341 lines in the Rational class!

The `struct PImpl{}` and `class Rational final{}` declarations should be in the header file `Rational.h`. Nonmember function prototypes also go in the header file (`Rational operator+(Rational a, Rational b);`)
- We moved the implementations outside the class declaration in order to allow modularization.
- `Rational.h` should not have `using namespace std`
  - but you still need to `#include <iostream>`
  - if you took out the `using namespace std`, have to put `std::istream` and `std::ostream` 
  
Implementations of the functions (ex. constructors) should stay in the source cpp file `Rational.cpp`
- `#include "Rational.h"` at the top, because "" indicates a path is provided to locate the file, while <> indicates the header file is in the default library locations from the environmental variables
- including iostream is optional, because it is already included from Rational.h. iostream library has header guard, so it is fine either way


Main function goes into the "client program", a separate cpp file
- Put all 3 files in the same folder
- Compile with `g++ -o prog main.cpp Rational.cpp`; this tells compiler to compile them together into a program and name it "prog"

Sometimes we put PIMPl into a separate header
- cut and paste `struct PImpl{...};` into `PImpl.h`
- #include "PImpl.h" in Rational.h
