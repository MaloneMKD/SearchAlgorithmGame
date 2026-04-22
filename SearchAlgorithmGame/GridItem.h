#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <tuple>

// ===============================POINT STRUCTURE==========================================
struct Point_Int
{
	int X;
	int Y;

	Point_Int() = default;
	Point_Int(int x, int y) : X(x), Y(y) {}

	// Equality operator for Point_Int to be used in comparisons
	bool operator==(const Point_Int& other) const noexcept
	{
		return X == other.X && Y == other.Y;
	}

	// Less-than operator for Point_Int to be used in ordered containers
	bool operator<(const Point_Int& other) const noexcept
	{
		return std::tie(X, Y) < std::tie(other.X, other.Y);
	}
};

// Custom hash function for Point_Int to be used in unordered containers
namespace std
{
	template<>
	struct hash<Point_Int>
	{
		std::size_t operator()(const Point_Int& point) const noexcept
		{
			std::size_t h1 = std::hash<int>{}(point.X);
			std::size_t h2 = std::hash<int>{}(point.Y);
			return h1 ^ (h2 << 1); // Combine the two hashes
		}
	};
}

// ===============================GRID ITEM CLASS==========================================
enum GridType
{
	Four_Connected,
	Eight_Connected
};

class GridItem
{
public:
	GridItem() = default;
	bool HitTest(winrt::Windows::Foundation::Point, int m_xOffset, int m_yOffset) const;

	// Members
	int m_width;
	int m_height;

	Point_Int position = { 0, 0 };
	winrt::Windows::UI::Color m_fillColor{ winrt::Microsoft::UI::Colors::White() };

	bool isWall = false;
	bool isStart = false;
	bool isGoal = false;
};

