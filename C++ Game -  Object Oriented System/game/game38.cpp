//
// Created by alexanderlinux on 12/9/16.
//

#include "game38.h"

#include <iostream>

using namespace std;

void Game38::run()
{
    //Set window size for all windows
    window_size.width = 1580;
    window_size.height = 880;

    //Begin game in the main menu
    window_state = Window_State::MainMenu;

    //Run game as long as something tells to quit game
    while (!quit_game)
    {
        //Find which window to display and run
        switch(window_state)
        {
            case Window_State::MainMenu:
                run_main_menu();
                break;
            case Window_State::Characterwindow:
                run_character_window();
                break;
            case Window_State::Gameboard:
                run_gameboard();
                break;
        }
    }
}

void Game38::run_main_menu()
{
    //Open and initialize a main menu window
    Main_Menu main_menu { window_size, "Game38 - Main Menu" };
    main_menu.setVerticalSyncEnabled(true);
    main_menu.initialize();

    while(main_menu.isOpen())
    {
        //Poll event from main menu's event queue
        while(main_menu.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    main_menu.close();
                    quit_game = true;
                    break;
                default:
                    break;
            }
        }

        //Check for keyboard clicks
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            //Go up in the main menu
            main_menu.goUp();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            //Go down in the main menu
            main_menu.goDown();
        }
        //User selected a menu choice
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
        {
            switch (main_menu.getMenuChoice())
            {
                case Menu_Choice::StartGame:
                    //Change to character selection window state
                    window_state = Window_State::Characterwindow;
                    break;
                case Menu_Choice::ExitGame:
                    quit_game = true;
                    break;
            }
            //Close the main menu and destroy all attached resources
            main_menu.close();
        }
        main_menu.updateGraphics();
    }
}

void Game38::run_character_window()
{
    //Open and initialize a character selection window
    Characterwindow characterwindow { window_size, "Character screen, choose wisely!"};
    characterwindow.setVerticalSyncEnabled(true);
    characterwindow.initialize();

    while(characterwindow.isOpen())
    {
        //Poll event from character selection window's event queue
        while(characterwindow.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    characterwindow.close();
                    quit_game = true;
                    break;
                case sf::Event::KeyPressed:
                    key_code = event.key.code;
                default:
                    break;
            }
        }
        //Check which was pressed
        switch(key_code)
        {
            //Change character for either player 1 or 2
            case sf::Keyboard::W:
                characterwindow.changeCharacter(Direction::Up, true);
                break;
            case sf::Keyboard::A:
                characterwindow.changeCharacter(Direction::Left, true);
                break;
            case sf::Keyboard::S:
                characterwindow.changeCharacter(Direction::Down, true);
                break;
            case sf::Keyboard::D:
                characterwindow.changeCharacter(Direction::Right, true);
                break;
            case sf::Keyboard::Up:
                characterwindow.changeCharacter(Direction::Up, false);
                break;
            case sf::Keyboard::Left:
                characterwindow.changeCharacter(Direction::Left, false);
                break;
            case sf::Keyboard::Down:
                characterwindow.changeCharacter(Direction::Down, false);
                break;
            case sf::Keyboard::Right:
                characterwindow.changeCharacter(Direction::Right, false);
                break;
            //Players are done selecting characters
            case sf::Keyboard::Return:
                //Get player's character ids
                player1_selected_id = characterwindow.getSelectedCharacterId(true);
                player2_selected_id = characterwindow.getSelectedCharacterId(false);

                //Close the main menu and destroy all attached resources
                characterwindow.close();
                //Change to main menu window state
                window_state = Window_State::Gameboard;
                break;
            case sf::Keyboard::Escape:
                //Close the character selection window and destroy all attached resources
                characterwindow.close();
                //Change to main menu window state
                window_state = Window_State::MainMenu;
                break;
            default:
                break;
        }
        characterwindow.updateGraphics();

        //Reset key event
        key_code = sf::Keyboard::Unknown;
    }
}

