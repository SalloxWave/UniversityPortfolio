/*************************************************************************
 *  An event during a particle collision simulation. Each event contains
 *  the time at which it will occur (assuming no supervening actions)
 *  and the particles a and b involved.
 *
 *    -  a and b both null:      redraw event
 *    -  a null, b not null:     collision with vertical wall
 *    -  a not null, b null:     collision with horizontal wall
 *    -  a and b both not null:  binary collision between a and b
 *
 *************************************************************************/
#ifndef EVENT_H
#define EVENT_H

#include "Particle.h"

class Event
{
public:
  Event();
  Event(double, Particle*, Particle*);

  bool isValid();
  Particle* getA();
  Particle* getB();
  double getTime();
  bool operator<(const Event&) const;
  bool operator==(const Event&) const;
  bool operator!=(const Event&) const;
  bool operator>(const Event&) const;
  bool operator<=(const Event&) const;
  bool operator>=(const Event&) const;

private:
  double time; // time that event is scheduled to occur
  Particle* a; // particle involved in event, possibly null
  Particle* b; // particle involved in event, possibly null
  int countA;  // collision counts at event creation
  int countB;  // collision counts at event creation
};

#endif
