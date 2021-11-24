object Display_Options_Form_1: TDisplay_Options_Form_1
  Left = 431
  Top = 158
  Caption = 'Display options'
  ClientHeight = 792
  ClientWidth = 1004
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Label6: TLabel
    Left = 16
    Top = 74
    Width = 103
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Size of the Data Point'
  end
  object Label9: TLabel
    Left = 210
    Top = 76
    Width = 17
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Min'
  end
  object Label10: TLabel
    Left = 333
    Top = 76
    Width = 20
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Max'
  end
  object Bevel6: TBevel
    Left = 6
    Top = 5
    Width = 180
    Height = 156
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
  end
  object Label11: TLabel
    Left = 487
    Top = 41
    Width = 74
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Contour interval'
  end
  object Label12: TLabel
    Left = 490
    Top = 61
    Width = 68
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Line thickness'
  end
  object Bevel7: TBevel
    Left = 465
    Top = 4
    Width = 181
    Height = 85
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
  end
  object Bevel1: TBevel
    Left = 6
    Top = 245
    Width = 180
    Height = 62
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
  end
  object Bevel3: TBevel
    Left = 190
    Top = 4
    Width = 274
    Height = 108
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
  end
  object Label20: TLabel
    Left = 16
    Top = 212
    Width = 103
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Size of ablation points'
  end
  object Label21: TLabel
    Left = 581
    Top = 303
    Width = 90
    Height = 13
    Caption = 'Transparency level'
    Visible = False
  end
  object Label13: TLabel
    Left = 16
    Top = 8
    Width = 109
    Height = 13
    Caption = 'Data points display'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label1: TLabel
    Left = 195
    Top = 8
    Width = 155
    Height = 13
    Caption = 'Color scale palette settings'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 468
    Top = 8
    Width = 94
    Height = 13
    Caption = 'Contours display'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Bevel4: TBevel
    Left = 7
    Top = 168
    Width = 180
    Height = 69
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
  end
  object Label4: TLabel
    Left = 17
    Top = 171
    Width = 128
    Height = 13
    Caption = 'Ablation points display'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 15
    Top = 135
    Width = 101
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Scaling factor [0-100]'
  end
  object Label2: TLabel
    Left = 38
    Top = 443
    Width = 75
    Height = 13
    Caption = 'Arrow thickness'
  end
  object Label7: TLabel
    Left = 23
    Top = 465
    Width = 87
    Height = 13
    Caption = 'Perpendicular shift'
  end
  object Label8: TLabel
    Left = 48
    Top = 488
    Width = 63
    Height = 13
    Caption = 'Arrow scaling'
  end
  object Label14: TLabel
    Left = 195
    Top = 117
    Width = 101
    Height = 13
    Caption = 'Text dist from surface'
  end
  object Font_RadioGroup: TRadioGroup
    Left = 466
    Top = 96
    Width = 180
    Height = 102
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Font type in 3D panel'
    ItemIndex = 2
    Items.Strings = (
      'Times Roman 10'
      'Halvetica 12'
      'Halvetica 18'
      'Times Roman 24')
    TabOrder = 1
  end
  object Fixed_Palette_Range_CheckBox: TCheckBox
    Left = 195
    Top = 25
    Width = 133
    Height = 17
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Fixed scale range'
    TabOrder = 2
    OnClick = Fixed_Palette_Range_CheckBoxClick
  end
  object Fixed_Palette_Range_Min_Edit: TEdit
    Left = 247
    Top = 76
    Width = 82
    Height = 21
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    TabOrder = 3
    Text = '0'
  end
  object Fixed_Palette_Range_Max_Edit: TEdit
    Left = 370
    Top = 76
    Width = 92
    Height = 21
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    TabOrder = 4
    Text = '1'
  end
  object Isovalues_CheckBox: TCheckBox
    Left = 487
    Top = 20
    Width = 144
    Height = 20
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display iso-values contours'
    TabOrder = 5
  end
  object Palette_Coarsening_LeveL_Edit: TEdit
    Left = 568
    Top = 37
    Width = 42
    Height = 21
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    TabOrder = 6
    Text = '2'
  end
  object Line_Thickness_ComboBox: TComboBox
    Left = 567
    Top = 61
    Width = 43
    Height = 21
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    AutoDropDown = True
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 7
    Text = '1'
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9'
      '10')
  end
  object DP_Name_CheckBox: TCheckBox
    Left = 29
    Top = 45
    Width = 97
    Height = 17
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display name'
    TabOrder = 8
  end
  object Display_Local_Signal_CheckBox: TCheckBox
    Left = 9
    Top = 249
    Width = 153
    Height = 17
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display Local Signal'
    Checked = True
    State = cbChecked
    TabOrder = 9
  end
  object Display_Ref1_Signal_CheckBox: TCheckBox
    Left = 9
    Top = 266
    Width = 167
    Height = 17
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display Reference Signal'
    Checked = True
    State = cbChecked
    TabOrder = 10
  end
  object Display_ECG_Signal_CheckBox: TCheckBox
    Left = 8
    Top = 282
    Width = 167
    Height = 17
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display ECG Signal'
    Checked = True
    State = cbChecked
    TabOrder = 11
  end
  object Pointing_Cross_Display_CheckBox: TCheckBox
    Left = 29
    Top = 94
    Width = 140
    Height = 18
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display pointing cross'
    Checked = True
    State = cbChecked
    TabOrder = 12
  end
  object Draw_Data_Points_CheckBox: TCheckBox
    Left = 29
    Top = 26
    Width = 113
    Height = 18
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display data points'
    Checked = True
    State = cbChecked
    TabOrder = 13
  end
  object Display_Geo_Nodes_CheckBox: TCheckBox
    Left = 192
    Top = 155
    Width = 146
    Height = 17
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display geometry nodes'
    TabOrder = 14
  end
  object Display_3D_arrow_CheckBox: TCheckBox
    Left = 432
    Top = 314
    Width = 271
    Height = 11
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display 3D arrow pointing direction of propagation'
    TabOrder = 15
    Visible = False
  end
  object Display_All_DP_Sets_CheckBox: TCheckBox
    Left = 438
    Top = 336
    Width = 251
    Height = 11
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display all data point sets'
    TabOrder = 16
    Visible = False
  end
  object Extremes_Colours_RadioGroup: TRadioGroup
    Left = 551
    Top = 222
    Width = 167
    Height = 76
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Extremes colours'
    ItemIndex = 0
    Items.Strings = (
      'Nothing special'
      'Low: black, high: white'
      'Low: white, high: black')
    TabOrder = 17
    Visible = False
  end
  object Display_Triangles_CheckBox: TCheckBox
    Left = 191
    Top = 191
    Width = 167
    Height = 13
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display 3D surface'
    Checked = True
    State = cbChecked
    TabOrder = 18
  end
  object Invert_Palette_CheckBox: TCheckBox
    Left = 195
    Top = 41
    Width = 141
    Height = 16
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Invert palette'
    TabOrder = 19
  end
  object Interpolation_CheckBox: TCheckBox
    Left = 192
    Top = 329
    Width = 177
    Height = 16
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Surface Interpolation'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 20
  end
  object NavX_Patient_Doll_CheckBox: TCheckBox
    Left = 401
    Top = 202
    Width = 177
    Height = 18
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display patient doll'
    TabOrder = 21
    Visible = False
  end
  object Display_Segments_CheckBox: TCheckBox
    Left = 192
    Top = 207
    Width = 167
    Height = 15
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display segments names'
    Checked = True
    State = cbChecked
    TabOrder = 22
  end
  object Display_17_LV_markersCheckBox: TCheckBox
    Left = 192
    Top = 290
    Width = 215
    Height = 19
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display 17 LV segmentation markers'
    TabOrder = 23
  end
  object DAP_CheckBox: TCheckBox
    Left = 30
    Top = 190
    Width = 127
    Height = 16
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display ablation points'
    Checked = True
    State = cbChecked
    TabOrder = 24
  end
  object P1_Limits_Button: TButton
    Left = 351
    Top = 24
    Width = 110
    Height = 22
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Preset: 0.2 - 1.0'
    TabOrder = 25
    OnClick = P1_Limits_ButtonClick
  end
  object Disp_Segm_Paths_CheckBox: TCheckBox
    Left = 401
    Top = 224
    Width = 168
    Height = 20
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display segmentation paths'
    Checked = True
    State = cbChecked
    TabOrder = 26
    Visible = False
  end
  object PaintRegPoints_CheckBox: TCheckBox
    Left = 192
    Top = 224
    Width = 178
    Height = 17
    Caption = 'Display 3D registraiton markers'
    Checked = True
    State = cbChecked
    TabOrder = 27
  end
  object Transparency_ScrollBar: TScrollBar
    Left = 432
    Top = 411
    Width = 121
    Height = 17
    PageSize = 0
    TabOrder = 28
    Visible = False
  end
  object Display_Map_Title_CheckBox: TCheckBox
    Left = 192
    Top = 253
    Width = 178
    Height = 17
    Caption = 'Display map title and value'
    TabOrder = 29
  end
  object Display_Normals_CheckBox: TCheckBox
    Left = 192
    Top = 310
    Width = 161
    Height = 17
    Caption = 'Display normals to surface'
    Checked = True
    State = cbChecked
    TabOrder = 30
  end
  object P2_Limits_Button: TButton
    Left = 351
    Top = 50
    Width = 110
    Height = 22
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Preset: 0.5 - 1.5'
    TabOrder = 31
    OnClick = P2_Limits_ButtonClick
  end
  object Auto_DP_Size_Scaling_CheckBox: TCheckBox
    Left = 29
    Top = 113
    Width = 146
    Height = 17
    Caption = 'Autmatic size scaling '
    Checked = True
    State = cbChecked
    TabOrder = 32
  end
  object OK_Button: TButton
    Left = 884
    Top = 708
    Width = 108
    Height = 64
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'CLOSE'
    TabOrder = 0
    OnClick = OK_ButtonClick
  end
  object DPSize_Scaling_Factor_Edit: TEdit
    Left = 126
    Top = 132
    Width = 42
    Height = 21
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    NumbersOnly = True
    TabOrder = 33
    Text = '50'
  end
  object DP_Size_ComboBox: TComboBox
    Left = 126
    Top = 70
    Width = 43
    Height = 21
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    AutoDropDown = True
    Style = csDropDownList
    ItemIndex = 7
    TabOrder = 34
    Text = '8'
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9'
      '10'
      '11'
      '12'
      '13'
      '14'
      '15')
  end
  object AblPointSizeComboBox: TComboBox
    Left = 126
    Top = 210
    Width = 43
    Height = 21
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    AutoDropDown = True
    Style = csDropDownList
    ItemIndex = 7
    TabOrder = 35
    Text = '8'
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9'
      '10'
      '11'
      '12'
      '13'
      '14'
      '15')
  end
  object Cancel_Button: TButton
    Left = 771
    Top = 708
    Width = 109
    Height = 64
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Cancel'
    TabOrder = 36
    OnClick = Cancel_ButtonClick
  end
  object Display_Paths_CheckBox: TCheckBox
    Left = 192
    Top = 273
    Width = 97
    Height = 17
    Caption = 'Display paths'
    TabOrder = 37
  end
  object Background_Color_RadioGroup: TRadioGroup
    Left = 190
    Top = 369
    Width = 138
    Height = 59
    Caption = 'Background color'
    ItemIndex = 0
    Items.Strings = (
      'Black'
      'White')
    TabOrder = 38
  end
  object Display_Edges_CheckBox: TCheckBox
    Left = 192
    Top = 171
    Width = 161
    Height = 17
    Caption = 'Display mesh geometry edges'
    Checked = True
    State = cbChecked
    TabOrder = 39
  end
  object Display_3D_Point_Cloud_CheckBox: TCheckBox
    Left = 192
    Top = 347
    Width = 177
    Height = 16
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Display 3D point cloud'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 40
  end
  object Palette_Type_RadioGroup: TRadioGroup
    Left = 2
    Top = 314
    Width = 185
    Height = 52
    Caption = 'Palette'
    ItemIndex = 0
    Items.Strings = (
      'Thermal palette'
      'White->Red')
    TabOrder = 41
  end
  object Min09Max_Range_CheckBox: TCheckBox
    Left = 192
    Top = 440
    Width = 141
    Height = 16
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Min-0.9*Max range'
    TabOrder = 42
    Visible = False
  end
  object AT_Edit: TEdit
    Left = 119
    Top = 437
    Width = 56
    Height = 21
    TabOrder = 43
    Text = 'AT_Edit'
  end
  object PS_Edit: TEdit
    Left = 119
    Top = 462
    Width = 56
    Height = 21
    TabOrder = 44
    Text = 'Edit1'
  end
  object AS_Edit: TEdit
    Left = 119
    Top = 487
    Width = 56
    Height = 21
    TabOrder = 45
    Text = 'Edit1'
  end
  object DTS_Edit: TEdit
    Left = 302
    Top = 114
    Width = 56
    Height = 21
    TabOrder = 46
    Text = 'DTS_Edit'
  end
  object Display_Arrows_CheckBox: TCheckBox
    Left = 41
    Top = 411
    Width = 127
    Height = 26
    Caption = 'Display arrows'
    TabOrder = 47
  end
end
