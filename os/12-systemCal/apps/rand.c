#include "os.h"

// int rand()
// {
//   uint64_t cycle = (uint64_t*)CLINT_MTIME;
//   return cycle;
// }


static uint32_t state = (uint32_t*)CLINT_MTIME;

// int rand(void) // RAND_MAX assumed to be 32767
// {
//     next = next * 1103515245 + 12345;
//     return (unsigned int)(next/65536) % 32768;
// }

void srand(uint32_t seed)
{
    state = seed;
}

//https://en.wikipedia.org/wiki/Lehmer_random_number_generator
uint32_t rand()
{
	uint64_t product = state * 48271;
	uint32_t x = (product & 0x7fffffff) + (product >> 31);

	x = (x & 0x7fffffff) + (x >> 31);
	return state = x;
}