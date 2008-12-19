all: all-server all-robots

## ------------------------------------
## ognuno scrive solo nella sua sezione
## ------------------------------------

## logic
all-logic:
server/testlogic.o: server/testlogic.c server/robotserver.h
server/robotserver.o: server/robotserver.c server/robotserver.h

## graphics
CFLAGS = -g `pkg-config cairo --cflags` `pkg-config sdl --cflags` -Icommon
LDFLAGS = -g `pkg-config cairo --libs` `pkg-config sdl --libs` -framework OpenGL
MAIN_SRC = server/main.o server/drawing.o server/field.o common/net_utils.o server/net_commands.o server/net_core.o server/robotserver.o

all-server: main
main: $(MAIN_SRC)
	$(CC) -o main $(MAIN_SRC) $(LDFLAGS)
server/field.o: server/field.c server/drawing.h server/field.h
server/drawing.o: server/drawing.c server/drawing.h
server/net_main.o: server/net_main.c common/net_utils.h server/robotserver.h
server/net_core.o: server/net_core.c server/robotserver.h common/net_utils.h server/net_defines.h server/field.h
server/net_commands.o: server/net_commands.c server/net_defines.h server/robotserver.h
common/net_utils.o: common/net_utils.c common/net_utils.h

## robots

ROBOTS = counter rabbit rook sniper spot
LIBROBOTS_OBJS = clients/robots.o common/net_utils.o

all-robots: $(ROBOTS)

clients/robots.o: clients/robots.c
robots.a: $(LIBROBOTS_OBJS)
	ar cru $@ $(LIBROBOTS_OBJS)

counter: clients/counter.c clients/robots.h robots.a
	$(CC) -o $@ $< robots.a

rabbit: clients/rabbit.c clients/robots.h robots.a
	$(CC) -o $@ $< robots.a

rook: clients/rook.c clients/robots.h robots.a
	$(CC) -o $@ $< robots.a

sniper: clients/sniper.c clients/robots.h robots.a
	$(CC) -o $@ $< robots.a

spot: clients/spot.c clients/robots.h robots.a
	$(CC) -o $@ $< robots.a
