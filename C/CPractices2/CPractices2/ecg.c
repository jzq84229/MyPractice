//
//  ecg.c
//  CPractices2
//
//  Created by Jun on 2018/11/6.
//  Copyright © 2018年 Jun. All rights reserved.
//

#include "ecg.h"

#define WIN32
#define SAMPLE 250
#define BUFFSIZE (SAMPLE * 1)

#define true 0
#define false 1

struct _heart_alarm
{
    char alarm_num[8];
    char pos[16];
    char szMsg[128];
    struct _heart_alarm *next;
};

struct _heart_alarm *pHead = NULL;
struct _heart_alarm *pCur  = NULL;

int __nTotalGanRaoNums=0;
int WarningCnt=0;

int    g_lastmax;                    //上次是否取的是极大值 false上次取的是极小,true上次是极大
int    g_lasterr;                    //上次有一个反向R波出现
/////////////////////////////////////////////////阀值////////////////////////////////////////////
int MAX_HR = 150;
int MIN_HR = 45;
int ZFGS_HR=0;//阵发性心动过速的心律计数器
int JSSXZZXL_HR=0;//加速性室性自主心律计数器
int MAX_ST = 40;//0.20mv+0.2
int MIN_ST = -30;//0.15mv-0.2
int SX_MAX_COUNT = 10;//室性早搏报警阀值
int SS_MAX_COUNT = 14;//室上性早搏报警阀值
int SX2_MAX_COUNT = 8;//室性二联律报警阀值
int SX3_MAX_COUNT = 8;//室性三联律报警阀值
int SXCD_MAX_COUNT = 8;//室性成对报警阀值
int SXX2_MAX_COUNT = 8;//室上性二联律报警阀值
int SXX3_MAX_COUNT = 8;//室上性三联律报警阀值
int SXXCD_MAX_COUNT = 8;//室上性成对报警阀值

int ST_HIGH_MAX_COUNT = 8;//ST抬高报警阀值
int ST_LOW_MAX_COUNT = 8;//ST抬低报警阀值

int CDZZ_MAX_COUNT = 20;//传导阻滞报警阀值

int SXZZXV_MAX_COUNT=8;//室性自主心律报警阀值
int MIN_SXZZXV=50;  //室性自主心律报警阀值
float MAX_TB_TIME=2.5;//停搏的阀值2.5秒

float MIN1_RR = 0.80;
float MIN2_RR = 0.33;//RONT 的阀值
//float MIN2_RR = 0.20;
float MAX_QRS = 0.11;//室性和室上性的QRS波形的宽度阀值
float MAX_RS = 1.5; //RS为QR的最大的倍数，防止s波的不正确性

////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char g_buf[BUFFSIZE];    //当前诊断系统用的数据缓冲
unsigned char g_ecg_buf[SAMPLE];    //提前准备的ECG数据缓冲
unsigned char g_bufmin;
unsigned char g_bufmax;
int g_ecg_count=0;//提前准备缓冲区的ECG数据计数器
int g_count=0;                        //总的数据记数
int g_upval=0;                        //心电图上阀值
int g_downval=0;                        //心电图下阀值
int g_maxcount=0;                        //极大数据出现在总数的位置
int g_oldmaxcount=0;                    //上一次R波位置
int g_maxposbuf=0;                    //极大数据出现在缓冲的位置，用于提取数据
int g_mincount=0;
int g_oldmincount=0;
int g_minposbuf;
int g_errcount;                        //偶尔出现的反向波形
int g_errposbuf;

int RR[8];     //R间期
int QRS[8];     //QRS波群宽度
int ST[8];     //ST电压
int RR_count;//RR计数
int ARR;     //8个RR的平均ARR
int cliff_ARR=0;//及时ARR，每来一个做平均
int cliff_QRS=0;//及时QRS，每来一个做平均
int HR;         //心律
int g_warr1=0; //室性早博计数
int g_warr1count;
int g_warr2=0; //室上性早博计数
int g_warr2count;
int iTotalMissRCount=0;//总的漏检R波计数器
int g_total_sxzb_count=0;//总的室性早搏计数器
int g_total_sxxzb_count=0;//总的室上性早搏计数器
int g_total_tb_count=0;//总的停搏计数器
int AQR=0;//平均QR的高度
int ARS=0;//平均RS的高度

int g_warr3=0; //室性二联律计数
int g_warr3count;
int g_warr4[20]; //室性三联律计数
int g_warr4count;
int g_warr5[20]; //室性成对计数
int g_warr5count;

int g_warr6=0; //室上性二联律计数
int g_warr6count;
int g_warr7[20]; //室上性三联律计数
int g_warr7count;
int g_warr8[20]; //室上性成对计数
int g_warr8count;
int g_warr9=0; //ST抬高计数
int g_warr9count;
int g_warr10=0; //ST抬低计数
int g_warr10count;
int g_warr11=0; //加速性室性自主心律
int g_warr11count;
int g_warr12=0; //室性自主心律计数
int g_warr12count;
int g_lastcount; //上次异常发生的位置
int g_ganraod[20];
int g_ganraocount=0;

int g_basex;
int g_basey;

int g_maxganraocnt=0;//干扰计数器
int R_count=0; //总探测到的心搏数量
int g_MaxHR=1; //记录到的最大心率
int g_MinHR=300; //记录到的最小心率

int z_max=0;
int z_min=255;
int z_zoom=0;
int z_base = 0;

#ifdef WIN32

#ifndef MMI_ON_WIN32
#define PC_VERSION 1
#endif
#define kal_bool int
#define U8 unsigned char
#define S8 char
int TotalWarrCnt=0;
#define RGB(r,g,b) ((unsigned long)(((U8)(r)|((unsigned long)((U8)(g))<<8))|(((unsigned long)(U8)(b))<<16)))
extern void winAddDrawData(int pos,int y, unsigned long color, const char* str);
extern void winAddLineData(int x0, int y0, int x1, int y1, unsigned long color);
void AddEcgInfo(char* szMsg1, char* szMsg2, char* szMsg3);
int get_MaxHR() { return g_MaxHR;}
int get_MinHR() { return g_MinHR;}
int get_TotalWarrCnt(){ return TotalWarrCnt;}
int get_iTotalMissRCount(){ return iTotalMissRCount;}
int get_total_tb_count(){ return g_total_tb_count;}
int get_total_sxzb_count(){ return g_total_sxzb_count;}
int get_total_sxxzb_count() { return g_total_sxxzb_count;}
int get__nTotalGanRaoNums() { return __nTotalGanRaoNums;}
int get_RR_count(){ return RR_count;}
int get_R_count() { return R_count;}

#endif

unsigned char  RS_Height=0;//RS之间的高度

//用来减少回扫描的次数
int rescan_oldmaxcount=0;//上次回扫描时的R波的位置
int rescan_oldstart=0;

void winAddDrawData(int pos,int y, unsigned long color, const char* str){
    
}
void winAddLineData(int x0, int y0, int x1, int y1, unsigned long color){
    
}

void DrawHart()
{
}

unsigned char ecg_ZOOM(unsigned char data)
{
    if(z_zoom > 1 )
    {
        int t;
        t =  ((data - z_min) << (z_zoom - 1)) + z_base;
        if(t <0 ) t = 0;
        else if(t > 255) t = 255;
        
        return (unsigned char)t;
    }//*/
    return data;
}

//恢复阀值
void reset_fz()
{
    int i;
    MAX_HR = 150;
    MIN_HR = 45;
    MAX_ST = 40;//0.20mv+0.2
    MIN_ST = -30;//0.15mv-0.2
    SX_MAX_COUNT = 10;//室性早搏报警阀值
    SS_MAX_COUNT = 14;//室上性早搏报警阀值
    SX2_MAX_COUNT = 8;//室性二联律报警阀值
    SX3_MAX_COUNT = 8;//室性三联律报警阀值
    SXCD_MAX_COUNT = 8;//室性成对报警阀值
    SXX2_MAX_COUNT = 8;//室上性二联律报警阀值
    SXX3_MAX_COUNT = 8;//室上性三联律报警阀值
    SXXCD_MAX_COUNT = 8;//室上性成对报警阀值
    
    ST_HIGH_MAX_COUNT = 8;//ST抬高报警阀值
    ST_LOW_MAX_COUNT = 8;//ST抬低报警阀值
    CDZZ_MAX_COUNT = 20;//传导阻滞报警阀值
    SXZZXV_MAX_COUNT=8;//室性自主心律报警阀值
    MIN_SXZZXV=50;  //室性自主心律报警阀值
    MAX_TB_TIME=2.5;//停搏的阀值2.5秒
    
    MIN1_RR = 0.85;
    MIN2_RR = 0.33;//RONT 的阀值
    MAX_QRS = 0.11;
    
    g_MaxHR=1; //记录到的最大心率
    g_MinHR=300; //记录到的最小心?
    ZFGS_HR=0;//振发心动过速的心律阀值
    JSSXZZXL_HR=0;//加速性室性自主心律的阀值
    
    g_warr1=0;//室性早搏
    g_warr2=0;//室上性早搏
    g_warr3=0;//室性二联律
    g_warr6=0;//室上性二联律
    g_warr9=0;//ST抬高
    g_warr10=0;//ST压低
    g_warr11=0;//加速性自主心律
    g_warr12=0;//室性自主心律
    
    AQR=0;//平均QR的高度
    ARS=0;//平均RS的高度
    
    iTotalMissRCount=0;//总的漏检R波计数器
    g_total_sxzb_count=0;//总的室性早搏计数器
    g_total_sxxzb_count=0;//总的室上性早搏计数器
    g_total_tb_count=0;//总的停搏次数
    
#ifdef PC_VERSION
    TotalWarrCnt=0;
#endif
    
    //初始化准备缓冲区
    for (i=0;i<BUFFSIZE;i++)
        g_ecg_buf[i]=0;
    g_ecg_count=0;
    
    g_basex =3*SAMPLE;
}


