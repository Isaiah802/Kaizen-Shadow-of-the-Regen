#include "Player.hpp"
#include "Physics.hpp"
#include "PixelArtGenerator.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <SFML/Window/Joystick.hpp>

Player::Player()
    : Entity()
    , m_onGround(false)
    , m_facingRight(true)
    , m_jumpCount(0)
    , m_onWall(false)
    , m_wallOnRight(false)
    , m_wallSlideTimer(0.0f)
    , m_inputDirection(0.f)
    , m_isAttacking(false)
    , m_attackCooldown(0.0f)
    , m_isParrying(false)
    , m_parryDuration(0.0f)
    , m_parryTimer(0.0f)
    , m_parryElapsed(0.0f)
    , m_chargingHeavy(false)
    , m_heavyChargeTime(0.f)
    , m_comboCount(0)
    , m_comboTimer(0.f)
    , m_comboWindowTimer(0.f)
    , m_spiritStrikeActive(false)
    , m_spiritStrikeCooldown(0.0f)
    , m_echoActive(false)
    , m_echoTimer(0.0f)
    , m_echoCooldown(0.0f)
    , m_echoEndLagTimer(0.0f)
    , m_onLedge(false)
    , m_ledgePosition()
    , m_ledgeGrabCooldown(0.0f)
    , m_pullingUp(false)
    , m_pullUpTimer(0.0f)
    , m_coyoteTimer(0.0f)
    , m_jumpInputBuffered(false)
    , m_jumpInputBufferTime(0.0f)
    , m_sprite(std::nullopt)
    , m_texture()
    , m_animController()
    , m_particles()
    , m_currentAnimState(AnimState::Idle)
{
    // Set player size
    setSize(32.f, 48.f);
    
    // Start position (centered horizontally, above ground)
    setPosition(640.f - 16.f, 300.f);
    
    // Create sprite sheet and setup animations
    createSpriteSheet();
    setupAnimations();
    m_sprite.emplace(m_texture);
    m_animController.setAnimation("idle");
}

void Player::createSpriteSheet() {
    // Use the high-quality pixel art generator to create detailed player sprites
    m_texture = PixelArtGenerator::createPlayerSpriteSheet();
    m_texture.setSmooth(false);  // Pixel art style
}

