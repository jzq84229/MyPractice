//
//  ls15.c
//  CPractice1
//
//  Created by Jun on 2018/9/29.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ls15.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define MAXSIZE 10

void ls15_func1(void);
void ls15_func2(void);
void ls15_func3(void);
void ls15_func4(void);

/*
 简单的多线程编程
 线程在Unix系统下，通常被称为轻量级的进程，线程虽然不是进程，但却可以看作是Unix进程的表亲，同一进程中的多条线程将共享该进程中的全部系统资源，如虚拟地址空间，文件描述符和信号处理等等。但同一进程中的多个线程有各自的调用栈(call stack)，自己的寄存器环境（register context)，自己的线程本地存储(thread-local storage)。 一个进程可以有很多线程，每条线程并行执行不同的任务。
 线程可以提高应用程序在多核环境下处理诸如文件I/O或者socket I/O等会产生堵塞的情况的表现性能。在Unix系统中，一个进程包含很多东西，包括可执行程序以及一大堆的诸如文件描述符地址空间等资源。在很多情况下，完成相关任务的不同代码间需要交换数据。如果采用多进程的方式，那么通信就需要在用户空间和内核空间进行频繁的切换，开销很大。但是如果使用多线程的方式，因为可以使用共享的全局变量，所以线程间的通信（数据交换）变得非常高效。
 
 Linux系统下的多线程遵循POSIX线程接口，称为pthread。编写Linux下的多线程程序，需要使用头文件pthread.h，连接时需要使用库libpthread.a。顺便说一下，Linux下pthread的实现是通过系统调用clone（）来实现的。clone（）是Linux所特有的系统调用，它的使用方式类似fork，关于clone（）的详细情况，有兴趣的读者可以去查看有关文档说明。下面我们展示一个最简单的多线程程序example1.c。
 */

void ls15(){
//    ls15_func1();
//    printf("\n \n");
//    ls15_func2();
//    printf("\n \n");
//    ls15_func3();
//    printf("\n \n");
    ls15_func4();
}

void thread(void){
    int i;
    for (i = 0; i < 3; i++) {
        printf("This is a pthread. \n");
    }
}

/*
 Linux下的多线程编程需要注意的是程序需要包含头文件pthread.h，在生成可执行文件的时候需要链接库libpthread.a或者libpthread.so。
 线程创建函数：
 pthread_create(pthread_t *thread, pthread_attr_t * attr, void *(*start_routine)(void *),void *arg);
 
 参数说明：
 thread         标示一个线程，它是一个pthread_t类型的变量（unsigned long int）
 attr           用于设置线程的属性，默认是null
 start_routine  当线程分配资源成功后，线程中所运行的单元，通俗的说就是你自己写的一个函数
 arg            线程函数运行时传入的一个参数，一般可以用这个传入的参数去控制线程结束
 函数返回值：
 创建成功返回0，创建失败返回非0值，常见错误返回代码为EAGAIN和EINVAL，EGAIN标示系统中线程的数量达到上限，错误代码EINVAL表示线程的属性非法。
 注意：线程创建城成功后，新创建的线程按照参数3和参数4确定一个运行函数，原来的线程在线程创建函数返回后继续运行下一行代码。
 */
void ls15_func1(){
    pthread_t id;
    int i, ret;
    ret = pthread_create(&id, NULL, (void *)thread, NULL);
    if (ret != 0) {
        printf("Create pthread error! \n");
        exit(1);
    }
    for (i = 0; i < 3; i++) {
        printf("This is the main process. \n");
    }
    pthread_join(id, NULL);
}

/*
 线程结束
 线程结束函数：pthread_exit(void *retval);
 retval     用于存放线程结束的退出状态
 
 线程等待 pthread_join()
 pthread_join() 用来等待一个线程运行结束。这个函数是阻塞函数，一直被等待的线程结束为止，函数才返回并且收回被等待线程的资源。函数的原型为：
 extern int pthread_join_P((pthread_t _th,void **__thread_return));
 _th：线程的标示符，也就是线程创建成功的值，在通俗的说就是pthread_create函数运行成功后的第一个参数
 __thread_return：返回值，它是一个指针用来存贮被等待线程的返回值。
 
 线程函数的结束方式有两种：
 一种是线程函数运行结束，不用返回结果；
 另一种就是通过函数pthread_exit()来实现，将结果传出。
 函数原型是：
 extern void pthread_exit_P((void *_retval))
 参数是函数的返回值，这个值可以被pthread_join函数捕获，通过__thread_return参数获得此值。
 说道线程的创建还有一点必须要提及，那就是线程的属性。一般在我们创建线程的时候设置attr属性的时候都是使用null，这个是默认参数。但是在很多时候需要调整线程的属性，特别是线程优先级。
 线程的属性结构为：pthread_attr_t，在头文件<pthreadtype.h>中定义
 typedef struct
 {
     int                                detachstate;    线程的终止状态
     int                                schedpolicy;    线程调度策略（优先级）
     struct sched_param                 schedparam;     线程的调度参数
     int                                inheritsched;   线程的继承性
     int                                scope;          线程的作用域
     size_t                             guardsize;      线程栈末尾的警戒缓冲区大小
     int                                stackaddr_set;  运行栈
     void *                             stackaddr;      线程栈的位置
     size_t                             stacksize;      线程栈的大小
 }pthread_attr_t;
 要注意的是线程的属性值不能直接设置，必须要用相关的函数进行操作。线程属性的初始化函数pthread_attr_init()，这个函数必须在pthread_create()函数之前调用。
 */
