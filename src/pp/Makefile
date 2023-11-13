CC = cc
LD = ${CC}

PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

CFLAGS = -Wall -Wextra -pedantic

pp: pp.o
pp.o: pp.c

.c.o:
	$(CC) $(CPPFLAGS) $< $(CFLAGS) -c -o $@
.o:
	$(LD) $< $(LDFLAGS) -o $@

clean:
	rm -rf pp *.o

install: pp pp.1
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	cp pp $(DESTDIR)$(PREFIX)/bin/pp
	cp pp.1 $(DESTDIR)$(MANPREFIX)/man1/
	chmod 755 $(DESTDIR)$(PREFIX)/bin/pp

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/pp
	rm $(DESTDIR)$(MANPREFIX)/man1/pp.1