void Player::setupAnimations() {
    // New sprite sheet layout: 1024x320 with 128x40 frames
    // 8 rows (IDLE, RUN, JUMP, FALL, ATTACK, PARRY, DASH, SPIRIT_STRIKE)
    // 8 frames per row
    
    const int frameWidth = 128;
    const int frameHeight = 40;
    
    // Idle animation (row 0) - 8 frames with breathing effect
    Animation idleAnim;
    for (int i = 0; i < 8; i++) {
        float frameDuration = 0.12f + (i % 4) * 0.02f;  // Subtle breathing variation
        idleAnim.addFrame(sf::IntRect({i * frameWidth, 0 * frameHeight}, {frameWidth, frameHeight}), frameDuration);
    }
    m_animController.addAnimation("idle", idleAnim);
    
    // Run animation (row 1) - 8 frames with alternating leg motion
    Animation runAnim;
    for (int i = 0; i < 8; i++) {
        float frameDuration = 0.06f;  // Fast run cycle
        runAnim.addFrame(sf::IntRect({i * frameWidth, 1 * frameHeight}, {frameWidth, frameHeight}), frameDuration);
    }
    m_animController.addAnimation("run", runAnim);
    
    // Jump animation (row 2) - 4 frames with better pacing
    Animation jumpAnim;
    jumpAnim.addFrame(sf::IntRect({0 * frameWidth, 2 * frameHeight}, {frameWidth, frameHeight}), 0.08f);  // Launch
    jumpAnim.addFrame(sf::IntRect({1 * frameWidth, 2 * frameHeight}, {frameWidth, frameHeight}), 0.10f);  // Peak ascent
    jumpAnim.addFrame(sf::IntRect({2 * frameWidth, 2 * frameHeight}, {frameWidth, frameHeight}), 0.10f);  // Apex
    jumpAnim.addFrame(sf::IntRect({3 * frameWidth, 2 * frameHeight}, {frameWidth, frameHeight}), 0.08f);  // Descent
    jumpAnim.setLooping(false);
    m_animController.addAnimation("jump", jumpAnim);
    
    // Fall animation (row 3) - 4 frames
    Animation fallAnim;
    for (int i = 0; i < 4; i++) {
        fallAnim.addFrame(sf::IntRect({i * frameWidth, 3 * frameHeight}, {frameWidth, frameHeight}), 0.1f);
    }
    m_animController.addAnimation("fall", fallAnim);
    
    // Attack animation (row 4) - 6 frames with wind-up, strike, and recovery
    Animation attackAnim;
    attackAnim.addFrame(sf::IntRect({0 * frameWidth, 4 * frameHeight}, {frameWidth, frameHeight}), 0.06f);  // Wind-up start
    attackAnim.addFrame(sf::IntRect({1 * frameWidth, 4 * frameHeight}, {frameWidth, frameHeight}), 0.05f);  // Wind-up
    attackAnim.addFrame(sf::IntRect({2 * frameWidth, 4 * frameHeight}, {frameWidth, frameHeight}), 0.04f);  // Strike initiate
    attackAnim.addFrame(sf::IntRect({3 * frameWidth, 4 * frameHeight}, {frameWidth, frameHeight}), 0.06f);  // Full strike
    attackAnim.addFrame(sf::IntRect({4 * frameWidth, 4 * frameHeight}, {frameWidth, frameHeight}), 0.08f);  // Recovery
    attackAnim.addFrame(sf::IntRect({5 * frameWidth, 4 * frameHeight}, {frameWidth, frameHeight}), 0.08f);  // Recovery end
    attackAnim.setLooping(false);
    m_animController.addAnimation("attack", attackAnim);
    
    // Parry animation (row 5) - 6 frames with shield up and return
    Animation parryAnim;
    parryAnim.addFrame(sf::IntRect({0 * frameWidth, 5 * frameHeight}, {frameWidth, frameHeight}), 0.05f);  // Raise shield
    parryAnim.addFrame(sf::IntRect({1 * frameWidth, 5 * frameHeight}, {frameWidth, frameHeight}), 0.04f);  // Shield up fast
    parryAnim.addFrame(sf::IntRect({2 * frameWidth, 5 * frameHeight}, {frameWidth, frameHeight}), 0.15f);  // Shield hold
    parryAnim.addFrame(sf::IntRect({3 * frameWidth, 5 * frameHeight}, {frameWidth, frameHeight}), 0.15f);  // Shield hold cont
    parryAnim.addFrame(sf::IntRect({4 * frameWidth, 5 * frameHeight}, {frameWidth, frameHeight}), 0.06f);  // Lower shield
    parryAnim.addFrame(sf::IntRect({5 * frameWidth, 5 * frameHeight}, {frameWidth, frameHeight}), 0.06f);  // Return
    parryAnim.setLooping(false);
    m_animController.addAnimation("parry", parryAnim);
    
    // Dash animation (row 6) - 4 frames with burst effect
    Animation dashAnim;
    dashAnim.addFrame(sf::IntRect({0 * frameWidth, 6 * frameHeight}, {frameWidth, frameHeight}), 0.04f);  // Burst start
    dashAnim.addFrame(sf::IntRect({1 * frameWidth, 6 * frameHeight}, {frameWidth, frameHeight}), 0.04f);  // Mid-dash
    dashAnim.addFrame(sf::IntRect({2 * frameWidth, 6 * frameHeight}, {frameWidth, frameHeight}), 0.05f);  // Fast dash
    dashAnim.addFrame(sf::IntRect({3 * frameWidth, 6 * frameHeight}, {frameWidth, frameHeight}), 0.05f);  // Dash end
    dashAnim.setLooping(false);
    m_animController.addAnimation("dash", dashAnim);
    
    // Ledge grab animation (row 7) - reuse spirit strike slot as hanging pose
    // In a full game, this would be a dedicated ledge hang animation
    Animation ledgeAnim;
    for (int i = 0; i < 4; i++) {
        ledgeAnim.addFrame(sf::IntRect({i * frameWidth, 7 * frameHeight}, {frameWidth, frameHeight}), 0.2f);
    }
    ledgeAnim.setLooping(true);
    m_animController.addAnimation("ledge", ledgeAnim);
    
    // Spirit Strike animation (row 8 - shifted) - 6 frames with energy buildup
    // Note: In the current sprite sheet, this overwrites the spirit strike position
    // In production, expand sprite sheet or reuse existing animations
    Animation spiritAnim;
    spiritAnim.addFrame(sf::IntRect({0 * frameWidth, 7 * frameHeight}, {frameWidth, frameHeight}), 0.08f);  // Charge start
    spiritAnim.addFrame(sf::IntRect({1 * frameWidth, 7 * frameHeight}, {frameWidth, frameHeight}), 0.07f);  // Charging
    spiritAnim.addFrame(sf::IntRect({2 * frameWidth, 7 * frameHeight}, {frameWidth, frameHeight}), 0.06f);  // Energy building
    spiritAnim.addFrame(sf::IntRect({3 * frameWidth, 7 * frameHeight}, {frameWidth, frameHeight}), 0.05f);  // Full charge
    spiritAnim.addFrame(sf::IntRect({4 * frameWidth, 7 * frameHeight}, {frameWidth, frameHeight}), 0.08f);  // Release
    spiritAnim.addFrame(sf::IntRect({5 * frameWidth, 7 * frameHeight}, {frameWidth, frameHeight}), 0.12f);  // Cooldown
    spiritAnim.setLooping(false);
    m_animController.addAnimation("spirit_strike", spiritAnim);
}

