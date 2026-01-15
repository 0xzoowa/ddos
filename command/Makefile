flags=-O2 -Wall -std=c2x -I../osapi
ldflags=-L../osapi -losapi -Wl,-rpath,@loader_path/../osapi -Wl,-install_name,@rpath/libosapi.dylib

.PHONY: clean

all: clean command.com  

command.com: command.o libosapi.dylib
	cc command.o -o command.com $(ldflags)

../osapi/osapi.so: ../osapi/osapi.c  ../osapi/osapi.h  ../osapi/disk.c  ../osapi/disk.o 
	$(MAKE) -C ../osapi

libosapi.dylib: ../osapi/osapi.c  ../osapi/osapi.h  ../osapi/disk.c  ../osapi/disk.o 
	$(MAKE) -C ../osapi

command.pp: command.o ../osapi/osapi.so 
	cc $(flags) $^ -o $@ $(ldflags)

command.o : command.c command.h
	cc $(flags) -c $<

clean:
	rm -f *.o command.com ../osapi/*.so ../osapi/*.o 