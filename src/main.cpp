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

int main(){
   
    try{
        life::Game game = life::readStateFile("testfile.txt");
        while (game.getActive()){
            system(clear);
            game.displayCells();
            game.updateCells();
            std::this_thread::sleep_for(std::chrono::milliseconds(34));
        }
        std::cout << "FINISHED" << std::endl;
    }
    catch (...){
        std::cout << "Error raised" << std::endl;
    }
   

}