//准备数据
void BuffECGData(int data,int * out_data)
{
    if(g_ecg_count>=SAMPLE)
        *out_data=g_ecg_buf[g_ecg_count%SAMPLE];
    g_ecg_buf[g_ecg_count%SAMPLE]=data;
    g_ecg_count++;
}

void reset()
{
    static int  last_reset_g_count=0;
    last_reset_g_count=g_count;
    R_count += (RR_count + 1);
    
    g_count = 0;
    g_lastcount=0;
    g_upval = 0;
    g_downval = 0;
    g_lastmax = false;
    g_lasterr = false;
    g_maxcount=0;
    g_oldmaxcount=0;
    g_maxposbuf=0;
    g_mincount=0;
    g_oldmincount=0;
    g_minposbuf=0;
    g_errcount = 0;
    
    RR[0] = 0;    RR[1] = 0;    RR[2] = 0;    RR[3] = 0;
    RR[4] = 0;    RR[5] = 0;    RR[6] = 0;    RR[7] = 0;
    ST[0] = 0;    ST[1] = 0;    ST[2] = 0;    ST[3] = 0;
    ST[4] = 0;    ST[5] = 0;    ST[6] = 0;    ST[7] = 0;
    QRS[0] = 0;    QRS[1] = 0;    QRS[2] = 0;    QRS[3] = 0;
    QRS[4] = 0;    QRS[5] = 0;    QRS[6] = 0;    QRS[7] = 0;
    
    RR_count = 0;
    ARR=0;
    HR=0;
    g_warr1count = 0;    g_warr2count = 0;    g_warr3count = 0;
    g_warr4count = 0;    g_warr5count = 0;    g_warr6count = 0;
    g_warr7count = 0;    g_warr8count = 0;    g_warr9count = 0;
    g_warr10count = 0;    g_warr11count = 0;    g_warr12count = 0;
    
    g_warr1=0;
    g_warr2=0;
    g_warr3=0;//室性二联律
    g_warr6=0;//室上性二联律
    g_warr9=0;//ST抬高
    g_warr10=0;//ST压低
    g_warr11=0;//加速性自主心律
    g_warr12=0;//室性自主心律
    
    g_lastcount = 0;
    g_ganraocount=0;
    g_basex =0;
    g_basey = 0;
    
    RS_Height=0;
    
    cliff_ARR=0;
    cliff_QRS=0;
    rescan_oldmaxcount=0;
    rescan_oldstart=0;
#ifndef PC_VERSION
    WarningCnt=0; //可以修改代码按类型报异常
#endif
}

void resetex()
{
    int count = g_basex + g_count;
    reset();
    g_basex = count;
    //干扰计数
    __nTotalGanRaoNums++;
}

void resetex1()
{
    int count = g_basex + g_count;
    reset();
    g_basex = count;
}


#ifdef PC_VERSION
void Notify(int pos, const char* str)
{
}
#else
#define Notify
#endif


int warring(int pos, const char* str)
{
    printf("warring ...");
#ifdef PC_VERSION
    char strMsg1[16] = {'\0'};
    char strMsg2[16] = {'\0'};
    char strMsg3[128] = {'\0'};
    
    //两分钟内不连续报警
    if(abs(g_count - g_lastcount) < SAMPLE*120){
        double fGap1,fGap2,fGap3;
        fGap1=(double)(g_count-g_lastcount);
        fGap2=60*SAMPLE;
        fGap3=fGap1/fGap2;
        return 0;
    }
    g_lastcount = g_count;
    //winLog("报警%04d: pos = %d, %s\n", TotalWarrCnt+1,pos+g_basex,str);
    ////////////////////////////////////////////////////////////////////////////// xiegege
//    sprintf(strMsg1,"%04d", TotalWarrCnt+1);
//    sprintf(strMsg2,"%d", pos+g_basex);
//    sprintf(strMsg3,"%s", str);
    printf(strMsg1,"%04d", TotalWarrCnt+1);
    printf(strMsg2,"%d", pos+g_basex);
    printf(strMsg3,"%s", str);
    AddEcgInfo(strMsg1, strMsg2, strMsg3);
    //////////////////////////////////////////////////////////////////////////////
    winAddDrawData(pos+g_basex,30, RGB(255, 0, 0), strMsg3);
    TotalWarrCnt++;
    return 1;
#else
    char strMsg[100];
    //两次报警时间太短，忽略不报警2分钟
    if(g_count-g_lastcount<=SAMPLE*120){
        //sprintf(strMsg,"%s（忽略)",str);
        //WriteLog(strMsg);//记录Log文件
        return 0;
    }
    g_lastcount = g_count;
    WarningCnt++; //可以修改代码按类型报异常
    sprintf(strMsg,"%d %s（报警)",pos+g_basex,str);
    WriteLog(strMsg);//记录Log文件
    return 1;
#endif
    
}



//室性早搏报警处理
void warr1(int pos)
{
    char buf[256];
    g_total_sxzb_count++;//总的室性早搏计数器
    if(g_warr1==0){
        //第一次进入
        g_warr1=pos;
        g_warr1count=1;
        return;
    }
    g_warr1count++;
    
    if(g_warr1count >= SX_MAX_COUNT && pos - g_warr1 <= SAMPLE * 60)
    {
        sprintf(buf, "室性早搏一分钟内计数达到或超过%d次,HR=%d\n", SX_MAX_COUNT,HR);
        warring(pos, buf);
        SX_MAX_COUNT=SX_MAX_COUNT+5;
        if(SX_MAX_COUNT > 40)
            SX_MAX_COUNT=40;
        g_warr1=0;
        g_warr1count = 0;
        return;
    }
    //一分钟内的计数还不够阀值，清空计数
    if((pos - g_warr1) > SAMPLE * 60  ){
        //重新计数
        g_warr1=pos;
        g_warr1count = 1;
    }
    return;
}

//室上性早搏报警处理
void warr2(int pos)
{
    char buf[256];
    
    g_total_sxxzb_count++;//总的室上性早搏计数器
    if(g_warr2==0){
        //第一次进入
        g_warr2=pos;
        g_warr2count=1;
        return;
    }
    
    //g_warr2 = pos;
    g_warr2count++;
    
    if(g_warr2count >= SS_MAX_COUNT && pos - g_warr2 <= SAMPLE * 60)
    {
        sprintf(buf, "室上性早搏一分钟内计数达到或超过%d次,HR=%d\n", SS_MAX_COUNT,HR);
        warring(pos, buf);
        SS_MAX_COUNT=SS_MAX_COUNT+5;
        if(SS_MAX_COUNT > 40)
            SS_MAX_COUNT=40;
        g_warr2=0;
        g_warr2count = 0;
        return;
    }
    //一分钟内的计数还不够阀值，清空计数
    if((pos - g_warr2) >= SAMPLE * 60 ){
        //重新计数
        g_warr2=pos;
        g_warr2count = 1;
    }
    return;
}

