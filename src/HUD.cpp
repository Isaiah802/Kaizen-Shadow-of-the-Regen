#include "HUD.hpp"
#include "Player.hpp"
#include "Boss.hpp"
#include <cmath>

HUD::HUD(unsigned int windowWidth, unsigned int windowHeight)
    : m_windowWidth(windowWidth)
    , m_windowHeight(windowHeight)
    , m_fontLoaded(false)
{
    // In production, load actual font file here
    // For now, we'll work without text rendering (SFML 3 requires explicit fonts)
}

void HUD::render(sf::RenderWindow& window, const Player* player, const Boss* boss) {
    const float padding = 15.f;
    const float barWidth = 180.f;
    const float barHeight = 14.f;
    const float spacing = 22.f;
    
    // ===== LEFT PANEL: Player Abilities & Status =====
    {
        float x = padding;
        float y = padding;
        
        // Player health bar
        if (player) {
            int health = player->getHealth();
            int maxHealth = player->getMaxHealth();
            drawHealthBar(window, x, y, barWidth, barHeight, health, maxHealth, 
                         sf::Color(200, 50, 50), "HEALTH");
            y += spacing + 8.f;
        }
        
        // Parry cooldown
        if (player) {
            float remaining = player->getParryCooldown();
            float max = player->getParryCooldownMax();
            drawCooldownBar(window, x, y, barWidth, barHeight, remaining, max, 
                           sf::Color(100, 180, 255), "PARRY");
            y += spacing;
        }
        
        // Spirit Strike cooldown
        if (player) {
            float remaining = player->getSpiritStrikeCooldown();
            float max = player->getSpiritStrikeCooldownMax();
            drawCooldownBar(window, x, y, barWidth, barHeight, remaining, max, 
                           sf::Color(150, 150, 255), "SPIRIT");
            y += spacing;
        }
        
        // Echo Step cooldown
        if (player) {
            float remaining = player->getEchoStepCooldown();
            float max = player->getEchoStepCooldownMax();
            drawCooldownBar(window, x, y, barWidth, barHeight, remaining, max, 
                           sf::Color(100, 255, 200), "ECHO");
            y += spacing;
        }
    }
    
    // ===== TOP CENTER: Boss Health =====
    if (boss && boss->isActive()) {
        float bossBarWidth = 500.f;
        float bossBarHeight = 16.f;
        float ox = (static_cast<float>(m_windowWidth) - bossBarWidth) / 2.f;
        float oy = padding;
        
        int bossHealth = boss->getHealth();
        int bossMaxHealth = boss->getMaxHealth();
        drawHealthBar(window, ox, oy, bossBarWidth, bossBarHeight, bossHealth, bossMaxHealth,
                     sf::Color(200, 80, 40), "BOSS");
    }
    
    // ===== CENTER: Combo Counter =====
    if (player && player->getComboCount() > 0) {
        float comboSize = 60.f + (player->getComboCount() * 20.f);  // Scale with combo
        float comboX = (static_cast<float>(m_windowWidth) - comboSize) / 2.f;
        float comboY = static_cast<float>(m_windowHeight) / 2.f - 100.f;
        
        // Combo number display
        sf::CircleShape comboCircle(comboSize / 2.f);
        comboCircle.setPosition(sf::Vector2f(comboX, comboY));
        
        // Color based on combo count
        sf::Color comboColor;
        switch (player->getComboCount()) {
            case 1: comboColor = sf::Color(255, 200, 100, 200); break;
            case 2: comboColor = sf::Color(255, 150, 50, 220); break;
            case 3: comboColor = sf::Color(255, 50, 50, 255); break;  // Finisher!
            default: comboColor = sf::Color(255, 255, 255, 200); break;
        }
        comboCircle.setFillColor(comboColor);
        comboCircle.setOutlineThickness(3.f);
        comboCircle.setOutlineColor(sf::Color(255, 255, 255, 180));
        window.draw(comboCircle);
        
        // Draw small dots to represent combo count visually
        int comboCount = player->getComboCount();
        for (int i = 0; i < comboCount && i < 3; ++i) {
            float dotSize = 8.f;
            float dotSpacing = 18.f;
            float dotX = comboX + comboSize / 2.f - ((comboCount - 1) * dotSpacing) / 2.f + i * dotSpacing;
            float dotY = comboY + comboSize / 2.f - dotSize / 2.f;
            
            sf::CircleShape dot(dotSize);
            dot.setPosition(sf::Vector2f(dotX - dotSize, dotY));
            dot.setFillColor(sf::Color(255, 255, 255));
            dot.setOutlineThickness(1.f);
            dot.setOutlineColor(sf::Color(200, 200, 200));
            window.draw(dot);
        }
        
        // Combo timer indicator (ring around circle)
        float timerFraction = player->getComboTimer() / 0.8f;  // COMBO_WINDOW
        if (timerFraction > 0.f) {
            sf::CircleShape timerRing(comboSize / 2.f + 5.f);
            timerRing.setPosition(sf::Vector2f(comboX - 5.f, comboY - 5.f));
            timerRing.setFillColor(sf::Color::Transparent);
            timerRing.setOutlineThickness(2.f);
            sf::Color ringColor = sf::Color(100, 255, 100);
            ringColor.a = static_cast<uint8_t>(255 * timerFraction);
            timerRing.setOutlineColor(ringColor);
            window.draw(timerRing);
        }
    }
    
    // ===== CENTER BOTTOM: Heavy Charge Indicator =====
    if (player && player->isChargingHeavy()) {
        float chargeLevel = player->getHeavyChargeLevel();
        float barWidth = 200.f;
        float barHeight = 20.f;
        float chargeX = (static_cast<float>(m_windowWidth) - barWidth) / 2.f;
        float chargeY = static_cast<float>(m_windowHeight) - 100.f;
        
        // Background
        sf::RectangleShape chargeBg(sf::Vector2f(barWidth, barHeight));
        chargeBg.setPosition(sf::Vector2f(chargeX, chargeY));
        chargeBg.setFillColor(sf::Color(40, 40, 40));
        chargeBg.setOutlineThickness(2.f);
        chargeBg.setOutlineColor(sf::Color(100, 100, 100));
        window.draw(chargeBg);
        
        // Charge fill
        sf::RectangleShape chargeFill(sf::Vector2f(barWidth * chargeLevel, barHeight));
        chargeFill.setPosition(sf::Vector2f(chargeX, chargeY));
        
        // Color changes as it charges
        if (chargeLevel >= 1.0f) {
            chargeFill.setFillColor(sf::Color(255, 255, 0));  // Max charge - yellow
            
            // Pulsing glow effect when fully charged
            float pulse = std::sin(std::clock() / 100000.f) * 0.4f + 0.6f;
            sf::RectangleShape chargeGlow(sf::Vector2f(barWidth, barHeight));
            chargeGlow.setPosition(sf::Vector2f(chargeX, chargeY));
            chargeGlow.setFillColor(sf::Color::Transparent);
            chargeGlow.setOutlineThickness(3.f);
            sf::Color glowColor = sf::Color(255, 255, 100);
            glowColor.a = static_cast<uint8_t>(200 * pulse);
            chargeGlow.setOutlineColor(glowColor);
            window.draw(chargeGlow);
        } else if (chargeLevel >= 0.5f) {
            chargeFill.setFillColor(sf::Color(255, 150, 50));  // Mid charge - orange
        } else {
            chargeFill.setFillColor(sf::Color(255, 100, 100));  // Starting - red
        }
        window.draw(chargeFill);
    }
    
    // ===== RIGHT PANEL: Status Effects & Info =====
    {
        float x = m_windowWidth - padding - 160.f;
        float y = padding;
        
        // Draw status indicators
        if (player) {
            // Wall slide indicator
            if (player->isWallSliding()) {
                sf::RectangleShape indicator(sf::Vector2f(140.f, 14.f));
                indicator.setFillColor(sf::Color(100, 150, 255, 150));
                indicator.setOutlineThickness(1.f);
                indicator.setOutlineColor(sf::Color(150, 200, 255));
                indicator.setPosition(sf::Vector2f(x + 10.f, y));
                window.draw(indicator);
                y += spacing;
            }
            
            // Ledge grab indicator
            if (player->isOnLedge()) {
                sf::RectangleShape indicator(sf::Vector2f(140.f, 14.f));
                indicator.setFillColor(sf::Color(200, 180, 100, 150));
                indicator.setOutlineThickness(1.f);
                indicator.setOutlineColor(sf::Color(220, 200, 150));
                indicator.setPosition(sf::Vector2f(x + 10.f, y));
                window.draw(indicator);
                y += spacing;
            }
            
            // Parry active indicator
            if (player->isParrying()) {
                sf::RectangleShape indicator(sf::Vector2f(140.f, 14.f));
                indicator.setFillColor(sf::Color(100, 200, 255, 200));
                indicator.setOutlineThickness(2.f);
                indicator.setOutlineColor(sf::Color(200, 255, 255));
                indicator.setPosition(sf::Vector2f(x + 10.f, y));
                window.draw(indicator);
                y += spacing;
            }
            
            // Spirit Strike active indicator
            if (player->isSpiritStrikeActive()) {
                sf::RectangleShape indicator(sf::Vector2f(140.f, 14.f));
                indicator.setFillColor(sf::Color(150, 150, 255, 200));
                indicator.setOutlineThickness(2.f);
                indicator.setOutlineColor(sf::Color(200, 200, 255));
                indicator.setPosition(sf::Vector2f(x + 10.f, y));
                window.draw(indicator);
                y += spacing;
            }
        }
    }
    
    // ===== BOTTOM CENTER: Debug Info =====
    #ifdef _DEBUG
    {
        float debugX = m_windowWidth / 2.f - 100.f;
        float debugY = m_windowHeight - 60.f;
        
        // Draw semi-transparent debug background
        sf::RectangleShape debugBg(sf::Vector2f(200.f, 50.f));
        debugBg.setFillColor(sf::Color(0, 0, 0, 100));
        debugBg.setOutlineThickness(1.f);
        debugBg.setOutlineColor(sf::Color(100, 100, 100));
        debugBg.setPosition(sf::Vector2f(debugX, debugY));
        window.draw(debugBg);
    }
    #endif
}

