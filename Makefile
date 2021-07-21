# Written by Aaron Barge
CC = g++
CFLAGS = -O3 -o run
BASE_LINE_FLAG = -D BASE_LINE
PERF_FLAG = -D PERFORMANCE_TEST
N = 100
SEED = 0

baseline: src/uInt.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) $(BASE_LINE_FLAG) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) > data/baseline.txt

baseline_set_seed: src/uInt.hpp tests/unit_tests.cpp
	src/uInt.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) $(BASE_LINE_FLAG) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) $(SEED) > data/baseline.txt

beta: src/uInt-beta.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) > data/beta.txt

beta_set_seed: src/uInt-beta.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) $(PERF_FLAG) tests/unit_tests.cpp
	./run $(N) $(SEED) > data/beta.txt

clean:
	rm *.out run
