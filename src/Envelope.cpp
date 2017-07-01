#include "Envelope.h"

#include <cmath>

Envelope::Envelope() :
	Envelope(scalar_t(0), scalar_t(0), default_sustain_level, scalar_t(0))
{}

Envelope::Envelope(scalar_t a, scalar_t d) :
	Envelope(a, d, scalar_t(0), scalar_t(0))
{}

Envelope::Envelope(scalar_t a, scalar_t d, scalar_t s, scalar_t r) :
	Envelope(a, d, s, r, default_ratio_A, default_ratio_DR)
{}

Envelope::Envelope(scalar_t a, scalar_t d, scalar_t s, scalar_t r,
                   scalar_t ratio_a, scalar_t ratio_dr) :
	state_           (State::IDLE),
	output_          (scalar_t(0)),

	attack_rate_     (a),
	decay_rate_      (d),
	release_rate_    (r),

	attack_coef_     (calc_coef(attack_rate_, ratio_a)),
	decay_coef_      (calc_coef(decay_rate_, ratio_dr)),
	release_coef_    (calc_coef(release_rate_, ratio_dr)),

	sustain_level_   (s),

	target_ratio_A_  (ratio_a),
	target_ratio_DR_ (ratio_dr),

	attack_base_     ((scalar_t(1) + target_ratio_A_)
	                  * (scalar_t(1) - attack_coef_)),
	decay_base_      ((sustain_level_ - target_ratio_DR_)
	                  * (scalar_t(1) - decay_coef_)),
	release_base_    (-target_ratio_DR_ * (scalar_t(1) - release_coef_))
{}

scalar_t Envelope::process()
{
	switch (state_)
	{
		case State::IDLE:
			break;
		case State::ATTACK:
			output_ = attack_base_ + output_ * attack_coef_;
			if (output_ >= scalar_t(1))
			{
				output_ = scalar_t(1);
				state_ = State::DECAY;
			}
			break;
		case State::DECAY:
			output_ = decay_base_ + output_ * decay_coef_;
			if (output_ <= sustain_level_)
			{
				output_ = sustain_level_;
				state_ = State::SUSTAIN;
			}
			break;
		case State::SUSTAIN:
			break;
		case State::RELEASE:
			output_ = release_base_ + output_ * release_coef_;
			if (output_ <= scalar_t(0))
			{
				output_ = scalar_t(0);
				state_ = State::IDLE;
			}
	}
	return output_;
}

void Envelope::gate(bool trig)
{
	if (trig)
		state_ = State::ATTACK;
	else if (state_ != State::IDLE)
		state_ = State::RELEASE;
}

void Envelope::reset()
{
	state_ = State::IDLE;
	output_ = scalar_t(0);
}

void Envelope::set_attack_rate(scalar_t rate)
{
	attack_rate_ = rate;
	attack_coef_ = calc_coef(rate, target_ratio_A_);
	attack_base_ = (scalar_t(1) + target_ratio_A_)
	                * (scalar_t(1) - attack_coef_);
}

void Envelope::set_decay_rate(scalar_t rate)
{
	decay_rate_ = rate;
	decay_coef_ = calc_coef(rate, target_ratio_DR_);
	decay_base_ = (sustain_level_ - target_ratio_DR_)
	              * (scalar_t(1) - decay_coef_);
}

void Envelope::set_release_rate(scalar_t rate)
{
	release_rate_ = rate;
	release_coef_= calc_coef(rate, target_ratio_DR_);
	release_base_ = -target_ratio_DR_ * (scalar_t(1) - release_coef_);
}

scalar_t Envelope::calc_coef(scalar_t rate, scalar_t target_ratio)
{
	return (rate <= scalar_t(0)) ? scalar_t(0) :
		exp(-log((scalar_t(1) + target_ratio) / target_ratio) / rate);
}

void Envelope::set_sustain_level(scalar_t level)
{
	sustain_level_ = level;
	decay_base_ = (sustain_level_ - target_ratio_DR_)
	              * (scalar_t(1) - decay_coef_);
}

void Envelope::set_target_ratio_A(scalar_t ratio)
{
	if (ratio < scalar_t(1e-9))
		ratio = scalar_t(1e-9); // -180 dB
	target_ratio_A_ = ratio;
	attack_coef_ = calc_coef(attack_rate_, target_ratio_A_);
	attack_base_ = (scalar_t(1) + target_ratio_A_)
	                * (scalar_t(1) - attack_coef_);
}

void Envelope::set_target_ratio_DR(scalar_t ratio)
{
	if (ratio < scalar_t(1e-9))
		ratio = scalar_t(1e-9); // -180 dB
	target_ratio_DR_ = ratio;
	decay_coef_ = calc_coef(decay_rate_, target_ratio_DR_);
	release_coef_ = calc_coef(release_rate_, target_ratio_DR_);
	decay_base_ = (sustain_level_ - target_ratio_DR_)
	               * (scalar_t(1) - decay_coef_);
	release_base_ = -target_ratio_DR_ * (scalar_t(1) - release_coef_);
}