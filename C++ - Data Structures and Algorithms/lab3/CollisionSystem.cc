#include <iostream>
#include "SDL/SDL.h"
#include "MinPQ.h"
#include "Event.h"
#include "Particle.h"
#include "CollisionSystem.h"

CollisionSystem::
CollisionSystem(vector<Particle> _particles, SDL_Surface* _screen)
  : t(0.0), hz(0.5), particles(_particles), screen(_screen), quit(false) {}

/**
 * Event based simulation for limit seconds
 */
void
CollisionSystem::
simulate(double limit)
{
  for (int i = 0; i < particles.size(); i++)
    {
      predict(&particles[i], limit);
    }
  pq.toss(Event(0.0, 0, 0));        // redraw event

  // the main event-driven simulation loop
  while (!pq.isEmpty() && !quit) {

    handleIOEvents();

    // get impending event, discard if invalidated
    Event e;
    pq.deleteMin(e);
    if (!e.isValid()) continue;

    Particle* a = e.getA();
    Particle* b = e.getB();

    // physical collision, so update positions, and then simulation clock
    for (int i = 0; i < particles.size(); i++)
      particles[i].move(e.getTime() - t);
    t = e.getTime();

    // process event
    if      (a != 0 && b != 0) a->bounceOff(*b);              // particle-particle collision
    else if (a != 0 && b == 0) a->bounceOffVerticalWall();   // particle-wall collision
    else if (a == 0 && b != 0) b->bounceOffHorizontalWall(); // particle-wall collision
    else if (a == 0 && b == 0) redraw(limit);               // redraw event

    // update the priority queue with new collisions involving a or b
    predict(a, limit);
    predict(b, limit);
  }
}

/**
 * Update priority queue with all new events for particle a
 */
void
CollisionSystem::
predict(Particle* a, double limit)
{
  if (a == 0) return;

  // particle-particle collisions
  for (int i = 0; i < particles.size(); i++) {
      double dt = a->timeToHit(particles[i]);
      if (t + dt <= limit)
    	{
    	  pq.toss(Event(t + dt, a, &particles[i]));
    	}
  }

  // particle-wall collisions
  double dtX = a->timeToHitVerticalWall();
  double dtY = a->timeToHitHorizontalWall();
  if (t + dtX <= limit) pq.toss(Event(t + dtX, a, 0));
  if (t + dtY <= limit) pq.toss(Event(t + dtY, 0, a));
}

/**
 * Redraw all particles
 */
void
CollisionSystem::
redraw(double limit)
{
  SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF )); // clear the screen with white
  SDL_LockSurface(screen);

  for (int i = 0; i < particles.size(); i++)
    {
      particles[i].draw(screen);
    }
  SDL_FreeSurface(screen);
  SDL_Flip(screen); // display screen
  SDL_Delay(20); // pause for 20 milliseconds
  if (t < limit)
    {
      pq.toss(Event(t + 1.0 / hz, 0, 0));
    }
}

/**
 * Handle keyboard and mouse events
 */
void
CollisionSystem::
handleIOEvents()
{
  while (SDL_PollEvent(&ioevents))
    {
      if (ioevents.type == SDL_QUIT) // The user closed the window?
	{
	  quit = true; // Quit the program
	}
    }
}
