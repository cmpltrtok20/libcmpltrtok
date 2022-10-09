#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "cmpltrtok.h"

int is_path_fully_provided(const char *path) {
	size_t len = strnlen(path, MAXLINE);
	if (!len) {
		return 0;
	}
	switch (path[0]) {
	case '/':
		return 1;
	default:
		return 0;
	}
}

int cmp_mkdir_p(const char *dir, mode_t mode) {
    char tmp[MAXLINE];
    char *p = 0;
    size_t len;

    snprintf(tmp, MAXLINE, "%s", dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            if(!cmp_mkdir_if_not_existed(tmp, mode)) {
            	return 0;
            }
            *p = '/';
        }
    if(!cmp_mkdir_if_not_existed(tmp, mode)) {
    	return 0;
    } else {
    	return 1;
    }
}

int cmp_mkdir_if_not_existed(const char *dir_path, mode_t mode) {
	if (cmp_dir_exists(dir_path)) {
		return 1;
	}
	if (mkdir(dir_path, mode)){
		fprintf(stderr, "Cannot mkdir |%s| as 0%o\n", dir_path, mode);
		return 0;
	}
	return 1;
}

int cmp_dir_exists(const char *dir_path) {
	DIR* dir = opendir(dir_path);
	if (dir) {
	    /* Directory exists. */
	    closedir(dir);
	    return 1;
	} else if (ENOENT == errno) {
	    /* Directory does not exist. */
		return 0;
	} else {
	    /* opendir() failed for some other reason. */
		fprintf(stderr, "dir_exists: opendir(%s) failed for some other reason.", dir_path);
		return 0;
	}
}

char *cmp_get_base_main_name(const char *path) {
	// get base
	char *base = cmp_path_split(path, 0, 0);

	// get main name of base
	char main_buf[MAXLINE];
	char *ext = cmp_path_split_ext(base, main_buf, MAXLINE);
	free(ext);
	return cmp_cp_str(main_buf);
}

char *cmp_get_dir(const char *path) {
	char dir_buf[MAXLINE];
	char *base = cmp_path_split(path, dir_buf, MAXLINE);
	free(base);
	return cmp_cp_str(dir_buf);
}

char *cmp_path_split(const char *path_ori, char *dir_buf, const size_t n) {
	// "xxx/yyy//" or "xxx/yyy///" => "xxx/yyy"
	char *path = cmp_path_remove_tail_slash(path_ori);

	// get last separator '/'
	char sep = '/';
	char *base = strrchr(path, sep);
	char *dir = 0;
	if (!base) { // path does not contain any separator
		dir = "";
		base = path; // "xxx" => "" + "xxx"
	} else if (base == path) { // path's right most separator is just its leading slash
		dir = "/";
		base += 1;  // "/xxx" => "/" + "xxx"
	} else {
		base += 1; // "xxx/yyy/zzz" => "xxx/yyy" + "zzz"
	}
	// make a copy of base ptr
	char *base_cp = cmp_cp_str(base);

	// determine dir part
	if (dir_buf) {
		if (dir) { // dir is determined already
			cmp_strncpy(dir_buf, dir, n);
		} else {
			// remove base part: "xxx/yyy///zzz" => "xxx/yyy//"
			char *path_cp = cmp_cp_str(path);
			char *sep_ptr = strrchr(path_cp, sep);
			*sep_ptr = 0;

			// no tail: "xxx/yyy//" => "xxx/yyy"
			char *no_tail = cmp_path_remove_tail_slash(path_cp);

			// copy it to dir buffer
			cmp_strncpy(dir_buf, no_tail, n);
			free(path_cp);
			free(no_tail);
		}
	}
	return base_cp;
}

char *cmp_path_split_ext(const char *path, char *main_buf, const size_t n) {
	char dir_buf[MAXLINE], base_main_buf[MAXLINE];
	char *base = cmp_path_split(path, dir_buf, MAXLINE); // path => dir + base

	// right most separator '.' determines the ext
	char sep = '.';
	char *ext = strrchr(base, sep);
	if (!ext) {
		ext = "";
	} else {
		ext += 1;
	}
	char *ext_cp = cmp_cp_str(ext);

	// determine the main name
	if (main_buf) {
		// get a copy of base main
		cmp_strncpy(base_main_buf, base, n);
		// erase ext
		if (ext[0]) {
			char *sep_ptr = strrchr(base_main_buf, sep);
			*sep_ptr = 0;
		}

		if (!dir_buf[0] && base_main_buf[0]) { // dir is "" and base main is not "" => main name is base main
			cmp_strncpy(main_buf, base_main_buf, n);
		} else if (dir_buf[0] && !base_main_buf[0]) { // dir is not "" and base main is "" => main name is dir
			cmp_strncpy(main_buf, dir_buf, n);
		} else if (!dir_buf[0] && !base_main_buf[0]) { // dir and base main are all "" => main name is ""
			main_buf[0] = 0;
		} else { // dir + base main => main name
			size_t len = strnlen(dir_buf, MAXLINE);
			if (dir_buf[len - 1] == '/') { // "xxx/" + "yyy" => "xxx/yyy"
				snprintf(main_buf, n, "%s%s", dir_buf, base_main_buf);
			} else { // "xxx" + "yyy" => "xxx/yyy"
				snprintf(main_buf, n, "%s/%s", dir_buf, base_main_buf);
			}
		}
	}
	return ext_cp;
}

