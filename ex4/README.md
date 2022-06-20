# Tests for ex4
There is only End to End (file tests) for this exersize.

## File Tests:
* Make an executable with the name **FileTester** how ever you choose using the **fileTests/testsMain.cpp** file.
* **fileTests/tester.sh**
* Expect the architecture below. if yours differs, change the files marked *MIGHT NEED CHANGES*
    ./ *terminal / cmd / wsl here* 
    
    ├── fileTests/ 
    
    |   ├── tester.sh    

    |   ├── inFiles/    

    |   |   └── ... 

    |   ├── outFiles/    
    
    |   |   └── ... 
    
    ├── FileTester  
    
    └── ... 
* we suggest you increase the amount of tests you run in *tester.sh* after you've passed the first 10
    
* Contains the following files:
    * tester.sh - Runs the the test, *you can change the amount of tests runs in it*
    * testsMain.cpp - The main program (simulates the game and writes the results)
    * inFiles - in files in the format of testXXXX.in and testXXXX.dock
    * outFiles - out files in the format of testXXX.out and after you ran the tests testXXX.result 
