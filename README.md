# **Single Pass Compiler**
This is a simple single-pass compiler.
### Compilation steps:
```
make
```
After Compilation two files will get generated - 

  - lextest
  - compile
  
### Clean all the .o files:
```
make clean
```
### Clean lextest, compile and all the .o, .class files:
```
make distclean 
```
#### 1.lextest: It is the scanner which will return a series of <token, tokenval> if a file is passed as input
```
Example:
  lextest < mytest
```  
#### 2.compile: It is our simple single-pass compiler which will take a file as input and generate Code.class
```
Example:
  compile < sample1 //Compilation step
  java Code 3 5     //Input
```  
#### Some sample program file is also included in the codebase- mytest, sample1, sample2, sample3, sample4 and sample5. 
 *sample5 is for checking Invalid Characters.*
