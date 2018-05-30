#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <stdtype.h>

// oneshot samples are triggered and track along the hit.curs
// pingpong and loop samples go off of the global tick count always for position
typedef enum {SH_ONESHOT, SH_PINGPONG, SH_LOOP} sh_sample_type;

typedef sh_sample uint16_t;
typedef sh_tick uint32_t;
typedef sh_note int16_t;	// 0 is A4, -12 is A3, etc

typedef struct {
	uint16_t length;	// length of sample
	sh_sample* points;	// actual samples of sound
	sh_sample_type type;	// type of looping
	sh_note natural;	// go by -pitch to get to A4
} sh_sound;

typedef struct {
	sh_tick start;		// global tick start time
	sh_tick end;		// (zero while still held)
	sh_note note;
} sh_hit;

typedef struct {
	sh_sound sound;

	// ASDR
	uint16_t A_ticks;
	uint16_t D_ticks;
	uint16_t S_ticks;
	uint16_t R_ticks;
	sh_sample peak;
	sh_sample sus;
} sh_instrument;

typedef struct {
} sh_kit;


// Function Definitions
sh_sample sh_getsample(sh_instrument* ins, sh_tick gticks, sh_hit* hit);

#endif
