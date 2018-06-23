//
// Created by alexanderjlinux on 11/29/16.
//

#include <fstream>

#include "player.h"

#include "json.hpp"
using json = nlohmann::json;

//Public
Player::Player(unsigned int character_id, bool is_player1 /*= true*/)
{
    //Load character data from JSON file
    std::ifstream json_file {"../resources/data/characters.json"};
    json j;
    json_file >> j;

    //Get index of character in file by specified level id
    int index {0};
    for (int i {0}; i < j.size(); ++i)
    {
        //Found specified index
        if (j[i]["id"] == character_id)
        {
            index = i;
            break;
        }
    }

    //Set player values from JSON data
    //Set if player is player one or not
    player1 = is_player1;
    max_health = j[index]["max_health"];
    current_health = max_health;
    dir = player1 ? Direction::Right : Direction::Left;
    sf::Time time { sf::Time::Zero };
    for (int i {0}; i < j[index]["projectiles"].size(); ++i)
    {
        //Connect projectile id to timer
        projectile_ids[j[index]["projectiles"][i]] = time;
    }
    //Set controls of player based on if player is player 1 or 2
    set_controls();
    //Set entity values
    name = j[index]["name"];
    width = j[index]["width"];
    height = j[index]["height"];
    speed = j[index]["speed"];
    sprite_src = j[index]["sprite_src"];
    portrait_src = j[index]["portrait_src"];
    //Set default values
    state = Player_State::Normal;
}

void Player::setRestrictions(std::array<int, 2> restr)
{
    restrictions = restr;
}

void Player::setRestrictions(int left, int right)
{
    restrictions[0] = left;
    restrictions[1] = right;
}

void Player::setStartPosition(Point position)
{
    start_position = position;
}

void Player::setStartPosition(int x, int y)
{
    start_position.x = x;
    start_position.y = y;
}

bool Player::hasKey(sf::Keyboard::Key key_code) const
{
    //Count amount of keys with specified keycode in controls
    //One is found means player has the specified key
    return (controls.count(key_code) == 1);
}

void Player::shoot(unsigned int projectile_id, sf::Time current_time)
{
    //Create a projectile based on the specified id
    Projectile proj { projectile_id };

    //Update cooldown to elapsed time + projectile cooldown
    projectile_ids[projectile_id] = current_time + sf::seconds(static_cast<float>( proj.getCooldown() ));

    //Set position of projectile based on projectile state
    if (proj.getState() == Projectile_State::High)
    {
        proj.setPosition((player1 ? position.x + 170 : position.x - 50), position.y - height + 40);
    }
    else
    {
        //Origin of player is at the feet to the left
        proj.setPosition((player1 ? position.x + 130: position.x - 20), position.y - proj.getHeight() - 40);
    }

    //Set projectile's direction to same as player direction
    proj.changeDirection(dir);

    //Add projectile to player's active projectiles
    active_projectiles.push_back(proj);
}

void Player::removeProjectile(int index)
{
    active_projectiles.erase(active_projectiles.begin()+index);
}

bool Player::collides(Projectile const & projectile) const
{
    int x_diff { 0 };
    //Check if player is to the left of the projectile
    if ( position.x < projectile.getPosition().x )
    {
        //Get difference
        x_diff = (projectile.getPosition().x + projectile.getWidth()) - position.x;
    }
    //Check if player is to the right of the projectile
    else
    {
        x_diff = (position.x + width) - projectile.getPosition().x;
    }

    //Calculate total width of player and projectile
    unsigned int total_width { width + projectile.getWidth() };

    //Projectile touches player
    if (x_diff <= total_width)
    {
        //Compare player state with projectile state
        switch(projectile.getState())
        {
            case Projectile_State::Low:
                //Player did not jump above low projectile
                return (state != Player_State::Jump);
            case Projectile_State::High:
                //Player did not duck under high projectile
                return (state != Player_State::Duck);
        }
    }
    return false;
}

void Player::giveDamage(int amount)
{
    //To avoid health going under 0
    if ( current_health - amount < 0)
    {
        current_health = 0;
    }
    else
    {
        current_health -= amount;
    }
}

bool Player::isDead() const
{
    return (current_health == 0);
}

void Player::reset()
{
    position = start_position;
    current_health = max_health;
    state = Player_State::Normal;
    active_projectiles.clear();
}

void Player::changeState(Player_State player_state)
{
    state = player_state;
}

Player_Action Player::getAction(sf::Keyboard::Key key_code)
{
    //Return action none if key was not found in player's set controls
    if (!hasKey(key_code))
    {
        return Player_Action::None;
    }
    return controls[key_code];
}

std::vector<Projectile> & Player::getActiveProjectiles()
{
    return active_projectiles;
}

std::vector<Projectile> const & Player::getActiveProjectiles() const
{
    return active_projectiles;
}

unsigned int Player::getProjectileId(Projectile_State const & projectile_state) const
{
    //Iterate through player's projectile ids
    for (auto it = begin(projectile_ids); it != end(projectile_ids); ++it)
    {
        //Create a projectile with the projectile id
        Projectile proj { it->first };
        //Check if the projectile has the specified projectile state
        if ( proj.getState() == projectile_state )
        {
            //Return id of the projectile
            return it->first;
        }
    }
}

std::map<sf::Keyboard::Key, Player_Action> Player::get_controls() const
{
    return controls;
}

Player_State Player::getState() const
{
    return state;
}

std::string Player::getPortraitSource() const
{
    return portrait_src;
}

int Player::getCurrentHealth() const
{
    return current_health;
}

unsigned int Player::getMaxHealth() const
{
    return max_health;
}

bool Player::isPlayerOne() const
{
    return player1;
}

bool Player::can_move(Direction direction) const
{
    //Decide if distance is positive or negative based on direction
    int distance = direction == Direction::Left ? -1 : 1;

    //Get future position of player
    int tmp_x {getPosition().x + distance};

    //Check if future position is inside player's restricted side of the gameboard
    return (tmp_x >= restrictions[0]
         && tmp_x <= restrictions[1] - width);
}

void Player::set_controls()
{
    //Set player 1 controls
    if(player1)
    {
        controls[sf::Keyboard::W] = Player_Action::Jump;
        controls[sf::Keyboard::S] = Player_Action::Duck;
        controls[sf::Keyboard::A] = Player_Action::Left;
        controls[sf::Keyboard::D] = Player_Action::Right;
        controls[sf::Keyboard::R] = Player_Action::ShootLow;
        controls[sf::Keyboard::T] = Player_Action::ShootHigh;
    }
    //Set player 2 controls
    else
    {
        controls[sf::Keyboard::Up] = Player_Action::Jump;
        controls[sf::Keyboard::Down] = Player_Action::Duck;
        controls[sf::Keyboard::Left] = Player_Action::Left;
        controls[sf::Keyboard::Right] = Player_Action::Right;
        controls[sf::Keyboard::Period] = Player_Action::ShootLow;
        controls[sf::Keyboard::Dash] = Player_Action::ShootHigh;
    }
}

bool Player::readyToUse(unsigned int projectile_id, sf::Time current_time)
{
    //Check if current time is newer than projectile's time
    return (current_time >= projectile_ids[projectile_id]);
}