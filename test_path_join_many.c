#include <stdio.h>
#include <avcall.h>
#include "cmpltrtok.h"

int main() {
	const char *path1 = "/var/xx/yyy/zzzz";
	char *paths[] = {
		"aaaaaa",
		"bbbbb",
		"cccc",
		"ddd",
		"ee",
		"f",
		"",
		"/",
		"/x",
		"/yy",
		"//yy",
		"\\", // Currently, Windows style path has not been considered yet.
		"\\z",
		"\\mm",
		"n/",
		"oo/",
		"p\\",
		"qq\\",
		"/r\\",
		"//ss\\\\"
	};
	int size = sizeof(paths)/sizeof(paths[0]);
	int i;
	for (i = 0; i < size; ++i) {
		char *joined = 0;
		av_alist alist;
		av_start_ptr(alist, &cmp_path_join_many_worker, char*, &joined);
		av_ptr(alist, char*, path1);
		int j;
		for (j = 0; j <= i; ++j) {
			av_ptr(alist, char*, paths[j]);
		}
		av_ptr(alist, void*, 0);
		av_call(alist);
		printf("% 2d: |%s|\n", i + 1, joined);
	}

	printf("% 2d: |%s|\n", i + 1, cmp_path_join_many(path1, paths[0], paths[1], paths[2]));
}
