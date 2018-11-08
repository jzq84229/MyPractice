//
//  ls6.c
//  CPractice1
//
//  Created by Jun on 2018/9/14.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls6.h"
#include <time.h>
#include <stdlib.h>

/*
 C 指针
 */
void ls6_func1(void);
void ls6_func2(void);
void ls6_func3(void);
void ls6_func4(void);
void ls6_func5(void);
void ls6_func6(void);
void ls6_func7(void);
void ls6_func8(void);
void getSeconds(unsigned long *par);
void ls6_func9(void);
double getAverage(int *arr, int size);
void ls6_func10(void);
int * getRandom(void);

void ls6(void){
    ls6_func1();
    printf("\n \n");
    ls6_func2();
    printf("\n \n");
    ls6_func3();
    printf("\n \n");
    ls6_func4();
    printf("\n \n");
    ls6_func5();
    printf("\n \n");
    ls6_func6();
    printf("\n \n");
    ls6_func7();
    printf("\n \n");
    ls6_func8();
    printf("\n \n");
    ls6_func9();
    printf("\n \n");
    ls6_func10();
}

/*
 每一个变量都有一个内存位置，每一个内存位置都定义了可使用连字号（&）运算符访问的地址，它表示了在内存中的一个地址。
 */
void ls6_func1(){
    int var1;
    char var2[10];
    
    printf("var1 变量的地址： %p\n", &var1);
    printf("var2 变量的地址： %p\n", &var2);
    printf("sizeof(var2): %lu \n", sizeof(var2));
}

/*
 指针是一个变量，其值为另一个变量的地址，即，内存位置的直接地址。就像其他变量或常量一样，您必须在使用指针存储其他变量地址之前，对其进行声明。
 所有指针的值的实际数据类型，不管是整型、浮点型、字符型，还是其他的数据类型，都是一样的，都是一个代表内存地址的长的十六进制数。不同数据类型的指针之间唯一的不同是，指针所指向的变量或常量的数据类型不同。
 */
void ls6_func2(){
    int var = 20;   /* 实际变量的声明 */
    int *ip;        /* 指针变量的声明 */
    
    ip = &var;      /* 在指针变量中存储 var 的地址 */
    printf("变量var的地址: %p\n", &var);
    printf("变量ip的地址: %p\n", &ip);
    /* 在指针变量中存储的地址 */
    printf("变量ip存储的地址: %p\n", ip);
    /* 使用指针访问值 */
    printf("变量ip指向的值: %d\n", *ip);
}

/*
 C 中的 NULL 指针
 在变量声明的时候，如果没有确切的地址可以赋值，为指针变量赋一个 NULL 值是一个良好的编程习惯。赋为 NULL 值的指针被称为空指针。
 NULL 指针是一个定义在标准库中的值为零的常量。
 
 在大多数的操作系统上，程序不允许访问地址为 0 的内存，因为该内存是操作系统保留的。然而，内存地址 0 有特别重要的意义，它表明该指针不指向一个可访问的内存位置。但按照惯例，如果指针包含空值（零值），则假定它不指向任何东西。
 如需检查一个空指针，您可以使用 if 语句，如下所示：
    if(ptr)      如果 p 非空，则完成
    if(!ptr)     如果 p 为空，则完成
 */
void ls6_func3(){
    int  *ptr = NULL;
    printf("ptr 的地址是 %p\n", ptr  );
}

const int MAX = 3;

/*
 指针的算术运算
 */
void ls6_func4(){
    /* 递增一个指针 */
    int var[] = {10, 100, 200};
    int i, *ptr;
    
    /* 指针中的数组地址 */
    printf("指针递增： \n");
    ptr = var;
    for (i=0; i < MAX; i++) {
        printf("存储地址：var[%d] = %x\n", i, ptr );
        printf("存储值：var[%d] = %d\n", i, *ptr );
        
        /* 移动到下一个位置 */
        ptr++;
    }
    printf("\n \n");
    
    /* 递减一个指针 */
    printf("指针递减： \n");
    /* 指针中最后一个元素的地址 */
    ptr = &var[MAX-1];
    for ( i = MAX; i > 0; i--)
    {
        printf("存储地址：var[%d] = %x\n", i-1, ptr);
        printf("存储值：var[%d] = %d\n", i-1, *ptr);
        
        /* 移动到下一个位置 */
        ptr--;
    }
    printf("\n \n");
    
    /* 指针的比较 */
    /* 指针中第一个元素的地址 */
    printf("指针的比较： \n");
    ptr = var;
    i = 0;
    while (ptr <= &var[MAX - 1])
    {
        printf("存储地址：var[%d] = %x\n", i, ptr);
        printf("存储值：var[%d] = %d\n", i, *ptr);
        
        /* 指向上一个位置 */
        ptr++;
        i++;
    }
}

