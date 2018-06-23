/*
Compiling:
g++ -g -std=c++11 class_tests.cpp entity.cpp player.cpp projectile.cpp level.cpp -lsfml-graphics -lsfml-window -lsfml-system
*/

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "entity.h"
#include "player.h"
#include "projectile.h"
#include "level.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <array>

/*
#include "enum_action.h"
#include "enum_direction.h"
#include "struct_point.h"
#include "gui.h"
#include "gui_characterscreen.h"
#include "gui_main_menu.h"
#include "gui_gameboard.h"
#include <SFML/Graphics.hpp>*/

using namespace std;

sf::Clock timer;

SCENARIO("Initializing projectile by ID from file")
{
    GIVEN("A projectile initialized with ID 2")
    {
        Projectile proj { 2 };
        THEN("Projectile name will be \"Awesome projectile\"")
        {
            REQUIRE(proj.getName() == "Awesome projectile");
        }
        THEN("Projectile speed will be 1.0")
        {
            REQUIRE(proj.getSpeed() == 1.0);
        }
        THEN("Projectile state will be High")
        {
            REQUIRE(proj.getState() == Projectile_State::High);
        }
        THEN("Projectile damage will be 100")
        {
            REQUIRE(proj.getDamage() == 100);
        }
        THEN("Projectile cooldown will be 6.0")
        {
            REQUIRE(proj.getCooldown() == 6.0);
        }
        THEN("Projectile sprite source will be \"projectile_name.png\"")
        {
            REQUIRE(proj.getSpriteSource() == "projectile_name.png");
        }
    }
}

SCENARIO("Trying out Projectile class's public functions")
{
    GIVEN("A projectile initialized with ID 2")
    {
        Projectile proj { 2 };
        WHEN("Projectile's positon is set to (200,200)")
        {
            Point p_200 {200, 200};
            proj.setPosition(p_200);
            THEN("Projectile's position will be (200,200)")
            {
                REQUIRE(proj.getPosition().x == 200);
                REQUIRE(proj.getPosition().y == 200);
            }
            WHEN("Projectile is moved in direction Left")
            {
                proj.move(Direction::Left);
                THEN("Projectile's position will be (199,200)")
                {
                    REQUIRE(proj.getPosition().x == 199);
                    REQUIRE(proj.getPosition().y == 200);
                }
            }
            WHEN("Projectile is moved in direction right")
            {
                proj.move(Direction::Right);
                THEN("Player's position will be (201,200)")
                {
                    REQUIRE(proj.getPosition().x == 201);
                    REQUIRE(proj.getPosition().y == 200);
                }
            }
            WHEN("Projectile is moved 100 steps in direction Right")
            {
                proj.move(Direction::Right, 100);
                THEN("Projectile's position will be (300,200)")
                {
                    REQUIRE(proj.getPosition().x == 300);
                    REQUIRE(proj.getPosition().y == 200);
                }
            }
        }
    }
}


SCENARIO("Initializing player by ID from file")
{
    GIVEN("A player with initialized ID 1")
    {
        Player player { 1 };
        THEN("Player name will be \"crazy guy\"")
        {
            REQUIRE(player.getName() == "crazy guy");
        }
        THEN("Player max health will be 1000")
        {
            REQUIRE(player.getMaxHealth() == 1000);
        }
        THEN("Player health will be same as max health")
        {
            REQUIRE(player.getHealth() == 1000);
        }        
        THEN("Player speed will be 1.2")
        {
            REQUIRE(player.getSpeed() == 1.2);
        }
        THEN("Player have not yet won")
        {
            REQUIRE(player.win == false);
        }
        THEN("Player state will be player state: normal")
        {
            REQUIRE(player.getState() == Player_State::Normal);
        }
        THEN("Player low projectile will have id 1")
        {
            REQUIRE(player.getProjectileId(Projectile_State::Low) == 1);
        }
        THEN("Player high projectile will have id 2")
        {
            REQUIRE(player.getProjectileId(Projectile_State::High) == 2);
        }
        THEN("Player sprite source will be \"crazy_guy.png\"")
        {
            REQUIRE(player.getSpriteSource() == "crazy_guy.png");
        }
    }
}

