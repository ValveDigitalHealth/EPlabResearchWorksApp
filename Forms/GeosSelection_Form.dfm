object Geos_Selection_Form: TGeos_Selection_Form
  Left = 0
  Top = 0
  Caption = 'Geos_Selection_Form'
  ClientHeight = 307
  ClientWidth = 603
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 144
  TextHeight = 19
  object Label1: TLabel
    Left = 24
    Top = 24
    Width = 157
    Height = 19
    Caption = 'Project this geometry:'
  end
  object Label2: TLabel
    Left = 24
    Top = 104
    Width = 126
    Height = 19
    Caption = 'on this geometry:'
  end
  object Geometry_Selection_ComboBox1: TComboBox
    Left = 45
    Top = 61
    Width = 372
    Height = 27
    Style = csDropDownList
    TabOrder = 0
  end
  object Geometry_Selection_ComboBox2: TComboBox
    Left = 45
    Top = 129
    Width = 372
    Height = 27
    Style = csDropDownList
    TabOrder = 1
  end
  object Cancel_Button: TButton
    Left = 296
    Top = 192
    Width = 105
    Height = 49
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = Cancel_ButtonClick
  end
  object Proceed_Button: TButton
    Left = 407
    Top = 192
    Width = 105
    Height = 49
    Caption = 'Proceed'
    TabOrder = 3
    OnClick = Proceed_ButtonClick
  end
end
