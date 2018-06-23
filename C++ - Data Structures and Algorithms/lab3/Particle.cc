#include <iostream>
#include <limits>
#include "SDL/SDL.h"
#include <cstdlib>
#include <math.h>
#include "Particle.h"

Particle::
Particle()
{
  rx = rand() / (RAND_MAX + 1.0);
  ry = rand() / (RAND_MAX + 1.0);
  vx = 0.01 * (rand() / (RAND_MAX + 1.0) - 0.5);
  vy = 0.01 * (rand() / (RAND_MAX + 1.0) - 0.5);
  radius = 0.005;
  mass = 0.5;
  R = rand() % 256;
  G = rand() % 256;
  B = rand() % 256;
  return;
}

Particle::
Particle(double _rx, double _ry, double _vx, double _vy, double _radius, double _mass, unsigned char _r, unsigned char _g, unsigned char _b)
  : rx(_rx), ry(_ry), vx(_vx), vy(_vy), radius(_radius), mass(_mass),
    R(_r), G(_g), B(_b) {}

/**
 * Updates position to reflect passage of time dt
 */
void
Particle::
move(double dt)
{
  rx += vx * dt;
  ry += vy * dt;
}

/**
 * Update velocities upon collision between this particle and particle b
 */
void
Particle::
bounceOff(Particle& b)
{
  double dx  = b.rx - rx;
  double dy  = b.ry - ry;
  double dvx = b.vx - vx;
  double dvy = b.vy - vy;
  double dvdr = dx*dvx + dy*dvy;   // dv dot dr
  double dist = radius + b.radius; // distance between particle centers at collison

  // normal force F, and in x and y directions
  double F = 2 * mass * b.mass * dvdr / ((mass + b.mass) * dist);
  double fx = F * dx / dist;
  double fy = F * dy / dist;

  // update velocities according to normal force
  vx += fx / mass;
  vy += fy / mass;
  b.vx -= fx / b.mass;
  b.vy -= fy / b.mass;

  // update collision counts
  count++;
  b.count++;
}

/**
 * Update velocity of this particle upon collision with a horizontal wall
 */
void
Particle::
bounceOffHorizontalWall()
{
  vy = -vy;
  count++;
}

/**
 * Update velocity of this particle upon collision with a vertical wall
 */
void
Particle::
bounceOffVerticalWall()
{
  vx = -vx;
  count++;
}

/**
 * Return the number of collisions involving this particle
 */
int
Particle::
collisions() const
{
  return count;
}

/**
 * How long into future until collision between this particle a and b?
 */
double
Particle::
timeToHit(const Particle& b) const
{
  if (this == &b) return std::numeric_limits<double>::infinity();
  double dx  = b.rx - rx;
  double dy  = b.ry - ry;
  double dvx = b.vx - vx;
  double dvy = b.vy - vy;
  double dvdr = dx*dvx + dy*dvy;
  if (dvdr > 0) return std::numeric_limits<double>::infinity();
  double dvdv = dvx*dvx + dvy*dvy;
  double drdr = dx*dx + dy*dy;
  double sigma = radius + b.radius;
  double d = (dvdr*dvdr) - dvdv * (drdr - sigma*sigma);
  if (d < 0) return std::numeric_limits<double>::infinity();
  return -(dvdr + sqrt(d)) / dvdv;
}

/**
 * How long into future until this particle collides with a horizontal wall?
 */
double
Particle::
timeToHitHorizontalWall() const
{
  if      (vy > 0) return (1.0 - ry - radius) / vy;
  else if (vy < 0) return (radius - ry) / vy;
  else             return std::numeric_limits<double>::infinity();
}

/**
 * How long into future until this particle collides with a vertical wall?
 */
double
Particle::
timeToHitVerticalWall() const
{
  if      (vx > 0) return (1.0 - rx - radius) / vx;
  else if (vx < 0) return (radius - rx) / vx;
  else             return std::numeric_limits<double>::infinity();
}

/**
 * Draw particle to screen
 */
void
Particle::
draw(SDL_Surface* screen) const
{
  if (radius < 0) throw particle_error("circle radius can't be negative");

  double xs = screen->w * rx;
  double ys = screen->h * ry;
  double rs = screen->w * radius;

  if (rs <= 1)
    {
      unsigned int *ptr = (unsigned int*)screen->pixels;
      double lineoffset = ys * (screen->pitch / 4);
      ptr[(int)(lineoffset + xs)] = SDL_MapRGB(screen->format,R,G,B);
    }
  else
    drawcircle(screen, xs, ys, rs, SDL_MapRGB(screen->format,R,G,B));
}

/**
 * Use fairly primitive to draw particle as filled circle
 */
void
Particle::
drawcircle(SDL_Surface* screen, int x, int y, int r, Uint32 c) const
{
  int i, j;
  for (i = 0; i < 2 * r; i++)
    {
      // vertical clipping: (top and bottom)
      if ((y - r + i) >= 0 && (y - r + i) < screen->h)
	{
	  int len = (int)(sqrt ((double)r * r - (r - i) * (r - i)) * 2.0);
	  int xofs = x - len / 2;

	  // left border
	  if (xofs < 0)
	    {
	      len += xofs;
	      xofs = 0;
	    }

	  // right border
	  if (xofs + len >= screen->w)
	    {
	      len -= (xofs + len) - screen->w;
	    }
	  int ofs = (y - r + i) * (screen->pitch / 4) + xofs;

	  // note that len may be 0 at this point,
	  // and no pixels get drawn!
	  for (j = 0; j < len; j++)
	    ((unsigned int*)screen->pixels)[ofs + j] = c;
	}
    }
}
