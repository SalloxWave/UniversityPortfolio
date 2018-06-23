#
# Makefile för simulering
#

CCC	= g++
LFLAGS	= -L/usr/lib/x86_64-linux-gnu -lSDL -lm

all: simulation.cc CollisionSystem.o Event.o Particle.o
	$(CCC) -o simulation simulation.cc CollisionSystem.o Event.o Particle.o $(LFLAGS$)

CollisionSystem.o: CollisionSystem.cc CollisionSystem.h MinPQ.h
	$(CCC) -c CollisionSystem.cc CollisionSystem.h MinPQ.h

Event.o: Event.cc Event.h
	$(CCC) -c Event.cc Event.h

Particle.o: Particle.cc Particle.h
	$(CCC) -c Particle.cc Particle.h

clean:
	@ \rm -rf *.o *.gch core

zap: clean
	@ \rm -f simulation *~
