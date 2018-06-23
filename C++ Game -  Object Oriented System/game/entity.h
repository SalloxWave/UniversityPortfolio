//
// Created by alexanderjlinux on 11/29/16.
//

#ifndef GAME38_ENTITY_H
#define GAME38_ENTITY_H

#include <string>

#include "struct_point.h"
#include "enum_direction.h"

//! A pure virtual class representing a movable entity
/*!
 * The class has basic members all movable entities should have,
 * such as position, direction, size etc.
 */
class Entity
{
public:
    //! Moves the entity in a direction
    /*!
     * Moves the entity in the specified direction a specified
     * amount of steps (essentially the same as distance)
     * @param direction The direction to move in
     * @param steps Amount of steps to move entity (essentially the same as distance)
     */
    void move(Direction direction, unsigned int steps = 1);

    //! Change direction entity is faced
    /*!
     * @param direction Direction to change into
     */
    void changeDirection(Direction direction);

    //! Change position of the entity
    /*!
     * @param pos Specified position
     */
    void setPosition(Point pos);

    //! Change position of the entity
    /*!
     * @param x x-position
     * @param y y-position
     */
    void setPosition(int x, int y);

    //! Get the position of the entity
    /*!
     * @return Position of entity
     */
    Point getPosition() const;

    //! Get the name of the entity
    /*!
     * @return Name of entity
     */
    std::string getName() const;

    //! Get the speed of the entity
    /*!
     * @return Speed of entity
     */
    double getSpeed() const;

    //! Get the width of the entity
    /*!
     * @return Width of entity
     */
    unsigned int getWidth() const;

    //! Get the height of the entity
    /*!
     * @return Height of the entity
     */
    unsigned int getHeight() const;

    //! Get the source to the sprite image
    /*!
     * @return Source to the sprite image
     */
    std::string getSpriteSource() const;

    //! Get the direction the entity is currently facing
    /*!
     * @return Direction entity is currently facing
     */
    Direction getDirection() const;
protected:
    //! A pure virtual function that checks if the entity can move or not
    /*!
     * Checks if the entity can move or not. Since this is checked differently for
     * each entity type the function is pure virtual.
     * @param direction
     * @return If entity can move or not
     */
    virtual bool can_move(Direction direction) const = 0;
    std::string name;
    Point position;
    Direction dir;
    double speed;
    unsigned int width;
    unsigned int height;
    std::string sprite_src;
};

#endif //GAME38_ENTITY_H
