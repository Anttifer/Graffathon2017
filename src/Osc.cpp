#include "Osc.h"

#include <cmath>

namespace
{
void intfrac(scalar_t value, integer_t &int_part, scalar_t &frac_part) {
	int_part = integer_t(value);
	frac_part = value - scalar_t(int_part);
}

scalar_t lerp(scalar_t x0, scalar_t x1, scalar_t t) {
	return x0 + t * (x1 - x0);
}
}

Osc::Osc(size_t tableSize) :
	phase_accumulator_(0), phase_increment_(0), phaseOffset_(0),
	table_size_(tableSize), table(table_size_)
{
	scalar_t phaseStep = scalar_t(1) / scalar_t(table_size_);
	for (size_t i = 0; i < table_size_; ++i) {
		table[i] = sin(scalar_t(2) * M_PI * scalar_t(i) * phaseStep);
	}
}

void Osc::update() {
	phase_accumulator_ += phase_increment_;
	phase_accumulator_ -= scalar_t(integer_t(phase_accumulator_));
}

scalar_t Osc::sample() {
	scalar_t phase = phase_accumulator_ + phaseOffset_;
	phase -= scalar_t(integer_t(phase));
	integer_t int_part;
	scalar_t fract_part;
	intfrac(phase * table_size_, int_part, fract_part);
	scalar_t sample0 = table[int_part];
	if (size_t(++int_part) >= table_size_) {
		int_part = 0;
	}
	scalar_t sample1 = table[int_part];
	return lerp(sample0, sample1, fract_part);
}