CC = clang
CFLAGS = -Wall -g

SOURCES = core.c level_editor.c score.c pacman.c movement.c lyquochung.c dijkstra.c nguyenvinhlinh.c lyquanliem.c astar.c btree.c bstree.c queue.c doanhaidang.c
OBJS = $(patsubst %.c,%.o,$(SOURCES))
HEADERS = core.h pacghost.h level_editor.h score.h movement.h lyquochung.h dijkstra.h nguyenvinhlinh.h lyquanliem.h astar.h btree.h bstree.h queue.h cell.h doanhaidang.h

pacman: $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o pacman $(OBJS) -lncurses -lrt -lm
	
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm *.o 
	
.PHONY: cleanall
cleanall:
	rm *.o *~ pacman