// expmain.cpp
//
// ICS 46 Spring 2020
// Project #3: Black and White
//
// Do whatever you'd like here.  This is intended to allow you to experiment
// with the given classes in the othellogame library, or with your own
// algorithm implementations, outside of the context of the GUI or
// Google Test.
#include "OthelloGameStateFactory.hpp"
#include "MyOthelloAI.hpp"
#include <iostream>
#include <string>

int main()
{
    int width = 8;
    int height = 8;
    std::string s = "white";
    std::string s2 = "black";
    OthelloGameStateFactory o;
    std::unique_ptr<OthelloGameState> state = o.makeNewGameState(width, height);
    haininz::MyOthelloAI ai;
    //ai.chooseMove(*state);
    //ai.search(state, 2, s);
    //ai.findMove(state);
    ai.evaluate(state);
    return 0;
}

