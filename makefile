OBJS1 = list_node.o Buffer.o NodeIndex.o Graph.o ArrayList.o Stack.o Components.o main.o 
OBJS2 = list_node.o Buffer.o NodeIndex.o Graph.o ArrayList.o Stack.o Components.o testing.o
SOURCE = list_node.cpp Buffer.cpp NodeIndex.cpp Graph.cpp ArrayList.cpp Stack.cpp Components.cpp main.cpp testing.cpp
HEADER = list_node.h Buffer.h NodeIndex.h Graph.h ArrayList.h Components.h Stack.h
OUT1 = out
OUT2 = testing
CC = g++
FLAGS = -g -c

all: $(OUT1) $(OUT2)

$(OUT1): $(OBJS1)
	$(CC) -g $(OBJS1) -o $@

$(OUT2): $(OBJS2)
	$(CC) -g $(OBJS2) -o $@

list_node.o: list_node.cpp
	$(CC) $(FLAGS) list_node.cpp list_node.h

Buffer.o: Buffer.cpp Buffer.h
	$(CC) $(FLAGS) Buffer.cpp

NodeIndex.o: NodeIndex.cpp NodeIndex.h
	$(CC) $(FLAGS) NodeIndex.cpp

Graph.o: Graph.cpp Graph.h
	$(CC) $(FLAGS) Graph.cpp

ArrayList.o: ArrayList.cpp ArrayList.h
	$(CC) $(FLAGS) ArrayList.cpp

Stack.o: Stack.cpp Stack.h
	$(CC) $(FLAGS) Stack.cpp

Components.o: Components.cpp Components.h
	$(CC) $(FLAGS) Components.cpp

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

testing.o: testing.cpp
	$(CC) $(FLAGS) testing.cpp

clean :
	rm -f $(OBJS1) testing.o $(OUT1) $(OUT1)

count :
	wc $(SOURCE) $(HEADER)