void Player::updateAnimation(float deltaTime) {
    m_animController.update(deltaTime);
    
    // Determine animation state based on player state
    AnimState newState = AnimState::Idle;
    
    if (m_onLedge && !m_pullingUp) {
        newState = AnimState::Ledge;
    } else if (m_echoActive) {
        newState = AnimState::Dash;
    } else if (m_isParrying) {
        newState = AnimState::Parry;
    } else if (!m_activeAttacks.empty()) {
        newState = AnimState::Attack;
    } else if (isWallSliding()) {
        newState = AnimState::Fall;
    } else if (!m_onGround) {
        newState = (m_velocity.y < 0) ? AnimState::Jump : AnimState::Fall;
    } else if (std::abs(m_velocity.x) > 10.f) {
        newState = AnimState::Run;
    } else {
        newState = AnimState::Idle;
    }
    
    // Update animation controller if state changed
    if (newState != m_currentAnimState) {
        m_currentAnimState = newState;
        
        switch (m_currentAnimState) {
            case AnimState::Idle:
                m_animController.setAnimation("idle");
                break;
            case AnimState::Run:
                m_animController.setAnimation("run");
                break;
            case AnimState::Jump:
                m_animController.setAnimation("jump", true);
                break;
            case AnimState::Fall:
                m_animController.setAnimation("fall");
                break;
            case AnimState::Attack:
                m_animController.setAnimation("attack", true);
                break;
            case AnimState::Parry:
                m_animController.setAnimation("parry", true);
                break;
            case AnimState::Dash:
                m_animController.setAnimation("dash", true);
                break;
            case AnimState::Ledge:
                m_animController.setAnimation("ledge");
                break;
        }
    }
}

void Player::handleInput() {
    // Reset input direction
    m_inputDirection = 0.f;
    
    // Horizontal movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        m_inputDirection = -1.f;
        m_facingRight = false;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || 
             sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        m_inputDirection = 1.f;
        m_facingRight = true;
    }

    // Controller movement via joystick axis
    if (sf::Joystick::isConnected(0)) {
        float axisX = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
        if (std::abs(axisX) > 30.f) {
            m_inputDirection = (axisX > 0.f) ? 1.f : -1.f;
            m_facingRight = axisX > 0.f;
        }
    }
    
    // Jump (with input buffering for better feel)
    static bool wasJumpPressed = false;
    bool isJumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
                         (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0));
    
    // Detect jump button press (not hold)
    if (isJumpPressed && !wasJumpPressed) {
        jump();
    }
    wasJumpPressed = isJumpPressed;
    
    // Update facing direction while on wall (allow turning away from wall)
    if (m_onWall && m_inputDirection != 0.f) {
        // If pushing away from wall, update facing
        bool pushingAwayFromWall = (m_wallOnRight && m_inputDirection < 0.f) || 
                                   (!m_wallOnRight && m_inputDirection > 0.f);
        if (pushingAwayFromWall) {
            m_facingRight = m_inputDirection > 0.f;
        }
    }
    
    // Attack inputs (only when not on cooldown)
    if (m_attackCooldown <= 0.0f && !m_isParrying) {
        // Light attack - J key
        static bool wasLightAttackPressed = false;
        bool isLightAttackPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) ||
                                    (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 2));
        
        if (isLightAttackPressed && !wasLightAttackPressed) {
            performLightAttack();
            if (m_echoActive && m_echoTimer <= ECHO_CANCEL_WINDOW) { m_echoActive = false; }
        }
        wasLightAttackPressed = isLightAttackPressed;
        
        // Heavy attack - K key (hold to charge)
        static bool wasHeavyAttackPressed = false;
        bool isHeavyAttackPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K) ||
                                    (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 3));
        
        if (isHeavyAttackPressed && !wasHeavyAttackPressed) {
            // Start charging
            startChargingHeavy();
        } else if (!isHeavyAttackPressed && wasHeavyAttackPressed) {
            // Released - fire charged attack
            releaseHeavyAttack();
            if (m_echoActive && m_echoTimer <= ECHO_CANCEL_WINDOW) { m_echoActive = false; }
        }
        wasHeavyAttackPressed = isHeavyAttackPressed;
    }
    
    // Parry - P key (kept separate from jump to avoid conflicts)
    static bool wasParryPressed = false;
    bool isParryPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) ||
                          (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1));
    
    if (isParryPressed && !wasParryPressed && m_parryTimer <= 0.0f && !m_isAttacking && !m_isParrying) {
        performParry();
    }
    wasParryPressed = isParryPressed;
    
    // Spirit Strike - Q key (magic enhancement)
    static bool wasSpiritStrikePressed = false;
    bool isSpiritStrikePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) ||
                                (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 4));
    
    if (isSpiritStrikePressed && !wasSpiritStrikePressed && m_spiritStrikeCooldown <= 0.0f && !m_spiritStrikeActive) {
        activateSpiritStrike();
    }
    wasSpiritStrikePressed = isSpiritStrikePressed;

    // Echo Step - E key (dash with brief invulnerability)
    static bool wasEchoPressed = false;
    bool isEchoPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) ||
                         (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 5));

    if (isEchoPressed && !wasEchoPressed && m_echoCooldown <= 0.0f && !m_echoActive) {
        activateEchoStep();
    }
    wasEchoPressed = isEchoPressed;
    
    // Ledge grab input handling - pull up from ledge with Up or W
    if (m_onLedge && !m_pullingUp) {
        bool isUpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
        if (sf::Joystick::isConnected(0)) {
            float axisY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
            if (axisY < -30.f) isUpPressed = true;
        }
        
        if (isUpPressed) {
            pullUpFromLedge();
        }
    }
}

