#pragma once
#include <Point.h>
#include <SDL_gpu.h>
#include <functional>

namespace VisualDebug
{
	class VisualDebugStruct;
}

namespace S
{
	extern VisualDebug::VisualDebugStruct visualDebug;
}

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
	
	class VisualDebugText
	{
	public:
		Point origin;
		std::string text;
	};

	class VisualDebugStruct
	{
	public:
		std::vector<VisualDebugLine> lines;
		std::vector<VisualDebugLine> arrows;
		std::vector<VisualDebugRect> rects;
		std::vector<VisualDebugCircle> circles;
		std::vector<VisualDebugText> texts;
		
		void clear();
	};
	
	extern int32_t interestingId;
	extern Point interestingPoint;

	void drawLine(Point from, Point to, uint32_t color);
	void drawArrow(Point from, Point to, uint32_t color);
	void drawArrow(Point from, Point to, int32_t length, uint32_t color);
	void drawRect(Point from, Point to, bool fill, uint32_t color, uint8_t alpha = 0xff);
	void drawRect(float x1, float y1, float w, float h, bool fill, uint32_t color, uint8_t alpha = 0xff);
	void drawRect(Point center, float size, bool fill, uint32_t color, uint8_t alpha = 0xff);
	void drawCircle(Point origin, uint32_t radius, bool fill, uint32_t color, uint8_t alpha = 0xff);
	
	template<typename... Args>
	void drawText(Point location, const char* text, Args... args)
	{
		char buffer[1024];
		auto len = std::snprintf(buffer, 1024, text, std::forward<Args>(args)...);
		std::string str(buffer);		
		S::visualDebug.texts.push_back(VisualDebugText{location, str.substr(0, len)});
	}
	void clear();
}

