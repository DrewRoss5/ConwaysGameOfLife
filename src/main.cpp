#include <iostream>
#include <thread>
#include <chrono>
#include "life.h"

// determine the system call to make to clear the screen depending on the user's OS
#ifdef WIN32
    const char* clear = "cls";
#else
    const char* clear = "clear";
#endif


int main(int argc, char** argv){
    // determine the desired simulation speed, the time each frame should take in tens of milliseconds 
    int simulationSpeed = 0;
    std::string speedStr;
    std::cout << "What simulation speed would you like? (between 1 and 100, lower is faster)" << std::endl;
    do{
        std::getline(std::cin, speedStr); 
        try{
            simulationSpeed = std::stoi(speedStr);
            if (simulationSpeed < 1 || simulationSpeed > 100){
                std::cout << "Please provide an integer between 1 and 100" << std::endl;
                simulationSpeed = 0;
            }
        }
        catch (...){
            std::cout << "Please provide a valid integer." << std::endl;
        }
    }
    while (simulationSpeed == 0);
    // validate a game state file was provided
    if (argc != 2){
        std::cout << "Error - This program accepts exactly one argument" << std::endl;
        return -1;
    }
      // attempt to create a game from the provided game state file
    std::string gameStateFile = argv[1];
    life::Game game;
    try{
        game = life::readStateFile(gameStateFile);
    }
    catch (...){
        std::cout << "Error raised, exiting" << std::endl;
    }
    // run the game
    while (game.getActive()){
        system(clear);
        game.displayCells();
        game.updateCells();
        std::this_thread::sleep_for(std::chrono::milliseconds(simulationSpeed * 10));   
    }
    std::cout << "FINISHED" << std::endl;

}
