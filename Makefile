# Written by Aaron Barge
CC = g++
CFLAGS = -O3 -o run
PERF_FLAG = -D PERFORMANCE_TEST
N = 100
SEED = 0


benchmark: tests/benchmark.cpp src/uInt.hpp
	$(CC) $(CFLAGS) $(PERF_FLAG) tests/benchmark.cpp
	./run $(N) > data/benchmark.txt

random_unit_tests: tests/unit_tests.cpp src/uInt.hpp
	$(CC) $(CFLAGS) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) > data/unit_tests.txt

set_unit_tests: tests/unit_tests.cpp src/uInt.hpp
	$(CC) $(CFLAGS) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) $(SEED) > data/unit_tests.txt

tests : benchmark random_unit_tests

clean:
	rm *.out run