void Player::update(float deltaTime) {
    handleInput();
    
    // Update attack cooldown
    if (m_attackCooldown > 0.0f) {
        m_attackCooldown -= deltaTime;
        if (m_attackCooldown <= 0.f) {
            m_isAttacking = false;
        }
    }
    
    // Update combo system
    if (m_comboTimer > 0.f) {
        m_comboTimer -= deltaTime;
        if (m_comboTimer <= 0.f) {
            // Combo expired
            m_comboCount = 0;
        }
    }
    if (m_comboWindowTimer > 0.f) {
        m_comboWindowTimer -= deltaTime;
    }
    
    // Update heavy charge
    if (m_chargingHeavy) {
        m_heavyChargeTime += deltaTime;
        if (m_heavyChargeTime >= MAX_CHARGE_TIME) {
            m_heavyChargeTime = MAX_CHARGE_TIME;  // Cap at max
        }
        
        // Enhanced charging particles
        if (static_cast<int>(m_heavyChargeTime * 20.f) % 2 == 0) {
            sf::Vector2f chargePos = m_position + sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f);
            float intensity = m_heavyChargeTime / MAX_CHARGE_TIME;
            sf::Color chargeColor = sf::Color(255, static_cast<uint8_t>(100 + 155 * intensity), 50);
            
            // Orbiting particles
            m_particles.emitGlow(chargePos, 2, chargeColor, 40.f, 0.4f, 5.f);
            
            // Sparks at higher charge
            if (intensity > 0.5f) {
                m_particles.emitSparks(chargePos, 1, chargeColor, 100.f, 0.2f);
            }
            
            // Ring pulse at max charge
            if (intensity >= 1.0f && static_cast<int>(m_heavyChargeTime * 10.f) % 5 == 0) {
                m_particles.emitRing(chargePos, sf::Color(255, 255, 100), 30.f, 12);
            }
        }
    }
    
    // Update parry state
    if (m_isParrying) {
        m_parryElapsed += deltaTime;
        m_parryDuration -= deltaTime;
        if (m_parryDuration <= 0.0f) {
            m_isParrying = false;
            m_parryTimer = PARRY_COOLDOWN;
        }
    }
    
    // Update parry cooldown
    if (m_parryTimer > 0.0f) {
        m_parryTimer -= deltaTime;
    }
    
    // Update Spirit Strike cooldown
    if (m_spiritStrikeCooldown > 0.0f) {
        m_spiritStrikeCooldown -= deltaTime;
    }
    
    // Update coyote time (grace period for jumping after leaving ground)
    if (m_coyoteTimer > 0.0f) {
        m_coyoteTimer -= deltaTime;
    }
    
    // Update jump input buffer
    if (m_jumpInputBuffered && m_jumpInputBufferTime > 0.0f) {
        m_jumpInputBufferTime -= deltaTime;
    } else if (m_jumpInputBuffered && m_jumpInputBufferTime <= 0.0f) {
        // Buffer expired without landing
        m_jumpInputBuffered = false;
    }
    
    // Update active attacks
    for (auto& attack : m_activeAttacks) {
        attack->update(deltaTime);
    }
    
    // Remove inactive attacks
    m_activeAttacks.erase(
        std::remove_if(m_activeAttacks.begin(), m_activeAttacks.end(),
            [](const std::unique_ptr<Attack>& attack) {
                return !attack->isActive();
            }),
        m_activeAttacks.end()
    );
    
    // Echo Step movement overrides input during dash
    if (m_echoActive) {
        m_echoTimer -= deltaTime;
        m_velocity.x = (m_facingRight ? 1.f : -1.f) * ECHO_STEP_SPEED;
        
        // Continuous dash trail effect
        sf::Vector2f trailPos = m_position + sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f);
        m_particles.emitTrail(trailPos, sf::Color(150, 200, 255), 3, 5.f);
        
        if (m_echoTimer <= 0.0f) {
            m_echoActive = false;
            m_echoEndLagTimer = ECHO_END_LAG;
        }
    } else {
        // Horizontal movement with acceleration
        if (m_inputDirection != 0.f) {
            float targetVelocityX = m_inputDirection * MOVE_SPEED;
            float velocityDiff = targetVelocityX - m_velocity.x;
            float acceleration = ACCELERATION * deltaTime;
            
            if (std::abs(velocityDiff) < acceleration) {
                m_velocity.x = targetVelocityX;
            } else {
                m_velocity.x += (velocityDiff > 0 ? acceleration : -acceleration);
            }
        }
        else {
            // Apply deceleration when no input
            if (std::abs(m_velocity.x) < DECELERATION * deltaTime) {
                m_velocity.x = 0.f;
            }
            else {
                m_velocity.x -= (m_velocity.x > 0 ? DECELERATION : -DECELERATION) * deltaTime;
            }
        }
    }

    // Apply end-lag smooth deceleration
    if (m_echoEndLagTimer > 0.0f) {
        m_echoEndLagTimer -= deltaTime;
        m_velocity.x *= 0.9f; // quick ease-out over end-lag
    }
    
    // Apply gravity when not on ground
    if (!m_onGround) {
        // Apply wall slide physics
        if (m_onWall && m_velocity.y > 0.f) {
            // Sliding down wall - cap fall speed
            m_velocity = Physics::applyGravity(m_velocity, deltaTime);
            if (m_velocity.y > WALL_SLIDE_SPEED) {
                m_velocity.y = WALL_SLIDE_SPEED;
            }
            // Emit wall slide particles
            if (static_cast<int>(m_position.y) % 5 == 0) {
                float wallX = m_wallOnRight ? (m_position.x + m_size.x) : m_position.x;
                m_particles.emit(sf::Vector2f(wallX, m_position.y + m_size.y / 2.f),
                               1, sf::Color(180, 180, 180), 30.f, 0.4f, 2.f);
            }
        } else {
            m_velocity = Physics::applyGravity(m_velocity, deltaTime);
        }
    }
    
    // Update wall slide timer
    if (m_wallSlideTimer > 0.0f) {
        m_wallSlideTimer -= deltaTime;
    }
    
    // Echo Step cooldown
    if (m_echoCooldown > 0.0f) {
        m_echoCooldown -= deltaTime;
    }
    
    // Update ledge grab cooldown
    if (m_ledgeGrabCooldown > 0.0f) {
        m_ledgeGrabCooldown -= deltaTime;
    }
    
    // Update pull-up animation
    if (m_pullingUp) {
        m_pullUpTimer -= deltaTime;
        if (m_pullUpTimer <= 0.0f) {
            m_pullingUp = false;
            m_onLedge = false;
            m_jumpCount = 0;  // Reset jumps after pulling up
            m_onGround = true;  // Now standing on ledge
        }
    }

    // Update animation
    updateAnimation(deltaTime);
    
    // Update particles
    m_particles.update(deltaTime);

    // Update position based on velocity
    Entity::update(deltaTime);
    
    // Keep player on screen (horizontal bounds for larger level)
    constexpr float LEVEL_WIDTH = 2560.f;
    if (m_position.x < 0.f) {
        m_position.x = 0.f;
        m_velocity.x = 0.f;
    }
    else if (m_position.x + m_size.x > LEVEL_WIDTH) {
        m_position.x = LEVEL_WIDTH - m_size.x;
        m_velocity.x = 0.f;
    }
}

