CC = g++
CFLAGS = -Wall -O3 --std=c++11 -DARWA_DONT_USE_WRAPPER -lopenblas -llapack
main.exe : main.o random.o
	$(CC) random.o main.o -o main.exe
main.o : main.cpp main.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : ../Libs/random/random.cpp ../Libs/random/random.h
	$(CC) -c ../Libs/random/random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o
