//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "ConfigCom.h"
#include <algorithm>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tmainform *mainform;
//---------------------------------------------------------------------------
__fastcall Tmainform::Tmainform(TComponent* Owner)
	: TForm(Owner)
{
	try{
		std::vector<TMyIp> VC_MyIpPool;
		if(GetMyIP(VC_MyIpPool) == 0)
			{throw Exception(L"�޿�����������");}
		this->m_ActiveIP = VC_MyIpPool.front();
		//���ͨ�����÷��������򿪱�����
		if(ParamCount() != 0){
			this->m_LRParams.AppIP = ParamStr(1);                    //����IP
			this->m_LRParams.DeviceIP = ParamStr(2);                 //�豸IP
			this->m_LRParams.ServerAppHwndName = ParamStr(3);        //��������
			const auto ipit = std::find_if(VC_MyIpPool.begin(),VC_MyIpPool.end(),
						   [&](const TMyIp &myip){if(myip.IpAddr == this->m_LRParams.AppIP)
													{return true;}
												  else
													{return false;}});
			this->m_ActiveIP = ipit == VC_MyIpPool.end()? VC_MyIpPool.front():*ipit;
			this->m_DeviceInfo.IP = this->m_LRParams.DeviceIP;
		}
		//������
		else{
			this->m_ActiveIP = VC_MyIpPool.front();
		}
		SetBindings();
		DataToUI(TUIType::uiStab);
	}
	catch(Exception &err){
		ShowMessage(L"��ʼ���쳣" + err.Message);
	}
}
//---------------------------------------------------------------------------
//UDP�趨
void __fastcall Tmainform::SetBindings(void)
{
	UDPSvr->Active = false;
	UDPSvr->Bindings->Clear();
	UDPSvr->Bindings->Add();
	UDPSvr->Bindings->Items[0]->IP = this->m_ActiveIP.IpAddr ;
	//UDPSvr->Bindings->Items[0]->Port = 2305;
	UDPSvr->Active = true;
}
//---------------------------------------------------------------------------
void __fastcall Tmainform::DataToUI(TUIType UIType)
{
	switch (UIType) {
		//״̬��
		case TUIType::uiStab:{
			if(UDPSvr->Bindings->Count){
				StaB->Panels->Items[0]->Text = L"����IP:" + UDPSvr->Bindings->Items[0]->IP;
				StaB->Panels->Items[1]->Text = L"�����˿�:" + IntToStr(UDPSvr->Bindings->Items[0]->Port);
			}
			else{
				StaB->Panels->Items[0]->Text = L"����IP:--";
                StaB->Panels->Items[1]->Text = L"�����˿�:--";
			}
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall Tmainform::FormShow(TObject *Sender)
{
    DataToUI(TUIType::uiStab);
}
//---------------------------------------------------------------------------
//�������� | ͨѶ����
void __fastcall Tmainform::actlSetBindingExecute(TObject *Sender)
{
	auto ntag = dynamic_cast<TAction*>(Sender)->Tag;

	switch (ntag) {
		//IP�趨
		case 0:{
			Tconfigform* pconfigform = new Tconfigform(this,this->m_ActiveIP,this->m_DeviceInfo);
			int modalresult = pconfigform->ShowModal();
			if(modalresult == mrOk){
				SetBindings();
				DataToUI(TUIType::uiStab);
				ShowMessage(L"���óɹ�");
			}
			delete pconfigform;
			break;
		}
	}
}
//---------------------------------------------------------------------------
/******************
* INDY UDP���ͺ���(����UDP���ͽ׶��л���������쳣,��������ͳһΪһ�����������쳣���������������)
* r_IP        :  ����Ŀ��IP
* Port        :  ����Ŀ��˿�
* r_VC_TByte  :  �������ݰ�
******************/
void __fastcall Tmainform::INDY_SafeSendTo(const String &r_IP,u16 Port,const std::vector<TBytes> &r_VC_TByte)
{
	try{
		if(!WhetherValid_IP(r_IP)) return;                      //���˲��Ϸ�IP
		if(UDPSvr->Bindings->Count >= 1){
			for(const auto &abyte:r_VC_TByte)
				{UDPSvr->Bindings->Items[0]->SendTo(r_IP,Port,abyte);}
		}
	}
	catch(EIdSocketError &socketerr){
        /* TODO : �����쳣 */
		switch (socketerr.LastError) {
			//cannot assign requested address
			case 10049:{
				ShowMessage(L"����ͨ�ų���,����ԭ�������޸��˱������������á���ȷ��������������������������");
				break;
			}
			default:{
				ShowMessage(L"����ͨ�ų���,������Ϣ" + socketerr.Message);
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
/******************
* INDY UDP���ͺ���(����UDP���ͽ׶��л���������쳣,��������ͳһΪһ�����������쳣���������������)
* r_IP        :  ����Ŀ��IP
* Port        :  ����Ŀ��˿�
* rAByte      :  �������ݰ�
******************/
void __fastcall Tmainform::INDY_SafeSendTo(const String &r_IP,u16 Port,const TBytes &rAByte)
{
	try{
		if(!WhetherValid_IP(r_IP)) return;                      //���˲��Ϸ�IP
		if(UDPSvr->Bindings->Count >= 1){
			UDPSvr->Bindings->Items[0]->SendTo(r_IP,Port,rAByte);
		}
	}
	catch(EIdSocketError &socketerr){
		/* TODO : �����쳣 */
		switch (socketerr.LastError) {
			//cannot assign requested address
			case 10049:{
				ShowMessage(L"����ͨ�ų���,����ԭ�������޸��˱������������á���ȷ��������������������������");
				break;
			}
			default:{
				ShowMessage(L"����ͨ�ų���,������Ϣ" + socketerr.Message);
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------