//
// Created by alexanderjlinux on 11/29/16.
//

#include "projectile.h"

Projectile::Projectile(unsigned int projectile_id)
{
    //Load json data from JSON file
    std::ifstream file {"../resources/data/projectiles.json"};
    json j;
    file >> j;

    //Get index of projectile in file by specified level id
    int index {0};
    for (int i {0}; i < j.size(); ++i)
    {
        //Found specified index
        if (j[i]["id"] == projectile_id)
        {
            index = i;
            break;
        }
    }

    //Set projectile values from JSON data
    damage = j[index]["damage"];
    cooldown = j[index]["cooldown"];
    state = j[index]["state"] == "high" ? Projectile_State::High : Projectile_State::Low;
    //Set entity values
    name = j[index]["name"];
    width = j[index]["width"];
    height = j[index]["height"];
    speed = j[index]["speed"];
    sprite_src = j[index]["sprite_src"];
}

bool Projectile::collides(Level const & level) const
{
    //Projectile's direction is to the left
    if (dir == Direction::Left)
    {
        //Projectile is outside of the level to the left
        return position.x < 0;
    }
    //Projectile's direction is to the right
    else
    {
        //Projectile is outside of the level to the right
        return (position.x + width) > level.getWidth();
    }
}

void Projectile::changeState(Projectile_State projectile_state)
{
    state = projectile_state;
}

int Projectile::getDamage() const
{
    return damage;
}

double Projectile::getCooldown() const
{
    return cooldown;
}

Projectile_State Projectile::getState() const
{
    return state;
}

bool Projectile::can_move(Direction direction) const
{
    return true;
}
