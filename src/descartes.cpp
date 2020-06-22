#include "descartes.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <cstdio>

std::ostream& operator<<(std::ostream& stream, Interval interval)
{
	return stream << '[' << interval.min << ", " << interval.max << ']';
}

bool nonzero(const double& x) { return x != 0.0; };

///////////////////////////////////////////////////////////////////////////////////////////////////////

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

	double choose(std::size_t n, std::size_t k)
	{
		assert(n >= k);
		assert(n <= max_static_binomial_length);
		return static_coefficients(n)[k];
	}
}

namespace polynomial {

	Monomials from_roots(const std::vector<double>& roots)
	{
		Monomials result = { 1.0 };	//function result(x) = 1
		for (const auto root : roots) {
			result = result * Monomials{ -root, 1.0 };	//new result(x) = (old result(x)) * (x - root)
		}
		return result;
	}

	Monomials from_complex_root_pairs(const std::vector<std::complex<double>>& pair_representatives)
	{
		Monomials result = { 1.0 };
		for (const auto root : pair_representatives) {
			//root = a+ib
			//second root from this elem is a-ib
			//these roots build parabola with f(x) = (x-a-ib)(x-a+ib) = (x-a)^2 -(ib)^2 = x^2 -2ax + a^2 + b^2
			const double a = root.real();
			const double b = root.imag();
			result = result * Monomials{ a * a + b * b, -2.0 * a, 1.0 };
		}
		return result;
	}

	Monomials operator*(const Monomials& p1, const Monomials& p2)
	{
		Monomials result(p1.degree() + p2.degree() + 1, 0.0);	//initialize result with 0.0

		for (int p1_idx = 0; p1_idx <= p1.degree(); p1_idx++) {
			for (int p2_idx = 0; p2_idx <= p2.degree(); p2_idx++) {
				result[p1_idx + p2_idx] += p1[p1_idx] * p2[p2_idx];
			}
		}

		return result;
	}

	Monomials operator*(const Monomials& p, double factor)
	{
		Monomials result = p;
		for (auto& elem : result) {
			elem *= factor;
		}
		return result;
	}

	//copied from here https://de.wikipedia.org/wiki/Polynomdivision#Algorithmus (k is called j there)
	Division operator/(const Monomials& numerator, const Monomials& denominator)
	{
		const int real_denominator_degree = std::distance(
			std::find_if(denominator.rbegin(), denominator.rend(), nonzero), 
			denominator.rend()
		) - 1;
		if (real_denominator_degree <= 0) {
			throw std::exception("dividing by 0");
		}
		
		Monomials quotient = Monomials(numerator.degree() - real_denominator_degree + 1, 0.0);
		Monomials rest = Monomials(numerator);
		while (rest.back() == 0.0) {	//guarantee to have highest coefficient in rest unequal to zero
			rest.pop_back();
		}

		for (int i = rest.degree() - real_denominator_degree; i >= 0; i--) {
			quotient[i] = rest[i + real_denominator_degree] / denominator[real_denominator_degree];

			rest.pop_back();
			for (int k = real_denominator_degree - 1; k >= 0; k--) {
				rest[i + k] -= denominator[k] * quotient[i];
			}
		}

		return Division{ quotient, rest };
	}

	//dumbed down version of operator/
	Monomials& operator%=(Monomials& a, const Monomials& m)
	{
		const int real_m_degree = std::distance(
			std::find_if(m.rbegin(), m.rend(), nonzero), 
			m.rend()
		) - 1;
		while (a.back() == 0.0) {	//guarantee to have highest coefficient in rest unequal to zero
			a.pop_back();
		}

		for (int i = a.degree() - real_m_degree; i >= 0; i--) {
			const double quotient_coeff = a[i + real_m_degree] / m[real_m_degree];

			a.pop_back();
			for (int k = real_m_degree - 1; k >= 0; k--) {
				a[i + k] -= m[k] * quotient_coeff;
			}
		}
		return a;
	}

