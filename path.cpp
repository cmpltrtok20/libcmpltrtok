#include <regex>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include "cmpltrtok.h"

using namespace std;

int cmp_get_prefix_from_path(const char* path, const char* ext, int force, char **ppchar) {
	if (!ppchar) {
		return 1;
	}
	if (!path || !path[0]) {
		*ppchar = cmp_cp_str("");
		return 1;
	}

	regex *pat = 0;
	string path_str{path};

	if (ext && !ext[0]) { // ext == ""
		*ppchar = cmp_cp_str(path);
		return 1;
	} else if (!ext) { // ext == 0
		pat = new regex(R"""((.+)\.[^\.]+)""");
	} else {
		ostringstream oss;
		oss << R"""(^(.+))""";
		char *ext_escape = cmp_escape_regex(ext);
		oss << ext_escape;
		free(ext_escape);
		oss << R"""($)""";
		pat = new regex(oss.str());
	}

	smatch matches;
	int res = regex_search(path_str, matches, *pat);
	if (!res) {
		if (force) {
			cerr << "Path \"" << path << "\" is not right" << endl;
			return 0;
		} else {
			*ppchar = cmp_cp_str(path);
			return 1;
		}
	}
	string match(matches[1]);
	*ppchar = cmp_cp_str(match.c_str());
	return 1;
}

char *cmp_get_path_from_prefix(const char* prefix, const char* ext) {
	if (!prefix || !prefix[0]) {
		return cmp_cp_str("");
	}
	if (!ext || !ext[0]) {
		return cmp_cp_str(prefix);
	}
	ostringstream oss;
	oss << prefix << ext;
	return cmp_cp_str(oss.str().c_str());
}
