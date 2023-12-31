#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>


class AShape
{
    public:
        sf::CircleShape circle;
        sf::RectangleShape rect;
        float speedX, speedY;
};

int main(int argc, char *argv[])
{
    const int wWidth = 1280;
    const int wHeight = 720;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");

    int r = 0;
    int g = 255;
    int b = 255;

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color(r, g, b));
    circle.setPosition(300.0f, 300.0f);
    float circleMoveSpeed = 0.01f;

    float rx = 300;
    float ry = 200;
    sf::Vector2f rSize(rx, ry);


    sf::Font myFont;

    if (!myFont.loadFromFile("../fonts/tech.ttf"))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    sf::Text text("Sample Text", myFont, 24);

    text.setPosition(0, wHeight - (float)text.getCharacterSize());




    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code = " << event.key.code << "\n";

                if (event.key.code == sf::Keyboard::X)
                    circleMoveSpeed *= -1.0f;

            }
        }

        float sx = 0.5f;
        float sy = 0.5f;
        circle.setPosition(circle.getPosition().x - circleMoveSpeed, circle.getPosition().y - circleMoveSpeed);

        window.clear();
        window.draw(circle);

        window.draw(text);
        window.display();
    }
    return 0;
}
