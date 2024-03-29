#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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
    int maxWidth;
    int maxHeight;
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
        throw Game::ERROR_CODES::INVALID_FILE;
    }
    fileStream.close();
    // porse the starting width and height of the game grid
    std::vector<std::string> dimensions = life::splitStr(lines[0], ", ");
    if (dimensions.size() != 4){
        throw Game::ERROR_CODES::INVALID_GAME_SIZE;
    }   
    try{
        width = std::stoi(dimensions[0]);
        height = std::stoi(dimensions[1]);
        maxWidth = std::stoi(dimensions[2]);
        maxHeight = std::stoi(dimensions[3]);
        if ((maxWidth < width && maxWidth != 0) ||  (maxHeight < height && maxHeight != 0))
            throw Game::ERROR_CODES::INVALID_GAME_SIZE;
    }
    catch (...){
        throw Game::ERROR_CODES::INVALID_GAME_SIZE;
    }
       // validate the number of lines (should be the specified height of the grid + 2) and raise an error if it's invalid
    if (lines.size() != height + 2){
        throw Game::ERROR_CODES::INVALID_ROW_COUNT;
    }
     // parse the game's rules
    std::vector<int> rules;
    try{
        std::vector<std::string> ruleStrs = life::splitStr(lines[1], ";");
        if (ruleStrs.size() != 3){
            throw Game::ERROR_CODES::INVALID_GAME_RULES;
        }
        rules = {std::stoi(ruleStrs[0]), std::stoi(ruleStrs[1]), std::stoi(ruleStrs[2])};
    }
     catch (...){
        throw Game::ERROR_CODES::INVALID_GAME_RULES;
    }
    // parse the game map
    char chr;
    std::vector<bool> row;
    std::string rowIn;
    for (int i = 2; i < lines.size(); i++){
        row.clear();
        rowIn = lines.at(i);
        // validate the length of the row, and throw an error if it's invalid
        if (rowIn.length() != width)
            throw Game::ERROR_CODES::INVALID_ROW_SIZE;
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
                    throw Game::ERROR_CODES::ILLEGAL_CHAR;
                    break;
            }
        }
        cells.push_back(row);
    }
    return life::Game(width, height, maxWidth, maxHeight, rules, cells);
}

// general constructor for the Game class
life::Game::Game(int width, int height, int maxWidth, int maxHeight, std::vector<int> rules, std::vector<std::vector<bool>> cells){
    generation_ = 0;
    active_ = true;
    width_ = width;
    height_  = height;
    maxWidth_ = maxWidth;
    maxHeight_ = maxHeight;
    cellRows_ = cells;
    // set the game's rules according to the given vector
    // we assume that the vector has already been validated by the readStateFile function
    underpop_ = rules[0];
    overpop_ = rules[1];
    reproduction_ = rules[2];
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
    bool topExpanded = false; 
    bool bottomExpanded  = false; 
    bool leftExpanded = false;
    bool rightExpanded = false;
    // check if the grid needs to be expanded vertically (if the maximum height hasn't been reached or is undefined)
    if (height_ < maxHeight_ || maxHeight_ == 0){
        for (int i = 0; i < width_; i++){
            if (getNeighbors(i, -1) == reproduction_ && !topExpanded){
                addRow(NORTH);
                topExpanded = true;
            }
            if (getNeighbors(i, height_) == reproduction_ && !bottomExpanded){
                addRow(SOUTH);
                bottomExpanded = true;
            }
        }
    }
    // check if the grid needs to be expanded horizontally
    if (width_ < maxWidth_ || maxWidth_ == 0){
        for (int i = 0; i < height_; i++){
            if (getNeighbors(-1, i) && !leftExpanded){
                addColumn(WEST);
                leftExpanded = true;
            }
            if (getNeighbors(width_, i) && !rightExpanded){
                addColumn(EAST);
                rightExpanded = true;

            }
        }
    }
    // create a copy of the board to update the board to
    std::vector<std::vector<bool>> newCells = cellRows_;
    for (int y = 0; y < height_; y++){
        for (int x = 0; x < width_; x++){
            neighbors = getNeighbors(x, y);
             // if the cell is alive, mark the cell as dead if its neighbor count is lower than the underpopulation threshold, or if its neighbor count is higher than the overpopulation threshold
            if (getCell(x, y) && (neighbors < underpop_ || neighbors > overpop_)) {
                newCells[y][x] = false;
                updates++;
            }
            // if the cell is dead, make it alive if it has the requisite number of neighbors
            else if (!getCell(x, y) && neighbors == reproduction_){
                newCells[y][x] = true;
                updates++;
            }
        }
    }
    // mark the game "inactive" if no transformations occured
    if (!updates){
        active_ = false;
    }
    cellRows_ = newCells;
}

// adds a new empty column to the grid
void life::Game::addColumn(int direction){
    width_++;
    switch (direction){
        case WEST:
            // add a new dead cell to the right of each row
            for (int i = 0; i < height_; i++)
                cellRows_[i].insert(cellRows_[i].begin(), false);
            break;
        case EAST:
            // add a new dead cell to the left of each row
            for (int i = 0; i < height_; i++)
                cellRows_[i].push_back(false);
            break;
    }
}

// adds an empty row to the grid
void life::Game::addRow(int direction){
    height_++;
    // create a new empty row
    std::vector<bool> newRow;
    for (int i = 0; i < width_; i++)
        newRow.push_back(false);
    // append the empty row to the row list
    switch (direction){
        case NORTH:
            // append the empty row to the top of the grid
            cellRows_.insert(cellRows_.begin(), newRow);
            break;
        case SOUTH:
            // append the empty tow to the bottom of the grid
            cellRows_.push_back(newRow);
    }
}

// getter methods
// returns the number of live neighbors a cell in column x and row y has
int life::Game::getNeighbors(int x, int y){
    if (x > width_ || y > height_ || x < -1 || y < -1){
        return -1;
    }
    int neighbors = 0;
    // these reperesent index the row above and below the cell (if present) respectively. These are determined based on if the cell is in range
    int topIndex = (y == -1) ? 0 : y; 
    int bottomIndex = (y == height_) ? y-1 : y; 
    // check if there are any cells above this cell and check the cell directly above it so
    if (y > 0){
        topIndex--;
        if (getCell(x, topIndex)) 
            neighbors++;
    }
    // check if there are any cells below this cell and check the cell below it so
    if (y < height_-1){
        bottomIndex = y+1;
        if (getCell(x, bottomIndex))
            neighbors++;
    }
    // check all cells to the left of this cell if it isn't the leftmost cell
    if (x > 0){
        for (int i = topIndex; i <= bottomIndex; i++)
            if (getCell(x-1, i))
                neighbors++;
    }
    // check all cells to the right of this cell if it isn't the rightmost cell
    if (x < width_ - 1){
        for (int i = topIndex; i <= bottomIndex; i++)
            if (getCell(x+1, i))
                neighbors++;
    }
    return neighbors;
}
