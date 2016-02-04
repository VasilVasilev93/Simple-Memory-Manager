#include "MemoryManager.h"

int main(){
    int *p1 = (int *) malloc(100 * sizeof(int));
    int *p2 = (int *) malloc(100 * sizeof(int));
    int *p3 = (int *) malloc(100 * sizeof(int));
    int *p4 = (int *) malloc(100 * sizeof(int));

    free(p3);

    int *p5 = (int *) malloc(2 * sizeof(int));
    int *p6 = (int *) malloc(10 * sizeof(int));
    free(p5);
    free(p6);
    free(p4);
    free(p1);
    free(p2);

    return 0;
}