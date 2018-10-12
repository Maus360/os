#include <stdio.h>
#include "mmemory.h"

void test_malloc() {
    for (int i = 1; i < 40; i++) {
        _init(1, 40);
        VA *p1 = (VA *) malloc(sizeof(VA));
        int retCode = _malloc(p1, (size_t) i);
        if (retCode == 0)
            printf("Malloc complete!\n");
        else if (retCode == -1)
            printf("Malloc has wrong parameters\n");
        else if (retCode == -2)
            printf("Malloc has not yet memory\n");
        else if (retCode == 1)
            printf("Malloc unknown error\n");
    }
}

void test_free() {
    for (int i = 1; i < 40; i++) {
        _init(1, 40);
        VA *p1 = (VA *) malloc(sizeof(VA));
        _malloc(p1, (size_t) i);
        int retCode = _free(0);
        if (retCode == 0)
            printf("Free complete!\n");
        else if (retCode == -1)
            printf("Free has wrong parameters\n");
        else if (retCode == 1)
            printf("Test unknown error\n");
    }
}

void test_read() {
    for (int i = 1; i < 40; i++) {
        _init(1, 40);
        char *userBuff = (char *) calloc(1, 50);
        VA *p1 = (VA *) malloc(sizeof(VA));
        _malloc(p1, (size_t) i);
        int retCode = _read(0, userBuff, i);
        if (retCode == 0)
            printf("Read complete!\n");
        else if (retCode == -1)
            printf("Read has wrong parameters\n");
        else if (retCode == -2)
            printf("Read max block size");
        else if (retCode == 1)
            printf("Read unknown error\n");
    }
}

void test_write() {
    for (int i = 1; i < 40; i++) {
        _init(1, 40);
        char *userBuff = (char *) calloc(1, 50);
        VA *p1 = (VA *) malloc(sizeof(VA));
        _malloc(p1, (size_t) i);
        int retCode = _write(0, userBuff, i);
        if (retCode == 0)
            printf("Write complete!\n");
        else if (retCode == -1)
            printf("Write has wrong parameters\n");
        else if (retCode == -2)
            printf("Write max block size");
        else if (retCode == 1)
            printf("Write unknown error\n");
    }
}

void test_init() {
    for (int i = 1; i < 40; i++) {
        int retCode = _init(1, i);
        if (retCode == 0)
            printf("Init complete!\n");
        else if (retCode == -1)
            printf("Init has wrong parameters\n");
        else if (retCode == 1)
            printf("Init unknown error\n");
    }
}

int main()
{
//    char c='1';
//    VA *p1 = (VA*) malloc(sizeof(VA));
//    VA p2 = (VA) malloc(sizeof(VA));
//    unsigned int blSize, adress;
//    char *userBuff =(char*) calloc(1,50);
//    int retCode;
//    *p1=p2;
//    _init(1,40);
//
//    while(c!='0'){
//        printf("0.Exit\n1.malloc\n2.free\n3.read\n4.write\n.CLS\n");
//        scanf("%c", &c);
//        if(c=='0') return 0;
//        if(c=='1'){
//            printf("\tenter block size for malloc\n");
//            scanf("%d",&blSize);
//            retCode = _malloc(p1,blSize);
//            printf("\tmalloc retCode=%d\n",retCode);
//            scanf("%c", &c);
//        }
//        if(c=='2'){
//            printf("\tenter block adress for free\n");
//            scanf("%d",&adress);
//            retCode = _free((VA)adress);
//            printf("\tfree retCode=%d\n",retCode);
//            scanf("%c", &c);
//        }
//        if(c=='4'){
//            memset(userBuff,'\0',50);
//            printf("\tenter block adress for write\n");
//            scanf("%d",&adress);
//            printf("\tenter buffer size for write\n");
//            scanf("%d",&blSize);
//            printf("enter buffer data\n");
//            scanf("%s",userBuff);
//            retCode = _write((VA)adress,userBuff,blSize);
//            printf("\twrite retCode=%d\n",retCode);
//            scanf("%c", &c);
//        }
//        if(c=='3'){
//            memset(userBuff,'\0',50);
//            printf("\tenter block adress for read\n");
//            scanf("%d",&adress);
//            printf("\tenter buffer size for read\n");
//            scanf("%d",&blSize);
//            retCode = _read((VA)adress,userBuff,blSize);
//            printf("\tread retCode=%d\n",retCode);
//            printf("buffer=%s\n",userBuff);
//            scanf("%c", &c);
//        }
//        if(c=='5') {system("cls"); scanf("%c", &c);}
//    }
//
//    scanf("%c", &c);
    test_malloc();
    test_init();
    test_free();
    test_read();
    test_write();
    return 0;
}




