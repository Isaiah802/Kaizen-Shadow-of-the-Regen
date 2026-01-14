#include "ParticleSystem.hpp"
#include <cmath>
#include <random>
#include <algorithm>

namespace {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    float randomFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }
    
    sf::Vector2f randomDirection() {
        float angle = randomFloat(0.f, 6.28318f); // 2 * PI
        return sf::Vector2f(std::cos(angle), std::sin(angle));
    }
}

ParticleSystem::ParticleSystem()
    : m_quality(ParticleQuality::High)
    , m_qualityScale(1.0f) {
    m_particles.reserve(2000);  // Increased capacity
}

void ParticleSystem::setQuality(ParticleQuality quality) {
    m_quality = quality;
    switch (quality) {
        case ParticleQuality::Low: m_qualityScale = 0.5f; break;
        case ParticleQuality::Medium: m_qualityScale = 0.75f; break;
        case ParticleQuality::High: m_qualityScale = 1.0f; break;
    }
}

int ParticleSystem::scaleCount(int count) const {
    return static_cast<int>(std::ceil(count * m_qualityScale));
}

void ParticleSystem::emit(const sf::Vector2f& position, int count, const sf::Color& color,
                          float speed, float lifetime, float size) {
    int actualCount = std::max(1, scaleCount(count));
    for (int i = 0; i < actualCount; ++i) {
        sf::Vector2f dir = randomDirection();
        sf::Vector2f vel = dir * randomFloat(speed * 0.5f, speed);
        float life = randomFloat(lifetime * 0.8f, lifetime * 1.2f);
        float particleSize = randomFloat(size * 0.7f, size * 1.3f);
        
        m_particles.emplace_back(position, vel, color, life, particleSize);
    }
}

void ParticleSystem::emitBurst(const sf::Vector2f& position, int count, const sf::Color& color,
                               float minSpeed, float maxSpeed, float lifetime, float size) {
    int actualCount = std::max(1, scaleCount(count));
    for (int i = 0; i < actualCount; ++i) {
        sf::Vector2f dir = randomDirection();
        sf::Vector2f vel = dir * randomFloat(minSpeed, maxSpeed);
        float life = randomFloat(lifetime * 0.8f, lifetime * 1.2f);
        float particleSize = randomFloat(size * 0.7f, size * 1.3f);
        
        m_particles.emplace_back(position, vel, color, life, particleSize);
    }
}

void ParticleSystem::emitDirectional(const sf::Vector2f& position, const sf::Vector2f& direction,
                                    int count, const sf::Color& color, float spread,
                                    float speed, float lifetime, float size) {
    float baseAngle = std::atan2(direction.y, direction.x);
    float spreadRad = spread * 3.14159f / 180.f;
    int actualCount = std::max(1, scaleCount(count));
    
    for (int i = 0; i < actualCount; ++i) {
        float angle = baseAngle + randomFloat(-spreadRad, spreadRad);
        sf::Vector2f dir(std::cos(angle), std::sin(angle));
        sf::Vector2f vel = dir * randomFloat(speed * 0.7f, speed * 1.3f);
        float life = randomFloat(lifetime * 0.8f, lifetime * 1.2f);
        float particleSize = randomFloat(size * 0.7f, size * 1.3f);
        
        m_particles.emplace_back(position, vel, color, life, particleSize);
    }
}

void ParticleSystem::update(float deltaTime) {
    for (auto& particle : m_particles) {
        particle.update(deltaTime);
    }
    
    // Remove dead particles
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
            [](const Particle& p) { return !p.isAlive(); }),
        m_particles.end()
    );
}

// New enhanced particle emitters
void ParticleSystem::emitSparks(const sf::Vector2f& position, int count, const sf::Color& color,
                                float speed, float lifetime) {
    int actualCount = std::max(1, scaleCount(count));
    for (int i = 0; i < actualCount; ++i) {
        sf::Vector2f dir = randomDirection();
        sf::Vector2f vel = dir * randomFloat(speed * 0.8f, speed * 1.5f);
        Particle p(position, vel, color, randomFloat(lifetime * 0.7f, lifetime * 1.3f), 
                   randomFloat(3.f, 6.f), ParticleShape::Spark, false);
        p.rotationSpeed = randomFloat(-360.f, 360.f);
        m_particles.push_back(p);
    }
}

void ParticleSystem::emitGlow(const sf::Vector2f& position, int count, const sf::Color& color,
                              float speed, float lifetime, float size) {
    int actualCount = std::max(1, scaleCount(count));
    for (int i = 0; i < actualCount; ++i) {
        sf::Vector2f dir = randomDirection();
        sf::Vector2f vel = dir * randomFloat(speed * 0.5f, speed);
        Particle p(position, vel, color, randomFloat(lifetime * 0.8f, lifetime * 1.2f), 
                   randomFloat(size * 0.8f, size * 1.2f), ParticleShape::Circle, true);
        m_particles.push_back(p);
    }
}

