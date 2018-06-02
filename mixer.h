#ifndef MIXER_H
#define MIXER_H

#include "instrument.h"
#include "shtypes.h"

#define NUM_INS	5

typedef struct sh_hit {
	sh_tick start;		// global tick start time
	sh_tick end;		// (zero while still held)
	sh_note note;
	size_t instsrument_index;
	
	struct sh_hit* next;
} sh_hit;

typedef struct {
	sh_instrument* instruments[NUM_INS];	
	sh_hit* active_hits;
} sh_mixer;

sherr sh_initmixer(sh_mixer* mixer);
sherr sh_addinstrument(sh_mixer* mixer, sh_instrument* ins, size_t* index);
sh_instrument* sh_getinstrument(sh_mixer* mixer, size_t index);
sherr sh_addhit(sh_mixer* mixer, sh_hit*);
sherr sh_input(sh_mixer* mixer, size_t index, sh_note note, int down);
sherr sh_fillbuf(sh_mixer* mixer, sh_tick start, sh_tick end);

 
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
