#include "mixer.h"


sherr sh_init_mixer(sh_mixer* mixer, sh_hit* hit_arr, size_t hit_arr_len) {
	size_t i;
	
	for (i=0; i<NUM_INS; i++) {
		mixer->instruments[i] = NULL;
	}

	mixer->active_hits = hit_arr;
	mixer->hits_len = hit_arr_len;
	for (i=0; i<hit_arr_len; i++) {
		mixer->active_hits[i].instrument_index = INVALID_INS;
	}

	mixer->low_used_hit = 0;
	mixer->high_used_hit = 0;
}

sherr sh_input(sh_mixer* mixer, size_t index, sh_note note, sh_tick gticks, int down) {
	size_t i, hlen, next;
	sh_hit* hit;

	hlen = mixer->hits_len;
	// check if this is an existing hit
	for (i = mixer->low_used_hit; i != mixer->high_used_hit; i = (i+1) % hlen) {
		hit = &(mixer->active_hits[i]);
		if (hit->instrument == index && hit->note == note) {
			// update this hit and be done
			if (down) {
				hit->start = gticks;
				switch (mixer->instruments[index].sound.type) {
				case SH_ONESHOT:
				case SH_TRIGGER:
					hit->end = gticks + mixer->instruments[index].sound.length;
					break;
				}
			} else {
				hit->end = gticks;
			}
			return SH_SUCCESS;
		}
	}

	if (!down) {
		// tried to end a hit we didn't have?
		return SH_UNKNOWN_HIT;
	}

	// ok, add a new hit
	i = mixer->high_used_hit;
	next = (i+1) % hlen;
	if (next == mixer->low_used_hit) {
		return SH_NO_FREE_HIT;
	}
	mixer->high_used_hit = next;

	hit = &(mixer->active_hits[i]);
	hit->start = gticks;
	hit->end = 0;
	hit->note = note;
	hit->instrument = index;

	return SH_SUCCESS;
}

sherr sh_fillbuf(sh_mixer* mixer, sh_tick start, sh_sample* buf, size_t buflen) {
	size_t i;
	size_t iins;
	size_t hlen;
	sh_instrument* ins;
	sherr err;
	// remove finished hits as you finish them
	// Also update mixer hits cache thing
	
	// for each active hit
	hlen = mixer->hits_len;
	// have the instrument add to the buffer
	for (i = mixer->low_used_hit; i != mixer->high_used_hit; i = (i+1) % hlen) {
		iins = mixer->active_hits[i].instrument;
		if (iins != INVALID_INS) {
			ins = mixer->instruments[iins];
			err = sh_getsamples(sh_instrument* ins, start, &(mixer->active_hits[i]), buflen, buf);
			// if this hit is done, clear it by setting it's ins invalid
			if (err == SH_DONE) {
				mixer->active_hits[i].instrument = INVALID_INS;
			} else if (!SH_OK(err)) {
				return err;
			}
		}
		// if it is the low_used_hit, up low_used hit to the next one
		if (i == mixer->low_used_hit && mixer->active_hits[i].instrument != INVALID_INS) {
			mixer->low_used_hit = (i+1) % hlen;
		}
	} 
	return SH_SUCCESS;
}
