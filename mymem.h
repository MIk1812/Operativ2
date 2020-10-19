#include <stdio.h>

#define TRUE 1

typedef struct node memoryNode;
struct node{
    struct node* right;
    struct node* left;
    size_t size;
    char alloc;
    void* memoryAddress;
};

typedef enum strategies_enum strategies;
enum strategies_enum{
    NotSet = 0,
    Best = 1,
    Worst = 2,
    First = 3,
    Next = 4
};

void initmem(strategies strategy, size_t sz);
void* mymalloc(size_t requested);
void myfree(void* block);

int mem_holes();
int mem_allocated();
int mem_free();
int mem_largest_free();
int mem_small_free(int size);
char mem_is_alloc(void *ptr);

void* mem_pool();
int mem_total();
char *strategy_name(strategies strategy);
strategies strategyFromString(char * strategy);
void try_mymem(int argc, char **argv);


