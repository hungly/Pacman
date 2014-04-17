CC = clang
CFLAGS = -Wall -g -pg

SOURCES = core.c level_editor.c score.c pacman.c movement.c
OBJS = $(patsubst %.c,%.o,$(SOURCES))
HEADERS = core.h pacghost.h level_editor.h score.h movement.h

pacman: $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o pacman $(OBJS) -lncurses
	
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm *.o 
	
.PHONY: cleanall
cleanall:
	rm *.o *~ pacman