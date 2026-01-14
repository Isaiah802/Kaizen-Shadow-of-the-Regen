#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

class Animation {
public:
    struct Frame {
        sf::IntRect rect;      // Sprite sheet rectangle
        float duration;         // Frame duration in seconds
    };
    
    Animation();
    Animation(const std::vector<Frame>& frames, bool loop = true);
    
    void addFrame(const sf::IntRect& rect, float duration);
    void update(float deltaTime);
    void reset();
    void setLooping(bool loop);
    
    sf::IntRect getCurrentFrame() const;
    bool isFinished() const;
    int getCurrentFrameIndex() const { return m_currentFrame; }
    
private:
    std::vector<Frame> m_frames;
    int m_currentFrame;
    float m_currentTime;
    bool m_loop;
    bool m_finished;
};

class AnimationController {
public:
    AnimationController();
    
    void addAnimation(const std::string& name, const Animation& animation);
    void setAnimation(const std::string& name, bool forceRestart = false);
    void update(float deltaTime);
    
    sf::IntRect getCurrentFrame() const;
    const std::string& getCurrentAnimationName() const { return m_currentAnimation; }
    bool isAnimationFinished() const;
    
private:
    std::unordered_map<std::string, Animation> m_animations;
    std::string m_currentAnimation;
};
