#pragma once

#include "animation.h"
#include "common.h"

class Assets
{
            std::map<std::string, sf::Texture> m_textureMap;
            std::map<std::string, Animation>   m_animationMap;
            std::map<std::string, sf::Font>    m_fontMap;

            void addTexture(const std::string &texture_types, const std::string &path,
                          bool smooth = true);
            void addAnimation(const std::string  &animation_types,
                            const std::string &texture_types, size_t frameCount,
                            size_t speed);
            void addFont(const std::string  &fontName, const std::string &path);

        public:
            Assets();

            void loadFromFile(const std::string &path);

            const sf::Texture &getTexture(const std::string & texture_types) const;
            const Animation &getAnimation(const std::string & animation_types) const;
            const sf::Font &getFont(const std::string & fontName) const;
};
