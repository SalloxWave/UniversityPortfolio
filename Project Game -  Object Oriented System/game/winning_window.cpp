//
// Created by alejo720 on 2016-12-14.
//

#include "winning_window.h"

void Winning_Window::initialize(Player const & winner)
{
    //Load all textures for winning window
    load_textures();

    //Set selected menu option to be at the top
    selected_index = 0;

    //Load the font
    font.loadFromFile("../resources/font/Ubuntu-B.ttf");

    /* Initialize background sprite */
    bg_sprite.setTexture(get_texture("winning_window_bg.png"));
    Point bg_pos {0,0};
    bg_sprite.setPosition(bg_pos.x, bg_pos.y);

    /* Initialize winner_text text */
    winner_text.setFont(font);
    std::string winning_text {"Player "};
    winning_text.append((winner.isPlayerOne()?"1: ":"2: "));
    winning_text.append(winner.getName());
    winning_text.append(" WON!");
    //Set color of winner_text text based on if player 1 or 2 won
    winner_text.setColor((winner.isPlayerOne()?sf::Color(57, 142, 209, 160):sf::Color(255, 0, 6, 100)));
    winner_text.setString(winning_text);
    winner_text.setCharacterSize(40);
    winner_text.setOrigin(winner_text.getLocalBounds().width / 2, 0);
    winner_text.setPosition(getSize().x/2,140);

    /* Initialize play again text */
    play_again.setFont(font);
    play_again.setString("Play again");
    play_again.setCharacterSize(30);
    play_again.setOrigin(play_again.getLocalBounds().width / 2, 0);
    play_again.setPosition(getSize().x / 2, 210);

    /* Initialize change character text */
    change_character.setFont(font);
    change_character.setString("Change character");
    change_character.setCharacterSize(30);
    change_character.setOrigin(change_character.getLocalBounds().width/2, 0);
    change_character.setPosition(getSize().x / 2,
                                      play_again.getPosition().y + change_character.getLocalBounds().height + 10);

    /* Initialize exit game text */
    exit_game.setFont(font);
    exit_game.setString("Exit game");
    exit_game.setCharacterSize(30);
    exit_game.setOrigin(exit_game.getLocalBounds().width/2, 0);
    exit_game.setPosition(getSize().x / 2,
                               change_character.getPosition().y + exit_game.getLocalBounds().height + 10);
}

void Winning_Window::updateGraphics()
{
    //Update color of menu options texts based on selected winning option
    switch( getWinningOption() )
    {
        case Winning_Option::PlayAgain:
            play_again.setColor(sf::Color::Yellow);
            change_character.setColor(sf::Color::White);
            exit_game.setColor(sf::Color::White);
            break;
        case Winning_Option::ChangeCharacter:
            change_character.setColor(sf::Color::Yellow);
            play_again.setColor(sf::Color::White);
            exit_game.setColor(sf::Color::White);
            break;
        case Winning_Option::ExitGame:
            exit_game.setColor(sf::Color::Yellow);
            play_again.setColor(sf::Color::White);
            change_character.setColor(sf::Color::White);
            break;
    }

    //Clear the window
    clear();
    //Draw updated components to the window
    draw(bg_sprite);
    draw(winner_text);
    draw(play_again);
    draw(change_character);
    draw(exit_game);
    //Display updated components to the window
    display();
}

Winning_Option Winning_Window::getWinningOption()
{
    //Get winning option based on selected index
    Winning_Option winning_option;
    switch(selected_index)
    {
        case 0:
            winning_option = Winning_Option::PlayAgain;
            break;
        case 1:
            winning_option = Winning_Option::ChangeCharacter;
            break;
        case 2:
            winning_option = Winning_Option::ExitGame;
            break;
        default:
            winning_option = Winning_Option::PlayAgain;
            break;
    }
    return winning_option;
}

void Winning_Window::goUp()
{
    //Go up if selected option is not at the top
    if (selected_index != 0)
    {
        selected_index-=1;
    }
}

void Winning_Window::goDown()
{
    //Go down if selected option is not at the bottom
    if (selected_index != 2)
    {
        selected_index+=1;
    }
}

void Winning_Window::load_textures()
{
    //Load background texture
    textures["winning_window_bg.png"].loadFromFile("../resources/img/winning_window_bg.png");
}