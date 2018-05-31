//
// Created by alexanderjlinux on 11/29/16.
//

#ifndef GAME38_GUI_MAIN_MENU_H
#define GAME38_GUI_MAIN_MENU_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "base_window.h"
#include "enum_menu_choice.h"

//! A class representing a main menu window
/*!
 * A class inheriting from "Base_Window" to make class represent a window.
 * In addition the class contains members needed for a main menu in the game.
 * A main menu is where a user selects what to do in the game
 */
class Main_Menu: public Base_Window, public sf::RenderWindow
{
public:
    //! A default constructor
    /*!
     * Constructor uses compiler's default way of constructing class.
     */
    Main_Menu() = default;

    //! Inherited contructor from "sf::RenderWindow"
    /*!
     * Make class be able to use constructor from "sf::RenderWindow".
     * This is because the class wants to represent a renderable window.
     */
    using sf::RenderWindow::RenderWindow;

    //! Initializes member variables of the gameboard window
    /*!
     * Initializes member variables by for example setting
     * background image, setting position and size of the members.
     */
    void initialize();

    //! Update graphical display of the window
    /*!
     * Member function to update the graphics of the window based on possible changes
     */
    void updateGraphics();

    //! Go up in the menu
    /*!
     * Go up in the menu by decreasing selected index
     */
    void goUp();

    //! Go down in the menu
    /*!
     * Go down in the menu by increasing selected index
     */
    void goDown();

    //! Get user's currently selected choice
    /*!
     * @return The selected menu choice
     */
    Menu_Choice getMenuChoice();
private:
    //! An overrided function loading textures needed for the window
    /*!
     * Loads all the textures needed for the window. This is done by
     * creating a container in which the filename of the texture is connected to the texture object.
     */
    void load_textures() override;
    sf::Sprite btn_start_game;
    sf::Sprite btn_exit_game;
    sf::Sprite logo_img;
    sf::Text credits;
    sf::Text selected_info;
    int selected_index;
};

#endif //GAME38_GUI_MAIN_MENU_H