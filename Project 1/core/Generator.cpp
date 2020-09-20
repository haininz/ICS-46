#include "Generator.hpp"
#include <random>
#include <iostream>
#include <ics46/factory/DynamicFactory.hpp>

ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator, Generator, "Haining's Generator");

void Generator::generateMaze(Maze& maze)
{
    maze.addAllWalls();
    std::vector<std::vector<bool>> check;
    check.resize(maze.getWidth(), std::vector<bool>(maze.getHeight(), false));
    startGeneration(0, 0, maze, check);
}

std::vector<Direction> Generator::findNext(int x, int y, Maze& maze, std::vector<std::vector<bool>>& check)
{
    std::vector<Direction> directionArray;

    if (y-1 >= 0 && check[x][y-1] == false)
    {
        directionArray.push_back(Direction::up);
    }
    if (y+1 < maze.getHeight() && check[x][y+1] == false)
    {
        directionArray.push_back(Direction::down);
    }
    if (x-1 >= 0 && check[x-1][y] == false)
    {
        directionArray.push_back(Direction::left);
    }
    if (x+1 < maze.getWidth() && check[x+1][y] == false)
    {
        directionArray.push_back(Direction::right);
    }

    return directionArray;
}

Direction Generator::generateDirection(std::vector<Direction> directionArray)
{
    std::random_device device;
    std::default_random_engine engine{device()};
    const int temp = directionArray.size()-1;
    std::uniform_int_distribution<int> distribution{0, temp};
    Direction newDirection = directionArray[distribution(engine)];
    return newDirection;
}

void Generator::move(int &x, int& y, Direction direction)
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

void Generator::startGeneration(int x, int y, Maze& maze, std::vector<std::vector<bool>>& check)
{
    check[x][y] = true;
    std::vector<Direction> directionArray = findNext(x, y, maze, check);
    while (directionArray.size() > 0)
    {
        Direction direction = generateDirection(directionArray);
        int x2 = x;
        int y2 = y;
        move(x2, y2, direction);
        if (check[x2][y2] == false && maze.wallExists(x, y, direction) == true)
        {
            maze.removeWall(x, y, direction);
            startGeneration(x2, y2, maze, check);
        }
        directionArray = findNext(x, y, maze, check);
    }
}