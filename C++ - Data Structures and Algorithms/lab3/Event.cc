#include "Event.h"
#include "Particle.h"

Event::
Event() {}

Event::
Event(double t, Particle* _a, Particle* _b)
  : time(t), a(_a), b(_b), countA(-1), countB(-1)
{
  if (a != 0) countA = a->collisions();
  if (b != 0) countB = b->collisions();
}

bool
Event::
isValid()
{
  if (a != 0 && a->collisions() != countA) return false;
  if (b != 0 && b->collisions() != countB) return false;
  return true;
}

Particle*
Event::
getA()
{
  return a;
}


Particle*
Event::
getB()
{
  return b;
}

double
Event::
getTime()
{
  return time;
}

bool
Event::
operator<(const Event& rhs) const
{
  return time < rhs.time;
}

bool
Event::
operator==(const Event& rhs) const
{
  return time == rhs.time;
}

bool
Event::
operator!=(const Event& rhs) const
{
  return !(*this == rhs);
}

bool
Event::
operator>(const Event& rhs) const
{
  return (rhs < *this);
}

bool
Event::
operator<=(const Event& rhs) const
{
  return !(rhs < *this);
}

bool
Event::
operator>=(const Event& rhs) const
{
  return !(*this < rhs);
}
