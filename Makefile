CC = gcc
CFLAGS = -g -c

SOURCES = main.cpp MemoryManager.cpp
OBJS = $(SOURCES:.cpp=.o)
EXECUTABLE = final.exe

$(EXECUTABLE): $(OBJS) 
	$(CC) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o