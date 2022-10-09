#include <stdlib.h>
#include <stdio.h>
#include "cmpltrtok.h"

int cmp_argmax(float* vals, int n) {
	if (n <= 0) {
		return 0;
	}
	int i, pos = 0;
	float max = vals[0];
	for (i = 1; i < n; ++i) {
		if (vals[i] > max) {
			max = vals[i];
			pos = i;
		}
	}
	return pos;
}

unsigned int* cmp_rand_perm(unsigned int range, unsigned int seed) {
	srand(seed);

	// allocate space
	unsigned int *perm = malloc(sizeof(int) * range);
	struct UINT_TAILQ_HEAD *plist_head = calloc(1, sizeof(struct UINT_TAILQ_HEAD));
	struct UINT_TAILQ_HEAD *pperm_head = calloc(1, sizeof(struct UINT_TAILQ_HEAD));
	TAILQ_INIT(plist_head);
	TAILQ_INIT(pperm_head);

	// the pool to permutate
	if (1) {
		int i;
		for (i = 0; i < range; ++i) {
			struct UINT_TAILQ_ENTRY *np = malloc(sizeof(struct UINT_TAILQ_ENTRY));
			np->data = i;
			TAILQ_INSERT_TAIL(plist_head, np, entries);
		}
	}

	// permutate
	if (1) {
		int size = range;
		int i;
		for (i = 0; i < range; ++i) {
			// randomly select from pool
			int rand_offset = rand() % size;
#ifdef TRACE
			printf("(%d: %u), ", i, rand_offset);
#endif
			struct UINT_TAILQ_ENTRY *n1 = 0, *n2 = 0;
			if (rand_offset <= (size - 1) / 2) {
				// count from head
				n1 = TAILQ_FIRST(plist_head);
				int counter = rand_offset;
				while (counter-- && n1 != NULL) {
					n2 = TAILQ_NEXT(n1, entries);
					n1 = n2;
				}
			} else {
				// count from tail
				n1 = TAILQ_LAST(plist_head, UINT_TAILQ_HEAD);
				int counter = size - 1 - rand_offset;
				while (counter-- && n1 != NULL) {
					n2 = TAILQ_PREV(n1, UINT_TAILQ_HEAD, entries);
					n1 = n2;
				}
			}

			// record to result list
			struct UINT_TAILQ_ENTRY *np = malloc(sizeof(struct UINT_TAILQ_ENTRY));
			np->data = n1->data;
			TAILQ_INSERT_TAIL(pperm_head, np, entries);

			// remove from pool
			TAILQ_REMOVE(plist_head, n1, entries);
			--size;
		}
#ifdef TRACE
			printf("\n");
#endif
	}

	// list to continuous mem space
	if (1) {
		struct UINT_TAILQ_ENTRY *n1 = TAILQ_FIRST(pperm_head), *n2 = 0;
		int i = 0;
		while (n1 != NULL) {
			perm[i++] = n1->data;
			n2 = TAILQ_NEXT(n1, entries);
			n1 = n2;
		}
	}

	// destroy lists
	if (1) {
		struct UINT_TAILQ_ENTRY *n1 = TAILQ_FIRST(plist_head), *n2 = 0;
		while (n1 != NULL) {
			n2 = TAILQ_NEXT(n1, entries);
			free(n1);
			n1 = n2;
		}
		TAILQ_INIT(plist_head);
		free(plist_head);
	}
	if (1) {
		struct UINT_TAILQ_ENTRY *n1 = TAILQ_FIRST(pperm_head), *n2 = 0;
		while (n1 != NULL) {
			n2 = TAILQ_NEXT(n1, entries);
			free(n1);
			n1 = n2;
		}
		TAILQ_INIT(pperm_head);
		free(pperm_head);
	}

	return perm;
}
