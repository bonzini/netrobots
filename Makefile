all: all-logic all-graphics all-networking all-robots

## ------------------------------------
## ognuno scrive solo nella sua sezione
## ------------------------------------

## logic

all-logic:

## graphics
CFLAGS = -g `pkg-config cairo --cflags` `pkg-config sdl --cflags` -O2
LDFLAGS = -g `pkg-config cairo --libs` `pkg-config sdl --libs` -framework OpenGL

all-graphics: main: anim.o field.o drawing.o main.o

anim.o: anim.c drawing.h
field.o: field.c drawing.h field.h anim.h
drawing.o: drawing.c drawing.h
main.o: main.c drawing.h


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
