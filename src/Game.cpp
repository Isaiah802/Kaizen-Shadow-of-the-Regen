#include "Game.hpp"
#include "Physics.hpp"
#include "Attack.hpp"
#include <iostream>
#include <algorithm>

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Nine Sols-like Game", sf::Style::Close)
    , m_player(std::make_unique<Player>())
    , m_camera(std::make_unique<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT))
    , m_hud(std::make_unique<HUD>(WINDOW_WIDTH, WINDOW_HEIGHT))
{
    std::cout << "Creating window and player..." << std::endl;
    m_window.setFramerateLimit(FPS_LIMIT);
    m_window.setVerticalSyncEnabled(true);
    
    std::cout << "Initializing level..." << std::endl;
    initializeLevel();
    
    // Set camera bounds to level size
    m_camera->setLevelBounds(0.f, 0.f, 2560.f, 720.f);
    
    std::cout << "Game initialized successfully!" << std::endl;
}

Game::~Game() {
    std::cout << "Game shutting down..." << std::endl;
}

void Game::run() {
    while (m_window.isOpen()) {
        float deltaTime = m_clock.restart().asSeconds();
        
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }
        
        // Handle keyboard input
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                m_window.close();
            }
            // Debug: teleport near boss for quick testing
            if (keyPressed->code == sf::Keyboard::Key::T) {
                if (m_player) {
                    m_player->setPosition(sf::Vector2f(2250.f, 520.f));
                }
            }
        }
    }
}

void Game::update(float deltaTime) {
    // Update effects manager and check for hit freeze
    m_effectsManager.update(deltaTime);
    
    // Skip game updates if hit freeze is active
    if (m_effectsManager.getHitFreeze().isActive()) {
        return;
    }
    
    // Update player
    if (m_player) {
        m_player->update(deltaTime);
        handlePlatformCollisions();
        
        // Update camera to follow player
        if (m_camera) {
            m_camera->update(m_player->getPosition(), deltaTime);
        }
    }
    
    // Update enemies and set player as target
    for (auto& enemy : m_enemies) {
        if (enemy && enemy->isActive()) {
            enemy->setTarget(m_player->getPosition());
            enemy->update(deltaTime);
        }
    }
    
    // Update boss
    if (m_boss && m_boss->isActive()) {
        m_boss->setTarget(m_player->getPosition());
        m_boss->update(deltaTime);
    }
    
    // Handle combat (attacks hitting entities)
    handleCombat();
}

void Game::render() {
    // Clear with a dark background color
    m_window.clear(sf::Color(20, 20, 30));
    
    // Apply camera view
    if (m_camera) {
        m_window.setView(m_camera->getView());
    }
    
    // Draw ground line (visual reference)
    sf::RectangleShape ground(sf::Vector2f(WINDOW_WIDTH, 10.f));
    ground.setPosition(sf::Vector2f(0.f, 600.f));
    ground.setFillColor(sf::Color(100, 100, 100));
    m_window.draw(ground);
    
    // Render platforms
    for (const auto& platform : m_platforms) {
        platform.render(m_window);
    }
    
    // Render enemies
    for (const auto& enemy : m_enemies) {
        if (enemy && enemy->isActive()) {
            enemy->render(m_window);
        }
    }
    
    // Render boss
    if (m_boss && m_boss->isActive()) {
        m_boss->render(m_window);
    }
    
    // Render player
    if (m_player) {
        m_player->render(m_window);
    }
    
    // Render visual effects (flash effects, attack trails, etc.)
    m_effectsManager.render(m_window);

    // Render HUD overlay
    if (m_hud) {
        m_hud->render(m_window, m_player.get(), m_boss.get());
    }
    
    // Display everything
    m_window.display();
}