//室性二联率报警处理处理
void warr3(int pos)
{
    char buf[256];
    
    if(g_warr3==0){
        //第一次进入
        g_warr3=pos;
        g_warr3count=1;
        return;
    }
    g_warr3count++;
    
    if(g_warr3count >= SX2_MAX_COUNT && pos - g_warr3 <= SAMPLE * 60)
    {
        sprintf(buf, "室性二联律一分钟内计数达到或超过%d次,HR=%d\n", SX2_MAX_COUNT,HR);
        warring(pos, buf);
        SX2_MAX_COUNT=SX2_MAX_COUNT+5;
        if(SX2_MAX_COUNT > 40)
            SX2_MAX_COUNT=40;
        g_warr3=0;
        g_warr3count = 0;
        return;
    }
    //一分钟内的计数还不够阀值，清空计数
    if((pos - g_warr3) >= SAMPLE * 60 ){
        //重新计数
        g_warr3=pos;
        g_warr3count = 1;
    }
    return;
}

//室性三联率报警处理处理
void warr4(int pos)
{
    char buf[256];
    return;
    
    g_warr4[g_warr4count % SX3_MAX_COUNT] = pos;
    g_warr4count++;
    if(g_warr4count >= 8&& pos - g_warr4[g_warr4count%SX3_MAX_COUNT] <= SAMPLE * 60)
    {
        sprintf(buf, "室性三联律,一分钟内计数达到或超过%d次", SX3_MAX_COUNT);
        warring(pos, buf);
        g_warr4count = 0;
    }
}

//室性成对报警处理
void warr5(int pos)
{
    char buf[256];
    return;
    
    g_warr5[g_warr5count % SXCD_MAX_COUNT] = pos;
    g_warr5count++;
    if(g_warr5count >= 8 && pos - g_warr5[g_warr5count%SXCD_MAX_COUNT] <= SAMPLE * 60)
    {
        sprintf(buf, "室性成对,一分钟内计数达到或超过%d次", SXCD_MAX_COUNT);
        warring(pos, buf);
        g_warr5count = 0;
    }
}

//室性二联率报警处理处理
void warr6(int pos)
{
    char buf[256];
    if(g_warr6==0){
        //第一次进入
        g_warr6=pos;
        g_warr6count=1;
        return;
    }
    g_warr6count++;
    
    if(g_warr6count >= SXX2_MAX_COUNT && pos - g_warr6 <= SAMPLE * 60)
    {
        sprintf(buf, "室性二联律一分钟内计数达到或超过%d次,HR=%d\n", SXX2_MAX_COUNT,HR);
        warring(pos, buf);
        SXX2_MAX_COUNT=SXX2_MAX_COUNT+5;
        if(SXX2_MAX_COUNT > 40)
            SXX2_MAX_COUNT=40;
        g_warr6=0;
        g_warr6count = 0;
        return;
    }
    //一分钟内的计数还不够阀值，清空计数
    if((pos - g_warr6) >= SAMPLE * 60  ){
        //重新计数
        g_warr6=pos;
        g_warr6count = 1;
    }
    return;
}


//室上性三联率报警处理处理
void warr7(int pos)
{
    char buf[256];
    return;
    
    g_warr7[g_warr7count % SXX3_MAX_COUNT] = pos;
    
    g_warr7count++;
    if(g_warr7count >= 8&& pos - g_warr7[g_warr7count%SXX3_MAX_COUNT] <= SAMPLE * 60)
    {
        sprintf(buf, "室上性三联律,一分钟内计数达到或超过%d次", SXX3_MAX_COUNT);
        warring(pos, buf);
        g_warr7count = 0;
    }
}

//室上性成对报警处理
void warr8(int pos)
{
    char buf[256];
    return;
    
    g_warr8[g_warr8count % SXXCD_MAX_COUNT] = pos;
    
    g_warr8count++;
    if(g_warr8count >= 8 && pos - g_warr8[g_warr8count%SXXCD_MAX_COUNT] <= SAMPLE * 60)
    {
        sprintf(buf, "室上性成对,一分钟内计数达到或超过%d次", SXXCD_MAX_COUNT);
        warring(pos, buf);
        g_warr8count = 0;
    }
}

//ST抬高报警处理
void warr9(int pos)
{
    char buf[256];
    
    if(g_warr9==0){
        //第一次进入
        g_warr9=pos;
        g_warr9count=1;
        return;
    }
    g_warr9count++;
    if(g_warr9count >= ST_HIGH_MAX_COUNT && pos - g_warr9 <= SAMPLE * 60)
    {
        sprintf(buf, "ST抬高%d,计数超过%d次",MAX_ST, ST_HIGH_MAX_COUNT);
        warring(pos, buf);
        g_warr9=0;
        g_warr9count = 0;
        MAX_ST += 20;//每次增加0.2mv
        return;
    }
    //一分钟内的计数还不够阀值，清空计数
    if((pos - g_warr9) >= SAMPLE * 60 ){
        //重新计数
        g_warr9=pos;
        g_warr9count = 1;
    }
    return;
}

//ST抬低报警处理
void warr10(int pos)
{
    char buf[256];
    
    if(g_warr10==0){
        //第一次进入
        g_warr10=pos;
        g_warr10count=1;
        return;
    }
    g_warr10count++;
    if(g_warr10count >= ST_LOW_MAX_COUNT && pos - g_warr10 <= SAMPLE * 60)
    {
        sprintf(buf, "ST压低%d一分钟内计数达到或超过%d次",MIN_ST,ST_LOW_MAX_COUNT);
        warring(pos, buf);
        g_warr10=0;
        g_warr10count = 0;
        MIN_ST -= 20;//每次减少0.2mv
        return;
    }
    //一分钟内的计数还不够阀值，清空计数
    if((pos - g_warr10) >= SAMPLE * 60 ){
        //重新计数
        g_warr10=pos;
        g_warr10count = 1;
    }
    return;
}

//加速性室性自主心律
void warr11(int pos)
{
    char buf[256];
    
    if(g_warr11==0){
        //第一次进入
        g_warr11=pos;
        g_warr11count=1;
        return;
    }
    g_warr11count++;
    if(g_warr11count >= SXZZXV_MAX_COUNT && pos - g_warr11 <= SAMPLE * 60)
    {
        sprintf(buf, "加速性自主心律：一分钟内计数达到或超过%d次", SXZZXV_MAX_COUNT);
        warring(pos, buf);
        //调整室性自主心律阀值
        g_warr11=0;
        g_warr11count = 0;
        JSSXZZXL_HR=JSSXZZXL_HR+2;
        return;
    }
    //一分钟内的计数还不够阀值，清空计数
    if((pos - g_warr11) >= SAMPLE * 60 ){
        //重新计数
        g_warr11=pos;
        g_warr11count = 1;
    }
    return;
}

//室性自主心律
void warr12(int pos)
{
    char buf[256];
    
    if(g_warr12==0){
        //第一次进入
        g_warr12=pos;
        g_warr12count=1;
        return;
    }
    g_warr12count++;
    if(g_warr12count >= SXZZXV_MAX_COUNT && pos - g_warr12 <= SAMPLE * 60)
    {
        sprintf(buf, "室性自主心律：一分钟内计数达到或超过%d次", SXZZXV_MAX_COUNT);
        warring(pos, buf);
        //调整室性自主心律阀值
        if(MIN_SXZZXV>5)
            MIN_SXZZXV=MIN_SXZZXV-5;
        g_warr12=0;
        g_warr12count = 0;
        return;
    }
    //一分钟内的计数还不够阀值，清空计数
    if((pos - g_warr12) >= SAMPLE * 60 ){
        //重新计数
        g_warr12=pos;
        g_warr12count = 1;
    }
    return;
}


void ganrao(int pos)
{
    g_ganraod[g_ganraocount % 5] = pos;
    g_ganraocount++;
    if(g_ganraocount >= 5)
    {
        int t = pos - g_ganraod[g_ganraocount % 5] ;
        if(t < SAMPLE/2)
        {
            Notify(pos, "干扰多....");
            resetex();
        }
    }
}

int testRR( int RRindex)
{
    if(RR[RRindex] < ARR * MIN1_RR)
    {
        if(QRS[RRindex - 1] > MAX_QRS * SAMPLE)
            return 1; //室性的
        else
            return 2; //室上性的
    }
    return 0; //正常
}

int testST(int pos)
{
    int st = ST[0];
    if(ARR && ST[0] >MAX_ST && ST[1] > MAX_ST && ST[2] > MAX_ST
       && ST[3] > MAX_ST&& ST[4] > MAX_ST&& ST[5] > MAX_ST&& ST[6] > MAX_ST &&  ST[7] > MAX_ST )
    {
        warr9(pos);//ST抬高报警计数
        return 1;
    }
    
    if(ARR && ST[0] < MIN_ST && ST[1]< MIN_ST && ST[2] < MIN_ST
       && ST[3] < MIN_ST && ST[4] < MIN_ST && ST[5] < MIN_ST && ST[6] < MIN_ST && ST[7] < MIN_ST)
    {
        warr10(pos);//ST抬低报警计数
        return 2;
    }
    return 0;
}

