#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>
#include "mmemory.h"

#define TRUE 1
#define FALSE 0
#define SIZE 50000

unsigned int _isUsedBlock(unsigned int x) {
    if (x >> (31)) return 1;
    else return 0;
}

unsigned int _maxBlockSize() {
    return (unsigned int) ((1 << (31)));
}

unsigned int _setUsedBlock(unsigned int size) {
    return size = size + (1 << (31));
}

unsigned int _setFreeBlock(unsigned int size) {
    return (size << 1) >> 1;
}

unsigned _getSize(unsigned int size) {
    return (size << 1) >> 1;
}

void test_init() {
    char *memory = (char *) calloc(40, sizeof(char));
    unsigned int memorySize = 40 * sizeof(char);
    memset(memory, '@', memorySize);
    begin = (struct block *) calloc(1, sizeof(struct block));
    begin->szBlock = 40;
    begin->pNext = NULL;
}

void mallocc(VA *ptr, size_t szBlock) {
    struct block *pointer = begin;
    unsigned int realBlockSize;
    int shift = 0;
    while (pointer != NULL) {
        realBlockSize = _getSize(pointer->szBlock);
        if (realBlockSize == szBlock) {
            // первый случай
            // ставим отметку что блок занят
            pointer->szBlock = _setUsedBlock(realBlockSize);
            *ptr = (char *) shift;

            return;
        } else {
            // второй случай
            // создаём структуру под новый блок
            struct block *newBlock = (struct block *) malloc(sizeof(struct block));
            if (newBlock == NULL) return;
            // уменьшаем размер имеющегося блока и ставим пометку что он используется
            pointer->szBlock = _setUsedBlock(szBlock);
            // указатель на следующий блок нового блока ставим равным адресу блока, следовавшему за разбиваемым блоком
            newBlock->pNext = pointer->pNext;
            // указатель на следующий блок устанавливаем на адрес созданного нового блока
            pointer->pNext = newBlock;
            // размер нового блока равен разности размера существовавшего блока и размера блока для выделения.
            //сразу ставится отметка что блок свободен
            newBlock->szBlock = realBlockSize - szBlock;
            // записываем результат
            *ptr = (char *) shift;
            return;
        }
    }
    shift += _getSize(pointer->szBlock);
    pointer = pointer->pNext;
}

void test_malloc_size_block_more_than_real_block_size(){
    VA *p1 = (VA*) malloc(sizeof(VA));
    test_init();
    assert(_malloc(p1, 50) == -2);
    _free(*p1);
}

void test_malloc_size_block_equals_real_block_size(){
    VA *p1 = (VA*) malloc(sizeof(VA));
    test_init();
    assert(_malloc(p1, 40) == 0);
    _free(*p1);
}

void test_malloc_size_block_less_than_real_block_size() {
    VA *p1 = (VA *) malloc(sizeof(VA));
    test_init();
    assert(_malloc(p1, 30) == 0);
    _free(*p1);
}

void test_init_negative_page_count() {
    assert(_init_(-2, 40) == -1);
}

void test_init_negative_page_size() {
    assert(_init_(1, -30) == -1);
}

void test_free_not_enougth_memory() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 10);
    assert(_free((VA) *p1 + 30) == -1);
    _free(*p1);
}

void test_free_not_used_block() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    assert(_free((VA) *p1) == 1);
    _free(*p1);
}

void test_free_used_block() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 1);
    assert(_free(*p1) == 0);
    _free(*p1);
}

void test_write_null_buffer() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 5);
    assert(_write((VA) *p1, NULL, 5) == -1);
    _free(*p1);
}

void test_write_wrong_size_buffer() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 5);
    assert(_write((VA) *p1, "5", -2) == -1);
    _free(*p1);
}

void test_write_wrong_pointer() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 5);
    assert(_write((VA) *p1 + 41, "5", 5) == -1);
    _free(*p1);
}

void test_write_free_block() {
    _init_(1, 40);
    VA *p1 = (VA *) malloc(sizeof(VA));
    _malloc(p1, 5);
    _free(*p1);
    assert(_write(*p1, "5", 5) == 1);
    _free(*p1);
}

void test_write() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 40);
//    assert(_write((VA) *p1, "d", 4) == 0);
    _free(*p1);
}

void test_read_null_buffer() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 5);
    assert(_read((VA) *p1, NULL, 5) == -1);
    _free(*p1);
}

