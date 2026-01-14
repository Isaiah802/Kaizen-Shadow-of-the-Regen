#include "Animation.hpp"

// Animation class implementation
Animation::Animation()
    : m_currentFrame(0)
    , m_currentTime(0.0f)
    , m_loop(true)
    , m_finished(false)
{
}

Animation::Animation(const std::vector<Frame>& frames, bool loop)
    : m_frames(frames)
    , m_currentFrame(0)
    , m_currentTime(0.0f)
    , m_loop(loop)
    , m_finished(false)
{
}

void Animation::addFrame(const sf::IntRect& rect, float duration) {
    m_frames.push_back({rect, duration});
}

void Animation::update(float deltaTime) {
    if (m_finished || m_frames.empty()) return;
    
    m_currentTime += deltaTime;
    
    while (m_currentTime >= m_frames[m_currentFrame].duration) {
        m_currentTime -= m_frames[m_currentFrame].duration;
        m_currentFrame++;
        
        if (m_currentFrame >= static_cast<int>(m_frames.size())) {
            if (m_loop) {
                m_currentFrame = 0;
            } else {
                m_currentFrame = static_cast<int>(m_frames.size()) - 1;
                m_finished = true;
                m_currentTime = 0.0f;
                return;
            }
        }
    }
}

void Animation::reset() {
    m_currentFrame = 0;
    m_currentTime = 0.0f;
    m_finished = false;
}

void Animation::setLooping(bool loop) {
    m_loop = loop;
}

sf::IntRect Animation::getCurrentFrame() const {
    if (m_frames.empty()) {
        return sf::IntRect({0, 0}, {32, 48});
    }
    return m_frames[m_currentFrame].rect;
}

bool Animation::isFinished() const {
    return m_finished;
}

// AnimationController class implementation
AnimationController::AnimationController()
    : m_currentAnimation("")
{
}

void AnimationController::addAnimation(const std::string& name, const Animation& animation) {
    m_animations[name] = animation;
}

void AnimationController::setAnimation(const std::string& name, bool forceRestart) {
    if (m_animations.find(name) == m_animations.end()) {
        return;
    }
    
    if (m_currentAnimation != name || forceRestart) {
        m_currentAnimation = name;
        m_animations[m_currentAnimation].reset();
    }
}

void AnimationController::update(float deltaTime) {
    if (m_currentAnimation.empty()) return;
    
    auto it = m_animations.find(m_currentAnimation);
    if (it != m_animations.end()) {
        it->second.update(deltaTime);
    }
}

sf::IntRect AnimationController::getCurrentFrame() const {
    if (m_currentAnimation.empty()) {
        return sf::IntRect({0, 0}, {32, 48});
    }
    
    auto it = m_animations.find(m_currentAnimation);
    if (it != m_animations.end()) {
        return it->second.getCurrentFrame();
    }
    
    return sf::IntRect({0, 0}, {32, 48});
}

bool AnimationController::isAnimationFinished() const {
    if (m_currentAnimation.empty()) return true;
    
    auto it = m_animations.find(m_currentAnimation);
    if (it != m_animations.end()) {
        return it->second.isFinished();
    }
    
    return true;
}
