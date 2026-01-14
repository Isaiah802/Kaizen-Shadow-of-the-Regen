#include "ResourceManager.hpp"
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::loadTexture(const std::string& id, const std::string& filepath) {
    // Check if already loaded
    if (hasTexture(id)) {
        std::cout << "Texture '" << id << "' already loaded." << std::endl;
        return true;
    }
    
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filepath)) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        return false;
    }
    
    m_textures[id] = std::move(texture);
    std::cout << "Loaded texture: " << id << " from " << filepath << std::endl;
    return true;
}

sf::Texture& ResourceManager::getTexture(const std::string& id) {
    auto it = m_textures.find(id);
    if (it == m_textures.end()) {
        std::cerr << "Texture '" << id << "' not found! Returning default." << std::endl;
        // Return first texture or create empty one
        static sf::Texture defaultTexture;
        return defaultTexture;
    }
    return *it->second;
}

bool ResourceManager::hasTexture(const std::string& id) const {
    return m_textures.find(id) != m_textures.end();
}

bool ResourceManager::loadFont(const std::string& id, const std::string& filepath) {
    if (hasFont(id)) {
        std::cout << "Font '" << id << "' already loaded." << std::endl;
        return true;
    }
    
    auto font = std::make_unique<sf::Font>();
    if (!font->openFromFile(filepath)) {
        std::cerr << "Failed to load font: " << filepath << std::endl;
        return false;
    }
    
    m_fonts[id] = std::move(font);
    std::cout << "Loaded font: " << id << " from " << filepath << std::endl;
    return true;
}

sf::Font& ResourceManager::getFont(const std::string& id) {
    auto it = m_fonts.find(id);
    if (it == m_fonts.end()) {
        std::cerr << "Font '" << id << "' not found! Returning default." << std::endl;
        static sf::Font defaultFont;
        return defaultFont;
    }
    return *it->second;
}

bool ResourceManager::hasFont(const std::string& id) const {
    return m_fonts.find(id) != m_fonts.end();
}

bool ResourceManager::loadSoundBuffer(const std::string& id, const std::string& filepath) {
    if (hasSoundBuffer(id)) {
        std::cout << "Sound buffer '" << id << "' already loaded." << std::endl;
        return true;
    }
    
    auto buffer = std::make_unique<sf::SoundBuffer>();
    if (!buffer->loadFromFile(filepath)) {
        std::cerr << "Failed to load sound: " << filepath << std::endl;
        return false;
    }
    
    m_soundBuffers[id] = std::move(buffer);
    std::cout << "Loaded sound: " << id << " from " << filepath << std::endl;
    return true;
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& id) {
    auto it = m_soundBuffers.find(id);
    if (it == m_soundBuffers.end()) {
        std::cerr << "Sound buffer '" << id << "' not found! Returning default." << std::endl;
        static sf::SoundBuffer defaultBuffer;
        return defaultBuffer;
    }
    return *it->second;
}

bool ResourceManager::hasSoundBuffer(const std::string& id) const {
    return m_soundBuffers.find(id) != m_soundBuffers.end();
}

void ResourceManager::clear() {
    m_textures.clear();
    m_fonts.clear();
    m_soundBuffers.clear();
    std::cout << "All resources cleared." << std::endl;
}
