# Written by Aaron Barge
CC = g++
CFLAGS = -O3 -o run
N = 100

benchmark: benchmark.cpp
	$(CC) $(CFLAGS) benchmark.cpp
	./run $(N) > benchmark.txt
