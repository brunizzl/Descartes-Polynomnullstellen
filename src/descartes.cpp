#include "descartes.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <cassert>

std::ostream& operator<<(std::ostream& stream, Interval interval)
{
	return stream << '[' << interval.min << ", " << interval.max << ']';
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

namespace polynomial {

	Monomials from_roots(const std::vector<double>& roots)
	{
		Monomials result = { 1.0 };	//function result(x) = 1
		for (auto root : roots) {
			result = result * Monomials{ -root, 1.0 };	//new result(x) = (old result(x)) * (x - root)
		}
		return result;
	}

	Monomials operator*(const Monomials& p1, const Monomials& p2)
	{
		Monomials result(p1.degree() + p2.degree() + 1, 0.0);	//initialize result with 0.0

		for (int p1_idx = 0; p1_idx <= p1.degree(); p1_idx++) {
			for (int p2_idx = 0; p2_idx <= p2.degree(); p2_idx++) {
				result.coeffs[p1_idx + p2_idx] += p1.coeffs[p1_idx] * p2.coeffs[p2_idx];
			}
		}

		return result;
	}

	Monomials operator*(const Monomials& p, double factor)
	{
		Monomials result = p;
		for (auto& elem : result.coeffs) {
			elem *= factor;
		}
		return result;
	}

	Monomials operator+(const Monomials& p1, const Monomials& p2)
	{
		Monomials result = p1.coeffs.size() > p2.coeffs.size() ? p1 : p2;
		const Monomials& smaller = p1.coeffs.size() > p2.coeffs.size() ? p2 : p1;

		for (int i = 0; i <= smaller.degree(); i++) {
			result.coeffs[i] += smaller.coeffs[i];
		}
		return result;
	}

	Monomials& operator+=(Monomials& p1, const Monomials& p2)
	{
		if (p1.coeffs.size() >= p2.coeffs.size()) {
			for (int i = 0; i <= p2.degree(); i++) {
				p1.coeffs[i] += p2.coeffs[i];
			}
		}
		else {
			p1.coeffs.reserve(p2.coeffs.size());
			for (int i = 0; i <= p1.degree(); i++) {	//part where both polinomials have coefficients
				p1.coeffs[i] += p2.coeffs[i];
			}
			for (int i = p1.degree() + 1; i <= p2.degree(); i++) {	//copy last part of p2 into p1
				p1.coeffs.push_back(p2.coeffs[i]);
			}
		}
		return p1;
	}

	double evaluate(const Monomials& p, double x)
	{
		//uses horner sheme to get better numerical stability
		double result = p.coeffs.back();
		for (int i = p.degree() - 1; i >= 0; i--) {
			result *= x;
			result += p.coeffs[i];
		}
		return result;
	}

	double evaluate_derivative(const Monomials& p, double x)
	{
		//uses horner sheme to get better numerical stability
		//differs in tree ways from evaluate: only needs last (size - 1) coefficients, so last one is i == 1
		//  also the polynomial may only consist of a constant, which would make its derivative 0, so we can not start with result beeing back().
		//  also also the ith coefficient is multiplied by i, as one does in a derivative
		double result = 0.0;
		for (int i = p.degree(); i >= 1; i--) {
			result *= x;
			result += p.coeffs[i] * i;
		}
		return result;
	}

	Bernstein from_monomials(const Monomials& monomials, const Interval& interval)
	{
		return Bernstein();
	}

	Bernstein& operator+=(Bernstein& p1, const Bernstein& p2)
	{
		assert(p1.degree() == p2.degree());
		for (int i = 0; i <= p2.degree(); i++) {
			p1.coeffs[i] += p2.coeffs[i];
		}
		return p1;
	}

} //namespace polynomial

//////////////////////////////////////////////////////////////////////////////////////////////////////

namespace binomial {

	constexpr std::size_t max_static_binomial_length = 100;

	const std::vector<double>& static_coefficients(std::size_t n)
	{
		assert(n <= max_static_binomial_length);

		const auto build_polinomials = []() {

			const auto next_coefficients = [](const std::vector<double>& prev_coefficients) {
				std::vector<double> result(prev_coefficients.size() + 1, 1.0);	//initialize all with 1.0 
				for (std::size_t i = 1; i < prev_coefficients.size(); i++) {	//overwrite all but first and last
					result[i] = prev_coefficients[i - 1] + prev_coefficients[i];
				}
				return result;
			};

			std::vector<std::vector<double>> result;
			result.reserve(max_static_binomial_length + 1);
			result.emplace_back(std::vector<double>{ 1.0 });	//first to build the others from
			for (unsigned int i = 1; i <= max_static_binomial_length; i++) {
				result.emplace_back(next_coefficients(result[i - 1]));
			}
			return result;
		};

		static const std::vector<std::vector<double>> all_binomial_coefficients = build_polinomials();
		return all_binomial_coefficients[n];
	}
}

std::vector<double> binomial_coefficients(std::size_t n_size)
{
	using namespace binomial;
	const bool below_static_max = n_size <= max_static_binomial_length;

	std::vector<double> coefficients = below_static_max ?
		static_coefficients(n_size) :
		std::vector<double>(n_size + 1, 0.0);	//allocate n + 1 slots, all initialized to 0.0

	if (!below_static_max) {
		std::memcpy(coefficients.data(), 
			static_coefficients(max_static_binomial_length).data(), 
			(max_static_binomial_length + 1) * sizeof(double));	//insert precomputed steps

		//idea: iterate over array n_size times, each time one element further
		//compute all binomial coeffitients for current n from last computed n - 1 coefficients
		//after 1st iteration: 1  1  0  0  0  0  0 ... 0
		//after 2nd iteration: 1  2  1  0  0  0  0 ... 0
		//after 3rd iteration: 1  3  3  1  0  0  0 ... 0
		//after 4th iteration: 1  4  6  4  1  0  0 ... 0
		//after 5th iteration: 1  5 10 10  5  1  0 ... 0
		//(see pascal's triangle)
		//result (after nth iteration): 1  n  (n chose 2) (n chose 3) ... (n chose (n - 3)) (n chose (n - 2))  n  1
		//as the first max_static_binomial_length steps have been computed and inserted here already, n starts one afer that

		for (std::size_t n = max_static_binomial_length + 1; n <= n_size; n++) {
			double prev_n_chose_prev_k = 1.0;	// (n - 1) chose (k - 1) 
			for (std::size_t k = 1; k <= n; k++) {
				const double n_chose_k = prev_n_chose_prev_k + coefficients[k];
				prev_n_chose_prev_k = coefficients[k];
				coefficients[k] = n_chose_k;
			}
		}
	}
	return coefficients;
}

using namespace polynomial;

Monomials line_pow(Line line, std::size_t n)
{
	Monomials result(binomial_coefficients(n));

	for (std::size_t i = 0; i <= n; i++) {
		result.coeffs[i] *= std::pow(line[1], i) * std::pow(line[0], n - i);
	}
	return result;
}

std::size_t number_sign_changes(const Monomials& p)
{
	std::size_t sign_changes = 0;
	enum class Sign
	{
		positive, negative, unknown
	} last_sign = Sign::unknown;

	for (auto coefficient : p.coeffs) {
		switch (last_sign) {
		case Sign::unknown:
			if (coefficient < 0) { last_sign = Sign::negative; }
			if (coefficient > 0) { last_sign = Sign::positive; }
			break;

		case Sign::positive:
			if (coefficient < 0) {
				sign_changes++;
				last_sign = Sign::negative;
			}
			break;

		case Sign::negative:
			if (coefficient > 0) {
				sign_changes++;
				last_sign = Sign::positive;
			}
			break;
		}
	}
	return sign_changes;
}

std::size_t upper_bound_roots(const Monomials& p, Interval search_area)
{
	Monomials roots_of_search_area(p.coeffs.size(), 0.0);	//called B in VikramSharma

	for (int i = 0; i <= p.degree(); i++) {
		//ith_summands = (x+1)^(n-i) * (ax+b)^i * a[i] where a[i] denotes coefficient in front of ith monom in A (names from VikramSharma)
		const Monomials ith_summands = line_pow(Line{ 1, 1 }, p.degree() - i) * line_pow(Line{ search_area.min, search_area.max }, i) * (p.coeffs[i]);
		roots_of_search_area += ith_summands;
	}
	const auto roots_at_0 = std::distance(roots_of_search_area.coeffs.begin(), 
		std::find_if(roots_of_search_area.coeffs.begin(), roots_of_search_area.coeffs.end(), [](double c) {return c != 0.0; }));

	return roots_at_0 + number_sign_changes(roots_of_search_area);
}

std::vector<Interval> descartes_root_isolation(const Monomials& polynomial, const Interval& start_zone, bool(*accept)(const Monomials& p, const Interval& i))
{
	std::vector<Interval> root_intervals;
	root_intervals.reserve(polynomial.degree());

	std::vector<Interval> search_intervals;		//called Q in VikramSharma
	search_intervals.reserve(polynomial.degree());
	search_intervals.push_back(start_zone);

	while (search_intervals.size()) {
		const auto current_interval = search_intervals.back();
		search_intervals.pop_back();

		const auto roots_in_interval = upper_bound_roots(polynomial, current_interval);
		if (roots_in_interval == 0) {
			continue;	//throw away current_interval
		}
		else if (roots_in_interval == 1 || accept(polynomial, current_interval)) {
			root_intervals.push_back(current_interval);	//accepted as final interval
		}
		else if (roots_in_interval > 1) {	//split current_interval
			const double midpoint = (current_interval.min / 2) + (current_interval.max / 2);
			search_intervals.emplace_back(Interval{ current_interval.min, midpoint });
			search_intervals.emplace_back(Interval{ midpoint, current_interval.max });
		}
	}
	return root_intervals;
}

bool default_accept(const polynomial::Monomials& p, const Interval& i)
{
	const double midpoint = (i.min / 2) + (i.max / 2);
	return evaluate_derivative(p, midpoint) < 0.1 && i.width() < 0.1;
}
