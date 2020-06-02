#pragma once

#include <vector>
#include <array>
#include <iostream>
#include <cassert>


struct Interval
{
	double min, max;

	constexpr double width() const { return std::max(0.0, this->max - this->min); }
};

std::ostream& operator<<(std::ostream& stream, Interval interval);



namespace polynomial {

	enum Base_Type
	{
		monomial,
		bernstein
	};

	template <Base_Type base>
	struct Polynomial
	{
		std::vector<double> coeffs;

		Polynomial(std::initializer_list<double> coeffs_) :coeffs(coeffs_) {
			assert(coeffs_.size() > 0);
		}

		Polynomial(std::size_t size, double value) :coeffs(size, value) {
			assert(size > 0);
		}

		Polynomial(const std::vector<double>&& coeffs_) :coeffs(std::move(coeffs_)) {}

		int degree() const { return coeffs.size() - 1; }
	};

	using Monomials = Polynomial<Base_Type::monomial>;

	//same as polynomial::Monomials, only allocated on the stack (and thus with size known at compile time)
	template <std::size_t n>
	using Static_Monomials = std::array<double, n + 1>;

	using Line = Static_Monomials<1>;

	Monomials from_roots(const std::vector<double>& roots);

	Monomials operator*(const Monomials& p1, const Monomials& p2);

	Monomials operator*(const Monomials& p, double factor);

	Monomials operator+(const Monomials& p1, const Monomials& p2);

	Monomials& operator+=(Monomials& p1, const Monomials& p2);

	double evaluate(const Monomials& polinomial, double x);

	double evaluate_derivative(const Monomials& polinomial, double x);


	using Bernstein = Polynomial<Base_Type::bernstein>;

	Bernstein from_monomials(const Monomials& monomials, const Interval& interval);

	Bernstein& operator+=(Bernstein& p1, const Bernstein& p2);
}





//returns n-th row of pascals triangle
std::vector<double> binomial_coefficients(std::size_t n_size);

//input l(x) = a*x+b and power n return p(x) = (a*x+b)^n
polynomial::Monomials line_pow(polynomial::Line line, std::size_t n);

//returns how many roots of polinomial are at most in search_area
std::size_t upper_bound_roots(const polynomial::Monomials& polinomial, Interval search_area);

//default parameter in descartes_root_isolation
bool default_accept(const polynomial::Monomials& p, const Interval& i);

//returns intervals with exactly one root in each (will not terminate if polynomial has roots with multiplicity > 1)
//polinomial is called A in VikramSharma, start_zone is called I_0
//parameter accept decides, if an interval should be accepted as final, despite still having multiple roots.
std::vector<Interval> descartes_root_isolation(const polynomial::Monomials& polinomial, const Interval& start_zone, 
	bool(*accept)(const polynomial::Monomials& p, const Interval& i) = default_accept);






template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec)
{
	stream << "{ ";
	if (vec.size() >= 1) {
		stream << vec[0];
		for (std::size_t i = 1; i < vec.size(); i++) {
			stream << ", " << vec[i];
		}
	}
	return stream << " }";
}