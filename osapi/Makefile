flags=-O2 -Wall -std=c2x -fPIC
soflags=-ldl -shared  -fPIC 
ldflags=-dynamiclib

.PHONY: clean

all: clean libosapi.dylib


libosapi.dylib: osapi.o disk.o
	cc $(ldflags) -o $@ $^

osapi.so: osapi.o disk.o
	cc $(flags) $(soflags) $^ -o $@

osapi.o : osapi.c osapi.h os.h
	cc $(flags) -c $<

disk.o: disk.c disk.h
	cc $(flags) -c $<


clean:
	rm -f *.so *.o *.dylib 