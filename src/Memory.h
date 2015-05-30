#ifndef MEMORY_H
#define MEMORY_H

#include "Types.h"

void kmem_init();
void * kmalloc(int32_t);
void * krealloc(void * , int32_t );
void memcpy(void *, void *, int32_t);
void memset(void *, int32_t, int32_t);
RESULT kfree(void * );


#endif // MEMORY_H
