/*
 * brute < input.txt
 *
 * Compute and plot all line segments involving 4 points in input.txt using SDL
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "SDL/SDL.h"
#include "Point.h"

using namespace std;

void
render_points(SDL_Surface* screen, const vector<Point>& points)
{
  SDL_LockSurface(screen);

  for(const auto& point : points)
    {
      point.draw(screen);
    }

  SDL_FreeSurface(screen);
  SDL_Flip(screen); // display screen
}

void
render_line(SDL_Surface* screen, const Point& p1, const Point& p2)
{
  SDL_LockSurface(screen);

  p1.lineTo(screen, p2);

  SDL_FreeSurface(screen);
  SDL_Flip(screen); // display screen
}

int
main(int argc, char* argv[])
{
  if (argc != 1)
    {
      cout << "Usage:" << endl << argv[0] << " < input.txt" << endl;

      return 0;
    }

  // we only need SDL video
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
      exit(1);
    }

  // register SDL_Quit to be called at exit
  atexit(SDL_Quit);

  // set window title
  SDL_WM_SetCaption("Pointplot", 0);

  // Set the screen up
  SDL_Surface* screen = SDL_SetVideoMode(512, 512, 32, SDL_SWSURFACE);

  if (screen == nullptr)
    {
      fprintf(stderr, "Unable to set 512x512 video: %s\n", SDL_GetError());
      exit(1);
    }

  // clear the screen with white
  SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

  // the array of points
  vector<Point> points;

  // read points from cin
  int N;
  unsigned int x;
  unsigned int y;

  cin >> N;

  for (int i{0}; i < N; ++i) {
    cin >> x >> y;
    points.push_back(Point(x, y));
  }

  // draw points to screen all at once
  render_points(screen, points);

  // sort points by natural order
  // makes finding endpoints of line segments easy
  sort(points.begin(), points.end());

  auto begin = chrono::high_resolution_clock::now();

  //For each point
  for (Point origin : points) {
    
    //Create sorting object using origin.
    PolarSorter sorter(origin);

    //Create vector of all other pointes;
    vector<Point> others;

    //Fill it
    for (Point other : points) {
      // if (origin == other) {
      //   continue;
      // }
      others.push_back(other);
    }

    //Sort it with respect to slope to the origin.
    sort(others.begin(), others.end(), sorter);

    //Create variables for the sequence-finding algorithm:
    Point prevPoint = others.front();
    int sequenceLength = 1;

    //Loop others
    for (int i = 1; i < others.size(); i++) {

      //If slope of prev and current is the same, increment the sequence.
      if (prevPoint.getSlopeToCurrentOrigin() == others[i].getSlopeToCurrentOrigin()) {
        sequenceLength++;
      }

      //Else sequence is cancelled
      else {

        //Check if sequenceLength is >= 3, if true sequence is found.
        //Draw if sequenceLength is below 5 (for performance)
        if (sequenceLength >= 3 && sequenceLength < 5) {          
          render_line(screen, prevPoint, others[i-1]);
        }

        //Reset sequence and set prevPoint to current point.
        sequenceLength = 1;
        prevPoint = others[i];
      }
    }

    //If the sequence didn't reset before loop was done we need to draw it.
    if (sequenceLength >= 3 && sequenceLength < 5) {
      render_line(screen, prevPoint, others.back());
    }
  }

  auto end = chrono::high_resolution_clock::now();
  cout << "Computing line segments took "
       << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
       << " milliseconds." << endl;

  // wait for user to terminate program
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
	break;
      case SDL_KEYUP:
	if (event.key.keysym.sym == SDLK_ESCAPE)
	  return 0;
	break;
      case SDL_QUIT:
	return(0);
      }
    }
  }

  return 0;
}
