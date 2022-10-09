#include <stdlib.h>
#include "cmpltrtok.h"

int *cmp_cp_int(int val) {
	int *p = malloc(sizeof(int));
	*p = val;
	return p;
}