void panduan(int pos)
{
    int aqrs;
    int maxrr;
    int minrr;
    int i;
    int warn_result;
    char strMsg[100];
    
    maxrr=RR[0];
    minrr=RR[0];
    
    for(i =1; i< 8; i++)
    {
        if(RR[i] > maxrr) maxrr = RR[i];
        if(RR[i] < minrr) minrr = RR[i];
    }
    ARR = (RR[7] + RR[6]+ RR[5] + RR[4] + RR[3] + RR[2] + RR[1] + RR[0] - minrr - maxrr) / 6;
    HR = SAMPLE * 60 / ARR;
    
    //记录最大心率和最大心律
    if(HR>g_MaxHR)         g_MaxHR=HR;
    if(HR<g_MinHR)         g_MinHR=HR;
    
    if(HR > MAX_HR)
    {
        warn_result=warring(pos,"心动过速");
        if(warn_result!=0){
            MAX_HR=MAX_HR+5;
            if(MAX_HR>220)
                MAX_HR=220;
        }
        return;
    }
    if(HR < MIN_HR)
    {
        sprintf(strMsg,"HR=%d，心动过缓",HR);
        warn_result=warring(pos,strMsg);
        if(warn_result!=0){
            MIN_HR=MIN_HR-5;
            if(MIN_HR<20)
                MIN_HR=20;
        }
        return;
    }
    
    if(RR[4] < MIN1_RR * ARR)
    {
        if(QRS[4] > MAX_QRS * SAMPLE)
        {
            //室性的
            warr1(pos);//先做早搏计数
            
            //20100705 按照连双要求，三联律不报警
            if(testRR(5) == 0 && testRR(6) == 1 && testRR(7) == 0)
            {
                //warring(pos,"室性二联律");
                warr3(pos);
                return;
            }
            //20100705 按照连双要求，成对不报警
            //20100705 RONT按照连双要求把RONT的逻辑和报警暂时关闭
            if((RR[1] +RR[2] +RR[3])<=1.5*SAMPLE)
            {
                if(ZFGS_HR==0){
                    warring(pos,"阵发型室性心动过速");
                    ZFGS_HR=HR;
                    ZFGS_HR=ZFGS_HR+2;
                }
                else{
                    if(HR>ZFGS_HR)
                        warring(pos,"阵发型室性心动过速");
                    ZFGS_HR=ZFGS_HR+2;
                }
                return;
            }
        }
        else
        {
            if(testRR(5) == 0 && testRR(6) == 2 && testRR(7) == 0)
            {
                //warring(pos,"室上性二联律");
                warr6(pos);
                return;
            }
            if(testRR(5) == 2 && testRR(6) == 0)
            {
                //成对的判定
                if((RR[2] +RR[3] +RR[4])>1.5*SAMPLE){
                    //warring(pos,"室上性成对");
                    //20100710按照连双要求，去掉成对报警。
                    //warr8(pos);
                    return;
                }
                else if((RR[1] +RR[2] +RR[3])<=1.5*SAMPLE){
                    
                    if(ZFGS_HR==0){
                        warring(pos,"阵发型室上性心动过速");
                        ZFGS_HR=HR;
                        ZFGS_HR=ZFGS_HR+2;
                    }
                    else{
                        if(HR>ZFGS_HR)
                            warring(pos,"阵发型室上性心动过速");
                        ZFGS_HR=ZFGS_HR+2;
                    }
                    return;
                }
            }
            warr2(pos);
        }
        return; //早搏
    }
    
#ifdef PC_VERSION
    if(RR[4] > ARR * 2 )
    {
        iTotalMissRCount++;//总的漏检R波计数器
        return;
    }
#endif
    
    testST(pos);
    
    //传导阻滞的判定
    aqrs = (QRS[0] +QRS[1] + QRS[2] +QRS[3] + QRS[4] +QRS[5] + QRS[6] +QRS[7])/8;
    if(aqrs > 0.12 * SAMPLE)
    {
        if(HR < MIN_SXZZXV)
        {
            //warring(pos,"室性自主心律");
            warr12(pos);
            return;
        }
        if( QRS[5] > 0.12 * SAMPLE && QRS[4] > 0.12 * SAMPLE &&  QRS[3] > 0.12 * SAMPLE &&  QRS[2] > 0.12 * SAMPLE )
        {
            //加速性室性自主心律
            //warr11(pos);
            if(JSSXZZXL_HR==0){
                warr11(pos);
                //warring(pos,"加速性室性自主心律");
                JSSXZZXL_HR=HR;
            }
            else{
                if(HR>JSSXZZXL_HR)
                    //warring(pos,"加速性室性自主心律");
                    warr11(pos);
            }
            return;
        }
        //warring(pos,"室性QRS宽，但不早博");
        return;
    }
}

