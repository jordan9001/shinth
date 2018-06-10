/*

This will be the most basic synth to get all the base features working

1: set up probeaudio

2: generate a 440 sine sample, just one period

3: create an instrument and mixer

4: Hook the keyboard up to playing the synth?

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portaudio.h>
#include "../../instrument.h"
#include "../../mixer.h"
#include "../../shtypes.h"

#define NUM_SECONDS		4
#define	SAMPLE_RATE		44100
#define FRAMES_PER_BUFFER	0	// Let them choose best
#define PI2			6.283185307179586f
#define PI2RATE			(PI2 / SAMPLE_RATE)
#define DEVICE			0

int setupShinth() {
	sh_sound* sinsamp;
	sh_instrument* ins;
	size_t i;

	// generate the sound
	sinsamp = (sh_sound*)malloc(sizeof(sh_sound));
	
	// allocate enough for one period
	sinsamp->length = SAMPLE_RATE / 440 // not a clean div, might cause buzz
	
	sinsamp->points = (sh_sample*)malloc(simsamp->length * sizeof(sh_sample));
	
	for (i=0; i<sinsamp->length; i++) {
		// sin(i * freq * 2 * pi / samp_rate)
		sinsamp->points[i] = sinf(i * 440 * PI2RATE);
	}
	
	// create the instrument
	ins = (sh_instrument*)malloc(sizeof(sh_instrument));

	// create the mixer
	
}

int main(int argc, char** argv) {
	
}
