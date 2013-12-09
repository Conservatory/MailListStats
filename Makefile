# Makefile for MailListStat (MLS)
# (C) 2001-2003 Marek -Marki- Podmaka <marki@nexin.sk>

# install won't work under Windows

# -m386 (486,pentium,pentiumpro)
#OPTIMIZE=-O7 -mpentiumpro
OPTIMIZE=-O3

# debug symbols will be stripped anyway during 'make install'
DEBUG=-g

#### DON'T CHANGE ANYTHING BELOW ####
DESTDIR=/usr/local
CFLAGS=-Wall $(OPTIMIZE) $(DEBUG)
LIBS=-lm
CC=gcc

OBJS=mls.o mls_mime.o mls_text.o mls_list.o mls_stat.o

mls: $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) -o mls

mls.o: mls.c mls.h Makefile
	$(CC) $(CFLAGS) -c mls.c -o mls.o

mls_mime.o: mls_mime.c mls_mime.h Makefile
	$(CC) $(CFLAGS) -c mls_mime.c -o mls_mime.o

mls_list.o: mls_list.c mls_list.h Makefile
	$(CC) $(CFLAGS) -c mls_list.c -o mls_list.o

mls_stat.o: mls_stat.c mls_stat.h Makefile
	$(CC) $(CFLAGS) -c mls_stat.c -o mls_stat.o

mls_text.o: mls_text.c mls_text.h mls_lang.h Makefile
	$(CC) $(CFLAGS) -c mls_text.c -o mls_text.o

install: mls
	install -m 755 -g root -o root -s mls $(DESTDIR)/bin
	install -m 644 -g root -o root mls.1 $(DESTDIR)/man/man1
	gzip -9f $(DESTDIR)/man/man1/mls.1
	@echo "**************************************************************"
	@echo "*** To use HTML feature or PHP wrapper, copy contents of   ***"
	@echo "*** 'html' subdirectory to location accessible by your     ***"
	@echo "*** webserver. See README for more info!                   ***"
	@echo "**************************************************************"
	@echo "*** See also 'examples' subdir for some hints...           ***"
	@echo "******************** MLS install complete ********************"

clean:
	rm -f $(wildcard *.o) mls

uninstall:
	rm -fv $(DESTDIR)/bin/mls
	rm -fv $(DESTDIR)/man/man1/mls.1.gz
