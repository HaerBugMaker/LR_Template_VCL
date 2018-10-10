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
			{throw Exception(L"无可用网络连接");}
		this->m_ActiveIP = VC_MyIpPool.front();
		//如果通过配置服务软件打开本软件
		if(ParamCount() != 0){
			this->m_LRParams.AppIP = ParamStr(1);                    //本机IP
			this->m_LRParams.DeviceIP = ParamStr(2);                 //设备IP
			this->m_LRParams.ServerAppHwndName = ParamStr(3);        //服务窗体名
			const auto ipit = std::find_if(VC_MyIpPool.begin(),VC_MyIpPool.end(),
						   [&](const TMyIp &myip){if(myip.IpAddr == this->m_LRParams.AppIP)
													{return true;}
												  else
													{return false;}});
			this->m_ActiveIP = ipit == VC_MyIpPool.end()? VC_MyIpPool.front():*ipit;
			this->m_DeviceInfo.IP = this->m_LRParams.DeviceIP;
		}
		//独立打开
		else{
			this->m_ActiveIP = VC_MyIpPool.front();
		}
		SetBindings();
		DataToUI(TUIType::uiStab);
	}
	catch(Exception &err){
		ShowMessage(L"初始化异常" + err.Message);
	}
}
//---------------------------------------------------------------------------
//UDP设定
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
		//状态栏
		case TUIType::uiStab:{
			if(UDPSvr->Bindings->Count){
				StaB->Panels->Items[0]->Text = L"本机IP:" + UDPSvr->Bindings->Items[0]->IP;
				StaB->Panels->Items[1]->Text = L"本机端口:" + IntToStr(UDPSvr->Bindings->Items[0]->Port);
			}
			else{
				StaB->Panels->Items[0]->Text = L"本机IP:--";
                StaB->Panels->Items[1]->Text = L"本机端口:--";
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
//基础动作 | 通讯配置
void __fastcall Tmainform::actlSetBindingExecute(TObject *Sender)
{
	auto ntag = dynamic_cast<TAction*>(Sender)->Tag;

	switch (ntag) {
		//IP设定
		case 0:{
			Tconfigform* pconfigform = new Tconfigform(this,this->m_ActiveIP,this->m_DeviceInfo);
			int modalresult = pconfigform->ShowModal();
			if(modalresult == mrOk){
				SetBindings();
				DataToUI(TUIType::uiStab);
				ShowMessage(L"配置成功");
			}
			delete pconfigform;
			break;
		}
	}
}
//---------------------------------------------------------------------------
/******************
* INDY UDP发送函数(由于UDP发送阶段有会产生各种异常,所以这里统一为一个函数方便异常处理避免代码膨胀)
* r_IP        :  发送目标IP
* Port        :  发送目标端口
* r_VC_TByte  :  发送数据包
******************/
void __fastcall Tmainform::INDY_SafeSendTo(const String &r_IP,u16 Port,const std::vector<TBytes> &r_VC_TByte)
{
	try{
		if(!WhetherValid_IP(r_IP)) return;                      //过滤不合法IP
		if(UDPSvr->Bindings->Count >= 1){
			for(const auto &abyte:r_VC_TByte)
				{UDPSvr->Bindings->Items[0]->SendTo(r_IP,Port,abyte);}
		}
	}
	catch(EIdSocketError &socketerr){
        /* TODO : 处理异常 */
		switch (socketerr.LastError) {
			//cannot assign requested address
			case 10049:{
				ShowMessage(L"网络通信出错,可能原因是您修改了本机的网络设置。请确保网络设置正常后重启本软件");
				break;
			}
			default:{
				ShowMessage(L"网络通信出错,出错信息" + socketerr.Message);
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
/******************
* INDY UDP发送函数(由于UDP发送阶段有会产生各种异常,所以这里统一为一个函数方便异常处理避免代码膨胀)
* r_IP        :  发送目标IP
* Port        :  发送目标端口
* rAByte      :  发送数据包
******************/
void __fastcall Tmainform::INDY_SafeSendTo(const String &r_IP,u16 Port,const TBytes &rAByte)
{
	try{
		if(!WhetherValid_IP(r_IP)) return;                      //过滤不合法IP
		if(UDPSvr->Bindings->Count >= 1){
			UDPSvr->Bindings->Items[0]->SendTo(r_IP,Port,rAByte);
		}
	}
	catch(EIdSocketError &socketerr){
		/* TODO : 处理异常 */
		switch (socketerr.LastError) {
			//cannot assign requested address
			case 10049:{
				ShowMessage(L"网络通信出错,可能原因是您修改了本机的网络设置。请确保网络设置正常后重启本软件");
				break;
			}
			default:{
				ShowMessage(L"网络通信出错,出错信息" + socketerr.Message);
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
