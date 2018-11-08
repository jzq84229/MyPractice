//
//  ls2.c
//  CPractice1
//
//  Created by Jun on 2018/9/13.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls2.h"

/*
 在 C 中，有两种简单的定义常量的方式：
 使用 #define 预处理器。
 使用 const 关键字。
 */
#define LENGTH 10
#define WIDTH 5
#define NEWLINE '\n'

// 变量声明
extern int a, b;
extern int c;
extern float f;

/* 函数声明 */
void ls2_func1(void);
static int count = 3; /* 全局变量 - static 是默认的 */

int ls2(void) {
    printf("变量： \n");
    /* 变量定义 */
    int a, b;
    int c;
    float f;
    
    /* 初始化 */
    a = 10;
    b = 20;
    
    c = a + b;
    printf("value of c : %d \n", c);
    
    f = 70.0/3.0;
    printf("value of f : %f \n", f);
    
    printf("\n \n");
    
    printf("常量：\n");
//    const int LENGTH = 10;
//    const int WIDTH = 5;
//    const char NEWLINE = '\n';
    int area;
    
    area = LENGTH * WIDTH;
    printf("value of area : %d", area);
    printf("%c", NEWLINE);
    
    while (count--) {
        ls2_func1();
    }
    
    printf("%c", NEWLINE);
    return 0;
}

/*
 C 存储类(auto、register、static、extern)
 auto 存储类
 auto 存储类是所有局部变量默认的存储类。
 
 register 存储类
 register 存储类用于定义存储在寄存器中而不是 RAM 中的局部变量。这意味着变量的最大尺寸等于寄存器的大小（通常是一个词），且不能对它应用一元的 '&' 运算符（因为它没有内存位置）。
 
 static 存储类
 static 存储类指示编译器在程序的生命周期内保持局部变量的存在，而不需要在每次它进入和离开作用域时进行创建和销毁。因此，使用 static 修饰局部变量可以在函数调用之间保持局部变量的值。
 static 修饰符也可以应用于全局变量。当 static 修饰全局变量时，会使变量的作用域限制在声明它的文件内。
 static 是全局变量的默认存储类，以下两个变量 (count 和 road) 都有一个 static 存储类。
 
 extern 存储类
 extern 存储类用于提供一个全局变量的引用，全局变量对所有的程序文件都是可见的。当您使用 'extern' 时，对于无法初始化的变量，会把变量名指向一个之前定义过的存储位置。
 当您有多个文件且定义了一个可以在其他文件中使用的全局变量或函数时，可以在其他文件中使用 extern 来得到已定义的变量或函数的引用。可以这么理解，extern 是用来在另一个文件中声明一个全局变量或函数。
 */
void ls2_func1(void){
    /*
     'thingy' 是 'func1' 的局部变量 - 只初始化一次
     每次调用函数 'func1' 'thingy' 值不会被重置。
     */
    static int thingy=5;
    thingy++;
    printf(" thingy 为 %d ， count 为 %d\n", thingy, count);
}

