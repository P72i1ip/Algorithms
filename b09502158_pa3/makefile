# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O2

all	: bin/cb
	@echo -n ""

# optimized version
bin/cb	: CycleBreaking.o main_opt.o
			$(CC) $(OPTFLAGS) CycleBreaking.o main_opt.o -o bin/cb
main_opt.o 	   	: src/main.cpp
			$(CC) $(CFLAGS) $< -Ilib -o $@
CycleBreaking.o	: src/CycleBreaking.cpp src/CycleBreaking.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*