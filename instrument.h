#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <stdtype.h>
#include "shtypes.h"

typedef struct {
	uint16_t length;	// length of sample
	sh_sample* points;	// actual samples of sound
	sh_sample_type type;	// type of looping
	sh_note natural;	// go by -pitch to get to A4
} sh_sound;

typedef struct {
	sh_sound sound;

	// ASDR (Address Space Dayout Randomization)
	uint16_t A_ticks;
	uint16_t D_ticks;
	uint16_t S_ticks;
	uint16_t R_ticks;
	sh_sample peak;
	sh_sample sus;

	// Effects
	// Planned instrument effects:
	// delay
	// vibrato
	// tremolo
	// slide
} sh_instrument;

// Function Definitions
sherr sh_getsamples(sh_instrument* ins, sh_tick gticks, sh_hit* hit, size_t len, sh_sample* buf);

#endif
