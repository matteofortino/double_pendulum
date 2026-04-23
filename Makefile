# Compiler
CC = clang

# Target
TARGET = main

# Source files
SRC = main.c

# Raylib paths (from Homebrew)
RAYLIB_PATH = $(shell brew --prefix raylib)

# Flags
CFLAGS = -I$(RAYLIB_PATH)/include
LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib \
		  -framework OpenGL \
		  -framework Cocoa \
		  -framework IOKit \
		  -framework CoreAudio \
		  -framework CoreVideo

# Build rule
all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Run the program
run: all
	./$(TARGET)

# Clean build files
clean:
	rm -f $(TARGET)
