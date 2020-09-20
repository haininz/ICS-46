#include <iostream>
#include <vector>
#include <string>
void enter_line(
    std::vector<Queue<unsigned int>>& lines, 
    unsigned int customers, unsigned int time, 
    unsigned int line_length, unsigned int& total_enter_line, 
    unsigned int& total_lost)
{
    for (int i = 0; i < customers; ++i)
    {
        unsigned int index = 0;
        unsigned int size = lines[0].size();
        for (int i = 0; i < lines.size(); ++i)
        {
            if (lines[i].size() < size)
            {
                size = lines[i].size();
                index = i;
            }
        }
        if (size == line_length)
        {
            std::cout << time << " lost" << std::endl;
            total_lost ++;
        }
        else
        {
            lines[index].enqueue(time);
            total_enter_line ++;
            std::cout << time << " entered line " << index + 1 << " length " << size + 1 << std::endl;
        }
    }
}


void reg(
    std::vector<std::pair<bool, unsigned int>>& regist_condition, 
    std::vector<unsigned int>regist_time, 
    std::vector<Queue<unsigned int>>& lines, 
    unsigned int time, unsigned int& total_exit_line,
    unsigned int& total_exit_reg, float& total_wait)
{
    for (int i = 0; i < regist_condition.size(); ++i)
    {
        if (regist_condition[i].second == regist_time[i])
        {
            regist_condition[i].first = false;
            regist_condition[i].second = 0;
            total_exit_reg ++;
            std::cout << time << " exited register " << i + 1 << std::endl;
        }
        if (regist_condition[i].first == false && lines[i].size() != 0)
        {
            std::cout << time
            << " exited line " << i + 1
            << " length " << lines[i].size() - 1
            << " wait time " << time - lines[i].front() << std::endl;
            total_wait += time - lines[i].front();
            lines[i].dequeue();
            regist_condition[i].first = true;
            total_exit_line ++;
            std::cout << time << " entered register " << i + 1 << std::endl;
        }
    }
}


void increment_time(std::vector<std::pair<bool, unsigned int>>& regist_condition)
{
    for (unsigned int i = 0; i < regist_condition.size(); ++i)
    {
        if (regist_condition[i].first == true)
        {
            regist_condition[i].second ++;
        }
    }
}