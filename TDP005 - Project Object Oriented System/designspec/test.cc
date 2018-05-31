#include <iostream>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;

enum Action
{
	Jump,
	Duck,
	Left,
	Right
};

//Set controls function in player class
void set_controls(string filename, Player player, bool player1)
{
	if (player1)
	{
		//Read controls keys from settings file
		//Fill controls map with keyboard event connected to action
		//Set player1 controls
	}
	else
	{
		//Read controls keys from settings file
		//Fill controls map with keyboard event connected to action
		//Set player2 controls
	}
}
int main()
{
	//Idea of map in player class to keep track of controls
	map<sf::Keyboard::Key, Action> controls
	{ 
		{sf::Keyboard::W, Action::Jump },
		{sf::Keyboard::S, Action::Duck },
		{sf::Keyboard::A, Action::Left },
		{sf::Keyboard::D, Action::Right }
	};

	cout << controls[sf::Keyboard::W] << endl;
	cout << controls[sf::Keyboard::S] << endl;
	cout << controls[sf::Keyboard::A] << endl;
	cout << controls[sf::Keyboard::D] << endl;

	//Create controls for player
	//Get key code from event
	//Send in key event into controls map

	return 0;
}

int main()
{
	//Iterate through player2's active projectiles (vector)
	for (Projectile proj: player2.getActiveProjectiles())
	{
		//Check if player 1 collides with player 2 projectile
		//collides checks point, player_state and projectile_state
		if (player1.collides(proj))
		{
			//Damage player by the projectiles damage
			player1.giveDamage(proj.getDamage());
			player2.removeProjectile(proj);
		}
	}


	//Init level by id
	Level level { gui_characterscreen.getSelectedLevelId() };
	//Add player1 and player2 to level by id
	level.addPlayer( gui_characterscreen.getPlayer1CharacterId(), true);
	level.addPlayer( gui_characterscreen.getPlayer2CharacterId(), false);
		//When adding a player to the level, set restrictions and start position of player based on the level.
		//If player wanted to be initialized from id, init player by id from JSON file.
		Player player1 { gui_characterscreen.getPlayer1CharacterId() };
		//Need to set restrictions based on the level, start position will automatically be set
		player1.setRestrictions(restriction_left);
		//Set start position based in middle of left side and y_level of the level
		player1.setStartPosition( (restriction_left[0] + restriction_left[1]) / 2, y_level );
		//Same for player2
		Player player2 { gui_characterscreen.getPlayer2CharacterId() };
		player2.setRestrictions(restriction_right);
		//Set start position based in middle of right side and y_level of the level
		player2.setStartPosition( (restriction_right[0] + restriction_right[1]) / 2, y_level );

			//When initializing player
			//Load values needed from value based on id
			//Set the controls based if player 1 or not
			set_controls();

	//Pressing a button
	//Check if player1 or player2 pressed
	if (player1.hasControl(event))
	{
		//Get action, what player1 wants to do
		Action action = player1.getAction(event);
		switch(action)
		{
			case Action::Jump: 
			player1.setState(Player_State::Jump); break;
			case Action::Duck: 
			player1.setState(Player_State::Duck); break;
			case Action::Left: 
			player1.move(Direction::Left) break;
			case Action::Right: 
			player1.move(Direction::Right) break;
			case Action::ShootLow: 
			player1.shoot(Projectile_State::Low) break;
			case Action::ShootHigh: 
			player1.shoot(Projectile_State::High) break;
		}
	}
	else
	{
		//Same as player 1
	}

	gui_gameboard.updateGraphics(level, player1, player2);
		//In updategraphics:
		//Update player positions
		//Update player sprites depending on direction and state
		//Update health based on players
		//Update active projectiles based on players

		//If any of the player has won (player1.win == true),
		//show winning screen and locks game (in some way)

	//Inside shoot function (class Player)
	//Go through all projectiles
	for (Projectile proj: projectiles)
	{
		if (proj.getState() == proj_state) //State from parameter
		{
			//Check if projectile is ready based on cooldown
			//Ready to use checks if timer > cooldown, if true reset timer
			if (proj.readyToUse())
			{
				//Set projectile to active
				proj.setActive(true);
				//proj.move(direction)  //Players direction
			}	
		}
	}
	for (projectile id from file )
	{
		Projectile p { id };
		projectiles.add(p);
	}
	Projectile { player.loadprojectileidfromfile };

	//Inside game loop
	//Get player1 and player2 active projectiles and move by distance

	GUI_Main_Menu gui_main;
	gui_main.init();


	return 0;

}