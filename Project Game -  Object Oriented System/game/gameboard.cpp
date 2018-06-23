//
// Created by alexanderjlinux on 11/29/16.
//

#include <string>
#include <fstream>

#include "gameboard.h"

void Gameboard::initialize(Level const & level, Player const & player1, Player const & player2)
{
    //Load textures needed for a gameboard
    load_textures();

    /* Initialize background sprite */
    bg_sprite.setTexture(get_texture(level.getBgSource()));

    /*Initialize player sprite for player 1*/
    player1_sprite.setTexture(get_texture(player1.getSpriteSource()));
    player1_sprite.setTextureRect(sf::IntRect(190, 0, 190, 220));
    //Move origin upwards based on player height to make y position be on the feet of the player
    player1_sprite.setOrigin(0, player1.getHeight());
    player1_sprite.setPosition(player1.getPosition().x, player1.getPosition().y);

    /*Initialize player sprite for player 2*/
    player2_sprite.setTexture(get_texture(player2.getSpriteSource()));
    player2_sprite.setTextureRect(sf::IntRect(190, 0, 190, 220));
    //Move origin upwards based on player height to make y position be on the feet of the player
    player2_sprite.setOrigin(0 + player2.getWidth(), player2.getHeight());
    //Flip player 2
    player2_sprite.scale(-1.f,1.f);
    player2_sprite.setPosition(player2.getPosition().x, player2.getPosition().y);

    //Load the font
    font.loadFromFile("../resources/font/Ubuntu-B.ttf");

    /* Initialize name of level */
    level_name.setFont(font);
    level_name.setString(level.getName());
    level_name.setOrigin(level_name.getLocalBounds().width/2, 0);
    level_name.setPosition(getSize().x/2, 50);

    /* Initialize portait for player 1 */
    player1_portrait.setTexture(get_texture(player1.getPortraitSource()));
    player1_portrait.setPosition(20, 20);
    //player1_portait.setScale(0.7,0.7);
    player1_portrait.setOrigin(0, 0);

    /* Initialize portait for player 2 */
    player2_portrait.setTexture(get_texture(player2.getPortraitSource()));
    player2_portrait.setPosition(getSize().x-20, 20);
    player2_portrait.setScale(-1,1);
    player2_portrait.setOrigin(0, 0);

    /* Initialize name text for player 1 */
    player1_name.setFont(font);
    player1_name.setString(player1.getName());
    player1_name.setPosition(20, player1_portrait.getPosition().y + player1_portrait.getLocalBounds().height + 10);

    /* Initialize name text for player 2 */
    player2_name.setFont(font);
    player2_name.setString(player2.getName());
    player2_name.setOrigin(player2_name.getLocalBounds().width, 0);
    player2_name.setPosition(getSize().x - 20, player2_portrait.getPosition().y + player2_portrait.getLocalBounds().height + 10);

    /* Initialize health information text for player 1 */
    player1_life.setFont(font);
    player1_life.setString(std::to_string(player1.getCurrentHealth()) + "/"
                           + std::to_string(player1.getMaxHealth()));
    player1_life.setPosition(20, player1_name.getPosition().y + player1_name.getLocalBounds().height + 10);

    /* Initialize health information text for player 2 */
    player2_life.setFont(font);
    player2_life.setString(std::to_string(player2.getCurrentHealth()) + "/"
                           + std::to_string(player2.getMaxHealth()));
    player2_life.setOrigin(player2_life.getLocalBounds().width, 0);
    player2_life.setPosition(getSize().x - 20, player2_name.getPosition().y + player2_name.getLocalBounds().height + 10);
}

