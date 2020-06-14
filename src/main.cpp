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

	Plot plot("test_plot.svg", 5, 10);
	plot.add_polinomial(p2, RGB{ 0, 100, 250 });
	for (const auto& interval : root_intervals) {
		plot.add_interval(interval);
	}
}