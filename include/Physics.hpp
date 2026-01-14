#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Physics constants and utility functions
 * 
 * Handles gravity, collision detection, and basic physics calculations
 */
namespace Physics {
    // Physics constants (in pixels per second)
    constexpr float GRAVITY = 980.f;  // Standard gravity (9.8 m/s² scaled to pixels)
    constexpr float MAX_FALL_SPEED = 800.f;
    constexpr float GROUND_Y = 600.f;  // Temporary ground level
    
    /**
     * @brief Apply gravity to velocity
     * @param velocity Current velocity
     * @param deltaTime Time elapsed
     * @return New velocity with gravity applied
     */
    sf::Vector2f applyGravity(const sf::Vector2f& velocity, float deltaTime);
    
    /**
     * @brief Check if two rectangles collide (AABB collision)
     * @param a First bounding box
     * @param b Second bounding box
     * @return True if collision detected
     */
    bool checkCollision(const sf::FloatRect& a, const sf::FloatRect& b);
    
    /**
     * @brief Check if entity is on ground
     * @param bounds Entity's bounding box
     * @param groundY Y position of ground
     * @return True if on ground
     */
    bool isOnGround(const sf::FloatRect& bounds, float groundY = GROUND_Y);
    
    /**
     * @brief Clamp velocity to maximum speed
     * @param velocity Current velocity
     * @param maxSpeed Maximum allowed speed
     * @return Clamped velocity
     */
    sf::Vector2f clampVelocity(const sf::Vector2f& velocity, float maxSpeed);
}
