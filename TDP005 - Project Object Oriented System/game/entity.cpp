//
// Created by alexanderjlinux on 11/29/16.
//

#include "entity.h"

void Entity::move(Direction direction, unsigned int steps /* =1 */)
{
    //Decide if distance is positive or negative based on direction
    int distance = direction == Direction::Left ? -1 : 1;

    //Go through amount of steps to move
    for (int i {0}; i < steps; ++i)
    {
        //Check if it's possible to move that direction
        if (can_move(direction))
        {
            //Move one step
            position.x += distance;
        }
        else
        {
            //Stop trying to move steps
            break;
        }
    }
}

void Entity::changeDirection(Direction direction)
{
    dir = direction;
}

void Entity::setPosition(Point pos)
{
    position = pos;
}

void Entity::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}

Point Entity::getPosition() const
{
    return position;
}

std::string Entity::getName() const
{
    return name;
}

double Entity::getSpeed() const
{
    return speed;
}

unsigned int Entity::getWidth() const
{
    return width;
}

unsigned int Entity::getHeight() const
{
    return height;
}

std::string Entity::getSpriteSource() const
{
    return sprite_src;
}

Direction Entity::getDirection() const
{
    return dir;
}