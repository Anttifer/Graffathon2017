#ifndef OSC_H__
#define OSC_H__

#include "AudioTypes.h"

#include <vector>

class Osc {
public:
	Osc(size_t tableSize);

	scalar_t current_phase() { return phase_accumulator_; }

	scalar_t phase_increment() { return phase_increment_; }
	void set_phase_increment(scalar_t inc) { phase_increment_ = inc; }

	scalar_t phase_offset() { return phaseOffset_; }
	void set_phase_offset(scalar_t offset) { phaseOffset_ = offset; }

	void update();

	scalar_t sample();

private:
	scalar_t phase_accumulator_;
	scalar_t phase_increment_;
	scalar_t phaseOffset_;
	size_t table_size_;
	std::vector<scalar_t> table;
};
#endif /* end of include guard: OSC_H__ */
