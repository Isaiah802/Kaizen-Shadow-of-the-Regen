#pragma once

#include "Entity.hpp"
#include "Attack.hpp"
#include "Animation.hpp"
#include "ParticleSystem.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <memory>
#include <vector>

/**
 * @brief Player character class
 * 
 * Handles player movement, input, rendering, and state
 */
class Player : public Entity {
public:
    Player();
    ~Player() override = default;
    
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;
    
    // Input handling
    void handleInput();
    
    // Player-specific methods
    void jump();
    bool isOnGround() const;
    void setOnGround(bool onGround);
    
    // Wall mechanics
    void setOnWall(bool onWall, bool wallOnRight);
    bool isOnWall() const { return m_onWall; }
    bool isWallSliding() const { return m_onWall && !m_onGround && m_velocity.y > 0; }
    
    // Ledge grab mechanics
    void setOnLedge(bool onLedge, const sf::Vector2f& ledgePos);
    bool isOnLedge() const { return m_onLedge; }
    bool isHangingFromLedge() const { return m_onLedge && !m_onGround; }
    void pullUpFromLedge();
    
    // Combat methods
    void performLightAttack();
    void performHeavyAttack();
    void startChargingHeavy();
    void releaseHeavyAttack();
    void performParry();
    void activateSpiritStrike();
    void activateEchoStep();
    bool isParrying() const { return m_isParrying; }
    bool isPerfectParryWindow() const { return m_isParrying && m_parryElapsed <= PARRY_PERFECT_WINDOW; }
    bool isSpiritStrikeActive() const { return m_spiritStrikeActive; }
    bool isEchoStepping() const { return m_echoActive; }
    bool isInvulnerable() const { return m_invulnerable; }
    // Combo getters
    int getComboCount() const { return m_comboCount; }
    float getComboTimer() const { return m_comboTimer; }
    // Heavy attack charge getters
    bool isChargingHeavy() const { return m_chargingHeavy; }
    float getHeavyChargeLevel() const { return m_heavyChargeTime / MAX_CHARGE_TIME; }
    // Cooldown getters for HUD
    float getParryCooldown() const { return m_parryTimer; }
    float getParryCooldownMax() const { return PARRY_COOLDOWN; }
    float getSpiritStrikeCooldown() const { return m_spiritStrikeCooldown; }
    float getSpiritStrikeCooldownMax() const { return SPIRIT_STRIKE_COOLDOWN_TIME; }
    float getEchoStepCooldown() const { return m_echoCooldown; }
    float getEchoStepCooldownMax() const { return ECHO_STEP_COOLDOWN; }
    const std::vector<std::unique_ptr<Attack>>& getActiveAttacks() const { return m_activeAttacks; }
    
private:
    // Movement constants
    static constexpr float MOVE_SPEED = 200.f;
    static constexpr float JUMP_FORCE = -400.f;
    static constexpr float ACCELERATION = 800.f;
    static constexpr float DECELERATION = 1200.f;
    
    // Wall mechanics
    static constexpr float WALL_SLIDE_SPEED = 60.f;  // Max fall speed while on wall
    static constexpr float WALL_JUMP_FORCE_X = 350.f;  // Horizontal push from wall
    static constexpr float WALL_JUMP_FORCE_Y = -380.f;  // Vertical jump from wall
    static constexpr float WALL_STICK_TIME = 0.15f;  // Grace period for wall jump input
    
    // State
    bool m_onGround;
    bool m_facingRight;
    int m_jumpCount;  // Track number of jumps (0, 1, or 2)
    static constexpr int MAX_JUMPS = 2;  // Allow double jump
    
    // Wall interaction
    bool m_onWall;
    bool m_wallOnRight;  // True if wall is on right side
    float m_wallSlideTimer;  // Timer for wall slide grace period
    
    // Input state
    float m_inputDirection;  // -1 (left), 0 (none), 1 (right)
    
