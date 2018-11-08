//
//  ls9.c
//  CPractice1
//
//  Created by Jun on 2018/9/18.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls9.h"
#include <string.h>

struct Books{
    char title[50];
    char author[50];
    char subject[100];
    int book_id;
};

void ls9_func1(void);
void ls9_func2(void);
void printBook(struct Books book);
void ls9_func3(void);
void printBook1(struct Books *book);

/*
 C 结构体
 C 数组允许定义可存储相同类型数据项的变量，结构是 C 编程中另一种用户自定义的可用的数据类型，它允许您存储不同类型的数据项。
 为了定义结构，您必须使用 struct 语句。struct 语句定义了一个包含多个成员的新的数据类型，
 //此声明声明了拥有3个成员的结构体，分别为整型的a，字符型的b和双精度的c
 //同时又声明了结构体变量s1
 //这个结构体并没有标明其标签
 struct
 {
 int a;
 char b;
 double c;
 } s1;
 
 //此声明声明了拥有3个成员的结构体，分别为整型的a，字符型的b和双精度的c
 //结构体的标签被命名为SIMPLE,没有声明变量
 struct SIMPLE
 {
 int a;
 char b;
 double c;
 };
 //用SIMPLE标签的结构体，另外声明了变量t1、t2、t3
 struct SIMPLE t1, t2[20], *t3;
 
 //也可以用typedef创建新类型
 typedef struct
 {
 int a;
 char b;
 double c;
 } Simple2;
 //现在可以用Simple2作为类型声明新的结构体变量
 Simple2 u1, u2[20], *u3;

 结构体的成员可以包含其他结构体，也可以包含指向自己结构体类型的指针，而通常这种指针的应用是为了实现一些更高级的数据结构如链表和树等。
 //此结构体的声明包含了其他的结构体
 struct COMPLEX
 {
 char string[100];
 struct SIMPLE a;
 };
 
 //此结构体的声明包含了指向自己类型的指针
 struct NODE
 {
 char string[100];
 struct NODE *next_node;
 };
 如果两个结构体互相包含，则需要对其中一个结构体进行不完整声明，
 struct B;    //对结构体B进行不完整声明
 
 //结构体A中包含指向结构体B的指针
 struct A
 {
 struct B *partner;
 //other members;
 };
 */
void ls9(void){
    ls9_func1();
    printf("\n \n");
    ls9_func2();
    printf("\n \n");
    ls9_func3();
}

/*
 访问结构成员
 为了访问结构的成员，我们使用成员访问运算符（.）。成员访问运算符是结构变量名称和我们要访问的结构成员之间的一个句号。您可以使用 struct 关键字来定义结构类型的变量。
 */
void ls9_func1(){
    struct Books Book1;        /* 声明 Book1，类型为 Books */
    struct Books Book2;        /* 声明 Book2，类型为 Books */
    
    /* Book1 详述 */
    strcpy( Book1.title, "C Programming");
    strcpy( Book1.author, "Nuha Ali");
    strcpy( Book1.subject, "C Programming Tutorial");
    Book1.book_id = 6495407;
    
    /* Book2 详述 */
    strcpy( Book2.title, "Telecom Billing");
    strcpy( Book2.author, "Zara Ali");
    strcpy( Book2.subject, "Telecom Billing Tutorial");
    Book2.book_id = 6495700;
    
    
    /* 输出 Book1 信息 */
    printf( "Book 1 title : %s\n", Book1.title);
    printf( "Book 1 author : %s\n", Book1.author);
    printf( "Book 1 subject : %s\n", Book1.subject);
    printf( "Book 1 book_id : %d\n", Book1.book_id);
    
    /* 输出 Book2 信息 */
    printf( "Book 2 title : %s\n", Book2.title);
    printf( "Book 2 author : %s\n", Book2.author);
    printf( "Book 2 subject : %s\n", Book2.subject);
    printf( "Book 2 book_id : %d\n", Book2.book_id);
}

/**
 结构作为函数参数
 您可以把结构作为函数参数，传参方式与其他类型的变量或指针类似。
 */
void ls9_func2(){
    struct Books Book1;        /* 声明 Book1，类型为 Books */
    struct Books Book2;        /* 声明 Book2，类型为 Books */
    
    /* Book1 详述 */
    strcpy( Book1.title, "C Programming");
    strcpy( Book1.author, "Nuha Ali");
    strcpy( Book1.subject, "C Programming Tutorial");
    Book1.book_id = 6495407;
    
    /* Book2 详述 */
    strcpy( Book2.title, "Telecom Billing");
    strcpy( Book2.author, "Zara Ali");
    strcpy( Book2.subject, "Telecom Billing Tutorial");
    Book2.book_id = 6495700;
    
    /* 输出 Book1 信息 */
    printBook(Book1);
    
    /* 输出 Book2 信息 */
    printBook(Book2);
}

void printBook(struct Books book){
    printf( "Book title : %s\n", book.title);
    printf( "Book author : %s\n", book.author);
    printf( "Book subject : %s\n", book.subject);
    printf( "Book book_id : %d\n", book.book_id);
}

/*
 指向结构的指针
 您可以定义指向结构的指针，方式与定义指向其他类型变量的指针相似，
 为了使用指向该结构的指针访问结构的成员，您必须使用 -> 运算符
 */
void ls9_func3(){
    struct Books Book1;        /* 声明 Book1，类型为 Books */
    struct Books Book2;        /* 声明 Book2，类型为 Books */
    
    /* Book1 详述 */
    strcpy( Book1.title, "C Programming");
    strcpy( Book1.author, "Nuha Ali");
    strcpy( Book1.subject, "C Programming Tutorial");
    Book1.book_id = 6495407;
    
    /* Book2 详述 */
    strcpy( Book2.title, "Telecom Billing");
    strcpy( Book2.author, "Zara Ali");
    strcpy( Book2.subject, "Telecom Billing Tutorial");
    Book2.book_id = 6495700;
    
    /* 通过传 Book1 的地址来输出 Book1 信息 */
    printBook1(&Book1);
    
    /* 通过传 Book2 的地址来输出 Book2 信息 */
    printBook1(&Book2);
}

/*
 结构体长度
 1，数据类型自身对齐
 数据类型的起始地址为其大小的整数倍
 2，结构体的自身对齐
 结构体的自身对齐值为其中最大的成员大小
 3，指定对齐
 可以使用关键词#pragma pack(1) 来指定结构体的对齐值
 4，有效对齐值
 有效对齐值为自身对齐值与指定对齐值中较小的一个。（即指定对齐值超过自身对齐值无意义）
 */
void printBook1(struct Books *book){
    printf( "Book title : %s\n", book->title);
    printf( "Book author : %s\n", book->author);
    printf( "Book subject : %s\n", book->subject);
    printf( "Book book_id : %d\n", book->book_id);
}
