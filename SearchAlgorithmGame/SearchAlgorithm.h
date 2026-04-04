#pragma once

#include <winrt/base.h>
#include <vector>
#include <deque>
#include <queue>
#include <map>
#include "GridItem.h"

/*=========================================BREADTH_FIRST=========================================*/

class Breadth_First_Search
{
public:
	std::vector<Point_Int> FindPath(const std::vector<std::vector<GridItem>>& grid,
		const Point_Int& start,
		const Point_Int& goal);

private:
	// The set of all leaf nodes available for expansion
	std::deque<Point_Int> m_frontier;

	// The set of all nodes that have been explored
	std::vector<Point_Int> m_explored;
};

/*=========================================DEPTH_FIRST=========================================*/

class Depth_First_Search
{
public:
	std::vector<Point_Int> FindPath(const std::vector<std::vector<GridItem>>& grid,
		const Point_Int& start,
		const Point_Int& goal);

private:
	// The set of all leaf nodes available for expansion
	std::vector<Point_Int> m_frontier;

	// The set of all nodes that have been explored
	std::vector<Point_Int> m_explored;
};

/*=========================================UNIFORM_COST=========================================*/

// A struct to represent a point with its associated priority for use in the priority queue
struct PriorityPoint
{
	Point_Int point;
	int cost;
	bool operator<(const PriorityPoint& other) const
	{
		return cost > other.cost; // Lower priority points should come first
	}

	bool operator==(const PriorityPoint& other) const
	{
		return cost == other.cost;
	}
};


class Uniform_Cost_Search
{
public:
	bool FindPath(const std::vector<std::vector<GridItem>>& grid,
		const Point_Int& start,
		const Point_Int& goal);

	/******* Public lists for user to fetch results after search is complete *******/

	// Solution - Shortest path from start to goal
	std::vector<Point_Int> m_solution;

	// The set of all leaf nodes available for expansion
	std::priority_queue<PriorityPoint> m_frontier;

	// The set of all nodes that have been explored
	std::vector<Point_Int> m_explored;

private:
	// Map to reconstruct the path from goal to start
	std::map<Point_Int, Point_Int> m_pathMap;
};


/*=========================================Greedy Best First=========================================*/

class Greedy_Best_First_Search
{
public:
	std::vector<Point_Int> FindPath(const std::vector<std::vector<GridItem>>& grid,
		const Point_Int& start,
		const Point_Int& goal);

private:
	// The set of all leaf nodes available for expansion
	std::priority_queue<PriorityPoint> m_frontier;

	// The set of all nodes that have been explored
	std::vector<Point_Int> m_explored;

	// The set of the path from start to goal
	std::vector<Point_Int> m_solution;
};
