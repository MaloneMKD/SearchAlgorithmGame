#pragma once

#include <winrt/base.h>
#include <vector>
#include <deque>
#include <queue>
#include <map>
#include <unordered_set>
#include "GridItem.h"

class SearchAlgorithm
{
protected:
	std::vector<std::pair<int, int>> m_neighbors;
	GridType m_gridType;
};

class InformedSearchAlgorithm : public SearchAlgorithm
{
protected:
	// Heuristic Functions
	double ManhattanDistance(const Point_Int& a, const Point_Int& b)
	{
		return std::abs(a.X - b.X) + std::abs(a.Y - b.Y);
	}

	double EuclideanDistance(const Point_Int& a, const Point_Int& b)
	{
		return (int)std::sqrt(std::pow(a.X - b.X, 2) + std::pow(a.Y - b.Y, 2));
	}

	double ChebyshevDistance(const Point_Int& a, const Point_Int& b)
	{
		return max(std::abs(a.X - b.X), std::abs(a.Y - b.Y));
	}
};

/*=========================================BREADTH_FIRST=========================================*/

class Breadth_First_Search : public SearchAlgorithm
{
public:
	Breadth_First_Search(GridType gridType = GridType::Four_Connected)
	{
		if (gridType == GridType::Four_Connected)
			m_neighbors = { {-1, 0}, { 0,1 }, { 1,0 }, { 0,-1 } };
		else
			m_neighbors =  { {-1, 0}, { -1,-1 }, { 0,-1 }, { 1,-1 }, { 1,0 }, { 1,1 }, { 0,1 }, { -1,1 } };

		m_gridType = gridType;
	}

	bool FindPath(const std::vector<std::vector<GridItem>>& grid,
		const Point_Int& start,
		const Point_Int& goal);

	// The set of all nodes that have been explored
	std::vector<Point_Int> m_explored;

private:
	// The set of all leaf nodes available for expansion
	std::deque<Point_Int> m_frontier;
};

/*=========================================DEPTH_FIRST=========================================*/

class Depth_First_Search : public SearchAlgorithm
{
public:
	Depth_First_Search(GridType gridType = GridType::Four_Connected)
	{
		if (gridType == GridType::Four_Connected)
			m_neighbors = { {-1, 0}, { 0,1 }, { 1,0 }, { 0,-1 } };
		else
			m_neighbors = { {-1, 0}, { -1,-1 }, { 0,-1 }, { 1,-1 }, { 1,0 }, { 1,1 }, { 0,1 }, { -1,1 } };

		m_gridType = gridType;
	}

	bool FindPath(const std::vector<std::vector<GridItem>>& grid,
		const Point_Int& start,
		const Point_Int& goal);

	// The set of all nodes that have been explored
	std::vector<Point_Int> m_explored;

private:
	// The set of all leaf nodes available for expansion
	std::vector<Point_Int> m_frontier;
};

/*=========================================UNIFORM_COST=========================================*/

// A struct to represent a point with its associated priority for use in the priority queue
struct PriorityPoint
{
	Point_Int point;
	double cost;
	bool operator<(const PriorityPoint& other) const
	{
		return cost > other.cost; // Lower priority points should come first
	}

	bool operator==(const PriorityPoint& other) const
	{
		return cost == other.cost;
	}
};


class Uniform_Cost_Search : public SearchAlgorithm
{
public:
	Uniform_Cost_Search(GridType gridType = GridType::Four_Connected)
	{
		if (gridType == GridType::Four_Connected)
			m_neighbors = { {-1, 0}, { 0,1 }, { 1,0 }, { 0,-1 } };
		else
			m_neighbors = { {-1, 0}, { -1,-1 }, { 0,-1 }, { 1,-1 }, { 1,0 }, { 1,1 }, { 0,1 }, { -1,1 } };

		m_gridType = gridType;
	}

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

class Greedy_Best_First_Search : public InformedSearchAlgorithm
{
public:
	Greedy_Best_First_Search(GridType gridType = GridType::Four_Connected)
	{
		if (gridType == GridType::Four_Connected)
			m_neighbors = { {-1, 0}, { 0,1 }, { 1,0 }, { 0,-1 } };
		else
			m_neighbors = { {-1, 0}, { -1,-1 }, { 0,-1 }, { 1,-1 }, { 1,0 }, { 1,1 }, { 0,1 }, { -1,1 } };

		m_gridType = gridType;
	}

	bool FindPath(const std::vector<std::vector<GridItem>>& grid,
		const Point_Int& start,
		const Point_Int& goal);


	// The set of all leaf nodes available for expansion
	std::priority_queue<PriorityPoint> m_frontier;

	// The set of all nodes that have been explored
	std::unordered_set<Point_Int> m_explored;

	// The set of the path from start to goal
	std::vector<Point_Int> m_solution;

private:
	std::unordered_set<Point_Int> m_frontierSet; // Used for checking if a point is already in the frontier
};


/*========================================= A* ==========================================*/
class A_Star_Search : public InformedSearchAlgorithm
{
public:
	A_Star_Search(GridType gridType = GridType::Four_Connected)
	{
		if (gridType == GridType::Four_Connected)
			m_neighbors = { {-1, 0}, { 0,1 }, { 1,0 }, { 0,-1 } };
		else
			m_neighbors = { {-1, 0}, { -1,-1 }, { 0,-1 }, { 1,-1 }, { 1,0 }, { 1,1 }, { 0,1 }, { -1,1 } };

		m_gridType = gridType;
	}

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
