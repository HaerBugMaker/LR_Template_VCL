//---------------------------------------------------------------------------

#pragma hdrstop

#include "SupportFuns.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
//StringGrid�ظ����ж�
/*******************
* tstr ����������
* �׳��쳣�ַ���
******************/
void __fastcall GridRepeatJudge(TStrings* tstr)
{
	String errstr;
	int errrow;
	for(int i=1;i<tstr->Count;i++){
		if((errrow =tstr->IndexOf(tstr->Strings[i])) != i){
			errstr = L"��" + IntToStr(errrow) + L"��" + IntToStr(i) + L"�г����ظ�";
			throw Exception(errstr);
		}
	}
}
//---------------------------------------------------------------------------
//���ȡ��ѡ��
/*************
* StringGridtmp Ŀ����
*************/
void __fastcall CancelGridSel(TStringGrid* StringGridtmp)
{
	TGridRect myRect;
	myRect.Left = StringGridtmp->ColCount+5;
	myRect.Top = StringGridtmp->RowCount+5;
	myRect.Right = StringGridtmp->ColCount+5;
	myRect.Bottom = StringGridtmp->RowCount+5;
	StringGridtmp->Selection = myRect;
}
//---------------------------------------------------------------------------
//��ձ��
/*************
* StringGridtmp Ŀ����
*************/
void __fastcall ClearGrid(TStringGrid* StringGridtmp)
{
	for(int row=0;row<StringGridtmp->RowCount;row++){
		for(int col=0;col<StringGridtmp->ColCount;col++){
			StringGridtmp->Cells[col][row] = L"";
		}
	}
}
//---------------------------------------------------------------------------
//������_��
/******************
* StringGridtmp Ŀ�굥Ԫ��
* col           Ŀ����
* val           Ŀ��ֵ
******************/
int __fastcall LocateGrid_Row(TStringGrid* StringGridtmp,int col,const String &val)
{
	TStrings *tstr = StringGridtmp->Cols[col];
  /*	for(int row=0;row<StringGridtmp->RowCount;row++){
		if(StringGridtmp->Cells[col][row] == val) return row;
	}        */
	return -1;
}
//---------------------------------------------------------------------------
//TCustomEditϵ�пؼ�������ʽ�趨
/*******************
* Edit �༭��
* style �༭�������ʽ
*******************/
void __fastcall TEditStyleSet(TCustomEdit *Edit,DWORD style)
{
	DWORD dwStyle = ::GetWindowLong(Edit->Handle,GWL_STYLE);
	// ES_LEFT: �����, ES_CENTER: ����, ES_RIGHT: �Ҷ���
	dwStyle = dwStyle | style;
	::SetWindowLong(Edit->Handle, GWL_STYLE, dwStyle);
	Edit->Invalidate();
}
//---------------------------------------------------------------------------
//stringgrid��������ʽ
/*****************
* StringGrid ���ؼ�
* ACol Ŀ����
* ARow Ŀ����
* str  Ŀ���ı�
* Style ��ʽ  0����� 1 ���� 2�Ҷ��� Ĭ��0��ȱʡ
*****************/
void __fastcall TStringGridStyleInput(TStringGrid* StringGrid,int ACol,int ARow,const String &str,int Style)
{
	TRect rect = StringGrid->CellRect(ACol,ARow);
	int width = StringGrid->Canvas->TextWidth(str);
	int height = StringGrid->Canvas->TextHeight(str);
	switch (Style) {
		//�����
		case 0:{
			StringGrid->Canvas->TextRect(rect,rect.Left,(rect.Top+rect.Bottom-height)/2,str);
			break;
		}
		//����
		case 1:{
			StringGrid->Canvas->TextRect(rect,(rect.Left+rect.Right-width)/2,(rect.Top+rect.Bottom-height)/2,str);
			break;
		}
		//�Ҷ���
		case 2:{
			StringGrid->Canvas->TextRect(rect,rect.Right-width,(rect.Top+rect.Bottom-height)/2,str);
			break;
		}
	}

}
//---------------------------------------------------------------------------