void HUD::drawCooldownBar(sf::RenderWindow& window, float x, float y, float width, float height,
                         float remaining, float max, const sf::Color& color, const std::string& label) {
    (void)label;
    
    // Ability icon (small colored circle)
    sf::CircleShape icon(height / 2.f);
    icon.setPosition(sf::Vector2f(x - height - 5.f, y));
    icon.setFillColor(color);
    icon.setOutlineThickness(1.f);
    icon.setOutlineColor(sf::Color(200, 200, 200));
    window.draw(icon);
    
    // Background
    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(40, 40, 40));
    background.setPosition(sf::Vector2f(x, y));
    window.draw(background);
    
    // Filled portion
    float fraction = max > 0.f ? std::max(0.f, std::min(1.f, 1.f - (remaining / max))) : 1.f;
    sf::RectangleShape fill(sf::Vector2f(width * fraction, height));
    fill.setFillColor(color);
    fill.setPosition(sf::Vector2f(x, y));
    window.draw(fill);
    
    // Border
    sf::RectangleShape border(sf::Vector2f(width, height));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(1.f);
    border.setOutlineColor(sf::Color(100, 100, 100));
    border.setPosition(sf::Vector2f(x, y));
    window.draw(border);
    
    // Ready indicator (glow when available)
    if (fraction >= 1.f) {
        sf::RectangleShape readyGlow(sf::Vector2f(width, height));
        readyGlow.setPosition(sf::Vector2f(x, y));
        readyGlow.setFillColor(sf::Color::Transparent);
        readyGlow.setOutlineThickness(2.f);
        sf::Color glowColor = color;
        float pulse = std::sin(std::clock() / 150000.f) * 0.3f + 0.7f;
        glowColor.a = static_cast<uint8_t>(180 * pulse);
        readyGlow.setOutlineColor(glowColor);
        window.draw(readyGlow);
    }
}

