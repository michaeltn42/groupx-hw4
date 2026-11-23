CC=g++
DEPS = bowling_frame.h
TARGET = bowling

all: $(TARGET)

$(TARGET): bowling_frame.o main.o
	$(CC) -std=c++11 bowling_frame.o main.o -o $(TARGET)

bowling_frame.o: bowling_frame.cpp $(DEPS)
	$(CC) -std=c++11 -c bowling_frame.cpp

main.o: main.cpp $(DEPS)
	$(CC) -std=c++11 -c main.cpp

clean: 
	rm -f *.o *.out $(TARGET)
