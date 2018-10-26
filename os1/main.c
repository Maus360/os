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
    _init_(1, 40);
    assert(_malloc(p1, -1) == -1);
}

void test_malloc_size_block_more_than_real_block_size(){
    VA *p1 = (VA*) malloc(sizeof(VA));
    _init_(1, 20);
    assert(_malloc(p1, 30) == -2);
}

void test_malloc_used_block(){
    VA *p1 = (VA*) malloc(sizeof(VA));
    _init_(1, 40);
    _malloc(p1, 20);
    _malloc(p1, 20);
    assert(_malloc(p1, 20) == -2);
}

void test_malloc_size_block_equals_real_block_size(){
    VA *p1 = (VA*) malloc(sizeof(VA));
    _init_(1, 50);
    assert(_malloc(p1, 50) == 0);
}

void test_malloc_size_block_less_than_real_block_size() {
    VA *p1 = (VA *) malloc(sizeof(VA));
    _init_(1, 50);
    assert(_malloc(p1, 40) == 0);
}

void test_init_negative_page_count() {
    assert(_init_(-2, 40) == -1);
}

void test_init_negative_page_size() {
    assert(_init_(1, -30) == -1);
}

void test_free_wrong_begin_pointer() {
    VA *p1 = (VA *) malloc(sizeof(VA));
    assert(_free(p1) == 1);
}

void test_free_not_enougth_memory() {
    _init_(1, 20);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 10);
    assert(_free(30) == -1);
}

void test_free_not_used_block() {
    _init_(1, 20);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 10);
    assert(_free(10) == 1);
}

void test_free_used_block() {
    _init_(1, 20);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 1);
    assert(_free(0) == 0);
}

void test_write_unintialized_memory() {
    assert(_write(0, 5, 5) == 1);
}

void test_write_null_buffer() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_write((VA) 0, NULL, 5) == -1);
}

void test_write_wrong_size_buffer() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_write((VA) 0, 5, -2) == -1);
}

void test_write_wrong_pointer() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_write((VA) 41, 5, 5) == -1);
}

void test_write_free_block() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    _free(0);
    assert(_write(0, 5, 5) == 1);
}

void test_write_size_of_buffer_bigger_than_size_of_block() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_write(0, 5, 10) == -2);
}

void test_write() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_write((VA) 0, "da", 4) == 0);
}

void test_read_unintialized_memory() {
    assert(_read(0, 5, 5) == 1);
}

void test_read_null_buffer() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_read((VA) 0, NULL, 5) == -1);
}

void test_read_wrong_size_buffer() {
    _init_(1, 40);
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    _write(0, "5", 5);
    assert(_read((VA) 0, userBuff, -2) == -1);
}

void test_read_wrong_pointer() {
    _init_(1, 40);
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    _write(0, "5", 5);
    assert(_read((VA) 41, userBuff, 5) == -1);
}

void test_read_free_block() {
    _init_(1, 40);
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    _write(0, "5", 5);
    _free(0);
    assert(_read(0, userBuff, 5) == 1);
}

void test_read_size_of_buffer_bigger_than_size_of_block() {
    _init_(1, 40);
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_read(0, userBuff, 10) == -2);
}

void test_read() {
    _init_(1, 40);
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    _write(0, "5", 5);
    assert(_read((VA) 0, userBuff, 5) == 0);
    assert(*userBuff == '5');
}



int test_all(){
    char c='1';
    VA *p1 = (VA*) malloc(sizeof(VA));
    VA p2 = (VA) malloc(sizeof(VA));
    unsigned int blSize, adress;
    char *userBuff =(char*) calloc(1,50);
    int retCode;
    *p1=p2;
    _init_(1, 40);

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
        if (c == '5') {
            system("cls");
            scanf("%c", &c);
        }
        if (c == '6') {
            print();
            scanf("%c", &c);
        }
    }

    scanf("%c", &c);
}

int main()
{
//    test_malloc_size_block_more_than_real_block_size();
//    test_malloc_used_block();
//    test_malloc_size_block_equals_real_block_size();
//    test_all();
//    test_init_negative_page_size();
//    test_init_huge_amount_of_memory_to_calloc();
//    test_free_not_used_block();
//    test_write_null_buffer();
//    test_write_free_block();
//    test_write_size_of_buffer_bigger_than_size_of_block();
//    test_write();
//    test_read_unintialized_memory();
    test_read();
    return 0;
}




