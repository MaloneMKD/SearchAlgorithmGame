#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.h>


class GridItem
{
public:
	GridItem() = default;
	bool HitTest(winrt::Windows::Foundation::Point, UINT m_xOffset, UINT m_yOffset) const;

	// Members
	static UINT m_width;
	static UINT m_height;

	bool m_isWall = false;
	bool isStartBlock = false;
	bool isGoal = false;

	winrt::Windows::Foundation::Point position = { 0, 0 };
	winrt::Windows::UI::Color m_fillColor{ winrt::Windows::UI::Colors::White() };
};

