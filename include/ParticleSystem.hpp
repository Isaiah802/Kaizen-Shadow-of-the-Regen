#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <deque>

enum class ParticleShape {
    Circle,
    Square,
    Star,
    Spark,
    Ring
};

class Particle {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;
    float maxLifetime;
    float size;
    float rotation;
    float rotationSpeed;
    ParticleShape shape;
    bool hasGlow;
    std::deque<sf::Vector2f> trail;  // For trail effects
    
    Particle(const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Color& col, 
             float life, float sz, ParticleShape sh = ParticleShape::Circle, bool glow = false)
        : position(pos), velocity(vel), color(col), lifetime(life), maxLifetime(life), 
          size(sz), rotation(0.f), rotationSpeed(0.f), shape(sh), hasGlow(glow) {}
    
    void update(float deltaTime) {
        position += velocity * deltaTime;
        lifetime -= deltaTime;
        
        // Add to trail
        if (trail.size() < 8) {
            trail.push_back(position);
        } else {
            trail.pop_front();
            trail.push_back(position);
        }
        
        // Fade out over lifetime
        float lifeRatio = lifetime / maxLifetime;
        float alpha = lifeRatio * 255.f;
        color.a = static_cast<unsigned char>(alpha);
        
        // Rotate
        rotation += rotationSpeed * deltaTime;
        
        // Shrink over time for some shapes
        if (shape == ParticleShape::Spark || shape == ParticleShape::Star) {
            size = size * (0.5f + lifeRatio * 0.5f);
        }
        
        // Gravity (varies by shape)
        if (shape != ParticleShape::Spark) {
            velocity.y += 400.f * deltaTime;
        } else {
            // Sparks have less gravity
            velocity.y += 100.f * deltaTime;
        }
        
        // Air resistance
        velocity *= 0.98f;
    }
    
    bool isAlive() const {
        return lifetime > 0.f;
    }
};

enum class ParticleQuality {
    Low,
    Medium,
    High
};

class ParticleSystem {
public:
    ParticleSystem();

    void setQuality(ParticleQuality quality);
    ParticleQuality getQuality() const { return m_quality; }
    
    void emit(const sf::Vector2f& position, int count, const sf::Color& color, 
              float speed = 150.f, float lifetime = 0.5f, float size = 3.f);
    
    void emitBurst(const sf::Vector2f& position, int count, const sf::Color& color,
                   float minSpeed, float maxSpeed, float lifetime, float size);
    
    void emitDirectional(const sf::Vector2f& position, const sf::Vector2f& direction,
                        int count, const sf::Color& color, float spread = 30.f,
                        float speed = 200.f, float lifetime = 0.4f, float size = 4.f);
    
    // New enhanced particle emitters
    void emitSparks(const sf::Vector2f& position, int count, const sf::Color& color,
                    float speed = 250.f, float lifetime = 0.3f);
    
    void emitGlow(const sf::Vector2f& position, int count, const sf::Color& color,
                  float speed = 80.f, float lifetime = 0.8f, float size = 8.f);
    
    void emitExplosion(const sf::Vector2f& position, const sf::Color& color,
                      int ringCount = 3, int particlesPerRing = 12);
    
    void emitTrail(const sf::Vector2f& position, const sf::Color& color,
                   int count = 5, float size = 4.f);
    
    void emitRing(const sf::Vector2f& position, const sf::Color& color,
                  float radius = 50.f, int segments = 16);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;
    void clear();
    
private:
    std::vector<Particle> m_particles;
    ParticleQuality m_quality;
    float m_qualityScale;

    int scaleCount(int count) const;
    
    void renderParticle(sf::RenderWindow& window, const Particle& particle) const;
    void renderTrail(sf::RenderWindow& window, const Particle& particle) const;
};
