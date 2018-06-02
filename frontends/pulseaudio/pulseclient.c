#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <pulse/pulseaudio.h>

#define BUFSZ	0x40000
#define RATE	44100
#define PI2	6.283185307179586f
#define PI2RATE	(PI2 / RATE)

#define STATE_UNREADY	0
#define STATE_READY	1
#define STATE_ERROR	2

unsigned int tick = 0;
static float sampledata[BUFSZ];
static pa_buffer_attr bufattr;
static const pa_sample_spec ss = {
	.format = PA_SAMPLE_FLOAT32LE,
	.rate = RATE,
	.channels = 1
};

void state_cb(pa_context* c, void* userdata) {
	pa_context_state_t state;
	int* pa_ready = userdata;

	state = pa_context_get_state(c);
	switch  (state) {
	case PA_CONTEXT_UNCONNECTED:
	case PA_CONTEXT_CONNECTING:
	case PA_CONTEXT_AUTHORIZING:
	case PA_CONTEXT_SETTING_NAME:
	default:
		// don't set the state, still waiting
		break;
	case PA_CONTEXT_FAILED:
	case PA_CONTEXT_TERMINATED:
		*pa_ready = STATE_ERROR;
		break;
	case PA_CONTEXT_READY:
		*pa_ready = STATE_READY;
		break;
	}
}

static void write_cb(pa_stream* s, size_t nbytes, void* userdata) {
	/*float* data;
	size_t i;

	printf("nb %d\n", nbytes);
	pa_stream_begin_write(s, (void**)&data, &nbytes);
	printf("na %d\n", nbytes);

	for (i=0; i<(nbytes / sizeof(sampledata[0])); i++) {
		data[i] = sinf((i + tick) * 440 * PI2RATE);
		tick++;
	}

	nbytes = i * sizeof(sampledata[0]);

	printf("np %d\n", nbytes);
	pa_stream_write(s, data, nbytes, NULL, 0, PA_SEEK_RELATIVE);
	printf("nw\n");*/

	size_t count;
	size_t i;

	count = nbytes / sizeof(sampledata[0]);
	if (count > BUFSZ) {
		count = BUFSZ;
	}

	for (i=0; i<count; i++) {
		sampledata[i] = sinf((i + tick) * 440 * PI2RATE);
		tick++;
	}

	pa_stream_write(s, sampledata, count * sizeof(sampledata[0]), NULL, 0, PA_SEEK_RELATIVE);
	printf(".\n");
}

int main(int argc, char** argv) {
	pa_mainloop* ml;
	pa_mainloop_api* mlapi;
	pa_context* ctx;
	pa_stream* playstream;

	int r = 0;
	int pa_ready = 0;
	unsigned int i;

	// create a tone
	for (i=0; i<BUFSZ; i++) {
		sampledata[i] = sinf(i * 440 * PI2RATE);
	}

	ml = pa_mainloop_new();
	mlapi = pa_mainloop_get_api(ml);
	ctx = pa_context_new(mlapi, "Simple PA test application");
	// connect the context to the audio server
	pa_context_connect(ctx, NULL, 0, NULL);

	// define callback for server state
	pa_context_set_state_callback(ctx, state_cb, &pa_ready);

	// can't do anything till ready, so just iterate till then
	while (pa_ready == 0) {
		pa_mainloop_iterate(ml, 1, NULL);
	}
	if (pa_ready == 2) {
		r = -1;
		goto exit;
	}

	printf("Connected\n");

	playstream = pa_stream_new(ctx, "Playback", &ss, NULL);
	if (!playstream) {
		printf("Failed to create a new stream\n");
		r = -1;
		goto exit;
	}

	pa_stream_set_write_callback(playstream, write_cb, NULL);

	bufattr.fragsize = (uint32_t)-1;
	bufattr.maxlength = (uint32_t)-1;
	bufattr.minreq = (uint32_t)-1;
	bufattr.prebuf = (uint32_t)-1;
	bufattr.tlength = (uint32_t)-1;

	r = pa_stream_connect_playback(playstream, NULL, &bufattr, PA_STREAM_INTERPOLATE_TIMING|PA_STREAM_AUTO_TIMING_UPDATE, NULL, NULL);

	if (r < 0) {
		printf("Stream connect failed\n");
		goto exit;
	}

	pa_mainloop_run(ml, NULL);

exit:
	printf("Done, exiting...\n");
	pa_context_disconnect(ctx);
	pa_context_unref(ctx);
	pa_mainloop_free(ml);
	return r;
}
