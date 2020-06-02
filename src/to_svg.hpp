#pragma once

#include <string>
#include <fstream>
#include <vector>

constexpr double pi = 3.14159265358979323846264338327950288419716939937510582097494;

struct RGB
{
	unsigned char red, green, blue;

	std::string to_sting() const;
};

namespace rgb {
	constexpr RGB black{ 0, 0, 0 };
}

struct HSV
{
	double hue;			//assumed to be in [0, 2 * pi]
	double saturation;	//assumed to be in [0, 1]
	double value;		//assumed to be in [0, 1]

	RGB to_rgb() const;
};


struct Vec2D
{
	double x, y;
};


class SVG
{
public:
	struct Style
	{
		double stroke_width;
		RGB stroke_color;
		RGB fill_color;
		bool fill;
		double transparency;
	};

	struct Text_Style
	{
		std::string font;
		double size;
		RGB color;
	};


	SVG(const std::string& name, Vec2D view_box_min, Vec2D view_box_max);
	~SVG();

	void add_line(Vec2D start, Vec2D end, const Style& style);
	void add_line_path(const std::vector<Vec2D>& path, bool close, const Style& style);
	void add_text(Vec2D position, const std::string& content, const Text_Style& style);

private:
	std::ofstream document;
};

namespace svg_text {
	const SVG::Text_Style small  = { "Veranda", 2, rgb::black };
	const SVG::Text_Style medium = { "Veranda", 4, rgb::black };
	const SVG::Text_Style large  = { "Veranda", 6, rgb::black };
}