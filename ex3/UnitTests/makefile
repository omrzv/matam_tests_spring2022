GPP=g++
PROJ_FILES_PATH=.
HEALTH_PATH=$(PROJ_FILES_PATH)
QUEUE_PATH=$(PROJ_FILES_PATH)
TESTS_DIR=UnitTests
O_FILES_DIR=$(TESTS_DIR)/OFiles
EXEC=UnitTester
TESTS_INCLUDED_FILES=$(TESTS_DIR)/QueueUnitTests.cpp $(TESTS_DIR)/HealthPointsUnitTests.cpp $(HEALTH_PATH)/HealthPoints.h $(QUEUE_PATH)/Queue.h $(TESTS_DIR)/catch.hpp
OBJS=$(O_FILES_DIR)/HealthPoints.o $(O_FILES_DIR)/UnitTests.o 
DEBUG_FLAG= -g# can add -g
COMP_FLAG=--std=c++11 -Wall -Werror -pedantic-errors $(DEBUG_FLAG)

$(EXEC) : $(OBJS)
	$(GPP) $(COMP_FLAG) $(OBJS) -o $@

$(O_FILES_DIR)/HealthPoints.o : $(HEALTH_PATH)/HealthPoints.h $(HEALTH_PATH)/HealthPoints.cpp 
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) $(HEALTH_PATH)/HealthPoints.cpp -o $@
$(O_FILES_DIR)/UnitTests.o : $(TESTS_DIR)/UnitTestsMain.cpp $(TESTS_INCLUDED_FILES)
	@mkdir -p $(O_FILES_DIR)
	$(GPP) -c $(COMP_FLAG) $(TESTS_DIR)/UnitTestsMain.cpp -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)