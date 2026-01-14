#include "Entity.hpp"

Entity::Entity()
    : m_position(0.f, 0.f)
    , m_velocity(0.f, 0.f)
    , m_size(32.f, 32.f)  // Default size
    , m_active(true)
    , m_health(100)
    , m_maxHealth(100)
    , m_invulnerable(false)
    , m_invulnerabilityTimer(0.0f)
{
}

void Entity::update(float deltaTime) {
    // Update invulnerability timer
    if (m_invulnerable) {
        m_invulnerabilityTimer -= deltaTime;
        if (m_invulnerabilityTimer <= 0.0f) {
            m_invulnerable = false;
        }
    }
    
    // Base update: apply velocity to position
    m_position += m_velocity * deltaTime;
}

void Entity::render(sf::RenderWindow& window) const {
    // Base class doesn't render anything
    // Derived classes override this
    (void)window; // Suppress unused parameter warning
}

void Entity::setPosition(const sf::Vector2f& pos) {
    m_position = pos;
}

void Entity::setPosition(float x, float y) {
    m_position = sf::Vector2f(x, y);
}

sf::Vector2f Entity::getPosition() const {
    return m_position;
}

void Entity::setVelocity(const sf::Vector2f& vel) {
    m_velocity = vel;
}

void Entity::setVelocity(float x, float y) {
    m_velocity = sf::Vector2f(x, y);
}

sf::Vector2f Entity::getVelocity() const {
    return m_velocity;
}

void Entity::setSize(const sf::Vector2f& size) {
    m_size = size;
}

void Entity::setSize(float width, float height) {
    m_size = sf::Vector2f(width, height);
}

sf::Vector2f Entity::getSize() const {
    return m_size;
}

sf::FloatRect Entity::getBounds() const {
    return sf::FloatRect(m_position, m_size);
}

bool Entity::isActive() const {
    return m_active;
}

void Entity::setActive(bool active) {
    m_active = active;
}

void Entity::takeDamage(int damage) {
    if (m_invulnerable || m_health <= 0) {
        return;  // Already dead or invulnerable
    }
    
    m_health -= damage;
    if (m_health < 0) {
        m_health = 0;
    }
    
    // Grant temporary invulnerability
    m_invulnerable = true;
    m_invulnerabilityTimer = INVULNERABILITY_DURATION;
    
    // Entity dies when health reaches 0
    if (m_health <= 0) {
        // Death logic can be overridden by subclasses
    }
}

void Entity::heal(int amount) {
    m_health += amount;
    if (m_health > m_maxHealth) {
        m_health = m_maxHealth;
    }
}

void Entity::setMaxHealth(int maxHealth) {
    m_maxHealth = maxHealth;
    if (m_health > m_maxHealth) {
        m_health = m_maxHealth;
    }
}
