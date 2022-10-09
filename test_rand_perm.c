#include <stdio.h>
#include <stdlib.h>
#include "cmpltrtok.h"

int main(int argc, char* argv[]) {
	unsigned int range = 10, seed = 666;
	if (argc >= 2) {
		range = atoi(argv[1]);
	}
	if (argc >= 3) {
		seed = atoi(argv[2]);
	}

	unsigned int *perm = cmp_rand_perm(range, seed);
	if (1) {
		int i;
		for (i = 0; i < range; ++i) {
			printf("%d: %u\n", i, perm[i]);
		}
	}
	free(perm);
}
