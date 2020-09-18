#include <algorithm>
#include <charconv>
#include <cstring>
#include <deque>
#include <iostream>
#include <iterator>
#include <memory_resource>
#include <memory>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

auto modulo_inverse(int a, int n)
{
	int t = 0, nt = 1;
	int r = n, nr = a;

	while (nr != 0) {
		uint q = r / nr;
		uint m = r - q * nr;
		t = std::exchange(nt, t - q * nt);
		r = std::exchange(nr, r - q * nr);
	}

	return 
		r > 1 ? 0 :
		t < 0 ? t + n : t;
}

inline uint positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

struct Z_Mod
{
	uint v = 0;
	static uint base;

	constexpr Z_Mod() noexcept = default;
	constexpr Z_Mod(uint v) noexcept : v(v) {}

	constexpr Z_Mod(Z_Mod const&) noexcept = default;
	constexpr Z_Mod(Z_Mod &&) noexcept = default;

	auto operator=(Z_Mod const&) -> Z_Mod& = default;
	auto operator=(Z_Mod &&) -> Z_Mod& = default;
	
	auto operator+(Z_Mod v) const { return *this + v.v; }
	auto operator-(Z_Mod v) const { return *this - v.v; }
	auto operator*(Z_Mod v) const { return *this * v.v; }
	auto operator/(Z_Mod v) const { return *this / v.v; }
	
	auto operator+(uint b) const -> Z_Mod { return (v + b) % base; }
	auto operator-(uint b) const -> Z_Mod { return positive_modulo(v - b, base);  }
	auto operator*(uint b) const -> Z_Mod { return (v * b) % base; }
	auto operator/(uint b) const -> Z_Mod { return (modulo_inverse(b, base) * v) % base; }

	auto operator==(Z_Mod v) const -> bool { return *this == v.v; }
	auto operator!=(Z_Mod v) const -> bool { return (*this != v.v); }

	auto operator==(uint b) const -> bool { return v % base == b % base; }
	auto operator!=(uint b) const -> bool { return v % base != b % base; }
};

auto operator<<(std::ostream &os, Z_Mod const& v) -> decltype(auto)
{
	return os << (v.v % Z_Mod::base);
}

uint Z_Mod::base;

using Polynomial = std::deque<Z_Mod>;

auto simplify(Polynomial poly) -> Polynomial
{
	while (!poly.empty() && poly.front() == 0)
		poly.pop_front();
	return poly;
}

auto operator<<(std::ostream& os, Polynomial const& poly) -> decltype(auto)
{
	if (poly.size() == 0)
		return os << "[ 0 ]";
	os << "[ ";
	for (auto x : poly)
		os << x << ' ';
	return os << ']';
}

auto operator+(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
	Polynomial result;
	auto lhs_begin = std::crbegin(lhs);
	auto rhs_begin = std::crbegin(rhs);
	auto const lhs_end = std::crend(lhs);
	auto const rhs_end = std::crend(rhs);

	for (; lhs_begin != lhs_end && rhs_begin != rhs_end; ++lhs_begin, ++rhs_begin)
		result.push_front(*lhs_begin + *rhs_begin);

	for (; lhs_begin != lhs_end; ++lhs_begin)
		result.push_front(*lhs_begin);
	
	for (; rhs_begin != rhs_end; ++rhs_begin)
		result.push_front(*rhs_begin);

	return simplify(result);
}

auto operator-(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
	Polynomial result;
	auto lhs_begin = std::crbegin(lhs);
	auto rhs_begin = std::crbegin(rhs);
	auto const lhs_end = std::crend(lhs);
	auto const rhs_end = std::crend(rhs);

	for (; lhs_begin != lhs_end && rhs_begin != rhs_end; ++lhs_begin, ++rhs_begin)
		result.push_front(*lhs_begin - *rhs_begin);

	for (; lhs_begin != lhs_end; ++lhs_begin)
		result.push_front(*lhs_begin);
	
	for (; rhs_begin != rhs_end; ++rhs_begin)
		result.push_front(*rhs_begin);

	return simplify(result);
}

auto operator*(Polynomial const& a, Polynomial const& b)
{
	Polynomial result;
	result.resize(a.size() + b.size() - 1);

	for (uint i = 0; i < a.size(); ++i)
		for (uint j = 0; j < b.size(); ++j)
			result[i + j] = result[i + j] + a[i] * b[j];
	
	return simplify(result);
}

auto operator==(Polynomial const& poly, typename Polynomial::value_type value)
{
	switch (auto p = simplify(poly); p.size()) {
		case 0: return value == 0;
		case 1: return value == p.front();
		default: return false;
	}
}

auto operator!=(Polynomial const& poly, typename Polynomial::value_type value)
{
	return !(poly == value);
}

auto operator/(Polynomial const& n, Polynomial const& d) -> std::pair<Polynomial, Polynomial>
{
	auto q = Polynomial{ 0 };
	auto r = simplify(n);

	auto c = d.front();
	while (r != 0 && r.size() >= d.size()) {
		auto t = Polynomial { r.front() / c };
		for (uint i = 0; i < r.size() - d.size(); ++i)
			t.push_back(0);
		
		q = simplify(std::move(q) + t);
		r = simplify(r - d * t);
	}

	return { q, r };
}

auto main() -> int
{
	std::cout << "Base: ";
	(std::cin >> Z_Mod::base).get();

	Polynomial poly[2];
	for (size_t i = 0; i < 2; ++i) {
		std::cout << (i + 1) << ": ";
		std::string line;
		std::getline(std::cin, line);
		std::stringstream ss(line);
		std::copy(std::istream_iterator<uint>(ss), {}, std::back_inserter(poly[i]));
	}

	char c;
	std::cout << "Op (+, -, *, /): ";
	std::cin >> c;
	
	switch (c) {
		case '+': std::cout << poly[0] << " + " << poly[1] << " = " << (poly[0] + poly[1]) << '\n'; break;
		case '-': std::cout << poly[0] << " - " << poly[1] << " = " << (poly[0] - poly[1]) << '\n'; break;
		case '*': std::cout << poly[0] << " * " << poly[1] << " = " << (poly[0] * poly[1]) << '\n'; break;
		case '/': {
			auto const [q, r] = poly[0] / poly[1];
			std::cout << poly[0] << " = " << poly[1] << " * " << q << " + " << r << '\n';
		}
	}
}
