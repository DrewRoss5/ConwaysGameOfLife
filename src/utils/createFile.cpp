// this is to create an empty file with the default rules and a user specified size
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char** argv){
    std::string fileName;
    int x, y; // the integers to define the size of the grid
    // validate and parse the two arguments
    if (argc != 4){
        std::cout << "This program accepts exactly three arguments, please run it again with three arguments" << std::endl;
        return -1;
    }
    try{
        x = std::stoi(argv[1]);
        y = std::stoi(argv[2]);
        fileName = argv[3];
    }
    catch(...){
        std::cout << "Invalid interger." << std::endl;
        return -1;
    }
    // create the file
    std::ofstream fileStream(fileName);
    if(fileStream.good()){
        // write the default rules and provided size to the file
        fileStream << x << ", " << y << "\n2;3;3\n";
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
