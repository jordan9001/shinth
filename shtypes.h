#ifndef SHERROR_H
#define SHERROR_H

#define SH_SUCCESS	0
#define SH_ERROR	-1

typedef sherr int;

// oneshot samples are triggered and track along the hit.curs
// trigger ones are oneshot, but they always finish playing the whole sample
// pingpong and loop samples go off of the global tick count always for position
typedef enum {SH_ONESHOT, SH_TRIGGER, SH_PINGPONG, SH_LOOP} sh_sample_type;

typedef sh_sample uint16_t;
typedef sh_tick uint32_t;
typedef sh_note int16_t;	// 0 is A4, -12 is A3, etc

#endif
