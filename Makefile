IDIR=include
ODIR=obj
SDIR=src
EXE=lisp
CC=gcc
CFLAGS=-std=c99 -I$(IDIR)
LIBS=-lm

ENSUREBUILDDIR = $(shell test -d $(ODIR) || mkdir $(ODIR))

CFILES = $(patsubst $(SDIR)/%.c,%.c, $(wildcard $(SDIR)/*.c))

HEADERS := $(wildcard $(IDIR)/*.h)

OBJS = $(patsubst %.c,$(ODIR)/%.o,$(CFILES)) 

all: $(ENSUREBUILDDIR) $(EXE) 

$(ODIR)/%.o : $(SDIR)/%.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJS)
	@echo 
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean test show

show:
	@echo $(CFILES)
	@echo $(HEADERS)
	@echo $(OBJS)
clean:
	rm -f $(OBJS) 
	rm -f $(EXE) 
