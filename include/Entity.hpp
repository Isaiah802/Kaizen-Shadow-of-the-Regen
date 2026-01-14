#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Base class for all game entities (player, enemies, objects)
 * 
 * Provides common functionality: position, velocity, rendering, updating
 * Uses composition over inheritance where possible
 */
class Entity {
public:
    Entity();
    virtual ~Entity() = default;
    
    /**
     * @brief Update entity logic
     * @param deltaTime Time elapsed since last frame in seconds
     */
    virtual void update(float deltaTime);
    
    /**
     * @brief Render entity to window
     * @param window Target render window
     */
    virtual void render(sf::RenderWindow& window) const;
    
    // Position and movement
    void setPosition(const sf::Vector2f& pos);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    
    void setVelocity(const sf::Vector2f& vel);
    void setVelocity(float x, float y);
    sf::Vector2f getVelocity() const;
    
    // Dimensions for collision
    void setSize(const sf::Vector2f& size);
    void setSize(float width, float height);
    sf::Vector2f getSize() const;
    
    // Bounding box for collision detection
    sf::FloatRect getBounds() const;
    
    // State
    bool isActive() const;
    void setActive(bool active);
    
    // Health system
    virtual void takeDamage(int damage);
    void heal(int amount);
    void setMaxHealth(int maxHealth);
    int getHealth() const { return m_health; }
    int getMaxHealth() const { return m_maxHealth; }
    bool isDead() const { return m_health <= 0; }
    float getHealthPercentage() const { return static_cast<float>(m_health) / m_maxHealth; }
    
protected:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Vector2f m_size;
    bool m_active;
    
    // Health system
    int m_health;
    int m_maxHealth;
    bool m_invulnerable;  // Temporary invulnerability after taking damage
    float m_invulnerabilityTimer;
    static constexpr float INVULNERABILITY_DURATION = 0.5f;  // Half second of invulnerability
};