void Game::initializeLevel() {
    // Create a larger test level that requires camera scrolling
    
    // Ground platform (extended width for scrolling)
    m_platforms.emplace_back(0.f, 600.f, 2560.f, 120.f);
    
    // Starting area - staircase up
    m_platforms.emplace_back(200.f, 500.f, 150.f, 20.f);
    m_platforms.emplace_back(400.f, 400.f, 150.f, 20.f);
    m_platforms.emplace_back(600.f, 300.f, 150.f, 20.f);
    m_platforms.emplace_back(800.f, 200.f, 150.f, 20.f);
    
    // Wall jump challenge area - tall walls for wall slide/jump practice
    m_platforms.emplace_back(950.f, 200.f, 20.f, 400.f);  // Left wall
    m_platforms.emplace_back(1200.f, 300.f, 20.f, 300.f);  // Right wall
    
    // High platform area
    m_platforms.emplace_back(1000.f, 150.f, 300.f, 20.f);
    m_platforms.emplace_back(1100.f, 250.f, 100.f, 20.f);
    
    // Mid-section with gaps and walls
    m_platforms.emplace_back(1400.f, 450.f, 150.f, 20.f);
    m_platforms.emplace_back(1550.f, 350.f, 20.f, 250.f);  // Wall for practice
    m_platforms.emplace_back(1600.f, 350.f, 150.f, 20.f);
    m_platforms.emplace_back(1800.f, 250.f, 150.f, 20.f);
    
    // End area - descending
    m_platforms.emplace_back(2000.f, 200.f, 150.f, 20.f);
    m_platforms.emplace_back(2200.f, 300.f, 150.f, 20.f);
    m_platforms.emplace_back(2350.f, 450.f, 200.f, 20.f);
    
    // Some floating challenge platforms with wall jump opportunities
    m_platforms.emplace_back(100.f, 250.f, 80.f, 20.f);
    m_platforms.emplace_back(1350.f, 150.f, 80.f, 20.f);
    m_platforms.emplace_back(1280.f, 200.f, 20.f, 200.f);  // Tall wall for wall climbing
    
    // Spawn enemies at strategic locations
    m_enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(500.f, 530.f), 150.f));
    m_enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(900.f, 530.f), 200.f));
    m_enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(1500.f, 380.f), 120.f));
    m_enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(2100.f, 530.f), 180.f));
    
    // Spawn boss at the end of the level
    m_boss = std::make_unique<Boss>(Boss::BossType::Executioner, sf::Vector2f(2300.f, 510.f));
}

void Game::handlePlatformCollisions() {
    if (!m_player) return;
    
    sf::FloatRect playerBounds = m_player->getBounds();
    sf::Vector2f playerVelocity = m_player->getVelocity();
    
    bool onGround = false;
    bool onWall = false;
    bool wallOnRight = false;
    bool onLedge = false;
    sf::Vector2f ledgePosition;
    
    for (const auto& platform : m_platforms) {
        sf::FloatRect platformBounds = platform.getBounds();
        
        // Check for collision
        if (playerBounds.findIntersection(platformBounds).has_value()) {
            // Determine collision side based on overlap
            float overlapLeft = (playerBounds.position.x + playerBounds.size.x) - platformBounds.position.x;
            float overlapRight = (platformBounds.position.x + platformBounds.size.x) - playerBounds.position.x;
            float overlapTop = (playerBounds.position.y + playerBounds.size.y) - platformBounds.position.y;
            float overlapBottom = (platformBounds.position.y + platformBounds.size.y) - playerBounds.position.y;
            
            // Find minimum overlap (primary collision direction)
            float minOverlap = std::min(std::min(overlapLeft, overlapRight), 
                                        std::min(overlapTop, overlapBottom));
            
            // Resolve collision based on direction
            if (minOverlap == overlapTop && playerVelocity.y > 0) {
                // Collision from top (player landing on platform)
                m_player->setPosition(playerBounds.position.x, 
                                     platformBounds.position.y - playerBounds.size.y);
                m_player->setVelocity(playerVelocity.x, 0.f);
                onGround = true;
            }
            else if (minOverlap == overlapBottom && playerVelocity.y < 0) {
                // Collision from bottom (player hitting head)
                m_player->setPosition(playerBounds.position.x, 
                                     platformBounds.position.y + platformBounds.size.y);
                m_player->setVelocity(playerVelocity.x, 0.f);
                
                // Check for ledge grab on platform edge
                if (!onGround) {
                    float platformLeftEdge = platformBounds.position.x;
                    float platformRightEdge = platformBounds.position.x + platformBounds.size.x;
                    float playerCenterX = playerBounds.position.x + playerBounds.size.x / 2.f;
                    
                    // Check if player is near edge and can grab
                    if (playerCenterX > platformLeftEdge - 40.f && playerCenterX < platformLeftEdge + 40.f) {
                        onLedge = true;
                        ledgePosition = sf::Vector2f(platformLeftEdge - 5.f, platformBounds.position.y);
                    } else if (playerCenterX > platformRightEdge - 40.f && playerCenterX < platformRightEdge + 40.f) {
                        onLedge = true;
                        ledgePosition = sf::Vector2f(platformRightEdge + 5.f, platformBounds.position.y);
                    }
                }
            }
            else if (minOverlap == overlapLeft && !onGround) {
                // Collision from left - wall on left side
                m_player->setPosition(platformBounds.position.x - playerBounds.size.x, 
                                     playerBounds.position.y);
                m_player->setVelocity(0.f, playerVelocity.y);
                onWall = true;
                wallOnRight = false;
            }
            else if (minOverlap == overlapRight && !onGround) {
                // Collision from right - wall on right side
                m_player->setPosition(platformBounds.position.x + platformBounds.size.x, 
                                     playerBounds.position.y);
                m_player->setVelocity(0.f, playerVelocity.y);
                onWall = true;
                wallOnRight = true;
            }
        }
    }
    
    m_player->setOnGround(onGround);
    
    // Update wall state
    if (!onGround) {
        m_player->setOnWall(onWall, wallOnRight);
    } else {
        m_player->setOnWall(false, false);
    }
    
    // Update ledge state (independent of wall/ground)
    if (onLedge && !onGround) {
        m_player->setOnLedge(true, ledgePosition);
    } else {
        m_player->setOnLedge(false, sf::Vector2f(0.f, 0.f));
    }
}

