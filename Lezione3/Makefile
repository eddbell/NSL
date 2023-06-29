CC = g++
CFLAGS = -Wall -O3 --std=c++11 -DARWA_DONT_USE_WRAPPER -lopenblas -llapack
main.exe : main.o block.o random.o
	$(CC) random.o block.o main.o -o main.exe
main.o : main.cpp main.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)
block.o : ../block/block.cpp ../block/block.h
	$(CC) -c ../block/block.cpp -o block.o $(CFLAGS)
random.o : ../random/random.cpp ../random/random.h
	$(CC) -c ../random/random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o main.exe
