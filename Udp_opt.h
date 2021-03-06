/*********************************************************************
* 版本记录
* 18.1.31 新增 StrToIPChar函数 支持XP系统
* 18.3.20 新增 WhetherValid_IP 用于判断IP是否合法
* 18.3.23 新增CAN打包报文
* 18.4.23 UDP透传打包函数更新PackPara_nonstruct
* 18.4.28 IP获取函数支持获取MAC地址(SendArp)
* 18.5.8  IP合法性判断函数BUG修复
* 18.5.15 修改IP获取方法的127.0.0.1 BUG
* 18.7.9  增加FLASH 读取数据打包函数
* 18.7.16 1 增加u32和标准时间结构互相转换的函数
		  2 增加flash下发打包函数
		  3 修复了Rd Wr的若干在多包情况下的BUG
* 18.7.17 增加IAP命令组包函数
* 18.7.23 1 StrToMac6函数增加空地址判断
		  2 StrToMac6抛出异常类型修改为Exception
* 18.8.20 修复WrPara_Flash的一个BUG
* 18.8.22 1 增加PackPara_StrToHexArr函数用于字符串和HEX数组的转换
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

//设备标识
char const EquIdStr[] = {"LRNRF602ConfigData"};

//设备包间隔
enum TPkgIncrease{increase_1,increase_1024};

//FLASH协议操作设备类型 外置芯片   CPU内部FLASH
enum TFLASHType{        at45db16,  cpu};

//u32和标准时间结构互相转换的时候用到的表
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
//函数原型
String IntToIpstr(u32 addr);
u16 GetMyIP(std::vector<TMyIp> &VC_MyIp);
String IPCharToStr(const u8 IP[4],bool Endian = false);
void __fastcall StrToIPChar(const char* ipstr,u8 ip[4]);
const String MACCharToStr(const u8 MAC[6]);
int StrToMac6(const char* strtmp,u8 * dp1);
void StrToMac6(const String &macstr,char mac[6]);
bool WhetherValid_IP(const String &IP);
//-------------标准UDP协议----------------
//要数据
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,TPkgIncrease PkgIncrease);
//要数据_外部识别字
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,const char* EquIdStr,TPkgIncrease PkgIncrease);
//下载数据
void __fastcall WrPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes);
//下载数据_外部识别字
int __fastcall WrPara(int startpos, int paranum,const void* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes);
//设备控制指令
void __fastcall CtlPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes);
//设备控制指令_外部识别字
int __fastcall CtlPara(int startpos, int paranum,const char* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes);
//广播包
void __fastcall SearchEqu(TBytes &AByte);
//校时指令
void __fastcall CalibTime(TBytes &AByte);
//非标准UDP结构体组包
void __fastcall PackPara_nonstruct(const void *data,size_t size,TBytes &AByte);
//特殊打包格式_将一串符合(aa bb cc dd格式的字符串转换为HEX数据)
void __fastcall PackPara_StrToHexArr(const String &HexStr,TBytes &AByte);
//----------CAN报文协议--------------------
//打包CAN报文
void __fastcall PackCan(int address,int dp_len,int cmd,const void* dp,TBytes &AByte);

//---------FLASH报文协议---------------------
//要数据
void __fastcall RdPara_FLASH(u16 Address,u32 StaAdd,u32 DatLen,std::vector<TBytes> &VC_TBytes);
//写数据
void __fastcall WrPara_FLASH(u16 Address,u32 StaAdd,u32 DatLen,const void* srcdata,std::vector<TBytes> &VC_TBytes);
//IAP命令组包
void __fastcall Pack_TIAPSigCmd(int naddress,const char* srccmd,TBytes &nabyte);

//------------时间处理函数--------------------
//标准时间结构转换为u32格式
u32 RtcRealToBin(TSysTime * SDT);
//u32格式的时间转换为标准时间格式
TSysTime RtcBinToReal(u32 RtcCnt);
//------------------------------------------------------------------------------

#endif
