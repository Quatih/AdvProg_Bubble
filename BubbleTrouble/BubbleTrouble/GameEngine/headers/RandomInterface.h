#include <random>
#include <ctime>

std::random_device random;
std::mt19937 rng(random());

/// Returns a pseudorandom float in the given range.
const float randFloat(const float min, const float max) {
	std::uniform_real_distribution<float> uni(min, max);
	//float r = min + (float)rand() *(max - min) / ((float)RAND_MAX + 1.0f);
	return uni(rng);
}
	
/// Returns a pseudorandom negative or positive float in the range provided
const float randFloatPosNeg(const float min, const float max) {
	std::uniform_real_distribution<float> uni(-(max - min), max - min);
	float r = uni(rng);
	if (r >= 0.0f) r += min;
	else r -= min;
	
	return r;
}

/// Returns a pseudorandom double in the given range.
const double randDouble(const double min, const double max) {
	std::uniform_real_distribution<double> uni(min, max);
	//float r = min + (float)rand() *(max - min) / ((float)RAND_MAX + 1.0f);
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

/// Returns a pseudorandom negative or positive float in the range provided
const int randIntPosNeg(const int min, const int max) {
	std::uniform_int_distribution<int> uni(-(max - min), max - min);
	int r = uni(rng);
	//int r = -(max - min) + rand() *(2 * (max - min)) / (RAND_MAX + 1);
	if (r >= 0) r += min;
	else r -= min;
	return r;
}
