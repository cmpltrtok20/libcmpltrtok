#include <stdio.h>
#include "cmpltrtok.h"

void cmp_title(const char* content) {
	printf("--------------------------------%s--------------------------------\n", content);
}

void cmp_newline(int n) {
	int i;
	for (i = 0; i < n; ++i) {
		printf("\n");
	}
}
