//
//  ecgHelp.h
//  CPractices2
//
//  Created by Jun on 2018/9/30.
//  Copyright © 2018年 Jun. All rights reserved.
//

#ifndef ecgHelp_h
#define ecgHelp_h

#pragma pack(1)

#include <stdio.h>

typedef unsigned char Byte;
typedef unsigned short Word;

/*
 版本号(6字节)
 */
struct TVersion{
    Word major;     //主版本号
    Word minor;     //次版本号
    Word revision;  //修订号
};

/*
 文件头信息(32字节)
 */
struct THeadInfo {
    char headTag[8];   //文件头标识
    Byte infoType;      //文件类型
    struct TVersion version;    //版本号
    char reserve[17];   //保留
};

/*
 设备信息(128字节)
 */
struct TMachineInfo{
    char machineID[20];     //设备号
    char authID[20];        //序列号
    char imei[15];          //IMEI
    char sim[20];           //SIM
    char reserve[128 - 75]; //保留
};

/*
 用户信息(384字节)
 */
struct TUserInfo{
    char userName[16];          //姓名
    char userSex[2];            //性别
    Byte userAge;               //年龄
    char userTel[16];           //手机
    char userAddress[64];       //地址
    char reserve[13];           //保留
    char relativeName1[16];     //联系人1
    char relativeTel1[16];      //联系人1电话
    char relativeName2[16];     //联系人2
    char relativeTel2[16];      //联系人2电话
    char relativeName3[16];     //联系人3
    char relativeTel3[16];      //联系人3电话
    char helpMsg[40];           //求助内容
    char reserve1[384 - 248];   //保留
};

/*
 导联信息(128字节)
 */
struct TLeadInfo{
    //导联V1  V2  V3  V4  V5  V6  V7  V8  V9 I II  III avR avL avF V3R V4R  V5R  V6R Pacer
    //编码 1  2   3   4   5   6   7   8   9  10 11  12  13  14  15  16  17   18  19   20
    //导联列表,如LeadTable[0]=1,[1]=5,[2]=15，则表示有V1,V5,avF三个导联，且顺序如此
    Byte leadTable[32];         //导联列表
    Byte plus;                  //增益
    Byte dataSize;              //数据精度
    Byte blockHeadSize;         //块头大小48
    Byte blockSecs;             //多少秒一个数据块，默认为1
    Word sampleRate;            //采样率，默认为250
    float voltageAccuracy;      //电压精度
    char reserve[128 - 42];     //保留
};

/*
 心电文件头信息(1024字节)
 */
struct TDataHead{
    struct THeadInfo headInfo;          //头信息
    struct TMachineInfo machineInfo;    //设备信息
    struct TUserInfo userInfo;          //用户信息
    struct TLeadInfo leadInfo;          //导联信息
    
    char reserve[1024 - 672];           //保留
};
#endif /* ecgHelp_h */

