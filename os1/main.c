#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdint.h>
#include "mmemory.h"

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

void test_free_not_enougth_memory() {
    _init_(1, 20);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 10);
    assert(_free((VA) 30) == -1);
}

void test_free_not_used_block() {
    _init_(1, 20);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 10);
    assert(_free((VA) 10) == 1);
}

void test_free_used_block() {
    _init_(1, 20);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 1);
    assert(_free(0) == 0);
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
    assert(_write((VA) 0, "5", -2) == -1);
}

void test_write_wrong_pointer() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_write((VA) 41, "5", 5) == -1);
}

void test_write_free_block() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    _free(0);
    assert(_write(0, "5", 5) == 1);
}

void test_write_size_of_buffer_bigger_than_size_of_block() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_write(0, "5", 10) == -2);
}

void test_write() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    assert(_write((VA) 0, "da", 4) == 0);
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
    assert(_read((VA) 0, userBuff, 7) == -2);
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

void test_perfomance() {
    VA *p1 = (VA *) malloc(sizeof(VA));
    _init_(2, 30);
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    _malloc(p1, 5);
    _malloc(p1, 10);
    _write(0, "5", 5);
    _free(5);
    assert(_write((VA) 5, "1", 10) == 1);
    _malloc(p1, 10);
    assert(_write((VA) 5, "1", 10) == 0);
    assert(_read((VA) 5, userBuff, 15) == -2);
    assert(_read((VA) 5, userBuff, 10) == 0);
    assert(*userBuff == '1');
    _free((VA) 5);
    _free(0);
    assert(_read(0, userBuff, 5) == 1);

}

void test_read_11_from_14_written() {
    _init_(1, 40);
    VA addr;
    _malloc(&addr, 10);
    _malloc(&addr, 20);
    char *buff = calloc(11 * sizeof(char), 11 * sizeof(char));
    _write(addr + 2, "teststring1234", 14);
    assert(_read(addr + 4, buff, 11) == 0);
    free(buff);
}

void test_read_5_from_14_written_with_offset() {
    _init_(1, 80);
    VA addr;
    _malloc(&addr, 80);
    char *buff = calloc(5 * sizeof(char), 5 * sizeof(char));
    _write(addr, "teststring1234", 14);
    addr += 4;
    assert(_read(addr, buff, 5) == 0);
    free(buff);
}

void test_read_3_with_4_written() {
    _init_(1, 96);
    VA addr;
    _malloc(&addr, 20);
    char *buff = calloc(3 * sizeof(char), 3 * sizeof(char));
    _write(addr, "lolz", 4);
    assert(_read(addr, buff, 3) == 0);
    free(buff);

}

//int test_all(){
//    char c='1';
//    VA *p1 = (VA*) malloc(sizeof(VA));
//    VA p2 = (VA) malloc(sizeof(VA));
//    unsigned int blSize, adress;
//    char *userBuff =(char*) calloc(1,50);
//    int retCode;
//    *p1=p2;
//    _init_(1, 40);
//
//    while(c!='0'){
//        printf("0.Exit\n1.malloc\n2.free\n3.read\n4.write\n5.CLS\n");
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
//        if (c == '5') {
//            system("cls");
//            scanf("%c", &c);
//        }
//        if (c == '6') {
//            print();
//            scanf("%c", &c);
//        }
//    }
//
//    scanf("%c", &c);
//}

int main()
{
//    void (*test1)() = test_malloc_size_block_more_than_real_block_size;
//    void (*test2)() = test_malloc_size_block_more_than_real_block_size;
//    void (*test3)() = test_malloc_used_block;
//    void (*test4)() = test_malloc_size_block_equals_real_block_size;
//    void (*test5)() = test_malloc_size_block_less_than_real_block_size;
//    void (*test6)() = test_init_negative_page_count;
//    void (*test7)() = test_init_negative_page_size;
//    void (*test8)() = test_free_not_enougth_memory;
//    void (*test9)() = test_free_not_used_block;
//    void (*test10)() = test_free_used_block;
//    void (*test11)() = test_write_null_buffer;
//    void (*test12)() = test_write_wrong_size_buffer;
//    void (*test13)() = test_write_wrong_pointer;
//    void (*test14)() = test_write_free_block;
//    void (*test15)() = test_write_size_of_buffer_bigger_than_size_of_block;
//    void (*test16)() = test_write;
//    void (*test17)() = test_read_null_buffer;
//    void (*test18)() = test_read_wrong_size_buffer;
//    void (*test19)() = test_read_wrong_pointer;
//    void (*test20)() = test_read_free_block;
//    void (*test21)() = test_read_size_of_buffer_bigger_than_size_of_block;
//    void (*test22)() = test_read;
//    void (*test23)() = test_perfomance;
//    void (*tests[])() = {test1, test2, test3, test4, test5, test6, test7, test8, test9, test10, test11, test12, test13,
//                         test14, test15, test16, test17, test18, test19, test20, test21, test22, test23};
//
//    for (int i = 0; i < 3; i++){
//        tests[i]();
//        printf(i);
//    }
    test_perfomance();
    test_write_size_of_buffer_bigger_than_size_of_block();
    test_write();
    test_write_free_block();
    test_write_wrong_pointer();
    test_write_wrong_size_buffer();
    test_write_null_buffer();
    test_read_free_block();
    test_read();
    test_read_null_buffer();
    test_read_size_of_buffer_bigger_than_size_of_block();
    test_read_wrong_pointer();
    test_read_wrong_size_buffer();
    test_malloc_size_block_equals_real_block_size();
    test_malloc_size_block_less_than_real_block_size();
    test_malloc_size_block_more_than_real_block_size();
    test_malloc_used_block();
    test_free_used_block();
    test_free_not_enougth_memory();
    test_free_not_used_block();
    test_read_free_block();
    test_init_negative_page_size();
    test_init_negative_page_count();
    test_read_3_with_4_written();
    test_read_5_from_14_written_with_offset();
    test_read_11_from_14_written();
    printf("26 tests accepted");
    return 0;
}