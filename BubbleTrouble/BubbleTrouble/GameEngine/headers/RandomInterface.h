#include <random>
#include <ctime>

std::random_device random_dev;
std::mt19937 rng(random_dev());

/// Returns a pseudorandom double in the given range.
float randFloat(const float min, const float max) {
	std::uniform_real_distribution<float> uni(min, max);
	return uni(rng);
}
	
/// Returns a pseudorandom negative or positive double in the range provided
float randFloatePosNeg(const float min, const float max) {
	std::uniform_real_distribution<float> uni(-(max - min), max - min);
	float r = uni(rng);
	if (r >= 0.0f) r += min;
	else r -= min;
	
	return r;
}

/// Returns a pseudorandom double in the given range.
double randDouble(const double min, const double max) {
	std::uniform_real_distribution<double> uni(min, max);
	return uni(rng);
}

/// Returns a pseudorandom negative or positive double in the range provided
double randDoublePosNeg(const double min, const double max) {
	std::uniform_real_distribution<double> uni(-(max - min), max - min);
	double r = uni(rng);
	if (r >= 0.0) r += min;
	else r -= min;

	return r;
}

/// Returns a pseudorandom integer in the given range.
int randInt(const int min, const int max) {
	std::uniform_int_distribution<int> uni(min, max);
	return uni(rng);
}

/// Returns a pseudorandom integer in the given range.
template <typename T> const T randInt(const T min, const T max) {
	std::uniform_int_distribution<T> uni(min, max);
	return uni(rng);
}

/// Returns a pseudorandom negative or positive double in the range provided
int randIntPosNeg(const int min, const int max) {
	std::uniform_int_distribution<int> uni(-(max - min), max - min);
	int r = uni(rng);
	if (r >= 0) r += min;
	else r -= min;
	return r;
}

int randMinusPlus() {
	int r = 0;
	while (r == 0) {
		r = randIntPosNeg(0, 1);
	}
	return r;
}

