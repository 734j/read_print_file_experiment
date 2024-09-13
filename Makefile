CC=clang++
CFLAGS=-Wall -s -static -O3 -Wextra -DNDEBUG -fomit-frame-pointer -flto -std=c++20

all:
	$(CC) $(CFLAGS) read_print_file.cpp -o rpf

clean:
	rm -f rpf
