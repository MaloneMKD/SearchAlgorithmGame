#include "pch.h"
#include "SearchAlgorithm.h"

std::vector<Point_Int> Breadth_First_Search::FindPath(const std::vector<std::vector<GridItem>>& grid, const Point_Int& start, const Point_Int& goal)
{
	if (grid[start.X][start.Y].isGoal)
        return m_explored;

	m_frontier.push_back(start);
	//for (int i = 0; i < m_frontier.size(); ++i)
	while(true)
	{
		// The goal was not found and there are no more nodes to expand
		if (m_frontier.empty())
			return m_explored;

		// Take the shallowest node
		Point_Int currentNode = m_frontier.front();
		m_frontier.pop_front();

		// Add to explored set
		m_explored.push_back(currentNode);

		// Check neighbors
		for (auto [dx, dy] : std::vector<std::pair<int, int>>{ {-1,0},{0,1},{1,0},{0,-1} }) // Order is: Left -> Top -> Right -> Bottom
		{
			int nx = currentNode.X + dx;
			int ny = currentNode.Y + dy;

			// Skip invalid neighbors
			if (nx < 0 || nx >= grid.size() || ny < 0 || ny >= grid[0].size())
				continue;

			// Skip walls
			if (grid[nx][ny].isWall)
				continue;

			Point_Int neighbor{ nx, ny };

			// Skip explored nodes
			if (std::find(m_explored.begin(), m_explored.end(), neighbor) != m_explored.end())
				continue;

			// Add to frontier if not already there
			if (std::find(m_frontier.begin(), m_frontier.end(), neighbor) == m_frontier.end())
				m_frontier.push_back(neighbor); // Add to frontier if not already there

			// Goal found
			if (grid[neighbor.X][neighbor.Y].isGoal)
			{
				m_explored.push_back(neighbor);
				return m_explored;
			}
		}
	}
}

std::vector<Point_Int> Depth_First_Search::FindPath(const std::vector<std::vector<GridItem>>& grid, const Point_Int& start, const Point_Int& goal)
{
	if (grid[start.X][start.Y].isGoal)
		return m_explored;

	m_frontier.push_back(start);
	//for (int i = 0; i < m_frontier.size(); ++i)
	while (true)
	{
		// The goal was not found and there are no more nodes to expand
		if (m_frontier.empty())
			return m_explored;

		// Take the shallowest node
		Point_Int currentNode = m_frontier.back();
		m_frontier.pop_back();

		// Add to explored set
		m_explored.push_back(currentNode);

		// Check neighbors
		for (auto [dx, dy] : std::vector<std::pair<int, int>>{ {-1,0},{0,1},{1,0},{0,-1} }) // Order is: Left -> Top -> Right -> Bottom
		{
			int nx = currentNode.X + dx;
			int ny = currentNode.Y + dy;

			// Skip invalid neighbors
			if (nx < 0 || nx >= grid.size() || ny < 0 || ny >= grid[0].size())
				continue;

			// Skip walls
			if (grid[nx][ny].isWall)
				continue;

			Point_Int neighbor{ nx, ny };

			// Skip explored nodes
			if (std::find(m_explored.begin(), m_explored.end(), neighbor) != m_explored.end())
				continue;

			// Add to frontier if not already there
			if (std::find(m_frontier.begin(), m_frontier.end(), neighbor) == m_frontier.end())
				m_frontier.push_back(neighbor); // Add to frontier if not already there

			// Goal found
			if (grid[neighbor.X][neighbor.Y].isGoal)
			{
				m_explored.push_back(neighbor);
				return m_explored;
			}
		}
	}
}

bool Uniform_Cost_Search::FindPath(const std::vector<std::vector<GridItem>>& grid, const Point_Int& start, const Point_Int& goal)
{
	if (grid[start.X][start.Y].isGoal)
		return true;

	m_frontier.push({ start, 0 });

	while (true)
	{
		// The goal was not found and there are no more nodes to expand
		if (m_frontier.empty())
			return false;

		// Take the node with the lowest cost
		PriorityPoint currentNode = m_frontier.top();
		m_frontier.pop();

		// Goal found
		if (grid[currentNode.point.X][currentNode.point.Y].isGoal)
		{
			// Build the path from goal to start using the path map
			Point_Int node = goal;

			while (node != start) 
			{
				m_solution.push_back(node);
				node = m_pathMap[node];
			}
			m_solution.push_back(start);
			std::reverse(m_solution.begin(), m_solution.end());

			return true;
		}

		// If explored, skip
		if (std::find(m_explored.begin(), m_explored.end(), currentNode.point) != m_explored.end())
		{
			m_explored.push_back(currentNode.point);
			continue;
		}

		// Add to explored set
		m_explored.push_back(currentNode.point);

		// Check neighbors
		for (auto [dx, dy] : std::vector<std::pair<int, int>>{ {-1,0},{0,1},{1,0},{0,-1} }) // Order is: Left -> Top -> Right -> Bottom
		{
			int nx = currentNode.point.X + dx;
			int ny = currentNode.point.Y + dy;

			// If invalid, skip
			if (nx < 0 || nx >= grid.size() || ny < 0 || ny >= grid[0].size())
				continue;

			// If wall, skip
			if (grid[nx][ny].isWall)
				continue;

			Point_Int neighbor{ nx, ny };
			int newCost = currentNode.cost + 1;

			if (std::find(m_explored.begin(), m_explored.end(), neighbor) == m_explored.end())
			{
				m_frontier.push({ neighbor, newCost });

				// Record the path to this neighbor
				m_pathMap[neighbor] = currentNode.point;
			}
		}
	}
}

std::vector<Point_Int> Greedy_Best_First_Search::FindPath(const std::vector<std::vector<GridItem>>& grid, const Point_Int& start, const Point_Int& goal)
{
	auto getCost = [](const Point_Int& a, const Point_Int& b)
		{
			return (int)(std::abs(a.X - b.X) + std::abs(a.Y - b.Y)); // Manhattan distance as cost
		};

	if (grid[start.X][start.Y].isGoal)
		return {};

	m_frontier.push({ start, getCost(start, goal) });

	while (true)
	{
		// The goal was not found and there are no more nodes to expand
		if (m_frontier.empty())
			return m_explored;

		// Take the node with the lowest cost
		PriorityPoint currentNode = m_frontier.top();
		m_frontier.pop();

		// Goal found
		if (grid[currentNode.point.X][currentNode.point.Y].isGoal)
			return m_explored;

		// Add to explored set
		m_explored.push_back(currentNode.point);

		// Check neighbors
		for (auto [dx, dy] : std::vector<std::pair<int, int>>{ {-1,0},{0,1},{1,0},{0,-1} }) // Order is: Left -> Top -> Right -> Bottom
		{
			int nx = currentNode.point.X + dx;
			int ny = currentNode.point.Y + dy;

			// If invalid, skip
			if (nx < 0 || nx >= grid.size() || ny < 0 || ny >= grid[0].size())
				continue;

			// If it is a wall, skip
			if (grid[nx][ny].isWall)
				continue;

			Point_Int neighbor{ nx, ny };
			int newCost = getCost(neighbor, goal);

			// If not in explored or frontier, add to frontier
			if (std::find(m_explored.begin(), m_explored.end(), neighbor) == m_explored.end())
				m_frontier.push({ neighbor, newCost });
		}
	}
}
