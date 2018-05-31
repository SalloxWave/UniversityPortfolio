#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

void processEvent(sf::Event event);
void update(sf::RenderWindow & window);

struct Point
{
    int x,y;
};

int main()
{
    GUI_Main_Menu gmm;
    GUI_Characterscreen cs;
    GUI_Gameboard gb;


    
    gmm.init();
    while(!gmm.closed())
    //sf::sleep(sf::seconds(2));
    sf::RenderWindow window {sf::VideoMode(1024, 768), "Amazing window!"};
    window.setKeyRepeatEnabled(true);
    window.setVerticalSyncEnabled(true);

    sf::CircleShape player;
    player.setRadius(100);
    Point playerPos {0,0};
    player.setPosition(playerPos.x, playerPos.y);
    sf::Texture* projTexture;
    projTexture->loadFromFile("cat.png");
    player.setTexture(projTexture);

    sf::CircleShape projectile;
    projectile.setRadius(20);
    Point projPos {500,500};
    projectile.setPosition(projPos.x, projPos.y);

    sf::Clock clock;
    float speed = 1000.0f;
    float distance = 0.0f;

    bool playerShoot = false;
    bool lowShoot = false;

    bool quit = false;
    while (!quit)
    {
        /*bool upPressed;
        bool leftPressed;
        bool upPressed;
        bool upPressed;*/
        sf::Event event;
        sf::Keyboard::Key keyCode;
        
        while (window.pollEvent(event))
        {           
            switch(event.type)
            {
                case sf::Event::Closed:
                    quit = true;
                    break;
                case sf::Event::KeyPressed:
                    keyCode = event.key.code;                   
            }
        }
        //Restart the clock
        sf::Time delta = clock.restart();
        distance = delta.asMilliseconds() * speed / 1000.0f;
        switch(keyCode)
        {
            case sf::Keyboard::W:                           
                playerPos.y-=static_cast<int>(distance);
                break;
            case sf::Keyboard::A:
                playerPos.x-=static_cast<int>(distance);
                break;
            case sf::Keyboard::S:                           
                playerPos.y+=static_cast<int>(distance);                
                break;
            case sf::Keyboard::D:
                playerPos.x+=static_cast<int>(distance);
                break;
            case sf::Keyboard::E:
            {
                playerShoot = true;
                lowShoot = true;                
                break;
            }
            case sf::Keyboard::R:
            {
                playerShoot = true;
                lowShoot = false;
                break;
            }
            default:
                break;
        }
        if(playerShoot == true)
        {
            if (projPos.x <= window.getSize().x)
            {
                projPos.x+=static_cast<int>(distance);
                if (lowShoot == true)
                {
                    projPos.y = playerPos.y + player.getRadius() * 2;
                    projectile.setFillColor(sf::Color::Blue);        
                }
                else
                {
                    projPos.y = playerPos.y;
                    projectile.setFillColor(sf::Color::Green);
                }
            }
            else
            {
                playerShoot = false;             
                projPos = playerPos;
                //projCollided = true;

                //window.clear(sf::Color::Red);
                //window.display();
                //sf::sleep(sf::seconds(1));
                //cout << "jhaskjhaskdjhasjkd" << endl;
            }
        }
        
        
        window.clear();
        player.setPosition(playerPos.x, playerPos.y);
        projectile.setPosition(projPos.x, projPos.y);
        window.draw(player);
        window.draw(projectile);
        window.display();

        keyCode = sf::Keyboard::Unknown;
    }

    return 0;
}

void processEvent(sf::Event event)
{

}

void update(sf::RenderWindow & window)
{
    window.clear();
    //window.draw();
    window.display();
}
