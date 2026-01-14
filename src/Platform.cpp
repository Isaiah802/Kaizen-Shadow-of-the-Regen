#include "Platform.hpp"
#include <cmath>

static sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len <= 0.0001f) return sf::Vector2f(0.f, 0.f);
    return sf::Vector2f(v.x / len, v.y / len);
}

Platform::Platform(float x, float y, float width, float height,
                   Type type,
                   sf::Vector2f moveDir,
                   float moveRange,
                   float moveSpeed)
    : m_position(x, y)
    , m_size(width, height)
    , m_color(100, 100, 100)
    , m_type(type)
    , m_origin(m_position)
    , m_moveDirNorm(normalize(moveDir))
    , m_moveRange(moveRange)
    , m_moveSpeed(moveSpeed)
    , m_distanceAlong(0.f)
    , m_directionSign(1)
    , m_prevPosition(m_position)
    , m_velocity(0.f, 0.f)
{
    // Tint color based on type
    switch (m_type) {
        case Type::Solid:
            m_color = sf::Color(100, 100, 100);
            break;
        case Type::OneWay:
            m_color = sf::Color(80, 120, 200); // blue-ish
            break;
        case Type::Moving:
            m_color = sf::Color(100, 180, 120); // green-ish
            break;
    }
}

void Platform::render(sf::RenderWindow& window) const {
    sf::RectangleShape shape(m_size);
    shape.setPosition(m_position);
    shape.setFillColor(m_color);
    shape.setOutlineColor(sf::Color(150, 150, 150));
    shape.setOutlineThickness(2.f);
    
    window.draw(shape);
}

void Platform::update(float deltaTime) {
    m_prevPosition = m_position;
    if (m_type == Type::Moving && m_moveRange > 0.f && (m_moveDirNorm.x != 0.f || m_moveDirNorm.y != 0.f)) {
        // Ping-pong along direction between 0 and moveRange
        m_distanceAlong += m_directionSign * m_moveSpeed * deltaTime;
        if (m_distanceAlong > m_moveRange) {
            m_distanceAlong = m_moveRange;
            m_directionSign = -1;
        } else if (m_distanceAlong < 0.f) {
            m_distanceAlong = 0.f;
            m_directionSign = 1;
        }
        m_position = m_origin + m_moveDirNorm * m_distanceAlong;
    }
    // Compute velocity from displacement
    sf::Vector2f disp = m_position - m_prevPosition;
    if (deltaTime > 0.f) {
        m_velocity = sf::Vector2f(disp.x / deltaTime, disp.y / deltaTime);
    } else {
        m_velocity = sf::Vector2f(0.f, 0.f);
    }
}

sf::FloatRect Platform::getBounds() const {
    return sf::FloatRect(m_position, m_size);
}
