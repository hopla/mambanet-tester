//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <values.h>
#include <Registry.hpp>
#include "mbn.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "CSPIN"
#pragma resource "*.dfm"

#define LF_APP 1
#define LF_CB  2
#define LF_OBJ 3
#define LF_ADR 4

#define NUMTYPE(type, dat) (\
  (type) == MBN_DATATYPE_FLOAT ? (dat).Float :\
  (type) == MBN_DATATYPE_SINT  ? (float)(dat).SInt :\
  (type) == MBN_DATATYPE_UINT || (type) == MBN_DATATYPE_STATE ? (float)(dat).UInt : MINFLOAT)
#define MMTYPE(t) (t == MBN_DATATYPE_OCTETS || t == MBN_DATATYPE_BITS ? MBN_DATATYPE_UINT : t)

TMain *Main;
TCriticalSection *lck;

struct mbn_node_info thisnode = {
  0, 0,
  "MambaNet Node Tester", "D&R Tester",
  1, 24, 0,
};

//---------------------------------------------------------------------------


void addlog(int sec, char *fmt, ...) {
    static char log[500], tmp[500];
    va_list va;
    va_start(va, fmt);

    //lck->Enter();
    sprintf(tmp, "[%s] %s", sec == LF_APP ? "app" : sec == LF_CB  ? "cb " :
        sec == LF_OBJ ? "obj" : "adr", fmt);
    vsprintf(log, tmp, va);
    if( sec == LF_APP && Main->chkLogApp->Checked
     || sec == LF_CB  && Main->chkLogCb->Checked
     || sec == LF_OBJ && Main->chkLogObj->Checked
     || sec == LF_ADR && Main->chkLogAdr->Checked)
        Main->memLog->Lines->Add(log);
    Main->StatusBar->Panels->Items[0]->Text = log;
    //lck->Leave();
    va_end(va);
}

unsigned long hex2int(const char *hex, int len) {
  int i;
  unsigned long r = 0;
  for(i=0; i<len; i++) {
    r <<= 4;
    if(hex[i] >= '0' && hex[i] <= '9')
      r += hex[i]-'0';
    else if(hex[i] >= 'a' && hex[i] <= 'f')
      r += hex[i]-'a'+10;
    else if(hex[i] >= 'A' && hex[i] <= 'F')
      r += hex[i]-'A'+10;
  }
  return r;
}

char data2strdat[256];
char *data2str(unsigned char type, union mbn_data dat) {
    char *d = data2strdat;
    switch(type) {
    case MBN_DATATYPE_NODATA: sprintf(d, "[no data]"); break;
    case MBN_DATATYPE_UINT:   sprintf(d, "%lu", dat.UInt); break;
    case MBN_DATATYPE_SINT:   sprintf(d, "%ld", dat.SInt); break;
    case MBN_DATATYPE_STATE:  sprintf(d, "0x%08lX", dat.State); break;
    case MBN_DATATYPE_OCTETS: sprintf(d, "\"%s\"", dat.Octets); break;
    case MBN_DATATYPE_FLOAT:  sprintf(d, "%f", dat.Float); break;
    case MBN_DATATYPE_BITS:
        sprintf(d, "0x%02X%02X%02X%02X%02X%02X%02X%02X",
            dat.Bits[0], dat.Bits[1], dat.Bits[2], dat.Bits[3],
            dat.Bits[4], dat.Bits[5], dat.Bits[6], dat.Bits[7]);
        break;
    case MBN_DATATYPE_OBJINFO: sprintf(d, "[object info]"); break;
    case MBN_DATATYPE_ERROR:  sprintf(d, "ERROR:\"%s\"", dat.Error); break;
    default: sprintf(d, "[unknown datatype]");
    }
    return d;
}

