### START VARIABLE DECLARATION ###

 CC := gcc
 TARGET := ../bin/singly-linked-list

# start compilation flags #
  STDFLAGS := -Wall -Wstrict-prototypes -ansi -pedantic -lm 
  OPTIM := -O2 
  GSLFLAGS := $(shell gsl-config --cflags)
  CFLAGS := $(STDFLAGS) $(OPTIM) $(GSLFLAGS) 
# end compilation flags #

# start linking flags #
  GSLLIB := $(shell gsl-config --libs)  
  LINK := $(GSLLIB)  
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