void Gameboard::updateGraphics(Level const & level, Player const & player1, Player const & player2)
{
    //Update position of player sprites
    player1_sprite.setPosition(player1.getPosition().x, player1.getPosition().y);
    player2_sprite.setPosition(player2.getPosition().x, player2.getPosition().y);

    //Update player 1's texture based on player's state
    switch (player1.getState())
    {
        case Player_State::Jump:
            player1_sprite.setTextureRect(sf::IntRect(0, 440, 190, 220)); // Left Top Width Height
            break;
        case Player_State::Duck:
            player1_sprite.setTextureRect(sf::IntRect(190, 440, 190, 220)); // Left Top Width Height
            break;
        case Player_State::ShootLow:
            player1_sprite.setTextureRect(sf::IntRect(220, 220, 190, 220)); // Left Top Width Height
            break;
        case Player_State::ShootHigh:
            player1_sprite.setTextureRect(sf::IntRect(0, 220, 190, 220)); // Left Top Width Height
            break;
        default:
            player1_sprite.setTextureRect(sf::IntRect(190, 0, 190, 220)); // Left Top Width Height
            break;
    }

    //Update player 2's texture based on player's state
    switch (player2.getState())
    {
        case Player_State::Jump:
            player2_sprite.setTextureRect(sf::IntRect(0, 440, 190, 220)); // Left Top Width Height
            break;
        case Player_State::Duck:
            player2_sprite.setTextureRect(sf::IntRect(190, 440, 190, 220)); // Left Top Width Height
            break;
        case Player_State::ShootLow:
            player2_sprite.setTextureRect(sf::IntRect(220, 220, 190, 220)); // Left Top Width Height
            break;
        case Player_State::ShootHigh:
            player2_sprite.setTextureRect(sf::IntRect(0, 220, 190, 220)); // Left Top Width Height
            break;
        default:
            player2_sprite.setTextureRect(sf::IntRect(190, 0, 190, 220)); // Left Top Width Height
            break;
    }

    //Update life text of players
    player1_life.setString(std::to_string(player1.getCurrentHealth()) + "/"
                               + std::to_string(player1.getMaxHealth()));
    player2_life.setString(std::to_string(player2.getCurrentHealth()) + "/"
                               + std::to_string(player2.getMaxHealth()));

    //Update color on player 1 health
    if (player1.getCurrentHealth() < player1.getMaxHealth() / 4)
    { player1_life.setColor(sf::Color(255, 0, 0, 255)); } // Set red color if player is low
    else if (player1.getCurrentHealth() < player1.getMaxHealth() / 1.3)
    { player1_life.setColor(sf::Color(255, 255, 0, 255)); } // Set yellow color if player is at medium health
    else
    { player1_life.setColor(sf::Color(0, 255, 0, 255)); } // Set green color if player is healty
    //Update color on player 2 health
    if (player2.getCurrentHealth() < player2.getMaxHealth() / 4)
    { player2_life.setColor(sf::Color(255, 0, 0, 255)); } // Set red color if player is low
    else if (player2.getCurrentHealth() < player2.getMaxHealth() / 1.3)
    { player2_life.setColor(sf::Color(255, 255, 0, 255)); } // Set yellow color if player is at medium health
    else
    { player2_life.setColor(sf::Color(0, 255, 0, 255)); } // Set green color if player is healty

    //Update projectile sprites based on player 1 and player 2's active projectiles
    active_projectiles.clear();
    for (Projectile proj: player1.getActiveProjectiles())
    {
        sf::Sprite proj_sprite;
        //Set texture of projectile sprite
        std::string sprite_src = proj.getSpriteSource();
        proj_sprite.setTexture(get_texture(sprite_src));
        //Set position of projectile sprite
        proj_sprite.setPosition(proj.getPosition().x, proj.getPosition().y);
        active_projectiles.push_back(proj_sprite);
    }
    for (Projectile proj: player2.getActiveProjectiles())
    {
        sf::Sprite proj_sprite;
        //Set texture of projectile sprite
        std::string sprite_src = proj.getSpriteSource();
        proj_sprite.setTexture(get_texture(sprite_src));
        //Set position of projectile sprite
        proj_sprite.setPosition(proj.getPosition().x, proj.getPosition().y);
        active_projectiles.push_back(proj_sprite);
    }

    //Clear the window
    clear();
    //Draw updated components to screen
    draw(bg_sprite);
    draw(player1_sprite);
    draw(player2_sprite);
    draw(player1_portrait);
    draw(player2_portrait);
    //Draw active projectiles
    for (int i {0}; i < active_projectiles.size(); ++i)
    {
        draw(active_projectiles[i]);
    }
    draw(player1_life);
    draw(player2_life);
    draw(player1_name);
    draw(player2_name);
    //Display updated components
    display();
}

void Gameboard::load_textures()
{
    std::string sprite_src;
    std::string portrait_src;

    //Load character textures
    std::ifstream char_file {"../resources/data/characters.json"};
    json char_data;
    char_file >> char_data;
    for (int i {0}; i < char_data.size(); ++i)
    {
        sprite_src = char_data[i]["sprite_src"];
        portrait_src = char_data[i]["portrait_src"];
        textures[sprite_src].loadFromFile("../resources/img/player/" + sprite_src);
        textures[portrait_src].loadFromFile("../resources/img/player/" + portrait_src);
    }

    //Load projectile textures
    std::ifstream proj_file {"../resources/data/projectiles.json"};
    json proj_data;
    proj_file >> proj_data;
    for (int i {0}; i < proj_data.size(); ++i)
    {
        sprite_src = proj_data[i]["sprite_src"];
        textures[sprite_src].loadFromFile("../resources/img/projectile/" + sprite_src);
    }

    //Load level background textures
    std::ifstream level_file {"../resources/data/levels.json"};
    json level_data;
    level_file >> level_data;
    for (int i {0}; i < level_data.size(); ++i)
    {
        sprite_src = level_data[i]["bg_src"];
        textures[sprite_src].loadFromFile("../resources/img/level/" + sprite_src);
    }
}