char *type2str(unsigned char type) {
    switch(type) {
    case MBN_DATATYPE_NODATA:  return "-"; break;
    case MBN_DATATYPE_UINT:    return "UInt"; break;
    case MBN_DATATYPE_SINT:    return "SInt"; break;
    case MBN_DATATYPE_STATE:   return "State"; break;
    case MBN_DATATYPE_OCTETS:  return "Octets"; break;
    case MBN_DATATYPE_FLOAT:   return "Float"; break;
    case MBN_DATATYPE_BITS:    return "Bits"; break;
    case MBN_DATATYPE_OBJINFO: return "Info"; break;
    case MBN_DATATYPE_ERROR:   return "Error"; break;
    }
    return "Unknown";
}

void mError(struct mbn_handler *mbn, int code, char *msg) {  
    addlog(LF_CB, "Error(%d): \"%s\")", code, msg);
}

void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid) {
    char tmp[9];
    sprintf(tmp, "%08lX", addr);
    lck->Enter();
    Main->txtMyAddress->Text = tmp;
    Main->validaddr = valid;
    addlog(LF_CB, "Address status changed: %08lX (%d)", addr, valid);
    lck->Leave();
}

void mAddressTableChange(struct mbn_handler *mbn, struct mbn_address_node *o, struct mbn_address_node *n) {
    char nm[50];
    struct mbn_address_node *cur;
    TListItem *it;

    lck->Enter();
    cur = o == NULL ? n : o;
    sprintf(nm, "%08lX (%04X:%04X:%04X)", cur->MambaNetAddr,
        cur->ManufacturerID, cur->ProductID, cur->UniqueIDPerProduct);
    if(!o && n) {
        addlog(LF_CB, "New node: %s", nm);

        Main->lvNodeList->Items->BeginUpdate();
        it = Main->lvNodeList->Items->Add();
        sprintf(nm, "%08lX", cur->MambaNetAddr);
        it->Data = (void *)cur->MambaNetAddr;
        it->Caption = nm;
        sprintf(nm, "%04X:%04X:%04X", cur->ManufacturerID, cur->ProductID, cur->UniqueIDPerProduct);
        it->SubItems->Add(nm);
        sprintf(nm, "%08lX", cur->EngineAddr);
        it->SubItems->Add(nm);
        sprintf(nm, "%02X", cur->Services);   
        it->SubItems->Add(nm);
        it->SubItems->Add("-");      
        it->SubItems->Add("-");
        Main->lvNodeList->Items->EndUpdate();
        if(Main->cbListFetch->Checked) {
            mbnGetActuatorData(mbn, cur->MambaNetAddr, MBN_NODEOBJ_NAME, 1);
            mbnGetSensorData(mbn, cur->MambaNetAddr, MBN_NODEOBJ_HWPARENT, 1);
        }
    } else if(o && !n) {
        addlog(LF_CB, "Node timeout: %s", nm);
        it = Main->lvNodeList->FindData(0, (void *)cur->MambaNetAddr, true, false);
        if(it != NULL)
            it->Delete();
    }
    lck->Leave();
}

int mActuatorDataResponse(struct mbn_handler *mbn, struct mbn_message *msg, unsigned short object, unsigned char type, union mbn_data dat) {
    int r = 0;  
    TListItem *it;
    bool t;

    addlog(LF_CB, "Actuator Data Response from %08lX object %d (type %d), data %s",
        msg->AddressFrom, object, type, data2str(type, dat));

    if(object == MBN_NODEOBJ_NAME) {  
        lck->Enter();
        it = Main->lvNodeList->FindData(0, (void *)msg->AddressFrom, true, false);
        if(it != NULL)
            it->SubItems->Strings[4] = (char *)dat.Octets;
        lck->Leave();
    }

    if(msg->AddressFrom != Main->fromaddr)
        return 1;

    lck->Enter();
    switch(object) {
    case MBN_NODEOBJ_NAME:
        Main->lblNodeName->Caption = (char *)dat.Octets;
        break;
    default:
        if(object == Main->objnr) {
            Main->lblObjActuator->Caption = data2str(type, dat);
            Main->txtObjStrAct->Text = data2str(type, dat);
        } else
            r = 1;
    }
    lck->Leave();
    return r;
}


