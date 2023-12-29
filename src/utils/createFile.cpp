// this is to create an empty file with the default rules and a user specified size
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char** argv){
    std::string fileName;
    int x, y, maxX, maxY; // the integers to define the size of the grid
    // validate and parse the two arguments
    if (argc != 6){
        std::cout << argc << std::endl;
        std::cout << "This program accepts exactly five arguments, please run it again with five arguments" << std::endl;
        return -1;
    }
    try{
        x = std::stoi(argv[1]);
        y = std::stoi(argv[2]);
        maxX = std::stoi(argv[3]);
        maxY = std::stoi(argv[4]);
        fileName = argv[5];
    }
    catch(...){
        std::cout << "Invalid interger." << std::endl;
        return -1;
    }
    if ((maxX < x && maxX != 0) || (maxY < y && maxY != 0)){
        std::cout << "Maximum size may not be smaller than the starting size" << std::endl;
        return -1;
    }
    // create the file
    std::ofstream fileStream(fileName);
    if(fileStream.good()){
        // write the default rules and provided size to the file
        fileStream << x << ", " << y << ", " << maxX << ", " << maxY << "\n2;3;3\n";
        // write a blank line of size x for y rows
        for (int i = 0; i < y; i++){
            for (int j = 0; j < x; j++)
                fileStream << ".";
            // create a newline if it's not the last line of the file
            if (i < (y-1))
                fileStream << "\n";
        }
    }
    else{
        std::cout << "The file could not be created..." << std::endl;
    }
    fileStream.close();
}
