#ifndef LIFE_H
#define LIFE_H

#include <vector>
#include <string>

namespace life{
    class Game{
        private:
            int width_;
            int height_;
            int overpop_;
            int underpop_;
            int reproduction_;
            int generation_;
            bool active_;
            std::vector<std::string> cellRows_;
        public:
            Game(int width, int height, std::vector<int> rules, std::vector<std::string> cells);
            bool isCellAlive(int x, int y);
            void displayCells();
            void updateCells();
            //getter functions
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