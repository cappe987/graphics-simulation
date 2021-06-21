
all: build


build:
	gcc main.c -lpthread -o main -lX11 -lXext -lXdamage -lXfixes -lXtst -lm
