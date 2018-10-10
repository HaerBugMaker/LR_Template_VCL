/*********************************************************************
* �汾��¼
* 18.1.31 ���� StrToIPChar���� ֧��XPϵͳ
* 18.3.20 ���� WhetherValid_IP �����ж�IP�Ƿ�Ϸ�
* 18.3.23 ����CAN�������
* 18.4.23 UDP͸�������������PackPara_nonstruct
* 18.4.28 IP��ȡ����֧�ֻ�ȡMAC��ַ(SendArp)
* 18.5.8  IP�Ϸ����жϺ���BUG�޸�
* 18.5.15 �޸�IP��ȡ������127.0.0.1 BUG
* 18.7.9  ����FLASH ��ȡ���ݴ������
* 18.7.16 1 ����u32�ͱ�׼ʱ��ṹ����ת���ĺ���
		  2 ����flash�·��������
		  3 �޸���Rd Wr�������ڶ������µ�BUG
* 18.7.17 ����IAP�����������
* 18.7.23 1 StrToMac6�������ӿյ�ַ�ж�
		  2 StrToMac6�׳��쳣�����޸�ΪException
* 18.8.20 �޸�WrPara_Flash��һ��BUG
* 18.8.22 1 ����PackPara_StrToHexArr���������ַ�����HEX�����ת��
*********************************************************************/
//---------------------------------------------------------------------------

#ifndef Udp_optH
#define Udp_optH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "UDPStructs.h"
#include <vector>
#include <memory>

//---------------------------------------------------------------------------


//------------------------------------------------------------------------------

//�豸��ʶ
char const EquIdStr[] = {"LRNRF602ConfigData"};

//�豸�����
enum TPkgIncrease{increase_1,increase_1024};

//FLASHЭ������豸���� ����оƬ   CPU�ڲ�FLASH
enum TFLASHType{        at45db16,  cpu};

//u32�ͱ�׼ʱ��ṹ����ת����ʱ���õ��ı�
const uint8_t CalibrationPpm[128]={0,1,2,3,4,5,6,7,8,9,10,10,11,12,13,14,15,16,17,\
						 18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,32,33,34,\
						 35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,51,\
						 52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,\
						 70,71,72,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,\
						 87,88,89,90,91,92,93,93,94,95,96,97,98,99,100,101,102,\
						 103,104,105,106,107,108,109,110,111,112,113,113,114,\
						 115,116,117,118,119,120,121};

//			      1  2  3  4  5  6  7  8  9 10 11 12
const u8 MonthDayTab[] =     {31,28,31,30,31,30,31,31,30,31,30,31};
const u8 LeapMonthDayTab[] = {31,29,31,30,31,30,31,31,30,31,30,31};
const u16 YearDayTab[] = {366,365,365,365};


//------------------------------------------------------------------------------
//����ԭ��
String IntToIpstr(u32 addr);
u16 GetMyIP(std::vector<TMyIp> &VC_MyIp);
String IPCharToStr(const u8 IP[4],bool Endian = false);
void __fastcall StrToIPChar(const char* ipstr,u8 ip[4]);
const String MACCharToStr(const u8 MAC[6]);
int StrToMac6(const char* strtmp,u8 * dp1);
void StrToMac6(const String &macstr,char mac[6]);
bool WhetherValid_IP(const String &IP);
//-------------��׼UDPЭ��----------------
//Ҫ����
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,TPkgIncrease PkgIncrease);
//Ҫ����_�ⲿʶ����
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,const char* EquIdStr,TPkgIncrease PkgIncrease);
//��������
void __fastcall WrPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes);
//��������_�ⲿʶ����
int __fastcall WrPara(int startpos, int paranum,const void* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes);
//�豸����ָ��
void __fastcall CtlPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes);
//�豸����ָ��_�ⲿʶ����
int __fastcall CtlPara(int startpos, int paranum,const char* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes);
//�㲥��
void __fastcall SearchEqu(TBytes &AByte);
//Уʱָ��
void __fastcall CalibTime(TBytes &AByte);
//�Ǳ�׼UDP�ṹ�����
void __fastcall PackPara_nonstruct(const void *data,size_t size,TBytes &AByte);
//��������ʽ_��һ������(aa bb cc dd��ʽ���ַ���ת��ΪHEX����)
void __fastcall PackPara_StrToHexArr(const String &HexStr,TBytes &AByte);
//----------CAN����Э��--------------------
//���CAN����
void __fastcall PackCan(int address,int dp_len,int cmd,const void* dp,TBytes &AByte);

//---------FLASH����Э��---------------------
//Ҫ����
void __fastcall RdPara_FLASH(u16 Address,u32 StaAdd,u32 DatLen,std::vector<TBytes> &VC_TBytes);
//д����
void __fastcall WrPara_FLASH(u16 Address,u32 StaAdd,u32 DatLen,const void* srcdata,std::vector<TBytes> &VC_TBytes);
//IAP�������
void __fastcall Pack_TIAPSigCmd(int naddress,const char* srccmd,TBytes &nabyte);

//------------ʱ�䴦����--------------------
//��׼ʱ��ṹת��Ϊu32��ʽ
u32 RtcRealToBin(TSysTime * SDT);
//u32��ʽ��ʱ��ת��Ϊ��׼ʱ���ʽ
TSysTime RtcBinToReal(u32 RtcCnt);
//------------------------------------------------------------------------------

#endif