int mSensorDataResponse(struct mbn_handler *mbn, struct mbn_message *msg, unsigned short object, unsigned char type, union mbn_data dat) {
    char tmp[100];
    int i, r = 0;
    TListItem *it;

    addlog(LF_CB, "Sensor Data Response from %08lX object %d (type %d), data %s",
        msg->AddressFrom, object, type, data2str(type, dat));

    if(object == MBN_NODEOBJ_HWPARENT) {
        lck->Enter();
        sprintf(tmp, "%02X%02X:%02X%02X:%02X%02X", dat.Octets[0], dat.Octets[1],
            dat.Octets[2], dat.Octets[3], dat.Octets[4], dat.Octets[5], dat.Octets[6]);
        it = Main->lvNodeList->FindData(0, (void *)msg->AddressFrom, true, false);
        if(it != NULL)
            it->SubItems->Strings[3] = tmp;
        lck->Leave();
    }               

    if(msg->AddressFrom != Main->fromaddr)
        return 1;

    lck->Enter();

    switch(object) {
    case MBN_NODEOBJ_DESCRIPTION:
        Main->lblNodeDesc->Caption = (char *)dat.Octets;
        break;
    case MBN_NODEOBJ_HWMAJOR:
        Main->lblNodeHWMa->Caption = data2str(type, dat);
        break;
    case MBN_NODEOBJ_HWMINOR:
        Main->lblNodeHWMi->Caption = data2str(type, dat);
        break;
    case MBN_NODEOBJ_FWMAJOR:
        Main->lblNodeFWMa->Caption = data2str(type, dat);
        break;
    case MBN_NODEOBJ_FWMINOR:
        Main->lblNodeFWMi->Caption = data2str(type, dat);
        break;
    case MBN_NODEOBJ_FPGAMAJOR:
        Main->lblNodeFPMa->Caption = data2str(type, dat);
        break;
    case MBN_NODEOBJ_FPGAMINOR:
        Main->lblNodeFPMi->Caption = data2str(type, dat);
        break;
    case MBN_NODEOBJ_PROTOMAJOR:
        Main->lblNodePRMa->Caption = data2str(type, dat);
        break;
    case MBN_NODEOBJ_PROTOMINOR:
        Main->lblNodePRMi->Caption = data2str(type, dat);
        break;
    case MBN_NODEOBJ_HWPARENT:
        Main->lblNodeHWPar->Caption = tmp;
        break;
    case MBN_NODEOBJ_SERVICEREQUEST:
        Main->lblNodeSR->Caption = dat.State ? "Service requested" : "No service required";
        break;
    case MBN_NODEOBJ_NUMBEROFOBJECTS:
        Main->lblObjCount->Caption = data2str(type, dat);
        if(dat.UInt != 0) {
            Main->objcount = dat.UInt;
            sprintf(tmp, "0/%d", Main->objcount);
            Main->lblObjStatus->Caption = tmp;
            Main->btnObjRetry->Enabled = true;
            for(i=0; i<dat.UInt; i++)
                mbnGetObjectInformation(mbn, Main->fromaddr, 1024+i, 0);
        }
        break;
    default:
        if(object == Main->objnr)
            Main->lblObjSensor->Caption = data2str(type, dat);
        else
            r = 1;
    }
    lck->Leave();
    return r;
}

void mAcknowledgeTimeout(struct mbn_handler *mbn, struct mbn_message *a) {
    addlog(LF_CB, "Acknowledge timeout for message #%d to %08lX", a->MessageID, a->AddressTo);
}

