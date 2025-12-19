#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

const int NUM_STEPS = 10;

// Small piece of code moved to a function to keep the algorithm's code clean
void print(double val) {
	cout << fixed
             << setprecision( numeric_limits<double>::max_digits10) 
	     << "\t\t Trying... " << val << endl;
}

double square_root_by_binary_search(int target) {
	// Find the first value (left side of the interval) to start the iterations
	int first_value = 0;
	while (first_value * first_value < target) ++first_value;
	first_value -=1 ; // We want the left side of the interval

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

double square_root_by_newton(int target) {
	// Find the first value (right side of the interval) to start the iterations
        int first_value = 0;
        while (first_value * first_value < target) ++first_value;

	double result = first_value;
	print(result);
        for (int step = 0; step < NUM_STEPS; step++) {
		result = 0.5 * (result + target/result);
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
	double result = square_root_by_binary_search(target);
	cout << "Result: " << result << endl;

	result = square_root_by_newton(target);
        cout << "Result: " << result << endl;

	return 0;
}
