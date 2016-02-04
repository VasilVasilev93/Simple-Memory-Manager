CC = gcc
CFLAGS = -g -c

SOURCES = main.cpp
OBJS = $(SOURCES:.cpp=.o)
EXECUTABLE = final

$(EXECUTABLE): $(OBJS) 
	$(CC) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o