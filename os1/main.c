#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "mmemory.h"

void test_malloc_without_init() {
    VA *p1 = (VA*) malloc(sizeof(VA));
    assert(_malloc(p1, 5) == 1);
}
void test_malloc_negative_size_block(){
    VA *p1 = (VA*) malloc(sizeof(VA));
    _init(1, 40);
    assert(_malloc(p1, -1) == -1);
}
void test_malloc_size_block_more_than_real_block_size(){
    VA *p1 = (VA*) malloc(sizeof(VA));
    _init(1,20);
    assert(_malloc(p1, 30) == -2);
}
void test_malloc_used_block(){
    VA *p1 = (VA*) malloc(sizeof(VA));
    _init(1,40);
    _malloc(p1, 20);
    _malloc(p1, 20);
    assert(_malloc(p1, 20) == -2);
}
void test_malloc_size_block_equals_real_block_size(){
    VA *p1 = (VA*) malloc(sizeof(VA));
    _init(1,50);
    assert(_malloc(p1, 50) == 0);
}
void test_malloc_size_block_less_than_real_block_size(){}


int test_all(){
    char c='1';
    VA *p1 = (VA*) malloc(sizeof(VA));
    VA p2 = (VA) malloc(sizeof(VA));
    unsigned int blSize, adress;
    char *userBuff =(char*) calloc(1,50);
    int retCode;
    *p1=p2;
    _init(1,40);

    while(c!='0'){
        printf("0.Exit\n1.malloc\n2.free\n3.read\n4.write\n5.CLS\n");
        scanf("%c", &c);
        if(c=='0') return 0;
        if(c=='1'){
            printf("\tenter block size for malloc\n");
            scanf("%d",&blSize);
            retCode = _malloc(p1,blSize);
            printf("\tmalloc retCode=%d\n",retCode);
            scanf("%c", &c);
        }
        if(c=='2'){
            printf("\tenter block adress for free\n");
            scanf("%d",&adress);
            retCode = _free((VA)adress);
            printf("\tfree retCode=%d\n",retCode);
            scanf("%c", &c);
        }
        if(c=='4'){
            memset(userBuff,'\0',50);
            printf("\tenter block adress for write\n");
            scanf("%d",&adress);
            printf("\tenter buffer size for write\n");
            scanf("%d",&blSize);
            printf("enter buffer data\n");
            scanf("%s",userBuff);
            retCode = _write((VA)adress,userBuff,blSize);
            printf("\twrite retCode=%d\n",retCode);
            scanf("%c", &c);
        }
        if(c=='3'){
            memset(userBuff,'\0',50);
            printf("\tenter block adress for read\n");
            scanf("%d",&adress);
            printf("\tenter buffer size for read\n");
            scanf("%d",&blSize);
            retCode = _read((VA)adress,userBuff,blSize);
            printf("\tread retCode=%d\n",retCode);
            printf("buffer=%s\n",userBuff);
            scanf("%c", &c);
        }
        if(c=='5') {system("cls"); scanf("%c", &c);}
    }

    scanf("%c", &c);
}

int main()
{
//    test_malloc_size_block_more_than_real_block_size();
//    test_malloc_used_block();
    test_malloc_size_block_equals_real_block_size();
//    test_all();

    return 0;
}




