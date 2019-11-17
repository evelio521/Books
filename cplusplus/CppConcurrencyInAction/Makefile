CFLAGS = -g -O2 -Wall -Wno-unused-function -Wpessimizing-move

INCLUDES = -I.

LDINCLUDE =

LIBS = -pthread -lboost_thread-mt -lboost_system-mt

MARCOS = -DDEBUG -D__LINUX__ -DSTOPWATCH_ON

CC = gcc
XX = g++

BASE_SRC = $(wildcard ./*.cc)
BASE_OBJ := $(BASE_SRC:.cc=.o)

all: concurrency

concurrency : $(BASE_OBJ)
	$(XX) -o $@ $^  $(LIBS) $(INCLUDES) $(MARCOS) -lstdc++
	@echo ---------------------SUCCESS-------------------------

%.o: %.cc
	$(XX) -fPIC -std=c++0x $(CFLAGS)  $(INCLUDES) $(MARCOS) -c -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(MARCOS) -c -o $@ $^

.PHONY: clean

clean:
	rm -f $(BASE_OBJ)
	rm -f concurrency
