# Unit tests for ex1
These are tests for all the functions in RLEList.h
They are testing most behaviors, with the exeption of memory allocation fail, and to test memory managment you must use valgrind.

to run the tests set up the makefileRLEUnitTests in the same folder as RLEList.h and RLEList.c, and set the RLEUnitTests folder in the same folder, and run the command:

make -f makefileRLEUnitTests && RLEUnitTests/RLEUnitTester.exe

make -f makefileRLEUnitTests && valgrind --leak-check=full RLEUnitTests/RLEUnitTester.exe

you can also run only a single test, for example:
RLEUnitTests/RLEUnitTester.exe 1
Will only run the example tests given by the metargelim
