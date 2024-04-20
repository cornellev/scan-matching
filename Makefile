# Copyright (C) 2023 Ethan Uppal. All rights reserved.

SRCDIR		:= src
INCLUDEDIR	:= src

CC			:= $(shell which g++ || which clang)
CFLAGS		:= -std=c++17 -pedantic -Wall -Wextra -I $(INCLUDEDIR)
CDEBUG		:= -g
CRELEASE	:= -O2 -DRELEASE_BUILD
TARGET		:= main

# follow instructions in README to install in /usr/local
LDFLAGS		:= $(shell sdl2-config --libs) \
			   /usr/local/lib/libcmdapp.a \
			   /usr/local/lib/libsdlwrapper.a \
			   /usr/local/lib/libconfig.a
CFLAGS		+= $(shell sdl2-config --cflags) \
			   -I/usr/local/include \
			   -I/usr/local/include/sdlwrapper \
			   -I /usr/local/include/eigen3

# CFLAGS 		+= $(CRELEASE)
CFLAGS 		+= $(CDEBUG)

SRC			:= main.cpp \
			   $(shell find $(SRCDIR) -name "*.cpp" -type f -not -path "$(SRCDIR)/sdl-wrapper/*" -not -path "$(SRCDIR)/icp/old/*")
OBJ			:= $(SRC:.cpp=.o)
DEPS 		:= $(OBJS:.o=.d) 

-include $(DEPS)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: bench
bench: $(TARGET) 
	./$(TARGET) --bench point_to_point

.PHONY: gui 
gui: $(TARGET) 
	./$(TARGET) --gui	

.PHONY: gui_debug
gui_debug: $(TARGET)
	echo "run" | lldb $(TARGET) -- --gui

%.o: %.cpp
	@echo 'Compiling $@'
	$(CC) $(CFLAGS) -MMD -MP $< -c -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ) $(TARGET) $(TARGET) $(DEPS) $(shell find . -name "*.dSYM") $(shell find . -name "*.d") docs

.PHONY: docs 
docs:
	doxygen
