//
// Created by alexanderlinux on 12/9/16.
//

#ifndef GAME38_GAME_H
#define GAME38_GAME_H

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Sleep.hpp>

#include "level.h"
#include "player.h"
#include "projectile.h"
#include "enum_player_action.h"
#include "enum_player_state.h"
#include "enum_projectile_state.h"
#include "struct_point.h"

#include "main_menu.h"
#include "characterwindow.h"
#include "gameboard.h"
#include "winning_window.h"
#include "enum_window_state.h"
#include "enum_winning_option.h"

//! A class for running the game "game38"
/*!
 * A class representing a playable "game38".
 * The only thing needed to play is to call function "run()".
 */
class Game38
{
public:
    //! A default constructor
    /*!
     * Constructor uses compiler's default way of constructing class.
     */
    Game38() = default;

    //! Run the game
    /*!
     * Start and run the game while something doesn't tell the game to quit.
     */
    void run();
private:
    //! Runs a main menu window
    /*!
     * Create objects needed for a main menu
     */
    void run_main_menu();

    //! Runs a character selection window
    /*!
     * Create objects needed for a character selection window
     */
    void run_character_window();

    //! Runs a gameboard window
    /*!
     * Create objects needed for a main menu window
     */
    void run_gameboard();

    unsigned int player1_selected_id;
    unsigned int player2_selected_id;
    Window_State window_state { Window_State::MainMenu };
    sf::Event event;
    sf::Keyboard::Key key_code { sf::Keyboard::Unknown };
    sf::VideoMode window_size;
    sf::Clock loop_clock;
    sf::Clock cooldown_clock;
    sf::Time delta_time;
    float distance;
    bool quit_game { false };
};


#endif //GAME38_GAME_H
