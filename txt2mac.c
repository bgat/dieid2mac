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

/* converts a string on stdin to a legal MAC address on stdout */
int main(int argc, const char *argv[], const char *env[])
{
	u8_t seed = 0;
	u8_t mac[6];
	int c = 0, wm;

	do {
		c = fgetc(stdin);
		seed = galois_lfsr8(seed ^ c);
	} while(c != EOF);

	for (wm = 0; wm < sizeof(mac); wm++)
		mac[wm] = seed = galois_lfsr8(seed);

	/* force unicast, locally-administered */
	mac[0] &= ~1;
	mac[0] += 2;

	for (wm = 0; wm < sizeof(mac); wm++) {
		fprintf(stdout, "%s%02x", wm == 0 ? "" : ":", mac[wm]);
	}
	fprintf(stdout, "\n");

	
	/* 1401903128120022 */
	return 0;
}