	Monomials operator+(const Monomials& p1, const Monomials& p2)
	{
		Monomials result = p1.size() > p2.size() ? p1 : p2;
		const Monomials& smaller = p1.size() > p2.size() ? p2 : p1;

		for (int i = 0; i <= smaller.degree(); i++) {
			result[i] += smaller[i];
		}
		return result;
	}

	Monomials& operator+=(Monomials& p1, const Monomials& p2)
	{
		if (p1.size() >= p2.size()) {
			for (int i = 0; i <= p2.degree(); i++) {
				p1[i] += p2[i];
			}
		}
		else {
			p1.reserve(p2.size());
			for (int i = 0; i <= p1.degree(); i++) {	//part where both polinomials have coefficients
				p1[i] += p2[i];
			}
			for (int i = p1.degree() + 1; i <= p2.degree(); i++) {	//copy last part of p2 into p1
				p1.push_back(p2[i]);
			}
		}
		return p1;
	}

	double evaluate(const Monomials& p, double x)
	{
		//uses horner sheme to get better numerical stability
		double result = p.back();
		for (int i = p.degree() - 1; i >= 0; i--) {
			result *= x;
			result += p[i];
		}
		return result;
	}

	Monomials derive(const Monomials& p)
	{
		Monomials derived(p.size() - 1, 0.0);
		for (int i = 1; i <= p.degree(); i++) {
			derived[i - 1] = p[i] * i;
		}
		return derived;
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
			result += p[i] * i;
		}
		return result;
	}

	//euclids algorithm found here: https://en.wikipedia.org/wiki/Polynomial_greatest_common_divisor#Euclid's_algorithm
	Monomials greatest_common_denominator(const Monomials& a_in, const Monomials& b_in, double allowed_err)
	{
		const auto absolute_biggest_coeff = [](const Monomials& p) -> double {
			if (p.size()) {
				const auto [min, max] = std::minmax_element(p.begin(), p.end());
				return std::max(std::abs(*min), *max);
			}
			else {
				return 0.0;
			}
		};

		Monomials a = a_in;
		Monomials b = b_in;

		while (absolute_biggest_coeff(b) > allowed_err) {	//while b is not ca. 0.0
			a %= b;
			std::swap(a, b);
		}
		return a;
	}

	void normalize(Monomials& p)
	{
		const int real_p_degree = std::distance(
			std::find_if(p.rbegin(), p.rend(), nonzero), 
			p.rend()
		) - 1;

		const double divisor = p[real_p_degree];
		for (int i = 0; i <= real_p_degree; i++) {
			p[i] /= divisor;
		}
	}



	Bernstein from_monomials(const Monomials& monomials, const Interval& interval)
	{
		Bernstein result(binomial_coefficients(monomials.size()), interval);
		//TODO
		return result;
	}

	double evaluate(const Bernstein& polynomial, double x)
	{
		const double a = polynomial.interval.min;
		const double b = polynomial.interval.max;
		const int n = polynomial.degree();

		double result = 0.0;
		for (int k = 0; k <= polynomial.degree(); k++) {
			result += polynomial[k] * binomial::choose(n, k) * std::pow(x - a, k) * std::pow(b - x, n - k);
		}
		return result / std::pow(b - a, n);
	}

	Bernstein& operator+=(Bernstein& p1, const Bernstein& p2)
	{
		assert(p1.degree() == p2.degree());
		for (int i = 0; i <= p2.degree(); i++) {
			p1[i] += p2[i];
		}
		return p1;
	}

} //namespace polynomial

//////////////////////////////////////////////////////////////////////////////////////////////////////

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
		result[i] *= std::pow(line[1], i) * std::pow(line[0], n - i);
	}
	return result;
}

