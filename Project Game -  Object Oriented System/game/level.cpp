//
// Created by alexanderjlinux on 11/29/16.
//

#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

#include "level.h"

Level::Level(unsigned int level_id)
{
    //Load json data from JSON file
    std::ifstream file {"../resources/data/levels.json"};
    json j;
    file >> j;

    //Get index of level in file by specified level id
    int index {0};
    for (int i {0}; i < j.size(); ++i)
    {
        //Found specified index
        if (j[i]["id"] == level_id)
        {
            index = i;
            break;
        }
    }
    //Set level values from JSON data
    name = j[index]["name"];
    width = j[index]["width"];
    height = j[index]["height"];
    y_level = j[index]["y_level"];
    bg_src = j[index]["bg_src"];
    restriction_left[0] = j[index]["restriction_left"][0];
    restriction_left[1] = j[index]["restriction_left"][1];
    restriction_right[0] = j[index]["restriction_right"][0];
    restriction_right[1] = j[index]["restriction_right"][1];
}

void Level::addPlayer(Player &player)
{
    Point start_pos;

    //Player should start on the platform of the level
    start_pos.y = y_level;

    //Player is player one
    if (player.isPlayerOne())
    {
        //Point to the specified player
        player1 = &player;
        //Set resctriction to the added player
        player1->setRestrictions(restriction_left);

        //Set position to middle of left side
        start_pos.x = ( (restriction_left[1] - restriction_left[0]) / 2) + restriction_left[0];
        player1->setStartPosition(start_pos);
        //Move player to start position
        player1->setPosition(start_pos);
    }
    //Player is player two
    else
    {
        //Point to the specified player
        player2 = &player;
        //Set resctriction to the added player
        player2->setRestrictions(restriction_right);

        //Set position to middle of right side
        start_pos.x = ( (restriction_right[1] - restriction_right[0]) / 2) + restriction_right[0];
        player2->setStartPosition(start_pos);
        //Move player to start position
        player2->setPosition(start_pos);
    }
}

void Level::reload()
{
    //Reset player one and two if they in level
    if (player1 != nullptr)
    {
        player1->reset();
    }
    if (player2 != nullptr)
    {
        player2->reset();
    }
}

std::string Level::getName() const
{
    return name;
}

unsigned int Level::getWidth() const
{
    return width;
}

unsigned int Level::getHeight() const
{
    return height;
}

std::string Level::getBgSource() const
{
    return bg_src;
}