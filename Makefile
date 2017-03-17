IDIR := src
SRCDIR := src
ODIR := obj
LDIR :=

AR=ar
C=gcc
CDEFINES = 
CLIBS = -L. -lcmpdism
CFLAGS =
CINCLUDES =
CFLAGS=-O2 -Wall $(CDEFINES) $(CINCLUDES) -I$(IDIR) $(CLIBS)

SRC := $(wildcard $(SRCDIR)/*/*.c) $(wildcard $(SRCDIR)/*/*/*.c)
OBJ := $(patsubst $(SRCDIR)/%,$(ODIR)/%,$(patsubst %.c,%.o,$(SRC)))
DEP := $(patsubst %.o,%.d,$(OBJ))
LIB := libcmpdism.a

all: $(OBJ) libcmpdism cmpdism
	
cmpdism: libcmpdism $(OBJ)
	$(C) src/main.c $(CLIBS) -o cmpdism $(CFLAGS)
	
libcmpdism: $(OBJ)
	$(AR) rcs $(LIB) $^

-include $(DEP)

$(ODIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(C) -c -MMD -MP -MF $(@:.o=.d) -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(LIB)
	rm -rf $(ODIR)
