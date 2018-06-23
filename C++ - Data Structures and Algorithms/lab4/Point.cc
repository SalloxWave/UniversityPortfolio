#include <limits>
#include "SDL/SDL.h"
#include <math.h>
#include "Point.h"
#include <stdlib.h> 
/**
 * Slope between this point and p
 *
 * If the points are the same, negative infinity is returned
 * If the line between the points is horizontal positive zero is returned
 * If the line between the points is vertical positive infinity is returned
 */
double
Point::
slopeTo(const Point& p) const
{
  if (x == p.x && y == p.y)
    return  -std::numeric_limits<double>::infinity();
  else if (y == p.y) // horizontal line segment
    return 0.0;
  else if (x == p.x) // vertical line segment
    return  std::numeric_limits<double>::infinity();
  else
    return (static_cast<double>(p.y) - static_cast<double>(y)) / 
           (static_cast<double>(p.x) - static_cast<double>(x));
}

/**
 * Draw point to screen
 */
void
Point::
draw(SDL_Surface* screen) const
{
  unsigned int* ptr{static_cast<unsigned int*>(screen->pixels)};
  unsigned int xscaled{x / ((COORD_MAX + 1) / screen->w)};
  unsigned int yscaled{(COORD_MAX - y) / ((COORD_MAX + 1) / screen->h)};
  unsigned int lineoffset{yscaled * (screen->pitch / 4)};

  // add a black pixel to screen at proper position
  ptr[lineoffset + xscaled] = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
}

/**
 * Draw line from this point to p to screen using Bresenham's line algorithm
 */
void
Point::
lineTo(SDL_Surface* screen, const Point& p) const
{
  unsigned int* ptr{static_cast<unsigned int*>(screen->pixels)};
  unsigned int xscaled{x / ((COORD_MAX + 1) / screen->w)};
  unsigned int yscaled{(COORD_MAX - y) / ((COORD_MAX + 1) / screen->h)};
  unsigned int pxscaled{(p.x / ((COORD_MAX + 1) / screen->w))};
  unsigned int pyscaled{(COORD_MAX - p.y) / ((COORD_MAX + 1) / screen->h)};
  // calculate the x and y spans of the line
  int dx = pxscaled - xscaled + 1;
  int dy = pyscaled - yscaled + 1;

  // figure out the correct increment for the major axis
  // account for negative spans
  int xinc;
  int yinc;
  if (dx < 0)
    {
      xinc = -1;
      dx = -dx;
    }
  else
    xinc = 1;

  if (dy < 0)
    {
      yinc = -screen->pitch / 4;
      dy = -dy;
    }
  else
    yinc = screen->pitch / 4;

  /* This is out current offset into the buffer. We use this
     variable so that we don't have to calculate the offset at
     each step; we simply increment this by the correct amount.

     Instead of adding 1 to the x coordinate, we add one to drawpos.
     Instead of adding 1 to the y coordinate, we add the surface's
     pitch (scanline width) to drawpos. */
  unsigned int drawpos{screen->pitch / 4 * yscaled + xscaled};

  int sum{0};

  // our major loop will be different depending on the major axis
  if (dx < dy)
    {
      // loop through each pixel along the major axis
      for (int i{0}; i < dy; ++i)
	{
	  //draw the pixel
	  ptr[drawpos] =  SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);

	  // update the incremental division
	  sum += dx;

	  // if we have reached the dividend, advance and reset
	  if (sum >= dy)
	    {
	      drawpos += xinc;
	      sum -= dy;
	    }

	  // increment the drawing position
	  drawpos += yinc;
	}
    }
  else
    {
      // symmetric case
      for (int i{0}; i < dx; ++i)
	{
	  ptr[drawpos] =  SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);
	  sum += dy;
	  if (sum >= dx)
	    {
	      drawpos += yinc;
	      sum -= dx;
	    }
	  drawpos += xinc;
	}
    }
}

/**
 * Is this point lexicographically smaller than p?
 * Comparing x-ccordinates and breaking ties by y-coordinates
 */
bool
Point::
operator<(const Point& other) const
{
  if (x == other.x)
    return y < other.y;
  else
    return x < other.x;
}

bool
Point::
operator>(const Point& other) const
{
  return other < *this;
}

ostream&
operator<<(std::ostream& out, const Point& p)
{
  out << "(" << p.x << ", " << p.y << ")";
  return out;
}

// bool
// Point::
// operator==(Point& other) const 
// {
//   return other.x == x && other.y == y;
// }

void 
Point::
setSlopeToCurrentOrigin(double slope) {
  slopeToCurrentOrigin = slope;
}

double 
Point::
getSlopeToCurrentOrigin() {
  return slopeToCurrentOrigin;
}