void rescan(int start1, int end)
{
    //搜索最大最小值，确定阀值
    int bufmin=255;
    int bufmax=0;
    int downval = 0;
    int v0=0, v1=0, v2=0;
    int aQRS;
    int cliff_maxcount,cliff_mincount;
    kal_bool lastmax = false;
    int maxi;
    int maxv= 0;
    int i,s,q,t,st,times;
    char buf[256];
    U8 QS_Height,RS_Height;
    int tt;
    int start;
    int mid_val;
    
    start=start1;
    
    if(rescan_oldstart==0)
        rescan_oldstart=start1;
    
    if (( g_oldmaxcount==rescan_oldmaxcount) && rescan_oldmaxcount){
        //调整回扫描的起点
        start=rescan_oldstart+(end-rescan_oldstart+1)/2;
    }
    
    //移植时注释该语句
#ifdef PC_VERSION
    {
        static int iHeight=0;
        static int iNums=0;
        if(iNums%3==0)
            winAddLineData(g_basex + start,g_basey + downval+iHeight+1,  g_basex + end, g_basey + downval+iHeight+1, RGB(100,45,20));
        else if(iNums%3==1)
            winAddLineData(g_basex + start,g_basey + downval+iHeight+1,  g_basex + end, g_basey + downval+iHeight+1, RGB(50,20,100));
        else if(iNums%3==2)
            winAddLineData(g_basex + start,g_basey + downval+iHeight+1,  g_basex + end, g_basey + downval+iHeight+1, RGB(100,100,200));
        
        iHeight=(iHeight+3)%100;
        iNums++;
    }
#endif
    maxi=start;
    for( i=start; i<end; i++)
    {
        v2 = v1; v1 = v0; v0 = g_buf[i % BUFFSIZE];
        if( v1 >= v0 && v1 >= v2)
        {
            //极大值，放大检查窗口.0.05-->0.06
            if(lastmax && v1 <= g_buf[maxi % BUFFSIZE] && i-maxi < SAMPLE * 0.06)
                continue;
            lastmax = true;
            maxi = i;
            maxv = v1;
            continue;
        }
        //added by cliff 20100714
        if(maxi==0) continue;
        
        //暂时保存下R波的位置
        cliff_maxcount=maxi-2;
        cliff_mincount=i;
        
        if(v1 <= v0 && v1 <= v2 && lastmax)
        {
            if((maxv - v1) < (g_upval - g_downval)*1.3) //回扫高度阀值
                continue;
            
            q = maxi -1;
            t = 255;
            times = 0;
            mid_val=(g_upval - g_downval)/2+g_downval;
            
            while(/*(g_buf[q % BUFFSIZE] <= t) &&*/ (maxi - q < 15 ))
            {
                if(abs(g_buf[q % BUFFSIZE] - t)<=1)
                    times++;
                else
                    times=0;
                if((times > 1)&&(t<mid_val))
                    break;
                t = g_buf[q % BUFFSIZE];
                q --;
            }
            
            if (t > maxv-10)
                continue; //基线漂移的检测
            
            //20100624，二次扫描发现的R波和反向波距离过进，忽略反向波
            if(maxi>g_errcount&&maxi-g_errcount<0.25*SAMPLE)
                g_errcount=0;
            if(maxi<g_errcount&&g_errcount-maxi<0.25*SAMPLE)
                g_errcount=0;
            
            s = maxi+1;
            t = 0;
            while((g_buf[s % BUFFSIZE] >= t || s - maxi < 6) && s-maxi < (maxi-q)*MAX_RS)
            {
                if(g_buf[s%BUFFSIZE] == t)
                    times++;
                else
                    times =0;
                if(times > 2)
                    break;
                t = g_buf[s % BUFFSIZE];
                
                s++;
            }
            
            //标记Q和S波的位置
#ifdef PC_VERSION
            winAddLineData(g_basex + q,g_basey+80 ,  g_basex + q, g_basey + 200, RGB(0,255,0));
            winAddLineData(g_basex + s,g_basey+80 ,  g_basex + s, g_basey + 200, RGB(0,0,255));
#endif
            st =( (g_buf[(s + 3) % BUFFSIZE] + g_buf[(s + 4) % BUFFSIZE] + g_buf[(s + 5) % BUFFSIZE] )
                 - (g_buf[(q- 1) % BUFFSIZE] + g_buf[(q - 2 ) % BUFFSIZE] + g_buf[(q - 3) % BUFFSIZE] ));
            
            QRS[7] =QRS[6]; QRS[6] =QRS[5];QRS[5]=QRS[4];QRS[4]=QRS[3];QRS[3] =QRS[2];QRS[2] =QRS[1];QRS[1] =QRS[0]; QRS[0] = s - q +1;
            ST[7] = ST[6];  ST[6] = ST[5]; ST[5]= ST[4]; ST[4] =ST[3]; ST[3] = ST[2]; ST[2] = ST[1]; ST[1] = ST[0];  ST[0] = st;
            
#ifdef PC_VERSION
            winAddLineData(g_basex + maxi -1,g_basey + 280,  g_basex + maxi - 1, g_basey + 200, RGB(255,255,0));
            
            if(    RR[4] < MIN1_RR * ARR){
                //早搏
                if((s-q+1)> MAX_QRS * SAMPLE){
                    sprintf(buf, "【v%d,%d,%d,%d,%d】", g_basex + maxi -1,ARR,RR[0],s-q+1,st/3);
                    winAddDrawData(g_basex + maxi -1, 280, RGB(255,0,0), buf);
                }
                else{
                    sprintf(buf, "【s%d,%d,%d,%d,%d】", g_basex + maxi -1,ARR,RR[0],s-q+1,st/3);
                    winAddDrawData(g_basex + maxi -1, 280, RGB(0,0,255), buf);
                }
            }
            else{
                sprintf(buf, "【%d,%d,%d,%d,%d】", g_basex + maxi -1,ARR,RR[0],s-q+1,st/3);
                winAddDrawData(g_basex + maxi -1, 280, RGB(0,0,0), buf);
            }
            
#endif
            if(g_errcount)
            {
                
                //二次扫描发现了反向波
                Notify(g_errcount, "二次扫描发现反向波");
                
                RR[7] = RR[6];  RR[6] = RR[5]; RR[5]= RR[4]; RR[4] =RR[3]; RR[3] = RR[2]; RR[2] = RR[1]; RR[1] = RR[0];  RR[0] = g_errcount - g_oldmaxcount;
                DrawHart();
                if (++RR_count >= 8)
                    panduan(g_errcount);
                else if(RR_count < 7 && z_zoom==0)
                {
                    if(z_max < g_bufmax) z_max = g_bufmax;
                    if(z_min > g_bufmin) z_min = g_bufmin;
                }
                else if(RR_count == 7 && z_zoom == 0)
                {
                    if(z_max - z_min < 32)
                    {
                        z_zoom = 3;
                        z_base = (255 - ((z_max - z_min) << 2)) / 2;
                    }
                    else if(z_max -z_min < 64)
                    {
                        z_zoom = 2;
                        z_base = (255 - ((z_max - z_min) << 1)) / 2;
                    }
                    
                    else
                    {
                        z_zoom = 1;
                    }
                }
                
                q = g_errcount-1;
                t = 0;
                times = 0;
                while(g_buf[q % BUFFSIZE] >= t || g_errcount - q < 5)
                {
                    if(g_buf[q % BUFFSIZE] == t)
                        times++;
                    else
                        times=0;
                    if(times > 2)
                        break;
                    
                    t = g_buf[q % BUFFSIZE];
                    q--;
                }
                
                s = g_errcount-1;
                t = 0;
                times = 0;
                while((g_buf[s % BUFFSIZE] >= t || s- g_errcount < 6) && s-g_errcount < (g_errcount - q)*MAX_RS)
                {
                    if(g_buf[s % BUFFSIZE] == t)
                        times++;
                    else
                        times=0;
                    if(times > 2)
                        break;
                    
                    t = g_buf[s % BUFFSIZE];
                    s++;
                }
                
                //移植时注释该语句
#ifdef PC_VERSION
                //二次扫描画出QS线
                winAddLineData(g_basex + q,g_basey+80 ,  g_basex + q, g_basey + 200, RGB(0,255,0));
                winAddLineData(g_basex + s,g_basey+80 ,  g_basex + s, g_basey + 200, RGB(0,0,255));
                
#endif
                st =( (g_buf[(s + 3) % BUFFSIZE] + g_buf[(s + 4) % BUFFSIZE] + g_buf[(s + 5) % BUFFSIZE] )
                     - (g_buf[(q- 1) % BUFFSIZE] + g_buf[(q -2 ) % BUFFSIZE] + g_buf[(q -3 ) % BUFFSIZE] ));
                
                QRS[7] =QRS[6]; QRS[6] =QRS[5];QRS[5]=QRS[4];QRS[4]=QRS[3];QRS[3] =QRS[2];QRS[2] =QRS[1];QRS[1] =QRS[0]; QRS[0] = s - q +1;
                ST[7] = ST[6];  ST[6] = ST[5]; ST[5]= ST[4]; ST[4] =ST[3]; ST[3] = ST[2]; ST[2] = ST[1]; ST[1] = ST[0];  ST[0] = st;//不计算st
                RR[7] = RR[6];  RR[6] = RR[5]; RR[5]= RR[4]; RR[4] =RR[3]; RR[3] = RR[2]; RR[2] = RR[1]; RR[1] = RR[0];  RR[0] = maxi - g_errcount + 1;
                g_errcount = 0;
            }
            else
            {
                //正向波
                RR[7] = RR[6];  RR[6] = RR[5]; RR[5]= RR[4]; RR[4] =RR[3]; RR[3] = RR[2]; RR[2] = RR[1]; RR[1] = RR[0];  RR[0] = maxi - g_oldmaxcount + 1;
            }
            
            g_oldmaxcount = maxi + 1;
            g_oldmincount = i + 1;
            
            DrawHart();
            if(++RR_count >= 8)
                panduan(i);
            else if(RR_count < 7 && z_zoom == 0 )
            {
                if(z_max < g_bufmax) z_max = g_bufmax;
                if(z_min > g_bufmin) z_min = g_bufmin;
            }
            else if(RR_count == 7 && z_zoom == 0)
            {
                if(z_max - z_min < 32)
                {
                    z_zoom = 3;
                    z_base = (255 - ((z_max - z_min) << 2)) / 2;
                }
                else if(z_max -z_min < 64)
                {
                    z_zoom = 2;
                    z_base = (255 - ((z_max - z_min) << 1)) / 2;
                }
                
                else
                {
                    z_zoom = 1;
                }
            }
            rescan_oldstart=start;
            rescan_oldmaxcount=0;
            return;
        }
    }
    rescan_oldmaxcount=g_oldmaxcount;
    rescan_oldstart=start;
    return;
}

void old_run(int data);
const static float VOL = 255 / 5.5;
void new_run(float data1){
    
//    printf("%f ", data1 * VOL);
    int temp = round(data1 * VOL) + 128;
    printf("%d ", temp);
    int data;
    //准备心电数据缓冲区
    BuffECGData(data1,&data);
    if(g_ecg_count<=SAMPLE)
        return;
    old_run(data);
}

void run(int data1)
{
//    printf("%d ", data1);
    int data;
    //准备心电数据缓冲区
    BuffECGData(data1,&data);
    if(g_ecg_count<=SAMPLE)
        return;
    old_run(data);
}

