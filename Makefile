# Copyright 2023 <Dumitrescu Toma-Ioan>

# variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm

# globbing
SRCS = $(wildcard *.c)
H = $(wildcard *.h)
OBJECTS=$(SRCS:%.c=%.o)
TARGETS=$(OBJS:%.o=%)

build:
	$(CC) -o image_editor $(SRCS) $(CFLAGS)

pack:
	zip -FSr 321CA_DumitrescuToma_Tema3.zip 'README' Makefile $(SRCS) $(H)

clean:
	rm -f $(TARGETS) $(OBJECTS)

.PHONY: pack clean
