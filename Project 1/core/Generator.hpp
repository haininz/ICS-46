#ifndef GENERATOR_HPP
#define GENERATOR_HPP
#include "Maze.hpp"
#include "Direction.hpp"
#include "MazeGenerator.hpp"
#include <vector>

class Generator:public MazeGenerator
{
public:
    void generateMaze(Maze& maze) override;
    // The function "findNext" will generate a vector containing all the possible directions that can be moved to at a certain location
    std::vector<Direction> findNext(int, int, Maze&, std::vector<std::vector<bool>>&);
    // The function "generateDirection" will return a specific direction from the Direction vector returned from the "findNext" function
    Direction generateDirection(std::vector<Direction>);
    // The function "move" will accomodate the x/y according to the "Direction" in the parameter
    void move(int&, int&, Direction);
    // The function "startGeneration" will start generating the maze and recursively call itself 
    // as long as there still exist a location(x,y) that is able to move along a certain direction
    void startGeneration(int, int, Maze&, std::vector<std::vector<bool>>&);
};

#endif