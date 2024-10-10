PROGNAME=debuglog
OBJ=main.o debug_log.o

.PHONY: all clean

$(PROGNAME): $(OBJ)
	gcc -g -O0 -rdynamic -o $@ $^

%.E: %.c
	gcc -E -C $< > $@

.c.o:
	gcc -g -O0 -c $<

clean:
	@rm -rf *.o *.E *~ *.bak *.deps $(PROGNAME)

