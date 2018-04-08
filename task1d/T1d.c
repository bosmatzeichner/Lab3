#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, char** argv) {
    int iarray[] = {0x01234567, 0x89ABCDEF, 0x13579BDF};
    char carray[] = {'a', 'b', 'c'};
    int *iarrayPtr;
    char *carrayPtr;

    iarrayPtr = iarray;
    carrayPtr = carray;
    int i = 0;
    while (i < 3) {
        printf(" iarray: %x\n",*iarrayPtr);
        i++;
        ++iarrayPtr;
    }
    i=0;
    while (i < 3) {
        printf(" iarray: %c\n",*carrayPtr);
        i++;
        ++carrayPtr;
    }

    //int *p;
    //printf("%d",p);


}
