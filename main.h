//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include "SupportFuns.h"
#include "Udp_opt.h"
#include "UDPStructs.h"
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
//---------------------------------------------------------------------------
class Tmainform : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TStatusBar *StaB;
	TIdUDPServer *UDPSvr;
	TActionList *actBase;
	TAction *actlSetBinding;
	TImageList *imglAct;
	TMenuItem *N1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall actlSetBindingExecute(TObject *Sender);
private:	// User declarations

	//UI操作类型
	enum TUIType{
					uiStab      //状态栏
				};

	TMyIp m_ActiveIP{};                   //活动IP
	TDeviceInfo m_DeviceInfo;             //设备虚拟RAM
	TLRParams m_LRParams;                 //主服务软件传递参数

public:		// User declarations
	__fastcall Tmainform(TComponent* Owner);
	void __fastcall SetBindings(void);
	void __fastcall DataToUI(TUIType UIType);
	void __fastcall INDY_SafeSendTo(const String &r_IP,u16 Port,const std::vector<TBytes> &r_VC_TByte);
    void __fastcall INDY_SafeSendTo(const String &r_IP,u16 Port,const TBytes &rAByte);
};
//---------------------------------------------------------------------------
extern PACKAGE Tmainform *mainform;
//---------------------------------------------------------------------------
#endif
