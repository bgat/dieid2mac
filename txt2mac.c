#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char u8_t;

static u8_t lsr8(u8_t v)
{
	return (v >> 1) | (v << 7);
}

u8_t galois_lfsr8(u8_t seed)
{
	if (seed & 1)
		return lsr8(seed) ^ 0xb8; // x^8 + x^6 + x^5 + x^4 + 1
	return lsr8(seed);
}

static const u8_t INIT_SEED = 0xb8;

/* converts data stdin to an ethernet MAC address on stdout */
int main(int argc, const char *argv[], const char *env[])
{
	u8_t seed = INIT_SEED;
	u8_t mac[6];
	int wm, c;

	while (1) {
		c = fgetc(stdin);
		if (c == EOF)
			break;
		if (!c)
			continue;
		seed = galois_lfsr8(c ^ seed);
	}

	for (wm = 0; wm < sizeof(mac) / sizeof(*mac); wm++) {
		seed = galois_lfsr8(seed);
		mac[wm] = seed;
	}

	/* unicast, locally-administered */
	mac[0] &= ~1;
	mac[0] |= 2;

	for (wm = 0; wm < sizeof(mac) / sizeof(*mac); wm++) {
		fprintf(stdout, "%s%02x", wm == 0 ? "" : ":", mac[wm]);
	}
	fprintf(stdout, "\n");
	return 0;
}

