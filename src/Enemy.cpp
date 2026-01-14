#include "Enemy.hpp"
#include "Physics.hpp"
#include "PixelArtGenerator.hpp"
#include <cmath>
#include <algorithm>

// ===== Projectile Implementation =====

Projectile::Projectile(const sf::Vector2f& position, const sf::Vector2f& direction, float speed, int damage, const sf::Color& color)
    : m_position(position)
    , m_velocity(direction * speed)
    , m_damage(damage)
    , m_active(true)
    , m_lifetime(3.0f)
{
    m_shape.setRadius(6.f);
    m_shape.setFillColor(color);
    m_shape.setOrigin(sf::Vector2f(6.f, 6.f));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color(255, 255, 255, 150));
}

void Projectile::update(float deltaTime) {
    if (!m_active) return;
    
    m_position += m_velocity * deltaTime;
    m_lifetime -= deltaTime;
    
    if (m_lifetime <= 0.f) {
        m_active = false;
    }
    
    // Emit trail particles
    sf::Color trailColor(m_shape.getFillColor().r, m_shape.getFillColor().g, m_shape.getFillColor().b, 180);
    m_trail.emitTrail(m_position, trailColor, 2, 3.f);
    m_trail.update(deltaTime);
}

void Projectile::render(sf::RenderWindow& window) const {
    if (!m_active) return;
    
    m_trail.render(window);
    m_shape.setPosition(m_position);
    window.draw(m_shape);
}

sf::FloatRect Projectile::getBounds() const {
    return sf::FloatRect(sf::Vector2f(m_position.x - 6.f, m_position.y - 6.f), sf::Vector2f(12.f, 12.f));
}

// ===== Enemy Implementation =====

