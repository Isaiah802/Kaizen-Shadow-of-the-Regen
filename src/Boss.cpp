#include "Boss.hpp"
#include "Physics.hpp"
#include "PixelArtGenerator.hpp"
#include <cmath>

Boss::Boss(BossType type, const sf::Vector2f& startPosition)
    : Entity()
    , m_bossType(type)
    , m_currentPhase(Phase::Phase1)
    , m_targetPosition(0.f, 0.f)
    , m_isAttacking(false)
    , m_attackTimer(0.f)
    , m_attackCooldown(PHASE1_ATTACK_COOLDOWN)
    , m_attackPattern(0)
    , m_attackDamage(15)
    , m_moveSpeed(PHASE1_SPEED)
    , m_movingRight(true)
    , m_facingRight(true)
{
    setPosition(startPosition);
    
    // Boss-specific setup based on type
    if (type == BossType::Executioner) {
        setSize(60.f, 80.f);  // Larger than regular enemies
        setMaxHealth(200);    // Much more health
        
        // Dark red appearance for executioner
        m_shape.setSize(m_size);
        m_shape.setFillColor(sf::Color(150, 20, 20));
        m_shape.setOutlineColor(sf::Color(100, 10, 10));
        m_shape.setOutlineThickness(3.f);
    }
    
    // Setup health bar
    m_healthBarBackground.setSize(sf::Vector2f(m_size.x * 1.5f, 8.f));
    m_healthBarBackground.setFillColor(sf::Color(40, 40, 40));
    
    m_healthBar.setSize(sf::Vector2f(m_size.x * 1.5f, 8.f));
    m_healthBar.setFillColor(sf::Color(200, 50, 0));  // Orange for boss health
    
    // Create sprite and animations
    createSpriteSheet();
    setupAnimations();
    m_sprite.emplace(m_texture);
    m_animController.setAnimation("idle");
}

void Boss::update(float deltaTime) {
    // Call base update for invulnerability timer
    Entity::update(deltaTime);
    
    // Don't update if dead
    if (isDead()) {
        setActive(false);
        return;
    }
    
    // Check for phase transition
    checkPhaseTransition();
    
    // Update attack timer
    if (m_attackTimer > 0.f) {
        m_attackTimer -= deltaTime;
        if (m_attackTimer <= 0.f) {
            m_isAttacking = false;
        }
    }
    
    // Apply friction
    m_velocity.x *= 0.92f;
    
    // Apply gravity
    m_velocity = Physics::applyGravity(m_velocity, deltaTime);
    
    // Ground collision (simplified)
    if (m_position.y + m_size.y >= 600.f) {
        m_position.y = 600.f - m_size.y;
        m_velocity.y = 0.f;
    }
    
    // Update behavior
    updateBehavior(deltaTime);
    
    // Update animation and particles
    updateAnimation(deltaTime);
    m_particles.update(deltaTime);
}

void Boss::updateBehavior(float deltaTime) {
    // Get distance to target
    float distanceToTarget = std::abs(m_targetPosition.x - m_position.x);
    
    // If target in detection range
    if (distanceToTarget < DETECTION_RANGE) {
        // Move toward target if not in attack range
        if (distanceToTarget > ATTACK_RANGE) {
            float direction = (m_targetPosition.x > m_position.x) ? 1.f : -1.f;
            m_velocity.x = direction * m_moveSpeed * deltaTime * 60.f;
            m_movingRight = (direction > 0);
        } else {
            // In attack range - try to attack
            if (m_attackTimer <= 0.f) {
                performAttack();
            }
        }
    }
}

void Boss::performAttack() {
    m_isAttacking = true;
    m_attackTimer = 0.5f;  // Attack active duration
    
    // Set cooldown based on phase
    m_attackCooldown = (m_currentPhase == Phase::Phase1) ? 
                       PHASE1_ATTACK_COOLDOWN : PHASE2_ATTACK_COOLDOWN;
    
    // Cycle through attack patterns
    m_attackPattern = (m_attackPattern + 1) % 3;
    
    // Different attack damage based on pattern
    if (m_attackPattern == 0) {
        m_attackDamage = 15;  // Normal attack
    } else if (m_attackPattern == 1) {
        m_attackDamage = 20;  // Heavy swing
    } else {
        m_attackDamage = 25;  // Overhead smash
    }
    
    // Emit attack particles
    float attackX = m_facingRight ? m_position.x + m_size.x : m_position.x;
    sf::Vector2f direction(m_facingRight ? 1.f : -1.f, -0.2f);
    m_particles.emitDirectional(
        sf::Vector2f(attackX, m_position.y + m_size.y / 2),
        direction,
        8,
        sf::Color(220, 80, 120),
        30.f,
        200.f,
        0.4f,
        3.f
    );
}

void Boss::checkPhaseTransition() {
    // Transition to Phase 2 at 50% health
    if (m_currentPhase == Phase::Phase1 && getHealthPercentage() <= 0.5f) {
        m_currentPhase = Phase::Phase2;
        m_moveSpeed = PHASE2_SPEED;
        m_attackCooldown = PHASE2_ATTACK_COOLDOWN;
        
        // Visual change - even darker red
        m_shape.setFillColor(sf::Color(180, 10, 10));
        m_shape.setOutlineThickness(5.f);
    }
}

