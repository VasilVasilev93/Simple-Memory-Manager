#include "MemoryManager.h"

void *base = 0x0;

//get block from given address
t_block get_block(void *p){
    // t_block b;
    // int offset = (int) (((char*)b->data) - ((char*) b));
    return (t_block) (((char*) p) - OFFSET);
}

//valid address is allocated by malloc for free
bool valid_addr(void *p){
    if(base){
        if(p > base && p < sbrk(0)){
            return (p == (get_block(p))->ptr);
        }
    }

    return false;
}

//find block with needed size
t_block find_block(t_block *last, size_t size){
    t_block block = (t_block) base;
    while(block && !(block->block_free && block->size >= size)){
        *last = block;
        block = block->next;
    }

    return(block);
}

//extends the allocated memory for the program
t_block extend_heap(t_block last, size_t size){
    int debug_block;
    t_block block;
    block = (t_block) sbrk(0);
    debug_block = *((int*) sbrk(OFFSET + size));

    //if(sbrk(OFFSET + size) == (void*)-1){
    if(debug_block < 0){
        return 0x0;
    }

    block->size = size;
    block->next = 0x0;
    block->prev = last;
    block->ptr = block->data;

    if(last){
        last->next = block;
    }

    block->block_free = 0;

    return block;
}

//splits block
void split_block(t_block block, size_t size){
    t_block newBlock;
    newBlock = (t_block) (block->data + size);
    newBlock->size = block->size - size - OFFSET;
    newBlock->next = block->next;
    newBlock->prev = block;
    newBlock->block_free = 1;
    newBlock->ptr = newBlock->data;
    block->size = size;
    block->next = newBlock;

    //if the block has allocated block next to him, restore the connection after splitting.
    if(newBlock->next){
        newBlock->next->prev = newBlock;
    }
}

//allocate memory
void *malloc(size_t size){
    t_block newBlock, last;
    size_t newSize = align8(size);

    if(base){
        last = (t_block) base;
        newBlock = find_block(&last, newSize);
        if(newBlock){
            //can we split
            if((newBlock->size - newSize) >= (OFFSET + PTR_SIZE)){
                split_block(newBlock, newSize);
            }

            newBlock->block_free = false;
        } else {
            //Can't find block, try extending the heap
            newBlock = extend_heap(last, newSize);
            if(!newBlock){
                //cannot extend heap
                return 0x0;
            }
        } 
    } else {
        //first call
        newBlock = extend_heap(0x0, newSize);
        if(!newBlock){
            //failed to extend
            return 0x0;
        }
        base = newBlock;
    }

    return newBlock->data;
}

//merge 2 blocks into 1
t_block merge(t_block block){
    if(block->next && block->next->block_free){
        block->size += OFFSET + block->next->size;
        block->next = block->next->next;

        if(block->next){
            block->next->prev = block;
        }

        return block;
    }
}

//frees dynamic allocated memory
void free(void *p){
    t_block block;
    if(valid_addr(p)){
        block = get_block(p);
        block->block_free = 1;
        //merge with previous if possible
        if(block->prev && block->prev->block_free){
            block = merge(block->prev);
        }
        //merge with next if possible
        if(block->next){
            if(block->next->block_free){
                merge(block);
            }
        } else {
            //free the end of the heap
            if(block->prev){
                //block->prev->next = 0x0;
            } else {
                // brk(block);
                // base = 0x0;
            }

        }

        //no more blocks
        if(!block->next && !block->prev){
            brk(block);
            base = 0x0;
        }
    }
}