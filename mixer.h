#ifndef MIXER_H
#define MIXER_H

#include "instrument.h"
#include "shtypes.h"

#define NUM_INS	5
#define INVALID_INS	(NUM_INS + 1)

// inputs are recorded as hits against a global tick counter
// ends can be added after the input is released
typedef struct sh_hit {
	sh_tick start;		// global tick start time
	sh_tick end;		// (same as start while still held)
	sh_note note;
	size_t instrument;
} sh_hit;

typedef struct {
	sh_instrument* instruments[NUM_INS];	
	sh_hit* active_hits;
	size_t hits_len;

	size_t low_used_hit;
	size_t high_used_hit;
} sh_mixer;

sherr sh_init_mixer(sh_mixer* mixer, sh_hit* hit_arr, size_t hit_arr_len);
sherr sh_input(sh_mixer* mixer, size_t index, sh_note note, int down);
sherr sh_fillbuf(sh_mixer* mixer, sh_tick start, sh_sample* buf, size_t buflen);
 
/*
Notes:
The way this is used is such
You initialize the synth with a few instruments
You have a mixer
the mixer holds the instruments and directs inputs to them
it can put tick ranges into a buffer that the callback can pass out to the audio device
or the callback itself can ask for a buffer to be filled
*/
#endif
