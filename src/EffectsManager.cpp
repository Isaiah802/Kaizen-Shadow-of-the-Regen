#include "EffectsManager.hpp"
#include <algorithm>

EffectsManager::EffectsManager() {
    // Create a default font programmatically for damage numbers
    // In a real game, this would load from file
}

void EffectsManager::update(float deltaTime) {
    // Update hit freeze
    m_hitFreeze.update(deltaTime);
    
    // Don't update other effects during hit freeze
    if (m_hitFreeze.isActive()) {
        return;
    }
    
    // Update flashes
    for (auto& flash : m_flashes) {
        flash->update(deltaTime);
    }
    
    m_flashes.erase(
        std::remove_if(m_flashes.begin(), m_flashes.end(),
            [](const std::unique_ptr<FlashEffect>& flash) {
                return !flash->isAlive();
            }),
        m_flashes.end()
    );
    
    // Update damage numbers
    for (auto& num : m_damageNumbers) {
        num->update(deltaTime);
    }
    
    m_damageNumbers.erase(
        std::remove_if(m_damageNumbers.begin(), m_damageNumbers.end(),
            [](const std::unique_ptr<DamageNumber>& num) {
                return !num->isAlive();
            }),
        m_damageNumbers.end()
    );
    
    // Update attack trail
    m_attackTrail.update(deltaTime);
}

void EffectsManager::render(sf::RenderWindow& window) const {
    // Render flashes
    for (const auto& flash : m_flashes) {
        flash->render(window);
    }
    
    // Render attack trails
    m_attackTrail.render(window);
    
    // Render damage numbers
    for (const auto& num : m_damageNumbers) {
        num->render(window);
    }
}

void EffectsManager::addFlash(const sf::Vector2f& position, const sf::Color& color, 
                              float radius, float duration) {
    m_flashes.push_back(std::make_unique<FlashEffect>(position, sf::Vector2f(radius * 2, radius * 2), color, duration));
}

void EffectsManager::addDamageNumber(int damage, const sf::Vector2f& position, const sf::Color& color) {
    (void)damage;
    (void)position;
    (void)color;
    // Only create if we have a valid font
    // For now, skip damage numbers if font isn't loaded
    // m_damageNumbers.push_back(std::make_unique<DamageNumber>(damage, position, color, m_font));
}

void EffectsManager::addAttackTrail(const sf::Vector2f& start, const sf::Vector2f& end, 
                                    const sf::Color& color, float lifetime) {
    (void)lifetime;
    // Calculate size from start and end
    sf::Vector2f size(end.x - start.x, end.y - start.y);
    m_attackTrail.addSegment(start, size, color);
}
