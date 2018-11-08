//
//  ls5.c
//  CPractice1
//
//  Created by Jun on 2018/9/14.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls5.h"

//#define MON  1
//#define TUE  2
//#define WED  3
//#define THU  4
//#define FRI  5
//#define SAT  6
//#define SUN  7

/*
 C enum(枚举)
 注意：第一个枚举成员的默认值为整型的 0，后续枚举成员的值在前一个成员上加 1。我们在这个实例中把第一个枚举成员的值定义为 1，第二个就为 2，以此类推。
 在C 语言中，枚举类型是被当做 int 或者 unsigned int 类型来处理的，所以按照 C 语言规范是没有办法遍历枚举类型的。
 */
//1、先定义枚举类型，再定义枚举变量
enum DAY{
    MON=1, TUE, WED, THU, FRI, SAT, SUN
};
//2、定义枚举类型的同时定义枚举变量
//enum DAY
//{
//    MON=1, TUE, WED, THU, FRI, SAT, SUN
//} day;
//
//3、省略枚举名称，直接定义枚举变量
//enum
//{
//    MON=1, TUE, WED, THU, FRI, SAT, SUN
//} day;

void ls5_func1(void);

void ls5(void){
    printf("枚举 \n");
    enum DAY day;
    day = WED;
    printf("%d \n", day);
    
    printf("\n \n");
    
}


