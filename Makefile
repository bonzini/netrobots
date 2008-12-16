all: all-logic all-graphics all-networking all-robots

## ------------------------------------
## ognuno scrive solo nella sua sezione
## ------------------------------------

## logic

all-logic:

## graphics

all-graphics:

## networking

NET_SRC = server/net_main.o server/net_utils.o server/net_commands.o

all-networking:	 $(NET_SRC)
	$(CC) -o server/netserver $(NET_SRC)
server/net_main.o: server/net_main.c server/net_utils.h
server/net_commands.o: server/net_commands.c server/net_commands.h
server/net_utils.o: server/net_utils.c server/net_utils.h

clean-networking:
	rm -rf server/net_*.o server/netserver

clean-obj-networking:
	rm -rf server/net_*.o

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