//诊断主流程
void old_run(int data)
{
//    printf("%d ", data);
    int v0, v1, v2;
    int i0, i1, i2;
    int x0,y0,xielv;
    int s,q,t,times,st;
    int mincount,maxcount;
    char buf[256];
    int i;
    int mid_val;
    int g_oldupval,g_olddownval;
    int tt;
#ifdef PC_VERSION
    static int linecolor=0;
#else
    S8 des[64] = {0};
#endif
    
    g_buf[g_count % BUFFSIZE] = data;
    
    g_count++;
    
    //起始数据,第一次进入的时候必须全部装满一次
    if(g_count < BUFFSIZE)
        return;
    
    if(g_count % SAMPLE == 0)
    {
        //更新阀值
        g_bufmin = 255, g_bufmax = 0;
        mincount = 0;
        maxcount =0;
        
        //第一条线
        for( i=0; i< SAMPLE; i++){
            if(g_ecg_buf[i] < g_bufmin) g_bufmin = g_ecg_buf[i], mincount = 0;
            if(g_ecg_buf[i] > g_bufmax) g_bufmax = g_ecg_buf[i], maxcount = 0;
            if(g_ecg_buf[i] == g_bufmin) mincount++;
            if(g_ecg_buf[i] == g_bufmax) maxcount++;
        }
        
        //干扰判定
        if( g_bufmin <= 28 && mincount > 20)
        {
            //如果0过多认为是干扰
            Notify(g_count, "0过多不更新基线");
            resetex1();
            return;
        }
        else if(g_bufmax >= 227 && maxcount > 15)
        {
            //如果255过多认为是干扰
            Notify(g_count, "255过多不更新基线");
            resetex();
            return;
        }
        else
            //基线阀值的调整逻辑
            if(( g_bufmax - g_bufmin < 36)/*||((g_upval-g_downval<8)&&g_upval)*/)//以前是40，后来调整到10
            {
                //如果最大跟最小值相差比较小就不更新阀值
                Notify(g_count,"落差小不更新基线");
                //画出基线区间来
#ifdef PC_VERSION
                linecolor++;
                if(linecolor%2==0){
                    winAddLineData(g_count+g_basex, g_upval, g_basex + g_count + SAMPLE,g_upval, RGB(100,100,100));
                    winAddLineData(g_count+g_basex, g_downval, g_basex + g_count + SAMPLE, g_downval, RGB(200,200,200));
                }
                else{
                    winAddLineData(g_count+g_basex, g_upval, g_basex + g_count + SAMPLE,g_upval, RGB(100,150,150));
                    winAddLineData(g_count+g_basex, g_downval, g_basex + g_count + SAMPLE, g_downval, RGB(0,50,50));
                }
                sprintf(buf,"保持-%d,%d,%d,%d,%d", g_bufmax,g_bufmin,g_upval,g_downval,g_upval-g_downval);
                winAddDrawData(g_basex + g_count -1, 280, RGB(200,200,200), buf);
#endif
            }
            else
            {
                //对基线进行调整的逻辑入口
                if(g_upval)
                {
                    //已经有过一次基线计算出来了。
                    g_oldupval=g_upval;
                    g_olddownval=g_downval;
                    
                    if(g_bufmax<g_upval){
                        //说明往下漂移
                        g_upval = ((g_bufmax-g_bufmin) * 1/3 + g_bufmin - g_downval) * 1/2 + g_downval ;
                        g_downval =g_upval-(g_oldupval-g_downval) ;
                        
#ifdef PC_VERSION
                        //画出基线区间来
                        linecolor++;
                        if(linecolor%2==0){
                            winAddLineData(g_count+g_basex, g_upval, g_basex + g_count + SAMPLE,g_upval, RGB(100,100,100));
                            winAddLineData(g_count+g_basex, g_downval, g_basex + g_count + SAMPLE, g_downval, RGB(200,200,200));
                        }
                        else{
                            winAddLineData(g_count+g_basex, g_upval, g_basex + g_count + SAMPLE,g_upval, RGB(100,150,150));
                            winAddLineData(g_count+g_basex, g_downval, g_basex + g_count + SAMPLE, g_downval, RGB(0,50,50));
                        }
                        sprintf(buf,"狂降-%d,%d,%d,%d,%d", g_bufmax,g_bufmin,g_upval,g_downval,g_upval-g_downval);
                        winAddDrawData(g_basex + g_count -1, 280, RGB(200,200,200), buf);
#endif
                    }
                    else
                        
                    {
                        //调整基线
                        g_upval = ((g_bufmax-g_bufmin) * 2/3 + g_bufmin - g_upval) * 1/2 + g_upval ;
                        g_downval =( (g_bufmax-g_bufmin) * 1/2 + g_bufmin - g_downval) * 1/2 + g_downval ;
#ifdef PC_VERSION
                        //画出基线区间来
                        linecolor++;
                        if(linecolor%2==0){
                            winAddLineData(g_count+g_basex, g_upval, g_basex + g_count + SAMPLE,g_upval, RGB(100,100,100));
                            winAddLineData(g_count+g_basex, g_downval, g_basex + g_count + SAMPLE, g_downval, RGB(200,200,200));
                        }
                        else{
                            winAddLineData(g_count+g_basex, g_upval, g_basex + g_count + SAMPLE,g_upval, RGB(100,150,150));
                            winAddLineData(g_count+g_basex, g_downval, g_basex + g_count + SAMPLE, g_downval, RGB(0,50,50));
                        }
                        //显示阀值
                        if(g_upval>g_oldupval)
                            sprintf(buf,"上%d,%d,%d,%d,%d",g_bufmax,g_bufmin,g_upval,g_downval,g_upval-g_downval);
                        else
                            if(g_upval<g_oldupval)
                                sprintf(buf,"下%d,%d,%d,%d,%d", g_bufmax,g_bufmin,g_upval,g_downval,g_upval-g_downval);
                            else
                                sprintf(buf,"=%d,%d,%d,%d,%d", g_bufmax,g_bufmin,g_upval,g_downval,g_upval-g_downval);
                        
                        winAddDrawData(g_basex + g_count -1, 280, RGB(200,200,200), buf);
#endif
                    }
                }
                else
                {
                    //第一次：初始化
                    g_upval = (g_bufmax-g_bufmin) * 2/3 + g_bufmin;
                    g_downval = (g_bufmax-g_bufmin) * 1/2 + g_bufmin;
#ifdef PC_VERSION
                    //画出阀值区间来
                    linecolor++;
                    if(linecolor%2==0){
                        winAddLineData(g_count+g_basex, g_upval, g_basex + g_count + SAMPLE,g_upval, RGB(200,200,200));
                        winAddLineData(g_count+g_basex, g_downval, g_basex + g_count + SAMPLE, g_downval, RGB(200,200,200));
                    }
                    else{
                        winAddLineData(g_count+g_basex, g_upval, g_basex + g_count + SAMPLE,g_upval, RGB(0,50,50));
                        winAddLineData(g_count+g_basex, g_downval, g_basex + g_count + SAMPLE, g_downval, RGB(0,50,50));
                    }
                    //显示阀值
                    sprintf(buf,"初始-%d,%d,%d,%d,%d",g_bufmax,g_bufmin,g_upval,g_downval,g_upval-g_downval);
                    winAddDrawData(g_basex + g_count -1, 280, RGB(200,200,200), buf);
#endif
                }
            }
    }
    
    if(g_oldmaxcount&&ARR)
    {
        //20100605 停搏报警阀值
        if((g_count - g_oldmaxcount > SAMPLE * MAX_TB_TIME)&&ARR)
        {
#ifdef PC_VERSION
            sprintf(buf, "%d,%d,%d", g_basex + g_count -1,ARR,g_count - g_oldmaxcount);
            winAddDrawData(g_basex + g_count -1, 280, RGB(255,0,0), buf);
#endif
            sprintf(buf,"停搏g_count=%d,g_oldcount=%d,RR=%d,ARR=%d",g_count,g_oldmaxcount,g_count - g_oldmaxcount,ARR);
            warring(g_count, buf);
            g_total_tb_count++;
            resetex();
            return;
        }
    }
    else if(g_count > SAMPLE * 15)
    {
        warring(g_count,"停搏(未检测到R波）");
        g_total_tb_count=g_total_tb_count+1;
#ifndef PC_VERSION
        //连接异常
        mmi_chset_text_to_ucs2((kal_uint8 *)des, 10, (kal_uint8 *)"连接异常");
        DisplayPopup((PU8) des, IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE);
        PlayECGVoice((U8) MESSAGE_TONE);
#else
        warring(g_count,"停搏(未检测到R波）");
#endif
        resetex();
        return;
    }
    
    if(g_upval ==0 || g_downval == 0)
        return;
    
    i0 = (g_count - 1) % BUFFSIZE;v0 = g_buf[i0];
    i1 = (g_count - 2) % BUFFSIZE;v1 = g_buf[i1];
    i2 = (g_count - 3) % BUFFSIZE;v2 = g_buf[i2];
    
    if(v2 <= 28 && v1 <= 28 && v0 <= 28 && g_buf[(g_count - 4)% BUFFSIZE] <= 28 && g_buf[(g_count - 5) % BUFFSIZE] <= 28)
    {
        //连续三个0，干扰
        ganrao(g_count - 3);
        return;
    }
    if(v2 >= 227 && v1 >= 227 && v0 >= 227 && g_buf[(g_count - 4)% BUFFSIZE] >= 227 && g_buf[(g_count - 5) % BUFFSIZE] >= 227)
    {
        //连续三个255，干扰
        ganrao(g_count - 3);
        return;
    }
    
    //干扰过滤条件和阀值
    if(v1 > v0 && v1 > v2 && v1 - v0 > 8 && v1 - v2 > 8)
        ganrao(g_count-2); //这里只计数,不返回
    
    //找出R波点，怀疑的R波在g_maxcount
    if(v1 >= g_upval && v1 >= v0 && v1 >= v2)
    {
        g_lasterr = false;
        if(g_lastmax && v1 < g_buf[g_maxposbuf] && g_count - 2 - g_maxcount < SAMPLE * 0.05)
            return;
        g_lastmax = true;
        g_maxcount = g_count - 2;
        g_maxposbuf = i1;
        return;
    }
    
    if(v1 <= g_downval && v1 <= v0 && v1 <= v2)
    {
        //极小值//反向波形的逻辑进入点
        if(v0 - v1 > 0x30 || v2 - v1 > 0x30)
        {
            //用斜率来判定下干扰，斜率过高就认定为干扰
            ganrao(g_count - 2);
            return;
        }
        
        if(g_lastmax == false)
        {
            //没有找到极大值 反向波形
            if(v1 < g_bufmin - (g_downval - g_bufmin))
            {
                //暂时去掉反向波,20100709
                return;
            }
            
            //正向波
            //修改二次扫描的条件，20100711,1.2ARR改成 1.8ARR
            if(ARR && g_count - g_oldmaxcount > ARR * 1.8)
            {
                //20100711，二次扫描 0.25 ARR改成0.5ARR
                rescan(g_oldmaxcount + 0.5 * ARR, g_count);
                return;
            }
            else if(ARR == 0 && RR[0] && g_count - g_oldmaxcount > RR[0] * 1.8)
            {
                //二次扫描
                rescan(g_oldmaxcount + 0.5 * RR[0], g_count);
                return;
            }
            else if(g_oldmaxcount && RR[0] == 0 && g_count - g_oldmaxcount > SAMPLE )
            {
                //初始检测不到第二个R波回扫
                rescan(g_oldmaxcount + SAMPLE * 15 / 150,g_count);
                return;
            }
            
            return;
        }
        
        //正向R波形,进行R波的相关检查
        g_lastmax = false;
        g_mincount = g_count -2;
        g_minposbuf = i1;
        
        //x0是宽度 y0是高度
        y0 = g_buf[g_maxposbuf] - g_buf[g_minposbuf];
        x0 = g_mincount - g_maxcount;
        xielv = y0 * 1000 / x0;
        if(y0 <0 || x0 <0)
        {
            Notify(g_count, "逻辑发生错误");
            return;
        }
        
        //RR间期判定1
        if(ARR && g_maxcount - g_oldmaxcount < ARR * 0.28)
        {
            sprintf(buf, "%d-RR小", g_basex + g_maxcount -1);
            Notify(g_count, buf);
            ganrao(g_count-2);
            return;
        }
        //RR间期判定2
        if(ARR ==0 && RR[0] && g_maxcount - g_oldmaxcount < RR[0] * 0.28)
        {
            sprintf(buf, "%d-RR小 RR[0] = %d", g_basex + g_maxcount -1,RR[0]);
            Notify(g_count, buf);
            ganrao(g_count-2);
            return;
        }
        //RR间期判定3
        if(g_maxcount - g_oldmaxcount < SAMPLE * 60 / 220)
        {
            sprintf(buf,"%d-RR小, 可能是干扰",g_basex + g_maxcount -1);
            Notify(g_count, buf);
            ganrao(g_count-2);
            return;
        }
        
        if(ARR && g_maxcount - g_oldmaxcount < ARR * 0.7)
        {
            //如果当前位置在0.6ARR,判定条件严格一些
            //也就是当前位置不再节律位置
            if(y0 < (g_upval - g_downval) * 2.5)
            {
                Notify(g_count, "非节律：落差不够");
                return;
            }
        }
        else{
            //在节律位置，判定条件稍微宽松一些
            if(y0 < (g_upval - g_downval)*1 )
            {
                sprintf(buf,"%d节律：落差不够",g_basex + g_maxcount -1);
                Notify(g_count, buf);
                return;
            }
        }
        //R波的斜率条件
        if(xielv < 1000)
        {
            Notify(g_count, "斜率太小，过滤");
            return;
        }
    }
    else if(ARR && g_count - g_oldmaxcount > ARR * 1.8 )
    {
        //二次扫描
        rescan(g_oldmaxcount + 0.5 * ARR, g_count);
        return;
    }
    else if(ARR == 0 && RR[0] && g_count - g_oldmaxcount > RR[0] * 1.8)
    {
        //二次扫描
        rescan(g_oldmaxcount + 0.5 * RR[0], g_count);
        return;
    }
    else if(g_oldmaxcount && RR[0] == 0 && g_count - g_oldmaxcount > SAMPLE )
    {
        //初始检测不到第二个R波回扫
        rescan(g_oldmaxcount + SAMPLE * 15 / 150,g_count);
        return;
    }
    else
    {
        return; //其他的数值直接返回
    }
    
    //判定Q和R的高度落差,20100624
    //找到Q点
    q = g_maxcount-1;
    t = 255;
    times = 0;
    mid_val=(g_upval - g_downval)/2+g_downval;
    while((g_maxcount - q)< 15)
    {
        if(abs(g_buf[q % BUFFSIZE] - t)<=1)
            times++;
        else
            times=0;
        if((times > 2)&&(t<mid_val))
            break;
        
        t = g_buf[q % BUFFSIZE];
        q--;
    }
    
    //找到S点
    s = g_maxcount ;
    tt = 0;
    while( (s - g_maxcount < 15)  &&  (s-g_maxcount < (g_maxcount-q)*MAX_RS) )
    {
        if((abs(g_buf[s%BUFFSIZE] - tt)<=1)/*&&(t<mid_val)*/)
            times++;
        else
            times =0;
        if(times > 1)
            break;
        tt = g_buf[s % BUFFSIZE];
        s++;
    }
    
    if(AQR==0){
        //第一次记录QR到AQR
        AQR=abs(g_buf[g_maxcount%BUFFSIZE]-t);
        ARS=abs(g_buf[g_maxcount%BUFFSIZE]-tt);
    }
    else{
        //AQR不为0，说明已经有平均的AQR了
        if(ARR==0){
            //条件宽一点
            if((abs(g_buf[g_maxcount%BUFFSIZE]-t) < AQR*0.3)&&(abs(g_buf[g_maxcount%BUFFSIZE]-tt) < ARS*0.3 ))
            {    Notify(g_count, "ARR=0,QR并且RS落差不够过滤");
                return;
            }
        }
        //AQR不为0，说明已经有平均的AQR了
        if(ARR&&(g_maxcount - g_oldmaxcount > ARR * 0.6))
        {
            //条件宽一点
            if((g_buf[g_maxcount%BUFFSIZE]-t < AQR*0.33)&&(abs(g_buf[g_maxcount%BUFFSIZE]-tt) < ARS*0.5))
            {
                Notify(g_count, "节律内：QR并且RS落差不够过滤");
                return;
            }
        }
        else{
            //条件宽一点
            if((ARR&&(g_buf[g_maxcount%BUFFSIZE]-t < AQR*0.5))&&(abs(g_buf[g_maxcount%BUFFSIZE]-tt) < ARS*0.5))
            {
                Notify(g_count, "节律外：QR并且RS落差不够过滤");
                return;
            }
        }
        //求平均的AQR
        AQR=(AQR+g_buf[g_maxcount%BUFFSIZE]-t)/2;
        ARS=(ARS+abs(g_buf[g_maxcount%BUFFSIZE]-tt))/2;
    }
    
    if(g_oldmaxcount == 0)
    {
        g_oldmaxcount = g_maxcount;
        g_oldmincount = g_mincount;
        return;
    }
    
    //S波边界
    s = g_maxcount ;
    t = 0;
    while( (s - g_maxcount < 15)  &&  (s-g_maxcount < (g_maxcount-q)*MAX_RS) )
    {
        if((abs(g_buf[s%BUFFSIZE] - t)<=1)/*&&(t<mid_val)*/)
            times++;
        else
            times =0;
        if(times > 1)
            break;
        t = g_buf[s % BUFFSIZE];
        s++;
    }
    
    //added by clff ,20100605
    if(cliff_ARR&cliff_QRS){
        //20100711,0.1*clif_QRS改成0.3
        if((s-q) < 0.3*cliff_QRS)
        {
            Notify(g_count, "QRS波宽度过小,过滤");
            return;
        }
    }
    
#ifdef PC_VERSION
    winAddLineData(g_basex + q,g_basey+80 ,  g_basex +q, g_basey + 200, RGB(0,255,0));
    winAddLineData(g_basex + s,g_basey+80 ,  g_basex +s, g_basey + 200, RGB(0,0,255));
#endif
    
    st =( (g_buf[(s + 3) % BUFFSIZE] + g_buf[(s + 4) % BUFFSIZE] + g_buf[(s + 5) % BUFFSIZE] )
         - (g_buf[(q- 1) % BUFFSIZE] + g_buf[(q -2 ) % BUFFSIZE] + g_buf[(q - 3) % BUFFSIZE] ));
    
    QRS[7] =QRS[6]; QRS[6] =QRS[5];QRS[5]=QRS[4];QRS[4]=QRS[3];QRS[3] =QRS[2];QRS[2] =QRS[1];QRS[1] =QRS[0]; QRS[0] = s - q+1;
    ST[7] = ST[6];  ST[6] = ST[5]; ST[5]= ST[4]; ST[4] =ST[3]; ST[3] = ST[2]; ST[2] = ST[1]; ST[1] = ST[0];  ST[0] = st;
    
#ifdef PC_VERSION
    winAddLineData(g_basex + g_maxcount -1,g_basey + 280,  g_basex + g_maxcount - 1, g_basey + 200, RGB(200,200,0));
    
    if(    RR[4] < MIN1_RR * ARR){
        if((s-q+1)> MAX_QRS * SAMPLE){
            sprintf(buf, "v%d,%d,%d,%d,%d,%d,%d", g_basex + g_maxcount -1,ARR,RR[0],s-q+1,st/3,AQR,ARS);
            winAddDrawData(g_basex + g_maxcount -1, 280, RGB(255,0,0), buf);
        }
        else{
            sprintf(buf, "s%d,%d,%d,%d,%d,%d,%d", g_basex + g_maxcount -1,ARR,RR[0],s-q+1,st/3,AQR,ARS);
            winAddDrawData(g_basex + g_maxcount -1, 280, RGB(0,0,255), buf);
        }
    }
    else{
        sprintf(buf, "%d,%d,%d,%d,%d,%d,%d", g_basex + g_maxcount -1,ARR,RR[0],s-q+1,st/3,AQR,ARS);
        winAddDrawData(g_basex + g_maxcount -1, 280, RGB(0,0,0), buf);
    }
#endif
    
    if(g_errcount)
    {
        //找到一个倒波
        RR[7] = RR[6];  RR[6] = RR[5]; RR[5]= RR[4]; RR[4] =RR[3]; RR[3] = RR[2]; RR[2] = RR[1]; RR[1] = RR[0];  RR[0] = g_errcount - g_oldmaxcount;
        
        rescan_oldmaxcount=0;
        DrawHart();
        if( ++RR_count >= 8)
            panduan(g_errcount);
        else if( RR_count < 7 && z_zoom == 0)
        {
            if(z_max < g_bufmax) z_max = g_bufmax;
            if(z_min > g_bufmin) z_min = g_bufmin;
        }
        else if(RR_count == 7 && z_zoom == 0)
        {
            if(z_max - z_min < 32)
            {
                z_zoom = 3;
                z_base = (255 - ((z_max - z_min) << 2)) / 2;
            }
            else if(z_max -z_min < 64)
            {
                z_zoom = 2;
                z_base = (255 - ((z_max - z_min) << 1)) / 2;
            }
            
            else
            {
                z_zoom = 1;
            }
        }
        //errcount 不找QRS
        q = g_errcount-1;
        t = 0;
        times = 0;
        while(g_buf[q % BUFFSIZE] >= t || g_errcount - q < 5)
        {
            if(g_buf[q % BUFFSIZE] == t)
                times++;
            else
                times=0;
            if(times > 2)
                break;
            t = g_buf[q % BUFFSIZE];
            q--;
        }
        
        s = g_errcount -1;
        t=0;
        times =0;
        while((g_buf[s% BUFFSIZE] >= t || s - g_errcount < 6) && s-g_errcount <(g_errcount - q) * MAX_RS)
        {
            if(g_buf[s%BUFFSIZE] == t)
                times++;
            else
                times =0;
            if(times > 2)
                break;
            t = g_buf[q%BUFFSIZE];
            s++;
        }
        
#ifdef PC_VERSION
        winAddLineData(g_basex + q,g_basey+80 ,  g_basex + q, g_basey + 200, RGB(0,255,0));
        winAddLineData(g_basex + s,g_basey+80 ,  g_basex + s, g_basey + 200, RGB(0,0,255));
#endif
        st =( (g_buf[(s + 3) % BUFFSIZE] + g_buf[(s + 4) % BUFFSIZE] + g_buf[(s + 5) % BUFFSIZE] )
             - (g_buf[(q- 1) % BUFFSIZE] + g_buf[(q - 2 ) % BUFFSIZE] + g_buf[(q - 3) % BUFFSIZE] ));
        QRS[7] =QRS[6]; QRS[6] =QRS[5];QRS[5]=QRS[4];QRS[4]=QRS[3];QRS[3] =QRS[2];QRS[2] =QRS[1];QRS[1] =QRS[0]; QRS[0] = s - q+1;
        ST[7] = ST[6];  ST[6] = ST[5]; ST[5]= ST[4]; ST[4] =ST[3]; ST[3] = ST[2]; ST[2] = ST[1]; ST[1] = ST[0];  ST[0] = 0;//不计算st
        RR[7] = RR[6];  RR[6] = RR[5]; RR[5]= RR[4]; RR[4] =RR[3]; RR[3] = RR[2]; RR[2] = RR[1]; RR[1] = RR[0];  RR[0] = g_maxcount - g_errcount;
        g_errcount = 0;
    }
    else
    {
        RR[7] = RR[6];  RR[6] = RR[5]; RR[5]= RR[4]; RR[4] =RR[3]; RR[3] = RR[2]; RR[2] = RR[1]; RR[1] = RR[0];  RR[0] = g_maxcount - g_oldmaxcount;
    }
    //保存当前的RR波的最大和最小位置
    g_oldmaxcount = g_maxcount;
    g_oldmincount = g_mincount;
    
    //求及时的cliff_ARR,cliff_QRS
    if((cliff_ARR==0)&&(RR[4]))
        cliff_ARR=RR[4];
    else
        cliff_ARR=(RR[4]+cliff_ARR)/2;
    
    if((cliff_QRS==0)&&(QRS[0]!=0))
        cliff_QRS=QRS[4];
    else
        cliff_QRS=(QRS[4]+cliff_QRS)/2;
    
    rescan_oldmaxcount=0;
    DrawHart();
    if(++RR_count < 8)
    {
        if(z_zoom == 0)
        {
            if(RR_count < 7 )
            {
                if(z_max < g_bufmax) z_max = g_bufmax;
                if(z_min > g_bufmin) z_min = g_bufmin;
            }
            else if(RR_count == 7 )
            {
                if(z_max - z_min < 32)
                {
                    z_zoom = 3;
                    z_base = (255 - ((z_max - z_min) << 2)) / 2;
                }
                else if(z_max -z_min < 64)
                {
                    z_zoom = 2;
                    z_base = (255 - ((z_max - z_min) << 1)) / 2;
                }
                
                else
                {
                    z_zoom = 1;
                }
            }
        }
        return;
    }
    panduan(g_count);
}


