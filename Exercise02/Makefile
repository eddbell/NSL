CC = g++
CFLAGS = -Wall -O3 --std=c++11 -DARWA_DONT_USE_WRAPPER -lopenblas -llapack
main.exe : main.o block.o funzioni.o random.o
	$(CC) random.o block.o funzioni.o main.o -o main.exe
main.o : main.cpp main.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)
block.o : ../Libs/block/block.cpp ../Libs/block/block.h
	$(CC) -c ../Libs/block/block.cpp -o block.o $(CFLAGS)
random.o : ../Libs/random/random.cpp ../Libs/random/random.h
	$(CC) -c ../Libs/random/random.cpp -o random.o $(CFLAGS)
funzioni.o : funzioni.cpp funzioni.h
	$(CC) -c funzioni.cpp -o funzioni.o $(CFLAGS)
clean :
	rm *.o main.exe
