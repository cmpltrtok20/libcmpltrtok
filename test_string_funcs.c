#include <stdio.h>
#include <stdlib.h>
#include "cmpltrtok.h"

int main() {
	cmp_title("Test cmp_strncat");
	const char *dest = "0123456789";
	int n_arr[] = {5, 7, 10, 13, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 5000};
	int i;
	for (i = 0; i < sizeof(n_arr)/sizeof(n_arr[0]); ++i) {
		int n = n_arr[i];
		char *joined = cmp_strncat(dest, n, "-aaaa", "-bbbb", "-cccc", "-Dave Rodgers", "-Edge the wingwoman");
		printf("n=% 5d: |%s|\n", n, joined);
		free(joined);
	}
	cmp_title("All over");
}
