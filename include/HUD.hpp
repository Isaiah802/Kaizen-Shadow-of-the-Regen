#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Player;
class Boss;

/**
 * @brief HUD system for displaying game UI elements
 * 
 * Shows player health, ability cooldowns, boss health, and status effects
 */
class HUD {
public:
    HUD(unsigned int windowWidth, unsigned int windowHeight);
    
    /**
     * @brief Render all HUD elements
     * @param window Target render window
     * @param player Player reference for stats
     * @param boss Boss reference for health display
     */
    void render(sf::RenderWindow& window, const Player* player, const Boss* boss);
    
private:
    unsigned int m_windowWidth;
    unsigned int m_windowHeight;
    mutable sf::Font m_font;  // Would need actual font file in production
    bool m_fontLoaded;
    
    // Helper methods
    void drawCooldownBar(sf::RenderWindow& window, float x, float y, float width, float height,
                        float remaining, float max, const sf::Color& color, const std::string& label);
    void drawHealthBar(sf::RenderWindow& window, float x, float y, float width, float height,
                      int current, int max, const sf::Color& color, const std::string& label);
    void drawText(sf::RenderWindow& window, const std::string& text, float x, float y,
                 unsigned int size, const sf::Color& color);
};
