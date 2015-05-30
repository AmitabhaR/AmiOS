#include "Memory.h"
#include "Display.h"

struct Page
{
	void * block_ptr;
	int32_t tot_bytes;
	unsigned free : 1;
};

// Protected Data.
static byte physical_memory[1024];
static struct Page mem_blocks[1024];
static int32_t block_cntr = 0;

void kmem_init()
{
	memset(physical_memory, 0, 1024 * sizeof(byte));
	memset(mem_blocks, 0, sizeof(struct Page) * 1024);
}

static void deletePage(int index)
{
	int32_t cnt = 0;
	bool hasDeleted = FALSE;

	for (; cnt < block_cntr; cnt++)
	{
		if (cnt == index)
		{
			if (cnt + 1 < block_cntr) mem_blocks[cnt] = mem_blocks[cnt + 1];
			hasDeleted = TRUE;
		}
		else
		{
			if (hasDeleted) mem_blocks[cnt] = mem_blocks[cnt + 1];
		}
	}

	block_cntr--;
}

static int getBytesAllocated()
{
	int32_t cnt = 0;
	int32_t size = 0;

	for (; cnt < block_cntr; cnt++)
	{
		size += mem_blocks[cnt].tot_bytes;
	}

	return size;
}

void * kmalloc(int32_t tot_size)
{
	int32_t cnt = 0;
	bool is_allFree = TRUE;

	// Check if all the memory are free.
	for (; cnt < block_cntr; cnt++)
	{
		if (!mem_blocks[cnt].free)
		{
			is_allFree = FALSE;
			break;
		}
	}

	if (is_allFree)
	{
		memset(physical_memory, 0, getBytesAllocated());
		memset(mem_blocks, 0, sizeof(struct Page) * block_cntr);
		block_cntr = 0;
	}

	for (cnt = 0; cnt < block_cntr; block_cntr++) // Look for free blocks.
	{
		if (mem_blocks[cnt].free)
		{
			if (mem_blocks[cnt].tot_bytes > tot_size)
			{
				if (block_cntr < 1024) // Due to limited allocations.
				{
					mem_blocks[block_cntr].tot_bytes = mem_blocks[cnt].tot_bytes - tot_size;
					mem_blocks[block_cntr].block_ptr = (byte *) mem_blocks[cnt].block_ptr + tot_size;
					mem_blocks[block_cntr].free = TRUE;
					mem_blocks[cnt].tot_bytes = tot_size;
					block_cntr++;
				}

				mem_blocks[cnt].free = FALSE;

				return mem_blocks[cnt].block_ptr;
			}
			else if (mem_blocks[cnt].tot_bytes == tot_size)
			{
				return mem_blocks[cnt].block_ptr;
			}
			else
			// This part is checking and mergeing of free blocks.
			{
				int32_t c = cnt + 1;
				int32_t req_bytes = tot_size;

				for (; c < block_cntr; c++)
				{
					if (mem_blocks[c].free)
					{
						if (mem_blocks[c].tot_bytes < req_bytes)
						{
							req_bytes -= mem_blocks[c].tot_bytes;
						}
						else if (mem_blocks[c].tot_bytes > req_bytes)
						{
							if (block_cntr < 1024) // Due to limited allocations.
							{
								mem_blocks[block_cntr].tot_bytes = mem_blocks[c].tot_bytes - req_bytes;
								mem_blocks[block_cntr].block_ptr = (byte *)mem_blocks[c].block_ptr + req_bytes;
								mem_blocks[block_cntr].free = TRUE;
								mem_blocks[c].tot_bytes = req_bytes;
								block_cntr++;
							}

							break;
						}
						else
						{
							req_bytes = 0;
							break;
						}
					}
					else
					{
						break;
					}
				}

				if (req_bytes == 0)
				{
					void * ptr = mem_blocks[cnt].block_ptr;
					int32_t cntr = cnt + 1;

					mem_blocks[cnt].tot_bytes = tot_size;
					mem_blocks[cnt].free = FALSE;

					// Free all merged pages allocations.
					for (; cntr <= c; cntr++)
					{
						deletePage(cntr);
					}

					return ptr;
				}
			}
		}
	}

	if (block_cntr < 1024 && 1024 - getBytesAllocated() >= tot_size)
	{
		mem_blocks[block_cntr].block_ptr = &physical_memory[getBytesAllocated()];
		mem_blocks[block_cntr].tot_bytes = tot_size;
		mem_blocks[block_cntr].free = FALSE;

		return mem_blocks[block_cntr].block_ptr;
	}
	else
	{
		// Panic , memory allocation failed.
		printf("Kernel Panic : Out of dynamic memory !", 0);
		printf("System Crash : ID - 0x36ff",0);

		__asm
		{
			cli;
			hlt;
		};
	}
}

void * krealloc(void * ptr,int32_t tot_size)
{
	void * ret_ptr = kmalloc(tot_size);

	memcpy(ret_ptr, ptr, tot_size);

	kfree(ptr);

	return ret_ptr;
}

void memcpy(void * dest, void * src, int32_t tot_size)
{
	char * dst = (char *)dest;
	char * source = (char *)src;
	int32_t cntr = 1;

	for (; cntr <= tot_size; cntr++)
	{
		dst[cntr - 1] = source[cntr - 1];
	}
}

void memset(void * src,int32_t value , int32_t tot_size)
{
	char * source = (char *)src;
	int32_t cntr = 1;

	for (; cntr <= tot_size; cntr++)
	{
		source[cntr - 1] = value;
	}
}

RESULT kfree(void * ptr)
{
	int32_t cnt = 0;

	for (; cnt < block_cntr; cnt++)
	{
		if (mem_blocks[cnt].block_ptr == ptr)
		{
			mem_blocks[cnt].free = TRUE;
			return TRUE;
		}
	}
	
	return FALSE;		
}
