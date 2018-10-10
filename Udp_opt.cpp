//---------------------------------------------------------------------------

#pragma hdrstop

#include "Udp_opt.h"
#include <vector>
#include <iphlpapi.h>
#pragma link "iphlpapi.lib"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
//��32λ��IPת��Ϊ���ʮ���Ƶ�IP�ַ���
String IntToIpstr(u32 addr)
{
  String ctmp = "";

  for (int ii=0;ii<4;ii++) {
	ctmp += IntToStr(int(addr & 0xff));
	if (ii != 3) ctmp += ".";
	addr >>= 8;
  }
  return ctmp;
}
//---------------------------------------------------------------------------
//��4���ֽڵ�IP��ַת��Ϊ���ʮ���Ƶ�IP�ַ���
/***************
* IP[4] ԴIP����
* Endian �ֽ��� false С�� true ���
***************/
String IPCharToStr(const u8 IP[4],bool Endian)
{
	String IPtmp;
	if(Endian == false) IPtmp.sprintf(L"%d.%d.%d.%d",IP[0],IP[1],IP[2],IP[3]);
	else                IPtmp.sprintf(L"%d.%d.%d.%d",IP[3],IP[2],IP[1],IP[0]);
	return IPtmp;
}
//---------------------------------------------------------------------------
//��4���ֽڵ�IP��ַת��Ϊ���ʮ���Ƶ�IP�ַ���
/***************
* IP[4] ԴIP����
***************/
const String MACCharToStr(const u8 MAC[6])
{
	String MACtmp;
	for(int i=0;i<6;i++){

		MACtmp += IntToHex(MAC[i],2);
		if(i!=5)
		MACtmp += L"-";
	}
	return MACtmp;
}
//---------------------------------------------------------------------------
//�����ʮ���Ƶ�IP�ַ���(const char*)ת��Ϊip[4]    �˹���ͬinet_pton ���ǿ���֧��XP    �ֽ����� 192.168.0.1 C0 A8 00 01
/*******************
* ipstr ���ʮ����IP�ַ���
* ip[4]    ���ص�ip[4]
*******************/
void __fastcall StrToIPChar(const char* ipstr,u8 ip[4])
{
	unsigned long longaddr;
	longaddr = ::inet_addr(ipstr);
	if(longaddr == INADDR_NONE) throw String(L"IP��ַ�����Ϲ淶");
	memcpy(ip,&longaddr,4);
}
//---------------------------------------------------------------------------
//���ԡ�-���ָ���MAC��ַ�ַ���ת��Ϊ6���ֽڵ�MAC��ַ
/***************
* strtmp ԴMAC��ַ�ַ���
* dp1    Ŀ��������ָ��
***************/
int StrToMac6(const char* strtmp,u8 * dp1)
{
	u8 ci;
	for (int ii=0;ii<6;ii++) {
		ci = 0;
		if ((strtmp[ii*2]>='0')&&(strtmp[ii*2]<='9')) ci = strtmp[ii*2]-'0';
		else if ((strtmp[ii*2]>='A')&&(strtmp[ii*2]<='F')) ci = strtmp[ii*2]-'A' + 10;
			 else if ((strtmp[ii*2]>='a')&&(strtmp[ii*2]<='f')) ci = strtmp[ii*2]-'a' + 10;
				  else return(0);
		ci *= 16;
		if ((strtmp[ii*2+1]>='0')&&(strtmp[ii*2+1]<='9')) ci += strtmp[ii*2+1]-'0';
		else if ((strtmp[ii*2+1]>='A')&&(strtmp[ii*2+1]<='F')) ci += strtmp[ii*2+1]-'A' + 10;
			 else if ((strtmp[ii*2+1]>='a')&&(strtmp[ii*2+1]<='f')) ci += strtmp[ii*2+1]-'a' + 10;
				  else return(0);
		*dp1 ++ = ci;
	}
	return (1);

}
//---------------------------------------------------------------------------
//���ԡ�-���ָ���MAC��ַ�ַ���ת��Ϊ6���ֽڵ�MAC��ַ
/***************
* macstr ԴMAC��ַ�ַ���
* mac6   Ŀ��������ָ��
***************/
void StrToMac6(const String &macstr,char mac[6])
{
	if(macstr.IsEmpty())
		{throw Exception(L"MAC��ַ����Ϊ��");}
	std::unique_ptr<TStringList>sl(new TStringList);
	sl->Delimiter = L'-';
	sl->DelimitedText = macstr;
	if((sl->Count!=6)||(macstr == L"")) throw Exception(L"�����Ϲ淶��MAC��ַ");
	String strtmp;
	for(int i=0;i<sl->Count;i++){
		strtmp = L"0x" + sl->Strings[i];
		mac[i] = StrToInt(strtmp);
	}
}
//---------------------------------------------------------------------------
//�ж��ǲ��ǺϷ��ĵ��ʮ����
/*************
* IP IP�ַ���
* ����ֵ true �Ϸ�  false ���Ϸ�
*************/
bool WhetherValid_IP(const String &IP)
{
	try{
		std::unique_ptr<TStringList>psl(new TStringList);
		psl->Delimiter = L'.';
		psl->DelimitedText = IP;
		if((psl->Count!=4)||(IP == L"0.0.0.0")) return false;
		int IPCell;
		for(int i=0;i<4;i++){
			IPCell = psl->Strings[i].ToInt();
			if((IPCell > 255)||(IPCell < 0)) return false;
		}
	}
	catch(...){
		return false;
	}
    return true;	
}
//---------------------------------------------------------------------------
//��ȡ������Ϣ
/***************
*��VC_MyIp IP��
***************/
u16 GetMyIP(std::vector<TMyIp> &VC_MyIp)
{
	TMyIp MyIp;
	VC_MyIp.clear();
	ULONG Maclen{6};
	//��ȡIP��һ���ṹ��
	PMIB_IPADDRTABLE pIPTable = NULL;
	//��С
	DWORD dwSize = 0;
	//��ȡһ��IP��ַ��д��ȥ
	::GetIpAddrTable(pIPTable, &dwSize, TRUE);
	//��������ռ�
	pIPTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR, dwSize);
	//��仰���ǻ�ȡIP��ַȻ������pIPTable����ȥ
	::GetIpAddrTable(pIPTable, &dwSize, TRUE);
	//dwNumEntries�����һ������ IP������
	if (pIPTable->dwNumEntries > 1) {
		//���ﻹҪ����������Ϣ����VECTOR�Ĳ���
		for (u8 i=0; i < pIPTable->dwNumEntries; i++) {
			if(IntToIpstr(pIPTable->table[i].dwAddr) == L"127.0.0.1") continue;
			MyIp.IpAddr = IntToIpstr(pIPTable->table[i].dwAddr);                   //IP
			MyIp.Mask = IntToIpstr(pIPTable->table[i].dwMask);                     //����
			MyIp.Bcast =  IntToIpstr(((pIPTable->table[i].dwAddr & pIPTable->table[i].dwMask) | ~pIPTable->table[i].dwMask));    //�㲥��ַ
			::SendARP(::inet_addr(AnsiString(MyIp.IpAddr).c_str()),0,(PULONG)MyIp.MAC,&Maclen);    //��ȡMAC��ַ�ǲ��ǻῨ
			VC_MyIp.push_back(MyIp);
			memset(&MyIp,0,size_t(sizeof(TMyIp)));
		}
	}//end of more then 1 ip
	::GlobalFree(pIPTable);
	return VC_MyIp.size();
}
//---------------------------------------------------------------------------
//Ҫ����   �������ڶ����ʱ�� parastaadd��+1����+1024
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* VC_TBytes   ���ݽӿ�
* PkgIncrease  ���ݰ����
*����ֵ ���ݰ�����
********/
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,TPkgIncrease PkgIncrease)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();

	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024;
		if(lastpkgnum == 0) lastpkgnum = 1024;
	}
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);

		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x3;
		switch (PkgIncrease) {
			//����+1
			case TPkgIncrease::increase_1:{
				UDPSysPara->ParaStaAdd = startpos + jj;
				break;
			}
			//����+1024
			case TPkgIncrease::increase_1024:{
				UDPSysPara->ParaStaAdd = startpos + jj*1024;
				break;
			}
			default:break;
		}
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;

		AByte.set_length(sizeof(TUDPSetParaConfirm) + 2);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//������_���� equid���ⲿ����
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* VC_TBytes   ���ݽӿ�
* EquIdStr   �豸ʶ����
* PkgIncrease  ���ݰ����
*����ֵ ���ݰ�����
********/
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,const char* EquIdStr,TPkgIncrease PkgIncrease)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();

	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);

		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x3;
		switch (PkgIncrease) {
			//����+1
			case TPkgIncrease::increase_1:{
				UDPSysPara->ParaStaAdd = startpos + jj;
				break;
			}
			//����+1024
			case TPkgIncrease::increase_1024:{
				UDPSysPara->ParaStaAdd = startpos + jj*1024;
				break;
			}
			default:break;
		}
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;

		AByte.set_length(sizeof(TUDPSetParaConfirm) + 2);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//д����
