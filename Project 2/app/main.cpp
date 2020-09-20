// main.cpp
//
// ICS 46 Spring 2020
// Project #2: Time Waits for No One
//
// This is the entry point for your simulation application.
//
// All of the code that makes up your simulation -- which is everything
// except for your DoublyLinkedList and Queue implementations -- should
// be in the "app" directory, though, naturally, it shouldn't all be in
// this file.  A design that keeps separate things separate is always
// part of the requirements.
#include <iomanip>
#include "Queue.hpp"
#include "HelperFunction.hpp"  
int main()
{
    unsigned int time = 0; //the time of the system
    unsigned int duration; //length of simulation
    unsigned int num_of_reg; //number of register
    unsigned int line_length; //the length of each line(queue)
    char line_format; //either "M"(multiple) or "S"(single)
    std::vector<unsigned int> regist_time; //a vector that stores the process time for each vector
    std::vector<std::pair<bool, unsigned int>> regist_condition; //a vector that stores if each register is occupied by a customer
    std::vector<std::pair<unsigned int, unsigned int>> customer; //a vector that stores the number of customers and when they come as a paired value
    unsigned int num_of_cust = 0; //the number of customers in a coming group
    unsigned int arrive_time = 0; //the time when a group of customers arrive
    unsigned int customer_index = 0; //keep track of the index of the customer vector

    //variables for final stats calculation
    unsigned int total_entered_line = 0;
    unsigned int total_exited_line = 0;
    unsigned int total_exited_reg = 0;
    float total_wait = 0.0;
    unsigned int total_lost = 0;

    //Get user input
    std::cin >> duration;
    std::cin >> num_of_reg;
    std::cin >> line_length;
    std::cin >> line_format;
    duration *= 60;

    //Create a vector that stores all of the lines(queues)
    std::vector<Queue<unsigned int>> lines;
    if (line_format == 'M')
    {
        for (unsigned int i = 0; i < num_of_reg; ++i)
        {
            lines.push_back(Queue<unsigned int>{});
        }
    }

    //Fill in reg_time
    for (unsigned int i = 0; i < num_of_reg; ++i)
    {
        unsigned int reg_time;
        std::cin >> reg_time;
        regist_time.push_back(reg_time);
        regist_condition.push_back(std::pair{ false, 0 });
    }

    //Fill in customer
    while (true)
    {
        std::string num_of_cust;
        unsigned int arrive_time;
        std::cin >> num_of_cust;
        if (num_of_cust == "END")
        {
            break;
        }
        std::cin >> arrive_time;
        customer.push_back(std::pair{ std::stoi(num_of_cust), arrive_time });
    }

    //start simulation
    std::cout << "LOG" << "\n" << "0 start" << std::endl;

    while (time < duration)
    {
        if (customer_index <= customer.size())
        {
            num_of_cust = customer[customer_index].first;
            arrive_time = customer[customer_index].second;
        }
        if (time == arrive_time)
        {
            enter_line(lines, num_of_cust, time, line_length, total_entered_line, total_lost);
            customer_index ++;
        }
        reg(regist_condition, regist_time, lines, time, total_exited_line, total_exited_reg, total_wait);
        increment_time(regist_condition);
        time ++;
    }

    //calculate "Left In Line" and "Left In Register"
    int left_in_line = 0;
    for (int i = 0; i < lines.size(); ++i)
    {
        left_in_line += lines[i].size();
    }

    int left_in_reg = 0;
    for (int i = 0; i < regist_condition.size(); ++i)
    {
        if (regist_condition[i].first == false || regist_condition[i].second != 0)
        {
            left_in_reg ++;
        }
    }
    //end simulation
    std::cout << duration << " end" << std::endl;

    //print out final stats
    std::cout << std::endl
    << "Entered Line    : " << total_entered_line << std::endl
    << "Exited Line     : " << total_exited_line << std::endl
    << "Exited Register : " << total_exited_reg << std::endl
    << "Avg Wait Time   : " << std::fixed << std::setprecision(2) << total_wait / total_exited_line << std::endl
    << "Left In Line    : " << left_in_line << std::endl
    << "Left In Register: " << left_in_reg << std::endl
    << "Lost            : " << total_lost << std::endl;

    return 0;
}