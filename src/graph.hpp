#pragma once

#include "to_svg.hpp"
#include "descartes.hpp"

class Plot
{
private:
	SVG picture;
	Vec2D min;
	Vec2D max;
	double x_scale;	//factors to get from math to svg coordinates
	double y_scale;

	Vec2D math_to_svg(Vec2D math) const;

public:
	Plot(const std::string& name, double x_max, double y_max);

	void add_polynomial(const polynomial::Monomials& p, const RGB& color, double stroke = 0.6);
	void add_polynomial(const polynomial::Bernstein& p, const RGB& color, double stroke = 0.6);
	void add_control_polygon(const polynomial::Bernstein& p, const SVG::Style& style = svg_style::red_line);
	void add_interval(Interval interval, const RGB& color = RGB{ 0, 200, 0 });
};