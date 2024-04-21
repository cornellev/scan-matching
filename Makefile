# Copyright (C) 2023 Ethan Uppal. All rights reserved.

SRCDIR		:= src
INCLUDEDIR	:= src

CC			:= $(shell which g++ || which clang)
PY			:= $(shell which python3 || which python)
CFLAGS		:= -std=c++17 -pedantic -Wall -Wextra -I $(INCLUDEDIR)
CDEBUG		:= -g
CRELEASE	:= -O3 -DRELEASE_BUILD #-fno-fast-math
TARGET		:= main

# follow instructions in README to install in /usr/local
LDFLAGS		:= $(shell sdl2-config --libs) \
			   /usr/local/lib/libcmdapp.a \
			   /usr/local/lib/libsdlwrapper.a \
			   /usr/local/lib/libconfig.a
CFLAGS		+= $(shell sdl2-config --cflags) \
			   -I/usr/local/include \
			   -I/usr/local/include/sdlwrapper \
			   -I/usr/local/include/eigen3

CFLAGS 		+= $(CRELEASE)
# CFLAGS 		+= $(CDEBUG)

SRC			:= $(shell find $(SRCDIR) -name "*.cpp" -type f -not -path "$(SRCDIR)/sdl-wrapper/*" -not -path "$(SRCDIR)/icp/old/*")
OBJ			:= $(SRC:.cpp=.o)
DEPS 		:= $(OBJS:.o=.d) 

-include $(DEPS)

# Config parameters
N		:= 1
METHOD	:= vanilla

$(TARGET): main.cpp $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	@make readme

# .PHONY: run
# run: $(TARGET)
# 	./$(TARGET)

.PHONY: test
test: test.cpp $(OBJ)
	@$(CC) $(CFLAGS) $(LDFLAGS) -DTEST -o _temp $^
	@echo 'Running tests...'
	@./_temp
	@rm -f ./_temp

.PHONY: view
view: $(TARGET)
	./$(TARGET) -S ex_data/scan$(N)/first.conf -D ex_data/scan$(N)/second.conf --method $(METHOD) --gui

.PHONY: bench
bench: $(TARGET) 
	./$(TARGET) -S ex_data/scan$(N)/first.conf -D ex_data/scan$(N)/second.conf --method $(METHOD) --bench

%.o: %.cpp
	@echo 'Compiling $@'
	$(CC) $(CFLAGS) -MMD -MP $< -c -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ) $(TARGET) $(TARGET) $(DEPS) $(shell find . -name "*.dSYM") $(shell find . -name "*.d") docs

.PHONY: docs 
docs:
	doxygen

.PHONY: cloc
cloc:
	cloc . --include-lang=c++,"c/c++ header" --by-file

.PHONY: readme
readme:
	cd script; $(PY) readme.py `curl https://api.github.com/repos/cornellev/scan-matching/releases/latest | jq .name`
