//
//  ls4.c
//  CPractice1
//
//  Created by Jun on 2018/9/13.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls4.h"
#include <stdlib.h>
#include <time.h>

void ls4_func1(int *param);
void ls4_func2(void);
int * getRandom(void);
/*
 C 数组
 C 语言支持数组数据结构，它可以存储一个固定大小的相同类型元素的顺序集合。数组是用来存储一系列数据，但它往往被认为是一系列相同类型的变量。
 所有的数组都是由连续的内存位置组成。最低的地址对应第一个元素，最高的地址对应最后一个元素。
 
 声明数组
 在 C 中要声明一个数组，需要指定元素的类型和元素的数量
 
 C 中数组详解
 在 C 中，数组是非常重要的，我们需要了解更多有关数组的细节。下面列出了 C 程序员必须清楚的一些与数组相关的重要概念
 多维数组    C 支持多维数组。多维数组最简单的形式是二维数组。
 传递数组给函数    您可以通过指定不带索引的数组名称来给函数传递一个指向数组的指针。
 从函数返回数组    C 允许从函数返回数组。
 指向数组的指针    您可以通过指定不带索引的数组名称来生成一个指向数组中第一个元素的指针。
 */

//double balance[10];
//double balance[] = {1000.0, 2.0, 3.4, 7.0, 50.0};
double balance[5] = {1000.0, 2.0, 3.4, 7.0, 50.0};
void ls4(void){
    int n[10];  /* n 是一个包含 10 个整数的数组 */
    int i, j;
    
    for (i = 0; i < 10; i++) {
        n[i] = i + 100;/* 设置元素 i 为 i + 100 */
    }
    
    printf("%p \n", n);
    printf("%p \n", &n[0]);
    printf("%p \n", &n);
    printf("%d \n", *n);
    /* 输出数组中每个元素的值 */
    for (j = 0; j < 10; j++ )
    {
//        printf("Element[%d] = %d\n", j, n[j] );
        printf("Element[%d] = %d\n", j, *(n+j));
    }

    printf("\n \n");
    ls4_func2();
}

/*
 C 传递数组给函数
 */
void ls4_func1(int *param){
    
}

/*
 C 从函数返回数组
 C 不支持在函数外返回局部变量的地址，除非定义局部变量为 static 变量。
 */
void ls4_func2(){
    /* 一个指向整数的指针 */
    int *p;
    int i;
    
    p = getRandom();
    for ( i = 0; i < 10; i++ )
    {
        printf( "*(p + %d) : %d\n", i, *(p + i));
    }
}

/* 要生成和返回随机数的函数 */
int * getRandom(){
    static int  r[10];
    int i;
    
    /* 设置种子 */
    srand( (unsigned)time( NULL ) );
    for ( i = 0; i < 10; ++i)
    {
        r[i] = rand();
        printf( "r[%d] = %d\n", i, r[i]);
        
    }
    return r;
}
