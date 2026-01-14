#pragma once

#include "Entity.hpp"
#include "Animation.hpp"
#include "ParticleSystem.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Boss enemy class with enhanced AI and multiple attack patterns
 */
class Boss : public Entity {
public:
    enum class BossType {
        Executioner  // First mini-boss
    };
    
    enum class Phase {
        Phase1,  // Normal attacks
        Phase2   // Enraged, faster attacks
    };
    
    Boss(BossType type, const sf::Vector2f& startPosition);
    
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;
    
    void setTarget(const sf::Vector2f& targetPos);
    bool isAttacking() const { return m_isAttacking; }
    sf::FloatRect getAttackHitbox() const;
    int getAttackDamage() const { return m_attackDamage; }
    
    void takeDamage(int damage) override;
    
private:
    void updateBehavior(float deltaTime);
    void performAttack();
    void checkPhaseTransition();
    
    BossType m_bossType;
    Phase m_currentPhase;
    
    // AI state
    sf::Vector2f m_targetPosition;
    bool m_isAttacking;
    float m_attackTimer;
    float m_attackCooldown;
    int m_attackPattern;  // Cycles through different attack types
    int m_attackDamage;
    
    // Movement
    float m_moveSpeed;
    bool m_movingRight;
    
    // Visual
    mutable sf::RectangleShape m_shape;
    mutable sf::RectangleShape m_healthBarBackground;
    mutable sf::RectangleShape m_healthBar;
    mutable AnimationController m_animController;
    mutable ParticleSystem m_particles;
    sf::Texture m_texture;
    mutable std::optional<sf::Sprite> m_sprite;
    bool m_facingRight;
    
    // Animation methods
    void createSpriteSheet();
    void setupAnimations();
    void updateAnimation(float deltaTime);
    
    // Boss-specific constants
    static constexpr float PHASE1_ATTACK_COOLDOWN = 2.0f;
    static constexpr float PHASE2_ATTACK_COOLDOWN = 1.2f;
    static constexpr float PHASE1_SPEED = 100.f;
    static constexpr float PHASE2_SPEED = 150.f;
    static constexpr float DETECTION_RANGE = 400.f;
    static constexpr float ATTACK_RANGE = 60.f;
};
