#include <stdio.h>
#include <stdlib.h>
#include "cmpltrtok.h"

int main() {
	char *exts[] = {
		"",
		".pth",
		".pth.numpy.tmp.wts",
		"/model.wts",
		"\\model.wts",
		".[good]wts",
		".|test|.nump",
		": \\ . ? * + [ ] ( ) < > |",
	};
	int i, ln;
	for (i = 0, ln = sizeof(exts)/sizeof(exts[0]); i < ln; ++i) {
		char *escaped = cmp_escape_regex(exts[i]);
		printf("|%s|=>\n|%s|\n\n", exts[i], escaped);
		free(escaped);
	}
}
