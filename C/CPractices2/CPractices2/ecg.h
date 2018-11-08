//
//  ecg.h
//  CPractices2
//
//  Created by Jun on 2018/11/6.
//  Copyright © 2018年 Jun. All rights reserved.
//

#ifndef ecg_h
#define ecg_h

#include <stdio.h>
#include <stdio.h>
#include <malloc/_malloc.h>
#include <string.h>

void run(int data1);
//恢复阀值
void reset_fz(void);
void new_run(float data1);
#endif /* ecg_h */
