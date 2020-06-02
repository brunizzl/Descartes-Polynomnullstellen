#include "to_svg.hpp"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <array>

std::string RGB::to_sting() const
{
	const auto char_to_hex = [](unsigned char ch) -> std::array<char, 2> {
		const char* const digits = "0123456789abcdef";
		return { digits[(ch >> 4) & 0xf], digits[ch & 0xf] };
	};

	const auto r = char_to_hex(this->red);
	const auto g = char_to_hex(this->green);
	const auto b = char_to_hex(this->blue);
	return std::string{ '#', r[0], r[1], g[0], g[1], b[0], b[1] };
}


RGB HSV::to_rgb() const
{
	//formulas taken from here : https://en.wikipedia.org/wiki/HSL_and_HSV

	const double angle = this->hue * 3 / pi;  //in interval [0, 5]
	const double chroma = this->value * this->saturation;
	const double rest = chroma * (1.0 - std::fabs(std::fmod(angle, 2.0) - 1.0));

	double red = 0, green = 0, blue = 0;

	switch (static_cast<int>(angle)) {
	case 0: red = chroma; green = rest;   blue = 0;      break;
	case 1: red = rest;   green = chroma; blue = 0;      break;
	case 2: red = 0;      green = chroma; blue = rest;   break;
	case 3: red = 0;      green = rest;   blue = chroma; break;
	case 4: red = rest;   green = 0;      blue = chroma; break;
	case 5: red = chroma; green = 0;      blue = rest;   break;
	default: assert(false);
	}

	const double grey_part = this->value - chroma;
	red = (red + grey_part) * 255;
	green = (green + grey_part) * 255;
	blue = (blue + grey_part) * 255;

	assert(red <= 255 && green <= 255 && blue <= 255);
	return RGB{ static_cast<unsigned char>(red), static_cast<unsigned char>(green), static_cast<unsigned char>(blue) };
}


SVG::SVG(const std::string& name, Vec2D view_box_min, Vec2D view_box_max)
	:document(name)
{
	this->document << "<!DOCTYPE html>\n";
	this->document << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"" 
		<< view_box_min.x << ' ' << view_box_min.y << ' ' << view_box_max.x - view_box_min.x << ' ' << view_box_max.y - view_box_min.y << "\">\n";
}

SVG::~SVG()
{
	this->document << "</svg>\n";
	this->document.close();
}

void SVG::add_line(Vec2D start, Vec2D end, const SVG::Style& style)
{
	this->document 
		<< "  <line x1=\"" << start.x << "\" y1=\"" << start.y << "\" x2=\"" << end.x << "\" y2=\"" << end.y << "\"\n"
		<< "    stroke=\"" << style.stroke_color.to_sting() << "\" stroke-width=\"" << style.stroke_width << "\""
		<< "/>\n";
}

void SVG::add_line_path(const std::vector<Vec2D>& path, bool close, const SVG::Style& style)
{
	this->document << "  <path d=\"M";
	for (std::size_t i = 0; i < path.size(); i++) {
		this->document << path[i].x << " " << path[i].y << " ";
		if ((i + 1) % 8 == 0) {
			this->document << "\n    ";
		}
	}
	if (close) {
		this->document << "Z";
	}
	this->document << "\"\n    stroke=\"" << style.stroke_color.to_sting() << "\" stroke-width=\"" << style.stroke_width << "\" fill=\"";
	if (style.fill) {
		this->document << style.fill_color.to_sting() << "\" fill-opacity=\"" << style.transparency << "\"";
	}
	else {
		this->document << "none\"";
	}
	this->document << "/>\n";
}

void SVG::add_text(Vec2D position, const std::string& content, const SVG::Text_Style& style)
{
	this->document 
		<< "  <text x=\"" << position.x << "\" y=\"" << position.y << "\" "
		<< "font-family=\"" << style.font << "\" fill=\"" << style.color.to_sting() <<"\" font-size=\"" << style.size << "\">"
		<< content
		<< "</text>\n";
}
