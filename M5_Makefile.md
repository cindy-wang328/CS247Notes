# Compiling CPP into programs
So far we used `g++ -o myprog main.cpp foo.cpp`
- `-o` creates an output program with the indicated name (myprog)
- **Bundled compilation**: can supply 1 or more source files
  - single line, multi file compilation won't work with complicated projects
- **Separate compilation with linking**: Compile in order before linking
  - `g++ -c A.cpp` 
  - `-c` means compile but don't link; this generates `A.o` object file.
  - Do this in the order you want to compile them in (ex. A.cpp, then B.cpp, then main.cpp)
  - then `g++ A.o B.o main.o -o myprog` to link the 3 files **in order** to produce the program
  
## Compilation dependencies: when compilation order matters
For the above `g++ A.o B.o main.o -o myprog`, you will have to recompile everything if you change A. It will be impossible to track for large projects

There is an automated build system that needs to:
- Derive dependency relationships
- Incremental; only rebuilds what is changed
- include all updated files in the right order
- executable linking
**make**: write a makefile to specify dependencies, tell the linker how to compile
- tracks timestamp of files so it knows which to recompile

![diagram](https://github.com/cindy-wang328/CS247Notes/blob/master/screenshots/Screen%20Shot%202020-05-29%20at%209.40.55%20PM.png) 
```
           prog
      /     |     \
main.o      A.o    B.o
   |        |       |
main.cpp    A.cpp  B.cpp
     \      |       |
      \ ->  A.h <-> B.h
(C includes A, A, B .h incl each other)
```
### Example makefile
`prog: main.o A.o B.o`
- **Default Build Target**: defines the order they are supposed to be linked
- in order, client program before modules
  
  `g++ -g -Wall main.o A.o B.o -o prog` 
- **Build Rule**: The command given to the g++ compiler, when building the target)
- `-Wall` means warning all, `-g` is also for showing errors

`main.o: main.cpp A.h`
- **Subtarget**: for each `.o` in the default build target, specify its dependencies and build rule.
- the subtarget main.o, which should be supplied to the default target build rule `prog: main.o A.o B.o`, depends on main.cpp and A.h
  
  `g++ -g -Wall -c main.cpp -o main.o` (Convert main.cpp into the output main.o)
  
`A.o: A.cpp A.h B.h` 
- A depends on A.cpp and the 2 headers

  `g++ -g -Wall -c A.cpp -o A.o` 
  
`B.o: B.cpp B.h A.h`

  `g++ -g -Wall -c B.cpp -o B.o`

Save the makefile, go to the terminal in the same folder, and type `make`. This wil invoke `gmake` which will go line by line through the makefile and try to link it.
- Indents are mandatory in a makefile, for the build rules

## Implicit build rules
Obvious build orders and source file dependencies can be omitted, the builder already knows.
- If the CPP and H file have the same name; A.o automatically depends on A.cpp and A.h
  - only need to specify non-obvious dependencies (ex. A.h depends on B.h, so A.o also depends on B.h)
- Redundant target rule: the `-o main.o` part is not needed. 
- Most build rules are easy predictable (`g++ -g -Wall -c main.cpp`) 
With implicit rules, the makefile will be
```
prog: main.o A.o B.o (Default target and build rule) 
  g++ -g -Wall main.o A.o B.o -o prog
main.o: A.h (Non obvious dependencies)  
A.o: B.h
B.o: A.h
```
but this is still inefficient if there's 100 files 

## Build Macro 
```cpp
#build macros
CXX = g++                 # default compiler
CXXFLAGS = -g -Wall       # compiler option flags
OBJECTS = main.o A.o B.o  # objects to be linked, in order
EXEC = prog               # output filename

${EXEC}: ${OBJECTS}
  ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}
main.o: A.h 
A.o: B.h
B.o: A.h
```

## Clean Target 
Sometimes you want to delete all the object and executable files and completely recompile the project. 
```
clean:
  rm -rf ${OBJECTS} ${EXEC}
```

## Auto Dependency Derivation (-MMD)
Add `-MMD` into the `CXXFLAGS`. Then the auto builder will go into the header files and try to figure out dependencies. It will create `.d` files. 
- `DEPENDS = ${OBJECTS: .o = .d}` to substitute .o with .d for dependency files
- Don't need a manual dependency list
- Add `-include ${DEPENDS}`
  - this reads the .d files for rerunning auto dependencies
- Also clean the dependencies: `rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}` in the clean: part

# Makefile for Rational ADT
Copy everything into 1 folder, then `gedit makefile` (no extension)
```
# Build macros
CXX = g++
CXXFLAGS = -Wall -MMD
OBJECTS = main.o Rational.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = prog

${EXEC} : ${OBJECTS}
  ${CXX} ${CXXFLAGS} ${OBJECTS} -O ${EXEC} 

clean :
  rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}
  
-include ${DEPENDS}
```
for example, `main.d` is `main.o: main.cpp Rational.h PImpl.h`. `Rational.d` is `Rational.o Rational.cpp Rational.h PImpl.h` 
- to force make to recompile when testing, add some comments to Rational.cpp because it checks timestamps and only recompiles when code is changed
- `make clean` command will run the clean command. 
