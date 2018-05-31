//
// Created by alexanderjlinux on 11/29/16.
//

#include <fstream>

#include <SFML/Graphics.hpp>
#include "json.hpp"
using json = nlohmann::json;

#include "main_menu.h"

#include "struct_point.h"

void Main_Menu::initialize()
{
    //Load the textures for the main menu
    load_textures();

    //Set selected menu choice to be at the top
    selected_index = 0;

    /* Initialize background sprite */
    bg_sprite.setTexture(get_texture("main_menu_background.png"));
    Point bgPos {0,0};
    bg_sprite.setPosition(bgPos.x, bgPos.y);

    /* Initialize game logo */
    logo_img.setTexture(get_texture("logo.png"));
    Point logoPos {640,190};
    logo_img.setPosition(logoPos.x, logoPos.y);
    logo_img.scale(0.15f, 0.15f);

    /* Initialize start button */
    btn_start_game.setTexture(get_texture("menu.png"));
    Point startPos {690,390};
    btn_start_game.setPosition(startPos.x, startPos.y);
    btn_start_game.scale(0.2f, 0.2f);

    /* Initialize exit button */
    btn_exit_game.setTexture(get_texture("menu.png"));
    Point exitPos {690,460};
    btn_exit_game.setPosition(exitPos.x, exitPos.y);
    btn_exit_game.scale(0.2f, 0.2f);

    /* Initialize credits text */
    font.loadFromFile("../resources/font/Ubuntu-B.ttf");
    credits.setFont(font);
    credits.setString("Credits: Alexander Jonsson, Joakim Johansson");
    credits.setCharacterSize(18); // in pixels, not points!
    credits.setColor(sf::Color(255,255,255,100));
    credits.setStyle(sf::Text::Italic);
    credits.setPosition(590.0f, 830.0f);

    /* Initialize info text*/
    selected_info.setFont(font);
    selected_info.setCharacterSize(14); // in pixels, not points!
    selected_info.setColor(sf::Color(255,255,255,120));
    selected_info.setPosition(690.0f, 530.0f);
}

void Main_Menu::updateGraphics()
{
    //Set texture from texture file based on selected index
    btn_start_game.setTextureRect(sf::IntRect(0, (selected_index==0?300:0), 1000, 280));
    btn_exit_game.setTextureRect(sf::IntRect(1000, (selected_index==1?300:0), 1000, 280));

    //Change selected info text based on the selected menu choice
    if(selected_index == 0)
    {
        selected_info.setString("Press Enter to Play Game38");
    }
    else
    {
        selected_info.setString("Press Enter to Exit Game38");
    }

    //Clear the window
    clear();
    //Draw updated components
    draw(bg_sprite);
    draw(logo_img);
    draw(btn_start_game);
    draw(btn_exit_game);
    draw(credits);
    draw(selected_info);
    //Display updated components
    display();
}

void Main_Menu::goUp()
{
    //User is already at the top of the list
    if(selected_index == 1)
    {
        selected_index -= 1;
    }
}

void Main_Menu::goDown()
{
    //User is already at the bottom of the list
    if(selected_index == 0)
    {
        selected_index += 1;
    }
}

Menu_Choice Main_Menu::getMenuChoice()
{
    Menu_Choice menu_choice;
    //Get menu choice based on selected menu index
    switch (selected_index)
    {
        case 0:
            menu_choice = Menu_Choice::StartGame;
            break;
        case 1:
            menu_choice = Menu_Choice::ExitGame;
            break;
        default:
            menu_choice = Menu_Choice::StartGame;
            break;
    }
    return menu_choice;
}

void Main_Menu::load_textures()
{
    //Load background texture
    textures["main_menu_background.png"].loadFromFile("../resources/img/main_menu_background.png");

    //Load logo texture
    textures["logo.png"].loadFromFile("../resources/img/logo.png");

    //Load menu texture
    textures["menu.png"].loadFromFile("../resources/img/menu.png");
}
