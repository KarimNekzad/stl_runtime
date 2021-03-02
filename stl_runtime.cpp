// Karim Nekzad

#include <iostream>
// For portability using std::endl
#include <ostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
// Shuffle, Find
#include <algorithm>
// Random number generator
#include <random>
// Measuring runtime
#include <chrono>

/**
 * Specify operation for inserting and printing.
 */
enum class Operation {
	end, begin, find
};

/**
 * Calculates runtime as percentage relative to vector's runtime for a given
 * operation.
 *
 * @param v_time Vector's runtime in milliseconds.
 * @param stl_time stl container's runtime in milliseconds.
 * @return Runtime of an stl container as a percentage relative to the runtime
 * 		   of a vector for the same operation.
 */
std::chrono::milliseconds percent_relative(std::chrono::milliseconds v_time,
		std::chrono::milliseconds stl_time) {
	/* (100 * stl_time)  because these are integers and dividing loses
	 * decimal accuracy. Since we want a percentage from this division,
	 * this preserves two decimal places of accuracy and converts it into
	 * a percentage format, killing two birds with one stone.
	 *
	 * The cast to milliseconds is necessary, otherwise it gives errors */
	return std::chrono::milliseconds((100 * stl_time) / v_time);
}

/**
 * Format and print an stl container's runtime for inserting and finding
 * relative to a vector's runtime.
 *
 * @param v_time Vector's runtime in milliseconds.
 * @param stl_time stl container's runtime in milliseconds.
 * @param stlcontainer Name of the stl container being compared to a vector.
 * 					   && here means that we accept an rvalue, in this case,
 * 					   a string literal.
 * @param operation Type of operation, inserting or finding, to determine the
 * 				    format of the output message.
 * @param is_vector Prevent comparing a vector's runtime to itself
 * 					percentage-wise.
 */
void print_result(std::chrono::milliseconds v_time,
		std::chrono::milliseconds stl_time, const std::string&& stlcontainer,
		Operation operation, bool is_vector = false) {

	switch (operation) {
	case Operation::begin:
		std::cout << stlcontainer << ": Inserting at the beginning"
				<< std::flush;
		break;
	case Operation::end:
		std::cout << stlcontainer << ": Inserting at the end" << std::flush;
		break;
	case Operation::find:
		std::cout << stlcontainer << ": Finding 1~10,000" << std::flush;
		break;
	}

	// Must print regardless of operation
	std::cout << ", elapsed time: " << stl_time.count() << "ms. " << std::flush;

	// Don't want to compare vector's runtime to itself
	if (is_vector) {
		std::cout << std::endl;
		return;
	}

	std::cout << percent_relative(v_time, stl_time).count()
			<< "% relative to vector's time." << std::endl;
}

/**
 * Calculate the runtime of inserting at the beginning or end of an stl container
 *
 * @param input Vector that contains the shuffled data to be inserted into
 *              an stl container.
 * @param stlcontainer stl container being inserted into.
 * @return Runtime of inserting 100,000 randomized numbers from 1~100,000
 *         at the beginning or end of a given stl container specified by
 *         the `stlcontainer` param.
 */
template<class T>
std::chrono::milliseconds my_insert(const std::vector<int>& input,
		T& stlcontainer, Operation position) {
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<int>::const_iterator it = input.cbegin();
	std::vector<int>::const_iterator itend = input.cend();

	switch (position) {
	case Operation::begin:
		for (; it != itend; ++it) {
			stlcontainer.insert(stlcontainer.begin(), *it);
		}
		break;
	case Operation::end:
		for (; it != itend; ++it) {
			stlcontainer.insert(stlcontainer.end(), *it);
		}
		break;
	default:
		break;
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
			end - start);
	return elapsed;
}

/**
 * Calculate the runtime of finding 1~10,000 in ascending order in an
 * stl vector or list.
 *
 * @param stlcontainer stl container being searched.
 * @return Runtime of finding 1~10,000 in ascending order for a given
 * 		   stl container.
 */
template<class T>
std::chrono::milliseconds v_l_find(const T& stlcontainer) {
	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 1; i <= 10000; ++i) {
		std::find(stlcontainer.cbegin(), stlcontainer.cend(), i);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
			end - start);
	return elapsed;
}

/**
 * Calculate the runtime of finding 1~10,000 in ascending order in an
 * stl set or unordered set.
 *
 * @param stlcontainer stl container being searched.
 * @return Runtime of finding 1~10,000 in ascending order for a given
 * 		   stl container.
 */
template<class T>
std::chrono::milliseconds s_uos_find(const T& stlcontainer) {
	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 1; i <= 10000; ++i) {
		stlcontainer.find(i);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
			end - start);
	return elapsed;
}

int main() {
	// To hold 1~100,000 numbers in shuffled order
	std::vector<int> v;

	// Add integers from 1~100,000 in ascending order to vector
	for (int i = 1; i <= 100000; ++i) {
		v.push_back(i);
	}

	// Randomize ordering of values in vector
	std::default_random_engine rng;
	std::shuffle(v.begin(), v.end(), rng);

	// To keep track of each stl container's runtime
	std::chrono::milliseconds v_time;
	std::chrono::milliseconds s_time;
	std::chrono::milliseconds l_time;
	std::chrono::milliseconds uos_time;

	// Measure runtime of inserting at the end of stl containers

	std::vector<int> v2;
	v_time = my_insert(v, v2, Operation::end);
	print_result(v_time, v_time, "Vector", Operation::end, true);

	std::list<int> l;
	l_time = my_insert(v, l, Operation::end);
	print_result(v_time, l_time, "List", Operation::end);

	std::set<int> s;
	s_time = my_insert(v, s, Operation::end);
	print_result(v_time, s_time, "Set", Operation::end);

	std::unordered_set<int> uos;
	uos_time = my_insert(v, uos, Operation::end);
	print_result(v_time, uos_time, "Unordered Set", Operation::end);

	std::cout << std::endl;

	// Measure runtime of inserting at the beginning of stl containers

	v2.clear();
	v_time = my_insert(v, v2, Operation::begin);
	print_result(v_time, v_time, "Vector", Operation::begin, true);

	l.clear();
	l_time = my_insert(v, l, Operation::begin);
	print_result(v_time, l_time, "List", Operation::begin);

	s.clear();
	s_time = my_insert(v, s, Operation::begin);
	print_result(v_time, s_time, "Set", Operation::begin);

	uos.clear();
	uos_time = my_insert(v, uos, Operation::begin);
	print_result(v_time, uos_time, "Unordered Set", Operation::begin);

	std::cout << std::endl;

	// Measure runtime of finding 1~10,000 in ascending order in stl containers

	v_time = v_l_find(v2);
	print_result(v_time, v_time, "Vector", Operation::find, true);

	l_time = v_l_find(l);
	print_result(v_time, l_time, "List", Operation::find);

	s_time = s_uos_find(s);
	print_result(v_time, s_time, "Set", Operation::find);

	uos_time = s_uos_find(uos);
	print_result(v_time, uos_time, "Unordered Set", Operation::find);

	return 0;
}
