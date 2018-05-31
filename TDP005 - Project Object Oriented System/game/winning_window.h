//
// Created by alejo720 on 2016-12-14.
//

#ifndef GAME38_WINNING_BOX_H
#define GAME38_WINNING_BOX_H

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "base_window.h"
#include "player.h"
#include "enum_winning_option.h"

//! A class representing a winning window
/*!
 * A class inheriting from "Base_Window" to make class represent a window.
 * In addition the class contains members needed for a winning window in the game.
 * A winning window is the window that should appear after a match is finished.
 * The window shows who has won and let player's select what do after the match.
 */
class Winning_Window: public Base_Window, public sf::RenderWindow
{
public:
    //! A default constructor
    /*!
     * Constructor uses compiler's default way of constructing class.
     */
    Winning_Window() = default;

    //! Inherited contructor from "sf::RenderWindow"
    /*!
    * Make class be able to use constructor from "sf::RenderWindow".
    * This is because the class wants to represent a renderable window.
    */
    using sf::RenderWindow::RenderWindow;

    //! Initializes member variables of the winning window
    /*!
    * Initializes member variables of the winning window based on who is the winner
    */
    void initialize(Player const & winner);

    //! Update graphical display of the gameboard window
    /*!
    * Member function to update the graphics of the window based on possible changes
    */
    void updateGraphics();

    //! Go up in the winning menu
    /*!
     * Go up in the selectable winning options
     */
    void goUp();

    //! Go up in the winning menu
    /*!
     * Go down in the selectable winning options
     */
    void goDown();

    //! Get user's currently selected winning choice
    /*!
     * @return The selected wining option
     */
    Winning_Option getWinningOption();
private:
    //! An overrided function loading textures needed for the window
    /*!
     * Loads all the textures needed for the window. This is done by
     * creating a container in which the filename of the texture is connected to the texture object.
     */
    void load_textures() override;
    int selected_index;
    sf::Text winner_text;
    sf::Text play_again;
    sf::Text change_character;
    sf::Text exit_game;
};


#endif //GAME38_WINNING_BOX_H