void Game::handleCombat() {
    if (!m_player) return;
    
    // Check player attacks hitting enemies
    const auto& playerAttacks = m_player->getActiveAttacks();
    for (const auto& attack : playerAttacks) {
        if (!attack || !attack->isActive()) continue;
        
        sf::FloatRect attackHitbox = attack->getHitbox();
        
        // Check each enemy
        for (auto& enemy : m_enemies) {
            if (!enemy || !enemy->isActive() || enemy->isDead()) continue;
            
            sf::FloatRect enemyBounds = enemy->getBounds();
            
            // Check if attack hits enemy
            if (Physics::checkCollision(attackHitbox, enemyBounds)) {
                int damage = attack->getDamage();
                enemy->takeDamage(damage);
                
                // Add flash effect on hit
                sf::Vector2f hitPos(
                    enemyBounds.position.x + enemyBounds.size.x / 2,
                    enemyBounds.position.y + enemyBounds.size.y / 2
                );
                m_effectsManager.addFlash(hitPos, sf::Color(255, 200, 100), 25.f, 0.15f);
                
                // Heavy attacks trigger hit freeze
                // m_effectsManager.getHitFreeze().trigger(0.08f);  // Disabled for smoother gameplay
                
                // Add attack trail
                m_effectsManager.addAttackTrail(
                    sf::Vector2f(attackHitbox.position.x, attackHitbox.position.y),
                    sf::Vector2f(attackHitbox.position.x + attackHitbox.size.x, attackHitbox.position.y + attackHitbox.size.y / 2),
                    sf::Color(255, 150, 50),
                    0.2f
                );
            }
        }
    }
    
    // Check enemy attacks hitting player
    for (const auto& enemy : m_enemies) {
        if (!enemy || !enemy->isActive() || enemy->isDead()) continue;
        
        if (enemy->isAttacking()) {
            sf::FloatRect enemyAttackHitbox = enemy->getAttackHitbox();
            sf::FloatRect playerBounds = m_player->getBounds();
            
            // Check if enemy attack hits player
            if (Physics::checkCollision(enemyAttackHitbox, playerBounds)) {
                // Check if player is parrying
                if (m_player->isParrying()) {
                    const bool perfect = m_player->isPerfectParryWindow();
                    // Hitstop for feedback
                    m_effectsManager.getHitFreeze().trigger(perfect ? 0.10f : 0.06f);
                    if (m_camera) {
                        m_camera->shake(perfect ? 8.f : 5.f, perfect ? 0.2f : 0.15f);
                    }
                    // Parry flash effect (bigger/brighter on perfect)
                    sf::Vector2f parryPos(
                        playerBounds.position.x + playerBounds.size.x / 2,
                        playerBounds.position.y + playerBounds.size.y / 2
                    );
                    m_effectsManager.addFlash(parryPos,
                        perfect ? sf::Color(170, 230, 255) : sf::Color(120, 210, 255),
                        perfect ? 45.f : 32.f,
                        perfect ? 0.25f : 0.18f);
                } else {
                    // Player takes damage (Entity handles invulnerability)
                    m_player->takeDamage(enemy->getAttackDamage());
                    // Shake only if damage applied (not invulnerable)
                    if (m_camera && !m_player->isInvulnerable()) {
                        m_camera->shake(12.f, 0.3f);
                        
                        // Add hit flash on player
                        sf::Vector2f hitPos(
                            playerBounds.position.x + playerBounds.size.x / 2,
                            playerBounds.position.y + playerBounds.size.y / 2
                        );
                        m_effectsManager.addFlash(hitPos, sf::Color(255, 50, 50), 20.f, 0.15f);
                        // m_effectsManager.getHitFreeze().trigger(0.05f);  // Disabled for smoother gameplay
                    }
                }
            }
        }
    }
    
    // Boss vs Player combat
    if (m_boss && m_boss->isActive()) {
        // Check if boss attacks hit player
        if (m_boss->isAttacking()) {
            sf::FloatRect bossAttackHitbox = m_boss->getAttackHitbox();
            sf::FloatRect playerBounds = m_player->getBounds();
            
            if (Physics::checkCollision(bossAttackHitbox, playerBounds)) {
                if (m_player->isParrying()) {
                    const bool perfect = m_player->isPerfectParryWindow();
                    m_effectsManager.getHitFreeze().trigger(perfect ? 0.12f : 0.08f);
                    if (m_camera) {
                        m_camera->shake(perfect ? 12.f : 8.f, perfect ? 0.28f : 0.2f);
                    }
                    // Add intense parry flash (bigger on perfect)
                    sf::Vector2f parryPos(
                        playerBounds.position.x + playerBounds.size.x / 2,
                        playerBounds.position.y + playerBounds.size.y / 2
                    );
                    m_effectsManager.addFlash(parryPos,
                        perfect ? sf::Color(190, 240, 255) : sf::Color(150, 220, 255),
                        perfect ? 55.f : 40.f,
                        perfect ? 0.3f : 0.25f);
                } else {
                    m_player->takeDamage(m_boss->getAttackDamage());
                    if (m_camera && !m_player->isInvulnerable()) {
                        m_camera->shake(15.f, 0.4f);  // Stronger shake for boss hits
                        
                        // Add strong hit flash
                        sf::Vector2f hitPos(
                            playerBounds.position.x + playerBounds.size.x / 2,
                            playerBounds.position.y + playerBounds.size.y / 2
                        );
                        m_effectsManager.addFlash(hitPos, sf::Color(255, 100, 100), 35.f, 0.2f);
                        // m_effectsManager.getHitFreeze().trigger(0.12f);  // Longer freeze for boss hits - Disabled for smoother gameplay
                    }
                }
            }
        }
        
        // Check if player attacks hit boss
        for (const auto& attack : m_player->getActiveAttacks()) {
            if (attack && attack->isActive()) {
                sf::FloatRect attackHitbox = attack->getHitbox();
                sf::FloatRect bossBounds = m_boss->getBounds();
                
                if (Physics::checkCollision(attackHitbox, bossBounds)) {
                    int damage = attack->getDamage();
                    m_boss->takeDamage(damage);
                    if (m_camera) {
                        m_camera->shake(10.f, 0.25f);
                    }
                    
                    // Add powerful hit flash for boss
                    sf::Vector2f hitPos(
                        bossBounds.position.x + bossBounds.size.x / 2,
                        bossBounds.position.y + bossBounds.size.y / 2
                    );
                    m_effectsManager.addFlash(hitPos, sf::Color(255, 180, 80), 35.f, 0.2f);
                    
                    // Heavy attacks on boss trigger longer freeze
                    // m_effectsManager.getHitFreeze().trigger(0.1f);  // Disabled for smoother gameplay
                    
                    // Add attack trail for boss hits
                    m_effectsManager.addAttackTrail(
                        sf::Vector2f(attackHitbox.position.x, attackHitbox.position.y),
                        sf::Vector2f(attackHitbox.position.x + attackHitbox.size.x, attackHitbox.position.y + attackHitbox.size.y / 2),
                        sf::Color(255, 200, 100),
                        0.25f
                    );
                }
            }
        }
    }
}

