all: all-logic all-graphics all-networking all-robots

## ------------------------------------
## ognuno scrive solo nella sua sezione
## ------------------------------------

## logic

all-logic:

## graphics

all-graphics:

## networking

all-networking:

## robots

ROBOTS = counter rabbit rook sniper

all-robots: $(ROBOTS)

clients/robots.o: clients/robots.c
robots.a: clients/robots.o
	cd clients && ar cru ../robots.a robots.o

counter: clients/counter.c clients/robots.h robots.a
	$(CC) -o $@ $< robots.a

rabbit: clients/rabbit.c clients/robots.h robots.a
	$(CC) -o $@ $< robots.a

rook: clients/rook.c clients/robots.h robots.a
	$(CC) -o $@ $< robots.a

sniper: clients/sniper.c clients/robots.h robots.a
	$(CC) -o $@ $< robots.a
