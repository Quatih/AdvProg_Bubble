OBJS = main.cpp GameEngine/GameEngine.cpp 

CC = g++

COMPILER_FLAGS = -std=c++17 -std=gnu++17 -Wall

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

OBJ_NAME = game

all : $(OBJS) 
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
