//
// Created by alexanderjlinux on 11/29/16.
//

#ifndef GAME38_GUI_CHARACTERSCREEN_H
#define GAME38_GUI_CHARACTERSCREEN_H

#include <string>
#include <map>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "json.hpp"
using json = nlohmann::json;

#include "base_window.h"
#include "enum_direction.h"

//! A class representing a character selection window
/*!
 * A class inheriting from "Base_Window" to make class represent a window.
 * In addition the class contains members needed for a character selection window in the game.
 * A character selection window is where the players select a character to play
 * (and other possible options before a match starts).
 */
class Characterwindow: public Base_Window, public sf::RenderWindow
{
public:
    //! A default constructor
    /*!
     * Constructor uses compiler's default way of constructing class.
     */
    Characterwindow() = default;

    //! Inherited contructor from "sf::RenderWindow"
    /*!
     * Make class be able to use constructor from "sf::RenderWindow".
     * This is because the class wants to represent a renderable window.
     */
    using sf::RenderWindow::RenderWindow;

    //! Initialize the window
    /*!
     * Initializes member variables by for example setting textures, position and sizes.
     */
    void initialize();

    //! Update graphical display of the window
    /*!
    * Member function to update the graphics of the window based on possible changes
    */
    void updateGraphics();

    //! Change selected character for a player
    /*! Changes either player 1's or player 2's character by moving player in the desired direction
     * in the character list. NOTE: It's only moved if possible.
     * @param direction The direction to go in the character list
     * @param is_player1 If player 1 should change character or not (player 2) (default is true)
     */
    void changeCharacter(Direction direction, bool is_player1 = true);

    //! Get the id of the selected character
    /*!
     * Get the selected character id for the specified player.
     * @param is_player1 If you should return selected character id for player 1 or not (player 2) (default is player 1)
     * @return Selected character id for the specified player
     */
    unsigned int getSelectedCharacterId(bool is_player1 = true) const;
private:
    //! An overrided function loading textures needed for the window
    /*!
     * Loads all the textures needed for the window. This is done by
     * creating a container in which the filename of the texture is connected to the texture object.
     */
    void load_textures() override;

    //! Initializing the list with selectable characters
    /*!
     * Giving positon, size and textures to list of selectable characters
     * @param filename name of file where to load the characters from
     */
    void init_character_list();

    //! Tells if a player can change to the wanted character
    /*!
     * Checks if either player 1 or player 2 can change character by moving player in the
     * desired direction in the character list.
     * @param direction The direction to go in the character list
     * @param is_player1 If player 1 should change character or not (else player 2) (default is true)
     * @return If player can move or not
     */
    bool can_change_character(Direction direction, bool is_player1 = true);
    unsigned int const max_row_size { 4 };
    std::map<int, sf::Sprite> character_list;
    unsigned int player1_selected_id;
    unsigned int player2_selected_id;
    sf::Text player1_text;
    sf::RectangleShape player1_text_bg;
    sf::Text player2_text;
    sf::RectangleShape player2_text_bg;
    sf::Sprite player1_selected_character;
    sf::Sprite player2_selected_character;
    sf::Text player1_name;
    sf::Text player1_info_medium;
    sf::Text player1_info_small;
    sf::Text player2_name;
    sf::Text player2_info_medium;
    sf::Text player2_info_small;
    sf::Text player1_controls;
    sf::Text player2_controls;
    sf::Text enterToPlay_text;
    int enterToPlay_opacity;
    bool enterToPlay_increase_opacity;
    json char_data;
    json projectile_data;
};

#endif //GAME38_GUI_CHARACTERSCREEN_H
