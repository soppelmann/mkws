CC = cc
LD = ${CC}

PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

CFLAGS = -Wall -Wextra -pedantic

BIN =         \
	lat   \
	lmt   \
	lct
MAN =         \
	lat.1 \
	lmt.1 \
	lct.1 \

OBJ = $(BIN:=.o)
SRC = $(BIN:=.c)
MAN = $(BIN:=.1)

all: $(BIN)

$(BIN): $(@:=.o)

$(OBJ):

.o:
	$(LD) $< $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CPPFLAGS) $< $(CFLAGS) -c -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	mkdir -p $(DESTDIR)$(MANPREFIX)/man5
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin
	cp -f $(MAN) $(DESTDIR)$(MANPREFIX)/man1
	cp -f lts.5 $(DESTDIR)$(MANPREFIX)/man5
	cd $(DESTDIR)$(PREFIX)/bin && chmod 755 $(BIN)
	cd $(DESTDIR)$(MANPREFIX)/man1 && chmod 644 $(MAN)
	cd $(DESTDIR)$(MANPREFIX)/man5 && chmod 644 lts.5

uninstall:
	cd $(DESTDIR)$(PREFIX)/bin && rm -f $(BIN)
	cd $(DESTDIR)$(MANPREFIX)/man1 && rm -f $(MAN)

clean:
	rm -f $(BIN) $(OBJ)
