#include "pch.h"
#include "GridItem.h"

// Initialize static variables
UINT GridItem::m_width = 10;
UINT GridItem::m_height = 10;

bool GridItem::HitTest(winrt::Windows::Foundation::Point p, UINT m_xOffset, UINT m_yOffset) const
{
	UINT xPosOffset = position.X + m_xOffset;
	UINT yPosOffset = position.Y + m_yOffset;
	if ((p.X > xPosOffset && p.X < xPosOffset + m_width) && (p.Y > yPosOffset && p.Y < yPosOffset + m_height))
		return true;
	return false;
}