void Player::render(sf::RenderWindow& window) const {
    if (!m_sprite) return;
    
    // Update sprite texture coordinates for current animation frame
    m_sprite->setTextureRect(m_animController.getCurrentFrame());
    
    // Set sprite origin to center of frame for proper positioning
    const auto& frameRect = m_animController.getCurrentFrame();
    m_sprite->setOrigin(sf::Vector2f(frameRect.size.x / 2.f, frameRect.size.y / 2.f));
    
    // Flip sprite based on facing direction and scale up
    if (!m_facingRight) {
        m_sprite->setScale(sf::Vector2f(-1.5f, 1.5f));
        m_sprite->setPosition(m_position + sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f));
    } else {
        m_sprite->setScale(sf::Vector2f(1.5f, 1.5f));
        m_sprite->setPosition(m_position + sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f));
    }
    
    window.draw(*m_sprite);
    
    // Render particles
    m_particles.render(window);
    
    // Render Spirit Strike glow effect
    if (m_spiritStrikeActive) {
        sf::RectangleShape glowEffect(m_size);
        glowEffect.setPosition(m_position);
        glowEffect.setFillColor(sf::Color(100, 200, 255, 80)); // Ethereal blue glow
        glowEffect.setOutlineColor(sf::Color(150, 220, 255, 180));
        glowEffect.setOutlineThickness(2.f);
        window.draw(glowEffect);
        
        // Emit spirit particles
        if (static_cast<int>(m_position.x) % 3 == 0) {
            m_particles.emit(m_position + sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f), 
                           2, sf::Color(150, 220, 255), 50.f, 0.6f, 2.f);
        }
    }
    
    // Echo Step afterimages for dash visual
    if (m_echoActive) {
        sf::RectangleShape trail1(m_size);
        trail1.setPosition(sf::Vector2f(m_position.x - (m_facingRight ? 10.f : -10.f), m_position.y));
        trail1.setFillColor(sf::Color(200, 240, 255, 60));
        window.draw(trail1);

        sf::RectangleShape trail2(m_size);
        trail2.setPosition(sf::Vector2f(m_position.x - (m_facingRight ? 20.f : -20.f), m_position.y));
        trail2.setFillColor(sf::Color(200, 240, 255, 30));
        window.draw(trail2);
        
        // Emit dash particles
        m_particles.emitDirectional(m_position + sf::Vector2f(m_size.x / 2.f, m_size.y), 
                                   sf::Vector2f(m_facingRight ? -1.f : 1.f, 0.f),
                                   1, sf::Color(200, 240, 255), 15.f, 150.f, 0.3f, 3.f);
    }
    
    // Render active attacks
    for (const auto& attack : m_activeAttacks) {
        attack->render(window);
    }
    
    // Render parry visual feedback - bright blue shield
    if (m_isParrying) {
        sf::CircleShape parryIndicator(35.f);
        parryIndicator.setPosition(sf::Vector2f(m_position.x - 20.f, m_position.y - 10.f));
        parryIndicator.setFillColor(sf::Color(50, 150, 255, 150)); // Bright blue translucent
        parryIndicator.setOutlineColor(sf::Color(100, 200, 255, 255));
        parryIndicator.setOutlineThickness(4.f);
        window.draw(parryIndicator);
    }
}

