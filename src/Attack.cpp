#include "Attack.hpp"

Attack::Attack(Type type, const sf::Vector2f& position, bool facingRight, float damageMultiplier)
    : m_type(type)
    , m_lifetime(0.0f)
    , m_isActive(true)
{
    // Configure attack properties based on type
    if (type == Type::Light) {
        m_damage = static_cast<int>(10 * damageMultiplier);
        m_duration = 0.3f; // Light attacks are quick
        
        // Light attack hitbox - centered horizontally on player
        float width = 45.0f;
        float height = 30.0f;
        float offsetX = facingRight ? 32.0f : -width;
        float offsetY = 9.0f; // Center vertically on player body
        
        m_hitbox = sf::FloatRect(
            sf::Vector2f(position.x + offsetX, position.y + offsetY),
            sf::Vector2f(width, height)
        );
    } else if (type == Type::Heavy) {
        m_damage = static_cast<int>(20 * damageMultiplier);
        m_duration = 0.5f; // Heavy attacks last longer
        
        // Heavy attack hitbox - longer range, centered
        float width = 65.0f;
        float height = 35.0f;
        float offsetX = facingRight ? 32.0f : -width;
        float offsetY = 7.0f;
        
        m_hitbox = sf::FloatRect(
            sf::Vector2f(position.x + offsetX, position.y + offsetY),
            sf::Vector2f(width, height)
        );
    } else if (type == Type::Up) {
        m_damage = static_cast<int>(12 * damageMultiplier);
        m_duration = 0.35f;
        
        // Upward attack hitbox - above player
        float width = 35.0f;
        float height = 50.0f;
        float offsetX = -1.5f; // Centered on player
        float offsetY = -50.0f; // Above player
        
        m_hitbox = sf::FloatRect(
            sf::Vector2f(position.x + offsetX, position.y + offsetY),
            sf::Vector2f(width, height)
        );
    } else { // Down
        m_damage = static_cast<int>(15 * damageMultiplier);
        m_duration = 0.4f;
        
        // Downward attack hitbox - below player
        float width = 35.0f;
        float height = 50.0f;
        float offsetX = -1.5f; // Centered on player
        float offsetY = 48.0f; // Below player (starting from bottom of player)
        
        m_hitbox = sf::FloatRect(
            sf::Vector2f(position.x + offsetX, position.y + offsetY),
            sf::Vector2f(width, height)
        );
    }
    
    // Setup visual hitbox for feedback
    m_visualHitbox.setPosition(m_hitbox.position);
    m_visualHitbox.setSize(m_hitbox.size);
    m_visualHitbox.setFillColor(sf::Color(255, 100, 100, 100)); // Semi-transparent red
    m_visualHitbox.setOutlineColor(sf::Color::Red);
    m_visualHitbox.setOutlineThickness(2.0f);
}

void Attack::update(float deltaTime) {
    m_lifetime += deltaTime;
    
    // Deactivate after duration expires
    if (m_lifetime >= m_duration) {
        m_isActive = false;
    }
    
    // Fade out effect
    if (m_isActive) {
        float alpha = 1.0f - (m_lifetime / m_duration);
        m_visualHitbox.setFillColor(sf::Color(255, 100, 100, static_cast<uint8_t>(alpha * 100)));
    }
}

void Attack::render(sf::RenderWindow& window) {
    if (m_isActive) {
        window.draw(m_visualHitbox);
    }
}
