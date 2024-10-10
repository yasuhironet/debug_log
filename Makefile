PROGNAME=debuglog
OBJ=main.o debug_log.o

.PHONY: all clean

$(PROGNAME): $(OBJ)
	gcc -o $@ $^

.c.o:
	gcc -c $<

clean:
	@rm -rf *.o *~ *.bak *.deps *.log $(PROGNAME)

