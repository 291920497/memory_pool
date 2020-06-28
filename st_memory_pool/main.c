#include <stdio.h>
#include "st_memory_pool.h"
#include <stdlib.h>

int main() {

	/*st_memory_pool_create();

	for (int i = 0; i < 512; ++i) {
		st_malloc(8192);
		st_malloc(8000);
	}

	st_memory_pool_destroy();*/


	void* ptr_arr[1024];
	for (int i = 0; i < 512; ++i) {
		ptr_arr[i * 2] = malloc(8192);
		ptr_arr[i * 2 + 1] = malloc(8000);
	}

	for (int i = 0; i < 1024; ++i) {
		free(ptr_arr[i]);
	}
	return 0;
}