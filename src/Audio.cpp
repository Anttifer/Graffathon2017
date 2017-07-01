#include "Audio.h"

#include <cassert>

namespace
{

const size_t table_size = 2048;

}

Audio::Audio(double sample_rate, unsigned long frames_per_buffer, int num_channels) :
	left_osc_      (table_size),
	right_osc_     (table_size),

	system_        (portaudio::System::instance()),
	output_params_ (system_.defaultOutputDevice(), num_channels, portaudio::FLOAT32, false,
	                system_.defaultOutputDevice().defaultLowOutputLatency(), nullptr),
	stream_params_ (portaudio::DirectionSpecificStreamParameters::null(),
	                output_params_, sample_rate, frames_per_buffer, paClipOff),
	stream_        (stream_params_, *this, &Audio::generate)
{
	left_osc_.set_phase_increment(scalar_t(100) / scalar_t(sample_rate));
	right_osc_.set_phase_increment(scalar_t(150) / scalar_t(sample_rate));
	right_osc_.set_phase_offset(scalar_t(1) / scalar_t(3));
}

int Audio::generate(const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo *timeInfo,
                    PaStreamCallbackFlags statusFlags)
{
	assert(outputBuffer != nullptr);
	(void)inputBuffer;
	(void)timeInfo;
	(void)statusFlags;

	float **out = static_cast<float **>(outputBuffer);

	for (unsigned int i = 0; i < framesPerBuffer; ++i) {
		out[0][i] = .3f * left_osc_.sample();
		out[1][i] = .3f * right_osc_.sample();

		left_osc_.update();
		right_osc_.update();
	}

	return paContinue;
}

void Audio::start()
{
	stream_.start();
}
