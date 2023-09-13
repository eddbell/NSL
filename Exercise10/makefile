CC = g++
CFLAGS = -Wall -O3 --std=c++11 -DARWA_DONT_USE_WRAPPER -lopenblas -llapack
MPI = /usr/bin/mpicxx

go.exe : main.o random.o block.o GA_TSP.o
	$(MPI) $^ -o $@
main.o : main.cpp main.h
	$(MPI) -c $< -o $@ $(CFLAGS)
./%.o: ./%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

random.o : ../Libs/random/random.cpp ../Libs/random/random.h
	$(CC) -c $< -o $@ $(CFLAGS)
block.o : ../Libs/block/block.cpp ../Libs/block/block.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean :
	rm *.o *.exe
