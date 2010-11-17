//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include <ComCtrls.hpp>
#include <Syncobjs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "cspin.h"
//---------------------------------------------------------------------------
class TMain : public TForm
{
__published:	// IDE-managed Components
  TGroupBox *GroupBox1;
  TLabel *Label1;
  TLabel *Label2;
  TButton *btnOpen;
  TLabel *Label3;
  TEdit *txtMyAddress;
  TButton *btnClose;
  TComboBox *lstInterfaces;
  TCSpinEdit *cseUniqueID;
  TPageControl *pcTabs;
  TTabSheet *TabSheet1;
  TMemo *memLog;
  TTabSheet *TabSheet2;
  TLabel *Label4;
  TLabel *lblNodeName;
  TLabel *Label5;
  TLabel *lblNodeDesc;
  TLabel *Label6;
  TLabel *Label7;
  TLabel *lblNodeHWMa;
  TLabel *Label9;
  TLabel *lblNodeHWMi;
  TLabel *Label8;
  TLabel *lblNodeFWMa;
  TLabel *Label11;
  TLabel *lblNodeFWMi;
  TLabel *Label10;
  TLabel *lblNodeFPMa;
  TLabel *Label13;
  TLabel *lblNodeFPMi;
  TLabel *lblNodePRMi;
  TLabel *Label14;
  TLabel *lblNodePRMa;
  TLabel *Label16;
  TLabel *Label12;
  TLabel *lblNodeEngine;
  TLabel *Label15;
  TLabel *lblNodeHWPar;
  TLabel *lblNodeSR;
  TStatusBar *StatusBar;
  TButton *btnLogClear;
  TCheckBox *chkLogCb;
  TCheckBox *chkLogObj;
  TCheckBox *chkLogAdr;
  TCheckBox *chkLogApp;
  TLabel *Label17;
  TLabel *lblObjCount;
  TTabSheet *TabSheet3;
  TCheckBox *cbListFetch;
  TButton *mbnPingAll;
  TListView *lvNodeList;
  TListView *lvObjects;
  TLabel *lblNodeAddr;
  TLabel *Label19;
  TLabel *Label18;
  TLabel *Label20;
  TLabel *lblObjSensor;
  TLabel *lblObjActuator;
  TLabel *Label21;
  TComboBox *cbObjFreq;
  TLabel *lblNodeID;
  TLabel *Label23;
  TEdit *txtObjStrAct;
  TButton *btnObjStrAct;
  TLabel *lblObjStatus;
  TButton *btnObjRetry;
  TCheckBox *cbObjRefresh;
  TTimer *RefreshTimer;
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall btnOpenClick(TObject *Sender);
  void __fastcall btnCloseClick(TObject *Sender);
  void __fastcall mbnPingAllClick(TObject *Sender);
  void __fastcall btnLogClearClick(TObject *Sender);
  void __fastcall cbListFetchClick(TObject *Sender);
  void __fastcall lvNodeListDblClick(TObject *Sender);
  void __fastcall lvObjectsSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
  void __fastcall cbObjFreqChange(TObject *Sender);
  void __fastcall lvObjectsDeletion(TObject *Sender, TListItem *Item);
  void __fastcall btnObjStrActClick(TObject *Sender);
  void __fastcall btnObjRetryClick(TObject *Sender);
  void __fastcall cseUniqueIDChange(TObject *Sender);
  void __fastcall RefreshTimerTimer(TObject *Sender);
  void __fastcall lvNodeListCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
  void __fastcall lvNodeListColumnClick(TObject *Sender,
          TListColumn *Column);
private:	// User declarations
public:		// User declarations
  struct mbn_handler *mbn;
  struct mbn_if_ethernet *iflist;
  char validaddr;
  unsigned char nodelistsort;
  unsigned long fromaddr, objnr, objcount, objget;
  __fastcall TMain(TComponent* Owner);
  __fastcall ~TMain();
};
//---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
//---------------------------------------------------------------------------
#endif