void Player::jump() {
    // Wall jump takes priority
    if (m_onWall || m_wallSlideTimer > 0.0f) {
        // Wall jump - push away from wall
        bool jumpRight = !m_wallOnRight;  // Jump opposite to wall direction
        m_velocity.x = (jumpRight ? 1.f : -1.f) * WALL_JUMP_FORCE_X;
        m_velocity.y = WALL_JUMP_FORCE_Y;
        m_facingRight = jumpRight;
        m_onWall = false;
        m_wallSlideTimer = 0.0f;
        m_jumpCount = 1;  // Wall jump counts as first jump, allows double jump after
        m_coyoteTimer = 0.0f;  // Consume coyote time
        
        // Enhanced wall jump particles
        float wallX = m_wallOnRight ? (m_position.x + m_size.x) : m_position.x;
        sf::Vector2f wallPos(wallX, m_position.y + m_size.y / 2.f);
        sf::Vector2f jumpDir(jumpRight ? -1.f : 1.f, -0.5f);
        
        m_particles.emitDirectional(wallPos, jumpDir, 12, sf::Color(220, 220, 240), 45.f, 200.f, 0.7f, 3.f);
        m_particles.emitSparks(wallPos, 8, sf::Color(255, 255, 255), 180.f, 0.4f);
        m_particles.emitGlow(wallPos, 5, sf::Color(200, 200, 255), 60.f, 0.5f, 6.f);
    }
    // Allow jump if on ground or coyote time active, or if we haven't used our double jump
    else if (m_onGround || m_coyoteTimer > 0.0f || m_jumpCount < MAX_JUMPS) {
        m_velocity.y = JUMP_FORCE;
        m_jumpCount++;
        m_onGround = false;
        m_coyoteTimer = 0.0f;  // Consume coyote time on jump
        m_jumpInputBuffered = false;  // Clear buffered input
        m_jumpInputBufferTime = 0.0f;
        
        // Enhanced jump particles
        sf::Vector2f jumpPos = m_position + sf::Vector2f(m_size.x / 2.f, m_size.y);
        
        if (m_jumpCount == 1) {
            // First jump - ground burst
            m_particles.emit(jumpPos, 8, sf::Color(200, 200, 220), 100.f, 0.6f, 3.f);
            m_particles.emitDirectional(jumpPos, sf::Vector2f(0.f, 1.f), 5, sf::Color(180, 180, 200), 30.f, 80.f, 0.4f, 4.f);
        } else {
            // Double jump - air burst with sparkles
            m_particles.emitGlow(jumpPos, 10, sf::Color(150, 200, 255), 80.f, 0.7f, 6.f);
            m_particles.emitSparks(jumpPos, 12, sf::Color(200, 230, 255), 150.f, 0.5f);
            m_particles.emitRing(jumpPos, sf::Color(255, 255, 255), 25.f, 12);
        }
    } else {
        // Buffer jump input for execution next frame or on landing
        m_jumpInputBuffered = true;
        m_jumpInputBufferTime = JUMP_INPUT_BUFFER;
    }
}

bool Player::isOnGround() const {
    return m_onGround;
}

void Player::setOnGround(bool onGround) {
    m_onGround = onGround;
    if (onGround) {
        m_jumpCount = 0;  // Reset jump count when landing
        m_onWall = false;  // Can't be on wall if on ground
        m_wallSlideTimer = 0.0f;
        m_coyoteTimer = COYOTE_TIME;  // Start coyote time grace period
        
        // Process buffered jump input if we just landed
        if (m_jumpInputBuffered) {
            m_jumpInputBuffered = false;
            m_jumpInputBufferTime = 0.0f;
            jump();  // Execute the buffered jump
        }
    }
}

void Player::setOnWall(bool onWall, bool wallOnRight) {
    // Only stick to wall if not on ground and moving against/down the wall
    if (!m_onGround && onWall) {
        bool movingTowardWall = (wallOnRight && m_inputDirection > 0.f) ||
                               (!wallOnRight && m_inputDirection < 0.f) ||
                               m_inputDirection == 0.f;
        
        if (movingTowardWall || m_velocity.y > 0.f) {
            m_onWall = true;
            m_wallOnRight = wallOnRight;
            m_wallSlideTimer = WALL_STICK_TIME;  // Give grace period for wall jump
            m_jumpCount = 0;  // Reset jumps when grabbing wall
        }
    } else {
        m_onWall = false;
    }
}

void Player::setOnLedge(bool onLedge, const sf::Vector2f& ledgePos) {
    // Only grab ledge if falling and not already on ground
    if (!m_onGround && onLedge && m_velocity.y > 0 && m_ledgeGrabCooldown <= 0.0f) {
        m_onLedge = true;
        m_ledgePosition = ledgePos;
        m_velocity = sf::Vector2f(0.f, 0.f);  // Stop movement
        m_jumpCount = 0;  // Reset jumps when grabbing ledge
        m_ledgeGrabCooldown = LEDGE_GRAB_COOLDOWN;
        
        // Emit ledge grab particles
        m_particles.emit(m_ledgePosition, 5, sf::Color(200, 220, 255), 60.f, 0.5f, 2.f);
    } else if (onLedge && m_onLedge && m_velocity.y > WALL_SLIDE_SPEED) {
        // Update ledge position while hanging
        m_ledgePosition = ledgePos;
    }
}

