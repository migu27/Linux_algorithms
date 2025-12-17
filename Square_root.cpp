#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

const int NUM_STEPS = 50;

// Small piece of code moved to a function to keep the algorithm's code clean
void print(double val) {
	cout << fixed
             << setprecision( numeric_limits<double>::max_digits10) 
	     << "\t\t Trying... " << val << endl;
}

// Find the first value (left side of the interval) to start the iterations
int find_first_value(int target) {
	int result, tmp = 0;
	while (result * result < target) ++result;
	return (result-1);
}

double square_root(int first_value, int target) {
	// Build the interval, defined by a left and a right value
	double left = (double)first_value;
	double right = left + 1.0;
	double result = left; // We always start with the left value, as per the 
			      // implementation of function find_first_value() which
			      // returns the greatest integer x such that x*x < target

	for (int step = 0; step < NUM_STEPS; step++) {
		double middle = left + ((right - left) / 2);
		if (middle * middle < (double)target) {
			left = middle;
			result = right; // As we DO know left is too small
		}
		else {
			right = middle;
			result = left; // As we DO know right is too big
		}
		print(result);
	}

	return result;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Usage: ./square_root <natural_number>" << endl;
		return -1;
	}
	int target = stoi(argv[1]);
	cout << "Square root of " << target << ":" << endl;
	int first_value = find_first_value(target);
	double result = square_root(first_value, target);
	cout << "Result: " << result << endl;

	return 0;
}
