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
	sh_ticks playlen, hitlen, pasthit;
	float speed = 1.0f;
	sh_note note;
	sh_sample s;
	sherr ret;

	ret = SH_SUCCESS;

	playlen = gticks - hit->start;

	// if we are finishing this play, set the hitlen
	if (hit->end != 0) {
		hitlen = (hit->end - hit->start) + ins->R_ticks;
		if (hitlen - playlen < len) {
			len = hitlen - playlen;
			ret = SH_DONE;
		}
		// oneshotters are handled by the above, because they set their lenght on start
	} else {
		hitlen = 0;
	}

	// for oneshotters, check end

	// finish if we finish soon?

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
				return ret;
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
		
		s = ins->sound.points[i];
		// Apply ADSR

		if ((playlen + j) <= ins->A_ticks) {				// A
			s *= ins->peak;
			s *= (playlen + j) / (sh_sample)ins->A_ticks;
		} else if ((playlen + j) <= (ins->A_ticks + ins->D_ticks)) {	// D
			// d[i] *= peak - ((peak - sus) * ((i-A)/float(D)))
			s *= ins->peak - ((ins->peak - ins->sus) * ((playlen+j - ins->A_ticks) / (sh_sample)ins->D_ticks));
		} else {
			s *= ins->sus;						// S
		}
		if (hitlen != 0) {
			pasthit = gticks + j - hit->end;			// R
			s *= (((sh_sample)ins->R_ticks) - pasthit) / (sh_sample)ins->R_ticks;
		}

		buf[j] += s;
	}
	return ret;
}

