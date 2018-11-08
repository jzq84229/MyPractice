//
//  ls12.c
//  CPractice1
//
//  Created by Jun on 2018/9/28.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls12.h"

void ls12_func1(void);
/*
 C 预处理器
 C 预处理器不是编译器的组成部分，但是它是编译过程中一个单独的步骤。简言之，C 预处理器只不过是一个文本替换工具而已，它们会指示编译器在实际编译之前完成所需的预处理。我们将把 C 预处理器（C Preprocessor）简写为 CPP。
 所有的预处理器命令都是以井号（#）开头。它必须是第一个非空字符，为了增强可读性，预处理器指令应从第一列开始。下面列出了所有重要的预处理器指令：
 
 指令    描述
 #define    定义宏
 #include    包含一个源代码文件
 #undef    取消已定义的宏
 #ifdef    如果宏已经定义，则返回真
 #ifndef    如果宏没有定义，则返回真
 #if    如果给定条件为真，则编译下面代码
 #else    #if 的替代方案
 #elif    如果前面的 #if 给定条件不为真，当前条件为真，则编译下面代码
 #endif    结束一个 #if……#else 条件编译块
 #error    当遇到标准错误时，输出错误消息
 #pragma    使用标准化方法，向编译器发布特殊的命令到编译器中
 
 */
void ls12(void){
    ls12_func1();
}

/*
 预定义宏
 ANSI C 定义了许多宏。在编程中您可以使用这些宏，但是不能直接修改这些预定义的宏。
 
 宏    描述
 __DATE__    当前日期，一个以 "MMM DD YYYY" 格式表示的字符常量。
 __TIME__    当前时间，一个以 "HH:MM:SS" 格式表示的字符常量。
 __FILE__    这会包含当前文件名，一个字符串常量。
 __LINE__    这会包含当前行号，一个十进制常量。
 __STDC__    当编译器以 ANSI 标准编译时，则定义为 1。
 */
void ls12_func1(){
    printf("File :%s\n", __FILE__ );
    printf("Date :%s\n", __DATE__ );
    printf("Time :%s\n", __TIME__ );
    printf("Line :%d\n", __LINE__ );
    printf("ANSI :%d\n", __STDC__ );
}
