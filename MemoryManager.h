#ifndef _MEMORY_MANAGER_H
#define _MEMORY_MANAGER_H

#include <stdio.h>
#include <unistd.h>

/* block struct */
struct s_block {
    size_t          size;
    struct s_block *next;
    struct s_block *prev;
    int            block_free;
    void           *ptr;
    /* A pointer to the allocated block */
    char            data[1];
};

#define OFFSET ((int) 40)

#define PTR_SIZE (sizeof(void*))
#define align8(x) (((((x)-1)>>3) << 3) + PTR_SIZE)

typedef struct s_block *t_block;

//get block from given address
t_block get_block(void *p);

//valid address is allocated by malloc for free
bool valid_addr(void *p);

t_block find_block(t_block *last, size_t size);

t_block extend_heap(t_block last, size_t size);

void split_block(t_block block, size_t size);

void *malloc(size_t size);

t_block merge(t_block block);

void free(void *p);

#endif