char *cmp_path_get_cousin(char *path, char *uncle_name, char *cousin_ext) {
	char cousin_path_buf[MAXLINE];
	char dir_buf[MAXLINE], parent_dir_buf[MAXLINE];
	char *base_name = cmp_path_split(path, dir_buf, MAXLINE); // path = dir + base name
	char *middle_name = cmp_path_split(dir_buf, parent_dir_buf, MAXLINE); // dir = parent dir + middle name

	if (!cousin_ext) { // no specified ext
		if (parent_dir_buf[0]) {
			snprintf(cousin_path_buf, MAXLINE, "%s/%s/%s", parent_dir_buf, uncle_name, base_name);
		} else {
			snprintf(cousin_path_buf, MAXLINE, "%s/%s", uncle_name, base_name);
		}
	} else { // with specifed ext
		char actual_cousin_ext_buf[MAXWORD], main_buf[MAXLINE];
		char *actual_cousin_ext = 0;
		if (!cousin_ext[0]) { // ext == ""
			actual_cousin_ext = "";
		} else { // ext == ".xxx"
			snprintf(actual_cousin_ext_buf, MAXWORD, ".%s", cousin_ext);
			actual_cousin_ext = actual_cousin_ext_buf;
		}
		char *ext = cmp_path_split_ext(base_name, main_buf, MAXLINE); // base name = main + ext
		if (parent_dir_buf[0]) {
			snprintf(cousin_path_buf, MAXLINE, "%s/%s/%s%s", parent_dir_buf, uncle_name, main_buf, actual_cousin_ext);
		} else {
			snprintf(cousin_path_buf, MAXLINE, "%s/%s%s", uncle_name, main_buf, actual_cousin_ext);
		}
		free(ext);
	}
	free(base_name);
	free(middle_name);
	return cmp_cp_str(cousin_path_buf);
}

int cmp_path_is_label(char *filepath, char *label) {
	char *base = cmp_path_split(filepath, 0, 0);
	char main_buf[MAXLINE];
	char *ext = cmp_path_split_ext(base, main_buf, MAXLINE);
	free(base);
	free(ext);
	char *p = strrchr(main_buf, '_');
	if (!p) {
		return 0;
	}
	p += 1;
	return !strncmp(p, label, MAXLINE);
}

char *cmp_path_remove_tail_slash(const char *path) {
	char *buf = cmp_cp_str(path);
	size_t len = strnlen(buf, MAXLINE);
	if (len <= 1) {
		return buf; // "" => "", "/" => "/", "x" => "x"
	}
	char *pbuf = buf + (len - 1);
	for (;'/' == *pbuf && pbuf > buf; --pbuf) { // "xxx/yyy///" => "xxx/yyy" but "////" => "/"
		*pbuf = 0;
	}
	return buf;
}

char *cmp_path_remove_head_slash(const char *path) {
	char *buf = cmp_cp_str(path);
	size_t len = strnlen(buf, MAXLINE);
	if (len <= 1) {
		return buf; // "" => "", "/" => "/", "x" => "x"
	}
	char *pbuf = buf, *end_pos = buf + (len - 1);
	for (;'/' == *pbuf && pbuf < end_pos; ++pbuf) { // "///xxx/yyy" => "xxx/yyy" but "////" => "/"
		//nothing
	}
	char *buf2 = cmp_cp_str(pbuf);
	free(buf);
	return buf2;
}

char *cmp_path_join(const char *path1_ori,const char *path2_ori) {
	char *path1 = cmp_path_remove_tail_slash(path1_ori); // "xxx/yyy////" => "xxx/yyy"
	char *path2 = cmp_path_remove_head_slash(path2_ori); // "////xxx/yyy" => "xxx/yyy"

	size_t len1 = strnlen(path1, MAXLINE);
	size_t len2 = strnlen(path2, MAXLINE);
	if (!len1 && !len2) {
		free(path1);
		free(path2);
		return cmp_cp_str(""); // "" + "" => ""
	} else if (!len1) {
		char *path_ret = cmp_cp_str(path2);
		free(path1);
		free(path2);
		return path_ret; // "" + "xxx" => "xxx"
	} else if (!len2) {
		char *path_ret = cmp_cp_str(path1);
		free(path1);
		free(path2);
		return path_ret; // "xxx" + "" => "xxx"
	} else if (1 == len1 && '/' == path1[0] && 1 == len2 && '/' == path2[0]) { // "/" + "/" => "/"
		free(path1);
		free(path2);
		return cmp_cp_str("/");
	}
	size_t len = len1 + 1 + len2 + 1;
	char *joined = malloc(sizeof(char)*len);
	char *infix = "/";
	if ('/' == path1[len1 - 1] || '/' == path2[0]) {
		infix = "";
	}
	snprintf(joined, len, "%s%s%s", path1, infix, path2);
	free(path1);
	free(path2);
	return joined;
}

char *cmp_path_join_many_worker(const char *path1, ...) {
	char *result = cmp_cp_str(path1);
	va_list ap;
	va_start(ap, path1);
	while (1) {
		const char* path2 = va_arg(ap, const char*);
		if (!path2) {
			break;
		}
		char *result_old = result;
		char *path2_cp = cmp_cp_str(path2);
		result = cmp_path_join(result, path2_cp);
		free(result_old);
		free(path2_cp);
	}
	va_end(ap);

	return result;
}
