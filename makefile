CC=g++
#CFLAGS=-O3
CFLAGS=-ggdb -O0


all: Hanoi_towers Naive_string_matching Square_root Bit_streaming

Hanoi_towers: Hanoi_towers.o 
	$(CC) $(CFLAGS) -o Hanoi_towers Hanoi_towers.o

Hanoi_towers.o: Hanoi_towers.cpp 
	$(CC) $(CFLAGS) -c Hanoi_towers.cpp	

Naive_string_matching: Naive_string_matching.o 
	$(CC) $(CFLAGS) -o Naive_string_matching Naive_string_matching.o

Naive_string_matching.o: Naive_string_matching.cpp 
	$(CC) $(CFLAGS) -c Naive_string_matching.cpp	

Square_root: Square_root.o 
	$(CC) $(CFLAGS) -o Square_root Square_root.o

Square_root.o: Square_root.cpp 
	$(CC) $(CFLAGS) -c Square_root.cpp	

Bit_streaming: Bit_streaming.o 
	$(CC) $(CFLAGS) -o Bit_streaming Bit_streaming.o

Bit_streaming.o: Bit_streaming.cpp 
	$(CC) $(CFLAGS) -c Bit_streaming.cpp	

clean:
	rm *.o
