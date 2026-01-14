#include "PixelArtGenerator.hpp"
#include <cmath>
#include <algorithm>

namespace PixelArtGenerator {

    void fillRect(sf::Image& img, int x, int y, int w, int h, const sf::Color& color) {
        for (int dy = 0; dy < h; ++dy) {
            for (int dx = 0; dx < w; ++dx) {
                int px = x + dx, py = y + dy;
                if (px >= 0 && px < static_cast<int>(img.getSize().x) &&
                    py >= 0 && py < static_cast<int>(img.getSize().y)) {
                    img.setPixel({static_cast<unsigned>(px), static_cast<unsigned>(py)}, color);
                }
            }
        }
    }
    
    void blendPixel(sf::Image& img, int x, int y, const sf::Color& color) {
        if (x >= 0 && x < static_cast<int>(img.getSize().x) &&
            y >= 0 && y < static_cast<int>(img.getSize().y)) {
            auto existing = img.getPixel({static_cast<unsigned>(x), static_cast<unsigned>(y)});
            sf::Color blended(
                static_cast<unsigned char>((existing.r + color.r) / 2),
                static_cast<unsigned char>((existing.g + color.g) / 2),
                static_cast<unsigned char>((existing.b + color.b) / 2),
                std::max(existing.a, color.a)
            );
            img.setPixel({static_cast<unsigned>(x), static_cast<unsigned>(y)}, blended);
        }
    }
    
