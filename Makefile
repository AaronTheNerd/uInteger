# Written by Aaron Barge
CC = g++
CFLAGS = -g -o run
N = 100
SEED = 0

baseline: src/uInt.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) -D BASE_LINE tests/unit_tests.cpp
	./run $(N) > data/baseline.txt

baseline_set_seed: src/uInt.hpp tests/unit_tests.cpp
	src/uInt.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) -D BASE_LINE tests/unit_tests.cpp
	./run $(N) $(SEED) > data/baseline.txt

beta: src/uInt-beta.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) tests/unit_tests.cpp
	./run $(N) > data/beta.txt

beta_set_seed: src/uInt-beta.hpp tests/unit_tests.cpp
	$(CC) $(CFLAGS) tests/unit_tests.cpp
	./run $(N) $(SEED) > data/beta.txt

clean:
	rm *.out run
