//
// Created by alexanderjlinux on 11/29/16.
//

#ifndef GAME38_GUI_GAMEBOARD_H
#define GAME38_GUI_GAMEBOARD_H

#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "base_window.h"
#include "level.h"
#include "player.h"

//! A class representing a main menu window
/*!
 * A class inheriting from "Base_Window" to make class represent a window.
 * In addition the class contains members needed for a gameboard in the game.
 * A gameboard is a graphical representation of a level with two players fighting.
 */
class Gameboard: public Base_Window, public sf::RenderWindow
{
public:
    //! A default constructor
    /*!
     * Constructor uses compiler's default way of constructing class.
     */
    Gameboard() = default;

    //! Inherited contructor from "sf::RenderWindow"
    /*!
     * Make class be able to use constructor from "sf::RenderWindow".
     * This is because the class wants to represent a renderable window.
     */
    using sf::RenderWindow::RenderWindow;

    //! Initializes member variables of the gameboard window
    /*!
     * Initializes member variables based on the specified level, player 1 and player 2
     * @param level The specified level for gameboard to represent
     * @param player1 Player 1 on the gameboard
     * @param player2 Player 2 on the gameboard
     */
    void initialize(Level const & level, Player const & player1, Player const & player2);

    //! Update graphical display of the gameboard window
    /*!
     * Updates graphics of window based on possible changes, the specified level, player 1 and player 2
     * @param level The specified level for gameboard to represent
     * @param player1 Player 1 on the gameboard
     * @param player2 Player 2 on the gameboard
     */
    void updateGraphics(Level const & level, Player const & player1, Player const & player2);
private:
    //! An overrided function loading textures needed for the window
    /*!
     * Loads all the textures needed for the window. This is done by
     * creating a container in which the filename of the texture is connected to the texture object.
     */
    void load_textures() override;
    sf::Sprite player1_sprite;
    sf::Sprite player2_sprite;
    sf::Sprite player1_portrait;
    sf::Sprite player2_portrait;
    sf::Text level_name;
    sf::Text player1_name;
    sf::Text player2_name;
    sf::Text player1_life;
    sf::Text player2_life;
    std::vector<sf::Sprite> active_projectiles;
};

#endif //GAME38_GUI_GAMEBOARD_H