int mObjectInformationResponse(struct mbn_handler *mbn, struct mbn_message *msg, unsigned short obj, struct mbn_object *nfo) {
    char tmp[30]; 
    TListItem *it;
    struct mbn_object *o;
    int i;

    if(nfo == NULL) {
        addlog(LF_CB, "WTF!? nfo = NULL!?");
        return 1;
    }

    addlog(LF_CB, "Object information response from %08lX object #%d", msg->AddressFrom, obj);
    if(msg->AddressFrom != Main->fromaddr)
        return 1;

    if(obj < 1024) {
        addlog(LF_APP, "Object %d out of bound!", obj);
        return 1;
    }

    lck->Enter();
    Main->lvObjects->Items->BeginUpdate();

    it = NULL;
    for(i=0; i<Main->lvObjects->Items->Count; i++) {
       o = (struct mbn_object *)Main->lvObjects->Items->Item[i]->Data;
       if(o->timeout == obj)
           it = Main->lvObjects->Items->Item[i];
    }
    if(it == NULL) {
        o = (struct mbn_object *)malloc(sizeof(struct mbn_object));
        memcpy(o, nfo, sizeof(mbn_object));
        o->timeout = obj;
        it = Main->lvObjects->Items->Add();
        it->Data = (void *)o;
        sprintf(tmp, "%d/%d", ++Main->objget, Main->objcount);
        Main->lblObjStatus->Caption = tmp;
        if(Main->objget == Main->objcount)
            Main->btnObjRetry->Enabled = false;
    }
    sprintf(tmp, "%6d", obj);
    it->Caption = tmp;       
    sprintf(tmp, "%02X", nfo->Services);
    it->SubItems->Add(tmp);
    it->SubItems->Add((char *)nfo->Description);

    if(nfo->SensorType != MBN_DATATYPE_NODATA) {
      sprintf(tmp, "%d %s", nfo->SensorSize, type2str(nfo->SensorType));
      it->SubItems->Add(tmp);
      sprintf(tmp, "%.0f %.0f",
        NUMTYPE(MMTYPE(nfo->SensorType), nfo->SensorMin),
        NUMTYPE(MMTYPE(nfo->SensorType), nfo->SensorMax));
      it->SubItems->Add(tmp);
    } else {
      it->SubItems->Add("-");
      it->SubItems->Add("");
    }

    if(nfo->ActuatorType != MBN_DATATYPE_NODATA) {
      sprintf(tmp, "%d %s", nfo->ActuatorSize, type2str(nfo->ActuatorType));
      it->SubItems->Add(tmp);
      sprintf(tmp, "%.0f %.0f %.0f",
        NUMTYPE(MMTYPE(nfo->ActuatorType), nfo->ActuatorMin),
        NUMTYPE(MMTYPE(nfo->ActuatorType), nfo->ActuatorMax),
        NUMTYPE(MMTYPE(nfo->ActuatorType), nfo->ActuatorDefault));
      it->SubItems->Add(tmp);
    } else {
      it->SubItems->Add("-");
      it->SubItems->Add("");      
    }

    Main->lvObjects->Items->EndUpdate();
    lck->Leave();
    return 0;
}

void mObjectError(struct mbn_handler *mbn, struct mbn_message *msg, unsigned short obj, char *err) {
    addlog(LF_CB, "Object error from %08lX object #%d: %s", msg->AddressFrom, obj, err);
}

int mObjectFrequencyResponse(struct mbn_handler *mbn, struct mbn_message *msg, unsigned short obj, unsigned char freq) {
    addlog(LF_CB, "Object frequency response from %08lX object #%d: %02X", msg->AddressFrom, obj, freq);
    if(msg->AddressFrom != Main->fromaddr)
        return 1;
    lck->Enter();
    if(obj == Main->objnr)
        Main->cbObjFreq->ItemIndex = freq+1;
    lck->Leave();
}

int mSensorDataChanged(struct mbn_handler *mbn, struct mbn_message *msg, unsigned short obj, unsigned char type, union mbn_data dat) {
    addlog(LF_CB, "Sensor data changed from %08lX object %d: %s", msg->AddressFrom, obj, data2str(type, dat));
}

//---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner)
  : TForm(Owner)
{
     validaddr = 0;
     mbn = NULL;
     iflist = NULL;
     lck = new TCriticalSection();
}

__fastcall TMain::~TMain()
{
    if(iflist)
        mbnEthernetIFFree(iflist);
    if(mbn)
        mbnFree(mbn);
    mbn = NULL;
    //delete lck;
}


