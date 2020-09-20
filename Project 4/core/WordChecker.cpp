// WordChecker.cpp
//
// ICS 46 Spring 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);;
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    // Nonsensical code because the compiler requires the member variables
    // 'words' to be used somewhere, or else it becomes a warning (which
    // turns unsigned into an error).
    std::vector<std::string> suggestions;
	std::string alphabets = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	//Delete
	for(unsigned int i = 0; i < word.size(); ++i)
	{	
		std::string temp = word;
		temp = temp.erase(i,1);
		if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
        {
			suggestions.push_back(temp);
        }
	}

	//Add
	for(unsigned int i = 0; i < word.size() + 1; ++i)
	{
		for(unsigned int j = 0; j < alphabets.size(); ++j)
		{
			std::string temp = word;
			temp.insert(i,alphabets.substr(j,1));
			if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
            {
				suggestions.push_back(temp);
            }
		}
	}

	//Replace
	for(unsigned int i = 0; i < word.size(); ++i)
	{
		for(unsigned int j = 0; j < alphabets.size(); ++j)
		{
			std::string temp = word;
			temp.replace(i, 1, alphabets.substr(j,1));
			if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
            {
				suggestions.push_back(temp);
            }
		}
	}

	//Swap
	for(unsigned int i = 0; i < word.size() - 1; ++i)
	{
		for (unsigned int j = i + 1; j < word.size(); ++j)
		{
			std::string temp = word;
			std::swap(temp[i],temp[j]);
			if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
            {
				suggestions.push_back(temp);
            }
		}
	}

	//Add Space
	for(unsigned int i = 1; i < word.size(); ++i)
	{
		std::string temp = word;
		temp.insert(i," ");
		if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
        {
            suggestions.push_back(temp);
        }
	}

    return suggestions;
}