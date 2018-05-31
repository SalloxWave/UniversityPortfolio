//
// Created by alexanderjlinux on 11/29/16.
//

#include <fstream>

#include <SFML/Graphics/Shape.hpp>

#include "characterwindow.h"
#include "struct_point.h"

void Characterwindow::initialize()
{
    //Load character data
    std::ifstream json_file {"../resources/data/characters.json"};
    json_file >> char_data;

    //Load projectile data
    std::ifstream json_file2 {"../resources/data/projectiles.json"};
    json_file2 >> projectile_data;

    //Load all textures for character window
    load_textures();

    //Load and initialize character portrait images
    init_character_list();

    //Change predetermed selected characters for players
    player1_selected_id = std::begin(character_list)->first;
    player2_selected_id = std::end(character_list)->first;

    //Load font
    font.loadFromFile("../resources/font/Ubuntu-B.ttf");

    /* Initialize background sprite */
    bg_sprite.setTexture(get_texture("characterscreen_background.png"));
    Point bgPos {0,0};
    bg_sprite.setPosition(bgPos.x, bgPos.y);

    /* Initialize player 1 text */
    player1_text.setFont(font);
    player1_text.setString("P1");
    player1_text.setOrigin(player1_text.getLocalBounds().width/2, player1_text.getLocalBounds().height / 2);
    player1_text.setColor(sf::Color::White);

    /* Initialize player 2 text */
    player2_text.setFont(font);
    player2_text.setString("P2");
    player2_text.setOrigin(player2_text.getLocalBounds().width/2, player2_text.getLocalBounds().height/2);
    player2_text.setColor(sf::Color::White);

    /* Initialize player 1 background of the text */
    player1_text_bg.setSize(sf::Vector2f(player1_text.getLocalBounds().width + 40, player1_text.getLocalBounds().height + 40));
    player1_text_bg.setFillColor(sf::Color(57, 142, 209, 160));
    player1_text_bg.setOrigin(player1_text_bg.getLocalBounds().width/2, player1_text_bg.getLocalBounds().height/2);

    /* Initialize player 2 background of the text */
    player2_text_bg.setSize(sf::Vector2f(player2_text.getLocalBounds().width + 40, player2_text.getLocalBounds().height + 40));
    player2_text_bg.setFillColor(sf::Color(255, 0, 6, 100));
    player2_text_bg.setOrigin(player2_text_bg.getLocalBounds().width/2, player2_text_bg.getLocalBounds().height/2);

    /* Initialize player 1 controls text */
    player1_controls.setFont(font);
    player1_controls.setCharacterSize(20);
    player1_controls.setString("W\nS\nA\nD\nR\nT");
    player1_controls.setColor(sf::Color(185,185,185,255));
    player1_controls.setPosition(216, 182);

    /* Initialize player 2 controls text */
    player2_controls.setFont(font);
    player2_controls.setCharacterSize(20);
    player2_controls.setString("Up-arrow\nDown-arrow\nLeft-arrow\nRight-arrow\n.\n-");
    player2_controls.setColor(sf::Color(185,185,185,255));
    player2_controls.setPosition(1396, 182);

    /* Initialize player 1 selected character sprite */
    player1_selected_character.setTexture(*character_list[player1_selected_id].getTexture());
    player1_selected_character.setPosition(382, 622);

    /* Initialize player 2 selected character sprite */
    player2_selected_character.setTexture(*character_list[player2_selected_id].getTexture());
    player2_selected_character.setPosition(1000, 623);
    //Flip sprite
    player2_selected_character.setOrigin(player2_selected_character.getLocalBounds().width, 0);
    player2_selected_character.setScale(-1.f, 1.f);

    /* Initialize "Press Enter to Start" text */
    enterToPlay_opacity = 255;
    enterToPlay_increase_opacity = true;
    enterToPlay_text.setFont(font);
    enterToPlay_text.setCharacterSize(20);
    enterToPlay_text.setString("Press Enter when\neveryone is Ready");
    enterToPlay_text.setOrigin(enterToPlay_text.getLocalBounds().width/2, enterToPlay_text.getLocalBounds().height/2);
    enterToPlay_text.setPosition(800, 690);
    enterToPlay_text.setStyle(sf::Text::Italic);

    /* Initialize player 1 selected character name */
    player1_name.setFont(font);
    player1_name.setCharacterSize(30);
    player1_name.setPosition(70, 583);
    player1_name.setColor(sf::Color(185,185,185,255));

    /* Initialize player 1 selected character info with text size medium */
    player1_info_medium.setFont(font);
    player1_info_medium.setCharacterSize(20);
    player1_info_medium.setPosition(70, 625);
    player1_info_medium.setColor(sf::Color(185,185,185,255));

    /* Initialize player 1 selected character info with text size small */
    player1_info_small.setFont(font);
    player1_info_small.setCharacterSize(15);
    player1_info_small.setPosition(70, 710);
    player1_info_small.setColor(sf::Color(185,185,185,255));

    /* Initialize player 2 selected character name */
    player2_name.setFont(font);
    player2_name.setCharacterSize(30);
    player2_name.setPosition(1250, 583);
    player2_name.setColor(sf::Color(185,185,185,255));

    /* Initialize player 2 selected character info with text size medium */
    player2_info_medium.setFont(font);
    player2_info_medium.setCharacterSize(20);
    player2_info_medium.setPosition(1250, 625);
    player2_info_medium.setColor(sf::Color(185,185,185,255));

    /* Initialize player 2 selected character info with text size small */
    player2_info_small.setFont(font);
    player2_info_small.setCharacterSize(15);
    player2_info_small.setPosition(1250, 710);
    player2_info_small.setColor(sf::Color(185,185,185,255));
}

