OBJ=main.o debug_log.o

.PHONY: all clean

debuglog: $(OBJ)
	gcc -o $@ $^

.c.o:
	gcc -c $<

clean:
	@rm -rf *.o *~ *.bak *.deps