//---------------------------------------------------------------------------
void __fastcall TMain::FormCreate(TObject *Sender)
{
    char err[MBN_ERRSIZE], ifdesc[512];
    struct mbn_if_ethernet *n;

    if((iflist = mbnEthernetIFList(err)) == NULL)
        addlog(LF_APP, "Error getting interface list: %s", err);
    else {
        lstInterfaces->Items->Clear();
        for(n=iflist; n!=NULL; n=n->next) {
            sprintf(ifdesc, "%02X:%02X:%02X:%02X:%02X:%02X",
                n->addr[0], n->addr[1], n->addr[2], n->addr[3], n->addr[4], n->addr[5]);
            lstInterfaces->Items->AddObject(ifdesc, (TObject *)n->name);
        }
        lstInterfaces->ItemIndex = 0;
    }

    TRegistry *reg = new TRegistry;
    if(reg->OpenKey("\\Software\\MambaNet Tester", false) && reg->ValueExists("UniqueID"))
      cseUniqueID->Value = reg->ReadInteger("UniqueID");
    delete reg;

    lvObjects->Columns->Items[0]->Width = 40;   
    lvObjects->Columns->Items[1]->Width = 25;
    lvObjects->Columns->Items[2]->Width = 130;
    lvObjects->Columns->Items[3]->Width = 60;
    lvObjects->Columns->Items[4]->Width = 65;
    lvObjects->Columns->Items[5]->Width = 60;
    lvObjects->Columns->Items[6]->Width = 90;  
    lvObjects->Selected = NULL;

    lvNodeList->Columns->Items[0]->Width = 60;
    lvNodeList->Columns->Items[1]->Width = 95;
    lvNodeList->Columns->Items[2]->Width = 60;
    lvNodeList->Columns->Items[3]->Width = 25;
    lvNodeList->Columns->Items[4]->Width = 95;
    lvNodeList->Columns->Items[5]->Width = 130;   
    lvNodeList->Selected = NULL;
      
    cbObjFreq->ItemIndex = -1;
    btnObjStrAct->Visible = false;
    txtObjStrAct->Visible = false;

    StatusBar->Panels->Items[0]->Text = "Not connected";
}
//---------------------------------------------------------------------------


