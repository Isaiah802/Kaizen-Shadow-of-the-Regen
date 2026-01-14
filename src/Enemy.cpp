#include "Enemy.hpp"
#include "Physics.hpp"
#include "PixelArtGenerator.hpp"
#include <cmath>

Enemy::Enemy(const sf::Vector2f& startPosition, float patrolRange)
    : Entity()
    , m_aiState(AIState::Patrol)
    , m_patrolOrigin(startPosition)
    , m_patrolRange(patrolRange)
    , m_patrolDirection(1.f)
    , m_targetPosition(0.f, 0.f)
    , m_isAttacking(false)
    , m_attackTimer(0.f)
    , m_attackDamage(10)
    , m_facingRight(true)
{
    setPosition(startPosition);
    setSize(40.f, 60.f);
    setMaxHealth(50);  // Enemies have less health than player
    
    // Setup visual appearance
    m_shape.setSize(m_size);
    m_shape.setFillColor(sf::Color(200, 50, 50));  // Red enemy
    m_shape.setOutlineColor(sf::Color(150, 30, 30));
    m_shape.setOutlineThickness(2.f);
    
    // Setup health bar
    m_healthBarBackground.setSize(sf::Vector2f(m_size.x, 5.f));
    m_healthBarBackground.setFillColor(sf::Color(60, 60, 60));
    
    m_healthBar.setSize(sf::Vector2f(m_size.x, 5.f));
    m_healthBar.setFillColor(sf::Color(255, 0, 0));
    
    // Create sprite and animations
    createSpriteSheet();
    setupAnimations();
    m_sprite.emplace(m_texture);
    m_animController.setAnimation("patrol");
}

void Enemy::update(float deltaTime) {
    // Call base update for invulnerability timer
    Entity::update(deltaTime);
    
    // Don't update if dead
    if (isDead()) {
        setActive(false);
        return;
    }
    
    // Update attack timer
    if (m_attackTimer > 0.f) {
        m_attackTimer -= deltaTime;
    }
    
    // Apply friction to horizontal velocity
    m_velocity.x *= 0.95f;
    
    // Apply gravity
    m_velocity = Physics::applyGravity(m_velocity, deltaTime);
    
    // Check if on ground (simplified - ground at y=600)
    if (m_position.y + m_size.y >= 600.f) {
        m_position.y = 600.f - m_size.y;
        m_velocity.y = 0.f;
    }
    
    // Update AI
    updateAI(deltaTime);
    
    // Update animation and particles
    updateAnimation(deltaTime);
    m_particles.update(deltaTime);
}

void Enemy::updateAI(float deltaTime) {
    switch (m_aiState) {
        case AIState::Patrol:
            updatePatrol(deltaTime);
            break;
        case AIState::Chase:
            updateChase(deltaTime);
            break;
        case AIState::Attack:
            updateAttack(deltaTime);
            break;
    }
}

void Enemy::updatePatrol(float deltaTime) {
    (void)deltaTime;
    // Move back and forth within patrol range
    m_velocity.x = m_patrolDirection * PATROL_SPEED;
    
    // Check if reached patrol boundary
    float distanceFromOrigin = m_position.x - m_patrolOrigin.x;
    if (std::abs(distanceFromOrigin) >= m_patrolRange) {
        m_patrolDirection *= -1.f;  // Reverse direction
    }
    
    // Check if player is in detection range
    float distanceToTarget = std::abs(m_targetPosition.x - m_position.x);
    if (distanceToTarget < DETECTION_RANGE) {
        m_aiState = AIState::Chase;
    }
}

void Enemy::updateChase(float deltaTime) {
    (void)deltaTime;
    // Move toward target
    float direction = (m_targetPosition.x > m_position.x) ? 1.f : -1.f;
    m_velocity.x = direction * CHASE_SPEED;
    
    // Check if in attack range
    float distanceToTarget = std::abs(m_targetPosition.x - m_position.x);
    if (distanceToTarget < ATTACK_RANGE) {
        m_aiState = AIState::Attack;
        m_velocity.x = 0.f;
    }
    // Return to patrol if target too far
    else if (distanceToTarget > DETECTION_RANGE * 1.5f) {
        m_aiState = AIState::Patrol;
    }
}

