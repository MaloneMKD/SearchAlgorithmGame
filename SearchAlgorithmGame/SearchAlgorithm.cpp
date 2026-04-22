#include "pch.h"
#include "SearchAlgorithm.h"

bool Breadth_First_Search::FindPath(const std::vector<std::vector<GridItem>>& grid, const Point_Int& start, const Point_Int& goal)
{
	if (grid[start.X][start.Y].isGoal)
        return true;

	m_frontier.push_back(start);
	while(true)
	{
		// The goal was not found and there are no more nodes to expand
		if (m_frontier.empty())
			return false;

		// Take the shallowest node
		Point_Int currentNode = m_frontier.front();
		m_frontier.pop_front();

		// Add to explored set
		m_explored.push_back(currentNode);

		// Check neighbors
		for (auto [dx, dy] : m_neighbors) // Order is clockwise from left: Left -> Top -> Right -> Bottom (and diagonals if 8-connected)
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
				return true;
			}
		}
	}
}

bool Depth_First_Search::FindPath(const std::vector<std::vector<GridItem>>& grid, const Point_Int& start, const Point_Int& goal)
{
	if (grid[start.X][start.Y].isGoal)
		return true;

	m_frontier.push_back(start);
	//for (int i = 0; i < m_frontier.size(); ++i)
	while (true)
	{
		// The goal was not found and there are no more nodes to expand
		if (m_frontier.empty())
			return false;

		// Take the shallowest node
		Point_Int currentNode = m_frontier.back();
		m_frontier.pop_back();

		// Add to explored set
		m_explored.push_back(currentNode);

		// Check neighbors
		for (auto [dx, dy] : m_neighbors) // Order is clockwise from left: Left -> Top -> Right -> Bottom (and diagonals if 8-connected)
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
				return true;
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
			continue;

		// Add to explored set
		m_explored.push_back(currentNode.point);

		// Check neighbors
		for (auto [dx, dy] : m_neighbors) // Order is clockwise from left: Left -> Top -> Right -> Bottom (and diagonals if 8-connected)
		{
			int nx = currentNode.point.X + dx;
			int ny = currentNode.point.Y + dy;

			// If invalid, skip
			if (nx < 0 || nx >= grid.size() || ny < 0 || ny >= grid[0].size())
				continue;

			// If wall, skip
			if (grid[nx][ny].isWall)
				continue;


			// Calculate goal cost g for this neighbor
			Point_Int neighbor{ nx, ny };
			bool isDiagonalMove = (dx != 0 && dy != 0);
			double stepCost = (isDiagonalMove ? std::sqrt(2.0) : 1.0);
			double g = currentNode.cost + stepCost;

			if (std::find(m_explored.begin(), m_explored.end(), neighbor) == m_explored.end())
			{
				m_frontier.push({ neighbor, g });
				// Record the path to this neighbor
				m_pathMap[neighbor] = currentNode.point;
			}
		}
	}
}

bool Greedy_Best_First_Search::FindPath(const std::vector<std::vector<GridItem>>& grid, const Point_Int& start, const Point_Int& goal)
{
	if (grid[start.X][start.Y].isGoal)
		return {};

	m_frontier.push({ start, ManhattanDistance(start, goal) });
	m_frontierSet.insert(start);

	while (true)
	{
		// The goal was not found and there are no more nodes to expand
		if (m_frontier.empty())
			return false;

		// Take the node with the lowest cost
		PriorityPoint currentNode = m_frontier.top();
		m_frontier.pop();
		m_frontierSet.erase(currentNode.point);

		// Goal found
		if (grid[currentNode.point.X][currentNode.point.Y].isGoal)
			return true;

		// Add to explored set
		m_explored.insert(currentNode.point);
		m_solution.push_back(currentNode.point);

		// Check neighbors
		for (auto [dx, dy] : m_neighbors) // Order is clockwise from left : Left->Top->Right->Bottom(and diagonals if 8 - connected)
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
			double h = ManhattanDistance(neighbor, goal);

			// If not in explored or frontier, add to frontier
			if (m_explored.find(neighbor) == m_explored.end() && m_frontierSet.find(neighbor) == m_frontierSet.end())
			{
				m_frontier.push({ neighbor, h });
				m_frontierSet.insert(neighbor);
			}
		}
	}
}

bool A_Star_Search::FindPath(const std::vector<std::vector<GridItem>>& grid, const Point_Int& start, const Point_Int& goal)
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
			continue;

		// Add to explored set
		m_explored.push_back(currentNode.point);

		// Check neighbors
		for (auto [dx, dy] : m_neighbors) // Order is clockwise from left: Left -> Top -> Right -> Bottom (and diagonals if 8-connected)
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
			
			// Determine if diagonal move (only relevant for 8-connected grids)
			bool isDiagonalMove = (dx != 0 && dy != 0);
			double stepCost = (m_gridType == GridType::Four_Connected) ? 1.0 : (isDiagonalMove ? std::sqrt(2.0) : 1.0);
			double g = currentNode.cost + stepCost;
			double h = (m_gridType == GridType::Four_Connected) ? ManhattanDistance(neighbor, goal) : ChebyshevDistance(neighbor, goal);
			double f = g + h;

			if (std::find(m_explored.begin(), m_explored.end(), neighbor) == m_explored.end())
			{
				m_frontier.push({ neighbor, f });

				// Record the path to this neighbor
				m_pathMap[neighbor] = currentNode.point;
			}
		}
	}
}