    sf::Texture createPlayerSpriteSheet() {
        sf::Image img({1024, 320}, sf::Color::Transparent);
        
        const sf::Color skin{230, 190, 150, 255};
        const sf::Color darkSuit{20, 30, 50, 255};
        const sf::Color blueSuit{50, 90, 170, 255};
        const sf::Color blueLight{100, 140, 220, 255};
        const sf::Color metal{200, 200, 210, 255};
        const sf::Color gold{220, 180, 60, 255};
        const sf::Color black{10, 10, 10, 255};
        
        const int frameW = 128;
        const int frameH = 40;
        
        auto drawPlayer = [&](int frameX, int frameY, int type, int frame) {
            int bx = frameX * frameW + 10;
            int by = frameY * frameH + 2;
            
            if (type == 0) {  // IDLE - breathing animation with slight stance changes
                int breathOffset = (frame % 4 < 2) ? 0 : 1;  // Breathing effect
                fillRect(img, bx + 48, by + 28, 10, 8, metal);
                fillRect(img, bx + 70, by + 28, 10, 8, metal);
                fillRect(img, bx + 48, by + 20, 9, 8, darkSuit);
                fillRect(img, bx + 71, by + 20, 9, 8, darkSuit);
                fillRect(img, bx + 44, by + 10 - breathOffset, 36, 10, darkSuit);
                fillRect(img, bx + 46, by + 12 - breathOffset, 32, 6, blueSuit);
                fillRect(img, bx + 40, by + 12 - breathOffset, 4, 10, darkSuit);
                fillRect(img, bx + 80, by + 12 - breathOffset, 4, 10, darkSuit);
                fillRect(img, bx + 38, by + 14, 6, 12, skin);
                fillRect(img, bx + 82, by + 14, 6, 12, skin);
                fillRect(img, bx + 54, by + 8 - breathOffset, 8, 3, skin);
                fillRect(img, bx + 50, by - 2 - breathOffset, 16, 12, skin);
                img.setPixel({static_cast<unsigned>(bx + 54), static_cast<unsigned>(by + 2 - breathOffset)}, black);
                img.setPixel({static_cast<unsigned>(bx + 62), static_cast<unsigned>(by + 2 - breathOffset)}, black);
                // Add sword at rest
                if (frame < 4) {
                    fillRect(img, bx + 85, by + 18, 4, 16, metal);
                    fillRect(img, bx + 87, by + 16, 2, 20, sf::Color(255, 255, 220));
                }
            } else if (type == 1) {  // RUN - dynamic leg motion
                float phase = frame * 0.5f;
                int bob = static_cast<int>(std::sin(phase) * 2.0f);
                int legOff = (frame % 2 == 0) ? 0 : 3;
                fillRect(img, bx + 46, by + 28 + bob, 10, 8, metal);
                fillRect(img, bx + 68, by + 28 + bob, 10, 8, metal);
                fillRect(img, bx + 46, by + 20 - legOff, 9, 10 + legOff, darkSuit);
                fillRect(img, bx + 68, by + 20 + legOff, 9, 10 - legOff, darkSuit);
                fillRect(img, bx + 43, by + 10 + bob, 36, 10, darkSuit);
                fillRect(img, bx + 45, by + 12 + bob, 32, 6, blueSuit);
                if (frame % 2 == 0) {
                    fillRect(img, bx + 38, by + 12, 5, 14, skin);
                    fillRect(img, bx + 81, by + 12, 5, 14, skin);
                } else {
                    fillRect(img, bx + 38, by + 10, 5, 12, skin);
                    fillRect(img, bx + 81, by + 14, 5, 12, skin);
                }
                fillRect(img, bx + 50, by - 3 + bob, 16, 12, skin);
                // Arm swing during run
                int armOffset = (frame % 4 < 2) ? 2 : -2;
                fillRect(img, bx + 35 + armOffset, by + 14, 4, 12, skin);
            } else if (type == 2) {  // JUMP - distinct phases
                if (frame == 0) {  // Launch - body compressed
                    fillRect(img, bx + 48, by + 30, 20, 4, metal);
                    fillRect(img, bx + 50, by + 22, 16, 8, darkSuit);
                    fillRect(img, bx + 44, by + 12, 36, 10, darkSuit);
                    fillRect(img, bx + 46, by + 14, 32, 5, blueSuit);
                    fillRect(img, bx + 38, by + 8, 5, 16, skin);
                    fillRect(img, bx + 81, by + 8, 5, 16, skin);
                    fillRect(img, bx + 50, by - 2, 16, 12, skin);
                } else if (frame == 1 || frame == 2) {  // Peak ascent - stretched
                    fillRect(img, bx + 48, by + 20, 20, 6, metal);
                    fillRect(img, bx + 50, by + 12, 16, 8, darkSuit);
                    fillRect(img, bx + 44, by + 4, 36, 8, darkSuit);
                    fillRect(img, bx + 46, by + 5, 32, 5, blueSuit);
                    fillRect(img, bx + 36, by + 2, 6, 14, skin);
                    fillRect(img, bx + 82, by + 2, 6, 14, skin);
                    fillRect(img, bx + 50, by - 4, 16, 12, skin);
                } else {  // Descent - preparing to land
                    fillRect(img, bx + 48, by + 26, 20, 4, metal);
                    fillRect(img, bx + 50, by + 18, 16, 8, darkSuit);
                    fillRect(img, bx + 44, by + 10, 36, 8, darkSuit);
                    fillRect(img, bx + 46, by + 11, 32, 5, blueSuit);
                    fillRect(img, bx + 38, by + 6, 5, 12, skin);
                    fillRect(img, bx + 81, by + 6, 5, 12, skin);
                    fillRect(img, bx + 50, by - 2, 16, 12, skin);
                }
            } else if (type == 3) {  // FALL - falling pose
                fillRect(img, bx + 46, by + 26, 9, 6, metal);
                fillRect(img, bx + 69, by + 26, 9, 6, metal);
                fillRect(img, bx + 46, by + 16, 9, 10, darkSuit);
                fillRect(img, bx + 69, by + 16, 9, 10, darkSuit);
                fillRect(img, bx + 44, by + 10, 36, 8, darkSuit);
                fillRect(img, bx + 46, by + 11, 32, 5, blueSuit);
                fillRect(img, bx + 36, by + 10, 6, 14, skin);
                fillRect(img, bx + 82, by + 10, 6, 14, skin);
                fillRect(img, bx + 50, by + 0, 16, 12, skin);
            } else if (type == 4) {  // ATTACK - wind-up, strike, recovery
                if (frame == 0) {  // Wind-up start
                    fillRect(img, bx + 50, by + 28, 10, 8, metal);
                    fillRect(img, bx + 68, by + 28, 10, 8, metal);
                    fillRect(img, bx + 50, by + 20, 10, 8, darkSuit);
                    fillRect(img, bx + 68, by + 20, 10, 8, darkSuit);
                    fillRect(img, bx + 45, by + 10, 36, 10, darkSuit);
                    fillRect(img, bx + 47, by + 12, 32, 6, blueSuit);
                    fillRect(img, bx + 38, by + 14, 8, 12, skin);
                    fillRect(img, bx + 80, by + 12, 6, 14, skin);
                    fillRect(img, bx + 50, by - 2, 16, 12, skin);
                } else if (frame == 1 || frame == 2) {  // Wind-up/strike initiate
                    fillRect(img, bx + 48, by + 28, 10, 8, metal);
                    fillRect(img, bx + 70, by + 26, 8, 8, metal);
                    fillRect(img, bx + 48, by + 20, 10, 8, darkSuit);
                    fillRect(img, bx + 70, by + 18, 8, 8, darkSuit);
                    fillRect(img, bx + 42, by + 10, 38, 10, darkSuit);
                    fillRect(img, bx + 44, by + 12, 34, 6, blueSuit);
                    fillRect(img, bx + 32, by + 14, 10, 12, skin);
                    fillRect(img, bx + 82, by + 8, 6, 18, skin);
                    fillRect(img, bx + 50, by - 2, 16, 12, skin);
                } else if (frame == 3) {  // Full strike
                    fillRect(img, bx + 48, by + 28, 10, 8, metal);
                    fillRect(img, bx + 66, by + 28, 10, 8, metal);
                    fillRect(img, bx + 48, by + 20, 10, 8, darkSuit);
                    fillRect(img, bx + 66, by + 20, 10, 8, darkSuit);
                    fillRect(img, bx + 42, by + 10, 38, 10, darkSuit);
                    fillRect(img, bx + 44, by + 12, 34, 6, blueSuit);
                    fillRect(img, bx + 36, by + 14, 6, 12, skin);
                    fillRect(img, bx + 82, by + 10, 6, 16, skin);
                    // Full sword strike
                    int swordStart = bx + 90;
                    int swordY = by + 14;
                    for (int i = 0; i < 28; ++i) {
                        int curveY = swordY - i/4;
                        img.setPixel({static_cast<unsigned>(swordStart + i), static_cast<unsigned>(curveY)}, sf::Color(240, 240, 240));
                    }
                    for (int i = 0; i < 24; ++i) {
                        int curveY = swordY - i/4;
                        img.setPixel({static_cast<unsigned>(swordStart + i - 1), static_cast<unsigned>(curveY)}, sf::Color(180, 180, 200));
                    }
                    fillRect(img, swordStart - 2, swordY + 1, 8, 3, gold);
                    fillRect(img, swordStart, swordY + 3, 4, 8, sf::Color(120, 80, 40));
                    fillRect(img, bx + 50, by - 2, 16, 12, skin);
                } else {  // Recovery
                    fillRect(img, bx + 48, by + 28, 10, 8, metal);
                    fillRect(img, bx + 70, by + 28, 10, 8, metal);
                    fillRect(img, bx + 48, by + 20, 10, 8, darkSuit);
                    fillRect(img, bx + 70, by + 20, 10, 8, darkSuit);
                    fillRect(img, bx + 44, by + 10, 36, 10, darkSuit);
                    fillRect(img, bx + 46, by + 12, 32, 6, blueSuit);
                    fillRect(img, bx + 40, by + 14, 6, 12, skin);
                    fillRect(img, bx + 82, by + 12, 6, 14, skin);
                    fillRect(img, bx + 50, by - 2, 16, 12, skin);
                }
            } else if (type == 5) {  // PARRY - shield raise and hold
                if (frame < 2) {  // Raise shield
                    fillRect(img, bx + 50, by + 28, 10, 8, metal);
                    fillRect(img, bx + 68, by + 26, 8, 8, metal);
                    fillRect(img, bx + 50, by + 20, 10, 8, darkSuit);
                    fillRect(img, bx + 68, by + 18, 8, 8, darkSuit);
                    fillRect(img, bx + 45, by + 10, 36, 10, darkSuit);
                    fillRect(img, bx + 47, by + 12, 32, 6, blueSuit);
                    fillRect(img, bx + 36, by + 10, 6, 16, skin);
                    // Shield coming up
                    fillRect(img, bx + 28 - frame * 2, by + 8 - frame * 2, 14 + frame, 18 + frame, metal);
                    fillRect(img, bx + 30 - frame, by + 10 - frame, 10 + frame, 14 + frame, blueLight);
                    fillRect(img, bx + 82, by + 14, 6, 12, skin);
                    fillRect(img, bx + 50, by - 2, 16, 12, skin);
                } else {  // Shield held
                    fillRect(img, bx + 50, by + 28, 10, 8, metal);
                    fillRect(img, bx + 68, by + 28, 8, 8, metal);
                    fillRect(img, bx + 50, by + 20, 10, 8, darkSuit);
                    fillRect(img, bx + 68, by + 20, 8, 8, darkSuit);
                    fillRect(img, bx + 44, by + 10, 36, 10, darkSuit);
                    fillRect(img, bx + 46, by + 12, 32, 6, blueSuit);
                    fillRect(img, bx + 36, by + 10, 6, 16, skin);
                    // Full shield
                    fillRect(img, bx + 28, by + 6, 14, 20, metal);
                    fillRect(img, bx + 30, by + 8, 10, 16, blueLight);
                    fillRect(img, bx + 31, by + 9, 8, 14, blueSuit);
                    fillRect(img, bx + 28, by + 6, 14, 2, gold);
                    fillRect(img, bx + 28, by + 24, 14, 2, darkSuit);
                    fillRect(img, bx + 28, by + 6, 2, 20, darkSuit);
                    fillRect(img, bx + 40, by + 6, 2, 20, gold);
                    fillRect(img, bx + 32, by + 10, 2, 2, sf::Color(200, 220, 255));
                    fillRect(img, bx + 34, by + 12, 2, 2, sf::Color(180, 200, 240));
                    fillRect(img, bx + 82, by + 14, 6, 12, skin);
                    fillRect(img, bx + 50, by - 2, 16, 12, skin);
                }
            } else if (type == 6) {  // DASH - speed blur effect
                float dashLean = 8 - frame * 2;
                fillRect(img, bx + 44 + dashLean, by + 28, 12, 6, metal);
                fillRect(img, bx + 42 + dashLean, by + 20, 14, 8, darkSuit);
                fillRect(img, bx + 40 + dashLean, by + 10, 42, 10, darkSuit);
                fillRect(img, bx + 42 + dashLean, by + 12, 38, 6, blueSuit);
                fillRect(img, bx + 38 + dashLean, by + 12, 4, 12, skin);
                fillRect(img, bx + 82 + dashLean, by + 12, 4, 12, skin);
                fillRect(img, bx + 48 + dashLean, by - 2, 16, 12, skin);
                // Speed trail
                if (frame < 3) {
                    for (int i = 1; i <= 3; ++i) {
                        blendPixel(img, bx + 40 - i * 3, by + 14, sf::Color(100, 140, 220, 60 - i * 20));
                    }
                }
            } else if (type == 7) {  // SPIRIT STRIKE - energy charging
                sf::Color chargeColor = sf::Color(220 - frame * 10, 200 - frame * 15, 100 + frame * 10);
                sf::Color glowColor = sf::Color(220 + frame * 5, 200 + frame * 15, 150 + frame * 20);
                fillRect(img, bx + 48, by + 28, 10, 8, chargeColor);
                fillRect(img, bx + 66, by + 28, 10, 8, chargeColor);
                fillRect(img, bx + 48, by + 20, 10, 8, blueSuit);
                fillRect(img, bx + 44, by + 10, 36, 10, glowColor);
                fillRect(img, bx + 46, by + 12, 32, 6, chargeColor);
                fillRect(img, bx + 36, by + 10, 6, 16, skin);
                fillRect(img, bx + 82, by + 10, 6, 16, skin);
                for (int r = 1; r <= frame + 1; ++r) {
                    blendPixel(img, bx + 58 + r, by + 14, sf::Color(220, 200, 100, 80 - r * 10));
                    blendPixel(img, bx + 58 - r, by + 14, sf::Color(220, 200, 100, 80 - r * 10));
                }
                fillRect(img, bx + 50, by - 2, 16, 12, skin);
            }
        };
        
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                drawPlayer(col, row, row, col);
            }
        }
        
        sf::Texture texture;
        if (!texture.loadFromImage(img)) {
            // Failed to load, return empty texture
        }
        return texture;
    }
    
    sf::Texture createEnemySpriteSheet() {
        sf::Image img({640, 320}, sf::Color::Transparent);
        
        const sf::Color bodyRed{160, 40, 40, 255};
        const sf::Color darkRed{100, 20, 20, 255};
        const sf::Color brightRed{220, 80, 60, 255};
        const sf::Color eyeRed{255, 100, 80, 255};
        
        const int frameW = 80;
        const int frameH = 64;
        
        auto drawEnemy = [&](int frameX, int frameY, int type, int frame) {
            int bx = frameX * frameW + 5;
            int by = frameY * frameH + 5;
            
            if (type == 0) {  // PATROL
                fillRect(img, bx + 28, by + 48, 12, 6, darkRed);
                fillRect(img, bx + 40, by + 48, 12, 6, darkRed);
                fillRect(img, bx + 28, by + 38, 12, 10, bodyRed);
                fillRect(img, bx + 40, by + 38, 12, 10, bodyRed);
                fillRect(img, bx + 24, by + 20, 32, 18, bodyRed);
                fillRect(img, bx + 26, by + 22, 28, 14, brightRed);
                fillRect(img, bx + 18, by + 24, 6, 16, darkRed);
                fillRect(img, bx + 56, by + 24, 6, 16, darkRed);
                img.setPixel({static_cast<unsigned>(bx + 36), static_cast<unsigned>(by + 12)}, eyeRed);
                img.setPixel({static_cast<unsigned>(bx + 44), static_cast<unsigned>(by + 12)}, eyeRed);
                fillRect(img, bx + 30, by + 8, 20, 14, bodyRed);
            } else if (type == 1) {  // CHASE
                float phase = frame * 0.4f;
                int bob = static_cast<int>(std::sin(phase) * 2.0f);
                fillRect(img, bx + 28, by + 48 + bob, 12, 6, darkRed);
                fillRect(img, bx + 40, by + 48 + bob, 12, 6, darkRed);
                if (frame % 2 == 0) {
                    fillRect(img, bx + 26, by + 38, 12, 12, bodyRed);
                    fillRect(img, bx + 42, by + 40, 12, 10, bodyRed);
                } else {
                    fillRect(img, bx + 26, by + 40, 12, 10, bodyRed);
                    fillRect(img, bx + 42, by + 38, 12, 12, bodyRed);
                }
                fillRect(img, bx + 23, by + 22 + bob, 34, 16, bodyRed);
                fillRect(img, bx + 25, by + 24 + bob, 30, 12, brightRed);
                fillRect(img, bx + 16, by + 24, 7, 18, darkRed);
                fillRect(img, bx + 57, by + 24, 7, 18, darkRed);
                fillRect(img, bx + 30, by + 8 + bob, 20, 14, bodyRed);
                img.setPixel({static_cast<unsigned>(bx + 36), static_cast<unsigned>(by + 12 + bob)}, eyeRed);
                img.setPixel({static_cast<unsigned>(bx + 44), static_cast<unsigned>(by + 12 + bob)}, eyeRed);
            } else if (type == 2) {  // ATTACK
                fillRect(img, bx + 28, by + 48, 12, 6, darkRed);
                fillRect(img, bx + 40, by + 48, 12, 6, darkRed);
                fillRect(img, bx + 28, by + 38, 12, 10, bodyRed);
                fillRect(img, bx + 40, by + 38, 12, 10, bodyRed);
                fillRect(img, bx + 24, by + 20, 32, 18, bodyRed);
                fillRect(img, bx + 26, by + 22, 28, 14, brightRed);
                // Enhanced claws
                fillRect(img, bx + 12, by + 22, 10, 4, brightRed);  // Left arm
                fillRect(img, bx + 58, by + 22, 10, 4, brightRed);  // Right arm
                // Left claws
                for (int i = 0; i < 6; ++i) {
                    img.setPixel({static_cast<unsigned>(bx + 10 - i), static_cast<unsigned>(by + 24 + i)}, sf::Color(200, 50, 50));  // Claw tip
                    img.setPixel({static_cast<unsigned>(bx + 11 - i), static_cast<unsigned>(by + 24 + i)}, brightRed);                 // Claw shade
                }
                // Right claws
                for (int i = 0; i < 6; ++i) {
                    img.setPixel({static_cast<unsigned>(bx + 68 + i), static_cast<unsigned>(by + 24 + i)}, sf::Color(200, 50, 50));  // Claw tip
                    img.setPixel({static_cast<unsigned>(bx + 67 + i), static_cast<unsigned>(by + 24 + i)}, brightRed);                 // Claw shade
                }
                fillRect(img, bx + 18, by + 24, 6, 16, darkRed);
                fillRect(img, bx + 56, by + 24, 6, 16, darkRed);
                fillRect(img, bx + 30, by + 8, 20, 14, bodyRed);
                img.setPixel({static_cast<unsigned>(bx + 36), static_cast<unsigned>(by + 12)}, eyeRed);
                img.setPixel({static_cast<unsigned>(bx + 44), static_cast<unsigned>(by + 12)}, eyeRed);
            } else if (type == 3) {  // HIT
                sf::Color flashColor = (frame % 2 == 0) ? brightRed : bodyRed;
                fillRect(img, bx + 28, by + 48, 12, 6, darkRed);
                fillRect(img, bx + 40, by + 48, 12, 6, darkRed);
                fillRect(img, bx + 28, by + 38, 12, 10, flashColor);
                fillRect(img, bx + 40, by + 38, 12, 10, flashColor);
                fillRect(img, bx + 24, by + 20, 32, 18, flashColor);
                fillRect(img, bx + 26, by + 22, 28, 14, brightRed);
                fillRect(img, bx + 16, by + 20, 8, 20, flashColor);
                fillRect(img, bx + 56, by + 20, 8, 20, flashColor);
                fillRect(img, bx + 30, by + 8, 20, 14, flashColor);
            }
        };
        
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 8; ++col) {
                drawEnemy(col, row, std::min(row, 3), col);
            }
        }
        
        sf::Texture texture;
        if (!texture.loadFromImage(img)) {
            // Failed to load, return empty texture
        }
        return texture;
    }
    
    sf::Texture createBossSpriteSheet() {
        sf::Image img({640, 320}, sf::Color::Transparent);
        
        const sf::Color bodyPurple{80, 20, 80, 255};
        const sf::Color armorGold{220, 180, 60, 255};
        const sf::Color eyeRed{255, 100, 100, 255};
        
        const int frameW = 80;
        const int frameH = 64;
        
        auto drawBoss = [&](int frameX, int frameY, int type, int frame) {
            int bx = frameX * frameW + 5;
            int by = frameY * frameH + 5;
            
            if (type == 0) {  // IDLE
                fillRect(img, bx + 26, by + 48, 14, 8, armorGold);
                fillRect(img, bx + 40, by + 48, 14, 8, armorGold);
                fillRect(img, bx + 28, by + 38, 12, 10, bodyPurple);
                fillRect(img, bx + 40, by + 38, 12, 10, bodyPurple);
                fillRect(img, bx + 22, by + 18, 36, 20, bodyPurple);
                fillRect(img, bx + 24, by + 20, 32, 16, armorGold);
                fillRect(img, bx + 18, by + 24, 8, 16, armorGold);
                fillRect(img, bx + 54, by + 24, 8, 16, armorGold);
                fillRect(img, bx + 28, by + 6, 24, 14, bodyPurple);
                fillRect(img, bx + 28, by + 4, 24, 4, armorGold);
                img.setPixel({static_cast<unsigned>(bx + 35), static_cast<unsigned>(by + 10)}, eyeRed);
                img.setPixel({static_cast<unsigned>(bx + 45), static_cast<unsigned>(by + 10)}, eyeRed);
            } else if (type == 1) {  // MOVE
                float phase = frame * 0.3f;
                int bob = static_cast<int>(std::sin(phase) * 1.5f);
                fillRect(img, bx + 26, by + 48 + bob, 14, 8, armorGold);
                fillRect(img, bx + 40, by + 48 + bob, 14, 8, armorGold);
                fillRect(img, bx + 28, by + 38 + bob, 12, 10, bodyPurple);
                fillRect(img, bx + 40, by + 40 - bob, 12, 10, bodyPurple);
                fillRect(img, bx + 20, by + 18 + bob, 40, 20, bodyPurple);
                fillRect(img, bx + 22, by + 20 + bob, 36, 16, armorGold);
                fillRect(img, bx + 16, by + 24, 8, 18, armorGold);
                fillRect(img, bx + 56, by + 24, 8, 18, armorGold);
                fillRect(img, bx + 28, by + 6 + bob, 24, 14, bodyPurple);
                img.setPixel({static_cast<unsigned>(bx + 35), static_cast<unsigned>(by + 10 + bob)}, eyeRed);
                img.setPixel({static_cast<unsigned>(bx + 45), static_cast<unsigned>(by + 10 + bob)}, eyeRed);
            } else if (type == 2) {  // ATTACK1
                fillRect(img, bx + 26, by + 48, 14, 8, armorGold);
                fillRect(img, bx + 40, by + 48, 14, 8, armorGold);
                fillRect(img, bx + 28, by + 38, 12, 10, bodyPurple);
                fillRect(img, bx + 40, by + 38, 12, 10, bodyPurple);
                fillRect(img, bx + 20, by + 18, 40, 20, bodyPurple);
                fillRect(img, bx + 22, by + 20, 36, 16, armorGold);
                fillRect(img, bx + 16, by + 26, 8, 16, armorGold);
                fillRect(img, bx + 56, by + 22, 8, 20, armorGold);
                // Enhanced boss sword with shading
                int swordX = bx + 64;
                int swordY = by + 18;
                // Bright blade
                for (int i = 0; i < 24; ++i) {
                    int curveY = swordY - i/4;
                    img.setPixel({static_cast<unsigned>(swordX + i), static_cast<unsigned>(curveY)}, sf::Color(220, 200, 100));
                }
                // Dark edge shading
                for (int i = 0; i < 20; ++i) {
                    int curveY = swordY - i/4;
                    img.setPixel({static_cast<unsigned>(swordX + i - 1), static_cast<unsigned>(curveY)}, sf::Color(180, 140, 40));
                }
                // Highlight edge
                for (int i = 2; i < 20; ++i) {
                    int curveY = swordY - i/4;
                    img.setPixel({static_cast<unsigned>(swordX + i + 1), static_cast<unsigned>(curveY)}, sf::Color(255, 230, 150));
                }
                // Crossguard
                fillRect(img, swordX - 2, swordY + 1, 8, 3, sf::Color(200, 100, 200));
                // Handle
                fillRect(img, swordX, swordY + 3, 4, 8, sf::Color(100, 50, 100));
                fillRect(img, bx + 28, by + 6, 24, 14, bodyPurple);
                img.setPixel({static_cast<unsigned>(bx + 35), static_cast<unsigned>(by + 10)}, eyeRed);
                img.setPixel({static_cast<unsigned>(bx + 45), static_cast<unsigned>(by + 10)}, eyeRed);
            } else if (type == 3) {  // ATTACK2
                fillRect(img, bx + 26, by + 48, 14, 8, sf::Color(255, 200, 100));
                fillRect(img, bx + 40, by + 48, 14, 8, sf::Color(255, 200, 100));
                fillRect(img, bx + 28, by + 38, 12, 10, sf::Color(100, 50, 120));
                fillRect(img, bx + 40, by + 38, 12, 10, sf::Color(100, 50, 120));
                fillRect(img, bx + 20, by + 16, 40, 22, bodyPurple);
                fillRect(img, bx + 22, by + 18, 36, 18, sf::Color(255, 150, 100));
                fillRect(img, bx + 14, by + 22, 10, 22, sf::Color(255, 200, 100));
                fillRect(img, bx + 56, by + 22, 10, 22, sf::Color(255, 200, 100));
                for (int r = 1; r <= 4; ++r) {
                    blendPixel(img, bx + 40 + r, by + 28, sf::Color(255, 200, 100, 100 - r * 25));
                    blendPixel(img, bx + 40 - r, by + 28, sf::Color(255, 200, 100, 100 - r * 25));
                }
                fillRect(img, bx + 28, by + 4, 24, 14, bodyPurple);
                img.setPixel({static_cast<unsigned>(bx + 35), static_cast<unsigned>(by + 8)}, sf::Color(255, 200, 100));
                img.setPixel({static_cast<unsigned>(bx + 45), static_cast<unsigned>(by + 8)}, sf::Color(255, 200, 100));
            } else if (type == 4) {  // PHASE 2
                fillRect(img, bx + 24, by + 48, 16, 8, sf::Color(255, 200, 100));
                fillRect(img, bx + 40, by + 48, 16, 8, sf::Color(255, 200, 100));
                fillRect(img, bx + 26, by + 36, 14, 12, sf::Color(120, 50, 140));
                fillRect(img, bx + 40, by + 36, 14, 12, sf::Color(120, 50, 140));
                fillRect(img, bx + 18, by + 14, 44, 24, bodyPurple);
                fillRect(img, bx + 20, by + 16, 40, 20, sf::Color(200, 100, 200));
                fillRect(img, bx + 12, by + 20, 12, 24, sf::Color(255, 200, 100));
                fillRect(img, bx + 56, by + 20, 12, 24, sf::Color(255, 200, 100));
                fillRect(img, bx + 26, by + 2, 28, 14, bodyPurple);
                fillRect(img, bx + 26, by + 0, 28, 4, sf::Color(255, 200, 100));
                img.setPixel({static_cast<unsigned>(bx + 35), static_cast<unsigned>(by + 8)}, sf::Color(255, 255, 200));
                img.setPixel({static_cast<unsigned>(bx + 45), static_cast<unsigned>(by + 8)}, sf::Color(255, 255, 200));
            }
        };
        
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 8; ++col) {
                drawBoss(col, row, row, col);
            }
        }
        
        sf::Texture texture;
        if (!texture.loadFromImage(img)) {
            // Failed to load, return empty texture
        }
        return texture;
    }

}  // namespace PixelArtGenerator
