#pragma once

#include "Entity.hpp"
#include "Animation.hpp"
#include "ParticleSystem.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Basic enemy class with AI
 */
class Enemy : public Entity {
public:
    enum class AIState {
        Patrol,
        Chase,
        Attack
    };
    
    Enemy(const sf::Vector2f& startPosition, float patrolRange = 200.f);
    ~Enemy() override = default;
    
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;
    
    // AI methods
    void setTarget(const sf::Vector2f& targetPos);
    AIState getAIState() const { return m_aiState; }
    
    // Combat
    sf::FloatRect getAttackHitbox() const;
    bool isAttacking() const { return m_isAttacking; }
    int getAttackDamage() const { return m_attackDamage; }
    
    // Override takeDamage to add knockback
    void takeDamage(int damage) override;
    
private:
    // AI constants
    static constexpr float PATROL_SPEED = 80.f;
    static constexpr float CHASE_SPEED = 120.f;
    static constexpr float DETECTION_RANGE = 300.f;
    static constexpr float ATTACK_RANGE = 50.f;
    static constexpr float ATTACK_COOLDOWN = 1.5f;
    
    // AI state
    AIState m_aiState;
    sf::Vector2f m_patrolOrigin;
    float m_patrolRange;
    float m_patrolDirection;  // -1 (left) or 1 (right)
    sf::Vector2f m_targetPosition;
    
    // Combat
    bool m_isAttacking;
    float m_attackTimer;
    int m_attackDamage;
    
    // Rendering
    mutable sf::RectangleShape m_shape;
    mutable sf::RectangleShape m_healthBar;
    mutable sf::RectangleShape m_healthBarBackground;
    sf::Texture m_texture;
    mutable std::optional<sf::Sprite> m_sprite;
    AnimationController m_animController;
    mutable ParticleSystem m_particles;
    bool m_facingRight;
    
    // Helper methods
    void createSpriteSheet();
    void setupAnimations();
    void updateAnimation(float deltaTime);
    void updatePatrol(float deltaTime);
    void updateChase(float deltaTime);
    void updateAttack(float deltaTime);
    void updateAI(float deltaTime);
};
