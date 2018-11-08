//
//  ls7.c
//  CPractice1
//
//  Created by Jun on 2018/9/18.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls7.h"
#include <stdlib.h>

void ls7_func1(void);
int max(int, int);
void ls7_func2(void);
void populate_array(int *array, size_t arraySize, int (*getNextValue)(void));
int getNextRandomValue(void);

/*
 函数指针
 */
void ls7(void){
//    ls7_func1();
//    printf("\n \n");
    ls7_func2();
}

/*
 函数指针
 函数指针是指向函数的指针变量。
 通常我们说的指针变量是指向一个整型、字符型或数组等变量，而函数指针是指向函数。
 函数指针可以像一般函数一样，用于调用函数、传递参数。
 函数指针变量的声明：
 typedef int (*fun_ptr)(int,int); // 声明一个指向同样参数、返回值的函数指针类型
 */
void ls7_func1(){
    /* p 是函数指针 */
    int (* p)(int, int) = &max; // &可以省略
    int a, b, c, d;
    
    printf("请输入三个数字:");
    scanf("%d %d %d", & a, & b, & c);
    
    /* 与直接调用函数等价，d = max(max(a, b), c) */
    d = p(p(a, b), c);
    
    printf("最大的数字是: %d\n", d);
}

int max(int x, int y){
    return x > y ? x : y;
}

/*
 回调函数
 函数指针作为某个函数的参数
 函数指针变量可以作为某个函数的参数来使用的，回调函数就是一个通过函数指针调用的函数。
 简单讲：回调函数是由别人的函数执行时调用你实现的函数。
 */
void ls7_func2(){
    int myarray[10];
    populate_array(myarray, 10, getNextRandomValue);
    for(int i = 0; i < 10; i++) {
        printf("%d \n", myarray[i]);
    }
    printf("\n");
}

// 回调函数
void populate_array(int *array, size_t arraySize, int (*getNextValue)(void)){
    for (size_t i=0; i<arraySize; i++)
        array[i] = getNextValue();
}

// 获取随机值
int getNextRandomValue(void){
    return rand();
}
