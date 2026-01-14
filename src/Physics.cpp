#include "Physics.hpp"
#include <cmath>
#include <algorithm>

namespace Physics {

sf::Vector2f applyGravity(const sf::Vector2f& velocity, float deltaTime) {
    sf::Vector2f newVelocity = velocity;
    newVelocity.y += GRAVITY * deltaTime;
    
    // Clamp to max fall speed
    if (newVelocity.y > MAX_FALL_SPEED) {
        newVelocity.y = MAX_FALL_SPEED;
    }
    
    return newVelocity;
}

bool checkCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.findIntersection(b).has_value();
}

bool isOnGround(const sf::FloatRect& bounds, float groundY) {
    // Check if bottom of entity is at or below ground level
    // Add small tolerance for floating point errors
    constexpr float TOLERANCE = 2.f;
    return (bounds.position.y + bounds.size.y) >= (groundY - TOLERANCE);
}

sf::Vector2f clampVelocity(const sf::Vector2f& velocity, float maxSpeed) {
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    
    if (speed > maxSpeed) {
        float scale = maxSpeed / speed;
        return sf::Vector2f(velocity.x * scale, velocity.y * scale);
    }
    
    return velocity;
}

} // namespace Physics
