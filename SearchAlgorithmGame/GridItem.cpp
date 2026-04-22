#include "pch.h"
#include "GridItem.h"

// Initialize static variables

bool GridItem::HitTest(winrt::Windows::Foundation::Point p, int m_xOffset, int m_yOffset) const
{
	int xPosOffset = position.X + m_xOffset;
	int yPosOffset = position.Y + m_yOffset;
	if ((p.X > xPosOffset && p.X < xPosOffset + m_width) && (p.Y > yPosOffset && p.Y < yPosOffset + m_height))
		return true;
	return false;
}
