//
// Created by joakim on 11/29/16.
//

#ifndef GAME38_PLAYER_H
#define GAME38_PLAYER_H


#include <SFML/Graphics/RectangleShape.hpp>
#include "Animation.h"

class Player {
public:
    Player(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, float speed);
    ~Player();

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

private:
    sf::RectangleShape body;
    Animation animation;
    unsigned int row;
    float speed;
    bool faceRight;
};


#endif //GAME38_PLAYER_H