void ParticleSystem::emitExplosion(const sf::Vector2f& position, const sf::Color& color,
                                  int ringCount, int particlesPerRing) {
    int actualRings = std::max(1, scaleCount(ringCount));
    int actualPerRing = std::max(1, scaleCount(particlesPerRing));
    for (int ring = 0; ring < actualRings; ++ring) {
        float ringSpeed = 150.f + (ring * 80.f);
        float ringDelay = ring * 0.05f;
        
        for (int i = 0; i < actualPerRing; ++i) {
            float angle = (static_cast<float>(i) / particlesPerRing) * 6.28318f;
            sf::Vector2f dir(std::cos(angle), std::sin(angle));
            sf::Vector2f vel = dir * ringSpeed;
            
            ParticleShape shape = (ring % 2 == 0) ? ParticleShape::Star : ParticleShape::Circle;
            Particle p(position, vel, color, 0.6f - ringDelay, randomFloat(4.f, 7.f), shape, true);
            p.rotationSpeed = randomFloat(-540.f, 540.f);
            m_particles.push_back(p);
        }
    }
}

void ParticleSystem::emitTrail(const sf::Vector2f& position, const sf::Color& color,
                              int count, float size) {
    int actualCount = std::max(1, scaleCount(count));
    for (int i = 0; i < actualCount; ++i) {
        sf::Vector2f vel = randomDirection() * randomFloat(20.f, 50.f);
        Particle p(position, vel, color, randomFloat(0.2f, 0.4f), 
                   randomFloat(size * 0.5f, size), ParticleShape::Circle, true);
        m_particles.push_back(p);
    }
}

void ParticleSystem::emitRing(const sf::Vector2f& position, const sf::Color& color,
                             float radius, int segments) {
    int actualSegments = std::max(1, scaleCount(segments));
    for (int i = 0; i < actualSegments; ++i) {
        float angle = (static_cast<float>(i) / segments) * 6.28318f;
        sf::Vector2f offset(std::cos(angle) * radius, std::sin(angle) * radius);
        sf::Vector2f vel(0.f, 0.f);  // Static ring particles
        
        Particle p(position + offset, vel, color, 0.5f, 5.f, ParticleShape::Ring, false);
        m_particles.push_back(p);
    }
}

void ParticleSystem::render(sf::RenderWindow& window) const {
    for (const auto& particle : m_particles) {
        // Render trail first (behind particle)
        if (!particle.trail.empty() && particle.trail.size() > 2) {
            renderTrail(window, particle);
        }
        
        // Render glow layer
        if (particle.hasGlow) {
            sf::CircleShape glow(particle.size * 2.5f);
            glow.setPosition(particle.position);
            sf::Color glowColor = particle.color;
            glowColor.a = static_cast<uint8_t>(glowColor.a * 0.3f);
            glow.setFillColor(glowColor);
            glow.setOrigin(sf::Vector2f(particle.size * 2.5f, particle.size * 2.5f));
            window.draw(glow);
        }
        
        // Render main particle
        renderParticle(window, particle);
    }
}

void ParticleSystem::renderParticle(sf::RenderWindow& window, const Particle& particle) const {
    switch (particle.shape) {
        case ParticleShape::Circle: {
            sf::CircleShape circle(particle.size);
            circle.setPosition(particle.position);
            circle.setFillColor(particle.color);
            circle.setOrigin(sf::Vector2f(particle.size, particle.size));
            window.draw(circle);
            break;
        }
        
        case ParticleShape::Square: {
            sf::RectangleShape square(sf::Vector2f(particle.size * 2.f, particle.size * 2.f));
            square.setPosition(particle.position);
            square.setFillColor(particle.color);
            square.setOrigin(sf::Vector2f(particle.size, particle.size));
            square.setRotation(sf::degrees(particle.rotation));
            window.draw(square);
            break;
        }
        
        case ParticleShape::Star: {
            sf::CircleShape star(particle.size, 5);  // 5 points
            star.setPosition(particle.position);
            star.setFillColor(particle.color);
            star.setOrigin(sf::Vector2f(particle.size, particle.size));
            star.setRotation(sf::degrees(particle.rotation));
            window.draw(star);
            break;
        }
        
        case ParticleShape::Spark: {
            // Elongated particle for spark effect
            sf::RectangleShape spark(sf::Vector2f(particle.size * 3.f, particle.size * 0.5f));
            spark.setPosition(particle.position);
            spark.setFillColor(particle.color);
            spark.setOrigin(sf::Vector2f(particle.size * 1.5f, particle.size * 0.25f));
            
            // Rotate in direction of movement
            float angle = std::atan2(particle.velocity.y, particle.velocity.x);
            spark.setRotation(sf::degrees(angle));
            window.draw(spark);
            break;
        }
        
        case ParticleShape::Ring: {
            sf::CircleShape ring(particle.size);
            ring.setPosition(particle.position);
            ring.setFillColor(sf::Color::Transparent);
            ring.setOutlineThickness(2.f);
            ring.setOutlineColor(particle.color);
            ring.setOrigin(sf::Vector2f(particle.size, particle.size));
            window.draw(ring);
            break;
        }
    }
}

void ParticleSystem::renderTrail(sf::RenderWindow& window, const Particle& particle) const {
    if (particle.trail.size() < 2) return;
    
    sf::VertexArray trail(sf::PrimitiveType::LineStrip, particle.trail.size());
    
    for (size_t i = 0; i < particle.trail.size(); ++i) {
        float alpha = (static_cast<float>(i) / particle.trail.size()) * particle.color.a * 0.5f;
        sf::Color trailColor = particle.color;
        trailColor.a = static_cast<uint8_t>(alpha);
        trail[i].position = particle.trail[i];
        trail[i].color = trailColor;
    }
    
    window.draw(trail);
}

void ParticleSystem::clear() {
    m_particles.clear();
}