    // Combat state
    bool m_isAttacking;
    float m_attackCooldown;
    bool m_isParrying;
    float m_parryDuration;
    float m_parryTimer;
    float m_parryElapsed;
    static constexpr float LIGHT_ATTACK_COOLDOWN = 0.4f;
    static constexpr float HEAVY_ATTACK_COOLDOWN = 0.7f;
    
    // Heavy attack charging
    bool m_chargingHeavy;
    float m_heavyChargeTime;
    static constexpr float MAX_CHARGE_TIME = 1.5f;
    static constexpr float MIN_CHARGE_TIME = 0.3f;
    static constexpr float LAUNCHER_FORCE = -450.f;  // Upward force for launcher
    
    // Combo system
    int m_comboCount;  // 0-3 (0 = no combo, 1-3 = combo hits)
    float m_comboTimer;  // Time remaining to continue combo
    float m_comboWindowTimer;  // Time until next attack can be input
    static constexpr float COMBO_WINDOW = 0.8f;  // Time to input next combo attack
    static constexpr float COMBO_ATTACK_DELAY = 0.35f;  // Delay between combo hits
    static constexpr float COMBO_1_DAMAGE = 10.f;
    static constexpr float COMBO_2_DAMAGE = 12.f;
    static constexpr float COMBO_3_DAMAGE = 20.f;  // Finisher does more damage
    static constexpr float PARRY_DURATION = 0.35f;   // Slightly longer active window for reliability
    static constexpr float PARRY_COOLDOWN = 0.60f;    // Faster reset to encourage use
    static constexpr float PARRY_PERFECT_WINDOW = 0.12f;  // First 120ms is perfect parry
    std::vector<std::unique_ptr<Attack>> m_activeAttacks;
    
    // Spirit Strike ability
    bool m_spiritStrikeActive;
    float m_spiritStrikeCooldown;
    static constexpr float SPIRIT_STRIKE_COOLDOWN_TIME = 3.0f;
    static constexpr float SPIRIT_STRIKE_DAMAGE_MULTIPLIER = 1.5f;
    
    // Echo Step (dash)
    bool m_echoActive;
    float m_echoTimer;
    float m_echoCooldown;
    float m_echoEndLagTimer;
    static constexpr float ECHO_STEP_DURATION = 0.18f;
    static constexpr float ECHO_STEP_COOLDOWN = 5.0f;
    static constexpr float ECHO_STEP_SPEED = 500.0f;
    static constexpr float ECHO_END_LAG = 0.08f;
    static constexpr float ECHO_CANCEL_WINDOW = 0.06f;
    
    // Ledge grab mechanics
    bool m_onLedge;
    sf::Vector2f m_ledgePosition;
    float m_ledgeGrabCooldown;
    bool m_pullingUp;
    float m_pullUpTimer;
    static constexpr float LEDGE_DETECTION_RANGE = 30.f;  // How far to search for ledges
    static constexpr float LEDGE_GRAB_COOLDOWN = 0.2f;  // Prevent spam grabbing
    static constexpr float PULL_UP_DURATION = 0.3f;  // Time to pull up
    static constexpr float PULL_UP_HEIGHT = 60.f;  // How high player rises
    
    // Coyote time & input buffering (game feel enhancements)
    float m_coyoteTimer;  // Grace period for jumping after leaving ground
    bool m_jumpInputBuffered;  // Whether jump input has been buffered
    float m_jumpInputBufferTime;  // How long to keep jump input buffered
    static constexpr float COYOTE_TIME = 0.13f;  // ~8 frames at 60fps grace period
    static constexpr float JUMP_INPUT_BUFFER = 0.08f;  // ~5 frames buffer window
    mutable std::optional<sf::Sprite> m_sprite;  // optional because SFML 3 sprite needs texture
    sf::Texture m_texture;  // Sprite sheet texture
    AnimationController m_animController;
    mutable ParticleSystem m_particles;  // mutable for const render method
    
    // Animation state
    enum class AnimState {
        Idle, Run, Jump, Fall, Attack, Parry, Dash, Ledge
    };
    AnimState m_currentAnimState;
    
    // Helper methods
    void createSpriteSheet();
    void setupAnimations();
    void updateAnimation(float deltaTime);
};