void Enemy::updateAttack(float deltaTime) {
    (void)deltaTime;
    // Stop moving during attack
    m_velocity.x = 0.f;
    
    // Attack if cooldown ready
    if (m_attackTimer <= 0.f) {
        m_isAttacking = true;
        m_attackTimer = ATTACK_COOLDOWN;
        
        // Emit attack particles
        float attackX = m_facingRight ? m_position.x + m_size.x : m_position.x;
        sf::Vector2f direction(m_facingRight ? 1.f : -1.f, 0.f);
        m_particles.emitDirectional(
            sf::Vector2f(attackX, m_position.y + m_size.y / 2),
            direction,
            5,
            sf::Color(220, 100, 50),
            20.f,
            150.f,
            0.3f,
            2.f
        );
    } else {
        m_isAttacking = false;
    }
    
    // Check if still in attack range
    float distanceToTarget = std::abs(m_targetPosition.x - m_position.x);
    if (distanceToTarget > ATTACK_RANGE) {
        m_aiState = AIState::Chase;
        m_isAttacking = false;
    }
}

void Enemy::render(sf::RenderWindow& window) const {
    if (!isActive()) return;
    
    // Render particles first (behind enemy)
    m_particles.render(window);
    
    // Use sprite if available
    if (m_sprite.has_value()) {
        // Get current animation frame
        sf::IntRect frameRect = m_animController.getCurrentFrame();
        m_sprite->setTextureRect(frameRect);
        
        // Position sprite centered on entity position
        m_sprite->setPosition(sf::Vector2f(
            m_position.x + m_size.x / 2.f,
            m_position.y + m_size.y / 2.f
        ));
        
        // Set origin to center for proper flipping
        m_sprite->setOrigin(sf::Vector2f(
            frameRect.size.x / 2.f,
            frameRect.size.y / 2.f
        ));
        
        // Scale to match entity size (increased by 1.5x)
        float scaleX = m_size.x / frameRect.size.x * 1.5f;
        float scaleY = m_size.y / frameRect.size.y * 1.5f;
        
        // Flip sprite based on facing direction
        if (!m_facingRight) {
            scaleX = -scaleX;
        }
        
        m_sprite->setScale(sf::Vector2f(scaleX, scaleY));
        
        // Flash when invulnerable
        if (m_invulnerable) {
            int blinkPhase = static_cast<int>(m_invulnerabilityTimer * 20) % 2;
            if (blinkPhase == 0) {
                window.draw(*m_sprite);
            }
        } else {
            window.draw(*m_sprite);
        }
    } else {
        // Fallback to old rendering
        m_shape.setPosition(sf::Vector2f(m_position.x, m_position.y));
        
        if (m_invulnerable) {
            int blinkPhase = static_cast<int>(m_invulnerabilityTimer * 20) % 2;
            if (blinkPhase == 0) {
                window.draw(m_shape);
            }
        } else {
            window.draw(m_shape);
        }
    }
    
    // Draw health bar above enemy
    m_healthBarBackground.setPosition(sf::Vector2f(m_position.x, m_position.y - 10.f));
    window.draw(m_healthBarBackground);
    
    float healthWidth = m_size.x * getHealthPercentage();
    m_healthBar.setSize(sf::Vector2f(healthWidth, 5.f));
    m_healthBar.setPosition(sf::Vector2f(m_position.x, m_position.y - 10.f));
    window.draw(m_healthBar);
}

void Enemy::setTarget(const sf::Vector2f& targetPos) {
    m_targetPosition = targetPos;
}

