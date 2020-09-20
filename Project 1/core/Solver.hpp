#ifndef SOLVER_HPP
#define SOLVER_HPP
#include "Maze.hpp"
#include "MazeSolver.hpp"
#include "MazeSolution.hpp"
class Solver:public MazeSolver
{
    void solveMaze(const Maze& maze, MazeSolution& mazeSolution) override;
    // The function "findNext" will generate a vector containing all the possible directions that can be moved to at a certain location
    std::vector<Direction> findNext(int, int, const Maze&, std::vector<std::vector<bool>>&);
    // The function "generateDirection" will return a specific direction from the Direction vector returned from the "findNext" function
    Direction generateDirection(std::vector<Direction>);
    // The function "move" will accomodate the x/y according to the "Direction" in the parameter
    void move(int&, int&, Direction);
    // The function "startSolving" will start solving the maze and recursively call itself as long as the end location is not reached
    void startSolving(int, int, const Maze&, MazeSolution&, std::vector<std::vector<bool>>&);
};

#endif