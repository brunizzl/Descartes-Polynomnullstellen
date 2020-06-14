#pragma once

#include <vector>
#include <array>
#include <iostream>
#include <cassert>
#include <complex>


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
		:public  std::vector<double>
	{
		Polynomial(std::initializer_list<double> coeffs) :std::vector<double>(coeffs) {
			assert(coeffs.size() > 0);
		}

		Polynomial(std::size_t size, double value) :std::vector<double>(size, value) {
			assert(size > 0);
		}

		Polynomial(std::vector<double>&& coeffs) :std::vector<double>(std::move(coeffs)) {
			assert(this->size() > 0);
		}

		int degree() const { return this->size() - 1; }
	};

	using Monomials = Polynomial<Base_Type::monomial>;

	//same as polynomial::Monomials, only allocated on the stack (and thus with size known at compile time)
	template <std::size_t n>
	using Static_Monomials = std::array<double, n + 1>;

	using Line = Static_Monomials<1>;

	Monomials from_roots(const std::vector<double>& roots);

	//each element in pair_representatives gets mirrored on the real axis and both values are roots of the result
	Monomials from_complex_root_pairs(const std::vector<std::complex<double>>& pair_representatives);

	Monomials operator*(const Monomials& p1, const Monomials& p2);

	Monomials operator*(const Monomials& p, double factor);

	//result of function from_divison
	struct Division
	{
		Monomials quotient;
		Monomials rest;
	};

	Division operator/(const Monomials& numerator, const Monomials& denominator);

	Monomials& operator%=(Monomials& a, const Monomials& m);

	Monomials operator+(const Monomials& p1, const Monomials& p2);

	Monomials& operator+=(Monomials& p1, const Monomials& p2);

	double evaluate(const Monomials& polinomial, double x);

	Monomials derive(const Monomials& p);

	Monomials greatest_common_denominator(const Monomials& p1, const Monomials& p2, double allowed_err = 0.001);

	//divide p by its highest coefficient
	void normalize(Monomials& p);

	using Bernstein = Polynomial<Base_Type::bernstein>;

	// the usual Bernstein base consists of elements B_k^n(x) = chose(n, k) * x^k * (1-x)^(n-k)
	// the full polynomial therefore is \sum_{k=0}^n b_k * B_k^n(x)
	// as noted here https://www.cise.ufl.edu/research/SurfLab/seminar/algebraicmanipulation.pdf
	// where b_k are the coefficients stored in the vector

	Bernstein from_monomials(const Monomials& monomials, const Interval& interval);

	double evaluate(const Bernstein& polinomial, double x);

	Bernstein& operator+=(Bernstein& p1, const Bernstein& p2);
}





//returns n-th row of pascals triangle
std::vector<double> binomial_coefficients(std::size_t n_size);

//input l(x) = a*x+b and power n return p(x) = (a*x+b)^n
polynomial::Monomials line_pow(polynomial::Line line, std::size_t n);

//returns how many roots of polinomial are at most in search_area
std::size_t upper_bound_roots(const polynomial::Monomials& polinomial, Interval search_area);

polynomial::Monomials no_root_multiplicities(const polynomial::Monomials& p, double allowed_err = 0.001);

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