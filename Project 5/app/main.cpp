// main.cpp
//
// ICS 46 Spring 2020
// Project #5: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.

#include "Digraph.hpp"
#include "InputReader.hpp"
#include "RoadMapReader.hpp"
#include "RoadMapWriter.hpp"
#include "RoadSegment.hpp"
#include "Trip.hpp"
#include "TripMetric.hpp"
#include "TripReader.hpp"
#include <iomanip>
#include <iostream>

//A function that returns distance information
std::function<double(RoadSegment)> getDistance = [](const RoadSegment rs)
{
    return rs.miles;
};

//A function that returns time information
std::function<double(RoadSegment)> getTime = [](const RoadSegment rs)
{
    return rs.miles/rs.milesPerHour * 3600;
};

//A function that print out time information
void printTime(double t)
{
    int hr = 0, min = 0;
    double sec = 0;
    hr = t/3600;
    min = (t - hr*3600) / 60;
    sec = t - hr*3600 - min*60;

    if (hr == 1)
    {
        std::cout << hr << " hr ";
    }
    else if (hr > 1)
    {
        std::cout << hr << " hrs ";
    }
    if (min == 0 || min == 1)
    {
        std::cout << min << " min ";
    }
    else if (min > 1)
    {
        std::cout << min << " mins ";
    }
    if (sec == 0 || sec == 1)
    {
        std::cout << sec << " sec ";
    }
    else if (sec > 1)
    {
        std::cout << sec << " secs ";
    }
}

int main()
{
    InputReader ir(std::cin);
    RoadMapReader rmr;
    TripReader tr;
    RoadMap rm = rmr.readRoadMap(ir);
    
    //Stop if the graph is not strongly connected
    if (!rm.isStronglyConnected())
    {
        std::cout << "Disconnect Map";
        return 0;
    }

    std::vector<Trip> trips = tr.readTrips(ir);

    for (int i = 0; i < trips.size(); ++i)
    {
        std::vector<int> directions;
        int temp = trips[i].endVertex;
        directions.push_back(temp);

        //Print time information
        if (trips[i].metric == TripMetric::Time)
        {
            double totalTime = 0.0;
            std::cout << "Shortest driving time from " << std::fixed << std::setprecision(1) << rm.vertexInfo(trips[i].startVertex) << " to " 
                      << rm.vertexInfo(trips[i].endVertex) << std::endl;
            std::map<int, int> path = rm.findShortestPaths(trips[i].startVertex, getTime);
            while (true)
            {
                directions.push_back(path.at(temp));
                temp = path.at(temp);
                if (temp == trips[i].startVertex)
                {
                    break;
                }
            }
            int fromVertex = directions.back();
            directions.pop_back();
            std::cout << "  Begin at " << rm.vertexInfo(fromVertex) << std::endl;
            while (directions.size() > 0)
            {
                int toVertex = directions.back();
                double t = getTime(rm.edgeInfo(fromVertex, toVertex));
                std::cout << "  Continue to " << std::fixed << std::setprecision(1) << rm.vertexInfo(toVertex) << " (" << rm.edgeInfo(fromVertex, toVertex).miles 
                          << " miles @ " << std::fixed << std::setprecision(1) << rm.edgeInfo(fromVertex, toVertex).milesPerHour << "mph = ";
                printTime(t);
                std::cout << ")" << std::endl;
                fromVertex = toVertex;
                totalTime += t;
                directions.pop_back();
            }
            std::cout << "Total time: ";
            printTime(totalTime);
            std::cout << std::endl;
        }

        //Print distance information
        else
        {
            double totalDistance = 0.0;
            std::cout << "Shortest distance from " << rm.vertexInfo(trips[i].startVertex) << " to " << rm.vertexInfo(trips[i].endVertex) << std::endl;
            std::map<int, int> path = rm.findShortestPaths(trips[i].startVertex, getDistance);
            while (temp != trips[i].startVertex)
            {
                directions.push_back(path.at(temp));
                temp = path.at(temp);
            }
            int fromVertex = directions.back();
            std::cout << "  Begin at " << rm.vertexInfo(fromVertex) << std::endl;
            directions.pop_back();
            while (directions.size() > 0)
            {
                int toVertex = directions.back();
                std::cout << "  Continue to " << rm.vertexInfo(toVertex) << " (" << std::fixed << std::setprecision(1) << rm.edgeInfo(fromVertex, toVertex).miles << " miles)" << std::endl;
				totalDistance += rm.edgeInfo(fromVertex, toVertex).miles;
                fromVertex = toVertex;
                directions.pop_back();
            }
            std::cout << "Total distance: " << std::fixed << std::setprecision(1) << totalDistance << " miles" << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}

