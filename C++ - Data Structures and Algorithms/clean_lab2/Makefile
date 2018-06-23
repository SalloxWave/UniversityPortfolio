#
# Makefile för sorteringslösning
#

CCC	= g++ -std=c++11 
LFLAGS	= -L/sw/gcc-${GCC4_V}/lib -static-libstdc++

all: encrypt brute

brute: brute.cc Key.o
	$(CCC) -O3 $(LFLAGS) -o brute brute.cc Key.o

encrypt: encrypt.cc Key.o
	$(CCC) $(LFLAGS) -o encrypt encrypt.cc Key.o

Key.o: Key.cc Key.h
	$(CCC) -c Key.cc Key.h

clean:
	@ \rm -rf *.o *.gch core

zap: clean
	@ \rm -f brute encrypt *~
