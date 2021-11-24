object NL_Analysis_Form: TNL_Analysis_Form
  Left = 348
  Top = 135
  Align = alClient
  Caption = 'NL analysis'
  ClientHeight = 670
  ClientWidth = 1267
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Stats_Label: TLabel
    Left = 17
    Top = 448
    Width = 27
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Stats:'
  end
  object Secondary_Grid_Label: TLabel
    Left = 129
    Top = 5
    Width = 74
    Height = 16
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'secondary'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Primary_Grid_Label: TLabel
    Left = 759
    Top = 0
    Width = 64
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Primary signal'
  end
  object Primary_Data_StringGrid: TStringGrid
    Left = 5
    Top = 26
    Width = 120
    Height = 287
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    DefaultColWidth = 90
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
    TabOrder = 0
    OnClick = Primary_Data_StringGridClick
    ColWidths = (
      91
      90
      90
      90
      90)
    RowHeights = (
      24
      24
      23
      24
      24)
  end
  object PageControl1: TPageControl
    Left = 179
    Top = 51
    Width = 1081
    Height = 587
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    ActivePage = Filtering_TabSheet
    TabOrder = 1
    OnChange = PageControl1Change
    object Filtering_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Pre-filtering'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label6: TLabel
        Left = 44
        Top = 198
        Width = 142
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Period of sinusoidal wave [ms]'
      end
      object Label13: TLabel
        Left = 115
        Top = 319
        Width = 71
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'C Window [ms]'
      end
      object Label14: TLabel
        Left = 109
        Top = 342
        Width = 78
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'LR Window [ms]'
      end
      object Label21: TLabel
        Left = 145
        Top = 72
        Width = 58
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Window[ms]'
      end
      object Label22: TLabel
        Left = 489
        Top = 46
        Width = 52
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Filter order '
      end
      object Label23: TLabel
        Left = 478
        Top = 68
        Width = 72
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Low cutoff [Hz]'
      end
      object Label24: TLabel
        Left = 471
        Top = 86
        Width = 66
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Up cutoff [Hz]'
      end
      object Label31: TLabel
        Left = 452
        Top = 139
        Width = 82
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Window size [ms]'
      end
      object Label60: TLabel
        Left = 15
        Top = 423
        Width = 96
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Range for derivative'
      end
      object Label12: TLabel
        Left = 438
        Top = 203
        Width = 82
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Window size [ms]'
      end
      object Butt_band_40_250_3th_order_checkbox: TCheckBox
        Left = 10
        Top = 21
        Width = 166
        Height = 11
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Butt_band_40_250_3rd_order'
        TabOrder = 0
        OnClick = r_ceckboxClick
      end
      object Rectification_checkbox: TCheckBox
        Left = 10
        Top = 36
        Width = 95
        Height = 11
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Rectification'
        TabOrder = 1
        OnClick = Rectification_checkboxClick
      end
      object Filter_All_Signals_Button: TButton
        Left = 272
        Top = 2
        Width = 117
        Height = 40
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Filter all signals'
        TabOrder = 2
        OnClick = Filter_All_Signals_ButtonClick
      end
      object Butt_low_20_3th_order_checkbox: TCheckBox
        Left = 10
        Top = 51
        Width = 166
        Height = 11
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Butt_low_20_3rd_order'
        TabOrder = 3
        OnClick = Butt_low_20_3th_order_checkboxClick
      end
      object Normalize__CheckBox: TCheckBox
        Left = 10
        Top = 139
        Width = 90
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Normalize'
        TabOrder = 4
        OnClick = Normalze_CheckBoxClick
      end
      object Hilb_Phase_CheckBox: TCheckBox
        Left = 12
        Top = 215
        Width = 166
        Height = 11
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Phase from Hilbert transform'
        TabOrder = 5
        OnClick = Hilb_Phase_CheckBoxClick
      end
      object Normalization_Window_Edit: TEdit
        Left = 207
        Top = 120
        Width = 52
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 6
        Text = '300'
      end
      object Set_av_SD_CheckBox: TCheckBox
        Left = 10
        Top = 119
        Width = 110
        Height = 16
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Set av=0,SD=1'
        TabOrder = 7
        OnClick = Normalze_CheckBoxClick
      end
      object FilterAllSigs_byDefault_CheckBox: TCheckBox
        Left = 272
        Top = 46
        Width = 202
        Height = 11
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Apply filtering to all signals by default'
        Checked = True
        State = cbChecked
        TabOrder = 8
      end
      object Sinusoidal_Recomposition_CheckBox: TCheckBox
        Left = 12
        Top = 183
        Width = 188
        Height = 11
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BiDiMode = bdLeftToRight
        Caption = 'Sinusoidal recomposition'
        ParentBiDiMode = False
        TabOrder = 9
        OnClick = Sinusoidal_Recomposition_CheckBoxClick
      end
      object Sinsoidal_Wavelet_Period_Edit: TEdit
        Left = 193
        Top = 196
        Width = 50
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 10
        Text = '180'
      end
      object LCR_RadioGroup: TRadioGroup
        Left = 11
        Top = 317
        Width = 89
        Height = 47
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'LCR eqution'
        ItemIndex = 0
        Items.Strings = (
          '2*C-L-R'
          'C/(L+R)')
        TabOrder = 11
      end
      object Variability_Type_RadioGroup: TRadioGroup
        Left = 610
        Top = 27
        Width = 105
        Height = 72
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Variability measure'
        ItemIndex = 0
        Items.Strings = (
          'SD'
          'p2p'
          'sum(abs(dv/dt))'
          'max(abs(dv/dt)')
        TabOrder = 12
      end
      object C_Window_Size_Edit: TEdit
        Left = 192
        Top = 317
        Width = 27
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 13
        Text = '30'
      end
      object LR_Window_Size_Edit: TEdit
        Left = 192
        Top = 340
        Width = 27
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 14
        Text = '30'
      end
      object Sliding_Window_SD_CheckBox: TCheckBox
        Left = 11
        Top = 69
        Width = 110
        Height = 22
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Sliding window SD'
        TabOrder = 15
        OnClick = Normalze_CheckBoxClick
      end
      object SD_Window_Edit: TEdit
        Left = 207
        Top = 70
        Width = 52
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 16
        Text = '30'
      end
      object Electrogram_Type_RadioGroup: TRadioGroup
        Left = 345
        Top = 61
        Width = 88
        Height = 45
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Electrogram type'
        ItemIndex = 0
        Items.Strings = (
          'Unipolar'
          'Bipolar')
        TabOrder = 17
      end
      object BW_Filter_Order_Edit: TEdit
        Left = 556
        Top = 40
        Width = 24
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 18
        Text = '3'
      end
      object BW_Filter_Up_Edit: TEdit
        Left = 556
        Top = 85
        Width = 24
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 19
        Text = '50'
      end
      object BW_Filter_Low_Edit: TEdit
        Left = 556
        Top = 62
        Width = 24
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 20
        Text = '3'
      end
      object Custom_Butt_Filter_CheckBox: TCheckBox
        Left = 470
        Top = 21
        Width = 165
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Custom Butterworth filter'
        TabOrder = 21
        OnClick = r_ceckboxClick
      end
      object MedianFilter_CheckBox: TCheckBox
        Left = 465
        Top = 114
        Width = 113
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Median filter'
        TabOrder = 22
        OnClick = MedianFilter_CheckBoxClick
      end
      object Median_WL_Edit: TEdit
        Left = 541
        Top = 137
        Width = 25
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 23
        Text = '40'
      end
      object First_Derivative_CheckBox: TCheckBox
        Left = 12
        Top = 378
        Width = 157
        Height = 17
        Caption = '1st derivative'
        TabOrder = 24
        OnClick = First_Derivative_CheckBoxClick
      end
      object Second_Derivative_CheckBox: TCheckBox
        Left = 12
        Top = 401
        Width = 157
        Height = 17
        Caption = '2nd derivative'
        TabOrder = 25
        OnClick = Second_Derivative_CheckBoxClick
      end
      object Derivative_Range_Edit: TEdit
        Left = 115
        Top = 421
        Width = 35
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 26
        Text = '3'
      end
      object Sum_Abs_Window_CheckBox: TCheckBox
        Left = 11
        Top = 93
        Width = 161
        Height = 22
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Sum of abs value in window'
        TabOrder = 27
        OnClick = Normalze_CheckBoxClick
      end
      object Sum_Abs_Window_Edit: TEdit
        Left = 207
        Top = 95
        Width = 52
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 28
        Text = '30'
      end
      object Corr_V_Wave_Signal_CheckBox: TCheckBox
        Left = 12
        Top = 239
        Width = 245
        Height = 16
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Correlation coefficient with V wave; step[ms]='
        TabOrder = 29
        OnClick = Hilb_Phase_CheckBoxClick
      end
      object VWaveCorrStepEdit: TEdit
        Left = 248
        Top = 236
        Width = 33
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 30
        Text = '5'
      end
      object PSimilarity_CheckBox: TCheckBox
        Left = 12
        Top = 270
        Width = 245
        Height = 16
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'P-similairty coefficient with V wave'
        TabOrder = 31
        OnClick = Hilb_Phase_CheckBoxClick
      end
      object XCorr_Filter_CheckBox: TCheckBox
        Left = 12
        Top = 254
        Width = 245
        Height = 16
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'XCorrelation coefficient with V wave'
        TabOrder = 32
        OnClick = Hilb_Phase_CheckBoxClick
      end
      object SLWMM_CheckBox: TCheckBox
        Left = 465
        Top = 178
        Width = 175
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = '(max-min) in sliding window'
        TabOrder = 33
        OnClick = MedianFilter_CheckBoxClick
      end
      object SlidingmaxminEdit: TEdit
        Left = 524
        Top = 201
        Width = 25
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 34
        Text = '40'
      end
    end
    object Preak_Detection_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Peak Det.'
      ImageIndex = 7
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label9: TLabel
        Left = 14
        Top = 170
        Width = 142
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Peak detection threshold [0-1]'
      end
      object Label10: TLabel
        Left = 59
        Top = 196
        Width = 95
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Blanking period [ms]'
      end
      object Label11: TLabel
        Left = 1
        Top = 222
        Width = 154
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Peak detection window size [ms]'
      end
      object Det_Peaks_Th_Button: TButton
        Left = 236
        Top = 169
        Width = 142
        Height = 27
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Detect Peaks'
        TabOrder = 0
        OnClick = Det_Peaks_Th_ButtonClick
      end
      object PDT_Edit: TEdit
        Left = 170
        Top = 168
        Width = 43
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 1
        Text = '0.1'
        TextHint = '0.1'
      end
      object PD_Blanking_Edit: TEdit
        Left = 170
        Top = 192
        Width = 43
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '80'
      end
      object PDTW_Edit: TEdit
        Left = 170
        Top = 220
        Width = 43
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 3
        Text = '500'
      end
      object PDT_SignalMode_RadioGroup: TRadioGroup
        Left = 2
        Top = 2
        Width = 183
        Height = 129
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Algorithm'
        ItemIndex = 1
        Items.Strings = (
          'Just values'
          'abs(First derivative)'
          'Max down slope'
          'First zero crossing'
          'Max diff zero crossing')
        TabOrder = 4
      end
      object CLs_RichEdit: TRichEdit
        Left = 239
        Top = 13
        Width = 120
        Height = 81
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Lines.Strings = (
          'CL=')
        ParentFont = False
        ReadOnly = True
        TabOrder = 5
        Zoom = 100
      end
    end
    object All_sigs_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'All display'
      ImageIndex = 13
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Signals_Display_PaintBox: TPaintBox
        Left = 11
        Top = 51
        Width = 576
        Height = 243
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        OnMouseDown = Signals_Display_PaintBoxMouseDown
        OnMouseMove = Signals_Display_PaintBoxMouseMove
        OnMouseUp = Signals_Display_PaintBoxMouseUp
        OnPaint = Signals_Display_PaintBoxPaint
      end
      object Label41: TLabel
        Left = 2
        Top = 2
        Width = 142
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Size of activation "block" [ms]'
      end
      object Label42: TLabel
        Left = 286
        Top = 7
        Width = 61
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'SmplPerPixel'
      end
      object Label48: TLabel
        Left = 504
        Top = 3
        Width = 54
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Marker size'
      end
      object Label52: TLabel
        Left = 13
        Top = 28
        Width = 290
        Height = 13
        Caption = 'ELECTRICAL SILENCE MAPPING:   Sliding window size [ms]'
      end
      object Label58: TLabel
        Left = 352
        Top = 28
        Width = 153
        Height = 13
        Caption = 'Electrocal silence threshold [mV]'
      end
      object COR_Label: TLabel
        Left = 672
        Top = 28
        Width = 124
        Height = 13
        Caption = 'Continuous Activity Ratio='
      end
      object Label59: TLabel
        Left = 745
        Top = 3
        Width = 62
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Bottom offset'
      end
      object PercOcc_Block_Edit: TEdit
        Left = 148
        Top = 2
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 0
        Text = '30'
      end
      object PercOcc_Rec_Button: TButton
        Left = 192
        Top = 1
        Width = 90
        Height = 24
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Based on peaks'
        TabOrder = 1
        OnClick = PercOcc_Rec_ButtonClick
      end
      object SPP_Edit: TEdit
        Left = 351
        Top = 2
        Width = 27
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '0.2'
      end
      object MCP_CheckBox: TCheckBox
        Left = 382
        Top = 2
        Width = 113
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Mark activity periods'
        TabOrder = 3
        OnClick = MCP_CheckBoxClick
      end
      object MS_Edit: TEdit
        Left = 563
        Top = 0
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 4
        Text = '4'
      end
      object Black_Markers_CheckBox: TCheckBox
        Left = 604
        Top = 4
        Width = 105
        Height = 11
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BiDiMode = bdLeftToRight
        Caption = 'Black markers'
        ParentBiDiMode = False
        TabOrder = 5
        OnClick = Black_Markers_CheckBoxClick
      end
      object SLWD_Edit: TEdit
        Left = 309
        Top = 25
        Width = 37
        Height = 21
        TabOrder = 6
        Text = '15'
      end
      object EST_Edit: TEdit
        Left = 511
        Top = 21
        Width = 37
        Height = 21
        TabOrder = 7
        Text = '0.04'
      end
      object CAR_Button: TButton
        Left = 552
        Top = 20
        Width = 115
        Height = 25
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Based on threshold'
        TabOrder = 8
        OnClick = CAR_ButtonClick
      end
      object BottomOffset_Edit: TEdit
        Left = 811
        Top = 2
        Width = 27
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 9
        Text = '0.2'
      end
      object Repaint_All_Signals_Button: TButton
        Left = 842
        Top = 2
        Width = 54
        Height = 25
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Repaint'
        TabOrder = 10
        OnClick = Repaint_All_Signals_ButtonClick
      end
    end
    object TabSheet4: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'MPC in time'
      ImageIndex = 15
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label53: TLabel
        Left = 8
        Top = 133
        Width = 81
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Single point MPC'
      end
      object Label54: TLabel
        Left = 21
        Top = 190
        Width = 67
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = '2 signals MPC'
      end
      object Label55: TLabel
        Left = 185
        Top = 183
        Width = 82
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Widnow size [ms]'
      end
      object Label56: TLabel
        Left = 185
        Top = 205
        Width = 84
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Widnow step [ms]'
      end
      object Label57: TLabel
        Left = 185
        Top = 231
        Width = 134
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Moving average size for CLs'
      end
      object MPC_C_Chart1: TChart
        Left = 21
        Top = 24
        Width = 780
        Height = 105
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Symbol.Visible = False
        Legend.Visible = False
        Title.Text.Strings = (
          'TChart')
        Title.Visible = False
        View3D = False
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          39
          15
          39)
        ColorPaletteIndex = 13
        object Series7: TLineSeries
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = 10708548
          Pointer.Gradient.EndColor = 10708548
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object MPC_Recalc_Button: TButton
        Left = 107
        Top = 124
        Width = 74
        Height = 32
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Recalculate'
        TabOrder = 1
        OnClick = MPC_Recalc_ButtonClick
      end
      object PreprocessandreclaclMPC_Button: TButton
        Left = 190
        Top = 124
        Width = 120
        Height = 31
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'preprocess+Recalc'
        TabOrder = 2
        OnClick = PreprocessandreclaclMPC_ButtonClick
      end
      object MT1_RadioGroup: TRadioGroup
        Left = 333
        Top = 124
        Width = 182
        Height = 53
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Algorithm used'
        ItemIndex = 0
        Items.Strings = (
          'Mean Phase Coherence'
          'Correlation')
        TabOrder = 3
      end
      object TPMPC_Button: TButton
        Left = 107
        Top = 181
        Width = 74
        Height = 32
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Recalculate'
        TabOrder = 4
        OnClick = TPMPC_ButtonClick
      end
      object WS1_Edit: TEdit
        Left = 281
        Top = 181
        Width = 48
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 5
        Text = '5000'
      end
      object WS2_Edit: TEdit
        Left = 281
        Top = 203
        Width = 48
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 6
        Text = '1000'
      end
      object MAVCL_Edit: TEdit
        Left = 325
        Top = 229
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 7
        Text = '10'
      end
    end
    object Poincare_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Poincare '
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PM_Delay_Label: TLabel
        Left = 267
        Top = 55
        Width = 27
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Delay'
      end
      object P_Map_PaintBox: TPaintBox
        Left = 2
        Top = 8
        Width = 249
        Height = 224
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Color = clWhite
        ParentColor = False
        OnPaint = P_Map_PaintBoxPaint
      end
      object PM_recalculate_Button: TButton
        Left = 263
        Top = 8
        Width = 129
        Height = 41
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'RECALCULATE'
        TabOrder = 0
        OnClick = PM_recalculate_ButtonClick
      end
      object PM_Delay_Edit: TEdit
        Left = 299
        Top = 53
        Width = 54
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 1
        Text = '1'
      end
    end
    object Correlation_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Corr'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Range_Label: TLabel
        Left = 7
        Top = 213
        Width = 73
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Values spread='
      end
      object Volume_Label: TLabel
        Left = 160
        Top = 285
        Width = 117
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Volume of phase space='
      end
      object CorrTable_PaintBox: TPaintBox
        Left = 7
        Top = 8
        Width = 137
        Height = 136
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        OnMouseDown = CorrTable_PaintBoxMouseDown
        OnPaint = CorrTable_PaintBoxPaint
      end
      object Label7: TLabel
        Left = 17
        Top = 174
        Width = 120
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Phase Space Resolution:'
      end
      object Scatter_Chart: TChart
        Left = 160
        Top = 2
        Width = 319
        Height = 279
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BackWall.Brush.Style = bsClear
        Legend.Visible = False
        Title.Text.Strings = (
          'TChart')
        Title.Visible = False
        BottomAxis.Title.Caption = 'L'
        LeftAxis.Title.Caption = 'L'
        View3D = False
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series2: TPointSeries
          SeriesColor = clRed
          ClickableLine = False
          Pointer.Brush.Color = clBlack
          Pointer.Brush.Gradient.EndColor = clRed
          Pointer.Gradient.EndColor = clRed
          Pointer.HorizSize = 1
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.VertSize = 1
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series3: TLineSeries
          SeriesColor = clBlack
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Calculate_CORR_Table_Button: TButton
        Left = 7
        Top = 148
        Width = 137
        Height = 22
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate CORR table'
        TabOrder = 1
        OnClick = Update_CORR_Table_ButtonClick
      end
      object Phase_Space_Resolution_Edit: TEdit
        Left = 94
        Top = 187
        Width = 44
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '0.1'
      end
    end
    object Histogram_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Histogram'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label3: TLabel
        Left = 3
        Top = 245
        Width = 36
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Bin size'
      end
      object Histogram_Chart: TChart
        Left = 11
        Top = 8
        Width = 432
        Height = 216
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BackWall.Brush.Style = bsClear
        Legend.Visible = False
        Title.Text.Strings = (
          '')
        View3D = False
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series4: TBarSeries
          Marks.Visible = False
          Marks.BackColor = clWhite
          Marks.Color = clWhite
          SeriesColor = clBlack
          BarWidthPercent = 80
          Dark3D = False
          MultiBar = mbNone
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object Histo_Recalculate_Button: TButton
        Left = 124
        Top = 237
        Width = 74
        Height = 25
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Recalculate'
        TabOrder = 1
        OnClick = Histo_Recalculate_ButtonClick
      end
      object Bin_Size_Edit: TEdit
        Left = 46
        Top = 239
        Width = 65
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '1'
      end
      object ShSD_CheckBox: TCheckBox
        Left = 15
        Top = 278
        Width = 146
        Height = 16
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Histogram bin = 0.125*SD'
        Checked = True
        State = cbChecked
        TabOrder = 3
        OnClick = ShSD_CheckBoxClick
      end
      object Hist_Tab_RichEdit: TRichEdit
        Left = 453
        Top = 4
        Width = 173
        Height = 329
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        Zoom = 100
      end
      object Hist_Outliers_Remove_CheckBox: TCheckBox
        Left = 175
        Top = 279
        Width = 174
        Height = 17
        Caption = 'Remove 5% and 95% outliers?'
        TabOrder = 5
        OnClick = Hist_Outliers_Remove_CheckBoxClick
      end
    end
    object FFT_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'FFT'
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label1: TLabel
        Left = 698
        Top = 213
        Width = 56
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Min DF [Hz]'
      end
      object Label4: TLabel
        Left = 698
        Top = 234
        Width = 59
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Max DF [Hz]'
      end
      object FFT_Spectrum_Chart: TChart
        Left = 2
        Top = 2
        Width = 364
        Height = 309
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BackWall.Brush.Style = bsClear
        Legend.Visible = False
        Title.Font.Color = clBlack
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'FFT spectrum')
        BottomAxis.Title.Caption = 'Hz'
        LeftAxis.Title.Caption = 'Amplitude'
        View3D = False
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object LineSeries1: TLineSeries
          SeriesColor = clBlack
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series5: TPointSeries
          ClickableLine = False
          Pointer.Brush.Color = clGreen
          Pointer.Brush.Gradient.EndColor = 10708548
          Pointer.Gradient.EndColor = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object FFT_Results_RichEdit: TRichEdit
        Left = 805
        Top = 20
        Width = 165
        Height = 120
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Lines.Strings = (
          'DF=')
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
        Zoom = 100
      end
      object FullFFT_Chart: TChart
        Left = 382
        Top = 14
        Width = 412
        Height = 128
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Visible = False
        Title.Text.Strings = (
          'Full spectrum (Re+Im)')
        View3D = False
        TabOrder = 2
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series11: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Min_DF_Edit: TEdit
        Left = 761
        Top = 209
        Width = 52
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 3
        Text = '3'
      end
      object Max_DF_Edit: TEdit
        Left = 761
        Top = 232
        Width = 52
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 4
        Text = '9'
      end
      object Resampled_Chart: TChart
        Left = 378
        Top = 159
        Width = 297
        Height = 128
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Visible = False
        Title.Text.Strings = (
          'Resampled')
        View3D = False
        TabOrder = 5
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object LineSeries4: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
    object Wavelet_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Wavelets'
      ImageIndex = 5
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label8: TLabel
        Left = 10
        Top = 156
        Width = 144
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Scaling factor range [Min,Max]'
      end
      object Label5: TLabel
        Left = 15
        Top = 204
        Width = 88
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Scaling factor step'
      end
      object Wavelet_Spectrum_Chart: TChart
        Left = 2
        Top = 2
        Width = 780
        Height = 129
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BackWall.Brush.Style = bsClear
        Legend.Visible = False
        Title.Text.Strings = (
          'Wavelet Spectrum')
        View3D = False
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object LineSeries2: TLineSeries
          SeriesColor = clRed
          Brush.BackColor = clDefault
          Pointer.Brush.Color = clBlack
          Pointer.Brush.Gradient.EndColor = clBlack
          Pointer.Gradient.EndColor = clBlack
          Pointer.HorizSize = 3
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.VertSize = 3
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object PointSeries1: TPointSeries
          ClickableLine = False
          Pointer.Brush.Gradient.EndColor = 10708548
          Pointer.Gradient.EndColor = 10708548
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Wavelet_Scaling_Max_Edit: TEdit
        Left = 72
        Top = 173
        Width = 42
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 1
        Text = '0.2'
      end
      object Wavelet_Scaling_Min_Edit: TEdit
        Left = 25
        Top = 173
        Width = 43
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '0.1'
      end
      object Calculate_Wavelet_Spectrum_Button: TButton
        Left = 109
        Top = 267
        Width = 63
        Height = 27
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate'
        TabOrder = 3
        OnClick = Calculate_Wavelet_Spectrum_ButtonClick
      end
      object Calculate_Wavelet_For_All_Sigs_Button: TButton
        Left = 9
        Top = 267
        Width = 96
        Height = 25
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate for ALL'
        TabOrder = 4
        OnClick = Calculate_Wavelet_For_All_Sigs_ButtonClick
      end
      object IndexORTime_RadioGroup: TRadioGroup
        Left = 270
        Top = 285
        Width = 80
        Height = 45
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Index or time'
        ItemIndex = 1
        Items.Strings = (
          'Index'
          'Time')
        TabOrder = 5
        OnClick = IndexORTime_RadioGroupClick
      end
      object CLs_Labels_Display_CheckBox: TCheckBox
        Left = 270
        Top = 334
        Width = 93
        Height = 11
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Display labels'
        TabOrder = 6
        OnClick = CLs_Labels_Display_CheckBoxClick
      end
      object ISI_RichEdit: TRichEdit
        Left = 9
        Top = 298
        Width = 189
        Height = 61
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 7
        Zoom = 100
      end
      object Cycle_Lengths_Chart: TChart
        Left = 176
        Top = 135
        Width = 780
        Height = 139
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Title.Text.Strings = (
          'Cycle lengths')
        Legend.Visible = False
        Title.Text.Strings = (
          'Cycle lengths')
        View3D = False
        TabOrder = 8
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          36
          15
          36)
        ColorPaletteIndex = 13
        object Series52: TLineSeries
          SeriesColor = clGreen
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = clGreen
          Pointer.Gradient.EndColor = clGreen
          Pointer.HorizSize = 1
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.VertSize = 1
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Range_Wavelet_Scaling_CheckBox: TCheckBox
        Left = 10
        Top = 135
        Width = 154
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Use range of scaling factors'
        TabOrder = 9
      end
      object Wavelet_Step_Edit: TEdit
        Left = 109
        Top = 199
        Width = 42
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 10
        Text = '0.02'
      end
      object Wavelet_integrated_spect_RadioGroup: TRadioGroup
        Left = 10
        Top = 221
        Width = 120
        Height = 42
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Integrated spectrum'
        ItemIndex = 0
        Items.Strings = (
          'Mean'
          'Max')
        TabOrder = 11
      end
      object WS_RadioGroup: TRadioGroup
        Left = 354
        Top = 291
        Width = 80
        Height = 44
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Source'
        ItemIndex = 0
        Items.Strings = (
          'Raw electrogram'
          'Filtered ')
        TabOrder = 12
        OnClick = IndexORTime_RadioGroupClick
      end
    end
    object Synchronization_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Synch'
      ImageIndex = 6
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Synchronization_Matrix_PaintBox: TPaintBox
        Left = 2
        Top = 7
        Width = 136
        Height = 137
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        OnMouseDown = Synchronization_Matrix_PaintBoxMouseDown
        OnPaint = Synchronization_Matrix_PaintBoxPaint
      end
      object Label51: TLabel
        Left = 478
        Top = 16
        Width = 82
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Window size [ms]'
      end
      object Synchrogram_Chart: TChart
        Left = 147
        Top = 2
        Width = 327
        Height = 283
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BackWall.Brush.Style = bsClear
        Legend.Visible = False
        MarginBottom = 1
        MarginLeft = 0
        MarginRight = 5
        MarginTop = 0
        Title.Text.Strings = (
          'Synchrogram')
        BottomAxis.Title.Caption = 'Id of consecutive activation'
        LeftAxis.Title.Caption = 'Relative phases in 2nd signal'
        View3D = False
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object PointSeries2: TPointSeries
          SeriesColor = clRed
          ClickableLine = False
          Pointer.Brush.Color = clBlack
          Pointer.Brush.Gradient.EndColor = clRed
          Pointer.Gradient.EndColor = clRed
          Pointer.HorizSize = 1
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.VertSize = 1
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Calculate_Synch_Table_Button: TButton
        Left = 2
        Top = 150
        Width = 136
        Height = 22
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate SYNCH table'
        TabOrder = 1
        OnClick = Calculate_Synch_Table_ButtonClick
      end
      object Sych_Results_RichEdit: TRichEdit
        Left = 2
        Top = 176
        Width = 136
        Height = 109
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 2
        Zoom = 100
      end
      object R11Edit: TEdit
        Left = 569
        Top = 14
        Width = 50
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 3
        Text = '3000'
      end
      object Synch_Movie_Start_Button: TButton
        Left = 499
        Top = 42
        Width = 49
        Height = 16
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'START'
        TabOrder = 4
        OnClick = Synch_Movie_Start_ButtonClick
      end
      object Synch_Movie_Pause_Button: TButton
        Left = 562
        Top = 42
        Width = 48
        Height = 16
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'PAUSE'
        TabOrder = 5
        OnClick = Synch_Movie_Pause_ButtonClick
      end
    end
    object TimeShiftsTabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Time shifts'
      ImageIndex = 11
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label36: TLabel
        Left = 1028
        Top = 50
        Width = 68
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Line thickness'
      end
      object Label37: TLabel
        Left = 1021
        Top = 96
        Width = 46
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Colors set'
      end
      object Label38: TLabel
        Left = 1021
        Top = 19
        Width = 234
        Height = 13
        Caption = 'Histogram parameters can be set in Histogram tab'
      end
      object Label2: TLabel
        Left = 386
        Top = 227
        Width = 136
        Height = 13
        Caption = 'Outliers cutoff threshold [ms]:'
      end
      object L_Label: TLabel
        Left = 17
        Top = 431
        Width = 36
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Length:'
      end
      object Label62: TLabel
        Left = 386
        Top = 266
        Width = 199
        Height = 13
        Caption = 'RED: LA, LA earlier, BLUE: RA, RA earlier'
      end
      object Line_Thickness_ComboBox: TComboBox
        Left = 1100
        Top = 47
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 0
        Text = '1'
        OnChange = Line_Thickness_ComboBoxChange
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5')
      end
      object ColorSet_ComboBox: TComboBox
        Left = 1071
        Top = 93
        Width = 83
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 1
        Text = 'Blue/red'
        OnChange = ColorSet_ComboBoxChange
        Items.Strings = (
          'Blue/red'
          'Black/gray')
      end
      object Background_grid_CheckBox: TCheckBox
        Left = 1039
        Top = 72
        Width = 109
        Height = 11
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Background grid'
        Checked = True
        State = cbChecked
        TabOrder = 2
        OnClick = Background_grid_CheckBoxClick
      end
      object TimeShifts_Chart: TChart
        Left = 2
        Top = 2
        Width = 379
        Height = 302
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BackWall.Brush.Style = bsClear
        Legend.Visible = False
        Title.Font.Color = -1
        Title.Text.Strings = (
          ''
          'Time shifts between corresponding activation times [ms]')
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Maximum = 90.000000000000000000
        BottomAxis.Minimum = -90.000000000000000000
        BottomAxis.Title.Caption = 'Time shift [ms]'
        LeftAxis.Title.Caption = '# of occurrences'
        View3D = False
        Color = clWhite
        TabOrder = 3
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object BarSeries2: TBarSeries
          Marks.Visible = False
          Marks.BackColor = clWhite
          Marks.Color = clWhite
          SeriesColor = clBlack
          BarWidthPercent = 100
          Dark3D = False
          MultiBar = mbNone
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object CalculateTimeShiftHist_Button: TButton
        Left = 1021
        Top = 131
        Width = 145
        Height = 25
        Caption = 'Calculate histogram'
        TabOrder = 4
        OnClick = CalculateTimeShiftHist_ButtonClick
      end
      object TimeShiftsDynamicChart: TChart
        Left = 2
        Top = 308
        Width = 852
        Height = 140
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Symbol.Visible = False
        Legend.Visible = False
        Title.Text.Strings = (
          'TChart')
        Title.Visible = False
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Maximum = 90.000000000000000000
        LeftAxis.Minimum = -90.000000000000000000
        LeftAxis.Title.Caption = 'Time shift [ms]'
        View3D = False
        Color = clWhite
        TabOrder = 5
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          39
          15
          39)
        ColorPaletteIndex = 13
        object LineSeries3: TBarSeries
          BarBrush.BackColor = clDefault
          Marks.Visible = False
          Marks.Callout.Length = 8
          BarWidthPercent = 100
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object TimeShiftsRichEdit: TRichEdit
        Left = 386
        Top = 3
        Width = 253
        Height = 44
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Lines.Strings = (
          'TimeShiftsRichEdit')
        ParentFont = False
        TabOrder = 6
        Zoom = 100
      end
      object OutThEdit: TEdit
        Left = 528
        Top = 224
        Width = 39
        Height = 21
        TabOrder = 7
        Text = '50'
      end
      object AFCLs_Global_Chart: TChart
        Left = 2
        Top = 452
        Width = 852
        Height = 140
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Symbol.Visible = False
        Legend.Visible = False
        Title.Text.Strings = (
          'Global AFCL [ms]')
        Title.Visible = False
        BottomAxis.Title.Caption = 'Time [ms]'
        LeftAxis.Title.Caption = '[ms]'
        View3D = False
        Color = clWhite
        TabOrder = 8
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          39
          15
          39)
        ColorPaletteIndex = 13
        object Series54: TLineSeries
          Brush.BackColor = clDefault
          LinePen.Width = 2
          Pointer.Brush.Gradient.EndColor = 10708548
          Pointer.Gradient.EndColor = 10708548
          Pointer.HorizSize = 3
          Pointer.InflateMargins = True
          Pointer.Style = psDiamond
          Pointer.VertSize = 3
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series55: TLineSeries
          SeriesColor = clRed
          Brush.BackColor = clDefault
          LinePen.Width = 2
          Pointer.Brush.Gradient.EndColor = clRed
          Pointer.Gradient.EndColor = clRed
          Pointer.HorizSize = 3
          Pointer.InflateMargins = True
          Pointer.Style = psDiamond
          Pointer.VertSize = 3
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object LARAAFCLRichEdit: TRichEdit
        Left = 386
        Top = 53
        Width = 253
        Height = 146
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Lines.Strings = (
          'TimeShiftsRichEdit')
        ParentFont = False
        TabOrder = 9
        Zoom = 100
      end
    end
    object Recur_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Recurrence'
      ImageIndex = 12
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label39: TLabel
        Left = 10
        Top = 10
        Width = 124
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Timestep for MPC diagram'
      end
      object Label40: TLabel
        Left = 16
        Top = 31
        Width = 120
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Threshold for 1-1 diagram'
      end
      object Recurrence_Plot_PaintBox: TPaintBox
        Left = 189
        Top = 8
        Width = 259
        Height = 253
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Color = clWhite
        ParentColor = False
        OnPaint = P_Map_PaintBoxPaint
      end
      object Rec_MPC_timestep_Edit: TEdit
        Left = 143
        Top = 8
        Width = 34
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 0
        Text = '50'
      end
      object Rec_11_Th_Edit: TEdit
        Left = 143
        Top = 31
        Width = 34
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 1
        Text = '0.1'
      end
      object Recurrence_ALL_Button: TButton
        Left = 10
        Top = 53
        Width = 167
        Height = 26
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate MPC recurrence (ALL)'
        TabOrder = 2
      end
      object Recurrence_Pair_Button: TButton
        Left = 10
        Top = 83
        Width = 167
        Height = 27
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate 1-1 recurrence (pair)'
        TabOrder = 3
        OnClick = Recurrence_Pair_ButtonClick
      end
    end
    object TabSheet2: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Mscal rcmp'
      ImageIndex = 14
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label43: TLabel
        Left = 79
        Top = 4
        Width = 77
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Range [ms] from'
      end
      object Label44: TLabel
        Left = 205
        Top = 5
        Width = 9
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'to'
      end
      object Label45: TLabel
        Left = 263
        Top = 6
        Width = 20
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'step'
      end
      object MSC_PaintBox: TPaintBox
        Left = 7
        Top = 36
        Width = 555
        Height = 89
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
      end
      object Label49: TLabel
        Left = 135
        Top = 131
        Width = 56
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Conv. steps'
      end
      object Label50: TLabel
        Left = 441
        Top = 129
        Width = 97
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Starting point CL[ms]'
      end
      object MultiSCale_Recomp_Button: TButton
        Left = 2
        Top = 2
        Width = 73
        Height = 31
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Recompute'
        TabOrder = 0
        OnClick = MultiSCale_Recomp_ButtonClick
      end
      object F1_Edit: TEdit
        Left = 163
        Top = 3
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 1
        Text = '50'
      end
      object F2_Edit: TEdit
        Left = 218
        Top = 2
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '300'
      end
      object F3_Edit: TEdit
        Left = 288
        Top = 2
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 3
        Text = '25'
      end
      object SCM_Chart: TChart
        Left = 566
        Top = 25
        Width = 256
        Height = 221
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        BackWall.Brush.Style = bsClear
        Legend.Visible = False
        Title.Text.Strings = (
          'Number of phase transitions')
        BottomAxis.Title.Caption = 'Scale [ms]'
        LeftAxis.Title.Caption = '#'
        View3D = False
        Color = clWhite
        TabOrder = 4
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object BarSeries1: TBarSeries
          Marks.Visible = False
          Marks.BackColor = clWhite
          Marks.Color = clWhite
          SeriesColor = clBlack
          BarWidthPercent = 80
          Dark3D = False
          MultiBar = mbNone
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object Self_Convergence_Chart: TChart
        Left = 7
        Top = 161
        Width = 368
        Height = 188
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Visible = False
        Title.Text.Strings = (
          'Self-convergence')
        View3D = False
        TabOrder = 5
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series16: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series17: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series18: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series19: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series20: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series21: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series22: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series23: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series24: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series25: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series26: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series27: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series28: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series29: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series30: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series31: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series32: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series33: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series34: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series35: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series36: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series37: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series38: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series39: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series40: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series41: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series42: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series43: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series44: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series45: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series46: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series47: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series48: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series49: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series50: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series51: TLineSeries
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object CnvST_Edit: TEdit
        Left = 195
        Top = 129
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 6
        Text = '5'
      end
      object Self_Conv_Button: TButton
        Left = 7
        Top = 129
        Width = 124
        Height = 22
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Self-conv recompute'
        TabOrder = 7
        OnClick = Self_Conv_ButtonClick
      end
      object OneStarting_convergentSR_Button: TButton
        Left = 236
        Top = 129
        Width = 201
        Height = 22
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'One-starting point convergence (all)'
        TabOrder = 8
        OnClick = OneStarting_convergentSR_ButtonClick
      end
      object SPC_Edit: TEdit
        Left = 441
        Top = 146
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 9
        Text = '180'
      end
      object SelfConvergenceCurrentButton: TButton
        Left = 379
        Top = 177
        Width = 201
        Height = 40
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'One-starting point convergence (current)'
        TabOrder = 10
        OnClick = SelfConvergenceCurrentButtonClick
      end
    end
    object TabSheet3: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Causality'
      ImageIndex = 15
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label15: TLabel
        Left = 10
        Top = 10
        Width = 99
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Range of delays [ms]'
      end
      object Label46: TLabel
        Left = 66
        Top = 36
        Width = 42
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'step [ms]'
      end
      object Label47: TLabel
        Left = 10
        Top = 166
        Width = 48
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'AFCL [ms]'
      end
      object Two_point_Causality_Chart: TChart
        Left = 197
        Top = 2
        Width = 261
        Height = 245
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Symbol.Visible = False
        Legend.Visible = False
        Title.Text.Strings = (
          'TChart')
        Title.Visible = False
        View3D = False
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object PointSeries3: TPointSeries
          Marks.Callout.Length = 8
          SeriesColor = -1
          ClickableLine = False
          Pointer.Brush.Gradient.EndColor = -1
          Pointer.Gradient.EndColor = -1
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object CMin_Edit: TEdit
        Left = 115
        Top = 8
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 1
        Text = '0'
      end
      object CMax_Edit: TEdit
        Left = 156
        Top = 8
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '100'
      end
      object CStep_Edit: TEdit
        Left = 115
        Top = 31
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 3
        Text = '5'
      end
      object CSource_RadioGroup: TRadioGroup
        Left = 10
        Top = 58
        Width = 80
        Height = 45
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Source'
        ItemIndex = 0
        Items.Strings = (
          'Raw electrogram'
          'Filtered ')
        TabOrder = 4
        OnClick = IndexORTime_RadioGroupClick
      end
      object ECTC_RadioGroup: TRadioGroup
        Left = 10
        Top = 107
        Width = 115
        Height = 44
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Electrogram type'
        ItemIndex = 0
        Items.Strings = (
          'Unipolar'
          'Bipolar')
        TabOrder = 5
        OnClick = IndexORTime_RadioGroupClick
      end
      object AFCL_Caus_Edit: TEdit
        Left = 66
        Top = 164
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 6
        Text = '180'
      end
      object Causality_Button: TButton
        Left = 94
        Top = 58
        Width = 99
        Height = 31
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate'
        TabOrder = 7
        OnClick = Causality_ButtonClick
      end
    end
    object Frac_TabSheet: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Fractionation'
      ImageIndex = 15
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label16: TLabel
        Left = 56
        Top = 10
        Width = 93
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Refraction time [ms]'
      end
      object Label17: TLabel
        Left = 90
        Top = 29
        Width = 62
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'PP sensitivity'
      end
      object Label18: TLabel
        Left = 89
        Top = 48
        Width = 62
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Duration [ms]'
      end
      object Label19: TLabel
        Left = 19
        Top = 67
        Width = 133
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Downstroke thrshld [mV/ms]'
      end
      object Compute_NavX_CAFEs_Button: TButton
        Left = 93
        Top = 91
        Width = 118
        Height = 24
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Compute NavX CAFEs'
        TabOrder = 0
        OnClick = Compute_NavX_CAFEs_ButtonClick
      end
      object CAFE_Ref_Edit: TEdit
        Left = 157
        Top = 7
        Width = 53
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 1
        Text = '50'
      end
      object CAFE_PP_Edit: TEdit
        Left = 157
        Top = 27
        Width = 53
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '0.1'
      end
      object CAFE_Dur_Edit: TEdit
        Left = 157
        Top = 47
        Width = 53
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 3
        Text = '10'
      end
      object CAFE_DTh_Edit: TEdit
        Left = 157
        Top = 66
        Width = 53
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 4
        Text = '0.02'
      end
    end
  end
  object Close_Button: TButton
    Left = 17
    Top = 485
    Width = 109
    Height = 25
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Close'
    TabOrder = 2
    OnClick = Close_ButtonClick
  end
  object Raw_Signals_Chart: TChart
    Left = 388
    Top = -5
    Width = 164
    Height = 52
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    BackWall.Brush.Style = bsClear
    Legend.Visible = False
    Title.Text.Strings = (
      'Original signal')
    OnClickSeries = Raw_Signals_ChartClickSeries
    OnUndoZoom = Raw_Signals_ChartUndoZoom
    OnZoom = Raw_Signals_ChartZoom
    View3D = False
    Color = clWhite
    TabOrder = 3
    OnDblClick = Raw_Signals_ChartDblClick
    OnMouseDown = Raw_Signals_ChartMouseDown
    OnMouseMove = Raw_Signals_ChartMouseMove
    OnMouseUp = Raw_Signals_ChartMouseUp
    DefaultCanvas = 'TGDIPlusCanvas'
    PrintMargins = (
      15
      34
      15
      34)
    ColorPaletteIndex = 13
    object Series1: TLineSeries
      SeriesColor = clBlue
      Brush.BackColor = clDefault
      Pointer.Brush.Color = clBlack
      Pointer.Brush.Gradient.EndColor = clBlack
      Pointer.Gradient.EndColor = clBlack
      Pointer.HorizSize = 3
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.VertSize = 3
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series8: TPointSeries
      Marks.BackColor = clRed
      Marks.Color = clRed
      ClickableLine = False
      Pointer.Brush.Color = clYellow
      Pointer.Brush.Gradient.EndColor = 10708548
      Pointer.Gradient.EndColor = 10708548
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series9: TLineSeries
      SeriesColor = clRed
      Brush.BackColor = clDefault
      Pointer.Brush.Gradient.EndColor = clRed
      Pointer.Gradient.EndColor = clRed
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series10: TPointSeries
      ClickableLine = False
      Pointer.Brush.Color = 16512
      Pointer.Brush.Gradient.EndColor = 3513587
      Pointer.Gradient.EndColor = 3513587
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series13: TPointSeries
      Legend.Text = 'CAFE peaks'
      LegendTitle = 'CAFE peaks'
      ClickableLine = False
      Pointer.Brush.Gradient.EndColor = 1330417
      Pointer.Gradient.EndColor = 1330417
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series15: TPointSeries
      ClickableLine = False
      Pointer.Brush.Gradient.EndColor = 11048782
      Pointer.Gradient.EndColor = 11048782
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series14: TLineSeries
      SeriesColor = clRed
      Title = 'Selected Template'
      Brush.BackColor = clDefault
      LinePen.Style = psDash
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object Disp_2nd_Signal_CheckBox: TCheckBox
    Left = 211
    Top = 5
    Width = 63
    Height = 11
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'disp 2nd'
    TabOrder = 4
    OnClick = Disp_2nd_Signal_CheckBoxClick
  end
  object Data_Source_RadioGroup: TRadioGroup
    Left = 5
    Top = 320
    Width = 146
    Height = 85
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Data source for analysis'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemIndex = 1
    Items.Strings = (
      'Raw signals'
      'Filtered signals'
      'Peaks positions '
      'Inter peak intervals')
    ParentFont = False
    TabOrder = 5
    WordWrap = True
    OnClick = Data_Source_RadioGroupClick
  end
  object Secondary_Data_StringGrid: TStringGrid
    Left = 89
    Top = 25
    Width = 74
    Height = 290
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    FixedCols = 0
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
    TabOrder = 6
    OnClick = Secondary_Data_StringGridClick
  end
  object Filtered_Signal_Chart: TChart
    Left = 562
    Top = 0
    Width = 141
    Height = 46
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Legend.Title.Text.Strings = (
      'Filtered signal')
    Legend.Visible = False
    Title.Text.Strings = (
      'Filtered signal')
    View3D = False
    Color = clWhite
    TabOrder = 7
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series6: TLineSeries
      Brush.BackColor = clDefault
      Pointer.Brush.Gradient.EndColor = 10708548
      Pointer.Gradient.EndColor = 10708548
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series12: TLineSeries
      SeriesColor = clRed
      Brush.BackColor = clDefault
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series53: TPointSeries
      ClickableLine = False
      Pointer.Brush.Gradient.EndColor = 3513587
      Pointer.Gradient.EndColor = 3513587
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object MainMenu1: TMainMenu
    Left = 112
    Top = 408
    object File1: TMenuItem
      Caption = 'File'
      object Exports1: TMenuItem
        Caption = 'Export'
        object ImportcurrentdataserieYvalue1: TMenuItem
          Caption = 'Import data serie (Y value)'
          OnClick = ImportcurrentdataserieYvalue1Click
        end
        object Exportcurrentdataserie1: TMenuItem
          Caption = 'Export current data serie (Y value)'
          OnClick = Exportcurrentdataserie1Click
        end
        object N2: TMenuItem
          Caption = '-'
        end
        object ExportactivationstimesSinRecompHilb1: TMenuItem
          Caption = 'Export activations times (SinRecomp+Hilb) of all data points'
          OnClick = ExportactivationstimesSinRecompHilb1Click
        end
        object ExportintervalsSinRecompHilbofalldatapoints1: TMenuItem
          Caption = 'Export intervals (SinRecomp+Hilb) of all data points'
          OnClick = ExportintervalsSinRecompHilbofalldatapoints1Click
        end
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Close1: TMenuItem
        Caption = 'Close'
        OnClick = Close1Click
      end
    end
    object Vwaveremoval1: TMenuItem
      Caption = 'Template'
      object TakeVwavetemplatemorphologycurrentsegment1: TMenuItem
        Caption = 'Take current segment as template morphology'
        OnClick = TakeVwavetemplatemorphologycurrentsegment1Click
      end
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 32
    Top = 600
  end
  object DWT_Chart_Add_Remove_PopupMenu: TPopupMenu
    Left = 320
    Top = 8
    object Addmarker1: TMenuItem
      Caption = 'Add marker'
      OnClick = Addmarker1Click
    end
    object Removemarker1: TMenuItem
      Caption = 'Remove marker'
      OnClick = Removemarker1Click
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 88
    Top = 592
  end
end
