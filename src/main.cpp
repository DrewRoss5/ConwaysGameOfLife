#include <iostream>
#include <thread>
#include <chrono>
#include "life.h"

// determine the system call to make to clear the screen depending on the user's OS
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    const char* clear = "cls";
#else
    const char* clear = "clear";
#endif


int main(int argc, char** argv){
    // validate a game state file was provided
    if (argc != 2){
        std::cout << "Error - This program accepts exactly one argument" << std::endl;
        return -1;
    }
    // determine the desired simulation speed, the time each frame should take in tens of milliseconds 
    int simulationSpeed = 0;
    std::string speedStr;
    std::cout << "What simulation speed would you like? (between 1 and 95, higher is faster)" << std::endl;
    do{
        std::getline(std::cin, speedStr); 
        try{
            simulationSpeed = std::stoi(speedStr);
            if (simulationSpeed < 1 || simulationSpeed > 95){
                std::cout << "Please provide an integer between 1 and 95" << std::endl;
                simulationSpeed = 0;
            }
        }
        catch (...){
            std::cout << "Please provide a valid integer." << std::endl;
        }
    }
    while (simulationSpeed == 0);
    int frameDelay = 1000 - (simulationSpeed * 10);
      // attempt to create a game from the provided game state file
    std::string gameStateFile = argv[1];
    life::Game game;
    try{
        game = life::readStateFile(gameStateFile);
    }
    catch (int errCode){
        switch (errCode){
            case life::Game::INVALID_FILE:
                std::cerr << "Error - The game state file could not be read!" << std::endl;
                break;
            case life::Game::INVALID_GAME_SIZE:
                std::cerr << "Error - The game size could not be parsed!" << std::endl;
                break;
            case life::Game::INVALID_GAME_RULES:
                std::cerr << "Error - The game rules could not be parsed!" << std::endl;
                break;
            case life::Game::INVALID_ROW_COUNT:
                std::cerr << "Error - Too many or to few rows" << std::endl;
                break;
            case life::Game::INVALID_ROW_SIZE:
                std::cerr << "Error - Invalid Row Size!" << std::endl;
                break;
            default:
                std::cerr << "Error encountered while parsing the game state file" << std::endl;
                break;
            }
    }
    // run the game
    while (game.getActive()){
        system(clear);
        game.displayCells();
        game.updateCells();
        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay));   
    }
    std::cout << "FINISHED" << std::endl;

}
