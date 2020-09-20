#include "Solver.hpp"
#include <vector>
#include <random>
#include <ics46/factory/DynamicFactory.hpp>
ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver, Solver, "Haining's Solver");

void Solver::solveMaze(const Maze& maze, MazeSolution& mazeSolution)
{
    mazeSolution.restart();
    std::vector<std::vector<bool>> check;
    check.resize(maze.getWidth(), std::vector<bool>(maze.getHeight(), false));
    startSolving(mazeSolution.getStartingCell().first, mazeSolution.getStartingCell().second, maze, mazeSolution, check);
}

std::vector<Direction> Solver::findNext(int x, int y, const Maze& maze, std::vector<std::vector<bool>>& check)
{
    std::vector<Direction> directionArray;

    if (maze.wallExists(x, y, Direction::up) == false && y-1 >= 0 && check[x][y-1] == false)
    {
        directionArray.push_back(Direction::up);
    }
    if (maze.wallExists(x, y, Direction::down) == false && y+1 < maze.getHeight() && check[x][y+1] == false)
    {
        directionArray.push_back(Direction::down);
    }
    if (maze.wallExists(x, y, Direction::left) == false && x-1 >= 0 && check[x-1][y] == false)
    {
        directionArray.push_back(Direction::left);
    }
    if (maze.wallExists(x, y, Direction::right) == false && x+1 < maze.getWidth() && check[x+1][y] == false)
    {
        directionArray.push_back(Direction::right);
    }

    return directionArray;
}

Direction Solver::generateDirection(std::vector<Direction> directionArray)
{
    std::random_device device;
    std::default_random_engine engine{device()};
    const int temp = directionArray.size()-1;
    std::uniform_int_distribution<int> distribution{0, temp};
    Direction newDirection = directionArray[distribution(engine)];
    return newDirection;
}

void Solver::move(int &x, int& y, Direction direction)
{
    if (direction == Direction::up) 
    {
        y--;
    }
    else if (direction == Direction::down) 
    {
        y++;
    }
    else if (direction == Direction::left)
    {
        x--;
    }
    else if (direction == Direction::right) 
    {
        x++;
    }
}

void Solver::startSolving(int x, int y, const Maze& maze, MazeSolution& mazeSolution, std::vector<std::vector<bool>>& check)
{
    check[x][y] = true;
    std::vector<Direction> directionArray = findNext(x, y, maze, check);
    while (directionArray.size() > 0 && mazeSolution.getCurrentCell() != mazeSolution.getEndingCell())
    {
        Direction direction = generateDirection(directionArray);
        int x2 = x;
        int y2 = y;
        move(x2, y2, direction);
        if (check[x2][y2] == false && maze.wallExists(x, y, direction) == false)
        {
            mazeSolution.extend(direction);
            startSolving(x2, y2, maze, mazeSolution, check);
        }
        directionArray = findNext(x, y, maze, check);
    }
    if (mazeSolution.getCurrentCell() != mazeSolution.getEndingCell())
    {
        mazeSolution.backUp();
    }
}