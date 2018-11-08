//
//  ls14.c
//  CPractice1
//
//  Created by Jun on 2018/9/28.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "ls14.h"

void ls14_func1(void);
void ls14_func2(void);
/*
 C 内存管理
 C 语言为内存的分配和管理提供了几个函数。这些函数可以在 <stdlib.h> 头文件中找到。
 
 序号    函数和描述
 1    void *calloc(int num, int size);
 在内存中动态地分配 num 个长度为 size 的连续空间，并将每一个字节都初始化为 0。所以它的结果是分配了 num*size 个字节长度的内存空间，并且每个字节的值都是0。
 2    void free(void *address);
 该函数释放 address 所指向的内存块,释放的是动态分配的内存空间。
 3    void *malloc(int num);
 在堆区分配一块指定大小的内存空间，用来存放数据。这块内存空间在函数执行完成后不会被初始化，它们的值是未知的。
 4    void *realloc(void *address, int newsize);
 该函数重新分配内存，把内存扩展到 newsize。
 注意：void * 类型表示未确定类型的指针。C、C++ 规定 void * 类型可以通过类型转换强制转换为任何其它类型的指针。
 */
void reverseSentence(void);
void ls14(){
    ls14_func1();
    printf("\n \n");
    ls14_func2();
    
}

/*
 动态分配内存
 */
void ls14_func1(){
    char name[100];
    char *description;
    
    strcpy(name, "Zara Ali");
    /* 动态分配内存 */
//    description = (char *)malloc(200 * sizeof(char));
    description = calloc(200, sizeof(char));
    if (description == NULL) {
        fprintf(stderr, "Error - unable to allocate required memory \n");
    } else {
        strcpy(description, "Zara ali is a DPS student in class 10th");
    }
    printf("Name = %s \n", name);
    printf("Description: %s \n", description);
    free(description);
}

/*
 重新调整内存的大小和释放内存
 当程序退出时，操作系统会自动释放所有分配给程序的内存，但是，建议您在不需要内存时，都应该调用函数 free() 来释放内存。
 或者，您可以通过调用函数 realloc() 来增加或减少已分配的内存块的大小。
 */
void ls14_func2(){
    char name[100];
    char *description;
    
    strcpy(name, "Zara Ali");
    /* 动态分配内存 */
    description = (char *)malloc(30 * sizeof(char));
    if (description == NULL) {
        fprintf(stderr, "Error - unable to allocate required memory \n");
    } else {
        strcpy(description, "Zara ali is a DPS student.");
    }
    description = realloc(description, 100 * sizeof(char));
    if (description == NULL) {
        fprintf(stderr, "Error - unable to allocate required memory \n");
    } else {
        strcat(description, "She is in class 10th");
    }
    printf("Name = %s \n", name);
    printf("Description: %s \n", description);
    
    /* 使用 free() 函数释放内存 */
    free(description);
}
