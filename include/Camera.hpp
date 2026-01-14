#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Camera system that follows a target (player)
 * 
 * Provides smooth camera movement with boundaries
 */
class Camera {
public:
    Camera(float windowWidth, float windowHeight);
    ~Camera() = default;
    
    /**
     * @brief Update camera to follow target position
     * @param targetPos Position to follow
     * @param deltaTime Time since last update
     */
    void update(const sf::Vector2f& targetPos, float deltaTime);
    
    /**
     * @brief Get the camera view for rendering
     */
    sf::View getView() const;
    
    /**
     * @brief Shake the camera for impact feedback
     */
    void shake(float intensity = 10.f, float duration = 0.2f);
    
    /**
     * @brief Set level boundaries to constrain camera
     */
    void setLevelBounds(float left, float top, float right, float bottom);
    
    /**
     * @brief Set camera smoothing speed (higher = faster)
     */
    void setSmoothSpeed(float speed) { m_smoothSpeed = speed; }
    
private:
    sf::View m_view;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    
    // Camera bounds
    float m_minX, m_maxX;
    float m_minY, m_maxY;
    bool m_hasBounds;
    
    // Smoothing
    float m_smoothSpeed;
    
    // Screen shake
    bool m_isShaking;
    float m_shakeIntensity;
    float m_shakeDuration;
    float m_shakeTimer;
    sf::Vector2f m_shakeOffset;
};