void __fastcall TMain::btnOpenClick(TObject *Sender) {
    struct mbn_interface *itf;
    char error[MBN_ERRSIZE];

    if(lstInterfaces->ItemIndex < 0)
        return addlog(LF_APP, "No device selected!");
    if((itf = mbnEthernetOpen((char *)lstInterfaces->Items->Objects[lstInterfaces->ItemIndex], error)) == NULL)
        return addlog(LF_APP, "EthernetOpen failed: %s", error);

    thisnode.UniqueIDPerProduct = cseUniqueID->Value;
    if((mbn = mbnInit(&thisnode, NULL, itf, error)) == NULL)
        return addlog(LF_APP, "Init failed: %s", error);

    mbnSetErrorCallback(mbn, mError);
    mbnSetOnlineStatusCallback(mbn, mOnlineStatus);
    mbnSetAddressTableChangeCallback(mbn, mAddressTableChange);
    mbnSetActuatorDataResponseCallback(mbn, mActuatorDataResponse);
    mbnSetSensorDataResponseCallback(mbn, mSensorDataResponse);
    mbnSetAcknowledgeTimeoutCallback(mbn, mAcknowledgeTimeout);
    mbnSetObjectInformationResponseCallback(mbn, mObjectInformationResponse);
    mbnSetObjectErrorCallback(mbn, mObjectError);
    mbnSetObjectFrequencyResponseCallback(mbn, mObjectFrequencyResponse);
    mbnSetSensorDataChangedCallback(mbn, mSensorDataChanged);
    addlog(LF_APP, "Created MambaNet node");

    lstInterfaces->Enabled = false;
    cseUniqueID->Enabled = false;
    btnOpen->Enabled = false;
    btnClose->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMain::btnCloseClick(TObject *Sender) {
    lck->Enter();
    lvNodeList->Items->Clear();
    lstInterfaces->Enabled = true;
    cseUniqueID->Enabled = true;
    btnOpen->Enabled = true;
    btnClose->Enabled = false;
    if(mbn)
        mbnFree(mbn);
    mbn = NULL;
    validaddr = 0;                          
    lck->Leave();
    addlog(LF_APP, "Closed MambaNet node");
}
//---------------------------------------------------------------------------



void __fastcall TMain::mbnPingAllClick(TObject *Sender) {
    if(!mbn)
        return addlog(LF_APP, "Not connected!");
    mbnSendPingRequest(mbn, MBN_BROADCAST_ADDRESS);  
}
//---------------------------------------------------------------------------




void __fastcall TMain::btnLogClearClick(TObject *Sender) {
    lck->Enter();
    memLog->Lines->Clear();
    lck->Leave();
}
//---------------------------------------------------------------------------


void __fastcall TMain::cbListFetchClick(TObject *Sender)
{
    int i;
    TListItem *it;

    lck->Enter();
    if(!cbListFetch->Checked) {
        lck->Leave();
        return;
    }

    for(i=0; i<lvNodeList->Items->Count; i++) {
        it = lvNodeList->Items->Item[i];
        if(it->SubItems->Strings[3].AnsiCompare("-") == 0)
            mbnGetSensorData(mbn, (unsigned long)it->Data, MBN_NODEOBJ_HWPARENT, 1);
        if(it->SubItems->Strings[4].AnsiCompare("-") == 0)
            mbnGetActuatorData(mbn, (unsigned long)it->Data, MBN_NODEOBJ_NAME, 1);
    }
    lck->Leave();
}
//---------------------------------------------------------------------------


void __fastcall TMain::lvNodeListDblClick(TObject *Sender)
{
    char tmp[100], ack;
    int n;

    lck->Enter();
    if(lvNodeList->Selected == NULL) {
        lck->Leave();
        return;
    }
    pcTabs->ActivePageIndex = 1;

    if(!validaddr) {
        lck->Leave();
        return addlog(LF_APP, "We don't have a valid address yet!");
    }

    fromaddr = (unsigned long)lvNodeList->Selected->Data;
    lblNodeName->Caption = "-";
    lblNodeDesc->Caption = "-";
    lblNodeAddr->Caption = lvNodeList->Selected->Caption;               
    lblNodeID->Caption   = lvNodeList->Selected->SubItems->Strings[0];
    lblNodeHWMa->Caption = "-";
    lblNodeHWMi->Caption = "-";
    lblNodeFWMa->Caption = "-";
    lblNodeFWMi->Caption = "-";
    lblNodeFPMa->Caption = "-";
    lblNodeFPMi->Caption = "-";
    lblNodePRMa->Caption = "-";
    lblNodePRMi->Caption = "-";
    lblNodeHWPar->Caption = "-";
    lblNodeSR->Caption = "-";
    lblObjCount->Caption = "-";
    lblNodeEngine->Caption = lvNodeList->Selected->SubItems->Strings[1];
    lvObjects->Items->BeginUpdate();
    lvObjects->Items->Clear();
    lvObjects->Items->EndUpdate();
    lblObjStatus->Caption = "0/0";
    btnObjRetry->Enabled = false;
    objnr = objcount = objget = 0; 
    lck->Leave();
    
    ack = 0;
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_DESCRIPTION, ack); 
    mbnGetActuatorData(mbn, fromaddr, MBN_NODEOBJ_NAME, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_HWMAJOR, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_HWMINOR, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_FWMAJOR, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_FWMINOR, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_FPGAMAJOR, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_FPGAMINOR, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_PROTOMAJOR, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_PROTOMINOR, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_HWPARENT, ack);
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_SERVICEREQUEST, ack); 
    mbnGetSensorData(mbn, fromaddr, MBN_NODEOBJ_NUMBEROFOBJECTS, ack);
}
//---------------------------------------------------------------------------

