#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Generates high-quality pixel art sprites in a consistent aesthetic
 * Designed to match Nine Sols' visual style with hand-drawn pixel art quality
 */
namespace PixelArtGenerator {
    
    /**
     * Create a detailed player sprite sheet (512x256)
     * 8 rows × 8 frames = 64 animation frames total
     */
    sf::Texture createPlayerSpriteSheet();
    
    /**
     * Create a detailed enemy sprite sheet (320x160)
     * 5 rows × 8 frames = 40 animation frames
     */
    sf::Texture createEnemySpriteSheet();
    
    /**
     * Create a detailed boss sprite sheet (320x160)
     * 5 rows × 8 frames for varied boss animations
     */
    sf::Texture createBossSpriteSheet();
    
}  // namespace PixelArtGenerator
