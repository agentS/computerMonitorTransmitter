CC=gcc
# enhance a program until there are no error messages any more
CFLAGS = -g -std=c11 -Wpedantic -Wall -Wconversion -Werror=vla -Wextra -O0
HEADERS = 
LIBS = -lserialport
LDFLAGS = -Wpedantic -Wall
PROGNAME = computerMonitorTransmitter

all: $(PROGNAME)

$(PROGNAME): main.o stats.o
	$(CC) $(LDFLAGS) -o $(PROGNAME) main.o stats.o $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c $(HEADERS)

stats.o: stats.c
	$(CC) $(CFLAGS) -c stats.c $(HEADERS)

clean:
	rm -rf $(PROGNAME) *.o
