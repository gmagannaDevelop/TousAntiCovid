# EXPLAINED MAKEFILE

# eval once :=
# eval each time =
# $@ : name of target
# $< name of first prerequisite
# $^ name of all prerequisites

### VARIABLES :
CC := gcc

STDFLAGS := -Wall -Wstrict-prototypes -ansi -pedantic -lm 
OPTIM := -O2 
GSLFLAGS := $(shell gsl-config --cflags)
SDLFLAGS := $(shell sdl-config --cflags)
CFLAGS := $(STDFLAGS) $(OPTIM) $(GSLFLAGS) $(SDLFLAGS)

GSLLIB := $(shell gsl-config --libs)  
SDLLIB := $(shell sdl-config --libs)
LINK := $(GSLLIB) $(SDLLIB)  

COMMONSRC := libraries.h constants.h
TARGET := ../bin/ahora_si_perros
OBJS := parsing.o SDL_ppm_graphics.o\
	simple_random.o visualization.o
DEPS := $(patsubst %.o,%.d,$(OBJS))
-include = $(DEPS)
DEPFLAGS = -MMD -MF $(@:.o=.d)

### END VARIABLE DECLARATION ###

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CC) -o $@ $^ $(CFLAGS) $(LINK)

%.o: %.c
	$(CC) -c $(CFLAGS) $(DEPFLAGS) -o $@ $< 

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)
