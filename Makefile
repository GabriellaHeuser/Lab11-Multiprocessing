CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=-ljpeg
SOURCES= mandel.c jpegrw.c mandelmovie.c
OBJECTS=$(SOURCES:.c=.o)
MANDEL_OBJECTS= mandel.o jpegrw.o
MOVIE_OBJECTS= mandelmovie.o
EXECUTABLES=mandel mandelmovie

all: $(SOURCES) $(EXECUTABLES)

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

mandel: $(MANDEL_OBJECTS)
	$(CC) $(MANDEL_OBJECTS) $(LDFLAGS) -o $@

mandelmovie: $(MOVIE_OBJECTS)
	$(CC) $(MOVIE_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d

clean:
	rm -rf $(OBJECTS) $(EXECUTABLES) *.d