#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <iostream>
#include "SDL/SDL.h"
#include "Particle.h"
#include "CollisionSystem.h"

int
main(int argc, char* argv[])
{
  if (argc > 2)
    {
      cout << "Usage:" << endl << argv[0] << " N" << endl << argv[0] << " < input.txt" << endl;

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
  SDL_WM_SetCaption("Colliding particles", 0);

  // Set the screen up
  SDL_Surface* screen = SDL_SetVideoMode(512, 512, 32, SDL_SWSURFACE);

  if (screen == 0)
    {
      fprintf(stderr, "Unable to set 512x512 video: %s\n", SDL_GetError());
      exit(1);
    }

  // the array of particles
  vector<Particle> particles;

  if (argc == 1)
    {
      // read particles from cin
      int N;
      double rx;
      double ry;
      double vx;
      double vy;
      double radius;
      double mass;
      unsigned char r;
      unsigned char g;
      unsigned char b;

      scanf("%d", &N);

      for (int i = 0; i < N; i++)
	{
	  scanf("%lE %lE %lE %lE %lE %lE %hhu %hhu %hhu", &rx, &ry, &vx, &vy, &radius, &mass, &r, &g, &b);
	  particles.push_back(Particle(rx, ry, vx, vy, radius, mass, r, g, b));
	}
    }
  if (argc == 2)
    {
      // N is number of particles to simulate
      int N = atoi(argv[1]);

      // seed random number generator
      srand(time(0));

      // generate N random particles
      for (int i = 0; i < N; i++)
	{
	  particles.push_back(Particle());
	}
    }

  // create collision system and simulate
  CollisionSystem system(particles, screen);
  system.simulate(10000);

  return 0;
}
