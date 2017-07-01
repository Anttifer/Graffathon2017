#include "Audio.h"

#include <cassert>

namespace
{

const size_t table_size = 2048;

}

Audio::Audio(double* time, double sample_rate,
             unsigned long frames_per_buffer, int num_channels) :
	left_osc_      (table_size),
	right_osc_     (table_size),
	envelope_      (scalar_t(sample_rate / 16.0), scalar_t(sample_rate / 4.0)),
	prev_beat_     (-1337),

	time_          (time),

	system_        (portaudio::System::instance()),
	output_params_ (system_.defaultOutputDevice(), num_channels,
	                portaudio::FLOAT32, false,
	                system_.defaultOutputDevice().defaultLowOutputLatency(),
	                nullptr),
	stream_params_ (portaudio::DirectionSpecificStreamParameters::null(),
	                output_params_, sample_rate, frames_per_buffer,
	                paClipOff),
	stream_        (stream_params_, *this, &Audio::generate)
{
	left_osc_.set_phase_increment(scalar_t(44) / scalar_t(sample_rate));
	right_osc_.set_phase_increment(scalar_t(66) / scalar_t(sample_rate));
}

int Audio::generate(const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo *timeInfo,
                    PaStreamCallbackFlags statusFlags)
{
	static double offset = timeInfo->outputBufferDacTime - timeInfo->currentTime;

	assert(outputBuffer != nullptr);
	(void)inputBuffer;
	(void)timeInfo;
	(void)statusFlags;

	float **out = static_cast<float **>(outputBuffer);

	for (unsigned int i = 0; i < framesPerBuffer; ++i) {
		double real_time = *time_ - offset + i/stream_params_.sampleRate();
		if (real_time - prev_beat_ >= 0.5) {
			envelope_.reset();
			envelope_.gate(true);
			prev_beat_ = real_time;
		}
		double mix = envelope_.process();
		out[0][i] = mix * left_osc_.sample();
		out[1][i] = mix * right_osc_.sample();

		left_osc_.update();
		right_osc_.update();
	}

	return paContinue;
}

void Audio::start()
{
	stream_.start();
}
