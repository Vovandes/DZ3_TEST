#include "custom_allocator.hpp"

const int vs::Factorial(int value) {
	int res = 1;
	for (int i = 2; i <= value; ++i) {
		res *= i;
	}
	return res;
}

void vs::PrintContainerMap(std::map<int, int, std::less<int>, vs::custom_allocator<std::pair<const int, int>, 10>>& dict) {
	for (const auto& node : dict) {
		std::cout << node.first << " " << node.second << std::endl;
	}
	std::cout << std::endl;
}
