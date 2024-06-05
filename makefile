OBJS = src/main.o \
	src/app/app.o \
	src/sprites/sprite.o \
	src/sprites/player.o \
	src/sprites/standalone.o \
	src/sprites/enemy.o \
	src/sprites/enemies.o \
	src/sprites/finals/meiling.o \
	src/sprites/bullet.o \
	src/factory/texture_factory.o \
	src/events/keyevent.o \
	src/stages/stage.o \
	src/stages/stage1.o \
	src/socket/socket.o

CC = g++

CXXFLAGS = -std=c++20 -Wall -Wextra # -Werror

LFLAGS = -l SDL2 \
	-l SDL2_image \
	-l SDL2_ttf \
	-l boost_json

TARGET = build/main

all : $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) $(LFLAGS) -o $(TARGET)