void HUD::drawHealthBar(sf::RenderWindow& window, float x, float y, float width, float height,
                       int current, int max, const sf::Color& color, const std::string& label) {
    (void)label;
    // Background
    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(40, 40, 40));
    background.setPosition(sf::Vector2f(x, y));
    window.draw(background);
    
    // Filled portion
    float fraction = max > 0 ? static_cast<float>(current) / static_cast<float>(max) : 0.f;
    fraction = std::max(0.f, std::min(1.f, fraction));
    sf::RectangleShape fill(sf::Vector2f(width * fraction, height));
    fill.setFillColor(color);
    fill.setPosition(sf::Vector2f(x, y));
    window.draw(fill);
    
    // Border
    sf::RectangleShape border(sf::Vector2f(width, height));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(1.5f);
    border.setOutlineColor(color);
    border.setPosition(sf::Vector2f(x, y));
    window.draw(border);
    
    // Damage flash effect (optional pulse)
    if (current < max) {
        // Subtle flashing border when damaged
        float pulse = std::sin(std::clock() / 300000.f) * 0.5f + 0.5f;
        sf::RectangleShape flashBorder(sf::Vector2f(width, height));
        flashBorder.setFillColor(sf::Color::Transparent);
        flashBorder.setOutlineThickness(0.5f);
        sf::Color flashColor = color;
        flashColor.a = static_cast<uint8_t>(100 * pulse);
        flashBorder.setOutlineColor(flashColor);
        flashBorder.setPosition(sf::Vector2f(x, y));
        window.draw(flashBorder);
    }
}

void HUD::drawText(sf::RenderWindow& window, const std::string& text, float x, float y,
                  unsigned int size, const sf::Color& color) {
    (void)window;
    (void)text;
    (void)x;
    (void)y;
    (void)size;
    (void)color;
    // Text rendering would go here with actual font
    // For SFML 3, would need explicit font loading
}
