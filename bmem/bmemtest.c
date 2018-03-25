#include "bmem.h"

int main(int argc, char **argv)
{
	int result;
	void *pool_1;
	size_t pool_1_size, pool_1_chunk;
	size_t size;
	uint8_t *ptr;

    butil_set_log_level(5);

	result = bmem_init();
	if (result)
	{
		fprintf(stderr, "Can't init pool\n");
		return -1;
	}
	pool_1_size = 0x100000;
	pool_1_chunk = 1024;
	pool_1 = (void*)malloc(pool_1_size);
	if (! pool_1)
	{
		fprintf(stderr, "Can't alloc pool 1\n");
		return -1;
	}
	result = bmem_add_pool(pool_1, pool_1_size, pool_1_chunk);

	size = pool_1_chunk;
	ptr = bmem_alloc(size);
	bmem_free(ptr);

	size = pool_1_chunk + 1;
	ptr = bmem_alloc(size);
	bmem_free(ptr);

	bmem_stats();

	result = bmem_deinit();
	if (result)
	{
		fprintf(stderr, "Can't deinit pool\n");
		return -1;
	}
	return 0;
}
