#include <iostream>
#include <thread>
#include <chrono>
#include "life.h"

// determine the system call to make to clear the screen depending on the user's OS
#ifdef _WIN32
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
            }
        }
        catch (...){
            std::cout << "Please provide a valid integer." << std::endl;
        }
    }
    while (simulationSpeed == 0);
    
    try{
        life::Game game = life::readStateFile("testfile.txt");
        while (game.getActive()){
            system(clear);
            game.displayCells();
            game.updateCells();
            std::this_thread::sleep_for(std::chrono::milliseconds(simulationSpeed * 10));   
        }
        std::cout << "FINISHED" << std::endl;
    }
    catch (...){
        std::cout << "Error parsing the game state file" << std::endl;
    }
}
