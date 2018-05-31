//
// Created by alexanderjlinux on 11/29/16.
//

#ifndef GAME38_GUI_H
#define GAME38_GUI_H

#include <string>
#include <vector>
#include <map>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

//! An abstract class representing a window
/*!
 * An abstract (pure virtual) class inheriting a "sf::RenderWindow" from the SFML library to behave as a window.
 * In addition the class also has other variable members every window wants to have in the game.
 */
class Base_Window//: public sf::RenderWindow
{
protected:
    //! A pure virtual function loading textures needed for the window
    /*!
     * Loads all the textures needed for the window. This is done by
     * creating a container in which the filename of the texture is connected to the texture object.
     * Since all windows want to load textures in a different way
     * the function needs to be overrided by each subwindow.
     */
    virtual void load_textures() = 0;

    //! A function member returning a texture based on specified filename of texture
    /*!
     * @param filename a std::string with the name of the texture file.
     * @return The texture
     */
    sf::Texture & get_texture(std::string const & filename);
    sf::Sprite bg_sprite;
    sf::Font font;
    std::map<std::string, sf::Texture> textures;
};

#endif //GAME38_GUI_H