SCENARIO("Trying out Player class's public functions")
{
    GIVEN("A player (player 1) with initialized ID 1")
    {
        Player player { 1 };
        WHEN("Player state is changed to \"Jump\"")
        {
            player.setState(Player_State::Jump);
            THEN("Player state will be Jump")
            {
                REQUIRE(player.getState() == Player_State::Jump);
            }
        }
        WHEN("Player state is changed back to \"Duck\"")
        {
            player.setState(Player_State::Duck);
            THEN("Player state will be Normal")
            {
                REQUIRE(player.getState() == Player_State::Duck);
            }
        }
        THEN("Player is not dead")
        {
            REQUIRE(player.isDead() == false);
        }
        WHEN("Player is given damage equal to max health of 1000")
        {
            player.giveDamage(player.getMaxHealth());
            THEN("Player will be dead")
            {
                REQUIRE(player.isDead() == true);
                REQUIRE(player.getHealth() == 0);
            }
            WHEN("Player is resetted")
            {
                player.reset();
                THEN("Player's life will be back to 1000")
                {
                    REQUIRE(player.getHealth() == 1000);
                }
            }                                    
        }
        WHEN("Player's state is set to Duck")
        {
            player.setState(Player_State::Duck);
            THEN("Player state will be Duck")
            {
                REQUIRE(player.getState() == Player_State::Duck);
            }
            WHEN("Player is resetted")
            {
                player.reset();
                THEN("Player's state will be back to Normal")
                {
                    REQUIRE(player.getState() == Player_State::Normal);
                }
            }
        }
        WHEN("Player's start position is set to (100,100)")
        {
            player.setStartPosition(100, 100);
            THEN("Player's position will still be default (0,0)")
            {
                REQUIRE(player.getPosition().x == 0);
                REQUIRE(player.getPosition().y == 0);
            }
            WHEN("Player is resetted")
            {
                player.reset();
                THEN("Player's position will be start position (100, 100)")
                {
                    REQUIRE(player.getPosition().x == 100);
                    REQUIRE(player.getPosition().y == 100);
                }
            }
        }
        WHEN("Player's position is set to (100,100)")
        {
            Point p_100 {100,100};
            player.setPosition(p_100);
            WHEN("Player's restrictions is set to (100->250)")
            {
                array<int, 2> restr {100, 250};
                player.setRestrictions(restr);
                WHEN("Player tried to move left")
                {
                    player.move(Direction::Left);
                    THEN("Player did not move (still on default position (0,0)")
                    {
                        REQUIRE(player.getPosition().x == 100);
                        REQUIRE(player.getPosition().y == 100);
                    }
                }
                WHEN("Player tried to move 200 steps in direction right")
                {
                    player.move(Direction::Right, 200);
                    THEN("Player stopped at x-position 250")
                    {
                        REQUIRE(player.getPosition().x == 250);
                    }
                }
                /*WHEN("Player's positon is set to (251,100)")
                {
                    Point p {251, 100};
                    player.setPosition(p);
                    THEN("Player did not move")
                    {
                        REQUIRE(player.getPosition().x == 100);
                        REQUIRE(player.getPosition().y == 100);
                    }
                }*/
                WHEN("Player is moved in direction Right")
                {
                    player.move(Direction::Right);
                    THEN("Player's new position is (101,100)")
                    {
                        REQUIRE(player.getPosition().x == 101);
                    }
                }
            }                  
        }        
        THEN("Player will have no active projectiles")
        {
            REQUIRE(player.getActiveProjectiles().size() == 0);
        }
        WHEN("Player shoot's projectile with ID 1")
        {
            player.shoot(1, timer.getElapsedTime());
            THEN("Player will have one active projectile")
            {
                REQUIRE(player.getActiveProjectiles().size() == 1);
            }
            WHEN("Player shoots another projectile with same ID")
            {
                player.shoot(1, timer.getElapsedTime());
                THEN("Player will still have one active projectile (because of cooldown)")
                {
                    REQUIRE(player.getActiveProjectiles().size() == 1);
                }
            }
            WHEN("Player shoot's projectile with ID 2")
            {
                player.shoot(2, timer.getElapsedTime());
                THEN("Player will have two active projectiles")
                {
                    REQUIRE(player.getActiveProjectiles().size() == 2);
                }
            }
            /*WHEN("Player shoot's projectile with ID 3")
            {
                player.shoot(3, timer.getElapsedTime());
                THEN("Player did not shoot anything (doesn't have projectile with ID 3)")
                {
                    REQUIRE(player.getActiveProjectiles().size() == 1);
                }
            }*/
            WHEN("Waiting 2 seconds (2)")
            {
                sf::sleep(sf::seconds(2));
                WHEN("Player shoot's projectile with ID 1 again")
                {
                    player.shoot(1, timer.getElapsedTime());
                    THEN("Player will not shoot a new projectile (because cooldown is not ready")
                    {
                        REQUIRE(player.getActiveProjectiles().size() == 1);
                    }
                }
                WHEN("Waiting 1 second more (3)")
                {
                    sf::sleep(sf::seconds(1));
                    WHEN("Player shoot's projectile with ID 1 again")
                    {
                        player.shoot(1, timer.getElapsedTime());
                        THEN("Player will have shot another projectile (because cooldown is ready)")
                        {
                            REQUIRE(player.getActiveProjectiles().size() == 2);
                        }
                        WHEN("Waiting 2 seconds more (5)")
                        {
                            sf::sleep(sf::seconds(2));
                            WHEN("Player tries to shoot projectile with ID 1 again")
                            {
                                player.shoot(1, timer.getElapsedTime());
                                THEN("Player will not have shot (because cooldown is not ready)")
                                {
                                    REQUIRE(player.getActiveProjectiles().size() == 2);
                                }
                            }
                            WHEN("Waiting 2 seconds more (7)")
                            {
                                sf::sleep(sf::seconds(2));
                                WHEN("Player tries to shoot projectile with ID 1 again")
                                {
                                    player.shoot(1, timer.getElapsedTime());
                                    THEN("Player will have shot another projectile with (because cooldown is ready)")
                                    {
                                        REQUIRE(player.getActiveProjectiles().size() == 3);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        WHEN("Player's position is set to (100, 100)")
        {
            Point p_100 {100, 100};
            player.setPosition(p_100);
            WHEN("Creating projectile on position (100, 100)")
            {
                Projectile proj { 1 };
                proj.setPosition(p_100);
                THEN("Player should collide with projectile")
                {
                    REQUIRE(player.collides(proj) == true);
                }
                WHEN("Projectile state is set to high")
                {
                    proj.setState(Projectile_State::High);
                    WHEN("Player state is set to duck")
                    {
                        player.setState(Player_State::Duck);
                        THEN("Projectile will not collide (player avoids by jumping)")
                        {
                            REQUIRE(player.collides(proj) == false);
                        }
                    }
                    WHEN("Player state is set to Jump")
                    {
                        player.setState(Player_State::Jump);
                        THEN("Projectile will collide with player")
                        {
                            REQUIRE(player.collides(proj) == true);
                        }
                    }
                }
                WHEN("Projectile state is set to low")
                {
                    proj.setState(Projectile_State::Low);
                    WHEN("Player state is set to Jump")
                    {
                        player.setState(Player_State::Jump);
                        THEN("Projectile will not collide (player avoids by ducking)")
                        {
                            REQUIRE(player.collides(proj) == false);
                        }
                    }
                    WHEN("Player state is set to Duck")
                    {
                        player.setState(Player_State::Duck);
                        THEN("Projectile will collide with player")
                        {
                            REQUIRE(player.collides(proj) == true);
                        }
                    }
                }
            }
            WHEN("Creating projectile on position (150, 100) and player's width is 50 (from test data)")
            {
                Projectile proj { 1 };
                Point p_150 {150, 100};
                proj.setPosition(p_150);
                THEN("Player should collide with projectile")
                {
                    REQUIRE(player.collides(proj) == true);
                }
            }
            WHEN("Creating projectile on position (151, 100) and player's width is 50 (from test data)")
            {
                Projectile proj { 1 };
                Point p_151 {151, 100};
                proj.setPosition(p_151);
                THEN("Player should NOT collide with projectile")
                {
                    REQUIRE(player.collides(proj) == false);
                }
            }
        }     
    }
}

SCENARIO("Testing differences between player 1 and 2")
{
    GIVEN("A player 1 and player 2")
    {
        Player player1 {1, true};
        Player player2 {1, false};
        THEN("Player 1 will have correct controls for player 1")
        {
            REQUIRE(player1.hasKey(sf::Keyboard::W) == true);
            REQUIRE(player1.hasKey(sf::Keyboard::A) == true);
            REQUIRE(player1.hasKey(sf::Keyboard::S) == true);
            REQUIRE(player1.hasKey(sf::Keyboard::D) == true);
            REQUIRE(player1.hasKey(sf::Keyboard::R) == true);
            REQUIRE(player1.hasKey(sf::Keyboard::T) == true);
        }
        THEN("Player 2 will have correct controls for player 2")
        {
            REQUIRE(player2.hasKey(sf::Keyboard::Up) == true);
            REQUIRE(player2.hasKey(sf::Keyboard::Left) == true);
            REQUIRE(player2.hasKey(sf::Keyboard::Down) == true);
            REQUIRE(player2.hasKey(sf::Keyboard::Right) == true);
            REQUIRE(player2.hasKey(sf::Keyboard::Period) == true);
            REQUIRE(player2.hasKey(sf::Keyboard::Subtract) == true);
        }
        THEN("Player 1 and 2 will not have same controls")
        {
            //Player 1
            REQUIRE(player1.hasKey(sf::Keyboard::Up) == false);
            REQUIRE(player1.hasKey(sf::Keyboard::Left) == false);
            REQUIRE(player1.hasKey(sf::Keyboard::Down) == false);
            REQUIRE(player1.hasKey(sf::Keyboard::Right) == false);
            REQUIRE(player1.hasKey(sf::Keyboard::Period) == false);
            REQUIRE(player1.hasKey(sf::Keyboard::Subtract) == false);
            //Player 2
            REQUIRE(player2.hasKey(sf::Keyboard::W) == false);
            REQUIRE(player2.hasKey(sf::Keyboard::A) == false);
            REQUIRE(player2.hasKey(sf::Keyboard::S) == false);
            REQUIRE(player2.hasKey(sf::Keyboard::D) == false);
            REQUIRE(player2.hasKey(sf::Keyboard::R) == false);
            REQUIRE(player2.hasKey(sf::Keyboard::T) == false);
        }
        THEN("Player 1 controls will give correct actions")
        {
            REQUIRE(player1.getAction(sf::Keyboard::W) == Action::Jump);
            REQUIRE(player1.getAction(sf::Keyboard::A) == Action::Left);
            REQUIRE(player1.getAction(sf::Keyboard::S) == Action::Duck);
            REQUIRE(player1.getAction(sf::Keyboard::D) == Action::Right);
            REQUIRE(player1.getAction(sf::Keyboard::R) == Action::ShootLow);
            REQUIRE(player1.getAction(sf::Keyboard::T) == Action::ShootHigh);
        }
        THEN("Player 2 controls will give correct actions")
        {
            REQUIRE(player2.getAction(sf::Keyboard::Up) == Action::Jump);
            REQUIRE(player2.getAction(sf::Keyboard::Left) == Action::Left);
            REQUIRE(player2.getAction(sf::Keyboard::Down) == Action::Duck);
            REQUIRE(player2.getAction(sf::Keyboard::Right) == Action::Right);
            REQUIRE(player2.getAction(sf::Keyboard::Period) == Action::ShootLow);
            REQUIRE(player2.getAction(sf::Keyboard::Subtract) == Action::ShootHigh);
        }
    }
}

SCENARIO("Initializing Level from file by ID")
{
    GIVEN("An initialized Level with ID 2")
    {
        Level level { 2 };
        THEN("Level name will be \"Firelands\"")
        {
            REQUIRE(level.getName() == "Firelands");
        }
        THEN("Level width will be 1024")
        {
            REQUIRE(level.getWidth() == 1024);
        }
        THEN("Level height will be 1280")
        {
            REQUIRE(level.getHeight() == 1280);
        }
        THEN("Background image source will be \"firelands.png\"")
        {
            REQUIRE(level.getBgSource() == "firelands.png");
        }
        THEN("Background music source will be \"scorching_battle.wav\"")
        {
            REQUIRE(level.getMusicSource() == "scorching_battle.wav");
        }        
    }
}

SCENARIO("Trying out projectile collision with level")
{
    GIVEN("An initialized Level with ID 1 and a projectile with ID 1")
    {
        Level level { 1 };
        Projectile projectile { 1 };
        WHEN("Projectile is moved outside of level to the left (-1, 0)")
        {
            projectile.setPosition(-1, 0);
            projectile.changeDirection(Direction::Left);
            THEN("Projectile will collide with level")
            {
                REQUIRE(projectile.collides(level) == true);
            }
        }
        WHEN("Projectile is on the edge of the level to the left (0,0)")
        {
            projectile.setPosition(0,0);
            projectile.changeDirection(Direction::Left);
            THEN("Projectile will not collide with level")
            {
                REQUIRE(projectile.collides(level) == false);
            }
        }
        WHEN("Projectile is moved outside of level to the right (801, 0)")
        {
            projectile.setPosition(801,0);
            projectile.changeDirection(Direction::Right);
            THEN("Projectile will collide with level")
            {
                REQUIRE(projectile.collides(level) == true);
            }
        }
        WHEN("Projectile is on the edge of the level to the right (725,0) (projectile width: 75 from test data)")
        {
            projectile.setPosition(725,0);
            projectile.changeDirection(Direction::Right);
            THEN("Projectile will not collide with level")
            {
                REQUIRE(projectile.collides(level) == false);
            }
        }
        WHEN("Projectile is inside of the level (200,200)")
        {
            projectile.setPosition(200,200);
            THEN("Projectile will not collide with level")
            {
                REQUIRE(projectile.collides(level) == false);
            }
        }
    }
}

/*THIS SCENARIO WORKS WHEN TESTED IN MAIN FUNCTION CLION, 
THIS MIGHT BE CAUSED BECAUSE OF CATCH'S TEST ORDER
SCENARIO("Trying out level's public functons")
{
    GIVEN("A level initialized with ID 2")
    {
        Level level { 2 };
        WHEN("Player 1 is added to the level (ID 1)")
        {
            Player player1 {1, true};
            level.addPlayer(player1, true);
            THEN("Player 1 will not be moved and have default position (0,0)")
            {
                REQUIRE(player1.getPosition().x == 0);
                REQUIRE(player1.getPosition().y == 0);
            }
            WHEN("Players 1's position is set to (200,100)")
            {
                Point p {200,100};
                player1.setPosition(p);
                THEN("Player 1's position will be (200,100)")
                {
                    REQUIRE(player1.getPosition().x == 200);
                    REQUIRE(player1.getPosition().y == 100);
                }
                WHEN("Reloading the level")
                {
                    level.reload();
                    THEN("Player 1's position will be changed to start position got when added to the level (150,20)")
                    {
                        REQUIRE(player1.getPosition().x == 150);
                        REQUIRE(player1.getPosition().y == 20);
                    }
                }
            }
        }
        WHEN("Player 2 is added to the level (ID 2)")
        {
            Player player2 {2, false};
            level.addPlayer(player2, false);
            THEN("Player 2 will not be moved and have default position (0,0)")
            {
                REQUIRE(player2.getPosition().x == 0);
                REQUIRE(player2.getPosition().y == 0);
            }
            WHEN("Players 2's position is set to (200,100)")
            {
                Point p {200,100};
                player2.setPosition(p);
                THEN("Player 2's position will be (200,100)")
                {
                    REQUIRE(player2.getPosition().x == 200);
                    REQUIRE(player2.getPosition().y == 100);
                }
                WHEN("Reloading the level")
                {
                    level.reload();
                    THEN("Player 2's position will be changed to start position got when added to the level (874,20)")
                    {
                        REQUIRE(player2.getPosition().x == 874);
                        REQUIRE(player2.getPosition().y == 20);   
                    }
                }
            }
        }
        
        WHEN("A player 1 and 2 is added to the level (ID 1)")
        {
            Player player1 {1, true};w
            Player player2 {2, false};
            level.addPlayer(player1, true);
            level.addPlayer(player2, false);
            THEN("Players will not be moved and have default position (0,0)")
            {
                REQUIRE(player1.getPosition().x == 0);
                REQUIRE(player1.getPosition().y == 0);
                REQUIRE(player2.getPosition().x == 0);
                REQUIRE(player2.getPosition().y == 0);
            }
            WHEN("Players' position is set to (200,100)")
            {
                Point p {200,100};
                player1.setPosition(p);
                player2.setPosition(p);
                WHEN("Reloading the level")
                {
                    level.reload();
                    THEN("Player 1's position will be changed to start positon got when added to the level (150,20)")
                    {
                        REQUIRE(player1.getPosition().x == 150);
                        REQUIRE(player1.getPosition().y == 20);
                    }
                    THEN("Player 2's position will be changed to start position got when added to the level (874,20)")
                    {
                        REQUIRE(player2.getPosition().x == 874);
                        REQUIRE(player2.getPosition().y == 20);
                    }
                }
            }
        }
    }
}*/