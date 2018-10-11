#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "mmemory.h"
#include <string.h>
int main()
{

    //unsigned int d =(1 << 31)-1;
    //int d = maxBlockSize();

    char c='1';
    VA *p1 = (VA*) malloc(sizeof(VA));
    VA p2 = (VA) malloc(sizeof(VA));
    VA buff = "1234567890";
    unsigned int blSize, adress;
    char *userBuff =(char*) calloc(1,50);
    int retCode;
    *p1=p2;
    _init(1,40);
    //_malloc(p1,10);
    //_malloc(p1,10);
    //_malloc(p1,10);

    while(c!='0'){
        printf("0.Exit\n1.malloc\n2.free\n3.read\n4.write\n5.print\n6.CLS\n");
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
        if(c=='6') {system("cls"); scanf("%c", &c);}
        if(c=='5') { print();scanf("%c", &c);}
    }

    scanf("%c", &c);

    return 0;
}