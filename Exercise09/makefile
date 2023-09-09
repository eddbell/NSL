CC = g++
CFLAGS = -Wall -O3 --std=c++11 -DARWA_DONT_USE_WRAPPER -lopenblas -llapack
go.exe : main.o random.o block.o
	$(CC) block.o random.o main.o -o go.exe
main.o : main.cpp main.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : ../Libs/random/random.cpp ../Libs/random/random.h
	$(CC) -c ../Libs/random/random.cpp -o random.o $(CFLAGS)
block.o : ../Libs/block/block.cpp ../Libs/block/block.h
	$(CC) -c ../Libs/block/block.cpp -o block.o $(CFLAGS)
clean :
	rm *.o