/********
* startpos  ��ʼλ��
* paranum   ��Ҫ����������
* dp        Դ����ָ��
* VC_TBytes ���ݽӿ�
********/
void __fastcall WrPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();


	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x10;
		UDPSysPara->ParaStaAdd = startpos + jj*1024;
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,reinterpret_cast<const char*>(dp)+jj*1024,UDPSysPara->ParaNum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
}
//---------------------------------------------------------------------------
//д��������equid���ⲿ����
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* dp       Դ����ָ��
* EquIdStr �豸�ⲿ����
* VC_TBytes ���ݽӿ�
********/
int __fastcall WrPara(int startpos, int paranum,const void* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();


	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x10;
		UDPSysPara->ParaStaAdd = startpos + jj*1024;
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,reinterpret_cast<const char*>(dp)+jj*1024,UDPSysPara->ParaNum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//���͹㲥��������ʶ���֣�
/************
* &AByte    ���ݽӿ�
************/
void __fastcall SearchEqu(TBytes &AByte)
{
	u8 *dp;
	int i, SendLen;
	//һ��Ҫ��ʼ�� ��Ȼ����ǳ���
	u8 sbuf[1000] = {0};
	TUDPAddressPara * pUDPAddressPara = (TUDPAddressPara *)&sbuf;
	WinExec("cmd /c arp -d",SW_HIDE);
	strcpy(pUDPAddressPara->Cmd,"GetNetPara");

	dp = (u8*)sbuf;
	pUDPAddressPara->ChkSum = 0;
	//����У����˲�Ҫ���������λУ��λ
	for (int i = 0; i<sizeof(TUDPAddressPara)-2; i++) {
		pUDPAddressPara->ChkSum += *dp++;
	}

	AByte.set_length(sizeof(TUDPAddressPara));
	u8 *p = sbuf;
	for (int i=AByte.Low; i<=AByte.High; i++)
		AByte[i] = *p++;
}
//---------------------------------------------------------------------------
//�·�����ָ��
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* dp       Դ����ָ��
* VC_TBytes   ���ݽӿ�
********/
void __fastcall CtlPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();


	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	VC_TBytes.resize(pkgnum);
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x06;
		UDPSysPara->ParaStaAdd = startpos + jj*1024;
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,reinterpret_cast<const char*>(dp)+jj*1024,UDPSysPara->ParaNum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes[jj] = AByte;
	}
}
//---------------------------------------------------------------------------
//�·�����ָ�� �豸��ʶ��Ϊ�ⲿ����
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* dp       Դ����ָ��
* VC_TBytes   ���ݽӿ�
********/
int __fastcall CtlPara(int startpos, int paranum,const char* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();


	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	VC_TBytes.resize(pkgnum);
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x06;
		UDPSysPara->ParaStaAdd = startpos + jj*1024;
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,reinterpret_cast<const char*>(dp)+jj*1024,UDPSysPara->ParaNum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes[jj] = AByte;
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//Уʱ port 15288 �㲥��ʽУʱ(����ʶ����)
/************
* AByte ���ݽӿ�
************/
void __fastcall CalibTime(TBytes &AByte)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	AByte.set_length(sizeof(TUDPCalibTime));
	TUDPCalibTime* pUDPCalibTime = reinterpret_cast<TUDPCalibTime*>(&AByte[AByte.Low]);

	strcpy(pUDPCalibTime->Cmd,"CalibDateTime");
	pUDPCalibTime->DT.year = sys.wYear-2000;
	pUDPCalibTime->DT.month = sys.wMonth;
	pUDPCalibTime->DT.date = sys.wDay;
	pUDPCalibTime->DT.hour = sys.wHour;
	pUDPCalibTime->DT.min = sys.wMinute;
	pUDPCalibTime->DT.sec = sys.wSecond;
	pUDPCalibTime->DT.msCnt = sys.wMilliseconds;
	pUDPCalibTime->DT.weekday = sys.wDayOfWeek;

}
//---------------------------------------------------------------------------
//�ǹ�˾��׼UDP���(UDP͸��)
/****************
* data  Դ����
* size  ���ݳ���(�ֽ�)
* AByte ���ݽӿ�
****************/
void __fastcall PackPara_nonstruct(const void *data,size_t size,TBytes &AByte)
{
	AByte.set_length(0);
	if(data == nullptr) return;
	const char *pdat = reinterpret_cast<const char*>(data);
	AByte.set_length(size);
	for(int i=AByte.Low;i<=AByte.High;i++){
		AByte[i] = *pdat++;
	}
}
//---------------------------------------------------------------------------
/****************
* ��������ʽ_��һ������(aa bb cc dd��ʽ���ַ���ת��ΪHEX����)
* HexStr   :  Դ����
* AByte    :  ���ݽӿ�
****************/
void __fastcall PackPara_StrToHexArr(const String &HexStr,TBytes &AByte)
{
	AByte.set_length(0);
	std::unique_ptr<TStringList>sl(new TStringList);
	sl->Delimiter = L' ';
	sl->DelimitedText = HexStr;
	int ii = sl->Count;
	AByte.set_length(sl->Count);
	for(int i=0;i<sl->Count;i++){
		AByte[i] = StrToInt(L"0x" + sl->Strings[i]);
	}
}
//---------------------------------------------------------------------------
//���CAN���ģ����ְ���
/**************
* address   װ�õ�ַ
* len       ���������ĳ���
* cmd       ָ��
* dp        ��������������
* AByte     ���ݽӿ�
**************/
void __fastcall PackCan(int address,int dp_len,int cmd,const void* dp,TBytes &AByte)
{
	u8 NetSBuf[1500] = {0x00};
	TCanTRDat* pCanTRDat = reinterpret_cast<TCanTRDat*>(NetSBuf);
	int packlen = dp_len + offsetof(TCanTRDat, CanDat);                 //�ܱ��ĳ���

	pCanTRDat->Address = address;         //װ�õ�ַ
	pCanTRDat->len = dp_len;              //���������ĳ���
	strcpy(pCanTRDat->Cmd,"CanTDat");
	pCanTRDat->Exid[0] = 0;
	pCanTRDat->Exid[1] = cmd;
	memcpy(pCanTRDat->CanDat,dp,dp_len);

	AByte.set_length(packlen);
	for (int i=AByte.Low; i<=AByte.High; i++){
		AByte[i] = NetSBuf[i];
	}
}
//---------------------------------------------------------------------------
/********************
* FLASHЭ��Ҫ����(528Ϊ��λ)
* Address   :   ����ͨѶʱ�õģ����豸��ַ������ʱ������
* StaAdd    :   Ҫ�������̵� FLASH �ľ��Ե�ַ  ����45DB161ʱ����528Ϊ��λ������CPU�ڲ�FLASHʱ����1024Ϊ��λ
* DatLen    :   ����Ҫ����������  ����45db16��ʱ�򲻿��Գ��� 528
* VC_TBytes :   ���ݽӿ�
********************/
void __fastcall RdPara_FLASH(u16 Address,u32 StaAdd,u32 DatLen,std::vector<TBytes> &VC_TBytes)
{
	VC_TBytes.clear();
	u8 NetSBuf[200]{0x00};

	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(DatLen == 0){
		pkgnum = 1;
		lastpkgnum = DatLen;
	}
	else{
		pkgnum = DatLen / 528 + ((DatLen % 528) ? 1 : 0);
		lastpkgnum = DatLen % 528;
		if(lastpkgnum == 0) lastpkgnum = 528;
	}

	for (jj=0; jj<pkgnum; jj++)
	{
		TFlashAddCmd* pFlashAddCmd = reinterpret_cast<TFlashAddCmd*>(NetSBuf);
		strcpy(pFlashAddCmd->Cmd,"FlashReadPage");   //����
		pFlashAddCmd->StaAdd = StaAdd + jj*528;      //��ʼ��ַ


		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) pFlashAddCmd->DatLen = lastpkgnum;
		else pFlashAddCmd->DatLen = 528;

		//����У���
		u16 nchksum{0};
		for(int i=0;i<sizeof(TFlashAddCmd) - 2;i++){
			nchksum += NetSBuf[i];
		}
		pFlashAddCmd->chksum = nchksum;

		//��������
		TBytes nabyte;
		nabyte.set_length(sizeof(TFlashAddCmd));
		for(int i=0;i<sizeof(TFlashAddCmd);i++){
			nabyte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(nabyte);
	}
}
//---------------------------------------------------------------------------
/********************
* FLASHЭ��д����(528Ϊ��λ)
* Address   :   ����ͨѶʱ�õģ����豸��ַ������ʱ������
* StaAdd    :   Ҫ�������̵� FLASH �ľ��Ե�ַ  ����45DB161ʱ����528Ϊ��λ������CPU�ڲ�FLASHʱ����1024Ϊ��λ
* DatLen    :   ����Ҫ����������  ����45db16��ʱ�򲻿��Գ��� 528
* srcdata   :   ��Ҫд�������
* VC_TBytes :   ���ݽӿ�
********************/
void __fastcall WrPara_FLASH(u16 Address,u32 StaAdd,u32 DatLen,const void* srcdata,std::vector<TBytes> &VC_TBytes)
{
	VC_TBytes.clear();
	u8 NetSBuf[1500]{0x00};

	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(DatLen == 0){
		pkgnum = 1;
		lastpkgnum = DatLen;
	}
	else{
		pkgnum = DatLen / 528 + ((DatLen % 528) ? 1 : 0);
		lastpkgnum = DatLen % 528;
		if(lastpkgnum == 0) lastpkgnum = 528;
	}

	for (jj=0; jj<pkgnum; jj++)
	{
		TFlashProg* pFlashAddProg = reinterpret_cast<TFlashProg*>(NetSBuf);
		strcpy(pFlashAddProg->Cmd,"FlashWritePage");  //����
		pFlashAddProg->StaAdd = StaAdd + jj*528;      //��ʼ��ַ


		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) pFlashAddProg->DatLen = lastpkgnum;
		else pFlashAddProg->DatLen = 528;
		memcpy(pFlashAddProg->databuf,reinterpret_cast<const char*>(srcdata)+jj*528,pFlashAddProg->DatLen);


		//����У���
		u16 nchksum{0};
		for(int i=0;i<sizeof(TFlashAddCmd) + pFlashAddProg->DatLen - 2;i++){
			nchksum += NetSBuf[i];
		}
		pFlashAddProg->chksum = nchksum;

		//��������
		TBytes nabyte;
		nabyte.set_length(sizeof(TFlashAddCmd) + pFlashAddProg->DatLen);
		for(int i=0;i<nabyte.Length;i++){
			nabyte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(nabyte);
	}
}
//---------------------------------------------------------------------------
/*****************
* IAP�������
* naddress   :   ��ַ
* srccmd     :   Դ����
* nabyte     :   ���ݽӿ�
*****************/
void __fastcall Pack_TIAPSigCmd(int naddress,const char* srccmd,TBytes &nabyte)
{
    nabyte.set_length(0);
	if(srccmd == nullptr) return;
	char SBuf[100]{0x00};
	TIAPSigCmd* pIAPSigCmd = reinterpret_cast<TIAPSigCmd*>(SBuf);
	pIAPSigCmd->Address = naddress;       //��ַ
	memcpy(pIAPSigCmd->Cmd,srccmd,16);    //ָ��
	for(int i=0;i<sizeof(TIAPSigCmd)-2;i++){
		pIAPSigCmd->chksum += SBuf[i];
	}
	nabyte.set_length(sizeof(TIAPSigCmd));
	for(int i=0;i<sizeof(TIAPSigCmd);i++){
		nabyte[i] = SBuf[i];
	}
}
//---------------------------------------------------------------------------
/******************************
* ��׼ʱ��ṹת��Ϊu32��ʽ
* SDT      :     ��׼ʱ��ṹ
* ����ֵ   :     u32��ʽ��ʱ��
******************************/
u32 RtcRealToBin(TSysTime * SDT)
{
if(SDT == nullptr) return 0x00000000;
u32 RtcCnt;
 u32 days;
 u8 i,j;
//get days
	days = (SDT->year >> 2) * 1461;
	i = SDT->year % 4;
	for (j=0;j<i;j++) days += YearDayTab[j];

	if (i)
		for (j=0;j<SDT->month - 1;j++) days += MonthDayTab[j];
	else
		for (j=0;j<SDT->month - 1;j++) days += LeapMonthDayTab[j];
	days += SDT->date - 1;
//get
    RtcCnt = SDT->hour*3600 + SDT->min*60 + SDT->sec;
	RtcCnt += (days * 86400);   //24*60*60
	return(RtcCnt);
}