void __fastcall TMain::lvObjectsSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    TListItem *l;
    struct mbn_object *nfo, *o;
    TItemStates sel = TItemStates() << isSelected;
    double min, max;
    char tmp[50];
    int same = 1;
    
    lck->Enter();
    lblObjSensor->Caption = "-";
    lblObjActuator->Caption = "-";
    cbObjFreq->ItemIndex = 0; 
    btnObjStrAct->Visible = false;
    txtObjStrAct->Visible = false;
    btnObjRetry->Visible = true;
    lblObjStatus->Visible = true;
    objnr = 0;

    if(lvObjects->SelCount < 1 || Item == NULL) {
        lck->Leave();
        return;
    }
               
    nfo = (struct mbn_object *)Item->Data;
    if(nfo == NULL) {
        lck->Leave();
        return;
    }
    if(lvObjects->SelCount == 1 && Selected) {
        objnr = nfo->timeout;
        if(nfo->SensorType != MBN_DATATYPE_NODATA)
            mbnGetSensorData(mbn, fromaddr, objnr, 1);
        if(nfo->ActuatorType != MBN_DATATYPE_NODATA)
            mbnGetActuatorData(mbn, fromaddr, objnr, 1);
        mbnGetObjectFrequency(mbn, fromaddr, objnr, 1);
    }

    /* make sure all selected actuators are of the same type */
    min = NUMTYPE(MMTYPE(nfo->ActuatorType), nfo->ActuatorMin);
    max = NUMTYPE(MMTYPE(nfo->ActuatorType), nfo->ActuatorMax);
    if(nfo->ActuatorType != MBN_DATATYPE_NODATA) {
        for(l=lvObjects->Selected; l!=NULL; l=lvObjects->GetNextItem(l, sdAll, sel)) {
            o = (struct mbn_object *)l->Data;
            if(o->ActuatorType != nfo->ActuatorType
             || (__int64)(NUMTYPE(MMTYPE(o->ActuatorType), o->ActuatorMax)*100) != (__int64)(max*100)
             || (__int64)(NUMTYPE(MMTYPE(o->ActuatorType), o->ActuatorMin)*100) != (__int64)(min*100)) {
                same = 0;
                break;
            }
        }
        if(same) {
            btnObjRetry->Visible = false;
            lblObjStatus->Visible = false;
            btnObjStrAct->Visible = true;
            txtObjStrAct->Text = "";
            txtObjStrAct->Visible = true;
        }
    }

    lck->Leave();
}
//---------------------------------------------------------------------------

void __fastcall TMain::cbObjFreqChange(TObject *Sender)
{
    TListItem *l;  
    struct mbn_object *nfo;
    TItemStates sel = TItemStates() << isSelected;

    if(cbObjFreq->ItemIndex < 1)
        return;
    if(lvObjects->SelCount < 1)
        return addlog(LF_APP, "Nothing selected!");

    lck->Enter();
    for(l=lvObjects->Selected; l!=NULL; l=lvObjects->GetNextItem(l, sdAll, sel)) {
        nfo = (struct mbn_object *)l->Data;
        addlog(LF_APP, "Setting object frequency of %08lX object #%d to %02X", fromaddr, nfo->timeout, cbObjFreq->ItemIndex-1);
        mbnSetObjectFrequency(mbn, fromaddr, nfo->timeout, cbObjFreq->ItemIndex-1, 1);
    }
    lck->Leave();
}
//---------------------------------------------------------------------------

