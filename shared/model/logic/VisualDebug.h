#pragma once
#include <Point.h>
#include <SDL_gpu.h>

namespace VisualDebug
{
	
	class VisualDebugLine
	{
	public:
		Point from;
		Point to;
		uint32_t color;
	};
	
	class VisualDebugRect
	{
	public:
		GPU_Rect rect;
		bool fill;
		uint32_t color;
		uint8_t alpha;
	};
	
	class VisualDebugCircle
	{
	public:
		Point origin;
		uint32_t radius;
		bool fill;
		uint32_t color;
		uint8_t alpha;
	};

	class VisualDebugStruct
	{
	public:
		std::vector<VisualDebugLine> lines;
		std::vector<VisualDebugLine> arrows;
		std::vector<VisualDebugRect> rects;
		std::vector<VisualDebugCircle> circles;
		
		void clear();
	};
	
	extern int32_t interestingId;
	extern Point interestingPoint;

	void drawLine(Point from, Point to, uint32_t color);
	void drawArrow(Point from, Point to, uint32_t color);
	void drawRect(Point from, Point to, bool fill, uint32_t color, uint8_t alpha = 0xff);
	void drawRect(float x1, float y1, float w, float h, bool fill, uint32_t color, uint8_t alpha = 0xff);
	void drawRect(Point center, float size, bool fill, uint32_t color, uint8_t alpha = 0xff);
	void drawCircle(Point origin, uint32_t radius, bool fill, uint32_t color, uint8_t alpha = 0xff);
	void clear();
}

namespace S
{
	extern VisualDebug::VisualDebugStruct visualDebug;
}
