#include <regex>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include "cmpltrtok.h"

using namespace std;

char *cmp_str_replace_all(const char* search, const char* replace, const char* subject) {
	string search_str{search}, replace_str{replace}, subject_str{subject};
	size_t pos = 0;
	while (1) {
		pos = search_str.find(subject_str, pos);
#ifdef TRACE
		cout << "|" << search_str << "|, pos=" << pos << ", sub=|" << subject_str << "|" << endl;
#endif
		if (pos == string::npos) {
			break;
		}
		search_str.replace(pos, subject_str.size(), replace_str);
		pos += replace_str.size();
	}
	return cmp_cp_str(search_str.c_str());
}

//escape: . ? * + [ ] ( ) < > | \_
char *cmp_escape_regex(const char* search) {
	if (!search || !search[0]) {
		return cmp_cp_str("");
	}
	string search_str{search};
	string specials_str{R"""(:.?*+[]()<>|\)"""};
	string quote_str("\1");
	for (int i = 0, ln = specials_str.size(); i < ln ; ++i) {
		string special_char_str = specials_str.substr(i, 1);
		ostringstream oss;
		oss << quote_str << special_char_str;
		string oss_str = oss.str();

		size_t pos = 0;
		char *replaced = cmp_str_replace_all(search_str.c_str(), oss_str.c_str(), special_char_str.c_str());
		string new_search_str{replaced};
		search_str = new_search_str;
		free(replaced);
	}

	char *replaced_final = cmp_str_replace_all(search_str.c_str(), "\\", quote_str.c_str());
	string new_search_str_final{replaced_final};
	free(replaced_final);
	return cmp_cp_str(new_search_str_final.c_str());
}
