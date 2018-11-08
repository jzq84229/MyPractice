//
//  ls1.c
//  CPractice1
//
//  Created by Jun on 2018/9/13.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls1.h"
#include <limits.h>
#include <float.h>

/*
 printf输出格式控制:
 %a(%A)     浮点数、十六进制数字和p-(P-)记数法(C99)
 %c         字符
 %d         有符号十进制整数
 %f         浮点数(包括float和doulbe)
 %e(%E)     浮点数指数输出[e-(E-)记数法]
 %g(%G)     浮点数不显无意义的零"0"
 %i         有符号十进制整数(与%d相同)
 %u         无符号十进制整数
 %o         八进制整数
 %x(%X)     十六进制整数0f(0F)   e.g.   0x1234
 %p         指针
 %s         字符串
 %%         输出字符%
 
 l：对整型指定长整型long
 %ld , %lx , %lo , %lu
 一个l表示long,即 long int, 两个l表示long long,即long long int
 
 h：只用于整型的格式字符修正为short
 %hd , %hx , %ho , %hu
 
 
 
 左对齐："-"   比如："%-20s"
 右对齐："+"   比如："%+20s"
 */
void ls1(){
    /*
     c语言基本数据类型（short、int、long、char、float、double）
     整型 short int long
     浮点型 float double
     字符型 char
     */
    char c = 'c';
    unsigned char uc = 's';
    signed char sc = 'c';
    int i = 14;
    unsigned int ui = -15;
    short s = 23;
    unsigned short us = 34;
    long l = 45;
    unsigned long ul = 46;
    float f = 3.14159;
    double d = 4.234212;
    char *str = "Hello World!";
    printf("C基本数据类型: \n");
    printf("short、int、long、char、float、double \n");
    printf("字符 char : %c \n", c);
    printf("字符 unsigned char : %d \n", uc);
    printf("字符 signed char : %c \n", sc);
    printf("int : %d \n", i);
    printf("unsigned int : %u \n", ui);
    printf("short : %hd \n", s);
    printf("unsigned short : %hd \n", us);
    printf("long : %ld \n", l);
    printf("unsigned long : % ld \n", ul);
    printf("float : %f \n", f);
    printf("double : %f \n", d);
    printf("字符串 %s \n", str);
    printf("左对齐：%-20s \n", str);
    printf("右对齐：%20s \n", str);
    printf("右对齐：%+20s \n", str);
    printf("\n \n");
    
    printf("基本数据类型长度：\n");
    printf("char 存储大小: %lu \n", sizeof(char));
    printf("short 存储大小: %lu \n", sizeof(short));
    printf("int 存储大小: %lu \n", sizeof(int));
    printf("long 存储大小: %lu \n", sizeof(long));
    printf("long long 存储大小: %lu \n", sizeof(long long));
    printf("float 存储大小: %lu \n", sizeof(float));
    printf("double 存储大小: %lu \n", sizeof(double));
    printf("long double 存储大小: %lu \n", sizeof(long double));
    
    printf("\n \n");
    
    /*
     #undef  SCHAR_MIN
     #undef  SCHAR_MAX
     #undef  UCHAR_MAX
     #undef  SHRT_MIN
     #undef  SHRT_MAX
     #undef  USHRT_MAX
     #undef  INT_MIN
     #undef  INT_MAX
     #undef  UINT_MAX
     #undef  LONG_MIN
     #undef  LONG_MAX
     #undef  ULONG_MAX
     
     #undef  CHAR_BIT
     #undef  CHAR_MIN
     #undef  CHAR_MAX
     
     #undef  LLONG_MIN
     #undef  LLONG_MAX
     #undef  ULLONG_MAX
     
     #undef   LONG_LONG_MIN
     #undef   LONG_LONG_MAX
     #undef   ULONG_LONG_MAX
     
     */
    printf("基本数据类型取值范围： \n");
    printf("char CHAR_BIT: %d \n", CHAR_BIT);
    printf("char 最小值：%d \n", CHAR_MIN);
    printf("char 最大值：%d \n", CHAR_MAX);
    printf("char 最小值：%d \n", SCHAR_MIN);
    printf("char 最大值：%d \n", SCHAR_MAX);
    printf("unsigned char 最大值：%d \n", UCHAR_MAX);
    
    printf("int 最小值: %d \n", INT_MIN);
    printf("int 最大值： %d \n", INT_MAX);
    printf("unsigned int 最大值： %u \n", UINT_MAX);
    
    printf("long 最小值： %ld \n", LONG_MIN);
    printf("long 最大值： %ld \n", LONG_MAX);
    printf("long 最大值： %lu \n", ULONG_MAX);
    printf("long long 最小值： %lld \n", LLONG_MIN);
    printf("long long 最大值： %lld \n", LLONG_MAX);
    printf("unsigned long long 最大值： %llu \n", ULLONG_MAX);
    
    printf("float 最小值: %E \n", FLT_MIN);
    printf("float 最大值: %E \n", FLT_MAX);
    
    printf("double 最小值: %E \n", DBL_MIN);
    printf("double 最大值: %E \n", DBL_MAX);
}
