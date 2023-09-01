if [[ ! -d data ]]
then
	mkdir -p -v data
else
	rm -rf -v data/output_*
fi

make

./main.exe