/*
 指针数组
 在这里，把 ptr 声明为一个数组，由 MAX 个整数指针组成。因此，ptr 中的每个元素，都是一个指向 int 值的指针。
 */
void ls6_func5(){
    int var[] = {10, 100, 200};
    int i, *ptr[MAX];
    for (i = 0; i < MAX; i++) {
        ptr[i] = &var[i];/* 赋值为整数的地址 */
    }
    
    for (i=0; i < MAX; i++) {
        printf("Value of var[%d] = %d\n", i, *ptr[i]);
    }
}

/*
 names是一个指向字符的指针数组来存储一个字符串列表
 */
void ls6_func6(){
    const char *names[] = {
        "Zara Ali",
        "Hina Ali",
        "Nuha Ali",
        "Sara Ali",
    };
    int i = 0;
    
    for ( i = 0; i < MAX; i++)
    {
        printf("Value of names[%d] = %s\n", i, names[i] );
    }
}

/*
 C 指向指针的指针
 指向指针的指针是一种多级间接寻址的形式，或者说是一个指针链。通常，一个指针包含一个变量的地址。当我们定义一个指向指针的指针时，第一个指针包含了第二个指针的地址，第二个指针指向包含实际值的位置。
 一个指向指针的指针变量必须如下声明，即在变量名前放置两个星号。
 int **var;
 当一个目标值被一个指针间接指向到另一个指针时，访问这个值需要使用两个星号运算符
 */
void ls6_func7(){
    int var;
    int *ptr;
    int **pptr;
    
    var = 3000;
    
    /* 获取 var 的地址 */
    ptr = &var;
    
    /* 使用运算符 & 获取 ptr 的地址 */
    pptr = &ptr;
    
    /* 使用 pptr 获取值 */
    printf("Value of var = %d\n", var );
    printf("Value available at *ptr = %d\n", *ptr );
    printf("Value available at **pptr = %d\n", **pptr);
}

/*
 C 传递指针给函数
 C 语言允许您传递指针给函数，只需要简单地声明函数参数为指针类型即可。
 */
void ls6_func8(){
    unsigned long sec;
    
    getSeconds( &sec );
    
    /* 输出实际值 */
    printf("Number of seconds: %ld\n", sec );
}

void getSeconds(unsigned long *par){
    /* 获取当前的秒数 */
    *par = time( NULL );
}

void ls6_func9(){
    /* 带有 5 个元素的整型数组  */
    int balance[5] = {1000, 2, 3, 17, 50};
    double avg;
    
    /* 传递一个指向数组的指针作为参数 */
    avg = getAverage( balance, 5 ) ;
    
    /* 输出返回值  */
    printf("Average value is: %f\n", avg );
}

double getAverage(int *arr, int size){
    int    i, sum = 0;
    double avg;
    
    for (i = 0; i < size; ++i){
        sum += arr[i];
    }
    
    avg = (double)sum / size;
    
    return avg;
}

/*
 C 从函数返回指针
 C 语言不支持在调用函数时返回局部变量的地址，除非定义局部变量为 static 变量。
 */
void ls6_func10(){
    /* 一个指向整数的指针 */
    int *p;
    int i;

    p = getRandom();
    for (i = 0; i < 10; i++) {
        printf("*(p + [%d]) : %d\n", i, *(p + i));
    }
}

///* 要生成和返回随机数的函数 */
//int * getRandom(){
//    static int  r[10];
//    int i;
//
//    /* 设置种子 */
////    srand( (unsigned)time( NULL ));
//    for ( i = 0; i < 10; ++i){
//        r[i] = 0;
////        r[i] = rand();
////        printf("%d\n", r[i]);
//    }
//
//    return r;
//}
