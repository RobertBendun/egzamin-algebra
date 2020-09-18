#include <algorithm>
#include <charconv>
#include <cstring>
#include <iostream>
#include <memory_resource>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

struct Argument_Scanner
{
	char const *const *argv;

	template<typename T>
	auto scan() -> std::enable_if_t<std::is_arithmetic_v<T>, T>
	{
		T value;
		std::from_chars(*argv, *argv + std::strlen(*argv), value);
		argv += 1;
		return value;
	}
};

auto modulo_inverse(int a, int n)
{
	int t = 0, nt = 1;
	int r = n, nr = a;

	while (nr != 0) {
		uint q = r / nr;
		uint m = r - q * nr;
		if (m != 0) {
			std::cout << '\t' << r << " = " << nr;
			if (q != 1) 
				std::cout << " * " << q;
			std::cout << " + " << m << '\n';			
		}
		t = std::exchange(nt, t - q * nt);
		r = std::exchange(nr, r - q * nr);
	}
	std::cout << '\n';

	return 
		r > 1 ? 0 :
		t < 0 ? t + n : t;
}

auto main(int argc, char **argv) -> int
{
	auto scanner = Argument_Scanner{argv + 1};
	auto n1 = scanner.scan<uint>();
	auto n2 = scanner.scan<uint>();

	std::tie(n1, n2) = std::minmax({n1, n2});

	if (auto r = modulo_inverse(n1, n2); r != 0)
		std::cout << n1 << "^-1 = " << r << " (mod " << n2 << ")\n";
	else
		std::cout << n1 << " is not invertible in Z mod " << n2 << '\n';
}
