#include <unistd.h>
#include <assert.h>

void* cmp_glb_init_brk = 0;

void cmp_set_init_brk() {
	cmp_glb_init_brk = sbrk(0);
}

int cmp_is_point_to_heap(void *ptr) {
	assert(cmp_glb_init_brk);
	void* cur_brk = sbrk(0);
	int res = (cmp_glb_init_brk <= ptr) && (ptr <= cur_brk);
}
