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

	bool operator==(const Point_Int& other) const noexcept
	{
		return X == other.X && Y == other.Y;
	}

	bool operator<(const Point_Int& other) const noexcept
	{
		return std::tie(X, Y) < std::tie(other.X, other.Y);
	}
};

// ===============================GRID ITEM CLASS==========================================
class GridItem
{
public:
	GridItem() = default;
	bool HitTest(winrt::Windows::Foundation::Point, int m_xOffset, int m_yOffset) const;

	// Members
	static int m_width;
	static int m_height;

	Point_Int position = { 0, 0 };
	winrt::Windows::UI::Color m_fillColor{ winrt::Windows::UI::Colors::White() };

	bool isWall = false;
	bool isStart = false;
	bool isGoal = false;
};

