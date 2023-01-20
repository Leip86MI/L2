CC=gcc
CFLAGS=-std=c89 -pedantic -Wall -Werror -g `pkg-config libxml-2.0 --cflags`

bm.out :bm.c base.o facility.o
	${CC} ${CFLAGS} -o bm.out bm.c base.o facility.o `pkg-config libxml-2.0 --libs-only-L` `pkg-config libxml-2.0 --libs-only-l`

base.o:base.c
	${CC} ${CFLAGS} -c -o base.o base.c

facility.o:facility.c
	${CC} ${CFLAGS} -c -o facility.o facility.c

