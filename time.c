#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "cmpltrtok.h"

/* https://stackoverflow.com/questions/3756323/how-to-get-the-current-time-in-milliseconds-from-c-in-linux */
long long cmp_current_timestamp() {
    struct timeval te;
    gettimeofday(&te, 0); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

char *cmp_milli2printable(long long milliseconds, int is_utc) {
	long sec = milliseconds / 1000, milli = milliseconds % 1000;
	struct tm *time_stru;
	if (is_utc) {
		time_stru = gmtime(&sec);
	} else {
		time_stru = localtime(&sec);
	}
	char buf[MAXLINE];
	strftime(buf, MAXLINE, "%Y-%m-%d %H:%M:%S", time_stru);
	char buf2[MAXLINE * 2];
	snprintf(buf2, MAXLINE * 2, "%s.%03ld", buf, milli);
	return cmp_cp_str(buf2);
}
