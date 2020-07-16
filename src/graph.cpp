#include "graph.hpp"

#include <algorithm>

Vec2D Plot::math_to_svg(Vec2D math) const
{
	return { math.x * this->x_scale, math.y * this->y_scale };
}

constexpr Vec2D svg_min = Vec2D{ -100, -100 };
constexpr Vec2D svg_max = Vec2D{  100,  100 };

Plot::Plot(const std::string& name, double x_max, double y_max)
	:picture(name, svg_min, svg_max), min({ -x_max, -y_max }), max({ x_max, y_max }),
	x_scale((svg_max.x - svg_min.x) / (2 * x_max)), 
	y_scale(-(svg_max.y - svg_min.y) / (2 * y_max))
{
	const SVG::Style black_line = { 0.4, rgb::black };

	//draw axes as arrows and label them
	this->picture.add_line({ svg_min.x, 0 }, { svg_max.x, 0 }, black_line);
	this->picture.add_line({ 0, svg_min.y }, { 0, svg_max.y }, black_line);
	this->picture.add_line_path({ {svg_max.x - 1, 1.5}, {svg_max.x, 0}, {svg_max.x - 1, -1.5} }, false, black_line);
	this->picture.add_line_path({ {1.5, svg_min.y + 1}, {0, svg_min.y}, {-1.5, svg_min.y + 1} }, false, black_line);
	//this->picture.add_text({ svg_max.x - 5, 6 }, "x", svg_text::large);
	//this->picture.add_text({ -9, svg_min.y + 5 }, "y", svg_text::large);

	if (false) { //draw scale for x
		const auto x_line_at = [](double x, Plot* plot) {
			const SVG::Style thin_black_line = { 0.5, rgb::black };
			plot->picture.add_line({ x * plot->x_scale, 1.5 }, { x * plot->x_scale, -1.5 }, thin_black_line);
		};
		const int x_stepsize = std::max(static_cast<int>((this->max.x - this->min.x) / 20), 1);
		for (int x = x_stepsize; x < this->max.x; x += x_stepsize) {
			x_line_at(x, this);
			this->picture.add_text(Vec2D{ x * x_scale - 2, 5 }, std::to_string(x), svg_text::medium);
			x_line_at(-x, this);
			this->picture.add_text(Vec2D{ -x * x_scale - 2, 5 }, std::to_string(-x), svg_text::medium);
		}
	}

	if (false) { //draw scale for y
		const auto y_line_at = [](double y, Plot* plot) {
			const SVG::Style thin_black_line = { 0.5, rgb::black };
			plot->picture.add_line({ 1.5, y * plot->y_scale }, { -1.5, y * plot->y_scale }, thin_black_line);
		};
		const int y_stepsize = std::max(static_cast<int>((this->max.y - this->min.y) / 20), 1);
		for (int y = y_stepsize; y < this->max.y; y += y_stepsize) {
			y_line_at(y, this);
			this->picture.add_text(Vec2D{ -7, y * y_scale + 1 }, std::to_string(y), svg_text::medium);
			y_line_at(-y, this);
			this->picture.add_text(Vec2D{ -7, -y * y_scale + 1 }, std::to_string(-y), svg_text::medium);
		}
	}
}

void Plot::add_polynomial(const polynomial::Monomials& p, const SVG::Style& style)
{
	std::vector<Vec2D> graph;
	const double dx = (this->max.x - this->min.x) / 500;
	for (double x = this->min.x; x < this->max.x; x += dx) {
		graph.push_back(this->math_to_svg(Vec2D{ x, evaluate(p, x) }));
	}
	this->picture.add_line_path(graph, false, style);
}

void Plot::add_polynomial(const polynomial::Bernstein& p, const SVG::Style& style)
{
	std::vector<Vec2D> graph;
	const double dx = (this->max.x - this->min.x) / 500;
	for (double x = this->min.x; x < this->max.x; x += dx) {
		graph.push_back(this->math_to_svg(Vec2D{ x, evaluate(p, x) }));
	}
	this->picture.add_line_path(graph, false, style);
}

void Plot::add_control_polygon(const polynomial::Bernstein& p, const SVG::Style& style)
{
	std::vector<Vec2D> points;
	points.reserve(p.size());
	for (int i = 0; i <= p.degree(); i++) {
		points.push_back(this->math_to_svg(Vec2D{ p.interval.min + i * p.interval.width() / p.degree(), p[i] }));
	}
	this->picture.add_line_path(points, false, style);
}

void Plot::add_interval(Interval interval, const RGB& color)
{
	const Vec2D upper_left = { interval.min * this->x_scale, svg_min.y };
	const Vec2D upper_right = { interval.max * this->x_scale, svg_min.y };
	const Vec2D lower_left = { interval.min * this->x_scale, svg_max.y };
	const Vec2D lower_right = { interval.max * this->x_scale, svg_max.y };

	this->picture.add_line_path({lower_left, lower_right, upper_right, upper_left}, true, svg_style::fill(color));

	this->picture.add_line(upper_left, lower_left, SVG::Style{ 0.2, color });
	this->picture.add_line(upper_right, lower_right, SVG::Style{ 0.2, color });
}
