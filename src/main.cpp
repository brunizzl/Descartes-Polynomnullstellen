#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

#include "descartes.hpp"
#include "graph.hpp"

using namespace polynomial;

int main()
{
	//const polynomial::Monomials p = polynomial::Monomials{ 1, 5, -2, 1, -1 };
	const Monomials p = from_roots({0, 0, 0, 1, 2, 2, 2, 2, 3});
	auto root_intervals = descartes_root_isolation(p, Interval{ -150, 150 });
	std::sort(root_intervals.begin(), root_intervals.end(), [](const Interval& i1, const Interval& i2) { return i1.min < i2.min; });

	std::cout << "roots of  " << p.coeffs << "   are in these intervals:\n";
	std::cout << root_intervals << std::endl;

	Plot plot("test_plot.svg", 5, 10);
	//plot.add_polinomial_derivative(p, RGB{ 0, 50, 125 });
	plot.add_polinomial(p, RGB{ 0, 100, 250 });
	for (const auto& interval : root_intervals) {
		plot.add_interval(interval);
	}
}