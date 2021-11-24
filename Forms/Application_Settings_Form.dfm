object App_Settings_Form: TApp_Settings_Form
  Left = 0
  Top = 0
  Caption = 'Application settings'
  ClientHeight = 458
  ClientWidth = 733
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 177
    Height = 13
    Caption = 'Maximum number of goemetry nodes'
  end
  object Close_Button: TButton
    Left = 612
    Top = 400
    Width = 113
    Height = 50
    Caption = 'CLOSE'
    TabOrder = 0
    OnClick = Close_ButtonClick
  end
  object MaxGeoNodes_Number_Edit: TEdit
    Left = 191
    Top = 12
    Width = 82
    Height = 21
    NumbersOnly = True
    TabOrder = 1
    Text = '0'
  end
  object Cancel_Button: TButton
    Left = 493
    Top = 400
    Width = 113
    Height = 50
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = Close_ButtonClick
  end
end
