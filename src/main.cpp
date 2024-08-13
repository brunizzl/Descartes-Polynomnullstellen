#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

#include "descartes.hpp"
#include "graph.hpp"

using namespace polynomial;

int main()
{

	{
		std::cout << "monomials base test ...";
		const Monomials m_0({ 1, 0, 0, 0, 0 });
		const Monomials m_1({ 0, 1, 0, 0, 0 });
		const Monomials m_2({ 0, 0, 1, 0, 0 });
		const Monomials m_3({ 0, 0, 0, 1, 0 });
		const Monomials m_4({ 0, 0, 0, 0, 1 });
		Plot plot("svg/monomials_base.svg", 2, 2);
		plot.add_polynomial(m_0, svg_style::line(RGB{ 0,   0, 0 }, 0.5));
		plot.add_polynomial(m_1, svg_style::line(RGB{ 0,  50, 0 }, 0.5));
		plot.add_polynomial(m_2, svg_style::line(RGB{ 0, 100, 0 }, 0.5));
		plot.add_polynomial(m_3, svg_style::line(RGB{ 0, 150, 0 }, 0.5));
		plot.add_polynomial(m_4, svg_style::line(RGB{ 0, 200, 0 }, 0.5));
		std::cout << "  done\n";
	}

	{
		std::cout << "monomials roots test...";
		const Monomials p = from_roots({-0.5, 0.5, 2.8, 3.4}) * from_complex_root_pairs({ {-2, 0.2} }) * 0.05;
		const Monomials p2 = no_root_multiplicities(p);
		auto root_intervals = descartes_root_isolation(p, Interval{ -60, 150 });

		Plot plot("svg/monomials_roots.svg", 5, 5);
		plot.add_polynomial(p2, svg_style::line(rgb::navy, 0.5));
		plot.add_polynomial(p, svg_style::line(rgb::salmon, 0.5));
		for (const auto& interval : root_intervals) {
			plot.add_interval(interval);
		}
		std::cout << "  done\n";
	}

	{
		std::cout << "bernstein base test ...";
		const Bernstein b_0({ 1, 0, 0, 0, 0 }, { 0, 10 });
		const Bernstein b_1({ 0, 1, 0, 0, 0 }, { 0, 10 });
		const Bernstein b_2({ 0, 0, 1, 0, 0 }, { 0, 10 });
		const Bernstein b_3({ 0, 0, 0, 1, 0 }, { 0, 10 });
		const Bernstein b_4({ 0, 0, 0, 0, 1 }, { 0, 10 });
		Plot plot("svg/bernstein_base.svg", 10, 2);
		plot.add_polynomial(b_0, svg_style::line(RGB{ 0,   0, 0 }, 0.5));
		plot.add_polynomial(b_1, svg_style::line(RGB{ 0,  50, 0 }, 0.5));
		plot.add_polynomial(b_2, svg_style::line(RGB{ 0, 100, 0 }, 0.5));
		plot.add_polynomial(b_3, svg_style::line(RGB{ 0, 150, 0 }, 0.5));
		plot.add_polynomial(b_4, svg_style::line(RGB{ 0, 200, 0 }, 0.5));
		std::cout << "  done\n";
	}

	{
		std::cout << "bernstein roots test...";
		const Bernstein b({ -4, 4, 4,-4, -4, 4, 4, -4, -4, 4 }, { -5, 5 });
		const auto [b1, b2] = de_casteljau_split(b);
		auto roots = descartes_root_isolation(b);
		Plot plot("svg/bernstein_roots.svg", 5, 5);
		plot.add_polynomial(b, svg_style::line(rgb::salmon, 0.8));
		plot.add_control_polygon(b, svg_style::line(rgb::red, 0.3));
		plot.add_control_polygon(b1, svg_style::line(rgb::blue, 0.3));
		plot.add_control_polygon(b2, svg_style::line(rgb::green, 0.3));

		for (const auto& interval : roots) {
			plot.add_interval(interval);
		}
		std::cout << "  done\n";
	}

	{
		std::cout << "base transform test ...";
		Monomials monomials = from_roots({ 0, 1, 2, 3, 4 });
		Bernstein bernstein = to_bernstein(monomials, { -5, 41 });
		monomials = monomials * 0.95;

		Plot plot("svg/base_transform.svg", 5, 5);
		plot.add_polynomial(monomials, svg_style::line(rgb::salmon, 0.5));
		plot.add_polynomial(bernstein, svg_style::line(rgb::navy, 0.5));
		std::cout << "  done\n";
	}
}