Enemy::Enemy(const sf::Vector2f& startPosition, float patrolRange, EnemyType type)
    : Entity()
    , m_type(type)
    , m_aiState(AIState::Patrol)
    , m_patrolOrigin(startPosition)
    , m_patrolRange(patrolRange)
    , m_patrolDirection(1.f)
    , m_targetPosition(0.f, 0.f)
    , m_hoverHeight(0.f)
    , m_swoopTimer(0.f)
    , m_isSwooping(false)
    , m_isAttacking(false)
    , m_attackTimer(0.f)
    , m_attackDamage(10)
    , m_canBeKnockedBack(true)
    , m_facingRight(true)
{
    setPosition(startPosition);
    
    // Configure stats based on type
    configureByType();
    
    // Setup visual appearance
    m_shape.setSize(m_size);
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

void Enemy::configureByType() {
    switch (m_type) {
        case EnemyType::Melee:
            // Default melee fighter
            setSize(40.f, 60.f);
            setMaxHealth(50);
            m_patrolSpeed = 80.f;
            m_chaseSpeed = 120.f;
            m_detectionRange = 300.f;
            m_attackRange = 50.f;
            m_attackCooldown = 1.5f;
            m_attackDamage = 10;
            m_preferredDistance = 0.f;
            m_canBeKnockedBack = true;
            m_shape.setFillColor(sf::Color(200, 50, 50));
            m_shape.setOutlineColor(sf::Color(150, 30, 30));
            break;
            
        case EnemyType::Ranged:
            // Archer - shoots from distance
            setSize(35.f, 55.f);
            setMaxHealth(35);  // Less health, stays at range
            m_patrolSpeed = 70.f;
            m_chaseSpeed = 90.f;
            m_detectionRange = 400.f;
            m_attackRange = 250.f;  // Attacks from far away
            m_attackCooldown = 2.0f;  // Slower attack rate
            m_attackDamage = 8;
            m_preferredDistance = 200.f;  // Maintains distance
            m_canBeKnockedBack = true;
            m_shape.setFillColor(sf::Color(100, 180, 100));
            m_shape.setOutlineColor(sf::Color(70, 130, 70));
            break;
            
        case EnemyType::Flying:
            // Aerial enemy - swoops down
            setSize(35.f, 35.f);
            setMaxHealth(40);
            m_patrolSpeed = 100.f;
            m_chaseSpeed = 150.f;
            m_detectionRange = 350.f;
            m_attackRange = 80.f;
            m_attackCooldown = 2.5f;
            m_attackDamage = 12;
            m_preferredDistance = 0.f;
            m_hoverHeight = 150.f;  // Hovers above ground
            m_swoopTimer = 0.f;
            m_canBeKnockedBack = true;
            m_shape.setFillColor(sf::Color(150, 100, 200));
            m_shape.setOutlineColor(sf::Color(110, 70, 150));
            break;
            
        case EnemyType::Tank:
            // Heavy enemy - slow but powerful
            setSize(55.f, 75.f);
            setMaxHealth(120);  // Very high health
            m_patrolSpeed = 50.f;  // Slow movement
            m_chaseSpeed = 80.f;
            m_detectionRange = 250.f;
            m_attackRange = 60.f;
            m_attackCooldown = 2.0f;
            m_attackDamage = 20;  // High damage
            m_preferredDistance = 0.f;
            m_canBeKnockedBack = false;  // Immune to light attack knockback
            m_shape.setFillColor(sf::Color(120, 120, 50));
            m_shape.setOutlineColor(sf::Color(90, 90, 30));
            break;
    }
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
    
    // Update projectiles
    for (auto& projectile : m_projectiles) {
        projectile->update(deltaTime);
    }
    // Remove inactive projectiles
    m_projectiles.erase(
        std::remove_if(m_projectiles.begin(), m_projectiles.end(),
            [](const std::unique_ptr<Projectile>& p) { return !p->isActive(); }),
        m_projectiles.end()
    );
    
    // Apply friction to horizontal velocity
    m_velocity.x *= 0.95f;
    
    // Flying enemies don't obey gravity
    if (m_type == EnemyType::Flying) {
        updateFlying(deltaTime);
    } else {
        // Apply gravity for ground enemies
        m_velocity = Physics::applyGravity(m_velocity, deltaTime);
        
        // Check if on ground (simplified - ground at y=600)
        if (m_position.y + m_size.y >= 600.f) {
            m_position.y = 600.f - m_size.y;
            m_velocity.y = 0.f;
        }
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
        case AIState::Retreat:
            updateRetreat(deltaTime);
            break;
    }
}

void Enemy::updatePatrol(float deltaTime) {
    (void)deltaTime;
    // Move back and forth within patrol range
    m_velocity.x = m_patrolDirection * m_patrolSpeed;
    
    // Check if reached patrol boundary
    float distanceFromOrigin = m_position.x - m_patrolOrigin.x;
    if (std::abs(distanceFromOrigin) >= m_patrolRange) {
        m_patrolDirection *= -1.f;  // Reverse direction
    }
    
    // Check if player is in detection range
    float distanceToTarget = std::abs(m_targetPosition.x - m_position.x);
    if (distanceToTarget < m_detectionRange) {
        m_aiState = AIState::Chase;
    }
}

void Enemy::updateChase(float deltaTime) {
    (void)deltaTime;
    float distanceToTarget = std::abs(m_targetPosition.x - m_position.x);
    
    // Ranged enemies maintain distance
    if (m_type == EnemyType::Ranged && distanceToTarget < m_preferredDistance) {
        m_aiState = AIState::Retreat;
        return;
    }
    
    // Move toward target
    float direction = (m_targetPosition.x > m_position.x) ? 1.f : -1.f;
    m_velocity.x = direction * m_chaseSpeed;
    
    // Check if in attack range
    if (distanceToTarget < m_attackRange) {
        m_aiState = AIState::Attack;
        if (m_type != EnemyType::Flying) {  // Flying enemies keep moving during attack
            m_velocity.x = 0.f;
        }
    }
    // Return to patrol if target too far
    else if (distanceToTarget > m_detectionRange * 1.5f) {
        m_aiState = AIState::Patrol;
    }
}

void Enemy::updateAttack(float deltaTime) {
    (void)deltaTime;
    // Stop moving during attack (except flying)
    if (m_type != EnemyType::Flying) {
        m_velocity.x = 0.f;
    }
    
    // Attack if cooldown ready
    if (m_attackTimer <= 0.f) {
        m_isAttacking = true;
        m_attackTimer = m_attackCooldown;
        
        // Ranged enemies shoot projectiles
        if (m_type == EnemyType::Ranged) {
            shootProjectile();
        } else {
            // Melee attack particles
            float attackX = m_facingRight ? m_position.x + m_size.x : m_position.x;
            sf::Vector2f direction(m_facingRight ? 1.f : -1.f, 0.f);
            m_particles.emitDirectional(
                sf::Vector2f(attackX, m_position.y + m_size.y / 2),
                direction,
                m_type == EnemyType::Tank ? 8 : 5,
                m_type == EnemyType::Tank ? sf::Color(220, 200, 50) : sf::Color(220, 100, 50),
                20.f,
                150.f,
                0.3f,
                m_type == EnemyType::Tank ? 3.f : 2.f
            );
        }
    } else {
        m_isAttacking = false;
    }
    
    // Check if still in attack range
    float distanceToTarget = std::abs(m_targetPosition.x - m_position.x);
    if (distanceToTarget > m_attackRange) {
        m_aiState = AIState::Chase;
        m_isAttacking = false;
    }
}

void Enemy::updateRetreat(float deltaTime) {
    (void)deltaTime;
    // Move away from target to maintain preferred distance
    float direction = (m_targetPosition.x > m_position.x) ? -1.f : 1.f;
    m_velocity.x = direction * m_chaseSpeed * 0.8f;
    
    float distanceToTarget = std::abs(m_targetPosition.x - m_position.x);
    
    // If far enough, go back to attacking
    if (distanceToTarget >= m_preferredDistance) {
        m_aiState = AIState::Chase;
    }
    // If target too far, return to patrol
    else if (distanceToTarget > m_detectionRange * 1.5f) {
        m_aiState = AIState::Patrol;
    }
}

void Enemy::updateFlying(float deltaTime) {
    // Hover at a certain height
    float targetY = m_patrolOrigin.y - m_hoverHeight;
    
    // Swooping attack logic
    if (m_aiState == AIState::Attack && !m_isSwooping) {
        m_swoopTimer += deltaTime;
        if (m_swoopTimer >= 1.5f) {  // Swoop every 1.5 seconds
            m_isSwooping = true;
            m_swoopTimer = 0.f;
        }
    }
    
    if (m_isSwooping) {
        // Swoop down toward player
        targetY = m_targetPosition.y - 20.f;
        m_swoopTimer += deltaTime;
        
        if (m_swoopTimer >= 0.8f) {  // Swoop duration
            m_isSwooping = false;
            m_swoopTimer = 0.f;
        }
    }
    
    // Smooth vertical movement
    float yDiff = targetY - m_position.y;
    m_velocity.y = yDiff * 3.f;  // Smooth hovering
    
    // Emit trail particles while flying
    if (std::abs(m_velocity.x) > 10.f || std::abs(m_velocity.y) > 10.f) {
        m_particles.emitTrail(
            sf::Vector2f(m_position.x + m_size.x / 2, m_position.y + m_size.y),
            sf::Color(180, 120, 220, 150),
            1,
            3.f
        );
    }
}

void Enemy::shootProjectile() {
    // Calculate direction to target
    sf::Vector2f direction = m_targetPosition - sf::Vector2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.f) {
        direction /= length;  // Normalize
    }
    
    // Create projectile
    sf::Vector2f spawnPos(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);
    m_projectiles.push_back(std::make_unique<Projectile>(spawnPos, direction, 300.f, m_attackDamage, sf::Color(100, 220, 100)));
    
    // Emit muzzle flash
    m_particles.emitBurst(
        spawnPos,
        8,
        sf::Color(150, 255, 150),
        0.3f,
        2.f,
        100.f,
        30.f
    );
}

void Enemy::render(sf::RenderWindow& window) const {
    if (!isActive()) return;
    
    // Render projectiles
    for (const auto& projectile : m_projectiles) {
        projectile->render(window);
    }
    
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
    
    // Add knockback when hit (unless tank)
    if (!isDead() && m_canBeKnockedBack) {
        m_velocity.y = -200.f;  // Small upward bounce
        // Horizontal knockback away from attacker
        m_velocity.x = m_patrolDirection * -150.f;
    }
    
    // Emit hit particles (more for tanks)
    int particleCount = m_type == EnemyType::Tank ? 15 : 10;
    m_particles.emitBurst(
        sf::Vector2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2),
        particleCount,
        sf::Color::Red,
        0.3f,
        2.f,
        100.f,
        50.f
    );
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
            case AIState::Retreat:  // Retreat uses chase animation
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

