object Main: TMain
  Left = 474
  Top = 35
  Width = 544
  Height = 666
  Caption = 'MambaNet Tester'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 5
    Top = 5
    Width = 523
    Height = 116
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Connection'
    TabOrder = 0
    object Label1: TLabel
      Left = 360
      Top = 39
      Width = 45
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'UniqueID'
    end
    object Label3: TLabel
      Left = 423
      Top = 39
      Width = 77
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'MambaNet Addr'
    end
    object btnOpen: TButton
      Left = 360
      Top = 14
      Width = 51
      Height = 21
      Anchors = [akTop, akRight]
      Caption = 'Open'
      TabOrder = 0
      OnClick = btnOpenClick
    end
    object txtMyAddress: TEdit
      Left = 423
      Top = 54
      Width = 86
      Height = 21
      Anchors = [akTop, akRight]
      Enabled = False
      TabOrder = 1
      Text = '0'
    end
    object btnClose: TButton
      Left = 423
      Top = 14
      Width = 51
      Height = 21
      Anchors = [akTop, akRight]
      Caption = 'Close'
      Enabled = False
      TabOrder = 2
      OnClick = btnCloseClick
    end
    object cseUniqueID: TCSpinEdit
      Left = 360
      Top = 54
      Width = 51
      Height = 22
      TabStop = True
      Anchors = [akTop, akRight]
      MaxValue = 65535
      ParentColor = False
      TabOrder = 3
      Value = 1
      OnChange = cseUniqueIDChange
    end
    object ConnectionPageControl: TPageControl
      Left = 16
      Top = 16
      Width = 323
      Height = 81
      ActivePage = EthernetPage
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 4
      object EthernetPage: TTabSheet
        Caption = 'Ethernet'
        object Label2: TLabel
          Left = 10
          Top = 7
          Width = 42
          Height = 13
          Caption = 'Interface'
        end
        object lstInterfaces: TComboBox
          Left = 10
          Top = 22
          Width = 300
          Height = 21
          Style = csDropDownList
          Anchors = [akLeft, akTop, akRight]
          ItemHeight = 13
          TabOrder = 0
          Items.Strings = (
            'test'
            'test 1')
        end
      end
      object UDPPage: TTabSheet
        Caption = 'UDP/IP'
        ImageIndex = 1
        object Label22: TLabel
          Left = 10
          Top = 7
          Width = 56
          Height = 13
          Caption = '<url>:<port>'
        end
        object UDPEdit: TEdit
          Left = 10
          Top = 22
          Width = 300
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
      end
      object TCPPage: TTabSheet
        Caption = 'TCP/IP'
        ImageIndex = 2
        object Label24: TLabel
          Left = 10
          Top = 7
          Width = 56
          Height = 13
          Caption = '<url>:<port>'
        end
        object TCPEdit: TEdit
          Left = 10
          Top = 22
          Width = 300
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
      end
    end
    object AboutButton: TButton
      Left = 412
      Top = 86
      Width = 97
      Height = 21
      Anchors = [akTop, akRight]
      Caption = 'About mbnlib.dll'
      TabOrder = 5
      OnClick = AboutButtonClick
    end
  end
  object pcTabs: TPageControl
    Left = 5
    Top = 136
    Width = 523
    Height = 483
    ActivePage = TabSheet3
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 1
    object TabSheet3: TTabSheet
      Caption = 'Node list'
      ImageIndex = 2
      object cbListFetch: TCheckBox
        Left = 5
        Top = 437
        Width = 161
        Height = 16
        Anchors = [akLeft, akBottom]
        Caption = 'Fetch additional information'
        TabOrder = 0
        OnClick = cbListFetchClick
      end
      object mbnPingAll: TButton
        Left = 439
        Top = 434
        Width = 71
        Height = 21
        Anchors = [akRight, akBottom]
        Caption = 'Ping all'
        TabOrder = 1
        OnClick = mbnPingAllClick
      end
      object lvNodeList: TListView
        Left = 5
        Top = 5
        Width = 503
        Height = 428
        Anchors = [akLeft, akTop, akRight, akBottom]
        Columns = <
          item
            Caption = 'Address'
          end
          item
            Caption = 'ID'
          end
          item
            Caption = 'Engine'
          end
          item
            Caption = 'Serv'
          end
          item
            Caption = 'Parent'
          end
          item
            Caption = 'Name'
          end>
        GridLines = True
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        SortType = stText
        TabOrder = 2
        ViewStyle = vsReport
        OnColumnClick = lvNodeListColumnClick
        OnCompare = lvNodeListCompare
        OnDblClick = lvNodeListDblClick
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Node information'
      ImageIndex = 1
      object Label4: TLabel
        Left = 10
        Top = 15
        Width = 31
        Height = 13
        Caption = 'Name:'
      end
      object lblNodeName: TLabel
        Left = 80
        Top = 15
        Width = 3
        Height = 13
        Caption = '-'
      end
      object Label5: TLabel
        Left = 10
        Top = 0
        Width = 56
        Height = 13
        Caption = 'Description:'
      end
      object lblNodeDesc: TLabel
        Left = 80
        Top = 0
        Width = 3
        Height = 13
        Caption = '-'
      end
      object Label6: TLabel
        Left = 10
        Top = 30
        Width = 49
        Height = 13
        Caption = 'Revisions:'
      end
      object Label7: TLabel
        Left = 80
        Top = 30
        Width = 22
        Height = 13
        Caption = 'HW:'
      end
      object lblNodeHWMa: TLabel
        Left = 127
        Top = 30
        Width = 3
        Height = 13
        Alignment = taRightJustify
        Caption = '-'
      end
      object Label9: TLabel
        Left = 130
        Top = 30
        Width = 3
        Height = 13
        Caption = '.'
      end
      object lblNodeHWMi: TLabel
        Left = 133
        Top = 30
        Width = 3
        Height = 13
        Caption = '-'
      end
      object Label8: TLabel
        Left = 170
        Top = 30
        Width = 20
        Height = 13
        Caption = 'FW:'
      end
      object lblNodeFWMa: TLabel
        Left = 217
        Top = 30
        Width = 3
        Height = 13
        Alignment = taRightJustify
        Caption = '-'
      end
      object Label11: TLabel
        Left = 220
        Top = 30
        Width = 3
        Height = 13
        Caption = '.'
      end
      object lblNodeFWMi: TLabel
        Left = 223
        Top = 30
        Width = 3
        Height = 13
        Caption = '-'
      end
      object Label10: TLabel
        Left = 290
        Top = 30
        Width = 31
        Height = 13
        Caption = 'FPGA:'
      end
      object lblNodeFPMa: TLabel
        Left = 347
        Top = 30
        Width = 3
        Height = 13
        Alignment = taRightJustify
        Caption = '-'
      end
      object Label13: TLabel
        Left = 350
        Top = 30
        Width = 3
        Height = 13
        Caption = '.'
      end
      object lblNodeFPMi: TLabel
        Left = 353
        Top = 30
        Width = 3
        Height = 13
        Caption = '-'
      end
      object lblNodePRMi: TLabel
        Left = 453
        Top = 30
        Width = 3
        Height = 13
        Caption = '-'
      end
      object Label14: TLabel
        Left = 450
        Top = 30
        Width = 3
        Height = 13
        Caption = '.'
      end
      object lblNodePRMa: TLabel
        Left = 447
        Top = 30
        Width = 3
        Height = 13
        Alignment = taRightJustify
        Caption = '-'
      end
      object Label16: TLabel
        Left = 395
        Top = 30
        Width = 33
        Height = 13
        Caption = 'PROT:'
      end
      object Label12: TLabel
        Left = 170
        Top = 45
        Width = 36
        Height = 13
        Caption = 'Engine:'
      end
      object lblNodeEngine: TLabel
        Left = 215
        Top = 45
        Width = 3
        Height = 13
        Caption = '-'
      end
      object Label15: TLabel
        Left = 10
        Top = 45
        Width = 56
        Height = 13
        Caption = 'HW Parent:'
      end
      object lblNodeHWPar: TLabel
        Left = 80
        Top = 45
        Width = 3
        Height = 13
        Caption = '-'
      end
      object lblNodeSR: TLabel
        Left = 395
        Top = 45
        Width = 3
        Height = 13
        Caption = '-'
      end
      object Label17: TLabel
        Left = 290
        Top = 45
        Width = 39
        Height = 13
        Caption = 'Objects:'
      end
      object lblObjCount: TLabel
        Left = 343
        Top = 45
        Width = 3
        Height = 13
        Caption = '-'
      end
      object lblNodeAddr: TLabel
        Left = 343
        Top = 0
        Width = 3
        Height = 13
        Caption = '-'
      end
      object Label19: TLabel
        Left = 290
        Top = 0
        Width = 41
        Height = 13
        Caption = 'Address:'
      end
      object Label18: TLabel
        Left = 10
        Top = 412
        Width = 36
        Height = 13
        Anchors = [akLeft, akBottom]
        Caption = 'Sensor:'
      end
      object Label20: TLabel
        Left = 252
        Top = 412
        Width = 43
        Height = 13
        Anchors = [akRight, akBottom]
        Caption = 'Actuator:'
      end
      object lblObjSensor: TLabel
        Left = 70
        Top = 412
        Width = 3
        Height = 13
        Anchors = [akLeft, akBottom]
        Caption = '-'
      end
      object lblObjActuator: TLabel
        Left = 302
        Top = 412
        Width = 3
        Height = 13
        Anchors = [akRight, akBottom]
        Caption = '-'
      end
      object Label21: TLabel
        Left = 10
        Top = 437
        Width = 53
        Height = 13
        Anchors = [akLeft, akBottom]
        Caption = 'Frequency:'
      end
      object lblNodeID: TLabel
        Left = 343
        Top = 15
        Width = 3
        Height = 13
        Caption = '-'
      end
      object Label23: TLabel
        Left = 290
        Top = 15
        Width = 48
        Height = 13
        Caption = 'UniqueID:'
      end
      object lblObjStatus: TLabel
        Left = 437
        Top = 437
        Width = 17
        Height = 13
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        Caption = '0/0'
      end
      object lvObjects: TListView
        Left = 5
        Top = 60
        Width = 503
        Height = 348
        Anchors = [akLeft, akTop, akRight, akBottom]
        Columns = <
          item
            Caption = 'Nr.'
          end
          item
            Caption = 'Serv'
          end
          item
            Caption = 'Description'
          end
          item
            Caption = 'S type'
          end
          item
            Caption = 'S min-max'
          end
          item
            Caption = 'A type'
          end
          item
            Caption = 'A min-max-def'
          end>
        ColumnClick = False
        GridLines = True
        HideSelection = False
        MultiSelect = True
        ReadOnly = True
        RowSelect = True
        SortType = stText
        TabOrder = 0
        ViewStyle = vsReport
        OnDeletion = lvObjectsDeletion
        OnSelectItem = lvObjectsSelectItem
      end
      object cbObjFreq: TComboBox
        Left = 70
        Top = 432
        Width = 91
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akBottom]
        ItemHeight = 13
        TabOrder = 1
        OnChange = cbObjFreqChange
        Items.Strings = (
          '[unknown]'
          'no changes'
          'all changes'
          '25 Hz'
          '10 Hz'
          '5 Hz'
          '1 Hz'
          '0.2 Hz'
          '0.1 Hz')
      end
      object txtObjStrAct: TEdit
        Left = 277
        Top = 432
        Width = 186
        Height = 21
        Anchors = [akRight, akBottom]
        TabOrder = 2
      end
      object btnObjStrAct: TButton
        Left = 467
        Top = 432
        Width = 46
        Height = 21
        Anchors = [akRight, akBottom]
        Caption = 'Set'
        TabOrder = 3
        OnClick = btnObjStrActClick
      end
      object btnObjRetry: TButton
        Left = 467
        Top = 432
        Width = 46
        Height = 21
        Anchors = [akRight, akBottom]
        Caption = 'Retry'
        Enabled = False
        TabOrder = 4
        OnClick = btnObjRetryClick
      end
      object cbObjRefresh: TCheckBox
        Left = 432
        Top = 412
        Width = 81
        Height = 16
        Anchors = [akRight, akBottom]
        Caption = 'Auto-refresh'
        TabOrder = 5
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Log'
      object memLog: TMemo
        Left = 5
        Top = 5
        Width = 491
        Height = 371
        Anchors = [akLeft, akTop, akRight, akBottom]
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Lucida Console'
        Font.Style = []
        ParentFont = False
        ScrollBars = ssBoth
        TabOrder = 0
      end
      object btnLogClear: TButton
        Left = 420
        Top = 380
        Width = 76
        Height = 21
        Anchors = [akRight, akBottom]
        Caption = 'Clear'
        TabOrder = 1
        OnClick = btnLogClearClick
      end
      object chkLogCb: TCheckBox
        Left = 90
        Top = 380
        Width = 71
        Height = 21
        Hint = 'Callbacks from mbn'
        Anchors = [akLeft, akBottom]
        Caption = 'Callbacks'
        Checked = True
        ParentShowHint = False
        ShowHint = True
        State = cbChecked
        TabOrder = 2
      end
      object chkLogObj: TCheckBox
        Left = 165
        Top = 380
        Width = 61
        Height = 21
        Hint = 'Show raw object messages'
        Anchors = [akLeft, akBottom]
        Caption = 'Objects'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
      end
      object chkLogAdr: TCheckBox
        Left = 235
        Top = 380
        Width = 76
        Height = 21
        Hint = 'Show raw address messages'
        Anchors = [akLeft, akBottom]
        Caption = 'Addresses'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
      end
      object chkLogApp: TCheckBox
        Left = 5
        Top = 380
        Width = 76
        Height = 21
        Hint = 'Application messages'
        Anchors = [akLeft, akBottom]
        Caption = 'App. msgs'
        Checked = True
        ParentShowHint = False
        ShowHint = True
        State = cbChecked
        TabOrder = 5
      end
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 614
    Width = 536
    Height = 18
    Panels = <
      item
        Width = 100
      end>
    SimplePanel = False
  end
  object RefreshTimer: TTimer
    OnTimer = RefreshTimerTimer
    Left = 495
    Top = 5
  end
end
