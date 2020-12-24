### START VARIABLE DECLARATION ###

 CC := gcc
 TARGET := ../bin/viral-simulation

# start compilation flags #
  STDFLAGS := -Wall -Wstrict-prototypes -pedantic -lm 
  OPTIM := -O2 
  GSLFLAGS := $(shell gsl-config --cflags)
  SDLFLAGS := $(shell sdl-config --cflags)
  CFLAGS := $(STDFLAGS) $(OPTIM) $(GSLFLAGS) 
# end compilation flags #

# start linking flags #
  GSLLIB := $(shell gsl-config --libs)  
  SDLLIB := $(shell sdl-config --libs)
  LINK := $(GSLLIB) $(SDLLIB)  
# end linking flags #

# start dependency tracking #
  CS := $(shell ls *.c)
  OBJS := $(patsubst %.c,%.o,$(CS))
  DEPS := $(patsubst %.o,%.d,$(OBJS))
  -include = $(DEPS)
  DEPFLAGS = -MMD -MF $(@:.o=.d)
# end dependency tracking #

### END VARIABLE DECLARATION ###

# start build rules #

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CC) -o $@ $^ $(CFLAGS) $(LINK)

%.o: %.c
	$(CC) -c $(CFLAGS) $(DEPFLAGS) -o $@ $< 

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)
	
# end build rules #

# := eval once 
#  = eval each time 
# $@ name of target
# $< name of first prerequisite
# $^ name of all prerequisites

