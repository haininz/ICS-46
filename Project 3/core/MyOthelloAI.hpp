#ifndef MYOTHELLOAI_HPP
#define MYOTHELLOAI_HPP
#include "OthelloAI.hpp"

namespace haininz
{
    class MyOthelloAI : public OthelloAI
    {
    public:
        // This function chooses the best coordinate (The move that will maximize the possibility of winning) for the current game state
        std::pair<int, int> chooseMove(const OthelloGameState&) override;
        // This function recursively searches for the score for each possible move and returns the highest score
        int search(std::unique_ptr<OthelloGameState>&, int, std::string);
        // This function find all the possible moves based on the current state
        std::vector<std::pair<int,int>> findMove(std::unique_ptr<OthelloGameState>&);
        // This function calculates the relative score for the player who is playing the current turn
		int evaluate(std::unique_ptr<OthelloGameState>&);
    };
}

#endif