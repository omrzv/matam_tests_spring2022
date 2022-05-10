GPP=g++
OBJS=utilities.o Card.o Player.o Mtmchkin.o tests.o
EXEC=unitest
DEBUG_FLAG= -g# can add -g
COMP_FLAG=--std=c++11 -Wall -Werror -pedantic-errors

$(EXEC) : $(OBJS)
	$(GPP) $(COMP_FLAG) $(OBJS) -o $@

utilities.o : utilities.cpp utilities.h
	$(GPP) -c $(COMP_FLAG) $*.cpp -o $@
Player.o : Player.cpp Player.h
	$(GPP) -c $(COMP_FLAG) $*.cpp -o $@
Card.o : Card.cpp Card.h Player.h utilities.h
	$(GPP) -c $(COMP_FLAG) $*.cpp -o $@
Mtmchkin.o : Mtmchkin.cpp Mtmchkin.h Player.h Card.h utilities.h
	$(GPP) -c $(COMP_FLAG) $*.cpp -o $@
tests.o : tests.cpp Mtmchkin.h Player.h Card.h catch.hpp
	$(GPP) -c $(COMP_FLAG) $*.cpp -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)
