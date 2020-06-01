# Libco Coroutine Library
# https://github.com/byuu/higan/tree/master/libco

OBJECTS = libco.o

CFLAGS = -fPIC

.PHONY: all 

all: $(OBJECTS)

%.o : %.cpp
	$(MPICXX) -c $(CFLAGS) $<

.PHONY: clean
clean:
	$(RM) $(OBJECTS) *.o 