void test_read_wrong_size_buffer() {
    test_init();
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 5);
    assert(_read((VA) *p1, userBuff, 85) == -2);
    _free(*p1);
}

void test_read_wrong_pointer() {
    test_init();
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    malloc((size_t) (p1 + 10));;
    assert(_read((VA) *p1 + 41, userBuff, 5) == -1);
    _free(*p1);
}

void test_read_free_block() {
    test_init();
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    malloc((size_t) (p1 + 5));
    free(*p1);
    assert(_read(*p1, userBuff, 5) == 1);
    _free(*p1);
}

void test_read_size_of_buffer_bigger_than_size_of_block() {
    test_init();
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 5);
    assert(_read(*p1, userBuff, 10) == -2);
    _free(*p1);
}

void test_read() {
    test_init();
    char *userBuff = (char *) calloc(1, 50);
    memset(userBuff, '\0', 50);
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 5);
    assert(_read((VA) *p1, userBuff, 5) == 0);
    _free(*p1);
}

struct block *get(int number) {
    struct block *block = begin;
    for (int i = 0; i < number; i++) {
        block = block->pNext;
    }
    return block;
}

void del(struct block *block) {
    if (block == begin) {
        begin = block->pNext;
        free(begin);
    } else {
        struct block *pointer = begin;
        while (pointer->pNext != NULL) {
            if (pointer->pNext == block) pointer->pNext = block->pNext;
            free(begin);
            pointer = pointer->pNext;
        }
    }
}

void stressTesting() {
    srand(time(0));
    VA va = NULL;
    unsigned int size = 0;
    int listSize = 0;
    struct block *block = NULL;

    _init_(1, 1000);
    for (int i = 0; i < 1000; i++) {
        _malloc(&va, 1);
        listSize++;
    }
//    for (int i = 0; i < 1000; i++)
//    {
//        begin = get(rand() % listSize);
//        _free((VA) block);
//        listSize--;
//        size = _maxBlockSize();
//        //printf("\t%d\t%d\t", i + 1, size);
//    }

    printf("\n");

    int res = 0;
    size = 0;
    listSize = 0;
    int tsize = 0;
    int iterations = 0;
    int sIterations = 0;
    int start = FALSE;
    for (double t = 0.05; t < 0.51; t += 0.02) {
        sIterations = 0;
        for (int iter = 0; iter < 100; iter++) {
            _init_(1, SIZE);
            res = 0;
            size = 0;
            listSize = 0;
            iterations = 0;
            start = FALSE;
            do {
                tsize = (int) (SIZE * t) + (rand() % (int) (SIZE * 0.02)) - (int) (SIZE * 0.01);
                if (size < SIZE - tsize) {
                    res = _malloc(&va, tsize);
                    if (res == 0) {
                        if (start) iterations++;
                        _malloc(&va, tsize);
                        listSize++;
                        size += tsize;
                    }
                    va = NULL;
                } else {
                    start = TRUE;
                    begin = get(rand() % listSize);
                    res = _free((VA) block);
                    listSize--;
                    size -= begin->szBlock;
                    del(begin);
                }
            } while (res == 0);
            sIterations += iterations;
        }
        printf("\tStep: %f\tIterations maked: %f\n", t, sIterations / 100);
    }
}

void test_perfomance() {
    VA *p1 = (VA *) malloc(sizeof(VA));
    _init_(1, 10);
    _malloc(p1, 1);
    print();
    _malloc(p1, 4);
    print();
    _malloc(p1, 5);
    print();
    _free((VA) 0);
    print();
    _free((VA) 1);
    print();
    _malloc(p1, 5);
    print();
}

void test_read_11_from_14_written() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 10);
    mallocc(p1, 20);
    char *buff = calloc(1, 50);
    _write(*p1 + 2, "testmyfunc", 14);
    assert(_read(*p1 + 4, buff, 11) == 0);
    printf("%s\n", buff);
    _free(*p1);
}

void test_read_11_from_14_written_buff() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 10);
    mallocc(p1, 20);
    char *buff = calloc(1, 50);
    _write(*p1 + 2, "testmyfunc", 14);
    _read(*p1 + 4, buff, 11);
    char *p = "stmyfunc";
    assert(*buff == *p);
    printf("%s\n", buff);
    _free(*p1);
}

