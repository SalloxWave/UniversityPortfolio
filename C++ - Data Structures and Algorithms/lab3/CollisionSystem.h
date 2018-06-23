/*************************************************************************
 *
 *  Simulates N particles and their motion according to the laws
 *  of elastic collisions.
 *
 *************************************************************************/
#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "SDL/SDL.h"
#include "MinPQ.h"
#include "Event.h"
#include "Particle.h"

class CollisionSystem
{
public:
  CollisionSystem(vector<Particle>, SDL_Surface*);

  void simulate(double);

private:
  MinPQ<Event> pq;        // the priority queue
  double t;                    // simulation clock time
  double hz;                   // number of redraw events per clock tick
  vector<Particle> particles; // the array of particles
  SDL_Surface* screen;         // main screen used for rendering
  SDL_Event ioevents;          // SDL event queue
  bool quit;                   // quit flag, program exits when tru

  void predict(Particle*, double);

  void redraw(double);
  void handleIOEvents();
};

#endif