void print_message_function( void *ptr);

void ls15_func2(){
    int tmp1, tmp2;
    void *retval;
    pthread_t thread1, thread2;
    char *message1 = "thread1";
    char *message2 = "thread2";
    
    int ret_thrd1, ret_thrd2;
    
    ret_thrd1 = pthread_create(&thread1, NULL, (void *)print_message_function, (void *)message1);
    ret_thrd2 = pthread_create(&thread2, NULL, (void *)print_message_function, (void *)message2);
    
    // 线程创建成功，返回0,失败返回失败号
    if (ret_thrd1 != 0) {
        printf("线程1创建失败 \n");
    } else {
        printf("线程1创建成功 \n");
    }
    
    if (ret_thrd2 != 0) {
        printf("线程2创建失败 \n");
    } else {
        printf("线程2创建成功 \n");
    }
    
    //同样，pthread_join的返回值成功为0
    tmp1 = pthread_join(thread1, &retval);
    printf("thread1 return value(retval) is %d \n", (int)retval);
    printf("thread1 return value(tmp) is %d \n", tmp1);
    
    if (tmp1 != 0) {
        printf("connot join with thread1 \n");
    }
    printf("thread1 end \n");
    
    tmp2 = pthread_join(thread2, &retval);
    printf("thread2 return value(retval) is %d \n", (int)retval);
    printf("thread2 return value(tmp) is %d \n", tmp2);
    if (tmp2 != 0) {
        printf("connot join with thread2 \n");
    }
    printf("thread2 end \n");
}

void print_message_function(void *ptr){
    int i;
    for (i = 0; i<5; i++) {
        printf("%s:%d\n", (char *)ptr, i);
    }
}

/*
 线程间的同步与互斥：
 方式一：锁
 在主线程中初始化锁为解锁状态
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
 在编译时初始化锁为解锁状态
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;    锁初始化
 访问对象时的加锁操作与解锁操作
    pthread_mutex_lock(&mutex)     加锁
    pthread_mutex_unlock(&mutex)   释放锁
 
 线程的互斥函数有：
     pthread_mutex_init()       互斥函数的初始化
     pthread_mutex_lock()       互斥函数的锁定函数
     pthread_mutex_trylock()    互斥函数的预锁定函数
     pthread_mutex_unlock()     互斥函数的解锁函数
     pthread_mutex_destroy()    互斥函数的销毁函数
 
 1.锁保护的并不是我们的共享变量（或者说是共享内存），对于共享的内存而言，用户是无法直接对其保护的，因为那是物理内存，无法阻止其他程序的代码访问。事实上，锁之所以对关键区域进行了保护，在本例中，是因为所有线程都遵循了一个规则，那就是在进入关键区域钱加同一把锁，在退出关键区域释放同一把锁
 2.我们从上述运行结果中可以看到，加锁是会带来额外的开销的，加锁的代码其运行速度，明显比不加锁的要慢一些，所以，在使用锁的时候，要合理，在不需要对关键区域进行保护的场景下，我们便不要画蛇添足，为其加锁了
 */
long sharedi = 0;
void increse_num(void);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void ls15_func3(){
    int ret;
    pthread_t thrd1, thrd2, thrd3;
    
    ret = pthread_create(&thrd1, NULL, (void *)increse_num, NULL);
    ret = pthread_create(&thrd2, NULL, (void *)increse_num, NULL);
    ret = pthread_create(&thrd3, NULL, (void *)increse_num, NULL);
    
    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    pthread_join(thrd3, NULL);
    
    printf("sharedi = %ld \n", sharedi);
}