void test_read_5_from_14_written_with_offset() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 40);
    char *buff = calloc(1, 50);
    _write(*p1, "testmy", 14);
    assert(_read(*p1 + 4, buff, 5) == 0);
    printf("%s\n", buff);
    _free(*p1);
}

void test_read_5_from_14_written_with_offset_buff() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 40);
    char *buff = calloc(1, 50);
    _write(*p1, "testmy", 14);
    _read(*p1 + 4, buff, 5);
    char *p = "my";
    assert(*buff == *p);
    printf("%s\n", buff);
    _free(*p1);
}

void test_read_3_with_4_written() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 20);
    char *buff = calloc(1, 50);
    _write(*p1, "lolz", 4);
    assert(_read(*p1, buff, 3) == 0);
    printf("%s\n", buff);
}

void test_read_3_with_4_written_buff() {
    test_init();
    VA *p1 = (VA *) malloc(sizeof(VA));
    mallocc(p1, 20);
    char *buff = calloc(1, 50);
    _write(*p1, "lolz", 4);
    _read(*p1, (void *) buff, 3);
    char *p = "lol";
    assert(*buff == *p);
    printf("%s\n", buff);
}

int test_all() {
    char c = '1';
    VA *p1 = (VA *) malloc(sizeof(VA));
    VA p2 = (VA) malloc(sizeof(VA));
    unsigned int blSize, adress;
    char *userBuff = (char *) calloc(1, 50);
    int retCode;
    *p1 = p2;
    _init_(1, 40);

    while (c != '0') {
        printf("0.Exit\n1.malloc\n2.free\n3.read\n4.write\n5.CLS\n");
        scanf("%c", &c);
        if (c == '0') return 0;
        if (c == '1') {
            printf("\tenter block size for malloc\n");
            scanf("%d", &blSize);
            retCode = _malloc(p1, blSize);
            printf("\tmalloc retCode=%d\n", retCode);
            scanf("%c", &c);
        }
        if (c == '2') {
            printf("\tenter block adress for free\n");
            scanf("%d", &adress);
            retCode = _free((VA) adress);
            printf("\tfree retCode=%d\n", retCode);
            scanf("%c", &c);
        }
        if (c == '4') {
            memset(userBuff, '\0', 50);
            printf("\tenter block adress for write\n");
            scanf("%d", &adress);
            printf("\tenter buffer size for write\n");
            scanf("%d", &blSize);
            printf("enter buffer data\n");
            scanf("%s", userBuff);
            retCode = _write((VA) adress, userBuff, blSize);
            printf("\twrite retCode=%d\n", retCode);
            scanf("%c", &c);
        }
        if (c == '3') {
            memset(userBuff, '\0', 50);
            printf("\tenter block adress for read\n");
            scanf("%d", &adress);
            printf("\tenter buffer size for read\n");
            scanf("%d", &blSize);
            retCode = _read((VA) adress, userBuff, blSize);
            printf("\tread retCode=%d\n", retCode);
            printf("buffer=%s\n", userBuff);
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
    void (*tests[])() = {test_write,
                         test_write_free_block,
                         test_write_wrong_pointer,
                         test_write_wrong_size_buffer,
                         test_write_null_buffer,
                         test_read_free_block,
                         test_read,
                         test_read_null_buffer,
                         test_read_size_of_buffer_bigger_than_size_of_block,
                         test_read_wrong_pointer,
                         test_read_wrong_size_buffer,
                         test_malloc_size_block_equals_real_block_size,
                         test_malloc_size_block_less_than_real_block_size,
                         test_malloc_size_block_more_than_real_block_size,
                         test_free_used_block,
                         test_free_not_enougth_memory,
                         test_free_not_used_block,
                         test_read_free_block,
                         test_init_negative_page_size,
                         test_init_negative_page_count,
                         test_read_3_with_4_written,
                         test_read_5_from_14_written_with_offset,
                         test_read_11_from_14_written,
                         test_read_3_with_4_written_buff,
                         test_read_5_from_14_written_with_offset_buff,
                         test_read_11_from_14_written_buff,
                         test_perfomance
    };
    int length = (int) (*(&tests + 1) - tests);
    printf("%d\n", length);
    for (int i = 0; i < length; i++) {
        (*tests[i])();
        printf("%d test has complete\n", i + 1);
    }
    return 0;
}
