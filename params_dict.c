#include <stdlib.h>
#include <stdio.h>
#include "cmpltrtok.h"

void params_dict_val_free(PARAMS_DICT_VAL *pdict_val) {
	if (pdict_val->type == PARAMS_DICT_TYPE_S) {
		free(pdict_val->data.str_val);
	}
	free(pdict_val);
}

int params_dict_init(PARAMS_DICT *pdict, size_t max_size) {
	pdict->max_size = max_size;
	pdict->size = 0;
	pdict->pdict = calloc(1, sizeof(struct hsearch_data));
	pdict->plist_head = calloc(1, sizeof(struct PARAMS_DICT_KEY_TAILQ_HEAD));
	if(!hcreate_r(max_size, pdict->pdict)) {
		return PARAMS_DICT_ERROR_CANNOT_INIT_VAL_DICT;
	}
	TAILQ_INIT(pdict->plist_head);
	return PARAMS_DICT_ERROR_NONE;
}

void params_dict_destroy(PARAMS_DICT *pdict) {
	struct PARAMS_DICT_KEY_TAILQ_ENTRY *np = 0, *n1 = 0, *n2 = 0;

	// destroy the type dict and the val dict
	TAILQ_FOREACH(np, pdict->plist_head, entries) {
		ENTRY search_entry;
		search_entry.key = np->data;
		ENTRY *pentry = 0;
		if (hsearch_r(search_entry, FIND, &pentry, pdict->pdict)) {
			free(pentry->key);
			free(pentry->data);
		}
	}
	hdestroy_r(pdict->pdict);

	// destroy list
	n1 = TAILQ_FIRST(pdict->plist_head);
	while (n1 != NULL) {
		n2 = TAILQ_NEXT(n1, entries);
		free(n1->data);
		free(n1);
		n1 = n2;
	}
	TAILQ_INIT(pdict->plist_head);

	// free ptr
	free(pdict->pdict);
	free(pdict->plist_head);
}

int params_dict_key_tailq_clone(struct PARAMS_DICT_KEY_TAILQ_HEAD *phead_src, struct PARAMS_DICT_KEY_TAILQ_HEAD *phead_dest) {
	struct PARAMS_DICT_KEY_TAILQ_ENTRY *np = 0, *np_dest = 0;
	int src_cnt = 0;
	TAILQ_FOREACH(np, phead_src, entries) {
		++src_cnt;
		np_dest = malloc(sizeof(struct PARAMS_DICT_KEY_TAILQ_ENTRY));
		np_dest->data = cmp_cp_str(np->data);
		TAILQ_INSERT_TAIL(phead_dest, np_dest, entries);
	}
	int dest_cnt = 0;
	TAILQ_FOREACH(np, phead_dest, entries) {
		++dest_cnt;
	}

	if (dest_cnt != src_cnt) {
		return PARAMS_DICT_ERROR_LIST_CLONE_FAILED;
	} else {
		return PARAMS_DICT_ERROR_NONE;
	}
}

int params_dict_callback_print(PARAMS_DICT *pdict, char* key, PARAMS_DICT_VAL *pval, void* ref_data) {
	int type = pval->type;
	PARAMS_DICT_VAL_UNION data = pval->data;
	char *repr = 0;
	switch (type) {
	case PARAMS_DICT_TYPE_I:
		printf("%s: (%c) |%d|\n", key, type, data.int_val);
		break;
	case PARAMS_DICT_TYPE_F:
		printf("%s: (%c) |%lf|\n", key, type, data.float_val);
		break;
	case PARAMS_DICT_TYPE_S:
		printf("%s: (%c) |%s|\n", key, type, data.str_val);
		break;
	case PARAMS_DICT_TYPE_L:
		repr = cmp_milli2printable(data.long_val, 1);
		printf("%s: (%c) |%lld| (%s)\n", key, type, data.long_val, repr);
		free(repr);
		break;
	case PARAMS_DICT_TYPE_N:
		printf("%s: (%c)\n", key, type);
		break;
	default:
		return PARAMS_DICT_ERROR_IVALID_TYPE;
		break;
	}
	return PARAMS_DICT_ERROR_NONE;
}

int params_dict_walk_through(PARAMS_DICT *pdict, PARAMS_DICT_CALLBACK *callback, void* ref_data) {
	struct PARAMS_DICT_KEY_TAILQ_ENTRY *np = 0;
	char *key;
	ENTRY search_item, *pentry;
	PARAMS_DICT_VAL *pval;
	TAILQ_FOREACH(np, pdict->plist_head, entries) {
		key = np->data;

		// make key of search item
		search_item.key = key;

		// search
		if (!hsearch_r(search_item, FIND, &pentry, pdict->pdict)) {
			// not found in val dict
			return PARAMS_DICT_ERROR_DATA_CORRUPTED;
		}
		pval = (PARAMS_DICT_VAL*)pentry->data;

		// run callback
		int ret = callback(pdict, key, pval, ref_data);
		if (ret) {
			return ret;
		}
	}
	return PARAMS_DICT_ERROR_NONE;
}

