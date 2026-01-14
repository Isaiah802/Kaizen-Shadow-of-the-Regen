#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Player.hpp"
#include "Platform.hpp"
#include "Camera.hpp"
#include "Enemy.hpp"
#include "Boss.hpp"
#include "EffectsManager.hpp"
#include "HUD.hpp"

/**
 * @brief Main game class following the Game Loop pattern
 * 
 * This class manages the game window, handles events, updates game state,
 * and renders everything. Follows the standard game loop architecture.
 */
class Game {
public:
    Game();
    ~Game();
    
    // Delete copy constructor and assignment (best practice for game class)
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    
    /**
     * @brief Main game loop - runs until window is closed
     */
    void run();
    
private:
    /**
     * @brief Process all input events (keyboard, mouse, window events)
     */
    void processEvents();
    
    /**
     * @brief Update game logic
     * @param deltaTime Time elapsed since last frame in seconds
     */
    void update(float deltaTime);
    
    /**
     * @brief Render all game objects
     */
    void render();
    
    /**
     * @brief Initialize level with platforms
     */
    void initializeLevel();
    
    /**
     * @brief Handle collision between player and platforms
     */
    void handlePlatformCollisions();
    
    /**
     * @brief Handle hit detection between attacks and entities
     */
    void handleCombat();
    
private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    
    // Game objects
    std::unique_ptr<Player> m_player;
    std::vector<Platform> m_platforms;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::unique_ptr<Boss> m_boss;  // Single boss for the level
    std::unique_ptr<Camera> m_camera;
    EffectsManager m_effectsManager;  // Visual effects system
    std::unique_ptr<HUD> m_hud;  // UI display system
    
    // Concept art overlays
    std::vector<sf::Texture> m_conceptArtTextures;
    std::vector<std::optional<sf::Sprite>> m_conceptArtSprites;
    
    // Game constants
    static constexpr unsigned int WINDOW_WIDTH = 1280;
    static constexpr unsigned int WINDOW_HEIGHT = 720;
    static constexpr unsigned int FPS_LIMIT = 60;
};
