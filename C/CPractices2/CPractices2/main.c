//
//  main.c
//  CPractices2
//
//  Created by Jun on 2018/9/13.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecgHelp.h"
#include "ecg.h"

void readEcgFile(char *filename){
    int sampleRate = 250;       //采样率
    int blockHeadSize = 48;     //块头大小48
    int blockSecs = 1;          //多少秒一个数据块，默认为1
    int dataSize = 2;           //数据精度

    FILE * ecgFp = fopen(filename, "rb");
    fseek(ecgFp,0L,SEEK_END);
    long flen = ftell(ecgFp);
    fseek(ecgFp, 0L, SEEK_SET);
    printf("sizeof file:%ld \n", flen);
    struct TDataHead dataHead;
    printf("dataHead size: %lu \n", sizeof(dataHead));
    printf("headInfo:%lu, machineInfo:%lu, userInfo:%lu, leadInfo:%lu \n", sizeof(dataHead.headInfo), sizeof(dataHead.machineInfo), sizeof(dataHead.userInfo), sizeof(dataHead.leadInfo));
    printf("headTag:%lu, infoType:%lu, version:%lu, reserve:%lu \n", sizeof(dataHead.headInfo.headTag), sizeof(dataHead.headInfo.infoType), sizeof(dataHead.headInfo.version), sizeof(dataHead.headInfo.reserve));
    printf("headTag:%p, infoType:%p, version:%p, reserve:%p \n", &dataHead.headInfo.headTag, &dataHead.headInfo.infoType, &dataHead.headInfo.version, &dataHead.headInfo.reserve);
    
    size_t size = 0;
    size = fread(&dataHead, sizeof(dataHead), 1, ecgFp);
    printf("read dataHead lead: %ld \n", size);
    printf("patient info \n name:%s  \n sex: %s  \n age:%d \n", dataHead.userInfo.userName, dataHead.userInfo.userSex, dataHead.userInfo.userAge);
    printf("sampleRate: %d \n", dataHead.leadInfo.sampleRate);
    printf("voltageAccuracy:%f \n", dataHead.leadInfo.voltageAccuracy);
    
    if (dataHead.leadInfo.sampleRate != 0) {
        sampleRate = dataHead.leadInfo.sampleRate;
    }
    if (dataHead.leadInfo.blockHeadSize != 0) {
        blockHeadSize = dataHead.leadInfo.blockHeadSize;
    }
    if (dataHead.leadInfo.blockSecs != 0) {
        blockSecs = dataHead.leadInfo.blockSecs;
    }
    if (dataHead.leadInfo.dataSize != 0) {
        dataSize = dataHead.leadInfo.dataSize;
    }
    //计算导联数
    int leadNum = 0;
    int i;
    for (i = 0; i < sizeof(dataHead.leadInfo.leadTable); i++) {
        if (dataHead.leadInfo.leadTable[i] != 0) {
            leadNum++;
        }
    }
    
    int blockDataLength = leadNum * sampleRate * blockSecs * dataSize + blockHeadSize;
    printf("leadNum: %d, sampleRate: %d, dataSize: %d, blockSecs: %d \n", leadNum, sampleRate, dataSize, blockSecs);
    printf("bockDataLenght:%d \n", blockDataLength);
    
    fseek(ecgFp, 0L, 1024);
    Byte data[blockDataLength];
    memset(&data, 0, sizeof(data));

    long arrLen = (flen - 1024) / blockDataLength * sampleRate;
    printf("arrLen %ld \n", arrLen);
    float tempData[leadNum][arrLen];
    int k = 0;
    while (fread(&data, sizeof(data), 1, ecgFp)) {
        for (int i = 0; i < sampleRate; i++) {
            for (int j = 0; j < leadNum; j++) {
                if (dataSize == 1) {
                    tempData[j][k * sampleRate + i] = data[blockHeadSize + i * leadNum + j] * dataHead.leadInfo.voltageAccuracy;
                } else if (dataSize == 2) {
                    short p = (data[blockHeadSize + (i * leadNum + j) * 2] & 0xff) |
                    (data[blockHeadSize + (i * leadNum + j) * 2 + 1] << 8 & 0xff00);
                    tempData[j][k * sampleRate + i] = p * dataHead.leadInfo.voltageAccuracy;
                }
            }
        }
        k++;
        memset(&data, 0, sizeof(data));
    }
    
    reset_fz();
    if (sampleRate == 500) {
        for (int i = 0; i <arrLen; i++) {
//            printf("%d ", tempData[1][i]);
            if (i % 2 == 0) {
//                run(tempData[1][i]);
                new_run(tempData[1][i]);
            }
        }
    } else if (sampleRate == 250){
        for (int i = 0; i < arrLen; i++) {
//            printf("%f ", tempData[1][i]);
//            run(tempData[1][i]);
            new_run(tempData[1][i]);
        }
    }
    fclose(ecgFp);
}


/*
 心电解析测试
 */
int main(int argc, const char * argv[])
{
    char *fileName = "/Users/jun/Desktop/sample3.data";
    readEcgFile(fileName);
    return 0;
}
