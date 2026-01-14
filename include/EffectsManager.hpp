#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>

// Visual effect for hit freeze/hitstop
class HitFreeze {
public:
    HitFreeze() : m_active(false), m_duration(0.f) {}
    
    void trigger(float duration) {
        m_active = true;
        m_duration = duration;
    }
    
    void update(float deltaTime) {
        if (m_active) {
            m_duration -= deltaTime;
            if (m_duration <= 0.f) {
                m_active = false;
            }
        }
    }
    
    bool isActive() const { return m_active; }
    
private:
    bool m_active;
    float m_duration;
};

// Flash effect for impacts
class FlashEffect {
public:
    FlashEffect(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color, float duration)
        : m_shape(size), m_lifetime(duration), m_maxLifetime(duration) {
        m_shape.setPosition(position);
        m_shape.setFillColor(color);
    }
    
    void update(float deltaTime) {
        m_lifetime -= deltaTime;
        float alpha = (m_lifetime / m_maxLifetime) * 255.f;
        sf::Color col = m_shape.getFillColor();
        col.a = static_cast<unsigned char>(alpha);
        m_shape.setFillColor(col);
    }
    
    bool isAlive() const { return m_lifetime > 0.f; }
    
    void render(sf::RenderWindow& window) const {
        window.draw(m_shape);
    }
    
private:
    sf::RectangleShape m_shape;
    float m_lifetime;
    float m_maxLifetime;
};

// Damage number popup
class DamageNumber {
public:
    DamageNumber(int damage, const sf::Vector2f& position, const sf::Color& color, const sf::Font& font)
        : m_text(font), m_lifetime(1.5f), m_velocity(0.f, -80.f) {
        m_text.setFont(font);
        m_text.setString(std::to_string(damage));
        m_text.setCharacterSize(20);
        m_text.setFillColor(color);
        m_text.setOutlineColor(sf::Color::Black);
        m_text.setOutlineThickness(2.f);
        m_text.setPosition(position);
    }
    
    void update(float deltaTime) {
        m_lifetime -= deltaTime;
        m_text.move(m_velocity * deltaTime);
        
        // Fade out
        float alpha = (m_lifetime / 1.5f) * 255.f;
        sf::Color col = m_text.getFillColor();
        col.a = static_cast<unsigned char>(alpha);
        m_text.setFillColor(col);
        
        sf::Color outline = m_text.getOutlineColor();
        outline.a = static_cast<unsigned char>(alpha);
        m_text.setOutlineColor(outline);
    }
    
    bool isAlive() const { return m_lifetime > 0.f; }
    
    void render(sf::RenderWindow& window) const {
        window.draw(m_text);
    }
    
private:
    sf::Text m_text;
    float m_lifetime;
    sf::Vector2f m_velocity;
};

// Attack trail effect
class AttackTrail {
public:
    struct TrailSegment {
        sf::Vector2f position;
        sf::Vector2f size;
        float lifetime;
        sf::Color color;
    };
    
    AttackTrail() {}
    
    void addSegment(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color) {
        m_segments.push_back({pos, size, 0.15f, color});
    }
    
    void update(float deltaTime) {
        for (auto& seg : m_segments) {
            seg.lifetime -= deltaTime;
        }
        
        m_segments.erase(
            std::remove_if(m_segments.begin(), m_segments.end(),
                [](const TrailSegment& seg) { return seg.lifetime <= 0.f; }),
            m_segments.end()
        );
    }
    
    void render(sf::RenderWindow& window) const {
        for (const auto& seg : m_segments) {
            sf::RectangleShape rect(seg.size);
            rect.setPosition(seg.position);
            sf::Color col = seg.color;
            col.a = static_cast<unsigned char>((seg.lifetime / 0.15f) * 100.f);
            rect.setFillColor(col);
            window.draw(rect);
        }
    }
    
    void clear() {
        m_segments.clear();
    }
    
private:
    std::vector<TrailSegment> m_segments;
};

// Visual effects manager
class EffectsManager {
public:
    EffectsManager();
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;
    
    void addFlash(const sf::Vector2f& position, const sf::Color& color, 
                  float radius, float duration = 0.1f);
    void addDamageNumber(int damage, const sf::Vector2f& position, const sf::Color& color);
    void addAttackTrail(const sf::Vector2f& start, const sf::Vector2f& end, 
                        const sf::Color& color, float lifetime = 0.2f);
    
    HitFreeze& getHitFreeze() { return m_hitFreeze; }
    AttackTrail& getAttackTrail() { return m_attackTrail; }
    
private:
    std::vector<std::unique_ptr<FlashEffect>> m_flashes;
    std::vector<std::unique_ptr<DamageNumber>> m_damageNumbers;
    AttackTrail m_attackTrail;
    HitFreeze m_hitFreeze;
    sf::Font m_font;  // For damage numbers
};
