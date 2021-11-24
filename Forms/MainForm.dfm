object Main_Application_Window: TMain_Application_Window
  Left = 0
  Top = 0
  Align = alClient
  Caption = 'EPLab Research Works'
  ClientHeight = 1020
  ClientWidth = 1918
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenuResearch
  OldCreateOrder = False
  OnAfterMonitorDpiChanged = FormAfterMonitorDpiChanged
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnPaint = FormPaint
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Annotation_Window_PaintBox: TPaintBox
    Left = 399
    Top = 401
    Width = 617
    Height = 193
    OnMouseDown = Annotation_Window_PaintBoxMouseDown
    OnMouseMove = Annotation_Window_PaintBoxMouseMove
    OnMouseUp = Annotation_Window_PaintBoxMouseUp
    OnPaint = Annotation_Window_PaintBoxPaint
  end
  object Panel_1: TPanel
    Left = 377
    Top = 62
    Width = 273
    Height = 242
    PopupMenu = OpenGL_Panel_PopupMenu
    TabOrder = 0
    OnMouseDown = Panel_1MouseDown
    OnMouseMove = Panel_1MouseMove
    OnMouseUp = Panel_1MouseUp
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 1918
    Height = 29
    ButtonHeight = 26
    ButtonWidth = 33
    Caption = 'ToolBar1'
    TabOrder = 1
    object AP_View_Button: TButton
      Left = 0
      Top = 0
      Width = 66
      Height = 26
      Caption = 'AP view'
      TabOrder = 0
      OnClick = AP_View_ButtonClick
    end
    object PA_View_Button: TButton
      Left = 66
      Top = 0
      Width = 66
      Height = 26
      Caption = 'PA view'
      TabOrder = 1
      OnClick = PA_View_ButtonClick
    end
  end
  object Left_PageControl: TPageControl
    Left = 0
    Top = 43
    Width = 322
    Height = 558
    ActivePage = TabSheet_Main
    TabOrder = 2
    object TabSheet_Main: TTabSheet
      Caption = 'Main'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label_p1: TLabel
        Left = 3
        Top = 2
        Width = 141
        Height = 13
        Caption = 'Currently selected geometry:'
      end
      object DPSet_Label: TLabel
        Left = 3
        Top = 48
        Width = 167
        Height = 13
        Caption = 'Currently selected data points set:'
      end
      object Label2: TLabel
        Left = 3
        Top = 93
        Width = 79
        Height = 13
        Caption = 'Displayed value:'
      end
      object Deleted_Data_Points_Label: TLabel
        Left = 3
        Top = 294
        Width = 98
        Height = 13
        Caption = 'Deleted data points:'
      end
      object Zoom_Label: TLabel
        Left = 263
        Top = 6
        Width = 26
        Height = 13
        Caption = 'Zoom'
      end
      object Zoom_Slider_Image: TImage
        Left = 263
        Top = 32
        Width = 34
        Height = 92
        Picture.Data = {
          0A544A504547496D61676520040000FFD8FFE000104A46494600010100000100
          010000FFDB0043000302020302020303030304030304050805050404050A0707
          06080C0A0C0C0B0A0B0B0D0E12100D0E110E0B0B1016101113141515150C0F17
          1816141812141514FFDB00430103040405040509050509140D0B0D1414141414
          1414141414141414141414141414141414141414141414141414141414141414
          14141414141414141414141414FFC00011080057001A03012200021101031101
          FFC4001A000101000301010000000000000000000008090006070305FFC40045
          10000004040106060D0B05000000000000010203050004071106081213212831
          1415183276B41622414552546171759196B5D31726274651555684A5C1D45781
          839395FFC40014010100000000000000000000000000000000FFC40014110100
          000000000000000000000000000000FFDA000C03010002110311003F00DAF292
          C758D589BEA94C36E2E7E6FE0693A9E5B8239AE968330151266669833736C16B
          6EB05A0DBCA7F127F589D3DA753E2C29F2A765F98F57A6337BDCF07BFF008961
          833F16F9203D306E50B8C711545C24DD27551F1C4179C5B4D2C8622595CF2049
          CC1BB628283700301475EAB80776D15829F2CBCCE02C34ACCACA4C4CA8D92C75
          5654C2639CC291444C611D62223711118951839B3495629FA76E739CC07E9B38
          3FB455CC129E870630A7E037CB97D499600D7953B2FD0FD5E5F37BC4F07BFE5D
          618167662D5E26FBECFCFF00C18A21953B2ECEF57A6337EAABC297FCA2C303EE
          2EF2406A34A1DA4B10574A6D292D2EE499F8CE6CC2338D7332A41006B9EDC655
          329447C8037DFAB50C550C3C9E8581B13F025922FA88113D69935E92BED2825B
          9CF3381FA3B80FED1452513D0CA229F804297D4101CD32A71D98EAF743DE3A92
          D032BC32F2A71D992AF7441E3A92D006F93167F1CC43ED2B8FC780E934A87687
          A45E9B9DF72B9450389BD43F07C861ECA4692CCCACC3AAAA19DE75312CF3BCDC
          D92C2CCE23A88B2A6280EA0D6017DE17D6314803704072BCA9C7665ABDD1078E
          A4B40CEE10CACA9C7666ABDD1078EA4B401FB3378FC05887FDEDDFCB80E934AC
          43943522F4D4EFB99C6282977044DDA1F8827DD7290A4A8CD61974664CAEF3A7
          0989E5650C430F13B80668022BA86BEB11D616D43AF75E91179A1E680E4F9538
          ECD157BA22F1D49681A6743072A69F4B93755E4EFDB7626F05FEFC0D681BF092
          7DB01B152B1DA1691FA6A73DCCE3141C9CC2F9A27852E9B4C9940D25308EA07A
          9CBFFC6718A1C8980E8A660DC25010F540711AFB80F15E35A5951D9195AC6727
          9DD99CA4A492E10927A55154142265B98C005B89802E61000BEBB414391163CF
          B8DFBDB15BF97191901F7E99648F5030555FC0B88B885CF81B6CF4CA936ACE62
          3E1644933B7CDA40604D4993DC748AA65B94B70CE1EE5E1F0DA9A89374A9162E
          6AA5488072DEF63582E1191901FFD9}
        OnMouseDown = Zoom_Slider_ImageMouseDown
        OnMouseMove = Zoom_Slider_ImageMouseMove
        OnMouseUp = Zoom_Slider_ImageMouseUp
      end
      object Displayed_Value_Selection_ComboBox_1: TComboBox
        Left = 3
        Top = 112
        Width = 178
        Height = 21
        Style = csDropDownList
        PopupMenu = Values_List_PopupMenu
        TabOrder = 0
        OnChange = Displayed_Value_Selection_ComboBox_1Change
      end
      object Geometry_Selection_ComboBox: TComboBox
        Left = 5
        Top = 21
        Width = 178
        Height = 21
        Style = csDropDownList
        PopupMenu = Geometry_PopupMenu
        TabOrder = 1
        OnChange = Geometry_Selection_ComboBoxChange
      end
      object Data_Point_Set_Selection_ComboBox: TComboBox
        Left = 3
        Top = 67
        Width = 178
        Height = 21
        Style = csDropDownList
        PopupMenu = DataPointsSet_PopupMenu
        TabOrder = 2
        OnChange = Data_Point_Set_Selection_ComboBoxChange
      end
      object Data_Points_List_StringGrid: TStringGrid
        Left = 1074
        Top = 32
        Width = 308
        Height = 140
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
        PopupMenu = Data_Poins_List_PopupMenu
        TabOrder = 3
        OnClick = Data_Points_List_StringGridClick
        OnDblClick = Data_Points_List_StringGridDblClick
      end
      object Display_Geo_CheckBox: TCheckBox
        Left = 189
        Top = 23
        Width = 82
        Height = 17
        Caption = 'Display map'
        TabOrder = 4
        OnClick = Display_Geo_CheckBoxClick
      end
      object Deleted_Data_Points_List_StringGrid: TStringGrid
        Left = 3
        Top = 313
        Width = 308
        Height = 140
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
        PopupMenu = Deleted_Data_Poins_List_PopupMenu
        TabOrder = 5
        OnClick = Data_Points_List_StringGridClick
      end
    end
  end
  object Data_FileListBox: TFileListBox
    Left = 376
    Top = 280
    Width = 73
    Height = 65
    ItemHeight = 13
    TabOrder = 4
    Visible = False
  end
  object Right_PageControl: TPageControl
    Left = 656
    Top = 43
    Width = 400
    Height = 342
    ActivePage = TabSheet2
    TabOrder = 3
    object TabSheet2: TTabSheet
      Caption = 'Segmentation'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label3: TLabel
        Left = 2
        Top = 29
        Width = 86
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Segment to paint:'
      end
      object Label1: TLabel
        Left = 2
        Top = 212
        Width = 221
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Remove surface marked with current segment'
      end
      object Paint_Segments_CheckBox: TCheckBox
        Left = 3
        Top = 1
        Width = 257
        Height = 17
        Caption = 'Segment painting mode (right mouse button)'
        TabOrder = 0
        OnClick = Paint_Segments_CheckBoxClick
      end
      object Segmentation_Brush_Size_RadioGroup: TRadioGroup
        Left = 3
        Top = 67
        Width = 78
        Height = 105
        Caption = 'Brush size'
        ItemIndex = 3
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5')
        TabOrder = 1
      end
      object Clear_Segments_Button: TButton
        Left = 86
        Top = 50
        Width = 81
        Height = 25
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Clear current'
        TabOrder = 2
        OnClick = Clear_Segments_ButtonClick
      end
      object ClearAll_segs_Button: TButton
        Left = 171
        Top = 50
        Width = 54
        Height = 25
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Clear all'
        TabOrder = 3
        OnClick = ClearAll_segs_ButtonClick
      end
      object Segment_No_ComboBox: TComboBox
        Left = 93
        Top = 24
        Width = 145
        Height = 21
        Style = csDropDownList
        DropDownCount = 30
        TabOrder = 4
      end
      object Undo_Segmentation_Step_Button: TButton
        Left = 230
        Top = 50
        Width = 80
        Height = 27
        Caption = 'Undo'
        TabOrder = 5
        OnClick = Undo_Segmentation_Step_ButtonClick
      end
      object Grow_Current_Segment_Button: TButton
        Left = 86
        Top = 104
        Width = 131
        Height = 25
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Grow current segment'
        TabOrder = 6
        OnClick = Grow_Current_Segment_ButtonClick
      end
      object Block_Existing_Segments_CheckBox: TCheckBox
        Left = 86
        Top = 162
        Width = 155
        Height = 30
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Lock existing segments'
        TabOrder = 7
      end
      object Shrink_Current_Segment_Button: TButton
        Left = 86
        Top = 133
        Width = 131
        Height = 25
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Shrink current segment'
        TabOrder = 8
        OnClick = Shrink_Current_Segment_ButtonClick
      end
      object Remove_Surface_Segment_Button: TButton
        Left = 228
        Top = 206
        Width = 61
        Height = 28
        Caption = 'Remove'
        TabOrder = 9
        OnClick = Remove_Surface_Segment_ButtonClick
      end
    end
    object GroupedEGMs: TTabSheet
      Caption = 'Grouped id egms'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object All_EGMs_PaintBox: TPaintBox
        Left = 39
        Top = 32
        Width = 210
        Height = 217
        OnMouseDown = All_EGMs_PaintBoxMouseDown
        OnMouseMove = All_EGMs_PaintBoxMouseMove
        OnMouseUp = All_EGMs_PaintBoxMouseUp
        OnPaint = All_EGMs_PaintBoxPaint
      end
      object All_Signals_Display_Control_Button: TButton
        Left = 2
        Top = 2
        Width = 33
        Height = 35
        Caption = 'T'
        PopupMenu = All_Signal_Tracings_PopupMenu
        TabOrder = 0
        OnClick = All_Signals_Display_Control_ButtonClick
      end
    end
    object ECGChannels: TTabSheet
      Caption = 'ECG'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ECG_Channels_PaintBox: TPaintBox
        Left = 47
        Top = 40
        Width = 210
        Height = 217
        OnMouseDown = ECG_Channels_PaintBoxMouseDown
        OnMouseMove = ECG_Channels_PaintBoxMouseMove
        OnMouseUp = ECG_Channels_PaintBoxMouseUp
        OnPaint = ECG_Channels_PaintBoxPaint
      end
      object ECG_Signals_Display_Control_Button: TButton
        Left = 3
        Top = 3
        Width = 33
        Height = 35
        Caption = 'T'
        PopupMenu = ECG_Signal_Tracings_PopupMenu
        TabOrder = 0
        OnClick = ECG_Signals_Display_Control_ButtonClick
      end
    end
    object ExtraChannels: TTabSheet
      Caption = 'Extra channels'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Extra_Channels_PaintBox: TPaintBox
        Left = 55
        Top = 48
        Width = 210
        Height = 217
        OnMouseDown = Extra_Channels_PaintBoxMouseDown
        OnMouseMove = Extra_Channels_PaintBoxMouseMove
        OnMouseUp = Extra_Channels_PaintBoxMouseUp
        OnPaint = Extra_Channels_PaintBoxPaint
      end
      object Extra_Channels_Button: TButton
        Left = 3
        Top = 3
        Width = 33
        Height = 35
        Caption = 'T'
        PopupMenu = Extra_Signal_Tracings_PopupMenu
        TabOrder = 0
        OnClick = Extra_Channels_ButtonClick
      end
    end
    object ImagesPage: TTabSheet
      Caption = 'Histology'
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Fluoro_Image_Paint_Box: TPaintBox
        Left = 22
        Top = 170
        Width = 203
        Height = 143
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        PopupMenu = Fluoro_Image_2D_PopupMenu
        OnMouseDown = Fluoro_Image_Paint_BoxMouseDown
        OnMouseMove = Fluoro_Image_Paint_BoxMouseMove
        OnMouseUp = Fluoro_Image_Paint_BoxMouseUp
        OnPaint = Fluoro_Image_Paint_BoxPaint
      end
      object Images_StringGrid: TStringGrid
        Left = 3
        Top = 55
        Width = 302
        Height = 110
        ColCount = 6
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
        PopupMenu = Images_List_PopupMenu
        TabOrder = 0
        OnClick = Images_StringGridClick
        OnDblClick = Images_StringGridDblClick
        OnMouseDown = Images_StringGridMouseDown
        ColWidths = (
          64
          64
          64
          64
          21
          64)
      end
      object Display_Images_In_3D_Panel_CheckBox: TCheckBox
        Left = 0
        Top = -1
        Width = 97
        Height = 17
        Caption = 'Display images'
        Checked = True
        State = cbChecked
        TabOrder = 1
        OnClick = Display_Images_In_3D_Panel_CheckBoxClick
      end
      object Limit_Image_Display_Bottom_CheckBox: TCheckBox
        Left = 95
        Top = -1
        Width = 131
        Height = 17
        Caption = 'Limit display (bottom)'
        TabOrder = 2
        OnClick = Limit_Image_Display_Bottom_CheckBoxClick
      end
      object Limit_Image_Display_Top_CheckBox: TCheckBox
        Left = 95
        Top = 15
        Width = 133
        Height = 17
        Caption = 'Limit display (top)'
        TabOrder = 3
        OnClick = Limit_Image_Display_Top_CheckBoxClick
      end
      object Remove_blue_background_CheckBox: TCheckBox
        Left = 217
        Top = -1
        Width = 160
        Height = 17
        Caption = 'Remove blue background'
        TabOrder = 4
        OnClick = Remove_blue_background_CheckBoxClick
      end
      object Remove_green_background_CheckBox: TCheckBox
        Left = 217
        Top = 15
        Width = 160
        Height = 17
        Caption = 'Remove green background'
        TabOrder = 5
        OnClick = Remove_green_background_CheckBoxClick
      end
      object Display_Surface_CheckBox: TCheckBox
        Left = 0
        Top = 15
        Width = 94
        Height = 17
        Caption = 'Display surface'
        Checked = True
        State = cbChecked
        TabOrder = 6
        OnClick = Display_Surface_CheckBoxClick
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Tools'
      ImageIndex = 5
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label4: TLabel
        Left = 3
        Top = 28
        Width = 52
        Height = 13
        Caption = 'Set sample'
      end
      object Label5: TLabel
        Left = 3
        Top = 61
        Width = 30
        Height = 13
        Caption = 'Speed'
      end
      object Phase_Movie_Mode_CheckBox: TCheckBox
        Left = 8
        Top = 5
        Width = 140
        Height = 17
        Caption = 'Phase movie mode'
        TabOrder = 0
        OnClick = Phase_Movie_Mode_CheckBoxClick
      end
      object Set_Sample_Button: TButton
        Left = 118
        Top = 23
        Width = 63
        Height = 25
        Caption = 'Set'
        TabOrder = 1
        OnClick = Set_Sample_ButtonClick
      end
      object SampleValue_Edit: TEdit
        Left = 59
        Top = 25
        Width = 55
        Height = 21
        TabOrder = 2
        Text = '100'
      end
      object Movie_Speed_Edit: TEdit
        Left = 39
        Top = 58
        Width = 55
        Height = 21
        TabOrder = 3
        Text = '10'
      end
      object Movie_Start_Button: TButton
        Left = 100
        Top = 55
        Width = 63
        Height = 25
        Caption = 'Start'
        TabOrder = 4
        OnClick = Movie_Start_ButtonClick
      end
      object Movie_Stop_Button: TButton
        Left = 164
        Top = 55
        Width = 63
        Height = 25
        Caption = 'Stop'
        TabOrder = 5
        OnClick = Movie_Stop_ButtonClick
      end
      object DirectoryListBox2: TDirectoryListBox
        Left = 279
        Top = 3
        Width = 110
        Height = 59
        TabOrder = 6
        Visible = False
      end
    end
  end
  object Annotation_Popup_Button: TButton
    Left = 399
    Top = 437
    Width = 33
    Height = 35
    Caption = 'T'
    TabOrder = 5
    OnClick = Annotation_Popup_ButtonClick
  end
  object Signals_ScrollBar: TScrollBar
    Left = 363
    Top = 600
    Width = 406
    Height = 30
    Max = 300
    PageSize = 0
    TabOrder = 6
    OnChange = Signals_ScrollBarChange
  end
  object MainMenuResearch: TMainMenu
    Left = 448
    Top = 40
    object File1: TMenuItem
      Caption = 'File'
      object ImportAbbottNavXdata1: TMenuItem
        Caption = 'Import NavX data'
        object ImportDxLmap1: TMenuItem
          Caption = 'Import DxL map'
          OnClick = ImportDxLmap1Click
        end
        object ImportSegment1: TMenuItem
          Caption = 'Import '#39'Segment'#39
        end
      end
      object ImportCARTOdata1: TMenuItem
        Caption = 'Import CARTO data'
        object ImportCARTOexportfolder1: TMenuItem
          Caption = 'Import single map from CARTO export folder'
          OnClick = ImportCARTOexportfolder1Click
        end
        object ImportALLmapsfromCARTOexportfolder1: TMenuItem
          Caption = 'Import ALL maps from CARTO export folder'
          OnClick = ImportALLmapsfromCARTOexportfolder1Click
        end
      end
      object ImportRhythmiadata1: TMenuItem
        Caption = 'Import Rhythmia data'
        object Importmatlabadvancedfile1: TMenuItem
          Caption = 'Import matlab advanced file'
          OnClick = Importmatlabadvancedfile1Click
        end
        object ImportablationpointslistLydiacsvformatv21: TMenuItem
          Caption = 'Import ablation points list (custom csv format)'
          OnClick = ImportablationpointslistLydiacsvformatv21Click
        end
      end
      object Otherimports1: TMenuItem
        Caption = 'Other imports'
        object Importvtkmeshfile1: TMenuItem
          Caption = 'Import vtk mesh file (text format)'
          OnClick = Importvtkmeshfile1Click
        end
        object Importimagesofslicedhearttissue1: TMenuItem
          Caption = 'Import slices images for registration'
          OnClick = Importimagesofslicedhearttissue1Click
        end
      end
      object N30: TMenuItem
        Caption = '-'
      end
      object Export1: TMenuItem
        Caption = 'Exports'
        object ExportcurrentmapasSTLfileegfor3Dprinting1: TMenuItem
          Caption = 'Export current map as STL file (e.g. for 3D printing)'
          OnClick = ExportcurrentmapasSTLfileegfor3Dprinting1Click
        end
        object Exportvaluesatdatapoints1: TMenuItem
          Caption = 'Export values at data points of current map'
          OnClick = Exportvaluesatdatapoints1Click
        end
        object Exportvaluesatgeometrynodes1: TMenuItem
          Caption = 'Export values at geometry nodes of current map'
          OnClick = Exportvaluesatgeometrynodes1Click
        end
        object Exportgeometryasvtkfile1: TMenuItem
          Caption = 'Export geometry as vtk file'
          OnClick = Exportgeometryasvtkfile1Click
        end
        object Exportcurrentdatapointegms1: TMenuItem
          Caption = 'Export current data point egms'
          OnClick = Exportcurrentdatapointegms1Click
        end
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Loadstudy1: TMenuItem
        Caption = 'Load workspace'
        ShortCut = 16463
        OnClick = Loadstudy1Click
      end
      object Saveworkspace1: TMenuItem
        Caption = 'Save workspace'
        ShortCut = 16467
        OnClick = Saveworkspace1Click
      end
      object N13: TMenuItem
        Caption = '-'
      end
      object Closeworkspace1: TMenuItem
        Caption = 'Close workspace'
        ShortCut = 16471
        OnClick = Closeworkspace1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
    object Opitons1: TMenuItem
      Caption = 'Options'
      object Displayoptions1: TMenuItem
        Caption = 'Display options'
        OnClick = Displayoptions1Click
      end
      object N7: TMenuItem
        Caption = '-'
      end
      object Annotationsignalprocessinparameters1: TMenuItem
        Caption = 'Annotation/signal processing parameters'
        OnClick = Annotationsignalprocessinparameters1Click
      end
      object N8: TMenuItem
        Caption = '-'
      end
      object Applicationsettings1: TMenuItem
        Caption = 'Application settings'
        OnClick = Applicationsettings1Click
      end
      object Showdatasetinfo1: TMenuItem
        Caption = 'Show dataset info'
        OnClick = Showdatasetinfo1Click
      end
    end
    object SignalProcessing1: TMenuItem
      Caption = 'Signal Processing '
      object Sh1: TMenuItem
        Caption = 'Open Singal Processing window'
        OnClick = Sh1Click
      end
      object N10: TMenuItem
        Caption = '-'
      end
      object CalculateWavefrontPropagationVelocityWPVmap1: TMenuItem
        Caption = 'Calculate Wavefront Propagation Velocity (WPV) map'
        OnClick = CalculateWavefrontPropagationVelocityWPVmap1Click
      end
      object CalculateEGMdurationmap1: TMenuItem
        Caption = 'Calculate EGM duration map'
        object BasedonslidingwindowSD1: TMenuItem
          Caption = 'Based on sliding window SD'
          OnClick = BasedonslidingwindowSD1Click
        end
        object BasedonslidingwindowIntegral1: TMenuItem
          Caption = 'Based on sliding window Integral'
          OnClick = BasedonslidingwindowIntegral1Click
        end
        object Combined1: TMenuItem
          Caption = 'Combined'
          OnClick = Combined1Click
        end
      end
      object CalculateAFCLmapfrom180msSinusoidalRecomposition1: TMenuItem
        Caption = 
          'Calculate AFCL map using Sinusoidal Recomposition + Hilbert tran' +
          'sform'
        OnClick = CalculateAFCLmapfrom180msSinusoidalRecomposition1Click
      end
      object N15: TMenuItem
        Caption = '-'
      end
      object UpdateReferenceBarpositionwithrespecttocurrentlyselectedVwave1: TMenuItem
        Caption = 'Update ref bar position using current template'
        OnClick = UpdateReferenceBarpositionwithrespecttocurrentlyselectedVwave1Click
      end
      object N33: TMenuItem
        Caption = '-'
      end
    end
    object Geometry1: TMenuItem
      Caption = 'Geometry'
      object Centeringtools1: TMenuItem
        Caption = 'Centering tools'
        object Centergeometry1: TMenuItem
          Caption = 'Center currently selected geometry'
          OnClick = Centergeometry1Click
        end
        object Centerallgeometries1: TMenuItem
          Caption = 'Center all geometries individually'
          OnClick = Centerallgeometries1Click
        end
        object CenterALLgeometriessimultaneously1: TMenuItem
          Caption = 'Center ALL geometries simultaneously'
          OnClick = CenterALLgeometriessimultaneously1Click
        end
        object CenterALLEXCPEPTFIRSTgeometriessimultaneously1: TMenuItem
          Caption = 'Center ALL EXCPEPT FIRST geometries simultaneously'
          OnClick = CenterALLEXCPEPTFIRSTgeometriessimultaneously1Click
        end
        object N38: TMenuItem
          Caption = '-'
        end
        object Undocentering1: TMenuItem
          Caption = 'Restore original spatial positions of all geometries'
          OnClick = Undocentering1Click
        end
      end
      object ranslatecurrentgeometry1: TMenuItem
        Caption = 'Translate current geometry'
        OnClick = ranslatecurrentgeometry1Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object ools1: TMenuItem
        Caption = 'Tools'
        object Importgeometrysegmentation1: TMenuItem
          Caption = 'Import geometry segmentation'
          OnClick = Importgeometrysegmentation1Click
        end
        object Exportgeometrysegmentation1: TMenuItem
          Caption = 'Export geometry segmentation'
          OnClick = Exportgeometrysegmentation1Click
        end
        object N36: TMenuItem
          Caption = '-'
        end
        object Downsamplemesh1: TMenuItem
          Caption = 'Downsample mesh'
          OnClick = Downsamplemesh1Click
        end
        object Upsamplemesh1: TMenuItem
          Caption = 'Upsample mesh'
          OnClick = Upsamplemesh1Click
        end
        object Addwheelgeometry1: TMenuItem
          Caption = 'Add wheel geometry'
          OnClick = Addwheelgeometry1Click
        end
        object Addsquaresample1: TMenuItem
          Caption = 'Add square sample'
          OnClick = Addsquaresample1Click
        end
        object N42: TMenuItem
          Caption = '-'
        end
        object Reclaculatenormals1: TMenuItem
          Caption = 'Reclaculate normals'
          OnClick = Reclaculatenormals1Click
        end
        object Smoothmapgeometry1stepofaveragereplacement1: TMenuItem
          Caption = 'Smooth map geometry (1 step of average replacement)'
          OnClick = Smoothmapgeometry1stepofaveragereplacement1Click
        end
        object Smoothmapgeometrymultiplestepswithvolumecorrection1: TMenuItem
          Caption = 'Smooth map geometry (multiple steps with volume correction)'
          OnClick = Smoothmapgeometrymultiplestepswithvolumecorrection1Click
        end
        object Smoothnormalswithinspecifiedrange1: TMenuItem
          Caption = 'Smooth normals (average normal within specified range)'
          OnClick = Smoothnormalswithinspecifiedrange1Click
        end
        object Restoreoriginalgeometry1: TMenuItem
          Caption = 'Restore original geometry'
          OnClick = Restoreoriginalgeometry1Click
        end
        object N43: TMenuItem
          Caption = '-'
        end
        object Restoreremovedsurface1: TMenuItem
          Caption = 'Restore removed surface'
          OnClick = Restoreremovedsurface1Click
        end
        object N57: TMenuItem
          Caption = '-'
        end
        object N58: TMenuItem
          Caption = 'Show current segment area'
          OnClick = N58Click
        end
        object Getsegmentwithmaxvalue1: TMenuItem
          Caption = 'Get segment with max value'
          OnClick = Getsegmentwithmaxvalue1Click
        end
        object N53: TMenuItem
          Caption = '-'
        end
        object N61: TMenuItem
          Caption = 'Segment disjointed geometry elements'
          OnClick = N61Click
        end
      end
      object N24: TMenuItem
        Caption = '-'
      end
      object Spatialregistration1: TMenuItem
        Caption = 'Spatial registration'
        object Registercurrentgeometrywithsetofhistologyslices1: TMenuItem
          Caption = 'Register current geometry with set of histology slices'
          OnClick = Registercurrentgeometrywithsetofhistologyslices1Click
        end
        object RegisterABCABCgeometries1: TMenuItem
          Caption = '-'
        end
        object ABCtoABCfitting1: TMenuItem
          Caption = 'ABC to ABC fitting'
          OnClick = ABCtoABCfitting1Click
        end
        object ABCtoABCfitting2: TMenuItem
          Caption = 'Refinement'
        end
      end
      object N40: TMenuItem
        Caption = '-'
      end
      object Geometrystatistics1: TMenuItem
        Caption = 'Show geometry statistics'
        OnClick = Geometrystatistics1Click
      end
    end
    object Datapoints2: TMenuItem
      Caption = 'Data points'
      object Deletepoints1: TMenuItem
        Caption = 'Remove data points'
        object Deletepointswithoutxyzcoordinates1: TMenuItem
          Caption = 'Remove not used or utilized points'
          OnClick = Deletepointswithoutxyzcoordinates1Click
        end
        object Deletedatapointswithcurrentvaluebelowgiventhreshold1: TMenuItem
          Caption = 'Remove data points with current value BELOW given threshold'
          OnClick = Deletedatapointswithcurrentvaluebelowgiventhreshold1Click
        end
        object DeletedatapointswithcurrentvalueABOVEgiventhreshold1: TMenuItem
          Caption = 'Remove data points with current value ABOVE given threshold'
          OnClick = DeletedatapointswithcurrentvalueABOVEgiventhreshold1Click
        end
        object Removedatapointsabovetop1: TMenuItem
          Caption = 'Remove data points above % threshold'
        end
        object Removedatapointsbelowtop1: TMenuItem
          Caption = 'Remove data points below % threshold '
          OnClick = Removedatapointsbelowtop1Click
        end
        object Deletepointswithspecificvalue1: TMenuItem
          Caption = 'Remove points with specific value'
          OnClick = Deletepointswithspecificvalue1Click
        end
        object RemovedatapointsnotonmapsurfaceHARDdelete1: TMenuItem
          Caption = 'Remove data points not on map surface (irreversible delete)'
          OnClick = RemovedatapointsnotonmapsurfaceHARDdelete1Click
        end
        object N22: TMenuItem
          Caption = '-'
        end
        object Undeletealldatapoints1: TMenuItem
          Caption = 'Restore all removed data points'
          OnClick = Undeletealldatapoints1Click
        end
      end
      object N9: TMenuItem
        Caption = '-'
      end
      object Forcedatapointsontosurface1: TMenuItem
        Caption = 'Force data points onto surface'
        OnClick = Forcedatapointsontosurface1Click
      end
      object Restoreoriginallocationsofdatapoints1: TMenuItem
        Caption = 'Restore original locations of data points'
        OnClick = Restoreoriginallocationsofdatapoints1Click
      end
      object N20: TMenuItem
        Caption = '-'
      end
      object Smoothingfunctions1: TMenuItem
        Caption = 'Smoothing functions'
        object Averagefilter1: TMenuItem
          Caption = 'Average spatial filter'
          OnClick = Averagefilter1Click
        end
      end
    end
    object DataPoints1: TMenuItem
      Caption = 'Quantitative analysis'
      object Getstatsofcurrentmapvalue1: TMenuItem
        Caption = 'Get stats of values on entire surface'
        OnClick = Getstatsofcurrentmapvalue1Click
      end
      object GetstatsofcurrentmapvaluePERSEGMENT1: TMenuItem
        Caption = 'Get stats of values on map surface [PER SEGMENT]'
        OnClick = GetstatsofcurrentmapvaluePERSEGMENT1Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object Getareaandareaofvalueaboveandbeyondspecifiedthreshold1: TMenuItem
        Caption = 
          'Get area and %area of current value above and below specified th' +
          'reshold'
        OnClick = Getareaandareaofvalueaboveandbeyondspecifiedthreshold1Click
      end
      object GetareaandareaofcurrentvalueaboveandbeyondspecifiedthresholdPERSEGMENT1: TMenuItem
        Caption = 
          'Get area and %area of current value above and below specified th' +
          'reshold [PER SEGMENT]'
        OnClick = GetareaandareaofcurrentvalueaboveandbeyondspecifiedthresholdPERSEGMENT1Click
      end
      object GetareaandareaofcurrentvaluewithinrangePERSEGMENT1: TMenuItem
        Caption = 'Get area and %area of current value within range [PER SEGMENT]'
        OnClick = GetareaandareaofcurrentvaluewithinrangePERSEGMENT1Click
      end
      object N14: TMenuItem
        Caption = '-'
      end
      object Histograms1: TMenuItem
        Caption = 'Histograms'
        object Showhistogramofcurrentvaluespatialgradient1: TMenuItem
          Caption = 'Show histogram of spatial gradient of current value '
          OnClick = Showhistogramofcurrentvaluespatialgradient1Click
        end
        object ShowhistogramofspatialgradientofcurrentvalueOFCURRENTSEGMENT1: TMenuItem
          Caption = 
            'Show histogram of spatial gradient of current value [OF CURRENT ' +
            'SEGMENT]'
          OnClick = ShowhistogramofspatialgradientofcurrentvalueOFCURRENTSEGMENT1Click
        end
      end
    end
    object PRESETS1: TMenuItem
      Caption = 'PRESETS'
      object Leaveonly1stmapvisible1: TMenuItem
        Caption = 'Leave only 1st map visible'
        OnClick = Leaveonly1stmapvisible1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object About1: TMenuItem
        Caption = 'About'
        OnClick = About1Click
      end
      object Help2: TMenuItem
        Caption = 'Help'
        OnClick = Help2Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Showlogwindow1: TMenuItem
        Caption = 'Show log window'
        OnClick = Showlogwindow1Click
      end
    end
  end
  object OpenGL_Panel_PopupMenu: TPopupMenu
    Left = 399
    Top = 147
    object Leftmousebuttonaction1: TMenuItem
      Caption = 'Left mouse button action'
      object Select1: TMenuItem
        Caption = 'Select'
        OnClick = Select1Click
      end
      object Rotate1: TMenuItem
        Tag = 1
        Caption = 'Rotate'
        Checked = True
        OnClick = Rotate1Click
      end
      object Zoom1: TMenuItem
        Tag = 2
        Caption = 'Zoom'
        OnClick = Zoom1Click
      end
      object Pan1: TMenuItem
        Tag = 3
        Caption = 'Pan'
        OnClick = Pan1Click
      end
    end
    object N26: TMenuItem
      Caption = '-'
    end
    object Selecteddatapoint1: TMenuItem
      Caption = 'Selected data point'
      object Changedatapointtype2: TMenuItem
        Caption = 'Change data point type'
        OnClick = Changedatapointtype2Click
      end
      object Deletedatapoint2: TMenuItem
        Caption = 'Delete current data point'
        OnClick = Deletedatapoint2Click
      end
    end
    object N11: TMenuItem
      Caption = '-'
    end
    object Pathanalsis1: TMenuItem
      Caption = 'Path analysis'
      object Markpathbeginnng1: TMenuItem
        Caption = 'Mark path beginning'
        OnClick = Markpathbeginnng1Click
      end
      object Markintermediarypoint1: TMenuItem
        Caption = 'Mark intermediary point'
        OnClick = Markintermediarypoint1Click
      end
      object Markpathend1: TMenuItem
        Caption = 'Mark path end'
        OnClick = Markpathend1Click
      end
      object N12: TMenuItem
        Caption = '-'
      end
      object Displayvaluesalongthepath1: TMenuItem
        Caption = 'Display values along the path'
        OnClick = Displayvaluesalongthepath1Click
      end
      object N21: TMenuItem
        Caption = '-'
      end
      object Deletepath1: TMenuItem
        Caption = 'Delete path'
        OnClick = Deletepath1Click
      end
    end
    object N23: TMenuItem
      Caption = '-'
    end
    object Setregistrationpoint1: TMenuItem
      Caption = 'Set registration point'
      object A1: TMenuItem
        Caption = 'A'
        OnClick = A1Click
      end
      object B1: TMenuItem
        Caption = 'B'
        OnClick = B1Click
      end
      object C1: TMenuItem
        Caption = 'C'
        OnClick = C1Click
      end
    end
    object LVsegmentation1: TMenuItem
      Caption = 'LV "17" segmentation'
      object Addgeometrylandmarkpoint1: TMenuItem
        Caption = '1. Add geometry landmark point'
        object Apes1: TMenuItem
          Caption = 'a. Apex'
          OnClick = Apes1Click
        end
        object SeptalAnteriorjunctionbase1: TMenuItem
          Caption = 'b. Septal-Anterior junction (base)'
          OnClick = SeptalAnteriorjunctionbase1Click
        end
        object SeptalLateraljunctionbase1: TMenuItem
          Caption = 'c. Septal-Lateral junction (base)'
          OnClick = SeptalLateraljunctionbase1Click
        end
        object AnteriorLateraljunctionbase1: TMenuItem
          Caption = 'd. Anterior-Lateral junction (base)'
          OnClick = AnteriorLateraljunctionbase1Click
        end
      end
      object Addintermediatemarkers1: TMenuItem
        Caption = '2. [optional] Add intermediate markers'
        object ApextoSeptalAnteriorintermediatepoint1: TMenuItem
          Caption = 'Apex to Septal-Anterior intermediate point'
          OnClick = ApextoSeptalAnteriorintermediatepoint1Click
        end
        object ApextoSeptalLateralintermediatepoint1: TMenuItem
          Caption = 'Apex to Septal-Lateral intermediate point'
          OnClick = ApextoSeptalLateralintermediatepoint1Click
        end
        object ApextoAnteriorLateralintermediatepoint1: TMenuItem
          Caption = 'Apex to Anterior-Lateral intermediate point'
          OnClick = ApextoAnteriorLateralintermediatepoint1Click
        end
        object N27: TMenuItem
          Caption = '-'
        end
        object Anteriorbaselineintermediate1: TMenuItem
          Caption = 'Base: Anterior line intermediate'
          OnClick = Anteriorbaselineintermediate1Click
        end
        object Septalbaselineintermediate1: TMenuItem
          Caption = 'Baes: Septal line intermediate'
          OnClick = Septalbaselineintermediate1Click
        end
        object Lateralbaselineintermediate1: TMenuItem
          Caption = 'Base: Lateral base line intermediate'
          OnClick = Lateralbaselineintermediate1Click
        end
      end
      object Markregions1: TMenuItem
        Caption = '3. Mark regions'
        object Anterior1: TMenuItem
          Caption = 'a. Anterior'
          OnClick = Anterior1Click
        end
        object Lateral1: TMenuItem
          Caption = 'b. Lateral'
          OnClick = Lateral1Click
        end
        object Septal1: TMenuItem
          Caption = 'c. Septal'
          OnClick = Septal1Click
        end
      end
      object N28: TMenuItem
        Caption = '-'
      end
      object Perform17nuclearsegmentation1: TMenuItem
        Caption = '4. Perform "17"/nuclear segmentation'
        OnClick = Perform17nuclearsegmentation1Click
      end
    end
    object N25: TMenuItem
      Caption = '-'
    end
    object Addlabel1: TMenuItem
      Caption = 'Place label'
      OnClick = Addlabel1Click
    end
    object Deletelabel1: TMenuItem
      Caption = 'Delete label'
      OnClick = Deletelabel1Click
    end
    object N31: TMenuItem
      Caption = '-'
    end
    object Varioustools1: TMenuItem
      Caption = 'Various tools'
      object Createtentingpoint1: TMenuItem
        Caption = 'Create conic "tenting point"'
        OnClick = Createtentingpoint1Click
      end
      object Createconichole1: TMenuItem
        Caption = 'Create conic "hole"'
        OnClick = Createconichole1Click
      end
      object Create1: TMenuItem
        Caption = 'Create uniform depression'
        OnClick = Create1Click
      end
      object Createuniformelevation1: TMenuItem
        Caption = 'Create uniform elevation'
        OnClick = Createuniformelevation1Click
      end
      object N62: TMenuItem
        Caption = '-'
      end
      object Leavegeometryrelatedwithonlycurrentlypointedsegment1: TMenuItem
        Caption = 'Leave geometry related with only currently pointed segment'
        OnClick = Leavegeometryrelatedwithonlycurrentlypointedsegment1Click
      end
    end
    object Displayrovingsignal1: TMenuItem
      Caption = 'Display local egm'
      OnClick = Displayrovingsignal1Click
    end
  end
  object OpenDialog: TOpenDialog
    Filter = 
      'all files|*.*|vtk geometry file (*.vtk)|*.vtk|intermediate file ' +
      'from rhythmia export (*EPLabWorks.txt)|*EPLabWorks.txt|eplab wor' +
      'ks workspace (*.eplab)|*.eplab|stl geometry file (*.stl)|*.stl|b' +
      'itmap file (*.bmp)|*.bmp'
    Left = 360
    Top = 376
  end
  object SaveDialog: TSaveDialog
    Filter = 
      'all files|*.*|vtk geometry file (*.vtk)|*.vtk|intermediate file ' +
      'from rhythmia export (*.intermediate_file_for_EPLabWorks.txt)|*.' +
      'intermediate_file_for_EPLabWorks.txt|eplab works workspace (*.ep' +
      'lab)|*.eplab|stl geometry file (*.stl)|*.stl'
    Left = 440
    Top = 376
  end
  object All_Signal_Tracings_PopupMenu: TPopupMenu
    Left = 616
    Top = 152
    object AllSigs_Increase_Scale: TMenuItem
      Caption = 'Increase time scale by factor 2'
      OnClick = AllSigs_Increase_ScaleClic
    end
    object AllSigsDecreaseScale: TMenuItem
      Caption = 'Decrease time scale by factor 2'
      OnClick = AllSigsDecreaseScaleClick
    end
  end
  object Data_Poins_List_PopupMenu: TPopupMenu
    Left = 192
    Top = 224
    object DisplaylocalsignalinSignalAnalysiswindow1: TMenuItem
      Caption = 'Display local egm'
      OnClick = DisplaylocalsignalinSignalAnalysiswindow1Click
    end
    object DisplayrefegminSignalAnalysiswindow1: TMenuItem
      Caption = 'Display ref egm'
      OnClick = DisplayrefegminSignalAnalysiswindow1Click
    end
    object DisplayECGegminSignalAnalysiswindow1: TMenuItem
      Caption = 'Display ECG egm '
      OnClick = DisplayECGegminSignalAnalysiswindow1Click
    end
    object N16: TMenuItem
      Caption = '-'
    end
    object Deletedatapoint1: TMenuItem
      Caption = 'Delete data point/points'
      OnClick = Deletedatapoint1Click
    end
    object N6: TMenuItem
      Caption = '-'
    end
    object Varia1: TMenuItem
      Caption = 'Varia'
      object Changedatapointtype1: TMenuItem
        Caption = 'Change data point type'
        OnClick = Changedatapointtype1Click
      end
      object N47: TMenuItem
        Caption = '-'
      end
      object Getdvdtslope1: TMenuItem
        Caption = 'Get dv/dt slope'
        OnClick = Getdvdtslope1Click
      end
      object Getmicrofractionation1: TMenuItem
        Caption = 'Get microfractionation'
        OnClick = Getmicrofractionation1Click
      end
    end
    object Sort1: TMenuItem
      Caption = 'Sort'
      object Sortvaluesascending1: TMenuItem
        Caption = 'Sort values in ascending order'
        OnClick = Sortvaluesascending1Click
      end
      object Sortvaluesdescending1: TMenuItem
        Caption = 'Sort values in descending order'
        OnClick = Sortvaluesdescending1Click
      end
    end
  end
  object Annotation_Window_PopupMenu: TPopupMenu
    Left = 496
    Top = 440
    object MenuItem1: TMenuItem
      Caption = 'Increase time scale by factor 2'
      OnClick = MenuItem1Click
    end
    object MenuItem2: TMenuItem
      Caption = 'Decrease time scale by factor 2'
      OnClick = MenuItem2Click
    end
    object N29: TMenuItem
      Caption = '-'
    end
    object Recalculateannotation1: TMenuItem
      Caption = 'Recalculate annotation'
      OnClick = Recalculateannotation1Click
    end
  end
  object DataPointsSet_PopupMenu: TPopupMenu
    Left = 208
    Top = 120
    object Changenameofdatapointset1: TMenuItem
      Caption = 'Change name of current data point set'
      OnClick = Changenameofdatapointset1Click
    end
    object N17: TMenuItem
      Caption = '-'
    end
    object Clonecurrentdatapointset1: TMenuItem
      Caption = 'Clone current data point set'
      OnClick = Clonecurrentdatapointset1Click
    end
    object N18: TMenuItem
      Caption = '-'
    end
    object Deletecurrentdatapointset1: TMenuItem
      Caption = 'Delete current data point set'
      OnClick = Deletecurrentdatapointset1Click
    end
  end
  object Geometry_PopupMenu: TPopupMenu
    Left = 328
    Top = 56
    object Changecurrentgeometryname1: TMenuItem
      Caption = 'Change current geometry name'
      OnClick = Changecurrentgeometryname1Click
    end
    object N19: TMenuItem
      Caption = '-'
    end
    object Clonecurrentsurface1: TMenuItem
      Caption = 'Clone current surface'
      OnClick = Clonecurrentsurface1Click
    end
    object N66: TMenuItem
      Caption = '-'
    end
    object N67: TMenuItem
      Caption = 'Project this geometry onto 1st one'
      OnClick = N67Click
    end
  end
  object ECG_Signal_Tracings_PopupMenu: TPopupMenu
    Left = 720
    Top = 200
    object MenuItem3: TMenuItem
      Caption = 'Increase time scale by factor 2'
      OnClick = MenuItem3Click
    end
    object MenuItem4: TMenuItem
      Caption = 'Decrease time scale by factor 2'
      OnClick = MenuItem4Click
    end
  end
  object Extra_Signal_Tracings_PopupMenu: TPopupMenu
    Left = 624
    Top = 240
    object MenuItem5: TMenuItem
      Caption = 'Increase time scale by factor 2'
      OnClick = MenuItem5Click
    end
    object MenuItem6: TMenuItem
      Caption = 'Decrease time scale by factor 2'
      OnClick = MenuItem6Click
    end
  end
  object OpenDialogMultiSelect: TOpenDialog
    Filter = 'bmp|*.bmp'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Left = 568
    Top = 384
  end
  object Fluoro_Image_2D_PopupMenu: TPopupMenu
    Left = 904
    Top = 304
    object Markfirstpointforimagescaling1: TMenuItem
      Caption = 'Mark first point for image scaling'
      OnClick = Markfirstpointforimagescaling1Click
    end
    object Marksecondpointforimagescaling1: TMenuItem
      Caption = 'Mark second point for image scaling'
      OnClick = Marksecondpointforimagescaling1Click
    end
    object Scaleimage1: TMenuItem
      Caption = 'Scale image'
      OnClick = Scaleimage1Click
    end
    object MenuItem7: TMenuItem
      Caption = '-'
    end
    object Markreferencepoint11: TMenuItem
      Caption = 'Set Registration Point A'
      OnClick = Markreferencepoint11Click
    end
    object Markreferencepoint21: TMenuItem
      Caption = 'Set Registration Point B'
      OnClick = Markreferencepoint21Click
    end
    object Markreferencepoint31: TMenuItem
      Caption = 'Set Registration Point C'
      OnClick = Markreferencepoint31Click
    end
    object ClearRegistrationPoints1: TMenuItem
      Caption = 'Clear Registration Points'
      OnClick = ClearRegistrationPoints1Click
    end
  end
  object Deleted_Data_Poins_List_PopupMenu: TPopupMenu
    Left = 160
    Top = 464
    object MenuItem8: TMenuItem
      Caption = 'Display roving egm in Signal Analysis window'
      OnClick = MenuItem8Click
    end
    object MenuItem9: TMenuItem
      Caption = 'Display ref egm in Signal Analysis window'
      OnClick = MenuItem9Click
    end
    object MenuItem10: TMenuItem
      Caption = 'Display ECG egm in Signal Analysis window'
      OnClick = MenuItem10Click
    end
    object MenuItem11: TMenuItem
      Caption = '-'
    end
    object MenuItem13: TMenuItem
      Caption = 'Undelete data point/points'
      OnClick = Undeletedatapoint1Click
    end
    object MenuItem14: TMenuItem
      Caption = '-'
    end
    object MenuItem15: TMenuItem
      Caption = 'Varia'
    end
  end
  object Images_List_PopupMenu: TPopupMenu
    Left = 936
    Top = 136
    object Setslicethickness1: TMenuItem
      Caption = 'Set slice thickness'
      OnClick = Setslicethickness1Click
    end
    object Setthicknessofallslices1: TMenuItem
      Caption = 'Set thickness of all slices'
      OnClick = Setthicknessofallslices1Click
    end
  end
  object Values_List_PopupMenu: TPopupMenu
    Left = 224
    Top = 168
    object MenuItem12: TMenuItem
      Caption = 'Change name of current value'
      OnClick = MenuItem12Click
    end
    object MenuItem16: TMenuItem
      Caption = '-'
    end
    object MenuItem17: TMenuItem
      Caption = 'Clone current value'
      OnClick = MenuItem17Click
    end
    object MenuItem18: TMenuItem
      Caption = '-'
    end
    object MenuItem19: TMenuItem
      Caption = 'Delete current value'
      OnClick = MenuItem19Click
    end
  end
end
