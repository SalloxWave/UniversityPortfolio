//
// Created by alexanderjlinux on 11/29/16.
//

#include "base_window.h"

sf::Texture & Base_Window::get_texture(std::string const & filename)
{
    return textures[filename];
}