void Characterwindow::updateGraphics()
{
    //Either increase of decrease the opacity of the "Press Enter to Play" text
    if(enterToPlay_increase_opacity)
    { if(enterToPlay_opacity > 100) { enterToPlay_opacity -= 10; } else { enterToPlay_increase_opacity = false; }}
    else
    { if(enterToPlay_opacity < 180) { enterToPlay_opacity += 10; } else { enterToPlay_increase_opacity = true; }}
    enterToPlay_text.setColor(sf::Color(255, 255, 255, enterToPlay_opacity));

    //Set position of players text based on players' selected ids
    player1_text.setPosition(character_list[player1_selected_id].getPosition().x + 7, character_list[player1_selected_id].getPosition().y);
    player1_text_bg.setPosition(player1_text.getPosition().x + 2, player1_text.getPosition().y + 8);
    player2_text.setPosition(character_list[player2_selected_id].getPosition().x + 7, character_list[player2_selected_id].getPosition().y);
    player2_text_bg.setPosition(player2_text.getPosition().x + 2, player2_text.getPosition().y + 8);

    //Set texture of selected character based on player's selected character
    player1_selected_character.setTexture(*character_list[player1_selected_id].getTexture());
    player2_selected_character.setTexture(*character_list[player2_selected_id].getTexture());

    //Iterate through the character's data
    for (int i {0}; i < char_data.size(); ++i)
    {
        //Find correct character data based on player 1 selected character
        if (char_data[i]["id"] == getSelectedCharacterId(true))
        {
            //Update name of player
            player1_name.setString(char_data[i]["name"]);

            //Update medium sized info of player
            std::string medium_text {"Health: "};
            int max_health{char_data[i]["max_health"]};
            medium_text.append(std::to_string(max_health));
            int speed{char_data[i]["speed"]};
            medium_text.append("\nSpeed: ");
            medium_text.append(std::to_string(speed));
            medium_text.append("\nProjectiles:");
            player1_info_medium.setString(medium_text);

            //Update small sized info of player (projectile info):
            std::string small_text{};
            //Iterate through player's projectiles
            for (int i2{0}; i2 < char_data[i]["projectiles"].size(); ++i2)
            {
                //Get projectile id
                int projectile_id{ char_data[i]["projectiles"].at(i2) };
                //Iterate through the projectile data
                for (int i3{0}; i3 < projectile_data.size(); ++i3)
                {
                    //Find correct projectile based on projectile id
                    if (projectile_data[i3]["id"] == projectile_id)
                    {
                        //Add information about projectile
                        small_text.append(projectile_data[i3]["name"]);
                        small_text.append(" (");
                        small_text.append(projectile_data[i3]["state"]);
                        small_text.append(")");
                        small_text.append("\nDamage: ");
                        int proj_damage{projectile_data[i3]["damage"]};
                        small_text.append(std::to_string(proj_damage));
                        small_text.append("\nSpeed: ");
                        int proj_speed{projectile_data[i3]["speed"]};
                        small_text.append(std::to_string(proj_speed));
                        small_text.append("\n");
                        small_text.append("\n");
                    }
                }
            }
            //Update text
            player1_info_small.setString(small_text);
        }

        //Find correct character data based on player 2 selected character
        if (char_data[i]["id"] == getSelectedCharacterId(false))
        {
            //Update name of player
            player2_name.setString(char_data[i]["name"]);

            //Update medium sized info of player
            std::string medium_text{"Health: "};
            int max_health{char_data[i]["max_health"]};
            medium_text.append(std::to_string(max_health));
            int speed{char_data[i]["speed"]};
            medium_text.append("\nSpeed: ");
            medium_text.append(std::to_string(speed));
            medium_text.append("\nProjectiles:");
            player2_info_medium.setString(medium_text);

            //Update small sized info of player (projectile info):
            std::string small_text{};
            //Iterate through player's projectiles
            for (int i2{0}; i2 < char_data[i]["projectiles"].size(); ++i2)
            {
                //Get projectile id
                int projectile_id{ char_data[i]["projectiles"].at(i2) };
                //Iterate through the projectile data
                for (int i3 {0}; i3 < projectile_data.size(); ++i3)
                {
                    //Find correct projectile based on projectile id
                    if (projectile_data[i3]["id"] == projectile_id)
                    {
                        //Add information about projectile
                        small_text.append(projectile_data[i3]["name"]);
                        small_text.append(" (");
                        small_text.append(projectile_data[i3]["state"]);
                        small_text.append(")");
                        small_text.append("\nDamage: ");
                        int proj_damage{projectile_data[i3]["damage"]};
                        small_text.append(std::to_string(proj_damage));
                        small_text.append("\nSpeed: ");
                        int proj_speed{projectile_data[i3]["speed"]};
                        small_text.append(std::to_string(proj_speed));
                        small_text.append("\n");
                        small_text.append("\n");
                    }
                }
            }
            //Update text
            player2_info_small.setString(small_text);
        }
    }

    //Clear the window
    clear();
    //Draw updated components to screen
    draw(bg_sprite);
    //Draw all selectable characters
    for (auto it = std::begin(character_list); it != std::end(character_list); ++it)
    {
        draw(it->second);
    }
    draw(enterToPlay_text);
    draw(player1_text_bg);
    draw(player2_text_bg);
    draw(player1_text);
    draw(player2_text);
    draw(player1_controls);
    draw(player2_controls);
    draw(player1_selected_character);
    draw(player2_selected_character);
    draw(player1_name);
    draw(player1_info_medium);
    draw(player1_info_small);
    draw(player2_name);
    draw(player2_info_medium);
    draw(player2_info_small);
    //Display updated components
    display();
}

