#include "instrument.h"
#include "pitchtable.h"

// Super simple fast shift algorithm
/*
 * >>> def shift(x,s):
 * ...     nd = []
 * ...     c = 0.0
 * ...     for ix in x:
 * ...             c = c + 1
 * ...             while c >= s:
 * ...                     c = c - s
 * ...                     nd.append(ix)
 * ...     return nd
 */

// This needs a lowpass filter afterwards
// Probably best to just put one lowpass filter on the output in the end

sherr sh_getsamples(sh_instrument* ins, sh_tick gticks, sh_hit* hit, size_t len, sh_sample* buf) {
	size_t i, j;
	sh_ticks playlen;
	float speed = 1.0f;
	sh_note note;

	playlen = gticks - hit->start;

	// finish if we finish soon?
	if (hit->end != 0 && (len > (hit->end - gticks))) {
		len = (hit->end - gticks);
	}

	// adjust for speed
	while (note > 12) {
		speed *= 2;
		note -= 12;
	}
	while (note < -12) {
		speed /= 2;
		note += 12;
	}
	speed = *(a_speed + note) * speed;

	// POSSIBLE TODO: Unravel this loop with i += speed?
	for (j = 0; j < len; j++) {
		// get index according to sound type and ticks and speed
		switch (ins->sound.type) {
		case SH_ONESHOT:
		case SH_TRIGGER:
			// The index is based on how long the hit has been going
			i = playlen + j;
			i = (size_t)(speed * (float)i);
			if (i >= ins->length) {
				return SH_SUCCESS;
			}
			break;
		case SH_LOOP:
			i = gticks + j;
			i = (size_t)(speed * (float)i);
			i = i % ins->length;
			break;
		case SH_PINGPONG:
			i = (gticks + j);
			i = (size_t)(speed * (float)i);
			i = i % (ins->length * 2);
			if (i >= ins->length) {
				i = (ins->length * 2) - i;
			}			
			break;
		}

		// Apply ASDR
		//TODO

		buf[j] += ins->sound.points[i];
	}
	return SH_SUCCESS;
}

