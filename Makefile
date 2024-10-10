PROGNAME=debuglog
OBJ=main.o debug_log.o

.PHONY: all clean

$(PROGNAME): $(OBJ)
	gcc -o $@ $^

%.E: %.c
	gcc -E -C $< > $@

.c.o:
	gcc -c $<

clean:
	@rm -rf *.o *.E *~ *.bak *.deps *.log $(PROGNAME)

