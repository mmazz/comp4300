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

struct entity{
    float velX, velY;
    float sizeX, sizeY;
    std::string name;
    std::shared_ptr<sf::Shape> shape;
    sf::Text text;
};

void LoadConfig(int& wWidth, int& wHeight, struct fontData* font, std::vector<entity>& entitys)
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
            entity enti;
            enti.name = name;
            enti.velX = velX;
            enti.velY = velY;
            enti.sizeX = 2*radius;
            enti.sizeY = 2*radius;
            std::shared_ptr<sf::Shape> circ = std::make_shared<sf::CircleShape>(radius);
            circ->setPosition(posX, posY);
            circ->setFillColor(sf::Color(r, g, b));
            enti.shape = circ;
            entitys.push_back(enti);
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
            entity enti;
            enti.name = name;
            enti.velX = velX;
            enti.velY = velY;
            enti.sizeX = sizeX;
            enti.sizeY = sizeY;
            std::shared_ptr<sf::Shape> rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(sizeX, sizeY));
            rect->setPosition(posX, posY);
            rect->setFillColor(sf::Color(r, g, b));
            enti.shape = rect;
            entitys.push_back(enti);
        }

    }
}
int main(int argc, char *argv[])
{
    int wWidth = 1280;
    int wHeight = 720;
    fontData font;
    std::vector<entity> entitys;
    LoadConfig(wWidth, wHeight, &font, entitys);
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");

    sf::Font myFont;

    if (!myFont.loadFromFile(font.file))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }
    for(auto& entity : entitys)
    {
        entity.text = sf::Text(entity.name, myFont, font.size);
        entity.text.setPosition(entity.shape->getPosition().x, entity.shape->getPosition().y - (float)entity.text.getCharacterSize());
    }




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
                else if (event.key.code == sf::Keyboard::Q)
                    window.close();
              //  else if (event.key.code == sf::Keyboard::X)
              //  {
              //      circ.velX *= -1.0f;
              //      circ.velY *= -1.0f;
              //  }

            }
        }

        window.clear();
        for(auto& entity : entitys)
        {
            entity.shape->setPosition(entity.shape->getPosition().x - entity.velX, entity.shape->getPosition().y - entity.velY);
            auto center = entity.text.getGlobalBounds().getSize() / 2.f;
            float text_posX = entity.shape->getPosition().x + entity.sizeX/2.0f - center.x;
            float text_posY = entity.shape->getPosition().y + entity.sizeY/2.0f - center.y;
            entity.text.setPosition(text_posX,  text_posY - (float)entity.text.getCharacterSize()/2.0f);
            window.draw(*entity.shape);
            window.draw(entity.text);
        }

        window.display();
    }
    return 0;
}
