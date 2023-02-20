# UNIX_assemblyprogramming

1 Task
One of the only remaining legitimate reasons for learning to program in assembly
is to write a compiler that translates instructions in some higher level language
to machine code for the target architecture. In this lab you will do just that, but
in order for you to not have to spend time learning compiler construction, the
compiler proper will be given to you and you only have to change the current
backend that emits code for a hypothetical stack machine to one that emits
assembly code for the x86-64 architecture on Ubuntu Linux. This lab requires
very little code from you, but a great deal of understanding.


2 Background


2.1 The Provided Compiler


The compiler compiles code in a very simple calculator language to assembly
pseudo code. In order to build the compiler, you should follow the instructions
in the file ’build’. For this assignment, only ’calc3b.exe’ is important1.
The basic idea of this assignment is that you rename the file ’calc3b.c’ into
’calc3i.c’ and change the print statements to one or more print statements that
emit the x86-64 instructions that you want instead. Note that if you use a
register in one of the case blocks you have to make sure that you either do not
need it at the end, or that it is saved (and restored) before you exit (or make
a recursive call to ’ex’). You probably do not have to save and restore registers
however.


2.2 The Language


The calculator language is equally simple; it doesn’t contain any instructions
for input and only one for output. There are variables that can be assigned to
and read from, but they are restricted to single letter identifiers that are the
lower letters from a-z, thus one cannot have more than 26 different variables2.
This makes handling the symbol table very easy as one can just allocate 26 memory
positions and index into that array given the single letter identifier (i.e. ’a’ is the first position,
’b’ the second, and so on), so when the compiler sees the variable ’b’ it can just store or fetch
the data at the second position in the array (doesn’t have to be an actual array, labels are
fine).


 The output consists of
’push/pop’ instructions that either push the value in variable a onto the stack
or pop the value off the top of the stack and stores it in the variable.
Operators are equally simple; arithmetic operators like ’sub’ above pop two
values off the stack, subtract them and push the result back on the stack. Thus
the statement ’a=a-b’ in the calculator language is converted to; ’push a; push
b; sub; pop a’, which puts the values in variables ’a’ and ’b’ on the stack in the
correct order, subtracts them, and stores the result back into variable ’a’. Since
the x86-64 ’sub’ instruction does not take its arguments from the stack and does
not put its result onto the stack, you will have to add extra instructions to do
so, when handling the substraction case.


The control structure consists of comparison instructions ’compGT’ for ’compare
greater than’ which sets an invisible true or false flag somewhere in the computer
that subsequent ’jz’ - ’jump if zero’ instruction can act on, incidentally the
only conditional jump instruction. The compiler proper (in file calc3b.c in the
included files) can thus do its work by a simple recursive strategy that does not
need to take more than the current input token into account, i.e. it only needs
to check if the previous comparison instruction returned true or false.


3 Requirements


3.1 Project Structure
The basic folder structure for your project has to look as follows (you may add
folders, if necessary):


project


|----- bin/


|----- lexyacc-code/


|----- lib/


|----- src/


|----- c-driver.sh


|----- x86-64-driver.sh


+----- Makefile


The lexyacc-code folder shall contain all source files for your compiler. Basi-
cally, you can just use the provided folder and add your calc3i.c (and calc3c.c -
cf. Section 3.5) file. You can either create build artefacts in this folder as well,
which is not so nice, but permitted, or you could create a dedicated folder for
the build artefacts.
The src folder should contain the source files for your external library . As with the compiler code, you can decide where the build artefacts
will be created.
Last but not least, it must be possible to build your compiler as well as the
external library by issuing the make command within the project folder, which
will place the compiler executable(s) in the bin folder, while the library should
be placed into the lib folder.


3.2 Basic Compiler


As described above, your task is to change calc3b.c into calc3i.c that instead of
emitting the pseudo assembly code emits actual x86-64 assembler instructions
(that uses the x86-64 stack). Your compiler should emit code that handles 64
bit signed integers.
Since the provided compiler only produces the instructions required to translate
the calc language code, it does not by itself create an assembly program, which
can be compiled into an executable. This is the case, since it does not define data
and text segments as well as the symbol table, which you need in order to handle
variables. Furthermore, the compiler also does not call the exit function/system
call to terminate the produced program.
In order to produce an assembly program, which can be compiled into an execu-
table, you will have to write a shell script, called ’x86-64-driver.sh’, which takes
as input a file with the ’.calc’ ending and then:


1. writes the required prologue (e.g. define data and text segment, define the
symbol table) into a new file with the ending ’.s’


2. appends this file with the output of your compiler (i.e., calc3i.exe) for the
given ’.calc’ file


3. appends an epilogue (e.g. for calling the exit function/system call)


After that your driver should call ’gcc’ (or ’as’ and ’ld’ separately) to assemble
and link the assembly file to produce an executable.
For example, when I run your shell script as follows: ’x86-64-driver.sh bcd.calc’,
I expect as output a file called ’bcd.s’, which contains the produced x86-64
assembler code for the file ’bcd.calc’, as well as a file called ’bcd’, which is an
executable program, which does what was written in ’bcd.calc’.


3.3 Additional Functions


The lab requires you to implement support for three new ”instructions” that are
not in the stack machine described in the original Lex and Yacc tutorial. These
must be implemented as function calls, that is your compiler should emit a call
to a library function whenever it handles one of these new instruction cases. You
will of course also have to supply the implementation (in assembly language) of
the routines that calculate the results of these new instructions.

gcd - Take two arguments and return the greatest common divisor between
the two arguments. For example: 36 gcd 24 = 12.
