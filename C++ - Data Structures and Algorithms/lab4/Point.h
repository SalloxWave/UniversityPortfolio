/*************************************************************************
 *
 * An immutable data type for points in the plane.
 *
 *************************************************************************/

#ifndef POINT_H
#define POINT_H

#include <iostream>
#include "SDL/SDL.h"

#define COORD_MAX 32767 // max value of x and y coordinates

using namespace std;

class Point
{
public:

  Point() = delete;
  Point(unsigned int _x, unsigned int _y) : x(_x), y(_y){}

  double slopeTo(const Point&) const;
  void draw(SDL_Surface*) const;
  void lineTo(SDL_Surface*, const Point&) const;

  void setSlopeToCurrentOrigin(double);
  double getSlopeToCurrentOrigin();

  bool operator<(const Point&) const;
  bool operator>(const Point&) const;
  // bool operator==(Point&) const;

  friend ostream& operator<<(ostream&, const Point&);

private:
  unsigned int x, y; // position
  double slopeToCurrentOrigin;
};

class PolarSorter
{
public:

  //Used when sorting points with respect to slope.
  //If slope is the same, sort using position.

  PolarSorter (const Point& _origin) : origin(_origin) {}
  bool operator() (Point& p1, Point& p2) const
  {
    double originP1 = origin.slopeTo(p1);
    double originP2 = origin.slopeTo(p2);

    p1.setSlopeToCurrentOrigin(originP1);
    p2.setSlopeToCurrentOrigin(originP2);

    if (originP1 == originP2) {
      return p1 < p2;
    }
    return originP1 < originP2;
  }
private:
  Point origin;
};

#endif
