#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Simple platform/tile class for level geometry
 * 
 * Represents static platforms that the player can stand on and collide with
 */
class Platform {
public:
    Platform(float x, float y, float width, float height);
    ~Platform() = default;
    
    void render(sf::RenderWindow& window) const;
    
    sf::FloatRect getBounds() const;
    
    // Getters
    sf::Vector2f getPosition() const { return m_position; }
    sf::Vector2f getSize() const { return m_size; }
    
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Color m_color;
};
