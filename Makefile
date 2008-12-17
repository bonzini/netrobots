all: all-logic all-graphics all-networking all-robots

## ------------------------------------
## ognuno scrive solo nella sua sezione
## ------------------------------------

## logic

all-logic:

## graphics
CFLAGS = -g `pkg-config cairo --cflags` `pkg-config sdl --cflags`
LDFLAGS = -g `pkg-config cairo --libs` `pkg-config sdl --libs` -framework OpenGL
MAIN_SRC = server/main.o server/anim.o server/drawing.o server/field.o server/robotserver.o 

all-graphics: main
main: $(MAIN_SRC)
	$(CC) -o main $(MAIN_SRC) $(LDFLAGS)
server/anim.o: server/anim.c server/drawing.h
server/field.o: server/field.c server/drawing.h server/field.h server/anim.h
server/drawing.o: server/drawing.c server/drawing.h
server/main.o: server/main.c server/drawing.h server/anim.h server/field.h


## networking

NET_SRC = server/net_main.o server/net_utils.o server/net_commands.o server/net_core.o server/robotserver.o

all-networking:	 $(NET_SRC)
	$(CC) -o server/netserver $(NET_SRC)
server/robotserver.o: server/robotserver.h
server/net_main.o: server/net_main.c server/net_utils.h server/robotserver.h
server/net_core.o: server/net_core.c server/robotserver.h server/net_utils.h server/net_defines.h
server/net_commands.o: server/net_commands.c server/net_defines.h server/robotserver.h
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
