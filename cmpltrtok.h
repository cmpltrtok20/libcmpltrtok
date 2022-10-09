#ifndef CMPLTRTOK_H_
#define CMPLTRTOK_H_

#include <stddef.h>
#include <sys/queue.h>
#include <search.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAXLINE 2048
#define MAXWORD 256

/* log.c */
void cmp_title(const char* content);
void cmp_newline(int n);

/* math.c */
struct UINT_TAILQ_ENTRY {
	unsigned int data;
	TAILQ_ENTRY(UINT_TAILQ_ENTRY) entries;
};

TAILQ_HEAD(UINT_TAILQ_HEAD, UINT_TAILQ_ENTRY);

int cmp_argmax(float* vals, int n);

unsigned int* cmp_rand_perm(unsigned int range, unsigned int seed);

/* other.c */
int *cmp_cp_int(int val);

/* params_dict */
#define PARAMS_DICT_TYPE_I ((int)'I')
#define PARAMS_DICT_TYPE_F ((int)'F')
#define PARAMS_DICT_TYPE_S ((int)'S')
#define PARAMS_DICT_TYPE_L ((int)'L')
#define PARAMS_DICT_TYPE_N ((int)'N')

#define PARAMS_DICT_ERROR_BASE_NO 100
#define PARAMS_DICT_ERROR_NONE 0
#define PARAMS_DICT_ERROR_IVALID_TYPE (1 + PARAMS_DICT_ERROR_BASE_NO)
#define PARAMS_DICT_ERROR_ADD_TO_TYPE_DICT_FAILED (2 + PARAMS_DICT_ERROR_BASE_NO)
#define PARAMS_DICT_ERROR_ADD_TO_VAL_DICT_FAILED (3 + PARAMS_DICT_ERROR_BASE_NO)
#define PARAMS_DICT_ERROR_DATA_CORRUPTED (4 + PARAMS_DICT_ERROR_BASE_NO)
#define PARAMS_DICT_ERROR_CANNOT_INIT_TYPE_DICT (5 + PARAMS_DICT_ERROR_BASE_NO)
#define PARAMS_DICT_ERROR_CANNOT_INIT_VAL_DICT (6 + PARAMS_DICT_ERROR_BASE_NO)
#define PARAMS_DICT_ERROR_DATA_FULL (7 + PARAMS_DICT_ERROR_BASE_NO)
#define PARAMS_DICT_ERROR_LIST_CLONE_FAILED (8 + PARAMS_DICT_ERROR_BASE_NO)
#define PARAMS_DICT_ERROR_CANNOT_APPEND_BSON (9 + PARAMS_DICT_ERROR_BASE_NO)

struct PARAMS_DICT_KEY_TAILQ_ENTRY {
	char* data;
	TAILQ_ENTRY(PARAMS_DICT_KEY_TAILQ_ENTRY) entries;
};

TAILQ_HEAD(PARAMS_DICT_KEY_TAILQ_HEAD, PARAMS_DICT_KEY_TAILQ_ENTRY);

typedef union PARAMS_DICT_VAL_UNION {
	int int_val;
	double float_val;
	char *str_val;
	long long long_val;
} PARAMS_DICT_VAL_UNION;

typedef struct PARAMS_DICT_VAL {
	int type;
	PARAMS_DICT_VAL_UNION data;
} PARAMS_DICT_VAL;

typedef struct PARAMS_DICT {
	struct hsearch_data *pdict;
	struct PARAMS_DICT_KEY_TAILQ_HEAD *plist_head;
	int size;
	int max_size;
} PARAMS_DICT;

typedef int PARAMS_DICT_CALLBACK(PARAMS_DICT *pdict, char* key, PARAMS_DICT_VAL* pval, void* ref_data);

void params_dict_val_free(PARAMS_DICT_VAL *pdict_val);
int params_dict_init(PARAMS_DICT *pdict, size_t max_size);
void params_dict_destroy(PARAMS_DICT *pdict);

int params_dict_key_tailq_clone(struct PARAMS_DICT_KEY_TAILQ_HEAD *phead_src, struct PARAMS_DICT_KEY_TAILQ_HEAD *phead_dest);
int params_dict_clone(PARAMS_DICT *psrc, PARAMS_DICT *pdest);

int params_dict_callback_print(PARAMS_DICT *pdict, char* key, PARAMS_DICT_VAL *pval, void *ref_data);
int params_dict_walk_through(PARAMS_DICT *pdict, PARAMS_DICT_CALLBACK *callback, void* ref_data);
int params_dict_print(PARAMS_DICT *pdict);

int params_dict_cp_val(int type, void *pval, PARAMS_DICT_VAL **ppval_cp);

PARAMS_DICT_VAL *params_dict_get(PARAMS_DICT *pdict, char *key);
int params_dict_upsert(
		PARAMS_DICT *pdict,
		char *key,
		int type,
		void *pval
);
#define params_dict_upsert_macro(...) if (1) {\
	int ret = params_dict_upsert(__VA_ARGS__);\
	if (ret) {\
		return ret;\
	}\
}

/* path */
int is_path_fully_provided(const char *path);
int cmp_mkdir_p(const char *dir, mode_t mode);
int cmp_mkdir_if_not_existed(const char *dir_path, mode_t mode);
int cmp_dir_exists(const char *dir_path);
char *cmp_get_base_main_name(const char *path);
char *cmp_get_dir(const char *path);
char *cmp_path_split(const char *path, char *dir_buf, const size_t n);
char *cmp_path_split_ext(const char *path, char *main_buf, const size_t n);
char *cmp_path_get_cousin(char *path, char *uncle_name, char *cousin_ext);
int cmp_path_is_label(char *filepath, char *label);
char *cmp_path_remove_tail_slash(const char *path);
char *cmp_path_remove_head_slash(const char *path);
char *cmp_path_join(const char *path1, const char *path2);
char *cmp_path_join_many_worker(const char *path1, ...);
#define cmp_path_join_many(path1, ...) cmp_path_join_many_worker(path1, __VA_ARGS__, (void *) 0)

/* pointer.c */
extern void* cmp_glb_init_brk;
void cmp_set_init_brk();
int cmp_is_point_to_heap(void *ptr);

/* string.c */
char *cmp_cp_str(const char* src);
char *cmp_substr(const char *inpStr, char *outStr, const size_t startPos, const size_t strLen);
char *cmp_substr_with_alloc(const char *inpStr, const size_t startPos, const size_t strLen);
void cmp_strncpy(char *dest, const char *src, const size_t n);
char *cmp_strncat_worker(const char *dest, const size_t n, ...);
#define cmp_strncat(dest, n, ...) cmp_strncat_worker(dest, n, __VA_ARGS__, (void *) 0)

/* time.c */
long long cmp_current_timestamp();
char *cmp_milli2printable(long long milliseconds, int is_utc);

/* util macros */
#define CMP_INIT cmp_set_init_brk()

#ifdef __cplusplus
}
#endif

#endif /* CMPLTRTOK_H_ */
