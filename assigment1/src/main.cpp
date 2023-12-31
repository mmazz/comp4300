#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <fstream>


struct fontData{
    std::string file;
    int size;
    int r, g, b;
};

void LoadConfig(int& wWidth, int& wHeight, struct fontData* font, std::vector<std::shared_ptr<sf::Shape>>& entitys)
{
    const std::string& filepath = "config.txt";
    std::ifstream stream(filepath);
    std::string line, word;

    while (getline(stream, line))
    {
        std::stringstream ss(line);
        ss >> word;

        if(word == "Window")
            ss >> wWidth >> wHeight;
        else if ( word == "Font")
            ss >> font->file >> font->size >> font->r >> font->g >> font->b;
        else if(word == "Circle")
        {
            std::string name;
            float posX, posY;
            float velX, velY;
            int r, g, b;
            float radius;
            ss >> name >> posX >> posY >> velX >> velY
            >> r >> g >> b >> radius;
            std::cout << r << " " << g << " " <<b << std::endl;
            std::cout << posX << " " << posY << std::endl;
            std::shared_ptr<sf::Shape> circ = std::make_shared<sf::CircleShape>(radius);
            circ->setPosition(posX, posY);
            circ->setFillColor(sf::Color(r, g, b));
            entitys.push_back(circ);
        }
        else if(word == "Rectangle")
        {
            std::string name;
            float posX, posY;
            float velX, velY;
            int r, g, b;
            float sizeX, sizeY;
            ss >> name >> posX >> posY >> velX >> velY
            >> r >> g >> b >> sizeX >> sizeY;
            std::shared_ptr<sf::Shape> rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(sizeX, sizeY));
            rect->setPosition(posX, posY);
            rect->setFillColor(sf::Color(r, g, b));
            entitys.push_back(rect);
        }

    }
}

int main(int argc, char *argv[])
{
    int wWidth = 1280;
    int wHeight = 720;
    fontData font;
    std::vector<std::shared_ptr<sf::Shape>> entitys;
    LoadConfig(wWidth, wHeight, &font, entitys);
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");

    sf::Font myFont;

    if (!myFont.loadFromFile(font.file))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    sf::Text text("Sample Text", myFont, font.size);

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

                if (event.key.code == sf::Keyboard::Escape)
                    window.close();

              //  else if (event.key.code == sf::Keyboard::X)
              //  {
              //      circ.velX *= -1.0f;
              //      circ.velY *= -1.0f;
              //  }

            }
        }

        float sx = 0.5f;
        float sy = 0.5f;
        window.clear();
        window.draw(text);
        for(auto& entity : entitys)
        {
            entity->setPosition(entity->getPosition().x - 0.01f, entity->getPosition().y - 0.02f);
            window.draw(*entity);
        }

        window.display();
    }
    return 0;
}
