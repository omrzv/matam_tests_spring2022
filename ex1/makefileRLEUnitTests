CC = gcc
OBJS = RLEList.o RLEUnitTests//RLEListUnitTest.o
EXEC = RLEUnitTests//RLEUnitTester.exe
DEBUG_FLAG = -g # now empty, assign -g for debug
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors $(DEBUG_FLAG)
$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@
RLEList.o : RLEList.c RLEList.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
RLEUnitTests//RLEListUnitTest.o : RLEUnitTests//RLEListUnitTest.c RLEUnitTests//test_utilities.h RLEList.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c -o $@
clean:
	rm -f $(OBJS) $(EXEC)
