//
// Created by maksi on 11.10.2018.
//

//typedef char* VA;

struct block {
    struct block*	pNext;	// следующий свободный или
    // занятый блок

    unsigned int	szBlock;	// размер блока
} *begin=NULL;

char *memory;
unsigned int memorySize=0;


unsigned int isUsedBlock(unsigned int x){
    if(x>> (31)) return 1;
    else return 0;
}

unsigned int maxBlockSize(){
    return (unsigned int) ((1 << (31)));
}

unsigned int setUsedBlock(unsigned int size){
    return size = size + (1<<(31));
}
unsigned int setFreeBlock(unsigned int size){
    return (size<<1)>>1;
}

unsigned getSize(unsigned int size){
    return (size<<1)>>1;
}

/**
 	@func	_init
 	@brief	Инициализация модели менеджера памяти

	@param	[in] n		количество страниц
	@param	[in] szPage	размер страницы

	В варианте 1 и 2 общий объем памяти расчитывается как n*szPage

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
int _init(int n, int szPage){

    int d = setUsedBlock(52);
    //d = getSize(d);
    //printf("%d\n", d);

    if( (n<=0) || (szPage<=0)){
        return -1;
    }
    else{
        memory = (char*) calloc(szPage,sizeof(char));
        memorySize = szPage*sizeof(char);
        memset(memory,'@',memorySize);
        begin =(struct block*) calloc(1,sizeof(struct block));
        if(memory==NULL){
            return 1;
        }
        else{
            begin->szBlock=(n*szPage);
            begin->pNext=NULL;
            return 0;
        }
    }
}


/**
 	@func	_malloc
 	@brief	Выделяет блок памяти определенного размера

	@param	[out] ptr		адресс блока
	@param	[in]  szBlock	размер блока

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	нехватка памяти
	@retval	1	неизвестная ошибка
 **/
int _malloc(VA* ptr, size_t szBlock){
    struct block *pointer = begin;
    unsigned int realBlockSize;
    int shift=0;
    if(szBlock<=0) return -1;
    if(pointer==NULL) return 1; // need initialize memory first
    while(pointer != NULL){
        realBlockSize= getSize(pointer->szBlock);
        if(!isUsedBlock(pointer->szBlock) && realBlockSize>=szBlock){
            // нашли первый свободный блок нужного размера
            // 2 случая
            // 1 - размер блока равен необходимому размеру - возвращаем просто указатель на блок
            // 2 - размер блока больше чем необходимый, нужно блок разбить на 2 блока
            if(realBlockSize==szBlock){
                // первый случай
                // ставим отметку что блок занят
                pointer->szBlock=setUsedBlock(realBlockSize);
                *ptr =(char*) shift;

                return 0;
            }
            else{
                // второй случай
                // создаём структуру под новый блок
                struct block* newBlock =(struct block*) malloc(sizeof(struct block));
                if(newBlock==NULL) return 1;
                // уменьшаем размер имеющегося блока и ставим пометку что он используется
                pointer->szBlock = setUsedBlock(szBlock);
                // указатель на следующий блок нового блока ставим равным адресу блока, следовавшему за разбиваемым блоком
                newBlock->pNext = pointer->pNext;
                // указатель на следующий блок устанавливаем на адрес созданного нового блока
                pointer->pNext = newBlock;
                // размер нового блока равен разности размера существовавшего блока и размера блока для выделения.
                //сразу ставится отметка что блок свободен
                newBlock->szBlock = realBlockSize-szBlock;
                // записываем результат
                *ptr =(char*) shift;
                return 0;
            }
        }
        shift += getSize(pointer->szBlock);
        pointer=pointer->pNext;
    }
    return -2;
}



