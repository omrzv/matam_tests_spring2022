#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

run_pretest() {
	if [ -f ${1%.*}_pre.sh ]
	then
		echo -n "Running pretest: "
		./${1%.*}_pre.sh
		if [ $? -eq 0 ]
		then
			printf "${GREEN}done${NC}\n"
		else
			printf "${RED}error${NC}\n"
			exit 1
		fi
	fi
}

run_posttest() {
	if [ -f ${1%.*}_post.sh ]
	then
		echo -n "Running posttest: "
		./${1%.*}_post.sh
		if [ $? -eq 0 ]
		then
			printf "${GREEN}done${NC}\n"
		else
			printf "${RED}error${NC}\n"
			exit 1
		fi
	fi
}

if [ $# -ne 2 ]
then
	echo "Usage: $0 <run file> <tests_dir>"
	exit 1
fi

shopt -s nullglob
RET=0

make -f MakefileTest clean
make -f MakefileTest 

for i in $2/{*.in,test*.sh}
do
	printf "test $i >>>  "
	run_pretest $i
	if [ "${i##*.}" = "in" ]
	then
		./$1 -e $i ${i%.*}e.out
		diff ${i%.*}e.out ${2}/encoded${i//[^0-9]/}.out
		rm ${i%.*}e.out
	else
		./$i -e $(realpath $1) ${i%.*}.out
	fi
	if [ $? -eq 0 ]
	then
		printf "Encoded Working: ${GREEN}pass${NC},   "
	else
		printf "Encoded Working: ${RED}fail${NC},   "
		RET=1
	fi
	if [ "${i##*.}" = "in" ]
	then
		./$1 -i $i ${i%.*}.out
		diff ${i%.*}.out ${2}/inverted${i//[^0-9]/}.out
		rm ${i%.*}.out
	else
		./$i -i $(realpath $1) ${i%.*}.out
	fi
	if [ $? -eq 0 ]
	then
		printf "Inverted Working: ${GREEN}pass${NC},   "
	else
		printf "Inverted Working: ${RED}fail${NC},   "
		RET=1
	fi
	if [ "${i##*.}" = "in" ]
	then
		valgrind --log-file=$i.valgrind_log --leak-check=full ./$1 < $i 1>/dev/null 2>/dev/null
	else
		./$i "valgrind --log-file=$(pwd)/$i.valgrind_log --leak-check=full $(realpath $1)" 1>/dev/null 2>/dev/null
	fi
	if [ -f $i.valgrind_log ]
	then
		cat $i.valgrind_log | grep "ERROR SUMMARY: 0" > /dev/null
		if [ $? -eq 0 ]
		then
			printf "Leak: ${GREEN}pass${NC}\n"
		else
			printf "Leak: ${RED}fail${NC}\n"
			cat $i.valgrind_log
			RET=1
		fi
	else
		printf "Leak: ${RED}couldn't get valgrind file${NC}\n"
		RET=1
	fi
	rm $i.valgrind_log
	run_posttest $i
done


echo "Running coverage:"
make -f MakefileTest MODE="c" clean
make -f MakefileTest MODE="c"

./$1_cov -k a b 1>/dev/null 2>/dev/null
./$1_cov -k 1>/dev/null 2>/dev/null
./$1_cov -i a b 1>/dev/null 2>/dev/null

for i in $2/{*.in,test*.sh}
do
	run_pretest $i
	if [ "${i##*.}" = "in" ]
	then
		./$1_cov -i $i ${i%.*}.out 1>/dev/null 2>/dev/null
		./$1_cov -e $i ${i%.*}.out 1>/dev/null 2>/dev/null
		rm ${i%.*}.out
	else
		./$i $(realpath $1_cov) 1>/dev/null 2>/dev/null
	fi
	run_posttest $i
done
gcov RLEList.c AsciiArt.c main.c
rm $1 $1_cov
exit $RET
