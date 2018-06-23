#ifndef PARTICLE_H
#define PARTICLE_H

#include <limits>
#include "SDL/SDL.h"
#include <stdexcept>

class particle_error : public std::logic_error
{
public:
  explicit particle_error(const std::string& what_arg = "")
    : std::logic_error(what_arg) {}
};

class Particle
{
public:

  Particle();
  Particle(double _rx, double _ry, double _vx, double _vy, double _radius, double _mass, unsigned char _r, unsigned char _g, unsigned char _b);

  void  move(double);
  void bounceOff(Particle&);
  void bounceOffHorizontalWall();
  void bounceOffVerticalWall();

  int collisions() const;
  double timeToHit(const Particle&) const;
  double timeToHitHorizontalWall() const;
  double timeToHitVerticalWall() const;

  void draw(SDL_Surface*) const;
  void drawcircle(SDL_Surface*, int, int, int, Uint32) const;
  double rx, ry;    // position
private:


  double vx, vy;    // velocity
  double radius;    // radius
  double mass;      // mass
  int count;        // number of collisions so far
  unsigned char R;  // color R value
  unsigned char G;  // color G value
  unsigned char B;  // color B value
};

#endif