sf::FloatRect Enemy::getAttackHitbox() const {
    if (!m_isAttacking) {
        return sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(0, 0));  // No hitbox when not attacking
    }
    
    // Simple attack hitbox in front of enemy
    float attackWidth = 45.f;
    float attackHeight = 40.f;
    float offsetX = (m_velocity.x >= 0) ? m_size.x : -attackWidth;
    
    return sf::FloatRect(
        sf::Vector2f(m_position.x + offsetX, m_position.y + 10.f),
        sf::Vector2f(attackWidth, attackHeight)
    );
}

void Enemy::takeDamage(int damage) {
    // Call base class takeDamage
    Entity::takeDamage(damage);
    
    // Add small knockback when hit
    if (!isDead()) {
        m_velocity.y = -200.f;  // Small upward bounce
        // Horizontal knockback away from attacker (determined by current patrol direction)
        m_velocity.x = m_patrolDirection * -150.f;  // Push back slightly
        
        // Emit hit particles
        m_particles.emitBurst(
            sf::Vector2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2),
            10,
            sf::Color::Red,
            0.3f,
            2.f,
            100.f,
            50.f
        );
    }
}

void Enemy::createSpriteSheet() {
    // Use the high-quality pixel art generator for detailed enemy sprites
    m_texture = PixelArtGenerator::createEnemySpriteSheet();
    m_texture.setSmooth(false);  // Pixel art style
}

void Enemy::setupAnimations() {
    // New sprite sheet layout: 640x320 with 80x64 frames
    // 5 rows (PATROL, CHASE, ATTACK, HIT, extra)
    // 8 frames per row
    
    const int frameWidth = 80;
    const int frameHeight = 64;
    const float frameDuration = 0.1f;
    
    // Patrol animation (row 0) - 8 frames
    Animation patrolAnim;
    patrolAnim.setLooping(true);
    for (int i = 0; i < 8; ++i) {
        patrolAnim.addFrame(sf::IntRect(sf::Vector2i(i * frameWidth, 0 * frameHeight), sf::Vector2i(frameWidth, frameHeight)), frameDuration);
    }
    m_animController.addAnimation("patrol", patrolAnim);
    
    // Chase animation (row 1) - 8 frames
    Animation chaseAnim;
    chaseAnim.setLooping(true);
    for (int i = 0; i < 8; ++i) {
        chaseAnim.addFrame(sf::IntRect(sf::Vector2i(i * frameWidth, 1 * frameHeight), sf::Vector2i(frameWidth, frameHeight)), frameDuration * 0.8f);
    }
    m_animController.addAnimation("chase", chaseAnim);
    
    // Attack animation (row 2) - 6 frames
    Animation attackAnim;
    attackAnim.setLooping(false);
    for (int i = 0; i < 6; ++i) {
        attackAnim.addFrame(sf::IntRect(sf::Vector2i(i * frameWidth, 2 * frameHeight), sf::Vector2i(frameWidth, frameHeight)), frameDuration * 0.7f);
    }
    m_animController.addAnimation("attack", attackAnim);
    
    // Hit animation (row 3) - 4 frames
    Animation hitAnim;
    hitAnim.setLooping(false);
    for (int i = 0; i < 4; ++i) {
        hitAnim.addFrame(sf::IntRect(sf::Vector2i(i * frameWidth, 3 * frameHeight), sf::Vector2i(frameWidth, frameHeight)), 0.05f);
    }
    m_animController.addAnimation("hit", hitAnim);
}

void Enemy::updateAnimation(float deltaTime) {
    // Determine animation state based on AI state
    std::string targetAnim = "patrol";
    
    if (m_invulnerabilityTimer > 0) {
        targetAnim = "hit";
    } else {
        switch (m_aiState) {
            case AIState::Patrol:
                targetAnim = "patrol";
                break;
            case AIState::Chase:
                targetAnim = "chase";
                break;
            case AIState::Attack:
                targetAnim = "attack";
                break;
        }
    }
    
    // Update animation
    m_animController.setAnimation(targetAnim);
    m_animController.update(deltaTime);
    
    // Update facing direction
    if (m_velocity.x > 0.1f) {
        m_facingRight = true;
    } else if (m_velocity.x < -0.1f) {
        m_facingRight = false;
    }
}