void AddEcgInfo(char* szMsg1, char* szMsg2, char* szMsg3)
{
    struct _heart_alarm *pTmp = NULL;
    
    if (pHead == NULL)
    {
        pHead = (struct _heart_alarm *)calloc(1, sizeof(struct _heart_alarm));
        memcpy(pHead->alarm_num, szMsg1, 8);
        memcpy(pHead->pos, szMsg2, 16);
        memcpy(pHead->szMsg, szMsg3, 128);
        pHead->next = NULL;
        pCur = pHead;
        //第一次，对头结点初始化
        return;
    }
    pTmp = (struct _heart_alarm *)calloc(1, sizeof(struct _heart_alarm));
    memcpy(pTmp->alarm_num, szMsg1, 8);
    memcpy(pTmp->pos, szMsg2, 16);
    memcpy(pTmp->szMsg, szMsg3, 128);
    pTmp->next = NULL;
    
    pCur->next = pTmp;
    pCur = pTmp;
    return;
}

void clearlink()
{
    //////////////////////////////////////////////////////////  xiegege
    if (pHead != NULL)
    {
        struct _heart_alarm *p = NULL;
        while (pHead)
        {
            p = pHead;
            pHead = pHead->next;
            free(p);
            p = NULL;
        }
        pHead = NULL;
    }
    if (!pCur)
    {
        free(pCur);
        pCur = NULL;
    }
    //////////////////////////////////////////////////////////
    return;
}
