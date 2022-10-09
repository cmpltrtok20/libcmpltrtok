#include <stdio.h>
#include <stdlib.h>
#include "cmpltrtok.h"

int main() {
	long long milli = cmp_current_timestamp();
	char *repr_utc = cmp_milli2printable(milli, 1),
		 *repr_local = cmp_milli2printable(milli, 0);
	printf("%lld\nUTC: %s\nLocal: %s\n", milli, repr_utc, repr_local);
	free(repr_utc);
	free(repr_local);
}
