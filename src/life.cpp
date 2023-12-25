#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include "life.h"


// splits a string by a given delimiter
std::vector<std::string> life::splitStr(std::string str, std::string delim){
    std::vector<std::string> output;
    int delimIndex = 0;
    while (delimIndex != std::string::npos){
        delimIndex = str.find(delim);
        if (delimIndex != std::string::npos){
            // add this "segment" of the string to the output and delete it from the string
            output.push_back(str.substr(0, delimIndex));
            str.erase(0, delimIndex + delim.length());
        }
    }
     // add the last segment of the string to the vector
    output.push_back(str);
    return output;
}

// creates a Game object from a starting state file
life::Game life::readStateFile(std::string fileName){
    // game variables to be defined
    int width;
    int height;
    std::vector<std::vector<bool>> cells;
    // read each line from the given file
    std::ifstream fileStream(fileName);
    std::vector<std::string> lines;
    if (fileStream.good()){
        std::string lineBuf;
        while(std::getline(fileStream, lineBuf))
            lines.push_back(lineBuf);
    }
    else{
        std::cout << "Invalid game state file -  File could not be read." << std::endl;
        throw std::runtime_error("Invalid game state file -  File could not be read.");
    }
    fileStream.close();
    // porse the width and height of the game grid
    std::vector<std::string> dimensions = life::splitStr(lines[0], ", ");
    if (dimensions.size() != 2){
        std::cout << "Invalid game state file - invalid game size." << std::endl;
        throw std::runtime_error("Invalid game state file - invalid game size.");
    }   
    try{
        width = std::stoi(dimensions[0]);
        height = std::stoi(dimensions[1]);
    }
    catch (...){
        std::cout << "Invalid game state file - invalid game size." << std::endl;
        throw std::runtime_error("Invalid game state file - invalid game size.");
    }
       // validate the number of lines (should be the specified height of the grid + 2) and raise an error if it's invalid
    if (lines.size() != height + 2){
        std::cout << "Invalid game state file - too many or too few rows" << std::endl;
        throw std::runtime_error("Invalid game state file - too many or too few rows");
    }
     // parse the game's rules
    std::vector<int> rules;
    try{
      
        std::vector<std::string> ruleStrs = life::splitStr(lines[1], ";");
        if (ruleStrs.size() != 3){
            std::cout << "Invalid game state file - invalid game rules." << std::endl;
            throw std::runtime_error("Invalid game state file - invalid game rules.");
        }
        // get the range of survival condtions
        std::vector<std::string> survivalStrs = splitStr(ruleStrs[1], "-");
        rules = {std::stoi(ruleStrs[0]), std::stoi(ruleStrs[1]), std::stoi(ruleStrs[2])};
    }
     catch (...){
        std::cout << "Invalid game state file - invalid game rules." << std::endl;
        throw std::runtime_error("Invalid game state file - invalid game rules.");
    }
    // parse the game map
    char chr;
    std::vector<bool> row;
    std::string rowIn;
    for (int i = 2; i < lines.size(); i++){
        row.clear();
        rowIn = lines.at(i);
        // validate the length of the row, and throw an error if it's invalid
        if (rowIn.length() != width){
            int rowNum = i-2;
            std::cout << "Invalid game state file - Invalid row size in row " << rowNum << std::endl;
            throw std::runtime_error("Invalid game state file - Invalid row size in row " + rowNum);
        }
        // add each character to the row
        for (int j = 0; j < width; j++){
            chr = rowIn.at(j);
            switch (chr){
                case 'O':
                    row.push_back(true);
                    break;
                case '.':
                    row.push_back(false); // dead cells are displayed as blank space in the game, which is why a dot is changed to a blank space.
                    break;
                default:
                    std::cout << "Invalid game state file - illegal character: " << chr;
                    throw std::runtime_error("Invalid game state file - illegal character: " + chr);
                    break;
            }
        }
        cells.push_back(row);
    }
    return life::Game(width, height, rules, cells);
}

// general constructor for the Game class
life::Game::Game(int width, int height, std::vector<int> rules, std::vector<std::vector<bool>> cells){
    generation_ = 0;
    active_ = true;
    width_ = width;
    height_  = height;
    cellRows_ = cells;
    // set the game's rules according to the given vector
    // we assume that the vector has already been validated by the readStateFile function
    underpop_ = rules[0];
    overpop_ = rules[1];
    reproduction_ = rules[2];
}

// returns if a cell is alive based on its character
bool life::Game::getCell(int x, int y){
    return cellRows_[x][y];
}

// displays the current cells
void life::Game::displayCells() const{
    for (std::vector<bool> row : cellRows_){
        for (bool cell : row){
            std::cout << (cell ? "O" : " "); 
        }
        std::cout << std::endl;
        
    }
    std::cout << "GEN: " << generation_ << std::endl;
}

// updates the cells according to the games rules
void life::Game::updateCells(){
    generation_++;
    int neighbors;
    int updates = 0;
    // create a copy of the board to update the board to
    std::vector<std::vector<bool>> newCells = cellRows_;
    for (int y = 0; y < height_; y++){
        for (int x = 0; x < width_; x++){
            neighbors = getNeighbors(x, y);
             // if the cell is alive, mark the cell as dead if its neighbor count is lower than the underpopulation threshold, or if its neighbor count is higher than the overpopulation threshold
            if (getCell(x, y) && (neighbors < underpop_ || neighbors > overpop_)) {
                newCells[x][y] = false;
                updates++;
            }
            // if the cell is dead, make it alive if it has the requisite number of neighbors
            else if (!getCell(x, y) && neighbors == reproduction_){
                newCells[x][y] = true;
                updates++;
            }
        }
    }
    // check if the grid to needs to be expanded vertically 
    // check on the top
    // mark the game "inactive" if no transformations occured
    if (!updates){
        active_ = false;
    }
    cellRows_ = newCells;
}

// getter methods
// gets the number of live neighbors adjacent to a cell in a given positon, returns -1 if the position doesn't exist
int life::Game::getNeighbors(int x, int y){
    if (x >= width_ || y >= height_ || x < 0 || y < 0){
        return -1;
    }
    int neighbors = 0;
    // these reperesent index the row above and below the cell (if present) respectively
    int topIndex = y; 
    int bottomIndex = y;
    // check if there are any cells above this cell and check the cell directly above it so
    if (y != 0){
        topIndex--;
        if (getCell(x, topIndex)) 
            neighbors++;
    }
    // check if there are any cells below this cell and check the cell below it so
    if (y != height_ - 1){
        bottomIndex = y+1;
        if (getCell(x, bottomIndex))
            neighbors++;
    }
    // check all cells to the left of this cell if it isn't the leftmost cell
    if (x != 0){
        for (int i = topIndex; i <= bottomIndex; i++)
            if (getCell(x-1, i))
                neighbors++;
    }
    // check all cells to the right of this cell if it isn't the rightmost cell
    if (x != width_ - 1){
        for (int i = topIndex; i <= bottomIndex; i++)
            if (getCell(x+1, i))
                neighbors++;
    }
    return neighbors;
}
