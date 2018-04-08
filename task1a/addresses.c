#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5; //global var
int addr6; //global var

int foo(); // func pointer
void point_at(void *p);

int main (int argc, char** argv){
    int addr2; //local var
    int addr3;
    char* yos="ree";
    int * addr4 = (int*)(malloc(50)); //heap
    printf("- &addr2: %p\n",&addr2);
    printf("- &addr3: %p\n",&addr3);
    printf("- foo: %p\n",foo);
    printf("- &addr5: %p\n",&addr5);

    point_at(&addr5);

    printf("- &addr6: %p\n",&addr6);
    printf("- yos: %p\n",yos);
    printf("- addr4: %p\n",addr4);
    printf("- &addr4: %p\n",&addr4);
    return 0;
}

int foo(){
    return -1;
}

void point_at(void *p){
    int local;
    static int addr0 = 2;
    static int addr1;


    long dist1 = (size_t)&addr6 - (size_t)p; //addr6 pushed to stack before 5
    long dist2 = (size_t)&local - (size_t)p; //local pushed in after
    long dist3 = (size_t)&foo - (size_t)p; // the function pushed before

    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);

    printf("- addr0: %p\n", & addr0);
    printf("- addr1: %p\n",&addr1);
}