void Characterwindow::changeCharacter(Direction direction, bool is_player1 /*= true*/)
{
    //If character can be changed
    if (can_change_character(direction, is_player1))
    {
        switch(direction)
        {
            //Change selected character id for player based on direction
            case Direction::Up:
                if (is_player1) { player1_selected_id -= max_row_size; }
                else { player2_selected_id -= max_row_size; }
                break;
            case Direction::Down:
                if (is_player1) { player1_selected_id += max_row_size; }
                else { player2_selected_id += max_row_size; }
                break;
            case Direction::Left:
                if (is_player1) { player1_selected_id -= 1; }
                else { player2_selected_id -= 1; }
                break;
            case Direction::Right:
                if (is_player1) { player1_selected_id += 1; }
                else { player2_selected_id += 1; }
                break;
        }
    }
}

unsigned int Characterwindow::getSelectedCharacterId(bool is_player1 /* = true*/) const
{
    return is_player1 ? player1_selected_id : player2_selected_id;
}

bool Characterwindow::can_change_character(Direction direction, bool is_player1 /* = true */)
{
    int new_id;

    //Get distance from image size of a character image
    switch (direction)
    {
        //Change selected character id for player based on direction
        //...and return if the character id is a valid character id or not
        case Direction::Up:
            new_id = (is_player1 ? player1_selected_id : player2_selected_id) - max_row_size;
            break;
        case Direction::Left:
            new_id = (is_player1 ? player1_selected_id : player2_selected_id) - 1;
            break;
        case Direction::Down:
            new_id = (is_player1 ? player1_selected_id : player2_selected_id) + max_row_size;
            break;
        case Direction::Right:
            new_id = (is_player1 ? player1_selected_id : player2_selected_id) + 1;
            break;
    }
    //Check if the id can be found in possible character ids
    return character_list.count(new_id) == 1;
}

void Characterwindow::init_character_list()
{
    int char_id;
    int y {0};
    int x {0};
    //Iterate through character data
    for (int i {0}; i < char_data.size(); ++i)
    {
        //Get id of character from file
        char_id = char_data[i]["id"];
        //Load the texture based on character's image
        std::string texture_src = char_data[i]["portrait_src"];
        //Connect character id with character's portrait sprite
        character_list[char_id].setTexture(get_texture(texture_src));

        //Set position of sprite
        character_list[char_id].setOrigin(172 / 2, 140 / 2);

        //Check if you should go to next row
        if (x == max_row_size)
        {
            //Go to next row
            ++y;
            x = 0;
        }
        //Set size and position of character sprite
        int x_extra = 420;
        int y_extra = 155;
        character_list[char_id].setScale(0.7f, 0.7f);
        character_list[char_id].setPosition(x_extra + (x * 185) + character_list[char_id].getOrigin().x,
                                        y_extra + (character_list[char_id].getOrigin().y + (y * 182)));
        //Go to next column
        ++x;
    }
}

void Characterwindow::load_textures()
{
    std::string texture_src;
    //Load backgroundimage
    textures["characterscreen_background.png"].loadFromFile("../resources/img/characterscreen_background.png");

    //Load character portraits
    std::ifstream char_file {"../resources/data/characters.json"};
    json char_data;
    char_file >> char_data;
    for (int i {0}; i < char_data.size(); ++i)
    {
        texture_src = char_data[i]["portrait_src"];
        textures[texture_src].loadFromFile("../resources/img/player/" + texture_src);
    }

    //Load level portraits
    std::ifstream level_file {"../resources/data/levels.json"};
    json level_data;
    level_file >> level_data;
    for (int i {0}; i < level_data.size(); ++i)
    {
        texture_src = level_data[i]["portrait_src"];
        textures[texture_src].loadFromFile("../resources/img/level/" + texture_src);
    }
}