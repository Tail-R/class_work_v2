OBJS = src/main.cpp \
	src/app/app.cpp \
	src/sprites/sprite.cpp \
	src/sprites/player.cpp \
	src/sprites/standalone.cpp \
	src/sprites/enemy.cpp \
	src/sprites/enemies.cpp \
	src/sprites/bullet.cpp \
	src/factory/texture_factory.cpp \
	src/events/keyevent.cpp \
	src/stages/stage.cpp \
	src/stages/stage1.cpp \
	src/socket/socket.cpp

C = g++

C_FLAGS = -std=c++20 -Wall -Wextra

LINKER_FLAGS = -l SDL2 -l SDL2_image -l boost_json

TARGET = build/main

all : $(OBJS)
	$(C) $(C_FLAGS) $(OBJS) $(LINKER_FLAGS) -o $(TARGET)

clean :
	rm ./$(TARGET)
