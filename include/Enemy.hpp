#pragma once

#include "Entity.hpp"
#include "Animation.hpp"
#include "ParticleSystem.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

/**
 * @brief Projectile class for ranged enemy attacks
 */
class Projectile {
public:
    Projectile(const sf::Vector2f& position, const sf::Vector2f& direction, float speed, int damage, const sf::Color& color);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;
    
    sf::FloatRect getBounds() const;
    bool isActive() const { return m_active; }
    void deactivate() { m_active = false; }
    int getDamage() const { return m_damage; }
    
private:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    int m_damage;
    bool m_active;
    float m_lifetime;
    mutable sf::CircleShape m_shape;
    mutable ParticleSystem m_trail;
};

/**
 * @brief Basic enemy class with AI
 */
class Enemy : public Entity {
public:
    enum class EnemyType {
        Melee,    // Default: Close-range fighter
        Ranged,   // Shoots projectiles from distance
        Flying,   // Aerial enemy with swooping attacks
        Tank      // Heavy, slow, high damage and health
    };
    
    enum class AIState {
        Patrol,
        Chase,
        Attack,
        Retreat   // For ranged enemies
    };
    
    Enemy(const sf::Vector2f& startPosition, float patrolRange = 200.f, EnemyType type = EnemyType::Melee);
    ~Enemy() override = default;
    
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;
    
    // AI methods
    void setTarget(const sf::Vector2f& targetPos);
    AIState getAIState() const { return m_aiState; }
    EnemyType getType() const { return m_type; }
    
    // Combat
    sf::FloatRect getAttackHitbox() const;
    bool isAttacking() const { return m_isAttacking; }
    int getAttackDamage() const { return m_attackDamage; }
    const std::vector<std::unique_ptr<Projectile>>& getProjectiles() const { return m_projectiles; }
    
    // Override takeDamage to add knockback
    void takeDamage(int damage) override;
    
private:
    // Enemy type
    EnemyType m_type;
    
    // AI constants (vary by type)
    float m_patrolSpeed;
    float m_chaseSpeed;
    float m_detectionRange;
    float m_attackRange;
    float m_attackCooldown;
    float m_preferredDistance;  // For ranged enemies
    
    // AI state
    AIState m_aiState;
    sf::Vector2f m_patrolOrigin;
    float m_patrolRange;
    float m_patrolDirection;  // -1 (left) or 1 (right)
    sf::Vector2f m_targetPosition;
    
    // Flying enemy specific
    float m_hoverHeight;  // Target height above ground
    float m_swoopTimer;   // Timer for swoop attacks
    bool m_isSwooping;
    
    // Combat
    bool m_isAttacking;
    float m_attackTimer;
    int m_attackDamage;
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
    bool m_canBeKnockedBack;  // Tank enemies resist knockback
    
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
    void configureByType();
    void createSpriteSheet();
    void setupAnimations();
    void updateAnimation(float deltaTime);
    void updatePatrol(float deltaTime);
    void updateChase(float deltaTime);
    void updateAttack(float deltaTime);
    void updateRetreat(float deltaTime);
    void updateAI(float deltaTime);
    void updateFlying(float deltaTime);
    void shootProjectile();
};
