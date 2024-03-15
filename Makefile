# the compiler: gcc/clang for C program, g++/clang++ for C++
CC = clang++
 
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -Wall
 
# The build target 
TARGET = main
EXEC_NAME = encoder

all: $(TARGET)
 
$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(EXEC_NAME) $(TARGET).cpp
 
clean:
	$(RM) $(TARGET)