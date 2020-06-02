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

	void add_polinomial(const polynomial::Monomials& p, const RGB& color);
	void add_polinomial_derivative(const polynomial::Monomials& p, const RGB& color);
	void add_interval(Interval interval, const RGB& color = RGB{ 0, 200, 0 });
};