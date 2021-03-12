#include "OOSDL2.h"

NAMESPACE_SDL2_BEGIN


AudioOutput::AudioOutput(Context* ctx) : Component(ctx) {
	sys_snd.Set(&snd_buf);
	
	SDL_zero(audio_fmt);
	SetDesiredAudioFmt(
		48000,
		4, true, // 32bit float
		2,
		1024
	);
}

int AudioOutput::GetSampleSize() {
	int sample_size = 0;
	switch(audio_fmt.format) {
		case AUDIO_S8:  sample_size = 1; break;
		case AUDIO_S16: sample_size = 2; break;
		case AUDIO_S32: sample_size = 4; break;
		case AUDIO_F32: sample_size = 4; break;
		default: break;
	}
	return sample_size;
}

bool AudioOutput::IsSampleFloating() {
	return audio_fmt.format == AUDIO_F32;
}

void AudioOutput::Put(Uint8* stream, int len) {
	int sample_size = GetSampleSize();
	
	if (len % sample_size != 0) {
		LOG("OOSDL2::AudioOutput::Put: error: invalid sample size in read length");
		return;
	}
	
	int channels = snd_buf.GetChannels();
	int read_total_samples = len / sample_size;
	if (read_total_samples % channels != 0) {
		LOG("OOSDL2::AudioOutput::Put: error: invalid channel size in read length");
		return;
	}
	int read_ch_samples = read_total_samples / channels;
	
	int sample_rate = snd_buf.GetSampleRate();
	if (read_ch_samples % sample_rate != 0) {
		LOG("OOSDL2::AudioOutput::Put: error: invalid sample rate in read length");
		return;
	}
	int read_frames = read_ch_samples / sample_rate;
	
	int queue_samples = snd_buf.GetQueueSize(); // per 1 channel (not total samples)
	int queue_frames = queue_samples / sample_rate;
	ASSERT(queue_samples % sample_rate == 0);
	
	snd_buf.Get(stream, len);
	
	/*static int counter;
	
	if (counter++ == 1000) {
		float* it = (float*)stream;
		for(int i = 0; i < read_total_samples; i++) {
			float f = it[i];
			LOG(i << ": " << DblStr(f));
		}
		LOG("");
		TODO
	}*/
	
	/*float* it = (float*)stream;
	float* end = it + read_total_samples;
	for(int i = 0; i < read_ch_samples; i++)
		for(int j = 0; j < channels; j++)
			*it++ = j == 0 ? 1.0 : 0.0;;*/
	
	
	/*int cpy_frames = min(read_frames, queue_frames);
	int zero_frames = read_frames - cpy_frames;
	
	if (cpy_frames > 0) {
		int cpy_total_samples = cpy_frames * sample_rate * channels;
		ASSERT(cpy_total_samples > 0);
		snd_buf.Get((float*)stream, cpy_total_samples);
	}
	
	if (zero_frames > 0) {
		float* zero_begin = (float*)stream;
		zero_begin += cpy_frames * channels * sample_rate;
		memset(zero_begin, 0, zero_frames * channels * sample_rate);
	}*/
}

bool AudioOutput::Open0() {
	SDL_zero(audio_fmt);
	audio_dev = SDL_OpenAudioDevice(NULL, 0, &audio_desired, &audio_fmt, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	if (audio_dev == 0) {
	    LOG("OOSDL2::AudioOutput::Open0: error: failed to open audio: " << SDL_GetError());
	    return false;
	}
	else {
	    if (audio_fmt.format != audio_desired.format) {
			// we let this one thing change.
	        LOG("OOSDL2::AudioOutput::Open0: warning: couldn't get desired audio format.");
	    }
	    
	    //int audio_frames = max(1, 1024 / audio_fmt.samples);
	    
		snd_buf.SetSize(
			GetSampleSize(),
			IsSampleFloating(),
			audio_fmt.freq,
			audio_fmt.samples,
			audio_fmt.channels,
			2
		);
		snd_buf.Zero();
		
	    SDL_PauseAudioDevice(audio_dev, 0); // start audio playing.
	    
	    return true;
	}
}

void AudioOutput::Close0() {
	if (!audio_dev) {
		SDL_CloseAudioDevice(audio_dev);
		audio_dev = 0;
	}
}



void StaticAudioOutputPut(void* userdata, Uint8* stream, int len) {
	if (userdata)
		((AudioOutput*)userdata)->Put(stream, len);
	else
		memset(stream, 0, len);
}

void AudioOutput::SetDesiredAudioFmt(int sample_freq, int sample_bytes, bool is_sample_floating, int channels, int sample_rate) {
	SDL_zero(audio_desired);
	audio_desired.freq = sample_freq;
	audio_desired.format = AUDIO_S16;
	if (!is_sample_floating) {
		switch (sample_bytes) {
			case 1:	audio_desired.format = AUDIO_S8; break;
			case 2:	audio_desired.format = AUDIO_S16; break;
			case 4:
			case 8:
			default:
					audio_desired.format = AUDIO_S32; break;
		}
	}
	else {
		audio_desired.format = AUDIO_F32;
	}
	audio_desired.channels = channels;
	audio_desired.samples = sample_rate;
	audio_desired.callback = StaticAudioOutputPut;
	audio_desired.userdata = this;
}



NAMESPACE_SDL2_END
