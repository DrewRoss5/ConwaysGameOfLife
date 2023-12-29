#ifndef LIFE_H
#define LIFE_H

#include <vector>
#include <string>

namespace life{
    class Game{
        private:
            // contant enumerator for the four cardinal direction
            enum DIRECTIONS_{NORTH, EAST, SOUTH, WEST};
            // class attributes
            int width_;
            int height_;
            int maxWidth_;
            int maxHeight_;
            int overpop_;
            int underpop_;
            int reproduction_;
            int generation_;
            bool active_;
            std::vector<std::vector<bool>> cellRows_;
        public:
            Game() : width_(0), height_(0), maxWidth_(0), maxHeight_(0), overpop_(3), underpop_(2), reproduction_(3), generation_(0), active_(false), cellRows_({}){} // default constructor
            Game(int width, int height, int maxWidth, int maxHeight, std::vector<int> rules, std::vector<std::vector<bool>> cells);
            void displayCells() const;
            void updateCells();
            void addColumn(int direction);
            void addRow(int direction);
            //getter functions
            bool getCell(int x, int y) {return cellRows_[y][x];}
            int getWidth() const {return width_;}
            int getHeight() const {return height_;}
            bool getActive() const {return active_;}
            int getNeighbors(int x, int y);
            
    };

    Game readStateFile(std::string filePath);
    // this namespace is probably not the best place to store this function, but there's nowhere else to put it, so here it stays
    std::vector<std::string> splitStr(std::string str, std::string delim);
}
#endif
