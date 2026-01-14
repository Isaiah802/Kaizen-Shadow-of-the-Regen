#include "Camera.hpp"
#include <cmath>
#include <cstdlib>

Camera::Camera(float windowWidth, float windowHeight)
    : m_view(sf::FloatRect({0.f, 0.f}, {windowWidth, windowHeight}))
    , m_position(windowWidth / 2.f, windowHeight / 2.f)
    , m_size(windowWidth, windowHeight)
    , m_minX(0.f)
    , m_maxX(windowWidth)
    , m_minY(0.f)
    , m_maxY(windowHeight)
    , m_hasBounds(false)
    , m_smoothSpeed(5.f)
    , m_isShaking(false)
    , m_shakeIntensity(0.f)
    , m_shakeDuration(0.f)
    , m_shakeTimer(0.f)
    , m_shakeOffset(0.f, 0.f)
{
    m_view.setCenter(m_position);
}

void Camera::update(const sf::Vector2f& targetPos, float deltaTime) {
    // Update screen shake
    if (m_isShaking) {
        m_shakeTimer -= deltaTime;
        if (m_shakeTimer <= 0.f) {
            m_isShaking = false;
            m_shakeOffset = sf::Vector2f(0.f, 0.f);
        } else {
            // Random shake offset
            float offsetX = ((rand() % 200 - 100) / 100.f) * m_shakeIntensity;
            float offsetY = ((rand() % 200 - 100) / 100.f) * m_shakeIntensity;
            m_shakeOffset = sf::Vector2f(offsetX, offsetY);
        }
    }
    
    // Calculate target camera position (centered on target)
    sf::Vector2f targetCameraPos = targetPos;
    
    // Smooth movement (lerp towards target)
    float lerpFactor = 1.f - std::exp(-m_smoothSpeed * deltaTime);
    m_position.x += (targetCameraPos.x - m_position.x) * lerpFactor;
    m_position.y += (targetCameraPos.y - m_position.y) * lerpFactor;
    
    // Apply bounds if set
    if (m_hasBounds) {
        float halfWidth = m_size.x / 2.f;
        float halfHeight = m_size.y / 2.f;
        
        if (m_position.x - halfWidth < m_minX) {
            m_position.x = m_minX + halfWidth;
        }
        if (m_position.x + halfWidth > m_maxX) {
            m_position.x = m_maxX - halfWidth;
        }
        if (m_position.y - halfHeight < m_minY) {
            m_position.y = m_minY + halfHeight;
        }
        if (m_position.y + halfHeight > m_maxY) {
            m_position.y = m_maxY - halfHeight;
        }
    }
    
    // Apply screen shake offset
    m_view.setCenter(m_position + m_shakeOffset);
}

sf::View Camera::getView() const {
    return m_view;
}

void Camera::shake(float intensity, float duration) {
    m_isShaking = true;
    m_shakeIntensity = intensity;
    m_shakeDuration = duration;
    m_shakeTimer = duration;
}

void Camera::setLevelBounds(float left, float top, float right, float bottom) {
    m_minX = left;
    m_maxX = right;
    m_minY = top;
    m_maxY = bottom;
    m_hasBounds = true;
}
