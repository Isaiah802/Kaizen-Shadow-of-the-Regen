#include "Game.hpp"
#include <iostream>
#include <exception>

/**
 * @brief Entry point of the application
 * 
 * Initializes the game and handles any exceptions that might occur
 */
int main() {
    try {
        Game game;
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
