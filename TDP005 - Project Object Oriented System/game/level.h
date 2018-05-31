//
// Created by alexanderjlinux on 11/29/16.
//

#ifndef GAME38_LEVEL_H
#define GAME38_LEVEL_H

#include <string>
#include <array>

class Player;
#include "player.h"

//! A class representing a level where entity can move
/*!
 * Essentially represents a gamebaord where entity can move.
 * A level for example has a size and contains players
 * and the player's side of the board
 */
class Level
{
public:
    //! A default constructor
    /*!
     * Constructor uses compiler's default way of constructing class.
     */
    Level() = default;

    //! Constructs a level based on level id
    /*!
    * Constructing a level with properties based on the level id
    * @param projectile_id The specified level id
    */
    Level(unsigned int level_id);

    //! Add a player to the level
    /*!
     * Adds a player to the level by pointing to the specified player
     * @param player A reference to the player
     */
    void addPlayer(Player &player);

    //! Reload the level
    /*!
     * Reload the level by resetting players
     */
    void reload();

    //! Get name of the level
    /*!
     * @return The name of the level
     */
    std::string getName() const;

    //! Get the width of the level
    /*!
     * @return The width of the level
     */
    unsigned int getWidth() const;

    //! Get the height of the level
    /*!
     * @return The height of the level
     */
    unsigned int getHeight() const;

    //! Get source to the background image of level
    /*!
     * @return The source to the background image of level
     */
    std::string getBgSource() const;
private:
    std::string name;
    unsigned int width;
    unsigned int height;
    int y_level;
    std::array<int, 2> restriction_left;
    std::array<int, 2> restriction_right;
    Player* player1;
    Player* player2;
    std::string bg_src;
};

#endif //GAME38_LEVEL_H