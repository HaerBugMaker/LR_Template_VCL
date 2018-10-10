//---------------------------------------------------------------------------
/**********************
* ����һЩ��������
* �����д����쳣�׳����� ʹ�������鴦���쳣
 Git  �汾��¼
* 2018.1.30 ���ӱ༭����ҹ���
* 2018.5.3 �޸��쳣�׳�����Ϊ��׼Exception����
* 2018.9.26 �޸ı�׼���ݲ���ΪTLRParams��ֹ���ܳ��ֵ�����
*********************/
#ifndef SupportFunsH
#define SupportFunsH
#include <System.Classes.hpp>
#include <Vcl.Grids.hpp>

//---------------------------------------------------------------------------
//�Զ�����Ϣ���Ʋ���
#define MY_MSG WM_USER + 0x310
#define MYMSG_ASK_AVAILABLEDEVICE WM_USER + 0x310
//---------------------------------------------------------------------------
/*****************
* �豸�����ĸ�ʽ
* 2018.1.15�汾
*****************/
struct TClipDevice{
	char ClipID[50];        //������ʶ���� ��Ŀ�����������EXE�ļ��� Ŀ���������Ҳ��ô��֤
	char DeviceIP[20];      //�豸IP
	char DeviceMAC[20];     //�豸MAC
};
/*****************
* ���δ��豸�����ṹ
* 2018.1.15�汾
*****************/
struct TLRParams{
	String AppIP;       //���IP
	String DeviceIP;    //�豸IP
	String ServerAppHwndName;  //��������

};


//---------------------------------------------------------------------------
//StringGrid�ظ����ж�
void __fastcall GridRepeatJudge(TStrings* tstr);
//���ȡ��ѡ��
/*************
* StringGridtmp Ŀ����
*************/
void __fastcall CancelGridSel(TStringGrid* StringGridtmp);
//��ձ��
/*************
* StringGridtmp Ŀ����
*************/
void __fastcall ClearGrid(TStringGrid* StringGridtmp);
//������_��
/******************
* StringGridtmp Ŀ�굥Ԫ��
* col           Ŀ����
* val           Ŀ��ֵ
******************/
int __fastcall LocateGrid_Row(TStringGrid* StringGridtmp,int col,const String &val);
//TEdit���ַ���趨
/*******************
* Edit �༭��
* style �༭�������ʽ
*******************/
void __fastcall TEditStyleSet(TCustomEdit *Edit,DWORD style);
//stringgrid��������ʽ
/*****************
* StringGrid ���ؼ�
* ACol Ŀ����
* ARow Ŀ����
* str  Ŀ���ı�
* Style ��ʽ  0����� 1 ���� 2�Ҷ��� Ĭ��0��ȱʡ
*****************/
void __fastcall TStringGridStyleInput(TStringGrid* StringGrid,int ACol,int ARow,const String &str,int Style = 0);
#endif
