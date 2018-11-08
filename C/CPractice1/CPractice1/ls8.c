//
//  ls8.c
//  CPractice1
//
//  Created by Jun on 2018/9/18.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls8.h"
#include <string.h>

void ls8_func1(void);
/*
 C 字符串
 在 C 语言中，字符串实际上是使用 null 字符 '\0' 终止的一维字符数组。因此，一个以 null 结尾的字符串，包含了组成字符串的字符。
 C 中有大量操作字符串的函数：
 strcpy(s1, s2);        复制字符串 s2 到字符串 s1。
 strcat(s1, s2);        连接字符串 s2 到字符串 s1 的末尾。
 strlen(s1);            返回字符串 s1 的长度。
 strcmp(s1, s2);        如果 s1 和 s2 是相同的，则返回 0；如果 s1<s2 则返回小于 0；如果 s1>s2 则返回大于 0。
 strchr(s1, ch);        返回一个指针，指向字符串 s1 中字符 ch 的第一次出现的位置。
 strstr(s1, s2);        返回一个指针，指向字符串 s1 中字符串 s2 的第一次出现的位置。
 */
void ls8(void){
    char greeting[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    
    printf("Greeting message: %s\n", greeting );
    
    printf("\n \n");
    ls8_func1();
}

void ls8_func1(){
    char str1[12] = "Hello";
    char str2[12] = "World";
    char str3[12];
    int len ;
    
    /* 复制 str1 到 str3 */
    strcpy(str3, str1);
    printf("strcpy( str3, str1) :  %s\n", str3 );
    
    /* 连接 str1 和 str2 */
    strcat( str1, str2);
    printf("strcat( str1, str2):   %s\n", str1 );
    
    /* 连接后，str1 的总长度 */
    len = strlen(str1);
    printf("strlen(str1) :  %d\n", len );
}
