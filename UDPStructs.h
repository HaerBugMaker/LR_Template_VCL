/***********************************************************************
* UDP���ýṹ��������Ŀ��Ҫ�Ľṹ
* 2018.4.28 TMyIp�ṹ֧��MAC��ַ
* 2018.5.9 ������ƽ̨�������Ͷ���
* 2018.6.22 1 �޸���TMyIp�ṹ
			2 TDeviceInfo�ṹIP��Port����ȱʡֵ
			3 ����GitHub���а汾����
* 2018.7.9 ����FLASH��ؽṹ
***********************************************************************/
#ifndef COMMBASE_H
#define COMMBASE_H
#include "System.hpp"
#include <vector>


//�����������Ͷ���
typedef char s8;
typedef unsigned char u8;
typedef unsigned char const uc8;
typedef unsigned char uchar;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;

//��ƽ̨�������Ͷ���
/*
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32
*/

//ͨ��UDP�ṹ��
typedef struct {
  char Cmd[10];
  char IdentifyingCode[20];
  u8 StartByte;
  u8 CmdType;
  u16 ParaStaAdd;
  u16 ParaNum;
  u8 Para[1024];
  u16 ChkSum;
}TUDPSysPara;

//Rd���ݽṹ��
typedef struct {
  char Cmd[10];
  char IdentifyingCode[20];
  u8 StartByte;
  u8 CmdType;
  u16 ParaStaAdd;
  u16 ParaNum;
  u16 ChkSum;
}TUDPSetParaConfirm;

//�㲥��Ϣ�ṹ����ɲ���1
typedef struct {
	char MyName[64];
	char EquType[16];
	char EquSer[16];
	char FWVer[16];
	char FWDate[16];
	u16 Address;
	u16 Baud;
	u8  mac[6];
	u8  ip[4];
	u8  maskip[4];
	u8  gatewayip[4];
	u16 UDPport;
	u16 WWWport;
	u16 TCPport;
	u16 UPGRADEport;
	char FirstUse[8];
	u32 rev;
}TAddressPara;

//�㲥��Ϣ�ṹ�������
typedef struct {
  u16  Address;
  char Cmd[16];
  TAddressPara AddressPara;
  u16 ChkSum;
}TUDPAddressPara;


//ϵͳʱ��ṹ
typedef struct
{
  u16 msCnt;
  u8 sec,min,hour,date,month,year;
  u8 weekday;
}TSysTime;

//Уʱ�ṹ
typedef struct {//port 15288
	char Cmd[20];  //"CalibDateTime"
	TSysTime DT;
}TUDPCalibTime;

//��׼CANת�����������ݽṹ
typedef struct { //can tran buf
  u16  Address;
  u16  len;
  char Cmd[12];   //"CanRDat"  "CanTDat"
  u16 Exid[2];   //Exid[0]:id5-12   Exid[1]:id13-20
  u8  CanDat[1200];
}TCanTRDat;

typedef struct {   //iap start,run app
  u16  Address;
  char Cmd[16];
  u16  chksum;
}TIAPSigCmd;

typedef struct { //erase  ,prog ack
  u16  Address;
  char Cmd[16];
  u32  StaAdd;
  u32  DatLen;
  u16  chksum;
}TFlashAddCmd;

typedef struct { //prog
  u16  Address;
  char Cmd[16];
  u32  StaAdd;
  u32  DatLen;
  u8   databuf[528*2];
  u16  chksum;
}TFlashProg;

//������������Ӳ���
typedef struct{
	String IpAddr;
	int Port{0};
	String Mask;
	String Bcast;
    u8 MAC[6];
}TMyIp;

//ͨ��ϵͳ�����ṹ
typedef struct {
	char MyName[64];
 	char EquType[16];
 	char EquSer[16];
 	char FWVer[16];
	char FWDate[16];
	u16 Address;
	u16 Baud;
	u8  mac[6];
	u8  ip[4];
	u8  maskip[4];
	u8  gatewayip[4];
	u16 UDPport;
	u16 WWWport;
	u16 TCPport;
	u16 UPGRADEport;
	char FirstUse[8];
	char IdentifyingCode[20];   //ʶ����
}TSysPara;

//ͨ���豸��Ϣ
struct TDeviceInfo{
	String IP{L"192.168.0.11"};      //�豸IP��Ϣ
	u16    Port{2305};               //�豸�˿�
};







#endif