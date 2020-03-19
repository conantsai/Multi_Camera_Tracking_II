// Example program
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <functional>
#include <cmath>
#include <numeric>

using namespace std;

// Computes the distance between two std::vectors
template <typename T>
double	vectors_distance(const std::vector<T>& a, const std::vector<T>& b)
{
	vector<double>	auxiliary;

	transform (a.begin(), a.end(), b.begin(), back_inserter(auxiliary),//
	[](T element1, T element2) {return pow((element1-element2),2);});
	auxiliary.shrink_to_fit();

	return  sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
} // end template vectors_distance

int main()
{
vector<double> v1 = {0.1,0.3,0.4};
vector<double> v2 = {-0.1,-0.3,-0.4};

cout << vectors_distance(v1,v2) << "\n";
}