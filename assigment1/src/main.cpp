#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <fstream>

#include "../include/imgui.h"
#include "../include/imgui-SFML.h"

struct fontData{
    std::string file;
    int size;
    float r, g, b;
};

struct entity{
    float vel[2];
    float sizeX, sizeY;
    float scale = 1.0f;
    float c[3];
    char name[255];
    std::shared_ptr<sf::Shape> shape;
    sf::Text text;
};

void checkBoundries(struct entity& ent, int width, int height)
{
    auto pos = ent.shape->getPosition();
    if(pos.x < 0.0f)
        ent.vel[0] *= -1;
    else if (pos.x > width - ent.sizeX)
        ent.vel[0] *= -1;
    if(pos.y < 0.0f)
        ent.vel[1] *= -1;
    else if (pos.y > height- ent.sizeY)
        ent.vel[1]*= -1;
}
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
            float posX, posY;
            float radius;
            entity enti;
            ss >> enti.name >> posX >> posY >> enti.vel[0] >> enti.vel[1]
            >> enti.c[0] >> enti.c[1] >> enti.c[2] >> radius;
            enti.sizeX = 2*radius;
            enti.sizeY = 2*radius;
            std::shared_ptr<sf::Shape> circ = std::make_shared<sf::CircleShape>(radius);
            circ->setPosition(posX, posY);
            circ->setFillColor(sf::Color(enti.c[0], enti.c[1], enti.c[2]));
            enti.shape = circ;
            entitys.push_back(enti);
        }
        else if(word == "Rectangle")
        {
            float posX, posY;
            entity enti;
            ss >> enti.name >> posX >> posY >> enti.vel[0] >> enti.vel[1]
            >>  enti.c[0] >> enti.c[1] >> enti.c[2] >> enti.sizeX >> enti.sizeY;
            std::shared_ptr<sf::Shape> rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(enti.sizeX, enti.sizeY));
            rect->setPosition(posX, posY);
            rect->setFillColor(sf::Color(enti.c[0], enti.c[1], enti.c[2]));
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
    bool draw = true;
    bool text_render = true;
    std::vector<entity> entitys;
    LoadConfig(wWidth, wHeight, &font, entitys);
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");


    ImGui::SFML::Init(window, true);

    sf::Clock deltaClock;

    ImGui::GetStyle().ScaleAllSizes(1.0f);

    sf::Font myFont;

    if (!myFont.loadFromFile(font.file))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }
    for(auto& entity : entitys)
    {
        entity.text = sf::Text(entity.name, myFont, font.size);
        auto center = entity.text.getGlobalBounds().getSize() / 2.f;
        float text_posX = entity.shape->getPosition().x + entity.sizeX/2.0f - center.x;
        float text_posY = entity.shape->getPosition().y + entity.sizeY/2.0f - center.y/2.0f;
        entity.text.setPosition(text_posX,  text_posY - (float)entity.text.getCharacterSize()/2.0f);
    }


    const char* items[entitys.size()];
    for(int i =0; i<entitys.size(); i++)
        items[i] = entitys[i].name;


    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code = " << event.key.code << "\n";

                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                else if (event.key.code == sf::Keyboard::Q)
                    window.close();

            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());


        ImGui::Begin("Window title");
        static int item_current = 0;
        ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
        ImGui::Text("Window text");
        ImGui::Checkbox("Draw", &draw);
        ImGui::SameLine();
        ImGui::Checkbox("Text", &text_render);
        ImGui::SliderFloat("Scale", &entitys[item_current].scale, 0.0f, 3.0f);
        ImGui::SliderFloat2("Velocity", entitys[item_current].vel, -1.0f, 1.0f);
        ImGui::ColorEdit3("Color shape", entitys[item_current].c);
        ImGui::InputText("Name", entitys[item_current].name, 255);
//        if(ImGui::Button("Set button"))
//                current_shape.shape->setScale(current_shape.sizeX, current_shape.sizeX);
//
        ImGui::End();


        window.clear();
        int index = 0;
        for(auto& entity : entitys)
        {
            entity.shape->setScale(entity.scale, entity.scale);
            checkBoundries(entity, wWidth, wHeight);
            entity.shape->setPosition(entity.shape->getPosition().x + entity.vel[0], entity.shape->getPosition().y + entity.vel[1]);
            auto center = entity.text.getGlobalBounds().getSize() / 2.f;
            float text_posX = entity.shape->getPosition().x + entity.sizeX*entity.scale/2.0f - center.x;
            float text_posY = entity.shape->getPosition().y + entity.sizeY*entity.scale/2.0f - center.y/2.0f;
            entity.text.setPosition(text_posX,  text_posY - (float)entity.text.getCharacterSize()/2.0f);
            if(index==item_current)
            {
                entity.shape->setFillColor(sf::Color(entity.c[0], entity.c[1], entity.c[2]));
                entity.text = sf::Text(entity.name, myFont, font.size);
                float text_posX = entity.shape->getPosition().x + entity.sizeX*entity.scale/2.0f - center.x;
                float text_posY = entity.shape->getPosition().y + entity.sizeY*entity.scale/2.0f - center.y/2.0f;
                entity.text.setPosition(text_posX,  text_posY - (float)entity.text.getCharacterSize()/2.0f);
                if(draw)
                    window.draw(*entity.shape);
                if(text_render)
                    window.draw(entity.text);

            }
            else
            {
                window.draw(*entity.shape);
                window.draw(entity.text);
            }
            index++;
        }
        ImGui::SFML::Render(window);
        window.display();
    }
    return 0;
}