void Game38::run_gameboard()
{
    //Create level
    Level level { 1 };

    //Create player one and two based on their selected characters
    Player player1 { player1_selected_id, true };
    Player player2 { player2_selected_id, false };

    //Add players to level
    level.addPlayer(player1);
    level.addPlayer(player2);

    //Open and initialize gameboard window
    Gameboard gameboard{ window_size, "Gameboard, fight!" };
    gameboard.setVerticalSyncEnabled(true);
    gameboard.initialize(level, player1, player2);

    while(gameboard.isOpen())
    {
        //Poll event from gameboard's event queue
        while (gameboard.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    //Close the gameboard window and destroy all attached resources
                    gameboard.close();
                    quit_game = true;
                    break;
                default:
                    break;
            }
        }

        //Get time it took for one iteration
        delta_time = loop_clock.restart();

        //Iterate through player 1's set controls
        std::map<sf::Keyboard::Key, Player_Action> player1_controls { player1.get_controls() };
        bool player1_pressed { false };
        for (auto it = std::begin(player1_controls); it != std::end(player1_controls); ++it)
        {
            //Check if the key was pressed
            if(sf::Keyboard::isKeyPressed(it->first))
            {
                player1_pressed = true;
                //Get the action connected to the key
                switch (it->second)
                {
                    case Player_Action::Left:
                        //Only move if player is at normal state
                        if (player1.getState() == Player_State::Normal)
                        {
                            //Move a distance to the left based on player speed
                            distance = static_cast<float>(delta_time.asMilliseconds() * player1.getSpeed() / 2);
                            player1.move(Direction::Left, static_cast<unsigned int>(distance));
                        }
                        break;
                    case Player_Action::Right:
                        //Only move if player is at normal state
                        if (player1.getState() == Player_State::Normal)
                        {
                            //Move a distance to the right based on player speed
                            distance = static_cast<float>(delta_time.asMilliseconds() * player1.getSpeed() / 2);
                            player1.move(Direction::Right, static_cast<unsigned int>(distance));
                        }
                        break;
                    case Player_Action::Jump:
                        //Make player jump
                        player1.changeState(Player_State::Jump);
                        break;
                    case Player_Action::Duck:
                        //Make player duck
                        player1.changeState(Player_State::Duck);
                        break;
                    case Player_Action::ShootLow:
                    {
                        //Only shoot if player is in normal state
                        if (player1.getState() == Player_State::Normal)
                        {
                            player1.changeState(Player_State::ShootLow);
                            //Get projectile id player's low shot
                            unsigned int proj_id = player1.getProjectileId(Projectile_State::Low);
                            //Check if wanted projectile is ready to use
                            if (player1.readyToUse(proj_id, cooldown_clock.getElapsedTime()))
                            {
                                player1.shoot(proj_id, cooldown_clock.getElapsedTime());
                            }
                        }
                        break;
                    }
                    case Player_Action::ShootHigh:
                    {
                        //Only shoot if player is in normal state
                        if (player1.getState() == Player_State::Normal)
                        {
                            player1.changeState(Player_State::ShootHigh);
                            //Get projectile id player's high shot
                            unsigned int proj_id = player1.getProjectileId(Projectile_State::High);
                            //Check if wanted projectile is ready to use
                            if (player1.readyToUse(proj_id, cooldown_clock.getElapsedTime()))
                            {
                                player1.shoot(proj_id, cooldown_clock.getElapsedTime());
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        //Change state to normal if player didn't press anything or player shot
        if (!player1_pressed)
        {
            player1.changeState(Player_State::Normal);
        }

        //Iterate through player 1's set controls
        std::map<sf::Keyboard::Key, Player_Action> player2_controls { player2.get_controls() };
        bool player2_pressed { false };
        for (auto it = std::begin(player2_controls); it != std::end(player2_controls); ++it)
        {
            //Check if the key was pressed
            if(sf::Keyboard::isKeyPressed(it->first))
            {
                player2_pressed = true;
                //Get the action connected to the key
                switch (it->second)
                {
                    case Player_Action::Left:
                        //Only move if player is at normal state
                        if (player2.getState() == Player_State::Normal)
                        {
                            //Move a distance to the left based on player speed
                            distance = static_cast<float>(delta_time.asMilliseconds() * player2.getSpeed() / 2);
                            player2.move(Direction::Left, static_cast<unsigned int>(distance));
                        }
                        break;
                    case Player_Action::Right:
                        //Only move if player is at normal state
                        if (player2.getState() == Player_State::Normal)
                        {
                            //Move a distance to the right based on player speed
                            distance = static_cast<float>(delta_time.asMilliseconds() * player2.getSpeed() / 2);
                            player2.move(Direction::Right, static_cast<unsigned int>(distance));
                        }
                        break;
                    case Player_Action::Jump:
                        //Make player jump
                        player2.changeState(Player_State::Jump);
                        break;
                    case Player_Action::Duck:
                        //Make player duck
                        player2.changeState(Player_State::Duck);
                        break;
                    case Player_Action::ShootLow:
                    {
                        //Only shoot if player is in normal state
                        if (player2.getState() == Player_State::Normal)
                        {
                            player2.changeState(Player_State::ShootLow);
                            //Get projectile id player's low shot
                            unsigned int proj_id = player2.getProjectileId(Projectile_State::Low);
                            //Check if wanted projectile is ready to use
                            if (player2.readyToUse(proj_id, cooldown_clock.getElapsedTime()))
                            {
                                player2.shoot(proj_id, cooldown_clock.getElapsedTime());
                            }
                        }
                        break;
                    }
                    case Player_Action::ShootHigh:
                    {
                        //Only shoot if player is in normal state
                        if (player2.getState() == Player_State::Normal)
                        {
                            player2.changeState(Player_State::ShootHigh);
                            //Get projectile id player's high shot
                            unsigned int proj_id = player2.getProjectileId(Projectile_State::High);
                            //Check if wanted projectile is ready to use
                            if (player2.readyToUse(proj_id, cooldown_clock.getElapsedTime()))
                            {
                                player2.shoot(proj_id, cooldown_clock.getElapsedTime());
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        //Change state to normal if player didn't press anything
        if (!player2_pressed)
        {
            player2.changeState(Player_State::Normal);
        }

        //Iterate through player 1's projectile
        for (int i {0}; i < player1.getActiveProjectiles().size(); ++i)
        {
            Projectile & proj = player1.getActiveProjectiles()[i];
            //Check if the projectile collides with the level (wall of the level)
            if (proj.collides(level))
            {
                player1.removeProjectile(i);
            }
            //Check if opposing player 2 collides with the projectile
            else if (player2.collides(proj))
            {
                //Damage player 2 by the projectile damage
                player2.giveDamage(proj.getDamage());
                //Remove player 1's projectile
                player1.removeProjectile(i);
            }
            else
            {
                //Move projectile based on speed of projectile
                distance = static_cast<float>(delta_time.asMilliseconds() * proj.getSpeed() / 2);
                proj.move(proj.getDirection(), static_cast<unsigned int>(distance));
            }
        }

        //Iterate through player 2's projectile
        for (int i {0}; i < player2.getActiveProjectiles().size(); ++i)
        {
            Projectile & proj = player2.getActiveProjectiles()[i];
            //Check if the projectile collides with the level (wall of the level)
            if (proj.collides(level))
            {
                //Remove player 2's projectile
                player2.removeProjectile(i);
            }
            //Check if opposing player 1 collides with the projectile
            else if (player1.collides(proj))
            {
                //Damage player 1 by the projectile damage
                player1.giveDamage(proj.getDamage());
                //Remove player 2's projectile
                player2.removeProjectile(i);
            }
            else
            {
                //Move projectile based on speed of projectile
                distance = static_cast<float>(delta_time.asMilliseconds() * proj.getSpeed() / 2);
                proj.move(proj.getDirection(), static_cast<unsigned int>(distance));
            }
        }

        //Update graphics of board based on game objects
        gameboard.updateGraphics(level, player1, player2);

        //Check if any player has died
        if (player1.isDead() || player2.isDead())
        {
            //Create winning box without borders
            Winning_Window winning_window { sf::VideoMode(790, 440),
                                            "Winning screen, what do you want to do next?",
                                            sf::Style::None};

            //Initialize winning box based on the winner
            if (player1.isDead()) { winning_window.initialize(player2); }
            else { winning_window.initialize(player1); }

            while (winning_window.isOpen())
            {
                //Poll event from winning box's event queue
                while(winning_window.pollEvent(event))
                {
                    switch(event.type)
                    {
                        case sf::Event::Closed:
                            //Close the main menu and gameboard windows, destroying their attached resources
                            winning_window.close();
                            gameboard.close();
                            quit_game = true;
                            break;
                        case sf::Event::KeyPressed:
                            //Get pressed key
                            key_code = event.key.code;
                            break;
                        default:
                            break;
                    }
                }

                //Check pressed key
                switch(key_code)
                {
                    //User selected a winning option choice
                    case sf::Keyboard::Return:
                        //Get selected winning option
                        switch(winning_window.getWinningOption())
                        {
                            case Winning_Option::PlayAgain:
                                //Reload the level to be able to play again
                                level.reload();
                                break;
                            case Winning_Option::ChangeCharacter:
                                //Close the gameboard window and destroy all attached resources
                                gameboard.close();
                                //Change to character selection window state
                                window_state = Window_State::Characterwindow;
                                break;
                            case Winning_Option::ExitGame:
                                //Close the gameboard window and destroy all attached resources
                                gameboard.close();
                                quit_game = true;
                                break;
                        }
                        //Close the winning window and destroy all attached resources
                        winning_window.close();
                        break;
                    case sf::Keyboard::Up:
                        //Go up in the winning menu
                        winning_window.goUp();
                        break;
                    case sf::Keyboard::Down:
                        //Go down in the winning menu
                        winning_window.goDown();
                        break;
                    default:
                        break;
                }
                winning_window.updateGraphics();

                //Reset the key code
                key_code = sf::Keyboard::Unknown;
            }
        }
        //Change player's state to normal if player shot
        if (player1.getState() == Player_State::ShootLow
           || player1.getState() == Player_State::ShootHigh)
        {
            player1.changeState(Player_State::Normal);
        }
        if (player2.getState() == Player_State::ShootLow
            || player2.getState() == Player_State::ShootHigh)
        {
            player2.changeState(Player_State::Normal);
        }
    }
}
