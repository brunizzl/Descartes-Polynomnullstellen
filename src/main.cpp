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
		const Monomials p = from_roots({0, 0, 0, 2, 2, 2, 2, 3}) * from_complex_root_pairs({ {1, 0.15} });
		const Monomials p2 = no_root_multiplicities(p);
		auto root_intervals = descartes_root_isolation(p2, Interval{ -150, 150 });
		std::sort(root_intervals.begin(), root_intervals.end(), [](const Interval& i1, const Interval& i2) { return i1.min < i2.min; });

		std::cout << "roots of  " << p2 << "   are in these intervals:\n";
		std::cout << root_intervals << std::endl;
		Plot plot("test_plot.svg", 5, 10);
		plot.add_polynomial(p2, RGB{ 0, 100, 250 });
		plot.add_polynomial(p, RGB{ 200, 0, 50 });
		for (const auto& interval : root_intervals) {
			plot.add_interval(interval);
		}
	}

	{
		std::cout << "bernstein base test... ";
		const Bernstein b_0({ 1, 0, 0, 0, 0 }, { 0, 1 });
		const Bernstein b_1({ 0, 1, 0, 0, 0 }, { 0, 1 });
		const Bernstein b_2({ 0, 0, 1, 0, 0 }, { 0, 1 });
		const Bernstein b_3({ 0, 0, 0, 1, 0 }, { 0, 1 });
		const Bernstein b_4({ 0, 0, 0, 0, 1 }, { 0, 1 });
		Plot plot2("test_b4.svg", 1, 2);
		plot2.add_polynomial(b_0, RGB{ 0, 0, 0 });
		plot2.add_polynomial(b_1, RGB{ 0, 50, 0 });
		plot2.add_polynomial(b_2, RGB{ 0, 100, 0 });
		plot2.add_polynomial(b_3, RGB{ 0, 150, 0 });
		plot2.add_polynomial(b_4, RGB{ 0, 200, 0 });
		std::cout << "  done\n";
	}

	{
		std::cout << "bernstein roots test...";
		const Bernstein b({ -4, 4, 4,-4, -4, 4, 4, -4, -4, 4 }, { -5, 5 });
		const auto [b1, b2] = de_casteljau_split(b);
		auto roots = descartes_root_isolation(b);
		Plot plot3("test_bernstein.svg", 5, 5);
		plot3.add_polynomial(b, RGB{ 200, 200, 0 });
		plot3.add_control_polygon(b, svg_style::line(rgb::red, 0.2));
		plot3.add_control_polygon(b1, svg_style::line(rgb::blue, 0.2));
		plot3.add_control_polygon(b2, svg_style::line(rgb::blue, 0.2));

		for (const auto& interval : roots) {
			plot3.add_interval(interval);
		}
		std::cout << "  done\n";
	}
}