std::size_t number_sign_changes(const std::vector<double> & p)
{
	std::size_t sign_changes = 0;
	enum class Sign
	{
		positive, negative, unknown
	} last_sign = Sign::unknown;

	for (auto coefficient : p) {
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
	Monomials roots_of_search_area(p.size(), 0.0);	//called B in VikramSharma

	for (int i = 0; i <= p.degree(); i++) {
		//ith_summands = (x+1)^(n-i) * (ax+b)^i * a[i] where a[i] denotes coefficient in front of ith monom in A (names from VikramSharma)
		const Monomials ith_summands = line_pow(Line{ 1, 1 }, p.degree() - i) * line_pow(Line{ search_area.min, search_area.max }, i) * (p[i]);
		roots_of_search_area += ith_summands;
	}
	const auto roots_at_0 = std::distance(roots_of_search_area.begin(), 
		std::find_if(roots_of_search_area.begin(), roots_of_search_area.end(), nonzero));

	return roots_at_0 + number_sign_changes(roots_of_search_area);
}

std::vector<Interval> descartes_root_isolation(const Monomials& polynomial, const Interval& start_zone, 
	bool(*accept)(const Monomials& p, const Interval& i))
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

std::pair<polynomial::Bernstein, polynomial::Bernstein> de_casteljau_split(const polynomial::Bernstein& polynomial)
{
	const int n = polynomial.degree();
	std::vector<std::vector<double>> coeffs;
	coeffs.reserve(n + 1);
	coeffs.push_back(polynomial);
	for (int i = 1; i <= n; i++) {
		coeffs.emplace_back(std::vector<double>(n + 1, 0.0));
	}

	for (int i = 1; i <= n; i++) {
		for (int j = 0; j <= n - i; j++) {
			coeffs[i][j] = 0.5 * coeffs[i - 1][j] + 0.5 * coeffs[i - 1][j + 1];
		}
	}

	const double a = polynomial.interval.min;
	const double b = polynomial.interval.max;
	const double m = polynomial.interval.min + polynomial.interval.width() / 2;
	const Interval fst_half_interval = { a, m };
	const Interval snd_half_interval = { m, b };

	polynomial::Bernstein fst_half(n + 1, 0.0, fst_half_interval);
	polynomial::Bernstein snd_half(n + 1, 0.0, snd_half_interval);

	for (int i = 0; i <= n; i++) {
		fst_half[i] = coeffs[i][0];
		snd_half[i] = coeffs[n - i][i];
	}

	return std::make_pair(std::move(fst_half), std::move(snd_half));
}

std::vector<Interval> descartes_root_isolation(const polynomial::Bernstein& polynomial)
{
	std::vector<Interval> root_intervals;
	root_intervals.reserve(polynomial.degree());

	std::vector<polynomial::Bernstein> search_objects;
	search_objects.reserve(polynomial.degree());
	search_objects.push_back(polynomial);

	while (search_objects.size()) {
		const auto current = std::move(search_objects.back());
		search_objects.pop_back();

		const auto sign_variations = number_sign_changes(current);
		if (sign_variations == 0) {
			continue;
		}
		else if (sign_variations == 1) {
			root_intervals.push_back(current.interval);
		}
		else if (sign_variations > 1 && current.interval.width() > 0.000001) {
			auto [b1, b2] = de_casteljau_split(current);
			search_objects.push_back(std::move(b1));
			search_objects.push_back(std::move(b2));
		}
	}
	return root_intervals;
}

bool default_accept(const polynomial::Monomials& p, const Interval& i)
{
	const double midpoint = (i.min / 2) + (i.max / 2);
	return evaluate_derivative(p, midpoint) < 0.001 && i.width() < 0.001;
}

polynomial::Monomials no_root_multiplicities(const polynomial::Monomials& p, double allowed_err)
{
	Division div = p / greatest_common_denominator(p, derive(p), allowed_err);

	if (div.rest.size()) {
		const auto [min, max] = std::minmax_element(div.rest.begin(), div.rest.end());
		if (std::max(std::abs(*min), *max) >= allowed_err) {
			throw std::exception("uuh oh. this is not good indeed.");
		}
	}
	normalize(div.quotient);
	return div.quotient;
}
