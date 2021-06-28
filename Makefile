
all: build


files = circle.c fps.c physics.c main.c

dirs = -I /usr/include/freetype2/

build:
	gcc $(files) $(dirs) -lpthread -o main -lX11 -lXext -lXdamage -lXfixes -lXtst -lm -lXft