void Boss::render(sf::RenderWindow& window) const {
    if (!isActive()) return;
    
    // Render particles first (behind boss)
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
    
    // Draw health bar above boss
    float healthBarWidth = m_size.x * 1.5f;
    m_healthBarBackground.setPosition(sf::Vector2f(m_position.x - (healthBarWidth - m_size.x) / 2, 
                                                     m_position.y - 15.f));
    window.draw(m_healthBarBackground);
    
    float currentHealthWidth = healthBarWidth * getHealthPercentage();
    m_healthBar.setSize(sf::Vector2f(currentHealthWidth, 8.f));
    m_healthBar.setPosition(sf::Vector2f(m_position.x - (healthBarWidth - m_size.x) / 2, 
                                          m_position.y - 15.f));
    window.draw(m_healthBar);
}

void Boss::setTarget(const sf::Vector2f& targetPos) {
    m_targetPosition = targetPos;
}

sf::FloatRect Boss::getAttackHitbox() const {
    if (!m_isAttacking) {
        return sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
    }
    
    // Large attack hitbox based on attack pattern
    float attackWidth = 70.f + (m_attackPattern * 10.f);
    float attackHeight = 50.f;
    float offsetX = m_movingRight ? m_size.x : -attackWidth;
    
    return sf::FloatRect(
        sf::Vector2f(m_position.x + offsetX, m_position.y + 15.f),
        sf::Vector2f(attackWidth, attackHeight)
    );
}

void Boss::takeDamage(int damage) {
    // Call base class takeDamage
    Entity::takeDamage(damage);
    
    // Add knockback when hit
    if (!isDead()) {
        m_velocity.y = -150.f;
        m_velocity.x = m_movingRight ? -100.f : 100.f;
        
        // Emit hit particles
        m_particles.emitBurst(
            sf::Vector2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2),
            15,
            sf::Color(200, 100, 50),
            100.f,
            200.f,
            0.4f,
            3.f
        );
    }
}

void Boss::createSpriteSheet() {
    // Use the high-quality pixel art generator for detailed boss sprites
    m_texture = PixelArtGenerator::createBossSpriteSheet();
    m_texture.setSmooth(false);  // Pixel art style
}

void Boss::setupAnimations() {
    const int frameWidth = 80;
    const int frameHeight = 64;
    const float frameDuration = 0.12f;
    
    // Idle animation (row 0)
    Animation idleAnim;
    idleAnim.setLooping(true);
    for (int i = 0; i < 8; ++i) {
        idleAnim.addFrame(sf::IntRect(sf::Vector2i(i * frameWidth, 0), sf::Vector2i(frameWidth, frameHeight)), frameDuration);
    }
    m_animController.addAnimation("idle", idleAnim);
    
    // Move animation (row 1)
    Animation moveAnim;
    moveAnim.setLooping(true);
    for (int i = 0; i < 8; ++i) {
        moveAnim.addFrame(sf::IntRect(sf::Vector2i(i * frameWidth, frameHeight), sf::Vector2i(frameWidth, frameHeight)), frameDuration * 0.9f);
    }
    m_animController.addAnimation("move", moveAnim);
    
    // Attack 1 animation (row 2)
    Animation attack1Anim;
    attack1Anim.setLooping(false);
    for (int i = 0; i < 8; ++i) {
        attack1Anim.addFrame(sf::IntRect(sf::Vector2i(i * frameWidth, frameHeight * 2), sf::Vector2i(frameWidth, frameHeight)), frameDuration * 0.8f);
    }
    m_animController.addAnimation("attack1", attack1Anim);
    
    // Attack 2 animation (row 3)
    Animation attack2Anim;
    attack2Anim.setLooping(false);
    for (int i = 0; i < 8; ++i) {
        attack2Anim.addFrame(sf::IntRect(sf::Vector2i(i * frameWidth, frameHeight * 3), sf::Vector2i(frameWidth, frameHeight)), frameDuration * 0.8f);
    }
    m_animController.addAnimation("attack2", attack2Anim);
    
    // Phase 2 / Hit animation (row 4)
    Animation phase2Anim;
    phase2Anim.setLooping(true);
    for (int i = 0; i < 8; ++i) {
        phase2Anim.addFrame(sf::IntRect(sf::Vector2i(i * frameWidth, frameHeight * 4), sf::Vector2i(frameWidth, frameHeight)), frameDuration * 0.6f);
    }
    m_animController.addAnimation("phase2", phase2Anim);
}

void Boss::updateAnimation(float deltaTime) {
    // Determine animation state based on phase and state
    std::string targetAnim = "idle";
    
    if (m_invulnerabilityTimer > 0) {
        targetAnim = "phase2";  // Flash during hit
    } else if (m_isAttacking) {
        // Alternate between attack animations
        targetAnim = (m_attackPattern % 2 == 0) ? "attack1" : "attack2";
    } else if (std::abs(m_velocity.x) > 10.f) {
        targetAnim = "move";
    } else {
        // Phase 2 changes idle animation
        targetAnim = (m_currentPhase == Phase::Phase2) ? "phase2" : "idle";
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