void __fastcall TMain::lvObjectsDeletion(TObject *Sender, TListItem *Item)
{
    if(Item->Data != NULL) {
        free(Item->Data);
        Item->Data = NULL;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMain::btnObjStrActClick(TObject *Sender)
{
    TItemStates sel = TItemStates() << isSelected; 
    TListItem *l;
    union mbn_data dat;
    struct mbn_object *nfo;
    unsigned char *str[100];
    char *tmp;

    lck->Enter();
    if(txtObjStrAct->Visible == false) {
        lck->Leave();
        return;
    }

    if(lvObjects->SelCount < 1) {
        addlog(LF_APP, "Nothing selected!");
        lck->Leave();
        return;
    }

    for(l=lvObjects->Selected; l!=NULL; l=lvObjects->GetNextItem(l, sdAll, sel)) {
        nfo = (struct mbn_object *)l->Data;
        switch(nfo->ActuatorType) {
        case MBN_DATATYPE_UINT:
        case MBN_DATATYPE_STATE:
            if(txtObjStrAct->Text.Pos("0x") == 1) {
                tmp = txtObjStrAct->Text.c_str();
                dat.UInt = hex2int(tmp+2, strlen(tmp)-2);
            } else
                dat.UInt = (unsigned long) txtObjStrAct->Text.ToDouble();
            break;
        case MBN_DATATYPE_FLOAT:  dat.Float = (float) txtObjStrAct->Text.ToDouble(); break;
        case MBN_DATATYPE_SINT:   dat.SInt = txtObjStrAct->Text.ToInt(); break;
        case MBN_DATATYPE_OCTETS:    
            memset((void *)str, 0, 100);
            strncpy((char *)str, txtObjStrAct->Text.c_str(), 100);
            dat.Octets = (unsigned char *)str;
            break;
        }
        addlog(LF_APP, "Setting actuator data of %08lX object #%d to %s", fromaddr, nfo->timeout, data2str(nfo->ActuatorType, dat));
        mbnSetActuatorData(mbn, fromaddr, nfo->timeout, nfo->ActuatorType, nfo->ActuatorSize, dat, 1);
    }
    lblObjActuator->Caption = data2str(nfo->ActuatorType, dat);
    lck->Leave();
}
//---------------------------------------------------------------------------

void __fastcall TMain::btnObjRetryClick(TObject *Sender)
{
    TListItem *it;
    struct mbn_object *nfo;
    int i, j, last;

    lck->Enter();
    last = 1023;
    for(i=0; i<lvObjects->Items->Count; i++) {
        nfo = (struct mbn_object *)lvObjects->Items->Item[i]->Data;
        for(j=last+1;j<nfo->timeout;j++)
            mbnGetObjectInformation(mbn, fromaddr, j, 0);
        last = nfo->timeout;
    }
    lck->Leave();
}
//---------------------------------------------------------------------------

void __fastcall TMain::cseUniqueIDChange(TObject *Sender)
{
   TRegistry *reg = new TRegistry;
   if(reg->OpenKey("\\Software\\MambaNet Tester", true))
     reg->WriteInteger("UniqueID", cseUniqueID->Value);
   delete reg;
}
//---------------------------------------------------------------------------

void __fastcall TMain::RefreshTimerTimer(TObject *Sender)
{
    if(mbn && cbObjRefresh->Checked && lvObjects->SelCount >= 0 && pcTabs->ActivePageIndex == 1)
        lvObjectsSelectItem(Sender, lvObjects->Selected, true);
}
//---------------------------------------------------------------------------

void __fastcall TMain::lvNodeListCompare(TObject *Sender, TListItem *Item1,
      TListItem *Item2, int Data, int &Compare)
{
    AnsiString a, b;
    int column = nodelistsort & ~0x80;

    if(nodelistsort & 0x80) { /* reversed */
       a = column ? Item2->SubItems->Strings[column-1] : Item2->Caption;
       b = column ? Item1->SubItems->Strings[column-1] : Item1->Caption;
    } else {
       a = column ? Item1->SubItems->Strings[column-1] : Item1->Caption;
       b = column ? Item2->SubItems->Strings[column-1] : Item2->Caption;
    }
    Compare = a.AnsiCompare(b);
}

void __fastcall TMain::lvNodeListColumnClick(TObject *Sender,
      TListColumn *Column)
{
    if((nodelistsort & ~0x80) == Column->Index) {
        if(nodelistsort & 0x80)
            nodelistsort -= 0x80;
        else
            nodelistsort |= 0x80;
    } else if(nodelistsort & 0x80)
        nodelistsort -= 0x80;
    nodelistsort = Column->Index | (nodelistsort & 0x80);
    lvNodeList->CustomSort(NULL, 0);
}
//---------------------------------------------------------------------------

