#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

/**
 * @brief Singleton resource manager for loading and caching game assets
 * 
 * Prevents duplicate loading of textures/sounds
 * Ensures resources stay loaded while in use
 * 
 * Usage: ResourceManager::getInstance().loadTexture("player", "assets/sprites/player.png");
 */
class ResourceManager {
public:
    // Singleton access
    static ResourceManager& getInstance();
    
    // Delete copy constructor and assignment (singleton pattern)
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    // Texture management
    bool loadTexture(const std::string& id, const std::string& filepath);
    sf::Texture& getTexture(const std::string& id);
    bool hasTexture(const std::string& id) const;
    
    // Font management
    bool loadFont(const std::string& id, const std::string& filepath);
    sf::Font& getFont(const std::string& id);
    bool hasFont(const std::string& id) const;
    
    // Sound buffer management
    bool loadSoundBuffer(const std::string& id, const std::string& filepath);
    sf::SoundBuffer& getSoundBuffer(const std::string& id);
    bool hasSoundBuffer(const std::string& id) const;
    
    // Clear all resources
    void clear();
    
private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    
    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
};
