#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Platform/tile class for level geometry
 * 
 * Supports solid, one-way, and moving platforms.
 */
class Platform {
public:
    enum class Type {
        Solid,
        OneWay,
        Moving
    };

    /**
     * @brief Construct a platform
     * @param x Position X
     * @param y Position Y
     * @param width Width
     * @param height Height
     * @param type Platform type (Solid/OneWay/Moving)
     * @param moveDir Movement direction for Moving (normalized internally)
     * @param moveRange Travel distance in pixels for Moving
     * @param moveSpeed Speed in pixels/second for Moving
     */
    Platform(float x, float y, float width, float height,
             Type type = Type::Solid,
             sf::Vector2f moveDir = sf::Vector2f(0.f, 0.f),
             float moveRange = 0.f,
             float moveSpeed = 0.f);
    ~Platform() = default;
    
    void render(sf::RenderWindow& window) const;
    void update(float deltaTime);
    
    sf::FloatRect getBounds() const;
    
    // Getters
    sf::Vector2f getPosition() const { return m_position; }
    sf::Vector2f getSize() const { return m_size; }
    Type getType() const { return m_type; }
    bool isOneWay() const { return m_type == Type::OneWay; }
    bool isMoving() const { return m_type == Type::Moving; }
    sf::Vector2f getVelocity() const { return m_velocity; }
    
private:
    // Base geometry
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Color m_color;
    
    // Type
    Type m_type;
    
    // Moving platform data
    sf::Vector2f m_origin;
    sf::Vector2f m_moveDirNorm;
    float m_moveRange;
    float m_moveSpeed;
    float m_distanceAlong; // 0..moveRange
    int m_directionSign;   // 1 or -1
    sf::Vector2f m_prevPosition;
    sf::Vector2f m_velocity; // computed per update
};
