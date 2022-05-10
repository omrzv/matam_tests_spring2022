shopt -s nullglob

if [ ! -d FileTests ]
then
	printf "Can't find FileTests, expacts the following dir:"
	printf "	./FileTests/..."
	exit 1
fi

make -f FileTests/makeFileTests clean
rmdir FileTests/oFiles

for i in FileTests/InOutFiles/*.result
do
	rm $i
done
printf "Cleaned Succsessfully"
echo 