int params_dict_print(PARAMS_DICT *pdict) {
	int ret = params_dict_walk_through(pdict, params_dict_callback_print, 0);
	return ret;
}

int params_dict_clone(PARAMS_DICT *psrc, PARAMS_DICT *pdest) {
	// init dest
	int ret = params_dict_init(pdest, psrc->max_size);
	if (ret) {
		return ret;
	}

	// iterate src and upsert into dest
	struct PARAMS_DICT_KEY_TAILQ_ENTRY *np = 0;
	char *key;
	ENTRY search_item, *pentry;
	PARAMS_DICT_VAL *pval;
	TAILQ_FOREACH(np, psrc->plist_head, entries) {
		key = np->data;

		// make keys of entries of type dict and val dict
		search_item.key = key;

		// search in val dict to get val
		if (!hsearch_r(search_item, FIND, &pentry, psrc->pdict)) {
			// not found in val dict
			return PARAMS_DICT_ERROR_DATA_CORRUPTED;
		}
		PARAMS_DICT_VAL *pdict_val = (PARAMS_DICT_VAL *)pentry->data;

		// upsert it
		ret = params_dict_upsert(pdest, key, pdict_val->type, &pdict_val->data);
		if (ret) {
			return ret;
		}
	}

	// all OK
	return PARAMS_DICT_ERROR_NONE;
}


int params_dict_cp_val(int type, void *pval, PARAMS_DICT_VAL **ppdict_val) {
	*ppdict_val = malloc(sizeof(PARAMS_DICT_VAL));
	(*ppdict_val)->type = type;
	switch (type) {
	case PARAMS_DICT_TYPE_I:
		(*ppdict_val)->data.int_val = *((int*)pval);
		break;
	case PARAMS_DICT_TYPE_F:
		(*ppdict_val)->data.float_val = *((double*)pval);
		break;
	case PARAMS_DICT_TYPE_S:
		(*ppdict_val)->data.str_val = cmp_cp_str(*((char**)pval));
		break;
	case PARAMS_DICT_TYPE_L:
		(*ppdict_val)->data.long_val = *((long long*)pval);
		break;
	case PARAMS_DICT_TYPE_N:
		// do nothing for null value
		break;
	default:
		return PARAMS_DICT_ERROR_IVALID_TYPE;
		break;
	}
	return PARAMS_DICT_ERROR_NONE;
}

PARAMS_DICT_VAL *params_dict_get(PARAMS_DICT *pdict, char *key) {
	struct hsearch_data *pval_dict = pdict->pdict;
	ENTRY search_item, *pentry;

	// make key of search item
	search_item.key = cmp_cp_str(key);

	// search in dict
	if (!hsearch_r(search_item, FIND, &pentry, pval_dict)) {
		// not found in dict
		free(search_item.key);
		return 0;
	}
	free(search_item.key);
	return (PARAMS_DICT_VAL *)pentry->data;
}

int params_dict_upsert(
		PARAMS_DICT *pdict,
		char *key,
		int type,
		void *pval
) {
	struct hsearch_data *pval_dict = pdict->pdict;
	struct PARAMS_DICT_KEY_TAILQ_HEAD *plist_head = pdict->plist_head;
	ENTRY search_item, *pentry;

	// make key of search item
	search_item.key = cmp_cp_str(key);

	// copy value
	PARAMS_DICT_VAL *pdict_val = 0;
	int ret = params_dict_cp_val(type, pval, &pdict_val);
	if (ret) {
		free(search_item.key);
		return ret;
	}
	search_item.data = pdict_val;

	// search in dict
	if (!hsearch_r(search_item, FIND, &pentry, pval_dict)) {
		// not found in dict, do insertion

		// if we meet the max_size
		if (pdict->size >= pdict->max_size) {
			free(search_item.key);
			params_dict_val_free(pdict_val);
			return PARAMS_DICT_ERROR_DATA_FULL;
		}

		// add to dict
		if (!hsearch_r(search_item, ENTER, &pentry, pval_dict)) {
			// cannot add to val dict
			free(search_item.key);
			params_dict_val_free(pdict_val);
			return PARAMS_DICT_ERROR_ADD_TO_VAL_DICT_FAILED;
		}

		// record the new key to list
		struct PARAMS_DICT_KEY_TAILQ_ENTRY *np = malloc(sizeof(struct PARAMS_DICT_KEY_TAILQ_ENTRY));
		np->data = cmp_cp_str(key);
		TAILQ_INSERT_TAIL(plist_head, np, entries);
		pdict->size += 1;

	} else {
		// found in type dict, do update

		// free string in item for searching
		free(search_item.key);

		// update val dict
		params_dict_val_free((PARAMS_DICT_VAL*)pentry->data);
		pentry->data = pdict_val;
	}
	return PARAMS_DICT_ERROR_NONE;
}
