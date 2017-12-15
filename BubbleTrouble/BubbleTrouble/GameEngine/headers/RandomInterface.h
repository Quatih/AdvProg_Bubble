#include <random>
#include <ctime>

std::random_device random_dev;
std::mt19937 rng(random_dev());

/// Returns a pseudorandom double in the given range.
const double randdouble(const double min, const double max) {
	std::uniform_real_distribution<double> uni(min, max);
	//double r = min + (double)rand() *(max - min) / ((double)RAND_MAX + 1.0f);
	return uni(rng);
}
	
/// Returns a pseudorandom negative or positive double in the range provided
const double randdoublePosNeg(const double min, const double max) {
	std::uniform_real_distribution<double> uni(-(max - min), max - min);
	double r = uni(rng);
	if (r >= 0.0f) r += min;
	else r -= min;
	
	return r;
}

/// Returns a pseudorandom double in the given range.
const double randDouble(const double min, const double max) {
	std::uniform_real_distribution<double> uni(min, max);
	//double r = min + (double)rand() *(max - min) / ((double)RAND_MAX + 1.0f);
	return uni(rng);
}

/// Returns a pseudorandom negative or positive double in the range provided
const double randDoublePosNeg(const double min, const double max) {
	std::uniform_real_distribution<double> uni(-(max - min), max - min);
	double r = uni(rng);
	if (r >= 0.0f) r += min;
	else r -= min;

	return r;
}

/// Returns a pseudorandom integer in the given range.
const int randInt(const int min, const int max) {
	std::uniform_int_distribution<int> uni(min, max);
	//int r = min + rand() *(max - min) / (RAND_MAX + 1);
	return uni(rng);
}

/// Returns a pseudorandom integer in the given range.
template <typename T> const T randInt(const T min, const T max) {
	std::uniform_int_distribution<T> uni(min, max);
	//int r = min + rand() *(max - min) / (RAND_MAX + 1);
	return uni(rng);
}

/// Returns a pseudorandom negative or positive double in the range provided
const int randIntPosNeg(const int min, const int max) {
	std::uniform_int_distribution<int> uni(-(max - min), max - min);
	int r = uni(rng);
	//int r = -(max - min) + rand() *(2 * (max - min)) / (RAND_MAX + 1);
	if (r >= 0) r += min;
	else r -= min;
	return r;
}

const int randMinusPlus() {
	int r = 0;
	while (r == 0) {
		r = randIntPosNeg(0, 1);
	}
	return r;
}

