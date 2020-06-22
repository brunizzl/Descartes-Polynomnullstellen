#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

#include "descartes.hpp"
#include "graph.hpp"

using namespace polynomial;

int main()
{
	const Monomials p = from_roots({0, 0, 0, 2, 2, 2, 2, 3}) * from_complex_root_pairs({ {1, 0.15} });
	const Monomials p2 = no_root_multiplicities(p);
	auto root_intervals = descartes_root_isolation(p2, Interval{ -150, 150 });
	std::sort(root_intervals.begin(), root_intervals.end(), [](const Interval& i1, const Interval& i2) { return i1.min < i2.min; });

	std::cout << "roots of  " << p2 << "   are in these intervals:\n";
	std::cout << root_intervals << std::endl;
	{
		Plot plot("test_plot.svg", 5, 10);
		plot.add_polynomial(p2, RGB{ 0, 100, 250 });
		plot.add_polynomial(p, RGB{ 200, 0, 50 });
		for (const auto& interval : root_intervals) {
			plot.add_interval(interval);
		}
	}

	{
		const Bernstein b({ 1, -2, 3, -4, 5, -6 }, { -5, 5 });
		auto bernstein_roots = descartes_root_isolation(b);
		Plot plot2("test_b.svg", 5, 10);
		plot2.add_polynomial(b, RGB{ 0, 100, 250 });
		for (const auto& interval : bernstein_roots) {
			plot2.add_interval(interval);
		}
	}
}