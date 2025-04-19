#include "util.h"

static u32 rndv;

void srand(u16 seed) {
	rndv = seed;
}

u16 rand(void) { // xorshift
	rndv = rndv ^ (rndv << 13);
	rndv = rndv ^ (rndv >> 17);
	rndv = rndv ^ (rndv << 5);
	return rndv & 0x7fff;
}
