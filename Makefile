# Copyright (C) 2023 Ethan Uppal. All rights reserved.

SRCDIR		:= ./src
INCLUDEDIR	:= ./src
LIBDIR		:= $(SRCDIR)/sdl-wrapper
LIB			:= $(LIBDIR)/libsdlwrapper.so

CC			:= $(shell which g++-13 || \
					   which g++ || which clang)
CFLAGS		:= -std=c++17 -pedantic -Wall -Wextra -I $(INCLUDEDIR)
CDEBUG		:= -g
CRELEASE	:= -O2 -DRELEASE_BUILD
TARGET		:= main

# CFLAGS 		+= $(CRELEASE)
CFLAGS 		+= $(CDEBUG)

# use SDL
CFLAGS		+= $(shell sdl2-config --cflags --libs)

# use sdlwrapper
CFLAGS		+= -L $(LIBDIR) -lsdlwrapper -I $(LIBDIR)/src

SRC			:= main.cpp $(shell find $(SRCDIR) -name "*.cpp" -type f -not -path "$(SRCDIR)/sdl-wrapper/*")
OBJ			:= $(SRC:.cpp=.o)
DEPS 		:= $(OBJS:.o=.d) 

.PHONY: run 
run: driver 
	DYLD_LIBRARY_PATH=$$DYLD_LIBRARY_PATH:$$PWD/$(LIBDIR) ./$(TARGET)

.PHONY: driver
driver: $(LIB)
	make $(TARGET)

# inv: $(LIB) is built
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(LIB):
	cd $(LIBDIR); make libsdlwrapper.so

%.o: %.cpp $(LIB)
	@echo 'Compiling $@'
	$(CC) $(CFLAGS) -MMD -MP $< -c -o $@

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(OBJ) $(TARGET) $(DEPS) $(shell find . -name "*.dSYM") $(shell find . -name "*.d")