//---------------------------------------------------------------------------
/******************************
* u32��ʽ��ʱ��ת��Ϊ��׼ʱ���ʽ
* RtcCnt   :     u32��ʽ��ʱ��
* ����ֵ   :     ת�����
******************************/
TSysTime RtcBinToReal(u32 RtcCnt)
{
 u32 days,time;
 u8 i,j;
 TSysTime nsdt;
//get time
	time = RtcCnt % 86400;
	nsdt.hour = time / 3600;
	time %= 3600;
	nsdt.min = time / 60;
	nsdt.sec = time % 60;
//get date
	days = RtcCnt / 86400;
	time = days % 1461;
	for (i=0;i<4;i++) {
		if (time<YearDayTab[i]) break;
		else time -= YearDayTab[i];
	}
	nsdt.year = (days / 1461) * 4 + i;

	for (j=0;j<12;j++) {
		if (i) {
			if (time < MonthDayTab[j]) break;
			else time -= MonthDayTab[j];
		}
		else {
			if (time < LeapMonthDayTab[j]) break;
			else time -= LeapMonthDayTab[j];
		}
	}
	nsdt.month = j+1;
	nsdt.date = time+1;
	nsdt.weekday = (days + 6) % 7;
	return nsdt;
}
//---------------------------------------------------------------------------






