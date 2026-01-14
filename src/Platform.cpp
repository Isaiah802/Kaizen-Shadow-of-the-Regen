#include "Platform.hpp"

Platform::Platform(float x, float y, float width, float height)
    : m_position(x, y)
    , m_size(width, height)
    , m_color(100, 100, 100)  // Gray platforms
{
}

void Platform::render(sf::RenderWindow& window) const {
    sf::RectangleShape shape(m_size);
    shape.setPosition(m_position);
    shape.setFillColor(m_color);
    shape.setOutlineColor(sf::Color(150, 150, 150));
    shape.setOutlineThickness(2.f);
    
    window.draw(shape);
}

sf::FloatRect Platform::getBounds() const {
    return sf::FloatRect(m_position, m_size);
}
