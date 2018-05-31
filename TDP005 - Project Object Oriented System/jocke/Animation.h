//
// Created by joakim on 11/29/16.
//

#ifndef GAME38_ANIMATION_H
#define GAME38_ANIMATION_H


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Animation {

public:
    Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
    ~Animation();

    void Update(int row, float deltaTime, bool faceRight);

    sf::IntRect uvRect;

private:
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;

    float totalTime;
    float switchTime;
};


#endif //GAME38_ANIMATION_H
