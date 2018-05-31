#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Player.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!", sf::Style::Close | sf::Style::Resize);
    //player.setOrigin(35.0f, 63.7f);
    sf::Texture playerTexture;
    playerTexture.loadFromFile("resources/img/player.png");

    Player player(&playerTexture, sf::Vector2u(3,9), 0.3f, 100.0f);

    float deltaTime = 0.0f;
    sf::Clock clock;

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            switch(evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                //std::cout << "New window width: " << evnt.size.width << "New window height: " << evnt.size.height << std::endl;
                printf("New window width: %i and New windows height: %i\n", evnt.size.width, evnt.size.height);
                break;
            case sf::Event::TextEntered:
                if (evnt.text.unicode < 128){
                    printf("%c\n", evnt.text.unicode);
                }
            }
        }

        /*
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            player.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
            player.move(-0.5f, 0.0f);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
            player.move(0.5f, 0.0f);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
            player.move(0.0f, -0.5f);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
            player.move(0.0f, 0.5f);
        }*/

        player.Update(deltaTime);

        window.clear(sf::Color(150, 150, 150));
        player.Draw(window);
        window.display();
    }

    return 0;
}