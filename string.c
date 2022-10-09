#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "cmpltrtok.h"

char* cmp_cp_str(const char* src) {
	size_t len = strnlen(src, MAXLINE) + 1; // null-terminated
	char* dest = malloc(sizeof(char) * len);
	cmp_strncpy(dest, src, len);
	return dest;
}

char *cmp_substr(const char *inpStr, char *outStr, const size_t startPos, const size_t strLen) {
  cmp_strncpy(outStr, inpStr + startPos, strLen);
  return outStr;
}

char *cmp_substr_with_alloc(const char* inpStr, const size_t startPos, const size_t strLen) {
	size_t len = strLen + 1;
	char* dest = malloc(sizeof(char) * len);
	cmp_substr(inpStr, dest, startPos, strLen);
	return dest;
}

void cmp_strncpy(char *dest, const char *src, const size_t n) {
	if (!n) {
		return;
	}
	*dest = 0;
	strncat(dest, src, n - 1);
}

char *cmp_strncat_worker(const char *dest, const size_t n, ...) {
	char buf[MAXLINE * 2];
	cmp_strncpy(buf, dest, MAXLINE * 2);

	int catenated_specified = 0;
	va_list ap;
	va_start(ap, n);
	while (1) {
		char *src = va_arg(ap, char*);
		if (!src) {
			break;
		}
		size_t occupied = strnlen(buf, MAXLINE * 2 - 1) + 1;
		size_t left = MAXLINE * 2 - occupied;
		if (left <= 0) {
			break;
		}
		int left_specified = n - catenated_specified;
		if (left_specified <= 0) {
			break;
		}
		int left_considered = (left <= left_specified ? left : left_specified);
		strncat(buf, src, left_considered);
		catenated_specified += strnlen(src, n);
	}
	va_end(ap);

	return cmp_cp_str(buf);
}
