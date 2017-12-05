#include <random>
#include <ctime>

std::random_device random;
std::mt19937 rng(random());


/// Returns a pseudorandom float in the given range.
const float floatInRange(const float lower_range, const float upper_range) {
	std::uniform_real_distribution<float> uni(lower_range, upper_range);
	//float r = lower_range + (float)rand() *(upper_range - lower_range) / ((float)RAND_MAX + 1.0f);
	return uni(rng);
}
	
/// Returns a pseudorandom negative or positive float in the range provided
const float floatInRangePositiveOrNegative(const float lower_range, const float upper_range) {
	std::uniform_real_distribution<float> uni(lower_range - upper_range, upper_range - lower_range);
	float r = uni(rng);
	if (r >= 0.0f) r += lower_range;
	else r -= lower_range;
	
	return r;
}

/// Returns a pseudorandom integer in the given range.
const int intInRange(const int lower_range, const int upper_range) {
	std::uniform_int_distribution<int> uni(lower_range, upper_range);
	//int r = lower_range + rand() *(upper_range - lower_range) / (RAND_MAX + 1);
	return uni(rng);
}

/// Returns a pseudorandom negative or positive float in the range provided
const int intInRangePositiveOrNegative(const int lower_range, const int upper_range) {
	std::uniform_int_distribution<int> uni(lower_range - upper_range, upper_range - lower_range);
	int r = uni(rng);
	//int r = -(upper_range - lower_range) + rand() *(2 * (upper_range - lower_range)) / (RAND_MAX + 1);
	if (r >= 0) r += lower_range;
	else r -= lower_range;
	return r;
}
