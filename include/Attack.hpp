#pragma once
#include <SFML/Graphics.hpp>

class Attack {
public:
    enum class Type {
        Light,
        Heavy,
        Up,
        Down
    };

    Attack(Type type, const sf::Vector2f& position, bool facingRight, float damageMultiplier = 1.0f);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
    bool isActive() const { return m_isActive; }
    sf::FloatRect getHitbox() const { return m_hitbox; }
    int getDamage() const { return m_damage; }
    Type getType() const { return m_type; }
    
private:
    Type m_type;
    sf::FloatRect m_hitbox;
    int m_damage;
    float m_lifetime;
    float m_duration;
    bool m_isActive;
    sf::RectangleShape m_visualHitbox; // For debugging/visual feedback
};
