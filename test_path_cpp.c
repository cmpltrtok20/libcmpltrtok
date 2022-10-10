#include <stdio.h>
#include <stdlib.h>
#include "cmpltrtok.h"

typedef struct {
	const char *prefix;
	const char *ext;
} MyPair;

int main() {
	MyPair pairs[] = {
		{"/var/xxx/yyy/zz/_save/abc.py/v3.0", "/model.index"},
		{"", ""},
		{"/var/xxx/yyy/zz/_save/abc.py/v3.0", ""},
		{"", "/model.index"},
		{"_save/abc.out/v2.0/name-id-epoch", ".pth"}
	};
	int i, ln;
	for (i = 0, ln = sizeof(pairs)/sizeof(pairs[0]); i < ln; ++ i) {
		char *full = cmp_get_path_from_prefix(pairs[i].prefix, pairs[i].ext);
		char *calculated_prefix = 0, *default_prefix = 0, *forced_wrong = 0, *tolerated = 0;
		if(!cmp_get_prefix_from_path(full, pairs[i].ext, 1, &calculated_prefix)) {
			fprintf(stderr, "Cannot get prefix from full=|%s|, ext=|%s|\n", full, pairs[i].ext);
		}
		if(!cmp_get_prefix_from_path(full, 0, 1, &default_prefix)) {
			fprintf(stderr, "Cannot get prefix from full=|%s|, ext=|%s|\n", full, pairs[i].ext);
		}
		if(!cmp_get_prefix_from_path(full, "XXX", 1, &forced_wrong)) {
			fprintf(stderr, "Cannot get prefix from full=|%s|, ext=|%s|\n", full, "XXX");
		}
		if(!cmp_get_prefix_from_path(full, "XXX", 0, &tolerated)) {
			fprintf(stderr, "Cannot get prefix from full=|%s|, ext=|%s|\n", full, "XXX");
		}
		printf("|%s| + |%s| =>\n|%s| =>\n|%s|\n|%s|(default)\n|%s|(forced wrong)\n|%s|(tolerated)\n\n",
			pairs[i].prefix, pairs[i].ext, full,
			calculated_prefix, default_prefix, forced_wrong, tolerated);
		free(full);
		free(calculated_prefix);
		free(default_prefix);
		free(forced_wrong);
		free(tolerated);
	}
}