void Player::pullUpFromLedge() {
    if (m_onLedge && !m_pullingUp) {
        m_pullingUp = true;
        m_pullUpTimer = PULL_UP_DURATION;
        m_velocity.y = -PULL_UP_HEIGHT / PULL_UP_DURATION;  // Rise smoothly
        
        // Emit pull-up particles
        m_particles.emitBurst(m_ledgePosition, 8, sf::Color(150, 200, 255), 100.f, 200.f, 0.4f, 2.5f);
    }
}

void Player::performLightAttack() {
    // Check if we can attack (either fresh attack or continuing combo)
    if (m_comboWindowTimer > 0.f) {
        return;  // Still in attack animation
    }
    
    // Advance combo or start new one
    if (m_comboTimer > 0.f && m_comboCount < 3) {
        m_comboCount++;  // Continue combo
    } else {
        m_comboCount = 1;  // Start new combo
    }
    
    // Reset combo timer
    m_comboTimer = COMBO_WINDOW;
    m_comboWindowTimer = COMBO_ATTACK_DELAY;
    
    // Determine attack type based on combo count
    Attack::Type attackType = Attack::Type::Light;
    float damageMultiplier = 1.0f;
    sf::Color particleColor = sf::Color(255, 200, 100);
    int particleCount = 8;
    
    // Check for up/down input (overrides combo)
    bool upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || 
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    bool downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || 
                       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
    if (sf::Joystick::isConnected(0)) {
        float axisY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
        if (axisY < -30.f) upPressed = true;
        if (axisY > 30.f) downPressed = true;
    }
    
    if (upPressed) {
        attackType = Attack::Type::Up;
        m_comboCount = 0;  // Special attacks reset combo
    } else if (downPressed && !m_onGround) {
        attackType = Attack::Type::Down;
        m_comboCount = 0;  // Special attacks reset combo
    } else {
        // Normal combo progression
        switch (m_comboCount) {
            case 1:
                // First hit - quick jab
                damageMultiplier = COMBO_1_DAMAGE / 10.f;
                particleColor = sf::Color(255, 200, 100);
                particleCount = 6;
                break;
            case 2:
                // Second hit - stronger
                damageMultiplier = COMBO_2_DAMAGE / 10.f;
                particleColor = sf::Color(255, 150, 50);
                particleCount = 10;
                break;
            case 3:
                // Third hit - finisher!
                damageMultiplier = COMBO_3_DAMAGE / 10.f;
                particleColor = sf::Color(255, 100, 100);
                particleCount = 15;
                m_comboTimer = 0.f;  // Finisher ends combo
                break;
        }
    }
    
    // Apply Spirit Strike multiplier
    if (m_spiritStrikeActive) {
        damageMultiplier *= SPIRIT_STRIKE_DAMAGE_MULTIPLIER;
    }
    
    m_activeAttacks.push_back(
        std::make_unique<Attack>(attackType, m_position, m_facingRight, damageMultiplier)
    );
    m_attackCooldown = LIGHT_ATTACK_COOLDOWN;
    m_isAttacking = true;
    
    // Enhanced attack particles (scaled by combo)
    sf::Vector2f attackPos = m_position + sf::Vector2f(m_facingRight ? m_size.x : 0.f, m_size.y / 2.f);
    
    // Base burst
    m_particles.emitBurst(attackPos, particleCount / 2, particleColor, 80.f, 150.f + (m_comboCount * 30.f), 0.3f, 3.f);
    
    // Add sparks for combo hits
    if (m_comboCount >= 2) {
        m_particles.emitSparks(attackPos, 5 + m_comboCount * 3, particleColor, 200.f, 0.25f);
    }
    
    // Finisher explosion!
    if (m_comboCount == 3) {
        m_particles.emitExplosion(attackPos, particleColor, 2, 8);
        m_particles.emitGlow(attackPos, 10, sf::Color(255, 255, 100), 60.f, 0.6f, 10.f);
    }
    
    // Consume Spirit Strike buff
    if (m_spiritStrikeActive) {
        m_spiritStrikeActive = false;
    }
}

void Player::performHeavyAttack() {
    // This is now deprecated - use startChargingHeavy/releaseHeavyAttack instead
    startChargingHeavy();
    releaseHeavyAttack();
}

void Player::startChargingHeavy() {
    if (m_attackCooldown > 0.f) return;  // On cooldown
    
    m_chargingHeavy = true;
    m_heavyChargeTime = 0.f;
}

