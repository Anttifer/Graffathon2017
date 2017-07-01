#ifndef ENVELOPE_H__
#define ENVELOPE_H__

#include "AudioTypes.h"

class Envelope
{
public:
	enum class State { IDLE = 0, ATTACK, DECAY, SUSTAIN, RELEASE };

	static constexpr scalar_t default_sustain_level = scalar_t(1);
	static constexpr scalar_t default_ratio_A = scalar_t(3e-1);
	static constexpr scalar_t default_ratio_DR = scalar_t(1e-4);

	static scalar_t calc_coef(scalar_t rate, scalar_t target_ratio);

	Envelope();
	Envelope(scalar_t a, scalar_t d);
	Envelope(scalar_t a, scalar_t d, scalar_t s, scalar_t r);
	Envelope(scalar_t a, scalar_t d, scalar_t s, scalar_t r,
	         scalar_t ratio_a, scalar_t ratio_dr);

	scalar_t process();
	void gate(bool trig);
	void reset();

	scalar_t output() const { return output_; }
	State state() const { return state_; }

	scalar_t attack_rate() const { return attack_rate_; }
	void set_attack_rate(scalar_t rate);

	scalar_t decay_rate() const { return decay_rate_; }
	void set_decay_rate(scalar_t rate);

	scalar_t release_rate() const { return release_rate_; }
	void set_release_rate(scalar_t rate);

	scalar_t sustain_level() const { return sustain_level_; }
	void set_sustain_level(scalar_t level);

	scalar_t target_ratio_A() const { return target_ratio_A_; }
	void set_target_ratio_A(scalar_t ratio);

	scalar_t target_ratio_DR() const { return target_ratio_DR_; }
	void set_target_ratio_DR(scalar_t ratio);

private:
	State state_;
	scalar_t output_;

	scalar_t attack_rate_;
	scalar_t decay_rate_;
	scalar_t release_rate_;

	scalar_t attack_coef_;
	scalar_t decay_coef_;
	scalar_t release_coef_;

	scalar_t sustain_level_;

	scalar_t target_ratio_A_;
	scalar_t target_ratio_DR_;

	scalar_t attack_base_;
	scalar_t decay_base_;
	scalar_t release_base_;
};

#endif /* end of include guard: ENVELOPE_H__ */