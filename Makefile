# Written by Aaron Barge
CC = g++
CFLAGS = -O3 -o run
BETA_FLAG = -D BETA
PERF_FLAG = -D PERFORMANCE_TEST
N = 100
F = 100
SEED = 0

baseline: src/uInt.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) > data/baseline_unit.txt

baseline_set_seed: src/uInt.hpp tests/unit_tests.cpp
	src/uInt.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) $(SEED) > data/baseline_unit.txt

beta: src/uInt-beta.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) $(BETA) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) > data/beta_unit.txt

beta_set_seed: src/uInt-beta.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) $(BETA) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) $(SEED) > data/beta_unit.txt

baseline_benchmark: tests/benchmark.cpp src/uInt.hpp
	$(CC) $(CFLAGS) $(PERF_FLAG) tests/benchmark.cpp
	./run $(N) $(F) > data/baseline_benchmark.txt

beta_benchmark: tests/benchmark.cpp src/uInt-beta.hpp
	$(CC) $(CFLAGS) $(BETA) $(PERF_FLAG) tests/benchmark.cpp
	./run $(N) $(F) > data/beta_benchmark.txt

clean:
	rm *.out run