void increse_num(){
    long i, tmp;
    //不加锁
//    for (i = 0; i < 100000; i++) {
//        tmp = sharedi;
//        tmp = tmp + 1;
//        sharedi = tmp;
//    }
    for (i = 0; i < 100000; i++) {
        //加锁
        if (pthread_mutex_lock(&mutex) != 0) {
            perror("pthread_mutex_lock");
            exit(EXIT_FAILURE);
        }
        tmp = sharedi;
        tmp = tmp + 1;
        sharedi = tmp;
        //解锁
        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("pthread_mutex_unlock");
            exit(EXIT_FAILURE);
        }
    }
}

/*
 线程间的同步与互斥：
 方式二：信号量
 锁有一个很明显的缺点，那就是它只有两种状态：锁定与不锁定。
 信号量本质上是一个非负数的整数计数器，它也被用来控制对公共资源的访问。当公共资源增加的时候，调用信号量增加函数sem_post()对其进行增加，当公共资源减少的时候，调用函数sem_wait()来减少信号量。其实，我们是可以把锁当作一个0-1信号量的。
 它们是在/usr/include/semaphore.h中进行定义的，信号量的数据结构为sem_t, 本质上，它是一个long型整数
 相关函数
 在使用semaphore之前，我们需要先引入头文件#include <semaphore.h>
    初始化信号量： int sem_init(sem_t *sem, int pshared, unsigned int value);
        成功返回0，失败返回-1
    参数
        sem：指向信号量结构的一个指针
        pshared： 不是0的时候，该信号量在进程间共享，否则只能为当前进程的所有线程们共享
        value：信号量的初始值
    信号量减1操作，当sem=0的时候该函数会堵塞 int sem_wait(sem_t *sem);
        成功返回0，失败返回-1
    参数
        sem：指向信号量的一个指针
    信号量加1操作 int sem_post(sem_t *sem);
        参数与返回同上
    销毁信号量 int sem_destroy(sem_t *sem);
        参数与返回同上
 
 https://blog.csdn.net/gatieme/article/details/50994533
 
 在OS X下，sem_init失败
 OS X不支持创建无名的信号量，只能使用sem_open创建有名的信号量。
 创建：
 sem_t* psemaphore = sem_open("/mysem",O_CREAT, S_IRUSR | S_IWUSR, 10);
 销毁：
 sem_close(psemaphore);
 sem_unlink("/mysem");
 https://blog.csdn.net/xiadasong007/article/details/8534184
 */
int stack[MAXSIZE];
int size = 0;
sem_t *sem;

//生产者
void provide_data(void);
//消费者
void handle_data(void);

void ls15_func4(){
    pthread_t provider, handler;
    
//    int res = sem_init(&sem, 0, 0);
    sem = sem_open("/mysem", O_CREAT, S_IRUSR|S_IWUSR, 0);
//    printf("sem_init res: %d \n", res);
    pthread_create(&handler, NULL, (void *)handle_data, NULL);
    pthread_create(&provider, NULL, (void *)provide_data, NULL);
    
    pthread_join(provider, NULL);
    pthread_join(handler, NULL);
    
    //    sem_destroy(&sem);
    sem_close(sem);
    sem_unlink("/mysem");
}

// 生产者
void provide_data(){
    int i;
    for (i = 0; i < MAXSIZE; i++) {
        stack[i] = i;
        sem_post(sem);//为信号量加1
        sleep(1);
    }
}

// 消费者
void handle_data(){
    int i;
    while ((i = size++) < MAXSIZE) {
        sem_wait(sem);
        printf("乘法: %d * %d = %d \n", stack[i], stack[i], stack[i] * stack[i]);
//        sleep(1);
    }
}

//pthread_mutex_t test_mutex;
//int testi = 0;
//int testis[10 * 1000];
//int count = 0;
//
//void testfun(){
//    testis[testi] = testi * 2;
//    usleep(1000);
//    testi++;
//}
//
//void thread_func(){
//    int m_count = 0;
//    while (m_count < 1000) {
//        pthread_mutex_lock(&test_mutex);
//        testfun();
//        pthread_mutex_unlock(&test_mutex);
//        m_count++;
//    }
//}
//
//void ls15_func3(){
//    pthread_t t[10];
//    pthread_mutex_init(&test_mutex, NULL);
//    int i;
//    for (i = 0; i < MAXSIZE; i++) {
//        if (pthread_create(&t[i], NULL, (void *)thread_func, NULL) == -1) {
//            printf("create Thread error! \n");
//            exit(1);
//        }
//    }
//
//    for (i = 0; i < MAXSIZE; i++) {
//        pthread_join(t[i], NULL);
//    }
//
//    pthread_mutex_destroy(&test_mutex);
//    for (i = 0; i < 10000; i++) {
//        if (testis[i] != i * 2) {
//            printf("第%d个数据出错！: %d \n", i, testis[i]);
//        }
//    }
//}
