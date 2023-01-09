
INC_DIR			= include
SRC_DIR			= src
MAIN_DIR		= main
SOURCES			= $(shell find $(SRC_DIR)/ -name '*.cc')
MAIN_SRC		= $(shell find $(MAIN_DIR)/ -name '*.cc')
OBJECTS			= $(SOURCES:.cc=.o)
MAIN_OBJ		= $(MAIN_SRC:.cc=.o)
MAIN_TARGET	= de
CC					= g++
CFLAGS			= -std=c++11 -g -Wall -Wextra -fopenmp


all: $(MAIN_TARGET)

$(MAIN_TARGET): $(OBJECTS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -I${INC_DIR} -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJECTS) $(MAIN_OBJ) $(MAIN_TARGET)
