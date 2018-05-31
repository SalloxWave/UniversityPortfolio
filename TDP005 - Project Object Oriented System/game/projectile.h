//
// Created by alexanderjlinux on 11/29/16.
//

#ifndef GAME38_PROJECTILE_H
#define GAME38_PROJECTILE_H

#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

#include "entity.h"
class Level;
#include "level.h"
#include "enum_projectile_state.h"

//! A class representing a shootable projectile
/*!
 * An entity but with extra members needed to represent a shootable projectile
 */
class Projectile: public Entity
{
public:
    //! A default constructor
    /*!
     * Constructor uses compiler's default way of constructing class.
     */
    Projectile() = default;

    /*!
     * Constructing a projectile with properties based on the projectile id
     * @param projectile_id The specified projectile id
     */
    Projectile(unsigned int projectile_id);

    /*!
     * Checks if the projectile collides with the specified level or not.
     * This is done by comparing position of projectile with width and height of the level.
     * @param level The specified level
     * @return If the projectile collides with the specified level or not
     */
    bool collides(Level const & level) const;

    /*!
     * Change the projectile state of the projectile
     * @param projectile_state The specfied projectile state
     */
    void changeState(Projectile_State projectile_state);

    /*!
     * Get the damage of the projectile
     * @return The damage of the projectile
     */
    int getDamage() const;

    /*!
     * Get the cooldown of the projectile
     * @return The cooldown of the projectile
     */
    double getCooldown() const;

    //! Get the current state of the projectile
    /*!
     * Get the current state of the projectile
     * @return The current state of the projectile
     */
    Projectile_State getState() const;
private:
    //! Checks if the projectile can move or not
    /*!
     * Checks if the projectile can move the specified direction.
     * NOTE: This always return true by now because there's currently
     * now way to tell if projectile can move or not without checking for collision.
     * @param direction The specified direction
     * @return If projectile can move or not
     */
    bool can_move(Direction direction) const override;
    int damage;
    double cooldown;
    Projectile_State state;
};


#endif //GAME38_PROJECTILE_H