/**
 	@func	_free
 	@brief	Удаление блока памяти

	@param	[in] ptr		адресс блока

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
int _free(VA ptr){
    struct block * pointer = begin;
    unsigned int ptrBlock =(unsigned int) (ptr);
    unsigned int shift=0;

    if(ptrBlock<0 || ptrBlock>memorySize) return -1;
    if(begin==NULL) return 1;
    if(ptr<0)  return -1;

    while(pointer!=NULL){

        if(shift==ptrBlock){
            // нашли необходимый нам блок для освобождения
            // нужно его осободить
            // делаем пометку что блок свободен
            // проверяем, а не занят ли блок
            if(isUsedBlock(pointer->szBlock)){
                // блок занят,особождаем его
                pointer->szBlock = setUsedBlock(pointer->szBlock);
                return 0;
            }
            else{
                // блок не занят, освобождать то нечего
                // возвращаем ошибку
                return 1;
            }

        }
        shift+= getSize(pointer->szBlock);
        pointer=pointer->pNext;
        if(shift>ptrBlock) return -1;
    }
    return -1;
}



/**
 	@func	_write
 	@brief	Запись информации в блок памяти

	@param	[in] ptr		адресс блока
	@param	[in] pBuffer	адресс буфера откуда копируется инфомация
	@param	[in] szBuffer	размер буфера

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
int _write (VA ptr, void* pBuffer, size_t szBuffer){
    unsigned int ptrBlock = (int) ptr;
    unsigned int shift=0;
    unsigned int curBlockSize=0;
    struct block* pointer=begin;

    if(pointer==NULL) return 1;
    if(pBuffer==NULL) return -1;
    if(szBuffer<1) return -1;
    if(ptrBlock >= 0 && ptrBlock < memorySize){
        // указатель на область памяти корректен
        // находим, какому блоку принадлежит указатель
        while(pointer!=NULL){
            curBlockSize = getSize(pointer->szBlock);
            if(ptrBlock>=shift && ptrBlock<shift+curBlockSize){
                // адрес попадает в текущий блок
                // нужно проверить, а выделен лми блок для записи\чтения
                if(isUsedBlock(pointer->szBlock)){
                    // блок выделен
                    // проверяем, нет ли выхода за пределы блока
                    if(shift+curBlockSize >= ptrBlock+szBuffer){
                        // выхода за пределы блока нет
                        // записываем информацию
                        memcpy((memory+ptrBlock),pBuffer, szBuffer);
                        return 0;
                    }
                    else{
                        // выход за пределы блока при попытке записи
                        return -2;
                    }
                }
                else{
                    // блок свободен, выдаём ошибку
                    return 1;
                }
            }

            shift+=curBlockSize;
            pointer = pointer->pNext;
        }
        return -1;
    }
    else{
        return -1;
    }

}



/**
 	@func	_read
 	@brief	Чтение информации из блока памяти

	@param	[in] ptr		адресс блока
	@param	[in] pBuffer	адресс буфера куда копируется инфомация
	@param	[in] szBuffer	размер буфера

	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
int _read(VA ptr, void* pBuffer, size_t szBuffer){
    unsigned int ptrBlock = (unsigned int) ptr;
    unsigned int shift = 0;
    unsigned int curBlockSize = 0;
    struct block* pointer = begin;

    if(pBuffer == NULL) return -1;
    if(szBuffer<1) return -1;
    if(begin==NULL) return 1;
    if(ptrBlock<0) return -1;
    if(ptrBlock < 0 || ptrBlock >= memorySize) return -1;
    while(pointer!=NULL){
        curBlockSize = getSize(pointer->szBlock);
        // ищем необходиый блок
        if(ptrBlock>=shift && ptrBlock<shift+curBlockSize){
            // нашли необходимый блок
            // проверяем, выделен ли он для чтения\записи
            if(isUsedBlock(pointer->szBlock)){
                // блок выделенен, чтение корректно
                // проверяем ситуацию выхода за пределы блока при чтении
                if(shift+curBlockSize >= ptrBlock+szBuffer){
                    // выхода за пределы блока нет
                    // записываем информацию
                    memcpy(pBuffer,(memory+ptrBlock), szBuffer);
                    return 0;
                }
                else{
                    // выход за пределы блока при попытке записи
                    return -2;
                }

            }
            else{
                // блок не выделене, ошибка
                return 1;
            }
        }
        shift += curBlockSize;
        pointer = pointer->pNext;
    }
    return -1;

}