void Player::releaseHeavyAttack() {
    if (!m_chargingHeavy) return;
    
    m_chargingHeavy = false;
    
    // Only attack if minimum charge reached
    if (m_heavyChargeTime < MIN_CHARGE_TIME) {
        m_heavyChargeTime = 0.f;
        return;
    }
    
    // Calculate charge level (0.0 to 1.0)
    float chargeLevel = std::min(m_heavyChargeTime / MAX_CHARGE_TIME, 1.0f);
    
    Attack::Type attackType = Attack::Type::Heavy;
    float damageMultiplier = 1.5f + (chargeLevel * 1.0f);  // 1.5x to 2.5x damage
    
    // Check for launcher (Up + Heavy)
    bool upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || 
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    bool downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || 
                       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
    if (sf::Joystick::isConnected(0)) {
        float axisY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
        if (axisY < -30.f) upPressed = true;
        if (axisY > 30.f) downPressed = true;
    }
    
    if (upPressed) {
        // LAUNCHER ATTACK - pops enemies up
        attackType = Attack::Type::Up;
        damageMultiplier *= 0.8f;  // Slightly less damage but launches
    } else if (downPressed && !m_onGround) {
        attackType = Attack::Type::Down;
    }
    
    // Apply Spirit Strike multiplier
    if (m_spiritStrikeActive) {
        damageMultiplier *= SPIRIT_STRIKE_DAMAGE_MULTIPLIER;
    }
    
    m_activeAttacks.push_back(
        std::make_unique<Attack>(attackType, m_position, m_facingRight, damageMultiplier)
    );
    m_attackCooldown = HEAVY_ATTACK_COOLDOWN;
    m_isAttacking = true;
    
    // Enhanced heavy attack particles (scale with charge)
    sf::Vector2f attackPos = m_position + sf::Vector2f(m_facingRight ? m_size.x : 0.f, m_size.y / 2.f);
    int particleCount = 15 + static_cast<int>(chargeLevel * 20.f);
    sf::Color particleColor = upPressed ? sf::Color(100, 255, 255) : sf::Color(255, 150, 50);
    
    // Main burst
    m_particles.emitBurst(attackPos, particleCount, particleColor, 100.f, 200.f + (chargeLevel * 150.f), 0.4f, 4.f);
    
    // Sparks for charged hits
    m_particles.emitSparks(attackPos, 10 + static_cast<int>(chargeLevel * 15), particleColor, 250.f, 0.35f);
    
    // Glow effect
    m_particles.emitGlow(attackPos, 8 + static_cast<int>(chargeLevel * 8), particleColor, 80.f, 0.7f, 8.f);
    
    // Fully charged attack creates massive explosion
    if (chargeLevel >= 1.0f) {
        m_particles.emitExplosion(attackPos, sf::Color(255, 255, 100), 3, 16);
        m_particles.emitRing(attackPos, sf::Color(255, 255, 255), 60.f, 20);
    }
    
    // Launcher effect (upward sparks)
    if (upPressed) {
        sf::Vector2f upDir(0.f, -1.f);
        m_particles.emitDirectional(attackPos, upDir, 20, sf::Color(100, 255, 255), 45.f, 300.f, 0.5f, 5.f);
    }
    
    // Consume Spirit Strike buff
    if (m_spiritStrikeActive) {
        m_spiritStrikeActive = false;
    }
    
    // Reset charge
    m_heavyChargeTime = 0.f;
}

void Player::performParry() {
    m_isParrying = true;
    m_parryDuration = PARRY_DURATION;
    m_parryElapsed = 0.0f;
    m_parryTimer = 0.0f;  // Reset timer when starting parry
    
    // Parry flash particles (slightly bigger for clarity)
    sf::Vector2f center = m_position + sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f);
    m_particles.emitBurst(center, 28, sf::Color(120, 210, 255), 140.f, 200.f, 0.45f, 3.5f);
}

void Player::activateSpiritStrike() {
    m_spiritStrikeActive = true;
    m_spiritStrikeCooldown = SPIRIT_STRIKE_COOLDOWN_TIME;
    
    // Enhanced Spirit Strike activation
    sf::Vector2f center = m_position + sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f);
    
    // Explosion burst
    m_particles.emitExplosion(center, sf::Color(150, 220, 255), 3, 12);
    
    // Glow aura
    m_particles.emitGlow(center, 20, sf::Color(200, 240, 255), 80.f, 1.0f, 10.f);
    
    // Rising sparks
    sf::Vector2f upDir(0.f, -1.f);
    m_particles.emitDirectional(center, upDir, 15, sf::Color(150, 220, 255), 60.f, 200.f, 0.8f, 5.f);
    
    // Radial ring
    m_particles.emitRing(center, sf::Color(255, 255, 255), 50.f, 16);
}

void Player::activateEchoStep() {
    m_echoActive = true;
    m_echoTimer = ECHO_STEP_DURATION;
    m_echoCooldown = ECHO_STEP_COOLDOWN;
    // Brief invulnerability during Echo Step
    m_invulnerable = true;
    m_invulnerabilityTimer = ECHO_STEP_DURATION;
    
    // Enhanced dash activation
    sf::Vector2f dashPos = m_position + sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f);
    
    // Burst with sparks
    m_particles.emitBurst(dashPos, 15, sf::Color(200, 240, 255), 150.f, 250.f, 0.5f, 3.f);
    m_particles.emitSparks(dashPos, 20, sf::Color(100, 200, 255), 300.f, 0.4f);
    
    // Dash direction indicator
    sf::Vector2f dashDir(m_facingRight ? 1.f : -1.f, 0.f);
    m_particles.emitDirectional(dashPos, dashDir, 10, sf::Color(255, 255, 255), 20.f, 400.f, 0.3f, 4.f);
}
