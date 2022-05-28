# Tests for ex3
There is only UnitTests for this exersize.

## Unit Tests:
* **make -f UnitTests/makefile && ./UnitTester && valgrind --leak-check=full ./UnitTester**
* Expect the architecture below. if yours differs, change the files marked *MIGHT NEED CHANGES*
    ./ *terminal / cmd / wsl here*
    ├── UnitTests/
    |   ├── makefile   
    |   └── ...
    ├── HealthPoints.cpp
    ├── HealthPoints.h
    ├── Queue.h
    └── ...
* Contains the following files:
    * makefile - To make the tests. *MIGHT NEED CHANGES*
    * UnitTestsMain.cpp - The main program (only includes)
    * relativeIncludes.h - Includes for YOUR project files. *MIGHT NEED CHANGES*
    * QueueUnitTests.cpp - Unit Tests for Queue, where errors might send you.
    * HealthPointsUnitTests.cpp - Unit Tests for HealthPoints, where errors might send you.
    * catch.hpp - The Unit Test library we use. Nothing to see there
