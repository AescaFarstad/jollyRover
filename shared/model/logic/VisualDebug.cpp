#include <VisualDebug.h>

namespace VisualDebug
{
	int32_t interestingId;
	Point interestingPoint;
	
	void drawLine(Point from, Point to, uint32_t color)
	{
		S::visualDebug.lines.push_back(VisualDebugLine{from, to, color});
	}

	void drawArrow(Point from, Point to, uint32_t color)
	{
		S::visualDebug.arrows.push_back(VisualDebugLine{from, to, color});
	}

	void drawRect(Point from, Point to, bool fill, uint32_t color, uint8_t alpha)
	{
		S::visualDebug.rects.push_back(VisualDebugRect{GPU_Rect{from.x, from.y, to.x - from.x, to.y - from.y}, fill, color, alpha});
	}

	void drawRect(float x, float y, float w, float h, bool fill, uint32_t color, uint8_t alpha)
	{
		S::visualDebug.rects.push_back(VisualDebugRect{GPU_Rect{x, y, w, h}, fill, color, alpha});
	}

	void drawRect(Point center, float size, bool fill, uint32_t color, uint8_t alpha)
	{
		S::visualDebug.rects.push_back(VisualDebugRect{GPU_Rect{center.x - size, center.y - size, size, size}, fill, color, alpha});
	}

	void drawCircle(Point origin, uint32_t radius, bool fill, uint32_t color, uint8_t alpha)
	{
		S::visualDebug.circles.push_back(VisualDebugCircle{origin, radius, fill, color, alpha});
	}

	void VisualDebugStruct::clear()
	{
		lines.clear();
		arrows.clear();
		rects.clear();
		circles.clear();
	}
	
	void clear()
	{
		S::visualDebug.clear();
	}
}

namespace S
{
	VisualDebug::VisualDebugStruct visualDebug;
}