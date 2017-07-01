#ifndef AUDIO_H__
#define AUDIO_H__

#include "Osc.h"
#include "Envelope.h"

#include <portaudiocpp/PortAudioCpp.hxx>

class Audio
{
public:
	Audio (double* time, double sample_rate,
	       unsigned long frames_per_buffer, int num_channels = 2);

	void start();
private:
	int generate(const void *inputBuffer, void *outputBuffer,
	             unsigned long framesPerBuffer,
	             const PaStreamCallbackTimeInfo *timeInfo,
	             PaStreamCallbackFlags statusFlags);

	Osc left_osc_;
	Osc right_osc_;
	Envelope envelope_;
	double prev_beat_;

	double* time_;

	portaudio::AutoSystem raii_;
	portaudio::System &system_;
	portaudio::DirectionSpecificStreamParameters output_params_;
	portaudio::StreamParameters stream_params_;
	portaudio::MemFunCallbackStream<Audio> stream_;
};
#endif /* end of include guard: AUDIO_H__ */
