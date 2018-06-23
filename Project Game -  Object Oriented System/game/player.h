//
// Created by alexanderjlinux on 11/29/16.
//

#ifndef GAME38_PLAYER_H
#define GAME38_PLAYER_H

#include <string>
#include <vector>
#include <array>
#include <map>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Time.hpp>

#include "entity.h"
class Projectile;
#include "projectile.h"
enum class Projectile_State;
#include "enum_projectile_state.h"
#include "enum_player_state.h"
#include "enum_player_action.h"
#include "struct_point.h"

//! A class representing a playable character.
/*!
 * An entity but with extra members needed for a playable character.
 */
class Player: public Entity
{
public:
    //! A default constructor
    /*!
     * Constructor uses compiler's default way of constructing class.
     */
    Player() = default;

    //! Constructs a new player
    /*!
     * Constructing a player with properties based on the character id and
     * if the player is player 1 or not (else player 2)
     * @param character_id The specified character id
     * @param is_player1 If the player should be player 1 or not (else player 2)
     */
    Player(unsigned int character_id, bool is_player1 = true);

    //! Set restrictions in x-axis
    /*!
     * A player can't move outside of the restriction. This is essentially the
     * player's side of the gamebaord
     * @param restrictions
     */
    void setRestrictions(std::array<int, 2> restrictions);

    //! Set restrictions in x-axis
    /*!
     * A player can't move outside of the restriction. This is essentially the
     * player's side of the gamebaord
     * @param left The restriction on the left side
     * @param right The restriction on the right side
     */
    void setRestrictions(int left, int right);

    //! Set where player should start
    /*!
     * @param position Specified position for player to start
     */
    void setStartPosition(Point position);

    //! Set where player should start
    /*!
     * @param x Where to start on x-axis
     * @param y Where to start on y-axis
     */
    void setStartPosition(int x, int y);

    //! Checks if player has a specified key
    /*!
     * Checks if the specified keyboard key can be used for the player by
     * comparing with player's set controls
     * @param key_code Specified keyboard key
     * @return If the specified keyboard key can be used for the player
     */
    bool hasKey(sf::Keyboard::Key key_code) const;

    //! Shoots a projectile with the specified projectile id
    /*!
     * Shoots a projectile with a specified projectile id
     * @param projectile_id The projectile id to shoot
     * @param current_time Time to update in player's projectile cooldowns
     */
    void shoot(unsigned int projectile_id, sf::Time current_time);

    //! Checks if player is ready to use a projectile
    /*!
     * Checks if the projectile with the specified projectile can id is
     * ready to use based on projectiles cooldown
     * @param projectile_id The specified projectile id
     * @param current_time Time to compare to player's projectile cooldowns
     * @return
     */
    bool readyToUse(unsigned int projectile_id, sf::Time current_time);

    //! Removes a projectile for the player
    /*!
     * Remove projectile in list of projectiles based on index
     * @param index Specified list index
     */
    void removeProjectile(int index);

    //! Checks if player collides with a projectile
    /*!
     * Checks if player collides with a specified projectile based on
     * position, player state and projectile state
     * @param projectile The specified projectile
     * @return If player collides with the projectile or not
     */
    bool collides(Projectile const & projectile) const;

    //! Damage the player
    /*!
     * Damage player with a specified amount by decreasing player's health
     * @param amount Amount of damage to give to player
     */
    void giveDamage(int amount);

    //! Tells if player is dead or not
    /*!
     * Checks if player is dead or not based on current health
     * @return If player is dead or not
     */
    bool isDead() const;

    //! Reset a player
    /*!
     * Resets a player by moving to start position and give player full health
     */
    void reset();

    //! Change the state of the player
    /*!
     * Change player state of the player
     * @param state The specified player state
     */
    void changeState(Player_State state);

    //! Get action of the player
    /*!
     * Get the player action based on the specified keyboard key. If the key was
     * not in the set controls of the player, return player action None
     * @param keycode The specified keyboard key
     * @return Player action (None if key was not found)
     */
    Player_Action getAction(sf::Keyboard::Key key_code);

    //! Get player's set controls
    /*!
     * Get list of controls where a keyboard key is connected
     * to a player action
     * @return A list of controls
     */
    std::map<sf::Keyboard::Key, Player_Action> get_controls() const;

    //! Get player's shot active projectiles
    /*!
     * Get a reference to player's currently active projectiles
     * (projectiles that have been shot)
     * @return A reference to player's active projectiles
     */
    std::vector<Projectile> & getActiveProjectiles();

    //! Get player's shot active projectiles
    /*!
     * Get an unchangeable reference to player's currently active projectiles
     * (projectiles that have been shot)
     * @return An unchangeable reference to player's active projectiles
     */
    std::vector<Projectile> const & getActiveProjectiles() const;

    //! Get a projectile id based on projectile state
    /*!
     * Get a player's projectile id based on the specified projectile state
     * @param projectile_state Specified projectile state
     * @return A projectile id with the specified projectile state
     */
    unsigned int getProjectileId(Projectile_State const & projectile_state) const;

    //! Get current player state
    /*!
     * @return Current player state
     */
    Player_State getState() const;

    //! Get player's portrait source
    /*!
     * @return Player's portrait source. This is the texture to
     * display when wanting to only show player's face
     */
    std::string getPortraitSource() const;

    //! Get current health
    /*!
     * @return The current health
     */
    int getCurrentHealth() const;

    //! Get max health
    /*!
     * @return The max health
     */
    unsigned int getMaxHealth() const;

    //! Tells if player is player is player 1
    /*!
     * Tells if the player is player 1 or not (else player 2)
     * @return If the player is player 1 or not
     */
    bool isPlayerOne() const;
private:
    //! Tells if the player can move
    /*!
     * Checks if player can move the specified direction based on player's restrictions
     * @param direction The specified direction
     * @return If player can move or not
     */
    bool can_move(Direction direction) const override;

    //! Sets controls for the player
    /*!
     * Set controls of based on if plaer is player 1 or player 2
     * and connect controls to player action
     */
    void set_controls();
    bool player1;
    unsigned int max_health;
    int current_health;
    Point start_position;
    Player_State state;
    std::string portrait_src;
    std::vector<Projectile> active_projectiles;
    std::map<unsigned int, sf::Time> projectile_ids;
    std::array<int, 2> restrictions;
    std::map<sf::Keyboard::Key, Player_Action> controls;
};

#endif //GAME38_PLAYER_H