#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>

// This macro is actually in the size of single MB
#define MEMORY_POOL_SIZE 1024 * 1024

static char memory_pool[MEMORY_POOL_SIZE];
static void *free_list;

typedef struct BlockHeader {
    size_t size;
    struct BlockHeader *next;
} BlockHeader;



void memory_init(){
    free_list = memory_pool;
    BlockHeader *initial_block = (BlockHeader *)free_list;
    initial_block->size = MEMORY_POOL_SIZE * sizeof(BlockHeader);
    initial_block-> next = NULL;
}

void *memory_alloc(size_t size){
    BlockHeader *prev = NULL;
    BlockHeader *current = (BlockHeader *)free_list;

    while(current != NULL){
        if(current->size >= size){
            if(current->size > size + sizeof(BlockHeader)){
                BlockHeader *new_block = (BlockHeader*)((char*)current + sizeof(BlockHeader) + size);
                new_block->next = current->next;
                current->size = size;
                current->next = new_block;
            }

            if(prev != NULL){
                prev->next = current->next;
            }else{
                free_list = current->next;
            }

            return (char*)current + sizeof(BlockHeader);
        }

        prev = current;
        current = current->new_next;
    }    
    return NULL;

}


void memory_free(void *ptr){
    if(ptr == NULL){
        return;
    }

    BlockHeader *block = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    block->next = (BlockHeader*)free_list;
    free_list = block;
}
