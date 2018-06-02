#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#define NUM_SECONDS		4
#define	SAMPLE_RATE		44100
#define FRAMES_PER_BUFFER	0	// Let them choose best
#define PI2			6.283185307179586f
#define PI2RATE			(PI2 / SAMPLE_RATE)
#define DEVICE			0

typedef struct {
	float level;
	float freq;
} paTestData;

static unsigned int tick = 0;

static int writeCallback (const void* inputbuf, void* outputbuf, unsigned long framesperbuf, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userdata) {
	paTestData* data = (paTestData*)userdata;
	float* out = (float*)outputbuf;
	unsigned int i;
	
	for (i=0; i<framesperbuf; i++) {
		*out = sinf((i + tick) * data->freq * PI2RATE) * data->level;
		out++;
	}
	tick += i;
	return 0;
}

int main(int argc, char** argv) {
	PaStream* stream;
	PaError err;
	paTestData cbdata;
	PaStreamParameters outp;

	printf("PortAudio Example\n");
	cbdata.level = 1.0f;
	cbdata.freq = 440.0f;
	
	err = Pa_Initialize();
	if (err != paNoError) {
		printf("Initialize Error: %s\n", Pa_GetErrorText(err));
		goto exit;
	}

	outp.channelCount = 1;
	outp.device = DEVICE;
	outp.hostApiSpecificStreamInfo = NULL;
	outp.sampleFormat = paFloat32;
	outp.suggestedLatency = Pa_GetDeviceInfo(DEVICE)->defaultHighOutputLatency;
	outp.hostApiSpecificStreamInfo = NULL;

	err = Pa_IsFormatSupported(NULL, &outp, SAMPLE_RATE);
	if (err != paNoError) {
		printf("Unsupported Format: %s\n", Pa_GetErrorText(err));
		goto exit;
	}

	err = Pa_OpenStream(&stream,
				NULL,
				&outp,
				SAMPLE_RATE,
				FRAMES_PER_BUFFER,
				0,
				writeCallback,
				&cbdata);

	if (err != paNoError) {
		printf("Open Stream Error: %s\n", Pa_GetErrorText(err));
		goto exit;
	}

	err = Pa_StartStream(stream);
	if (err != paNoError) {
		printf("Start Stream Error: %s\n", Pa_GetErrorText(err));
		goto exit;
	}

	getchar();

	cbdata.freq = 220.0f;

	getchar();
	cbdata.freq = 880.0f;

	getchar();
	cbdata.freq = 440.0f;

	Pa_Sleep(NUM_SECONDS * 1000);

	err = Pa_StopStream(stream);
	if (err != paNoError) {
		printf("Stop Stream Error: %s\n", Pa_GetErrorText(err));
		goto exit;
	}
	
	err = Pa_CloseStream(stream);
	if (err != paNoError) {
		printf("Close Stream Error: %s\n", Pa_GetErrorText(err));
		goto exit;
	}

exit:
	Pa_Terminate();
	printf("Finished\n");
	return err;
}
