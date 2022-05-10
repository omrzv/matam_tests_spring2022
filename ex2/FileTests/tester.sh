#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

shopt -s nullglob
FAILED_TESTS=0

if [ ! -d FileTests ]; then
	printf "Can't find FileTests, expacts the following dir:\n"
	printf "	./FileTests/...\n"
	printf "	./Mtmchkin.cpp\n"
	printf "	./Mtmchkin.h\n"
	printf "	./Card.cpp\n"
	printf "	./Card.h\n"
	printf "	./Player.cpp\n"
	printf "	./Player.h\n"
	printf "	./utilities.cpp\n"
	printf "	./utilities.h\n"
	exit 1
fi

if [ ! -d FileTests/oFiles ]; then
	mkdir FileTests/oFiles
fi

make -f FileTests/makeFileTests clean

make -f FileTests/makeFileTests

for i in FileTests/InOutFiles/*.in
do
	printf "test $i >>>  "
	./FileTester $i ${i%.*}.result
	diff ${i%.*}.result ${i%.*}.out
	#rm ${i%.*}.result

	if [ $? -eq 0 ]
	then
		printf "Game Simulation: ${GREEN}pass${NC},   "
	else
		printf "Game Simulation: ${RED}fail${NC},   "
		FAILED_TESTS+=1
	fi
	valgrind --log-file=$i.valgrind_log --leak-check=full ./FileTester $i ${i%.*}.vresult 1>/dev/null 2>/dev/null
	rm ${i%.*}.vresult
	if [ -f $i.valgrind_log ]
	then
		cat $i.valgrind_log | grep "ERROR SUMMARY: 0" > /dev/null
		if [ $? -eq 0 ]
		then
			printf "Leak: ${GREEN}pass${NC}\n"
		else
			printf "Leak: ${RED}fail${NC}\n"
			cat $i.valgrind_log
			FAILED_TESTS+=1
		fi
	else
		printf "Leak: ${RED}couldn't get valgrind file${NC}\n"
		FAILED_TESTS+=1
	fi
	rm $i.valgrind_log
done

if [ ${FAILED_TESTS} -eq 0 ]; then
	printf "\n${GREEN} All tests passed :)${NC}\n\n"
else
	printf "\n${RED} Failed ${FAILED_TESTS}${NC} tests.\n\n"
fi
printf "Use FileTests/clean.sh to remove the tests results and compiled files\n"
