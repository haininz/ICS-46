#include <ics46/factory/DynamicFactory.hpp>
#include "MyOthelloAI.hpp"
#include "OthelloException.hpp"
#include "limits.h"
#include<iostream>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, haininz::MyOthelloAI, "HAINING AI(REQUIRED)");

// This function chooses the best coordinate (The move that will maximize the possibility of winning) for the current game state
std::pair<int, int> haininz::MyOthelloAI::chooseMove(const OthelloGameState& state)
{
	int best_score = INT_MIN;
	std::pair<int,int> answer;
	std::unique_ptr<OthelloGameState> othello_copy = state.clone();
	std::vector<std::pair<int,int>> all_possible_moves = haininz::MyOthelloAI::findMove(othello_copy);
	std::string turn;
	if (state.isWhiteTurn())
	{
		turn = "white";
	}
	else
	{
		turn = "black";
	}
	if (all_possible_moves.empty())
	{
		std::string error_message = "NO MORE MOVE!";
		throw OthelloException(error_message);
	}
	for (std::vector<std::pair<int,int>>::iterator i = all_possible_moves.begin(); i != all_possible_moves.end(); ++i)
	{
		std::unique_ptr<OthelloGameState> temp = state.clone();
		temp->makeMove(i->first, i->second);
		int score = search(temp, 2, turn);
		if (score >= best_score)
		{
			best_score = score;
			answer.first = i->first;
			answer.second = i->second;
		}
	}
	//std::cout << turn << ": move to (" << answer.first << ", " << answer.second << ")" << std::endl;
	return answer;
}

// This function recursively searches for the score for each possible move and returns the highest score
int haininz::MyOthelloAI::search(std::unique_ptr<OthelloGameState>& state, int depth, std::string turn)
{
	std::vector<std::pair<int,int>> all_possible_moves = haininz::MyOthelloAI::findMove(state);
	if (depth == 0)
	{
		return evaluate(state);
	}
	else
	{
		int my_turn_score = INT_MIN;
		int opponent_turn_score = INT_MAX;
		if ((state->isWhiteTurn() && turn == "white") || (state->isBlackTurn() && turn == "black"))
		{
			for (std::vector<std::pair<int,int>>::iterator i = all_possible_moves.begin(); i != all_possible_moves.end(); ++i)
			{
				std::unique_ptr<OthelloGameState> state_copy = state->clone();
				state_copy->makeMove(i->first, i->second);
				int temp = search(state_copy, depth-1, turn);
				if (temp > my_turn_score)
				{
					my_turn_score = temp;
				}				
			}
			return my_turn_score;
		}
		else
		{
			for (std::vector<std::pair<int,int>>::iterator j = all_possible_moves.begin(); j != all_possible_moves.end(); ++j)
			{
				std::unique_ptr<OthelloGameState> state_copy = state->clone();
				state_copy->makeMove(j->first, j->second);
				int temp = search(state_copy, depth-1, turn);
				if (temp < opponent_turn_score)
				{
					opponent_turn_score = temp;
				}				
			}
			return opponent_turn_score;
		}
	}
}

// This function find all the possible moves based on the current state
std::vector<std::pair<int,int>> haininz::MyOthelloAI::findMove(std::unique_ptr<OthelloGameState>& state)
{
	std::vector<std::pair<int,int>> all_possible_moves;
	for (int i = 0; i < state->board().width(); ++i)
		for (int j = 0; j < state->board().height(); ++j)
			if (state->isValidMove(i,j))
            {
				all_possible_moves.push_back(std::pair<int,int>(i,j));
            }
	return all_possible_moves;
}

// This function calculates the relative score for the player who is playing the current turn
int haininz::MyOthelloAI::evaluate(std::unique_ptr<OthelloGameState>& state)
{
	if (state->isWhiteTurn())
	{
		return (state->whiteScore()-state->blackScore());
	}
	else
	{
		return (state->blackScore()-state->whiteScore());
	}
}