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

sh_sample sh_getsample(sh_instrument* ins, sh_tick gticks, sh_hit* hit) {
	uint16_t i;
	float speed = 1.0f;
	sh_note note;

	// get index according to sound type and ticks
	switch (ins->sound.type) {
		//TODO
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

	speed = speed * (a_speed + note);

	i = (int)(speed * (float)i);
	
	// get actual sample at this speed and time
	//TODO
	ins->sound.points[
}

